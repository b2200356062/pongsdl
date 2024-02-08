#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Surface* surface = nullptr;
SDL_Surface* surface2 = nullptr;
SDL_Surface* surface3 = nullptr;
SDL_Texture* texture1 = nullptr;
SDL_Texture* texture2 = nullptr;
SDL_Texture* texture3 = nullptr;

void clear(){
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    texture1 = nullptr;
    texture2 = nullptr;
    texture3 = nullptr;
    surface = nullptr;
    surface2 = nullptr;
    surface3 = nullptr;
    renderer = nullptr;
    window = nullptr;

    IMG_Quit();
    SDL_Quit();
}

bool checkCollision(SDL_Rect bar, SDL_FRect ball){

    return false;
}

int main(int argc, char** argv){

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL INIT FAILED: %s", SDL_GetError());
        return -1;
    }
    if(IMG_Init(IMG_INIT_PNG) != 2){
        printf("IMG INIT FAILED: %s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(window == nullptr){
        printf("window could not be created: %s", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr){
        printf("could not create renderer: %s" ,SDL_GetError());
        return -1;
    }
    SDL_Rect playerrectangle;
    playerrectangle.x = 50;
    playerrectangle.y = 100;
    playerrectangle.w = 60;
    playerrectangle.h = 100;

    surface = IMG_Load("textures/pong1.png");
    if(surface == nullptr){
        printf("cannot load surface :%s", SDL_GetError());
        return -1;
    }
    texture1 = SDL_CreateTextureFromSurface(renderer, surface);
    if(texture1 == nullptr){
        printf("cannot load texture : %s", SDL_GetError());
        return -1;
    }
   SDL_Rect airectangle;
    airectangle.x = 700;
    airectangle.y = 100;
    airectangle.w = 60;
    airectangle.h = 100;

    surface2 = IMG_Load("textures/pong1.png");
    if(surface2 == nullptr){
        printf("cannot load surface 2 :%s", SDL_GetError());
        return -1;
    }
    texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    if(texture2 == nullptr){
        printf("cannot load texture 2 : %s", SDL_GetError());
        return -1;
    }

    SDL_FRect ballrect;
    ballrect.x = 300;
    ballrect.y = 300;
    ballrect.w = 100;
    ballrect.h = 100;

    surface3 = IMG_Load("textures/pong2.png");
    if(surface3 == nullptr){
        printf("cannot load surface 3 :%s", SDL_GetError());
        return -1;
    }
    texture3 = SDL_CreateTextureFromSurface(renderer, surface3);
    if(texture3 == nullptr){
        printf("cannot load texture 3: %s", SDL_GetError());
        return -1;
    }

    int startside = rand() % 2;
    printf("start side: %d", startside);

    bool gameRunning = true;
    SDL_Event e;
    while(gameRunning){
        if(startside == 0){
            ballrect.x -= 0.05;
            ballrect.y -= 0.02;
        }
        else{
            ballrect.x += 0.05;
            ballrect.x += 0.02;
        }
        while( SDL_PollEvent( &e ) ){ 
            if( e.type == SDL_QUIT ){
                gameRunning = false;
            }
            switch(e.key.keysym.sym){
                case SDLK_w:
                    if(playerrectangle.y < 0 || checkCollision(playerrectangle, ballrect)){
                        break;
                    }
                    playerrectangle.y -= 8;
                    break;
                case SDLK_s:
                    if(playerrectangle.y > 480){
                        break;
                    }
                    playerrectangle.y += 8;
                    break;
                case SDLK_ESCAPE:
                gameRunning = false;
                break;
            }
        }     
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderCopy(renderer, texture1, nullptr, &playerrectangle);
        SDL_RenderCopy(renderer, texture2, nullptr, &airectangle);
        SDL_RenderCopyF(renderer, texture3, nullptr, &ballrect);

        SDL_RenderPresent(renderer);
    }

    clear();
    return 0;
}