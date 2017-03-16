all: rpc utils Socket librpc.a binder

rpc : rpc.cpp
	g++ -c rpc.cpp -std=c++0x

utils : utils.cpp
	g++ -c utils.cpp -std=c++0x

Socket : Socket.cpp
	g++ -c Socket.cpp -std=c++0x -stdlib=libc++

librpc.a : rpc.o utils.o Socket.o
	ar rcs librpc.a rpc.o utils.o Socket.o

binder : binder.cpp Socket.cpp utils.cpp utils.h
	g++ server_functions.c server_function_skels.c utils.cpp Socket.cpp binder.cpp -w -std=c++0x -stdlib=libc++ -std=c++11  -lpthread -o binder
