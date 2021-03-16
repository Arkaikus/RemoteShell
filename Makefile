all: clean init_dirs build_client build_server

build_client: build_tcp
	gcc client.c ./obj/*.o -o ./dist/client

build_server: build_tcp
	gcc server.c ./obj/*.o -o ./dist/server

build_tcp:
	gcc -c tcp.c -o ./obj/tcp.o

init_dirs:
	mkdir dist obj

clean:
	rm -rf dist/ obj/
