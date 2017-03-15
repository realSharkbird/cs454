all: rpc utils Socket skels funcs librpc.a binder

rpc : rpc.cpp
	g++ -c rpc.cpp -std=c++0x

utils : utils.cpp
	g++ -c utils.cpp -std=c++0x

Socket : Socket.cpp
	g++ -c Socket.cpp -std=c++0x

skels : server_function_skels.c
	g++ -c server_function_skels.c

funcs : server_functions.c
	g++ -c server_functions.c

librpc.a : rpc.o utils.o Socket.o server_function_skels.o server_functions.o
	ar rcs librpc.a rpc.o utils.o Socket.o server_function_skels.o server_functions.o

binder : binder.cpp Socket.cpp utils.cpp utils.h
	g++ server_functions.c server_function_skels.c utils.cpp Socket.cpp binder.cpp -w -std=c++0x -lpthread -o binder
