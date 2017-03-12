all: client1 server binder

client1 : client1.c Socket.cpp rpc.cpp utils.cpp utils.h
	g++ -L. -w server_functions.c server_function_skels.c utils.cpp Socket.cpp rpc.cpp client1.c -stdlib=libc++ -std=c++0x -lpthread -o client1

server : server.c Socket.cpp rpc.cpp server_functions.c server_function_skels.c utils.cpp utils.h
	g++ -L. -w utils.cpp Socket.cpp rpc.cpp server.c server_functions.c server_function_skels.c -stdlib=libc++ -std=c++0x -lpthread -o server

binder : binder.cpp Socket.cpp utils.cpp utils.h
	g++ server_functions.c server_function_skels.c utils.cpp Socket.cpp binder.cpp -w -stdlib=libc++ -std=c++0x -lpthread -o binder
