#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1200
#define HEIGHT 800
#define NUM_RECTS 10
#define RECT_SIZE 50

typedef struct {
    float x, y, vx, vy;
    int size, m;
} Rect;

Rect create_rect(int, int);
void draw_rect(SDL_Renderer *, Rect);
int colliding(Rect *, int);
void handle_bouncing(Rect *, Rect *);

void print_error(char *type) {
    fprintf(stderr, "SDL %s failed to initialise: %s\n", type, SDL_GetError());
}

int rand_range(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
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

    Rect rects[NUM_RECTS];
    for (int i = 0; i < NUM_RECTS; i++) {
        rects[i] = create_rect(rand_range(RECT_SIZE, WIDTH - RECT_SIZE), rand_range(RECT_SIZE, HEIGHT - RECT_SIZE));
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
                    SDL_SetRenderDrawColor(renderer, 255 - (100 * i),
                                           255, 255 - (100 * i),
                                           255);
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

Rect create_rect(int x, int y) {
    int size = RECT_SIZE;
    int min_ax = -5;
    int max_ax = 5;
    int vx = rand_range(min_ax, max_ax);
    int vy = rand_range(min_ax, max_ax);
    Rect rect = (Rect){x - ((float) size / 2.0f),
                       y - ((float) size / 2.0f),
                       vx,
                       vy,
                       size,
                       ((size * size) * (0.01 * size)) / 1000};
    return rect;
}

void draw_rect(SDL_Renderer *renderer, Rect rect) {
    SDL_RenderFillRectF(renderer,
                        &(SDL_FRect){rect.x, rect.y, rect.size, rect.size});
}

int colliding(Rect *rects, int i) {
    Rect *r1 = &rects[i];
    Rect *r2;
    for (i += 1; i < NUM_RECTS; i++) {
        r2 = &rects[i];
        if (r1->x < r2->x + r2->size && r1->x + r1->size > r2->x &&
            r1->y < r2->y + r2->size && r1->y + r1->size > r2->y) {
            // Collision detected
            return i;
        } else {
            continue;
        }
    }

    return 0;
}

void handle_bouncing(Rect *r1, Rect *r2) {
    // Get centers of rects
    int r1_center[2] = {r1->x + (r1->size / 2), r1->y + (r1->size / 2)};
    int r2_center[2] = {r2->x + (r2->size / 2), r2->y + (r2->size / 2)};

    // Get the distance between the centers
    float dist = (SDL_sqrtf(powf(r1_center[0] - r2_center[0], 2) +
                            powf(r1_center[1] - r2_center[1], 2)));
    // Early exit to avoid division by zero
    if (dist == 0.0f) {
        return;
    }
    
    // Calculate the normal of the collision and the velocity along that normal
    float nx = (r1_center[0] - r2_center[0]) / dist;
    float ny = (r1_center[1] - r2_center[1]) / dist;
    float vn = ((r1->vx - r2->vx) * nx) + ((r1->vy - r2->vy) * ny);
    // Early exit if no velocity along the normal
    if (vn > 0) {
        return;
    }

    // Calculate the force of the impulse
    float imp = -(2.0f * vn) / (1.0f / r1->m + 1.0f / r2->m);

    // Calculate the final velocities in each axis
    r1->vx += (imp / r1->m) * nx;
    r1->vy += (imp / r1->m) * ny;
    r2->vx -= (imp / r2->m) * nx;
    r2->vy -= (imp / r2->m) * ny;

    // Separate the collided rects
    float overlap = (r1->size / 2.0f + r2->size / 2.0f) - dist;
    if (overlap > 0) {
        r1->x += (overlap / 2.0f) * nx;
        r1->y += (overlap / 2.0f) * ny;
        r2->x -= (overlap / 2.0f) * nx;
        r2->y -= (overlap / 2.0f) * ny;
    }
}