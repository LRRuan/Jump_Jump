#ifndef UNTITLED_VARIABLE_H
#define UNTITLED_VARIABLE_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_mixer.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<time.h>
#include<stdbool.h>

#undef main

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FONT_SIZE 30
#define TITLE_FONT_SIZE 60
#define MAX_POWER 40 // 设置蓄力上限


typedef enum shape{rect,cyl}Shape;

typedef struct {
    Shape shape;
    int x,y;
    int h,r;
    int length,width;
} Platform;

typedef struct {
    SDL_Rect rect;
    int x,y;
    double dx,dy;
    int power;
} Player;

;

typedef struct {
    SDL_Rect rect;
    const char* text;
    SDL_Color textColor;
    SDL_Color bgColor;
} Button;


enum Page {
    MainPage,
    IntroductionPage,
    GamePage,
};


extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern TTF_Font* font;
extern TTF_Font* titleFont;
extern SDL_Texture* introImageTexture;
extern SDL_Texture* mainImageTexture;
extern SDL_Texture* backgroundtexture;
extern SDL_Texture* ant;
extern SDL_Event e;

extern Button startButton;
extern Button introButton;
extern Button exitButton;

extern enum Page currentPage;

extern int maxScore; // 最高分
extern int currentScore; // 当前分数
extern int heartCount; // 爱心数量
extern int maxPoint; // 最高分
extern bool jumping; // 用于检测是否在跳跃状态
extern bool powering; // 用于检测是否在蓄力状态
extern double power; // 蓄力值
extern bool gameOver; // 游戏结束标志
extern bool moving;
extern int platseq;
extern bool togetnew;
extern bool cangetnew;
extern bool judging;
extern int bonus;
extern bool paused;

extern Button backButton;
extern SDL_mutex* pauseMutex;

extern const char* introductionText[];

#endif// UNTITLED_VARIABLE_H
