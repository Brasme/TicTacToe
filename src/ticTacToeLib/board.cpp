#include "ticTacToe3D/board.h"
#include "ticTacToe3D/field.h"
#include "ticTacToe3D/color.h"
#include "ticTacToe3D/field.h"
#include <sstream>

namespace ttt {

    Field Board::Get(uint8_t row, uint8_t col) const
    {   
        Field f; 
        if ((row > 2) || (col > 2))
            return f;
        const size_t bitsIdx=7*((size_t)row*3+col);
        f.state=(uint8_t)((state>>bitsIdx)&0x7f);
        return f;
    }

    Board &Board::Set(uint8_t row, uint8_t col, const Field &field)
    {
        if (row > 2 || col > 2)
            return *this;
        const size_t bitsIdx=7*((size_t)row*3+col);
        const uint64_t mask=0x7full<<bitsIdx;
        const uint64_t bits=(uint64_t)field.state<<bitsIdx;
        state = (state&~mask)|bits;
        return *this;        
    }

    Board& Board::Clr(uint8_t row, uint8_t col)
    {
        if (row > 2 || col > 2)
            return *this;
        const size_t bitsIdx = 7 * ((size_t)row * 3 + col);
        const uint64_t mask = 0x7full << bitsIdx;
        state = state & ~mask;
        return *this;
    }

    std::string Board::ToStr(bool showIndex) const
    {
        std::stringstream ss;
        ToStr(ss,showIndex);
        return ss.str();
    }

    std::ostream& Board::ToStr(std::ostream& os,bool showIndex) const 
    {
        if (showIndex) {
            os << "r\\c| 0 | 1 | 2 |\n";
            os << "===+===+===+===+\n";
            for (uint8_t row = 0; row < 3; ++row) {
                os << ' ' << (int)row << " |" << Get(row, 0) << '|' << Get(row, 1) << '|' << Get(row, 2) << "|\n";
            }
            os << "===+===+===+===+\n";
            return os;
        }
        
        for (uint8_t row = 0; row < 3; ++row) {
            os << Get(row, 0) << ',' << Get(row, 1) << ',' << Get(row, 2) << "\n";
        }
        return os;
    }

    size_t Board::Solved() const
    {
        return Solved(ColorName::Yellow)+ Solved(ColorName::Green)+ Solved(ColorName::Blue)+ Solved(ColorName::Red);
         
    }

    static size_t numSolved(const Colors& a, const Colors& b, const Colors& c, uint8_t i)
    {
        size_t n = 0;
        n += a[0].idx == i ? (b[0].idx == i && c[0].idx == i ? 1 : 0) + (b[1].idx == i && c[2].idx == i ? 1 : 0) : 0;
        n += a[1].idx == i ? (b[1].idx == i && c[1].idx == i ? 1 : 0) : 0;
        n += a[2].idx == i ? (b[2].idx == i && c[2].idx == i ? 1 : 0) + (b[1].idx == i && c[0].idx == i ? 1 : 0) : 0;
        return n;
    }

    static size_t numSolved(const Colors& a, uint8_t i)
    {
        return a[0].idx == i && a[1].idx == i && a[2].idx == i ? 1 : 0;        
    }

    size_t Board::Solved(const Color& color) const
    {
        const uint8_t i = color.idx;
        if (i == 0 || i > 4)
            return 0;

        size_t n=0;

        // Check along columns
        for (uint8_t row = 0; row < 3; ++row) {
            n += numSolved(Get(row, 0).ToColors(), Get(row, 1).ToColors(), Get(row, 2).ToColors(), i);
        };
        
        // Check along rows
        for (uint8_t col = 0; col < 3; ++col) {
            n += numSolved(Get(0, col).ToColors(), Get(1, col).ToColors(), Get(2, col).ToColors(), i);
        };

        // Check vertically
        for (uint8_t row = 0; row < 3; ++row) {
            for (uint8_t col = 0; col < 3; ++col) {
                n += numSolved(Get(row, col).ToColors(), i);
            }
        }

        // Check along diagonals
        n += numSolved(Get(0, 0).ToColors(), Get(1, 1).ToColors(), Get(2, 2).ToColors(), i);
        n += numSolved(Get(2, 0).ToColors(), Get(1, 1).ToColors(), Get(0, 2).ToColors(), i);
        
        return n;
    }
    
    
    size_t Board::Num() const
    {
        size_t n = 0;
        Field f;
        for (size_t bitsIdx = 0; bitsIdx < 63; bitsIdx+=7) {
            f.state = (uint8_t)((state >> bitsIdx) & 0x7f);
            n += f.Num();
        }
        return n;
    }

    size_t Board::Num(uint8_t vIdx) const
    {
        if (vIdx > 2)
            return 0;
        size_t n = 0;
        Field f;
        for (size_t bitsIdx = 0; bitsIdx < 63; bitsIdx += 7) {
            f.state = (uint8_t)((state >> bitsIdx) & 0x7f);
            n += f.Num(vIdx);
        }
        return n;
    }

    size_t Board::Num(const Color& c) const
    {
        if (c.idx > 4)
            return 0;
        size_t n = 0;
        Field f;
        for (size_t bitsIdx = 0; bitsIdx < 63; bitsIdx += 7) {
            f.state = (uint8_t)((state >> bitsIdx) & 0x7f);
            n += f.Num(c);
        }
        return n;
    }

    size_t Board::Num(uint8_t vIdx,const Color& c) const
    {
        if (c.idx > 4)
            return 0;
        size_t n = 0;
        Field f;
        for (size_t bitsIdx = 0; bitsIdx < 63; bitsIdx += 7) {
            f.state = (uint8_t)((state >> bitsIdx) & 0x7f);
            n += f.Num(vIdx,c);
        }
        return n;
    }
}
