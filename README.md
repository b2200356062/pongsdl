simple pong game with AI using sdl2 and c++. 
compile and run with "make" in the project directory.

for networking, first:
    g++ src/server.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_net -o server
    ./server
    then run make and ./pong <ip from other server> 2000