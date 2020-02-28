#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

uv_stream_t* stream;

void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
}

void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  if (nread == UV_EOF) {
    printf("read end.\n");
    return;
  } else if (nread < 0) {
    fprintf(stderr, "read err:%zd\n", nread);
  }
  printf("read:%s\n", buf->base);
}

void cb(uv_write_t* req, int status) {
  printf("req, status:%d\n", status);
  uv_read_start(stream, alloc_cb, read_cb);
  // uv_close(req);
}

void on_connect(uv_connect_t* req, int status) {
  printf("status:%d\n", status);
  if (status < 0) return;
  uv_write_t req1;
  char* str = "Hello libuv.\n";
  uv_buf_t a[] = {
    { .base = str, .len = strlen(str) },
  };
  stream = req->handle;
  // uv_write(&req1, req->handle, a, 2, cb);
  uv_write(&req1, stream, a, 1, cb);
}

int main(int argc, char** argv) {
  uv_tcp_t* socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(uv_default_loop(), socket);

  uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));

  struct sockaddr_in dest;
  uv_ip4_addr("127.0.0.1", 1234, &dest);

  uv_tcp_connect(connect, socket, &dest, on_connect);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
