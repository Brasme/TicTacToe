#include "utils/lib.h"

#ifndef UTILS_VERSION
#define UTILS_VERSION "0.0.0"
#endif

namespace utils {
    const char *version() {
        return UTILS_VERSION;
    }
}