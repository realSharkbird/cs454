all: rpc librpc.a binder client server

rpc : rpc.cpp
	g++ -c rpc.cpp utils.cpp Socket.cpp -std=c++0x -stdlib=libc++

librpc.a : rpc.o utils.o Socket.o
	ar rcs librpc.a rpc.o utils.o Socket.o

binder : binder.cpp Socket.cpp utils.cpp utils.h
	g++ server_functions.c server_function_skels.c utils.cpp Socket.cpp binder.cpp -w -std=c++0x -stdlib=libc++ -std=c++11  -lpthread -o binder

client : client1.c
	g++ -L. -w -lrpc client1.c -stdlib=libc++ -std=c++0x -lpthread -o client1

server : server.c
	g++ -L. server_functions.c server_function_skels.c server.c -lrpc -stdlib=libc++ -std=c++0x -lpthread -o server
