To compile and run:

- run make
- Compile your client
	-eg. g++ -L. client.o -lrpc -o client
- Compile your server
	-eg. g++  -L. serverfunctions.o serverfunctionskels.o server.o -lrpc -o server
- Run the binder
	-eg ./binder
- Set the environment variables for binder port # and address on both respective endpoints
	- eg. export BINDER_PORT={the port you see}
      	      export BINDER_ADDRESS={the address you see}
- Run server
- Run client
.
.
.
- Profit

Other Dependencies: Socket.h, Socket.cpp and Utils.h, Utils.cpp and binder.h, binder.cpp
Group Member ids and names: 
20526389 - Jayson Mark
20430011 - Alex Vassilenko
