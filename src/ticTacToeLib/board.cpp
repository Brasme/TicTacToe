#include "ticTacToe3D/board.h"
#include "ticTacToe3D/field.h"
#include "ticTacToe3D/color.h"
#include "ticTacToe3D/field.h"
#include <sstream>

namespace ttt {

    uint64_t Board::state() const
    {
        uint64_t s = 0;
        for (uint8_t row = 0; row < 3; row++) {
            const Row& r = rows[row];
            for (uint8_t col = 0; col < 3; col++) {
                const Field& f = r[col];
                s = s * 125ull + (uint64_t)f.state;
            }
        }
        return s;
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
                const Row& r = rows[row];
                os << ' ' << (int)row << " |" << r[0] << '|' << r[1] << '|' << r[2] << "|\n";
            }
            os << "===+===+===+===+\n";
            return os;
        }
        
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row];
            os << r[0] << ',' << r[1] << ',' << r[1] << "\n";
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
            n += numSolved(rows[row][0].ToColors(), rows[row][1].ToColors(), rows[row][2].ToColors(), i);
        };
        
        // Check along rows
        for (uint8_t col = 0; col < 3; ++col) {
            n += numSolved(rows[0][col].ToColors(), rows[1][col].ToColors(), rows[2][col].ToColors(), i);
        };

        // Check vertically
        for (uint8_t row = 0; row < 3; ++row) {
            for (uint8_t col = 0; col < 3; ++col) {
                n += numSolved(rows[row][col].ToColors(), i);
            }
        }

        // Check along diagonals
        n += numSolved(rows[0][0].ToColors(), rows[1][1].ToColors(), rows[2][2].ToColors(), i);
        n += numSolved(rows[2][0].ToColors(), rows[1][1].ToColors(), rows[0][2].ToColors(), i);
        
        return n;
    }
    
    
    size_t Board::Num() const
    {
        size_t n = 0;
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row]; 
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.Num();
            }
        }            
        return n;
    }

    size_t Board::Num(uint8_t vIdx) const
    {
        if (vIdx > 2)
            return 0;
        size_t n = 0;        
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row];
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.Num(vIdx);
            }
        }

        return n;
    }

    size_t Board::Num(const Color& c) const
    {
        if (c.idx > 4)
            return 0;
        size_t n = 0;        
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row];
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.Num(c);
            }
        }
        return n;
    }

    size_t Board::Num(uint8_t vIdx,const Color& c) const
    {
        if (c.idx > 4)
            return 0;
        size_t n = 0;        
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row];
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.Num(vIdx,c);
            }
        }
        return n;
    }
}
