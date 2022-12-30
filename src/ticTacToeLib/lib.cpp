#include "ticTacToe3D/lib.h"

#ifndef TTT_VERSION
#define TTT_VERSION "0.0.0"
#endif

namespace ttt {
    const char *version() {
        return TTT_VERSION;
    }
}