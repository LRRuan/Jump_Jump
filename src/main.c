#include"SDLcommon.h"
#include"variable.h"
#include"page.h"
#include"game.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
TTF_Font* titleFont = NULL;
SDL_Texture* introImageTexture = NULL;
SDL_Texture* mainImageTexture = NULL;
SDL_Texture* backgroundtexture=NULL;
SDL_Texture* ant=NULL;
SDL_Event e;

Button startButton = {{100, 200, 200, 80}, "start", {255, 255, 255, 255}, {0, 0, 255, 255}};
Button introButton = {{100, 300, 200, 80}, "introduction", {255, 255, 255, 255}, {0, 255, 0, 255}};
Button exitButton = {{100, 400, 200, 80}, "quit", {255, 255, 255, 255}, {255, 0, 0, 255}};

enum Page currentPage = MainPage;

int maxScore = 0; // 最高分
int currentScore = 0; // 当前分数
int heartCount = 1; // 爱心数量
int maxPoint = 0; // 最高分
bool jumping = 0; // 用于检测是否在跳跃状态
bool powering = 0; // 用于检测是否在蓄力状态
double power = 0; // 蓄力值
bool gameOver = 0; // 游戏结束标志
bool moving=0;
int platseq=0;
bool togetnew=0;
bool cangetnew=0;
bool judging=0;
int bonus=4;
bool paused=0;

Button backButton = {{10, 10, 100, 40}, "back", {255, 255, 255, 255}, {128, 128, 128, 255}};
SDL_mutex* pauseMutex;

int main(int argc,char* argv[]) {
    //setWorkingDirectoryToExecutable;
    Platform *p[3];
    for(int i=0;i<3;i++){
        p[i]=(Platform*)malloc(sizeof(Platform));
    }
    Player *player=(Player*) malloc(sizeof(Player));
    if (initializeSDL() != 0) {
        cleanupAndQuit(player,p);
        return -1;
    }

    font = loadFont("./res/fonts/Bear-hard-candy-2.ttf", FONT_SIZE);

    if (font == NULL) {
        cleanupAndQuit(player,p);
        return -1;
    }

    titleFont = loadFont("res/fonts/Bear-hard-candy-2.ttf", TITLE_FONT_SIZE);

    if (titleFont == NULL) {
        cleanupAndQuit(player,p);
        return -1;
    }

    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_Point mousePos = {e.motion.x, e.motion.y};
                if (currentPage == MainPage) {
                    if (SDL_PointInRect(&mousePos, &startButton.rect)) {
                        handleGameStart(player,p);
                    } else if (SDL_PointInRect(&mousePos, &introButton.rect)) {
                        handleIntroduction();
                    } else if (SDL_PointInRect(&mousePos, &exitButton.rect)) {
                        quit = 1;
                    }
                } else if (currentPage == IntroductionPage) {
                    if (SDL_PointInRect(&mousePos, &backButton.rect)) {
                        handleReturnToMainPage();
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (currentPage == MainPage) {
            renderMainPage();
        } else if (currentPage == IntroductionPage) {
            renderIntroductionPage();
        }
        //drawCylinder( 300, 100, 50, 40);
        SDL_RenderPresent(renderer);
        SDL_Delay(128);
    }

    cleanupAndQuit(player,p);

    return 0;
}

