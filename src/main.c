#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 800
#define HEIGHT 600
#define NUM_RECTS 2

SDL_Rect create_rect(int, int);
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

    SDL_Rect rects[NUM_RECTS];
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i] = create_rect(rand() % WIDTH, rand() % HEIGHT);
    }
    int current_rect = 0;
    SDL_Rect rect;

    SDL_RaiseWindow(window);
    SDL_PumpEvents();
    SDL_Event e;
    int running = 1;
    int mouse_x, mouse_y;
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
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    rect = create_rect(mouse_x, mouse_y);
                    rects[current_rect] = rect;
                    current_rect = (current_rect + 1) % NUM_RECTS;
                default:
                    continue;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for (int i = 0; i < NUM_RECTS; i++) {
            draw_rect(renderer, rects[i]);
        }
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

SDL_Rect create_rect(int x, int y) {
    int size = 100;
    SDL_Rect rect = (SDL_Rect){x - (size / 2), y - (size / 2), size, size};
    return rect;
}

void draw_rect(SDL_Renderer *renderer, SDL_Rect rect) {
    SDL_RenderFillRect(renderer, &rect);
}