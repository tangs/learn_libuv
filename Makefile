BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

CPPFLAGS = -g -o3 -std=c++17 -I include
CFLAGS = -g -o3 -I include

vpath %.o $(OBJ_DIR)
vpath %.cc src
vpath %.out out 
vpath %.c src
vpath %.h include

all: server.out client.out Makefile

mkdir:
	@mkdir -p build/obj out

clean:
	@rm -rf $(OBJ_DIR)/*.o
	@rm -rf out/server

server.out: server.o mkdir
	cc $(CFLAGS) -o out/server.out $(OBJ_DIR)/server.o -luv

client.out: client.o mkdir
	cc $(CFLAGS) -o out/client.out $(OBJ_DIR)/client.o -luv

server.o: server.c utils.h 
	cc $(CFLAGS) -c -o $(OBJ_DIR)/server.o src/server.c

client.o: client.c utils.h 
	cc $(CFLAGS) -c -o $(OBJ_DIR)/client.o src/client.c

