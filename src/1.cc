#include <uv.h>
#include <stdio.h>

char buffer[1024];
uv_fs_t open_req, read_req, write_req;
uv_buf_t iov;
int idx = -1;

void on_read(uv_fs_t *req) {
  printf("read:%zd\n", req->result);
  int res = req->result;
  if (res < 0) {
    fprintf(stderr, "error read file:%s\n", uv_strerror(res));
  } else if (res == 0) {
    printf("EOF\n");
    uv_fs_t close_req;
    uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
  } else {
    // printf("len:%zd\n", iov.len);
    printf("len:%zd\n", iov.len);
    auto buf = iov.base;
    for (int i = 0; i < (res + 15) / 16; ++i) {
      for (int j = 0; j < 16; ++j) {
        printf("%02x ", buf[i * 16 +j]);
      }
      printf("\t\t");
      for (int j = 0; j < 16; ++j) {
        char ch = buf[i * 16 +j];
        printf("%c", ch == '\n' ? ' ' : ch);
      }
      printf("\n");
    }
    idx += iov.len;
    uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, idx, on_read);
  }
}

void on_open(uv_fs_t *req) {
  printf("open:%zd\n", req->result); 
  if (req->result >= 0) {
    printf("open1\n");
    iov = uv_buf_init(buffer, sizeof(buffer));
    idx = 0;
    uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, idx, on_read);  
    printf("open2\n");
  } else {
    fprintf(stderr, "error open file:%s\n", uv_strerror((int)req->result));
  }
}

int main(int argc, char **argv) {
  auto loop = uv_default_loop();
  // printf("%p\n", &open_req); 

  printf("1\n");
  uv_fs_open(loop, &open_req, argv[1], O_RDONLY, 0, on_open);
  printf("2\n");
  uv_run(loop, UV_RUN_DEFAULT);
  printf("3\n");

  uv_fs_req_cleanup(&open_req);
}
