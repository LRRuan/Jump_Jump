//
// Created by ruan on 21/1/2024.
//
#include"variable.h"
#include"page.h"
#include"SDLcommon.h"


int judge(const Player *player,Platform*p[3]){
    for(int i=0;i<3;i++){
        if(p[i]->shape==cyl){
            if(player->x+25>=p[i]->x-p[i]->r&&player->x+25<=p[i]->x+p[i]->r){
                return i;
            }
        }else{
            if(player->x+25>=p[i]->x+50&&player->x+25<=p[i]->x+50+p[i]->width){
                return i;
            }
        }
    }
    return -1;
}

void randomPlatform(Platform*p,int score,const Platform *prev){
    if(Random(1,2)==1){
        p->shape=cyl;
        p->y=440;
        p->h=60;
        p->r=Random(60-score>30?60-score:30,100-2*score>60?100-score:60);
        p->x=Random(801+p->r,801+p->r+150);
        p->width=p->length=0;
    }else{
        p->shape=rect;
        p->y=500;
        p->length=150;
        p->h=100;
        p->width=Random(160-4*score>60?160-4*score:60,250-4*score>150?250-4*score:150);
        p->x=Random(801,801+150);
        p->r=0;
    }
}

void move(Platform*p[3],Player*player){
    if(player->x>150) {
        player->x -= 30;
        for (int i = 0; i < 3; i++) {
            p[i]->x -= 30;
        }
        if (player->x < 150) {
            int det = 150 - player->x;
            player->x += det;
            for (int i = 0; i < 3; i++) {
                p[i]->x += det;
            }
            moving=0;
        }
    }
}

void initGame(Player* player,Platform *p[3]) {
    bonus=4;
    platseq=0;
    currentScore = 0;
    heartCount = 1;
    power = 0;
    jumping = 0;
    moving=0;
    gameOver=0;
    paused=0;
    randomPlatform(p[2],0,p[1]);
    // 清除上一帧的按钮
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    p[0]->x=50;p[0]->y=500;p[0]->length=150;p[0]->h=100;p[0]->width=200;p[0]->shape=rect;p[0]->r=0;
    p[1]->shape=cyl;p[1]->h=50;p[1]->r=100;p[1]->x=600;p[1]->y=450;p[1]->width=p[1]->length=0;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    draw(p[0]);
    draw(p[1]);
    draw(p[2]);

    player->x=150;player->y=400;
    SDL_Rect redSquare = {player->x, player->y, 50, 50};
    if (ant == NULL) {
        SDL_Surface* ants = IMG_Load("./res/img/2.png");
        if (ants != NULL) {
            ant = SDL_CreateTextureFromSurface(renderer, ants);
            SDL_FreeSurface(ants);
        }
    }

    if (ant != NULL) {
        SDL_RenderCopy(renderer, ant, NULL, &player->rect);
    }
    player->rect=redSquare;

    SDL_Color textColor = {0, 0, 0, 255};
    char scoreText[20];
    char heartText[20];
    snprintf(scoreText, sizeof(scoreText), "score:%d", currentScore);
    snprintf(heartText, sizeof(heartText), "heart:%d", heartCount);

    renderText(font, scoreText, textColor, WINDOW_WIDTH - 120, 10);
    renderText(font, heartText, textColor, WINDOW_WIDTH - 120, 50);

    SDL_RenderPresent(renderer);
    SDL_Delay(32);
}

