#include "ticTacToe3D/board.h"
#include <sstream>

namespace ttt {
    Field Board::Get(uint8_t row, uint8_t col) const
    {   
        Field f;
        // assert((row>=0) && (row<=2));
        const size_t bitsIdx=3*(row*3+col);
        f.state=(uint8_t)((state>>bitsIdx)&0x7);
        return f;
    }

    Board &Board::Set(uint8_t row, uint8_t col, const Field &field)
    {
        const size_t bitsIdx=3*(row*3+col);
        const uint64_t mask=0x7ull<<bitsIdx;
        const uint64_t bits=(uint64_t)field.state<<bitsIdx;
        state = (state&~mask)|bits;
        return *this;        
    }

    std::string Board::ToStr() const
    {
        std::stringstream ss;
        ToStr(ss);
        return ss.str();
    }

    std::ostream& Board::ToStr(std::ostream& os) const 
    {
        for (uint8_t row=0;row<3;++row) {
            os << Get(row,0) << ',' << Get(row,1) << ',' << Get(row,2) << "\n";            
        }    
        return os;
    }

    static const char *colorIdxChar="-ygbr...";
    char Color::ToChar() const {
        return colorIdxChar[idx&0x7];    
    }

    TTT_API std::ostream &operator<<(std::ostream &os, const Color &c) { os << c.ToChar(); return os; }

    static const FieldColors fieldToFieldColorIdx[128] = {
        FieldColors(0, 0, 0), FieldColors(1, 0, 0), FieldColors(2, 0, 0), FieldColors(3, 0, 0), FieldColors(4, 0, 0), FieldColors(0, 1, 0), FieldColors(1, 1, 0), FieldColors(2, 1, 0),
        FieldColors(3, 1, 0), FieldColors(4, 1, 0), FieldColors(0, 2, 0), FieldColors(1, 2, 0), FieldColors(2, 2, 0), FieldColors(3, 2, 0), FieldColors(4, 2, 0), FieldColors(0, 3, 0),
        FieldColors(1, 3, 0), FieldColors(2, 3, 0), FieldColors(3, 3, 0), FieldColors(4, 3, 0), FieldColors(0, 4, 0), FieldColors(1, 4, 0), FieldColors(2, 4, 0), FieldColors(3, 4, 0),
        FieldColors(4, 4, 0), FieldColors(0, 0, 1), FieldColors(1, 0, 1), FieldColors(2, 0, 1), FieldColors(3, 0, 1), FieldColors(4, 0, 1), FieldColors(0, 1, 1), FieldColors(1, 1, 1),
        FieldColors(2, 1, 1), FieldColors(3, 1, 1), FieldColors(4, 1, 1), FieldColors(0, 2, 1), FieldColors(1, 2, 1), FieldColors(2, 2, 1), FieldColors(3, 2, 1), FieldColors(4, 2, 1),
        FieldColors(0, 3, 1), FieldColors(1, 3, 1), FieldColors(2, 3, 1), FieldColors(3, 3, 1), FieldColors(4, 3, 1), FieldColors(0, 4, 1), FieldColors(1, 4, 1), FieldColors(2, 4, 1),
        FieldColors(3, 4, 1), FieldColors(4, 4, 1), FieldColors(0, 0, 2), FieldColors(1, 0, 2), FieldColors(2, 0, 2), FieldColors(3, 0, 2), FieldColors(4, 0, 2), FieldColors(0, 1, 2),
        FieldColors(1, 1, 2), FieldColors(2, 1, 2), FieldColors(3, 1, 2), FieldColors(4, 1, 2), FieldColors(0, 2, 2), FieldColors(1, 2, 2), FieldColors(2, 2, 2), FieldColors(3, 2, 2),
        FieldColors(4, 2, 2), FieldColors(0, 3, 2), FieldColors(1, 3, 2), FieldColors(2, 3, 2), FieldColors(3, 3, 2), FieldColors(4, 3, 2), FieldColors(0, 4, 2), FieldColors(1, 4, 2),
        FieldColors(2, 4, 2), FieldColors(3, 4, 2), FieldColors(4, 4, 2), FieldColors(0, 0, 3), FieldColors(1, 0, 3), FieldColors(2, 0, 3), FieldColors(3, 0, 3), FieldColors(4, 0, 3),
        FieldColors(0, 1, 3), FieldColors(1, 1, 3), FieldColors(2, 1, 3), FieldColors(3, 1, 3), FieldColors(4, 1, 3), FieldColors(0, 2, 3), FieldColors(1, 2, 3), FieldColors(2, 2, 3),
        FieldColors(3, 2, 3), FieldColors(4, 2, 3), FieldColors(0, 3, 3), FieldColors(1, 3, 3), FieldColors(2, 3, 3), FieldColors(3, 3, 3), FieldColors(4, 3, 3), FieldColors(0, 4, 3),
        FieldColors(1, 4, 3), FieldColors(2, 4, 3), FieldColors(3, 4, 3), FieldColors(4, 4, 3), FieldColors(0, 0, 4), FieldColors(1, 0, 4), FieldColors(2, 0, 4), FieldColors(3, 0, 4),
        FieldColors(4, 0, 4), FieldColors(0, 1, 4), FieldColors(1, 1, 4), FieldColors(2, 1, 4), FieldColors(3, 1, 4), FieldColors(4, 1, 4), FieldColors(0, 2, 4), FieldColors(1, 2, 4),
        FieldColors(2, 2, 4), FieldColors(3, 2, 4), FieldColors(4, 2, 4), FieldColors(0, 3, 4), FieldColors(1, 3, 4), FieldColors(2, 3, 4), FieldColors(3, 3, 4), FieldColors(4, 3, 4),
        FieldColors(0, 4, 4), FieldColors(1, 4, 4), FieldColors(2, 4, 4), FieldColors(3, 4, 4), FieldColors(4, 4, 4), FieldColors(0, 0, 0), FieldColors(0, 0, 0), FieldColors(0, 0, 0)};

    std::string FieldColors::ToStr() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    std::ostream &operator<<(std::ostream &os, const FieldColors &c)
    {
        os << c.colorIdx[0] << c.colorIdx[1] << c.colorIdx[2];
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const Field &c)
    {
        os << c.Colors();
        return os;
    }
    
    const FieldColors &Field::Colors() const
    {
        return fieldToFieldColorIdx[state&0x7f];
    }
}
