all: client1 server binder

client1 : client1.c
	g++ -L. rpc.c client1.c -stdlib=libc++ -std=c++0x -lpthread -o client

server : server.c
	g++ -L. rpc.c server.c server_functions.c server_function_skels.c -stdlib=libc++ -std=c++0x -lpthread -o server

binder : binder.c
	g++ binder.c -std=c++0x -lpthread -o binder
