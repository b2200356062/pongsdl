#include <iostream>
#include <cmath>
#include <time.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define FPS 144
#define FRAME_TARGET_TIME (1000 / FPS)

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Surface* surface = nullptr;
SDL_Surface* surface2 = nullptr;
SDL_Surface* surface3 = nullptr;
SDL_Surface* surface4 = nullptr;
SDL_Texture* texture = nullptr;
SDL_Texture* texture2 = nullptr;
SDL_Texture* texture3 = nullptr;
SDL_Texture* texture4 = nullptr;

SDL_Rect scoreRect; SDL_Rect lScore; SDL_Rect rScore; SDL_Rect end;

int scoreleft = 0, scoreright = 0; bool restart = false;
int last_frame = 0;

bool gameRunning = true;
SDL_Event e;

typedef struct{
    SDL_FRect model;
    int width;
    int height;
    float vel;
}bar;

typedef struct{
    SDL_FRect model;
    SDL_Texture* texture = nullptr;
    SDL_Surface* surf = nullptr;
    SDL_Rect clip;
    int width;
    int height;
    float vel_x;
    float vel_y;
}balle;

bar p;
bar ai;
balle ball;

int init(int& startSide, int& verticalSide){

    p.model.w = 10;
    p.model.h = 60;
    p.model.x = 50;
    p.model.y = 300;
    p.vel = 1000;

    ai.model.w = 10;
    ai.model.h = 60;
    ai.model.x = 700;
    ai.model.y = 300;
    ai.vel = 150;

    ball.model.x = SCREEN_WIDTH / 2 - 30;
    ball.model.y = SCREEN_HEIGHT / 2;
    ball.model.w = 16;
    ball.model.h = 16;

    ball.clip.x = 32; // start pos
    ball.clip.y = 16; 
    ball.clip.w = 16; // actual size
    ball.clip.h = 16;

    if(startSide == 0){
    ball.vel_x = -(rand() % 30 + 100);
    }
    else{
        ball.vel_x = (rand() % 30 + 100);
    }
    if(verticalSide == 0){
        ball.vel_y = -(rand() % 30 + 100);
    }
    else{
        ball.vel_y = (rand() % 30 + 100);
    }

    ball.surf = IMG_Load("assets/pong2.png");
    if(ball.surf == nullptr){
        printf("cannot load surface 3 :%s", SDL_GetError());
        return -1;
    }
    ball.texture = SDL_CreateTextureFromSurface(renderer, ball.surf);
    if(ball.texture == nullptr){
        printf("cannot load texture 3: %s", SDL_GetError());
        return -1;
    }

    scoreRect.x = SCREEN_WIDTH / 2 - 50;
    scoreRect.y = 5;
    scoreRect.w = 100;
    scoreRect.h = 50;

    lScore.x = 100;
    lScore.y = 5;
    lScore.w = 30;
    lScore.h = 50;

    rScore.x = 650;
    rScore.y = 5;
    rScore.w = 30;
    rScore.h = 50;

    end.x = 10;
    end.y = 200;
    end.w = 780;
    end.h = 100;
    
    return 0;
}

bool checkCollision(SDL_FRect *bar, SDL_FRect *ball){
    if(SDL_HasIntersectionF(bar, ball) == SDL_TRUE){
        return true;
    }
    else{
        return false;
    }
}

void AI(bar* ai, balle* ball, float &deltatime){
    //printf("ball velocity x: %f, ball velocity y: %f\n", ball->vel_x, ball->vel_y);
    
    // y: negatif yukarı pozitif aşşa
    // x: negatif sol pozitif sağ

    printf("ball posx: %f, posy: %f\n", ball->model.x, ball->model.y);
    if(ball->model.y > ai->model.y){
        if(ai->model.y > 535){
             ai->model.y -= ai->vel * deltatime;
        }
        else{
            if(ball->model.x < SCREEN_WIDTH / 2){
                ai->model.y += ai->vel * deltatime / 1.5;
            }
            else{
                ai->model.y += ai->vel * deltatime;
            }
        }
    }

    if(ball->model.y < ai->model.y){
        if(ai->model.y < 5){
            ai->model.y += ai->vel * deltatime;
        }
        else{
            if(ball->model.x > SCREEN_WIDTH / 2){
                ai->model.y -= ai->vel * deltatime / 1.5;
            }
            else{
                ai->model.y -= ai->vel * deltatime;
            }
        }
        
    }
}

