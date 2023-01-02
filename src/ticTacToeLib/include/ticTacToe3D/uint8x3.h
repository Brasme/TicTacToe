#ifndef TTT_UINT8X3_H_
#define TTT_UINT8X3_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API UInt8x3;   
}

#include "ticTacToe3D/base5x3.h"

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    struct TTT_API UInt8x3 {
        UInt8x3() {}
        explicit UInt8x3(uint8_t i0, uint8_t i1, uint8_t i2) : v{ i0,i1,i2 } {}
        explicit UInt8x3(const ttt::Base5x3& b5x3);
        const uint8_t& operator[](uint8_t idx) const { return v[idx]; }
        uint8_t& operator[](uint8_t idx) { return v[idx]; }
        uint8_t v[3];
        std::string ToStr() const;
    };
    TTT_API std::ostream& operator<<(std::ostream& os, const UInt8x3& c);
}

#endif
