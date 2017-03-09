all: client1 server binder

client1 : client1.c
	g++ -L. -w Socket.cpp rpc.cpp client1.c -stdlib=libc++ -std=c++0x -lpthread -o client

server : server.c
	g++ -L. -w Socket.cpp rpc.cpp server.c server_functions.c server_function_skels.c -stdlib=libc++ -std=c++0x -lpthread -o server

binder : binder.c
	g++ Socket.cpp binder.cpp -w -stdlib=libc++ -std=c++0x -lpthread -o binder
