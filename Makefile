client_server: client server

client: tcp.o
	gcc -o client client.c tcp.o

server: tcp.o
	gcc -o server server.c tcp.o

tcp.o:
	gcc -c tcp.c

clean:
	rm -f tcp.o client server