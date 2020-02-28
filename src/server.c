#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int DEFAULT_BACKLOG = 1;
//char alloc_buffer[1024];
//char buffer[1024*1024];
//int buf_offset;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = malloc(suggested_size); 
  buf->len = suggested_size;
}

void echo_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  if (nread == UV_EOF) {
    //buffer[buf_offset++] = 0;
    printf("read end:.\n");
    return;
  }
  if (nread < 0) {
    fprintf(stderr, "Read error:%s, nread:%zd\n", uv_err_name(nread), nread);
    return;
  }
  // memcpy(buffer + buf_offset, buf->base, nread);
  // buf_offset += nread;
  printf("Read len:%zd, str:\n%s", nread, buf->base);

  // uv_read_start(stream, alloc_buffer, echo_read);
  // print_buf_hex(buf->base, nread); 
}

void on_new_connection(uv_stream_t *server, int status) {
  if (status < 0) {
    fprintf(stderr, "New connection error %s\n", uv_strerror(status));
    return;
  }

  uv_tcp_t *client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(uv_default_loop(), client);
  if (uv_accept(server, (uv_stream_t*)client) == 0) {
    // buf_offset = 0;
    uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
  } else {
    uv_close((uv_handle_t*)client, NULL);
  }
}

int main(int argc, char** argv) {
  uv_loop_t* loop = uv_default_loop();  
  
  uv_tcp_t server;
  uv_tcp_init(loop, &server);

  struct sockaddr_in addr;
  uv_ip4_addr("0.0.0.0", 1234, &addr);

  uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
  int r = uv_listen((uv_stream_t*)&server, DEFAULT_BACKLOG, on_new_connection);
  if (r) {
    fprintf(stderr, "Lisent error %s\n", uv_strerror(r));
    return 1;
  }
  return uv_run(loop, UV_RUN_DEFAULT);
}
