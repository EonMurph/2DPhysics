#include "utils.h"

void print_error(char *type) {
    fprintf(stderr, "SDL %s failed to initialise: %s\n", type, SDL_GetError());
}

int rand_range(int min, int max) {
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}
