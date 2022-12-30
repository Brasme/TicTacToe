#pragma once

#include "ticTacToe3D/lib.h"
#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    /// @brief Color 0='-' 1='y'(yellow), 2='g'(green), 3='b'(blue), 4='r'(red)
    struct TTT_API Color {
        Color() : idx(0) {}
        Color(int i) : idx(i<0?0:i>4?4:(uint8_t)i) {}
        uint8_t idx;        
        char ToChar() const;
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const Color& c);

    struct TTT_API FieldColors {
        FieldColors() {}
        FieldColors(Color i0,Color i1,Color i2) : colorIdx{i0,i1,i2} {}
        Color colorIdx[3];        
        std::string ToStr() const;
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const FieldColors& c);

    struct TTT_API Field {
        Field() : state(0) {}
        Field(const FieldColors& c) : state(c.colorIdx[0].idx + c.colorIdx[1].idx*5 + c.colorIdx[2].idx*25) {}
        uint8_t state;
        const FieldColors& Colors() const;
        inline std::string ToChar() const { return Colors().ToStr(); }
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const Field& c);

    struct TTT_API Board {
        Board() : state(0) {}
        uint64_t state;
        Field Get(uint8_t row,uint8_t col) const;
        Board& Set(uint8_t row,uint8_t col,const Field& field);

        std::string ToStr() const;
        std::ostream& ToStr(std::ostream& os) const;
    };
}