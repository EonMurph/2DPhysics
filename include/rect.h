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