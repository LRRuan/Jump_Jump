//
// Created by ruan on 21/1/2024.
//
#include"variable.h"
#include"SDLcommon.h"

const char* introductionText[] = {
        "press space to jump",
        "press longer,jump further",
        "press P to pause(while !jumping)",
        "catch wings to fly",
        "get heart to revive",
        "when dead or paused,press R to restart",
        "when playing, press esc to back"
};

void handleIntroduction() {
    currentPage = IntroductionPage;
}

void handleReturnToMainPage() {
    currentPage = MainPage;
}

void renderMainPage() {
    SDL_Color titleColor = {0, 0, 0, 255};  // 黑色
    int titleX = WINDOW_WIDTH / 2;  // 窗口的水平中心
    int titleY = 100;  // 距离窗口上边缘的距离
    renderText(titleFont, "jump  jump", titleColor, titleX, titleY);

    SDL_Point mousePos = {e.motion.x, e.motion.y};
    renderButton(&startButton, mousePos);
    renderButton(&introButton, mousePos);
    renderButton(&exitButton, mousePos);

    if (mainImageTexture == NULL) {
        SDL_Surface* mainImageSurface = IMG_Load("./res/img/1.png");
        if (mainImageSurface != NULL) {
            mainImageTexture = SDL_CreateTextureFromSurface(renderer, mainImageSurface);
            SDL_FreeSurface(mainImageSurface);
        }
    }

    if (mainImageTexture != NULL) {
        SDL_Rect imageRect = {420, 230, 260, 260};
        SDL_RenderCopy(renderer, mainImageTexture, NULL, &imageRect);
    }
}

void renderIntroductionPage() {
    SDL_Color textColor = {0, 0, 0, 255};
    int textX = 50;
    int textY = 150;

    if (introImageTexture == NULL) {
        SDL_Surface* introImageSurface = IMG_Load("./res/img/3.png");
        if (introImageSurface != NULL) {
            introImageTexture = SDL_CreateTextureFromSurface(renderer, introImageSurface);
            SDL_FreeSurface(introImageSurface);
        }
    }

    if (introImageTexture != NULL) {
        SDL_Rect imageRect = {0, 0, 800, 600};
        SDL_RenderCopy(renderer, introImageTexture, NULL, &imageRect);
    }

    for (int i = 0; i < sizeof(introductionText) / sizeof(introductionText[0]); ++i) {
        renderText(font, introductionText[i], textColor, textX, textY + i * 40);
    }

    // 渲染返回按钮
    SDL_Point mousePos = {0, 0};
    renderButton(&backButton, mousePos);
    //renderText(font, backButton.text, textColor, backButton.rect.x + 10, backButton.rect.y + 10);
}