void handleGameStart(Player*player,Platform*p[3]) {
    currentPage = GamePage;
    int quit0 = 0;

    initGame(player,p);

    while (!quit0) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        while (SDL_PollEvent(&e) != 0&&!jumping) {
            SDL_LockMutex(pauseMutex);
            if (e.type == SDL_QUIT) {
                handleReturnToMainPage();
                quit0 = 1;
            }

            if (!paused) {
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        handleReturnToMainPage();
                        quit0=1;
                        return;
                    }

                    // R键重新开始
                    if (e.key.keysym.sym == SDLK_r) {
                        printf("Restarted");
                        initGame(player,p); // 重新开始时重新初始化游戏页面
                    }

                    // P键暂停
                    if (e.key.keysym.sym == SDLK_p) {
                        paused = 1;
                    }
                    if (e.key.keysym.sym == SDLK_h) {
                        heartCount++;
                    }
                    if(e.key.keysym.sym ==SDLK_c){
                        jumping=1;
                        int tmp=(platseq+1)%3;
                        int distance;
                        if(p[tmp]->shape==cyl){
                            distance=p[tmp]->x-player->x-50;
                        }else{
                            distance=p[tmp]->x+p[tmp]->width/2-player->x;
                        }
                        power=distance/16.0;
                    }
                }

                // 处理空格键，长按增加power
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE&&!jumping) {
                    if (power < MAX_POWER) {
                        power++;
                    }
                }

                if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_SPACE&&!jumping) {
                    // 处理空格键释放，执行跳跃操作
                    jumping = 1;
                }
            } else {
                // 处理暂停状态下的事件
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_p) {
                        printf("unpause");
                        paused = 0;
                    }
                }
            }
        }
        SDL_UnlockMutex(pauseMutex);

        if(currentScore==bonus){
            bonus*=2;
            heartCount++;
        }

        // 跳跃逻辑
        if (!paused && player->y<=400&&jumping) {
            // 根据power来计算跳跃高度
            if(power!=0) {
                player->dy = 2.5 * power;
                power=0;
            }
            player->dx=16;
            // 更新位置
            player->y-=player->dy;
            player->x+=player->dx;
            player->dy-=5;


            if(player->y>=400){
                player->y=400;
                player->dy=0;
                player->dx=0;
                jumping=0;
                judging=1;
            }
        }
        player->rect.x=player->x;player->rect.y=player->y;

        if(!jumping&&judging) {
            judging=0;
            if (judge(player, p) == -1) {
                heartCount--;
                if(heartCount>0) {
                    player->x = 150;
                }else{
                    gameOver=1;
                }
            }else{
                int tmp=judge(player,p);
                if((tmp==0&&platseq==2)||tmp-platseq==1){
                    currentScore++;
                    platseq=tmp;
                    cangetnew=1;
                }
            }
        }

        if(player->x>150&&!jumping&&!gameOver){
            moving=1;
            move(p,player);
            if(player->x==150&&cangetnew){
                togetnew=1;
            }
        }

        if(togetnew){
            int tmp=(platseq+2)%3;
            int tmpp=(platseq+1)%3;
            randomPlatform(p[tmp],currentScore,p[tmpp]);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        if (backgroundtexture == NULL) {
            SDL_Surface* backgroundsurface = IMG_Load("./res/img/4.png");
            if (backgroundsurface != NULL) {
                backgroundtexture = SDL_CreateTextureFromSurface(renderer, backgroundsurface);
                SDL_FreeSurface(backgroundsurface);
            }
        }
        if (backgroundtexture != NULL) {
            SDL_Rect imageRect = {0, 0, 800, 600};
            SDL_RenderCopy(renderer, backgroundtexture, NULL, &imageRect);
        }
        if (ant == NULL) {
            SDL_Surface* ants = IMG_Load("./res/img/2.png");
            if (ants != NULL) {
                ant = SDL_CreateTextureFromSurface(renderer, ants);
                SDL_FreeSurface(ants);
            }
        }

        if (ant != NULL) {
            SDL_RenderCopy(renderer, ant, NULL, &player->rect);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Color textColor = {0, 0, 0, 255};
        if(currentScore==0) {
            renderText(font, "Press space to jump", textColor, 200, 50);
        }
        draw(p[0]);
        draw(p[1]);
        draw(p[2]);
        SDL_Rect powerBar = {50, 550, (int)(power * 2.5), 10};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &powerBar);
        char scoreText[20];
        char heartText[20];
        snprintf(scoreText, sizeof(scoreText), "score:%d", currentScore);
        snprintf(heartText, sizeof(heartText), "heart:%d", heartCount);

        renderText(font, scoreText, textColor, WINDOW_WIDTH - 150, 10);
        renderText(font, heartText, textColor, WINDOW_WIDTH - 150, 50);
        if(!paused&&!gameOver){
            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            continue;
        }
        while(paused){
            printf("pausing");
            renderText(font, "paused", textColor, 350, 250);
            SDL_RenderPresent(renderer);
            SDL_Delay(128);
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    handleReturnToMainPage();
                    paused=0;
                    quit0 = 1;
                }
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_p) {
                        printf("unpause");
                        paused = 0;
                    }
                    if (e.key.keysym.sym == SDLK_r) {
                        printf("Restarted");
                        platseq=0;
                        currentScore = 0;
                        heartCount = 1;
                        power = 0;
                        jumping = 0;
                        moving=0;
                        gameOver=0;
                        paused=0;
                        initGame(player,p); // 重新开始时重新初始化游戏页面
                    }
                }
            }
        }
        while(gameOver){
            if(currentScore>maxScore)
                maxScore=currentScore;
            renderText(font, "Game Over", textColor, 350, 200);
            char highscore[20];
            snprintf(highscore, sizeof(highscore), "highest:%d", maxScore);
            renderText(font, highscore, textColor, 350, 250);
            renderText(font, "Press R to restart", textColor, 320, 300);
            SDL_RenderPresent(renderer);
            SDL_Delay(128);
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    handleReturnToMainPage();
                    gameOver=0;
                    quit0 = 1;
                }
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_r) {
                        printf("Restarted");
                        platseq=0;
                        currentScore = 0;
                        heartCount = 1;
                        power = 0;
                        jumping = 0;
                        moving=0;
                        gameOver=0;
                        paused=0;
                        initGame(player,p); // 重新开始时重新初始化游戏页面
                    }
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        printf("quit");
                        handleReturnToMainPage();
                        gameOver=0;
                        quit0 = 1;
                    }
                }
            }
        }
    }
}