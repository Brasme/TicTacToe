#ifndef TTT_BASE5X3_H_
#define TTT_BASE5X3_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API Base5x3;
}

#include "ticTacToe3D/uint8x3.h"

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    struct TTT_API Base5x3 {
        Base5x3() : b5x3(0) {}
        Base5x3(const UInt8x3& v) : // Accept as non-explicit cast conversion
            state(V(v.v[0]) + V(v.v[1]) * 5 + V(v.v[2]) * 25) {} 
        explicit Base5x3(uint8_t i0, uint8_t i1, uint8_t i2) : b5x3(V(i0) + 5 * V(i1) + 25 * V(i2)) {}
        explicit Base5x3(uint8_t baseIdx,const uint8_t& v) : b5x3(V(v)*B(baseIdx)) { }

        bool operator ==(const Base5x3& o) const;
        bool operator !=(const Base5x3& o) const { return !operator==(o); }

        uint8_t b5x3;
        const Colors& ToColors() const;
        std::string ToStr() const;

        Base5x3& Set(uint8_t idx, const Color& c);
        Base5x3& Add(const Base5x3 &f);

        size_t Num() const;
        size_t Num(uint8_t vIdx) const;
        size_t Num(const Color& c) const;
        size_t Num(uint8_t vIdx,const Color& c) const;

        constexpr uint8_t V(uint8_t i) { return i >= 5 ? 0 : i; }
        constexpr uint8_t B(uint8_t i) { return i==0?1:i==1?5:25; }
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const Base5x3& c);
}

#endif
