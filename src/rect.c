#include <SDL2/SDL.h>
#include <math.h>
#include "rect.h"
#include "utils.h"

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