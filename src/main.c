#include <SDL2/SDL.h>

#include "rect.h"
#include "utils.h"

#define WIDTH 1200
#define HEIGHT 800

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

    Rect rects[NUM_RECTS];
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i] = create_rect(rand_range(RECT_SIZE, WIDTH - RECT_SIZE),
                               rand_range(RECT_SIZE, HEIGHT - RECT_SIZE));
    }
    int current_rect = 0;

    SDL_RaiseWindow(window);
    SDL_PumpEvents();
    SDL_Event e;
    int running = 1;
    int paused = 0;
    int mouse_x, mouse_y;
    int collision_rect;
    while (running) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_q) {
                        running = 0;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mouse_x, &mouse_y);
                    rects[current_rect] = create_rect(mouse_x, mouse_y);
                    current_rect = (current_rect + 1) % NUM_RECTS;
                    break;
                    // paused = !paused;
                    printf("\n");
                default:
                    break;
            }
        }
        if (!paused) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            for (int i = 0; i < NUM_RECTS; i++) {
                Rect *rect = &rects[i];
                rect->x += rect->vx;
                rect->y += rect->vy;
                if (rect->x + rect->size > WIDTH) {
                    rect->vx *= -1;
                    rect->x += rect->vx;
                } else if (rect->x < 0) {
                    rect->vx *= -1;
                    rect->x += rect->vx;
                }
                if (rect->y + rect->size > HEIGHT) {
                    rect->vy *= -1;
                    rect->y += rect->vy;
                } else if (rect->y < 0) {
                    rect->vy *= -1;
                    rect->y += rect->vy;
                }
            }
            for (int i = 0; i < NUM_RECTS; i++) {
                collision_rect = colliding(rects, i);
                if (collision_rect != 0) {
                    handle_bouncing(&rects[i], &rects[collision_rect]);
                    SDL_SetRenderDrawColor(renderer, 200, 0, 200, 255);
                } else {
                    SDL_SetRenderDrawColor(renderer, 255 - (100 * i), 255,
                                           255 - (100 * i), 255);
                }
                draw_rect(renderer, rects[i]);
            }

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
