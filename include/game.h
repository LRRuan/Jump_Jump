//
// Created by ruan on 21/1/2024.
//

#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H
#include"variable.h"

int judge(const Player *player,Platform*p[3]);

void randomPlatform(Platform*p,int score,const Platform *prev);

void move(Platform*p[3],Player*player);

void initGame(Player* player,Platform *p[3]);

void handleGameStart(Player*player,Platform*p[3]);

#endif //UNTITLED_GAME_H
