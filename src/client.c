#include <uv.h>
#include <stdio.h>
#include <stdlib.h>

void cb(uv_write_t* req, int status) {
  printf("req, status:%d\n", status);
  // uv_close(req);
}

void on_connect(uv_connect_t* req, int status) {
  printf("status:%d\n", status);
  if (status < 0) return;
  uv_write_t req1;
  uv_buf_t a[] = {
    { .base = "1", .len = 1 },
    { .base = "2", .len = 1 }
  };
  uv_write(&req1, req->handle, a, 2, cb);
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
