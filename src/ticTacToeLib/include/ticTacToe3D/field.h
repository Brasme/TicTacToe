#ifndef TTT_FIELD_H_
#define TTT_FIELD_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API Field;
}

#include "ticTacToe3D/color.h"

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    struct TTT_API Field {
        Field() : state(0) {}
        Field(const Colors& c) : // Accept as non-explicit cast conversion
            state(V(c.colors[0].idx) + V(c.colors[1].idx) * 5 + V(c.colors[2].idx) * 25) {} 
        explicit Field(uint8_t i0, uint8_t i1, uint8_t i2) : state(V(i0) + 5 * V(i1) + 25 * V(i2)) {}
        explicit Field(const Color& c0, const Color& c1, const Color& c2) : state(V(c0.idx) + V(c1.idx) * 5 + V(c2.idx) * 25) {}
        explicit Field(uint8_t idx,const Color& c) : state(V(c.idx)*P(idx)) { }

        bool operator ==(const Field& o) const { return state == o.state; }
        bool operator !=(const Field& o) const { return !operator==(o); }

        uint8_t state;
        const Colors& ToColors() const;
        std::string ToStr() const;

        Field& Set(uint8_t idx, const Color& c);
        Field& Add(const Field &f);

        size_t Num() const;
        size_t Num(uint8_t vIdx) const;
        size_t Num(const Color& c) const;
        size_t Num(uint8_t vIdx,const Color& c) const;

        constexpr uint8_t V(uint8_t i) { return i >= 5 ? 0 : i; }
        constexpr uint8_t P(uint8_t vIdx) { return vIdx==0?1:vIdx==1?5:25; }
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const Field& c);
}

#endif
