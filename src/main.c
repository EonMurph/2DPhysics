#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600

SDL_Rect create_rect(SDL_Rect *, int *);
void draw_rect(SDL_Renderer *, SDL_Rect);

void print_error(char *type) {
    fprintf(stderr, "SDL %s failed to initialise: %s\n", type, SDL_GetError());
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        print_error("context");
        return 1;
    }

    SDL_Window *window =
        SDL_CreateWindow("Physics", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (window == NULL) {
        print_error("window");
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        print_error("renderer");
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rects[2];
    int current_rect = 0;
    SDL_Rect rect;

    SDL_RaiseWindow(window);
    SDL_PumpEvents();
    SDL_Event e;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_q) {
                        running = 0;
                    }
                case SDL_MOUSEBUTTONDOWN:
                    rect = create_rect(rects, &current_rect);
                    draw_rect(renderer, rect);
                default:
                    continue;
            }
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_Rect create_rect(SDL_Rect * rects, int * current_rect) {
    SDL_Rect rect = (SDL_Rect){rand() % WIDTH, rand() % HEIGHT, 100, 100};
    rects[*current_rect] = rect;
    current_rect++;

    return rect;
}

void draw_rect(SDL_Renderer *renderer, SDL_Rect rect) {
    SDL_RenderFillRect(renderer, &rect);
}