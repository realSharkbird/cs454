all: rpc librpc.a binder

rpc : rpc.cpp
	g++ -c -w rpc.cpp utils.cpp Socket.cpp -std=c++0x -std=c++11

librpc.a : rpc.o utils.o Socket.o
	ar rcs librpc.a rpc.o utils.o Socket.o

binder : binder.cpp Socket.cpp utils.cpp utils.h
	g++ server_functions.c server_function_skels.c utils.cpp Socket.cpp binder.cpp -w -std=c++0x -std=c++11  -lpthread -o binder