void update(bar *p, bar *ai, balle *ball, TTF_Font *font, SDL_Color color){
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
                    p->model.y -= p->vel * deltatime;
                    break;
                case SDLK_s:
                    if(p->model.y > 535){
                        break;
                    }
                    p->model.y += p->vel * deltatime;
                    break;
                case SDLK_ESCAPE:
                gameRunning = false;
                break;
            }
    }    

    // ball movement
    ball->model.x += ball->vel_x * deltatime;
    ball->model.y += ball->vel_y * deltatime;

    // score update 
    if(ball->model.x < 0){
        scoreright++;
        // c++ gaming
        std::string rscore = std::to_string(scoreright);
        char const *rchar = rscore.c_str();
       
        surface3 = TTF_RenderText_Solid(font, rchar, color);
        texture3 = SDL_CreateTextureFromSurface(renderer, surface3);

        restart = true;
    }
    else if(ball->model.x > 780){
        scoreleft++;
        
        std::string lscore = std::to_string(scoreleft);
        char const *lchar = lscore.c_str();

        surface2 = TTF_RenderText_Solid(font, lchar, color);
        texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
        restart = true;
    }

    // out of bounds
    if(ball->model.y > SCREEN_HEIGHT - 16){
        ball->vel_y = -ball->vel_y;
    }

    if(ball->model.y < 0){
        ball->vel_y = -ball->vel_y;
    }

    // collision check
    if(checkCollision(&p->model, &ball->model) == true){
        ball->vel_x = -ball->vel_x;
    }
    
    if(checkCollision(&ai->model, &ball->model) == true){
        ball->vel_x = -ball->vel_x;
    };

    // AI movement
    AI(ai, ball, deltatime);

}

// renders the lines in the middle
void renderMap(){
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

// main render function
void render(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    renderMap();
    
    SDL_RenderFillRectF(renderer, &p.model);
    SDL_RenderFillRectF(renderer, &ai.model);
    SDL_RenderCopyF(renderer, ball.texture, &ball.clip, &ball.model);

    // UI RENDER
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
    SDL_RenderCopy(renderer, texture2, nullptr, &lScore);
    SDL_RenderCopy(renderer, texture3, nullptr, &rScore);

    SDL_RenderPresent(renderer);
}

void clear(){

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(texture2);
    SDL_DestroyTexture(texture3);
    SDL_DestroyTexture(texture4);
    SDL_DestroyTexture(ball.texture);

    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
    SDL_FreeSurface(surface4);
    SDL_FreeSurface(ball.surf);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    texture = nullptr;
    texture2 = nullptr;
    texture3 = nullptr;
    texture4 = nullptr;
    ball.texture = nullptr;

    surface = nullptr;
    surface2 = nullptr;
    surface3 = nullptr;
    surface4 = nullptr;
    ball.surf = nullptr;

    renderer = nullptr;
    window = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


int main(int argc, char** argv){

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL INIT FAILED: %s", SDL_GetError());
        return 1;
    }
    if(IMG_Init(IMG_INIT_PNG) != 2){
        printf("IMG INIT FAILED: %s", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0){
        printf("TTF INIT FAILED: %s", SDL_GetError());
        return 1;
    }
    
    window = SDL_CreateWindow("pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(window == nullptr){
        printf("window could not be created: %s", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr){
        printf("could not create renderer: %s" ,SDL_GetError());
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("assets/font.ttf", 25);
    if(font == nullptr){
        printf("font is null: %s", SDL_GetError());
        return 1;
    }

    SDL_Color color = {255, 255, 0};
    
    surface = TTF_RenderText_Solid(font, "SCORE", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);

     std::string lscore = std::to_string(scoreleft);
    char const *lchar = lscore.c_str();
       
    surface2 = TTF_RenderText_Solid(font, lchar, color);
    texture2 = SDL_CreateTextureFromSurface(renderer, surface2);

    std::string rscore = std::to_string(scoreright);
    char const *rchar = rscore.c_str();

    surface3 = TTF_RenderText_Solid(font, rchar, color);
    texture3 = SDL_CreateTextureFromSurface(renderer, surface3);


    // randomize ball movement side
    srand(time(NULL));
    int startSide = rand() % 2;
    int verticalSide = rand() % 2;

    if(init(startSide, verticalSide) != 0){
        printf("init went wrong! returning");
        return 1;
    }

    while(gameRunning){    
        update(&p, &ai, &ball, font, color);
        render();
        if(restart){
            SDL_Delay(100);
            restart = false;
            startSide = rand() % 2;
            verticalSide = rand() % 2;
            if(startSide == 0){
                ball.vel_x = -(rand() % 30 + 100);
            }
            else{
                ball.vel_x = (rand() % 30 + 100);
            }
            if(verticalSide == 0){
                ball.vel_y = -(rand() % 30 + 100);
            }
            else{
                ball.vel_y = (rand() % 30 + 100);
            }

            ball.model.x = SCREEN_WIDTH / 2 - 30;
            ball.model.y = SCREEN_HEIGHT / 2;
            ai.model.x = 700;
            ai.model.y = 300;
            p.model.x = 50;
            p.model.y = 300;
        }

        if(scoreleft == 3){
            surface4 = TTF_RenderText_Solid(font, "GAME OVER, YOU WON!\nTHANKS FOR PLAYING!", color);
            texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture4, nullptr, &end);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            break;
        }
        else if(scoreright == 3){
            surface4 = TTF_RenderText_Solid(font, "GAME OVER, YOU LOST!\nTHANKS FOR PLAYING!", color);
            texture4 = SDL_CreateTextureFromSurface(renderer, surface4);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture4, nullptr, &end);
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);
            break;
        }
    }

    clear();
    return 0;
}