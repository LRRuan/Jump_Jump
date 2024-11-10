//
// Created by ruan on 21/1/2024.
//

#ifndef UNTITLED_SDLCOMMON_H
#define UNTITLED_SDLCOMMON_H
#include"variable.h"

int Random(int lower, int upper);

void drawCube(const Platform *p);

void drawCylinder(const Platform *p);

void draw(const Platform *p);

void renderText(TTF_Font* font1, const char* text, SDL_Color textColor, int x, int y);

void renderButton(const Button* button, SDL_Point mousePos);

int initializeSDL();

TTF_Font* loadFont(const char* fontPath, int fontSize);

void cleanupSDL();

void cleanupMainPage();
void cleanupIntroductionPage();
void cleanupGamePage();

void cleanupAndQuit(Player*player,Platform*p[3]);

#endif //UNTITLED_SDLCOMMON_H
