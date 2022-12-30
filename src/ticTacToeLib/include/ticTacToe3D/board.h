#ifndef TTT_BOARD_H_
#define TTT_BOARD_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API Field;
    struct TTT_API Board;
    struct TTT_API Color;
}

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    struct TTT_API Board {
        Board() : state(0) {}
        explicit Board(const uint64_t &v) : state(v) {}
        uint64_t state;
        Field Get(uint8_t row,uint8_t col) const;
        Board& Set(uint8_t row,uint8_t col,const Field& field);
        Board& Clr(uint8_t row, uint8_t col);
        Board& Clr() { state = 0; return *this; }

        std::string ToStr() const;
        std::ostream& ToStr(std::ostream& os) const;

        size_t Solved() const;
        size_t Solved(const Color& color) const;

        // Number of pieces on board
        size_t Num() const;

        // Number of pieces on board vertically at idx 0..2 (size)
        size_t Num(uint8_t vIdx) const;

        // Number of pieces on board of color
        size_t Num(const Color &c) const;

        // Number of pieces on board of color at idx 0..2 (size)
        size_t Num(uint8_t vIdx,const Color& c) const;
    };
}

#endif