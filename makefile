all: client1 server binder

client1 : client1.c Socket.c rpc.c
	g++ -L. -w Socket.c rpc.c client1.c -stdlib=libc++ -std=c++0x -lpthread -o client

server : server.c Socket.c rpc.c server_functions.c server_function_skels.c
	g++ -L. -w Socket.c rpc.c server.c server_functions.c server_function_skels.c -stdlib=libc++ -std=c++0x -lpthread -o server

binder : binder.c Socket.c
	g++ Socket.c binder.c -w -stdlib=libc++ -std=c++0x -lpthread -o binder
