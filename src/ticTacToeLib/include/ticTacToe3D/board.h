#pragma once

#include "ticTacToe3D/lib.h"
#include <stdint.h>

namespace ttt {
    struct TTT_API Board {
        Board();
        uint64_t state;
    };
}