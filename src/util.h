#ifndef RR_UTIL_H
#define RR_UTIL_H

#include "cutil/vector.h"

const char* rr_path_sep();
void string_split(const char* str, const char* split, cutil_vector* out);

#endif
