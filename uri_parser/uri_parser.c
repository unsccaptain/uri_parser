#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <np_base.h>
#include <np_uri.h>

#include "uri_internal.h"

void uri__append_path_segment(char *temp, unsigned len, uri *uri) {
  if (len == 0)
    return;

  char *seg_name = base_dup_fixed_str(temp, len);
  uri_path_seg *seg = (uri_path_seg *)malloc(sizeof(uri_path_seg));
  assert(seg != null);

  seg->segment = seg_name;
  seg->next = null;

  if (uri->head_path == null) {
    uri->head_path = uri->tail_path = seg;
  } else {
    uri->tail_path->next = seg;
    uri->tail_path = seg;
  }
}

#define TRANSITION(state)                                                      \
  {                                                                            \
    uri->fsm = (state);                                                        \
    len = 0;                                                                   \
  }

void uri_parser_init(uri *uri) {
  memset(uri, 0, sizeof(*uri));
  uri->fsm = fsm_init;
}

int uri_parse(input_stream *s, uri *uri) {
  char temp[256];
  unsigned len = 0;
  char c;
  int valid_result;
  while (1) {
    if (stream_consume_char(s, &c) != np_status_ok)
      goto complete;

    switch (c) {
    case ':':
      if (uri->fsm == fsm_init) {
        if (stream_look_forward(s) == '/') {
          uri->scheme = base_dup_fixed_str(temp, len);
          TRANSITION(fsm_scheme);
        } else {
          uri->host = base_dup_fixed_str(temp, len);
          TRANSITION(fsm_begin_port);
        }
      } else if (uri->fsm == fsm_begin_host) {
        uri->host = base_dup_fixed_str(temp, len);
        TRANSITION(fsm_begin_port);
      }
      break;
    case '/':
      if (uri->fsm == fsm_scheme) {
        uri->fsm =
            stream_look_forward(s) == '/' ? fsm_first_slash : fsm_begin_segment;
      } else if (uri->fsm == fsm_first_slash) {
        TRANSITION(fsm_begin_host);
      } else if (uri->fsm == fsm_init || uri->fsm == fsm_begin_host) {
        uri->host = base_dup_fixed_str(temp, len);
        TRANSITION(fsm_begin_segment);
      } else if (uri->fsm == fsm_begin_port) {
        uri->port = base_dup_fixed_str(temp, len);
        TRANSITION(fsm_begin_segment);
      } else if (uri->fsm == fsm_begin_segment) {
        uri__append_path_segment(temp, len, uri);
        TRANSITION(fsm_begin_segment);
      }
      break;
    case '?':
      if (uri->fsm == fsm_begin_host) {
        uri->host = base_dup_fixed_str(temp, len);
      } else if (uri->fsm == fsm_begin_port) {
        uri->port = base_dup_fixed_str(temp, len);
      } else if (uri->fsm == fsm_scheme) {
        uri__append_path_segment(temp, len, uri);
      } else if (uri->fsm == fsm_begin_segment) {
        uri__append_path_segment(temp, len, uri);
      }
      TRANSITION(fsm_begin_query);
      break;
    case '#':
      if (uri->fsm == fsm_begin_query) {
        uri->query = base_dup_fixed_str(temp, len);
      } else if (uri->fsm == fsm_begin_segment) {
        uri__append_path_segment(temp, len, uri);
      } else if (uri->fsm == fsm_begin_host) {
        uri->host = base_dup_fixed_str(temp, len);
      } else if (uri->fsm == fsm_begin_port) {
        uri->port = base_dup_fixed_str(temp, len);
      }
      TRANSITION(fsm_begin_frag);
      break;
    case ' ':
      stream_backward(s, 1);
      goto complete;
    default:
      temp[len++] = c;
      break;
    }
  }
complete:
  switch (uri->fsm) {
  case fsm_begin_host:
    uri->host = base_dup_fixed_str(temp, len);
    break;
  case fsm_begin_port:
    uri->port = base_dup_fixed_str(temp, len);
    break;
  case fsm_begin_query:
    uri->query = base_dup_fixed_str(temp, len);
    break;
  case fsm_begin_frag:
    uri->frgment = base_dup_fixed_str(temp, len);
    break;
  case fsm_begin_segment:
    uri__append_path_segment(temp, len, uri);
    break;
  case fsm_init:
    uri->host = base_dup_fixed_str(temp, len);
    break;
  case fsm_scheme:
    uri__append_path_segment(temp, len, uri);
    break;
  default:
    break;
  }
  TRANSITION(fsm_finish);

  if ((valid_result = uri__valid_scheme(uri)) != URI_OK)
    return valid_result;

  if ((valid_result = uri__valid_port(uri)) != URI_OK)
    return valid_result;

  return URI_OK;
}