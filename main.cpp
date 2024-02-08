#include <iostream>
#include "SDL2/SDL.h"
using namespace std;

int main(int argc, char** argv){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow("pencere", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    return 0;
}