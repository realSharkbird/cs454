all: client1 server binder

client1 : client1.c Socket.cpp rpc.cpp
	g++ -L. -w message.cpp Socket.cpp rpc.cpp client1.c -stdlib=libc++ -std=c++0x -lpthread -o client

server : server.c Socket.cpp rpc.cpp server_functions.c server_function_skels.c
	g++ -L. -w message.cpp Socket.cpp rpc.cpp server.c server_functions.c server_function_skels.c -stdlib=libc++ -std=c++0x -lpthread -o server

binder : binder.cpp Socket.cpp
	g++ message.cpp Socket.cpp binder.cpp -w -stdlib=libc++ -std=c++0x -lpthread -o binder
