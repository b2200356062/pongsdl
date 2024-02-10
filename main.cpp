#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 144
#define FRAME_TARGET_TIME (1000 / FPS)

using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Surface* surface = nullptr;
SDL_Surface* surface2 = nullptr;
SDL_Surface* surface3 = nullptr;
SDL_Texture* texture1 = nullptr;
SDL_Texture* texture2 = nullptr;
SDL_Texture* texture3 = nullptr;

int scoreleft = 0, scoreright = 0; bool restart = false;
int last_frame = 0;
int startside = rand() % 2;

bool gameRunning = true;
SDL_Event e;

typedef struct{
    SDL_Rect model;
    SDL_Rect collisionbox;
    SDL_Surface *surf = nullptr;
    SDL_Texture *texture = nullptr;
    int width;
    int height;

}bar;

typedef struct{
    SDL_FRect model;
    SDL_Rect collisionbox;
    SDL_Surface *surf = nullptr;
    SDL_Texture *texture = nullptr;
    int width;
    int height;
    float velocity;
}balle;

bar p;
bar ai;
balle ball;

int init(){

    p.model.w = 50;
    p.model.h = 60;
    p.model.x = 50;
    p.model.y = 300;

    p.surf = IMG_Load("textures/pong1.png");
    if(p.surf == nullptr){
        printf("cannot load surface :%s", SDL_GetError());
        return -1;
    }

    p.texture = SDL_CreateTextureFromSurface(renderer, p.surf);
    if(p.texture == nullptr){
        printf("cannot load texture : %s", SDL_GetError());
        return -1;
    }
  
    ai.model.x = 700;
    ai.model.y = 300;
    ai.model.w = 50;
    ai.model.h = 60;

    ai.surf = IMG_Load("textures/pong1.png");
    if(ai.surf == nullptr){
        printf("cannot load surface 2 :%s", SDL_GetError());
        return -1;
    }
    ai.texture = SDL_CreateTextureFromSurface(renderer, ai.surf);
    if(ai.texture == nullptr){
        printf("cannot load texture 2 : %s", SDL_GetError());
        return -1;
    }

    ball.model.x = 300;
    ball.model.y = 300;
    ball.model.w = 60;
    ball.model.h = 60;

    ball.surf = IMG_Load("textures/pong2.png");
    if(ball.surf == nullptr){
        printf("cannot load surface 3 :%s", SDL_GetError());
        return -1;
    }
    ball.texture = SDL_CreateTextureFromSurface(renderer, ball.surf);
    if(ball.texture == nullptr){
        printf("cannot load texture 3: %s", SDL_GetError());
        return -1;
    }

    return 0;
}

bool checkCollision(SDL_Rect *bar, SDL_FRect *ball){
    return false;
}

void scoreupdate(int side){
    // score up based on int side 0 left 1 right
    if(side == 0){
        scoreright += 1;
    }
    else if(side == 1){
        scoreleft += 1;
    }
}

void update(bar *p, bar *ai, balle *ball){
    while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame + FRAME_TARGET_TIME));
    
    float deltatime = (SDL_GetTicks() - last_frame) / 1000.0f;

    last_frame = SDL_GetTicks();

     while( SDL_PollEvent( &e ) ){ 
            if( e.type == SDL_QUIT ){
                gameRunning = false;
            }
            switch(e.key.keysym.sym){
                case SDLK_w:
                    if(p->model.y < 0){
                        break;
                    }
                    p->model.y -= 1000 * deltatime;
                    break;
                case SDLK_s:
                    if(p->model.y > 540){
                        break;
                    }
                    p->model.y += 1000* deltatime;
                    break;
                case SDLK_ESCAPE:
                gameRunning = false;
                break;
            }
    }    

    if(startside == 0){
        ball->model.x -= 100 * deltatime;
        //ballrect->y -= 100 * deltatime;
    }
    else{
        ball->model.x += 100 * deltatime;
        //ballrect->y += 100 * deltatime;
    }

    if(ball->model.x < 0){
        scoreupdate(0);
        restart = true;
    }
    else if(ball->model.x > 780){
        scoreupdate(1);
        restart = true;
    }

    // checkCollision(playerrectangle, ballrect);
    
    // checkCollision(airectangle, ballrect);

}

void rendermap(){
    int midpx, midpy, midpyend;
    midpx = SCREEN_WIDTH / 2;
    midpy = 5;
    midpyend = 15;
    for (int i = 0; i < SCREEN_HEIGHT / 20; i++)
    {
        SDL_RenderDrawLine(renderer, midpx, midpy, midpx, midpyend);
        midpy += 20;
        midpyend += 20;
    }

}

void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    rendermap();

    SDL_RenderCopy(renderer, p.texture, nullptr, &p.model);
    SDL_RenderCopy(renderer, ai.texture, nullptr, &ai.model);
    SDL_RenderCopyF(renderer, ball.texture, nullptr, &ball.model);
    SDL_RenderPresent(renderer);
}

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
    
    if(init() != 0){
        printf("init went wrong! returning");
        return -1;
    }

    while(gameRunning){    
        update(&p, &ai, &ball);
        render();
    }

    clear();
    return 0;
}