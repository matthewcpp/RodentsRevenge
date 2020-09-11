#ifndef RR_DEFS_H
#define RR_DEFS_H

#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER < 1900
    #define snprintf_func _snprintf
#else
    #define snprintf_func snprintf
#endif

#endif
