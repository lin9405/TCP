all : client server

client :
	gcc -o client hw2-1_tcp_client.c -g

server :
	gcc -o server hw2-1_tcp_server.c -g

clean :
	rm -rf client server

runc33 :
	./client 10.1.0.1 214337

runc34 :
	./client 10.1.0.2 214337

runs :
	./server 214337
