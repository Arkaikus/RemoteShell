all: init_dirs build_client build_server

build_client: build_tcp
	gcc ./src/client.c ./obj/*.o -o ./dist/client

build_server: build_tcp
	gcc ./src/server.c ./obj/*.o -lpthread -o ./dist/server

build_tcp:
	gcc -c ./src/tcp.c -o ./obj/tcp.o

init_dirs:
	mkdir dist obj -p

clean:
	rm -rf ./dist/ ./obj/
