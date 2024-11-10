//
// Created by ruan on 21/1/2024.
//
#include"SDLcommon.h"

int Random(int lower, int upper) {
    srand((unsigned int)time(NULL));
    return (rand() % (upper - lower + 1)) + lower;
}

void drawCube(const Platform *p) {
    SDL_RenderDrawLine(renderer, p->x, p->y, p->x + p->width, p->y);
    SDL_RenderDrawLine(renderer, p->x + p->width, p->y, p->x + p->width, p->y + p->h);
    SDL_RenderDrawLine(renderer, p->x + p->width, p->y + p->h, p->x, p->y + p->h);
    SDL_RenderDrawLine(renderer, p->x, p->y + p->h, p->x, p->y);

    SDL_RenderDrawLine(renderer, p->x + p->length, p->y - p->length, p->x + p->width + p->length, p->y - p->length);
    SDL_RenderDrawLine(renderer, p->x + p->width + p->length, p->y - p->length, p->x + p->width + p->length, p->y + p->h - p->length);
    //SDL_RenderDrawLine(renderer, p->x + p->width + p->length, p->y + p->h - p->length, p->x + p->length, p->y + p->h - p->length);
    //SDL_RenderDrawLine(renderer, p->x + p->length, p->y + p->h - p->length, p->x + p->length, p->y - p->length);

    SDL_RenderDrawLine(renderer, p->x, p->y, p->x + p->length, p->y - p->length);
    SDL_RenderDrawLine(renderer, p->x + p->width, p->y, p->x + p->width + p->length, p->y - p->length);
    SDL_RenderDrawLine(renderer, p->x + p->width, p->y + p->h, p->x + p->width + p->length, p->y + p->h - p->length);
    //SDL_RenderDrawLine(renderer, p->x, p->y + p->h, p->x + p->length, p->y + p->h - p->length);
}

void drawCylinder(const Platform *p) {
    const int segments = 30; // 圆周上的线段数

    for (int i = 0; i < segments; ++i) {
        float angle1 = ((float)i / segments) * 2 * M_PI;
        float angle2 = ((float)(i + 1) / segments) * 2 * M_PI;

        int x1 = p->x + (int)(p->r * cos(angle1));
        int y1 = p->y + (int)(p->r * sin(angle1));

        int x2 = p->x + (int)(p->r * cos(angle2));
        int y2 = p->y + (int)(p->r * sin(angle2));

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    for (int i = 0; i < segments/2; ++i) {
        float angle1 = ((float)i / segments) * 2 * M_PI;
        float angle2 = ((float)(i + 1) / segments) * 2 * M_PI;

        int x1 = p->x + (int)(p->r * cos(angle1));
        int y1 = p->y + p->h + (int)(p->r * sin(angle1));

        int x2 = p->x + (int)(p->r * cos(angle2));
        int y2 = p->y + p->h + (int)(p->r * sin(angle2));

        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    }

    SDL_RenderDrawLine(renderer, p->x - p->r, p->y, p->x - p->r, p->y + p->h);
    SDL_RenderDrawLine(renderer, p->x + p->r, p->y, p->x + p->r, p->y + p->h);
}

void draw(const Platform *p){
    if(p->shape==cyl){
        drawCylinder(p);
    }else{
        drawCube(p);
    }
}

void renderText(TTF_Font* font1, const char* text, SDL_Color textColor, int x, int y) {
    SDL_Surface* textSurface = TTF_RenderText_Solid(font1, text, textColor);

    if (textSurface == NULL) {
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    if (textTexture == NULL) {
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect destRect = { x, y, textSurface->w, textSurface->h };

    SDL_RenderCopy(renderer, textTexture, NULL, &destRect);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void renderButton(const Button* button, SDL_Point mousePos) {
    SDL_SetRenderDrawColor(renderer, button->bgColor.r, button->bgColor.g, button->bgColor.b, button->bgColor.a);
    SDL_RenderFillRect(renderer, &button->rect);

    int textWidth;
    int textHeight;
    TTF_SizeText(font, button->text, &textWidth, &textHeight);

    renderText(font, button->text, button->textColor,
               button->rect.x + (button->rect.w - textWidth) / 2,
               button->rect.y + (button->rect.h - textHeight) / 2);

    if (SDL_PointInRect(&mousePos, &button->rect)) {
        // 在按钮内
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    } else {
        // 不在内
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        SDL_RenderDrawRect(renderer, &button->rect);  // 绘制按钮边框
    }
}

int initializeSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return -1;
    }

    pauseMutex = SDL_CreateMutex();
    if (pauseMutex == NULL) {
        printf("SDL_CreateMutex failed: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    if (TTF_Init() == -1) {
        printf("TTF_Init failed: %s\n", TTF_GetError());
        SDL_Quit();
        return -1;
    }

    // 初始化SDL_Image
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return -1;
    }

    // 初始化SDL_Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Mix_OpenAudio failed: %s\n", Mix_GetError());
        SDL_Quit();
        IMG_Quit();
        TTF_Quit();
        return -1;
    }

    window = SDL_CreateWindow("跳一跳", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == NULL) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_CloseAudio();
        IMG_Quit();
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    return 0;
}

TTF_Font* loadFont(const char* fontPath, int fontSize) {
    char relativePath[256];
    snprintf(relativePath, sizeof(relativePath), "%s", fontPath);

    TTF_Font* loadedFont = TTF_OpenFont(relativePath, fontSize);

    if (loadedFont == NULL) {
        printf("Error opening font: %s\n", TTF_GetError());
        return NULL;
    }

    return loadedFont;
}

void cleanupSDL() {
    SDL_DestroyMutex(pauseMutex);
    TTF_CloseFont(font);
    TTF_CloseFont(titleFont);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void cleanupMainPage() {
    if (mainImageTexture != NULL) {
        SDL_DestroyTexture(mainImageTexture);
        mainImageTexture = NULL;
    }
}
void cleanupIntroductionPage() {
    if (introImageTexture != NULL) {
        SDL_DestroyTexture(introImageTexture);
        introImageTexture = NULL;
    }
}
void cleanupGamePage(){
    if (ant != NULL) {
        SDL_DestroyTexture(ant);
        ant = NULL;
    }
    if(backgroundtexture!=NULL){
        SDL_DestroyTexture(backgroundtexture);
        ant=NULL;
    }
}

void cleanupAndQuit(Player*player,Platform*p[3]) {
    cleanupMainPage();
    cleanupIntroductionPage();
    cleanupGamePage();
    cleanupSDL();
    free(player);
    for(int i=0;i<3;i++){
        free(p[i]);
    }
}