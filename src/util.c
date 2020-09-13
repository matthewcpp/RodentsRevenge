#include "util.h"

#ifdef RR_PLATFORM_DESKTOP
static const char* path_sep = "/";
#else
static const char* path_sep = "\\";
#endif

const char* rr_path_sep() {
    return path_sep;
}
