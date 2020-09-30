#include "util.h"

#include <stdlib.h>

#ifdef RR_PLATFORM_DESKTOP
static const char* path_sep = "/";
#else
static const char* path_sep = "\\";
#endif

const char* rr_path_sep() {
    return path_sep;
}

int rand_between(int min, int max) {
    return min + (rand() % (max - min));
}
