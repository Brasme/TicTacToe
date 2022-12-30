#pragma once

#include "ticTacToe3D/lib.h"
#include <stdint.h>
#include <string>

namespace ttt {
    struct TTT_API Board {
        Board();
        uint64_t state;
        std::string ToStr() const;
    };
}