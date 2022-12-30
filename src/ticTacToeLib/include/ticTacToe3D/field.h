#ifndef TTT_FIELD_H_
#define TTT_FIELD_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API Color;
    struct TTT_API Colors;
    struct TTT_API Field;
}

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    struct TTT_API Field {
        Field() : state(0) {}
        Field(const ttt::Colors& c); // Accept as non-explicit cast conversion
        explicit Field(uint8_t i0, uint8_t i1, uint8_t i2) : state(V(i0) + 5 * V(i1) + 25 * V(i2)) {}
        explicit Field(const ttt::Color& c0, const ttt::Color& c1, const ttt::Color& c2);

        bool operator ==(const Field& o) const;
        bool operator !=(const Field& o) const { return !operator==(o); }

        uint8_t state;
        const Colors& Colors() const;
        std::string ToStr() const;

        Field& Set(uint8_t idx, const Color& c);

        size_t Num() const;
        size_t Num(uint8_t vIdx) const;
        size_t Num(const Color& c) const;
        size_t Num(uint8_t vIdx,const Color& c) const;

        static uint8_t V(uint8_t i) { return i >= 5 ? 0 : i; }
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const Field& c);
}

#endif
