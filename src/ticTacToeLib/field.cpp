#include "ticTacToe3D/field.h"
#include "ticTacToe3D/color.h"
#include <sstream>

namespace ttt {
    static const Colors fieldToColors[128] = {
        Colors(0, 0, 0), Colors(1, 0, 0), Colors(2, 0, 0), Colors(3, 0, 0), Colors(4, 0, 0), Colors(0, 1, 0), Colors(1, 1, 0), Colors(2, 1, 0),
        Colors(3, 1, 0), Colors(4, 1, 0), Colors(0, 2, 0), Colors(1, 2, 0), Colors(2, 2, 0), Colors(3, 2, 0), Colors(4, 2, 0), Colors(0, 3, 0),
        Colors(1, 3, 0), Colors(2, 3, 0), Colors(3, 3, 0), Colors(4, 3, 0), Colors(0, 4, 0), Colors(1, 4, 0), Colors(2, 4, 0), Colors(3, 4, 0),
        Colors(4, 4, 0), Colors(0, 0, 1), Colors(1, 0, 1), Colors(2, 0, 1), Colors(3, 0, 1), Colors(4, 0, 1), Colors(0, 1, 1), Colors(1, 1, 1),
        Colors(2, 1, 1), Colors(3, 1, 1), Colors(4, 1, 1), Colors(0, 2, 1), Colors(1, 2, 1), Colors(2, 2, 1), Colors(3, 2, 1), Colors(4, 2, 1),
        Colors(0, 3, 1), Colors(1, 3, 1), Colors(2, 3, 1), Colors(3, 3, 1), Colors(4, 3, 1), Colors(0, 4, 1), Colors(1, 4, 1), Colors(2, 4, 1),
        Colors(3, 4, 1), Colors(4, 4, 1), Colors(0, 0, 2), Colors(1, 0, 2), Colors(2, 0, 2), Colors(3, 0, 2), Colors(4, 0, 2), Colors(0, 1, 2),
        Colors(1, 1, 2), Colors(2, 1, 2), Colors(3, 1, 2), Colors(4, 1, 2), Colors(0, 2, 2), Colors(1, 2, 2), Colors(2, 2, 2), Colors(3, 2, 2),
        Colors(4, 2, 2), Colors(0, 3, 2), Colors(1, 3, 2), Colors(2, 3, 2), Colors(3, 3, 2), Colors(4, 3, 2), Colors(0, 4, 2), Colors(1, 4, 2),
        Colors(2, 4, 2), Colors(3, 4, 2), Colors(4, 4, 2), Colors(0, 0, 3), Colors(1, 0, 3), Colors(2, 0, 3), Colors(3, 0, 3), Colors(4, 0, 3),
        Colors(0, 1, 3), Colors(1, 1, 3), Colors(2, 1, 3), Colors(3, 1, 3), Colors(4, 1, 3), Colors(0, 2, 3), Colors(1, 2, 3), Colors(2, 2, 3),
        Colors(3, 2, 3), Colors(4, 2, 3), Colors(0, 3, 3), Colors(1, 3, 3), Colors(2, 3, 3), Colors(3, 3, 3), Colors(4, 3, 3), Colors(0, 4, 3),
        Colors(1, 4, 3), Colors(2, 4, 3), Colors(3, 4, 3), Colors(4, 4, 3), Colors(0, 0, 4), Colors(1, 0, 4), Colors(2, 0, 4), Colors(3, 0, 4),
        Colors(4, 0, 4), Colors(0, 1, 4), Colors(1, 1, 4), Colors(2, 1, 4), Colors(3, 1, 4), Colors(4, 1, 4), Colors(0, 2, 4), Colors(1, 2, 4),
        Colors(2, 2, 4), Colors(3, 2, 4), Colors(4, 2, 4), Colors(0, 3, 4), Colors(1, 3, 4), Colors(2, 3, 4), Colors(3, 3, 4), Colors(4, 3, 4),
        Colors(0, 4, 4), Colors(1, 4, 4), Colors(2, 4, 4), Colors(3, 4, 4), Colors(4, 4, 4), Colors(0, 0, 0), Colors(0, 0, 0), Colors(0, 0, 0) 
    };

    std::ostream &operator<<(std::ostream &os, const Field &c)
    {
        os << c.toColors();
        return os;
    }
    
    const Colors &Field::toColors() const
    {
        return fieldToColors[state&0x7f];
    }

    Field& Field::set(uint8_t idx, const Color& c)
    {
        state = Field(Colors(*this).set(idx, c)).state;
        return *this;
    }

    Field& Field::add(const Field& f)
    {
        Colors c0(toColors());
        const Colors& c1 = f.toColors();
        for (uint8_t vIdx = 0; vIdx < 3; ++vIdx) {
            const uint8_t cIdx = c1[vIdx].idx;
            if (cIdx > 0)
                c0[vIdx].idx = cIdx;
        }
        state = Field(c0).state;
        return *this;
    }

    std::string Field::toStr() const { 
        return toColors().toStr(); 
    }

    /// <summary>
    /// Array of "number" lookup where group 2 bits represents the sum of each color '0/none','1/yellow','2/green','3/blue','4/red'
    ///   0-1: # of none   (idx==0)
    ///   2-3: # of yellow (idx==1)
    ///   4-5: # of green  (idx==2)
    ///   6-7: # of blue   (idx==3)
    ///   8-9: # of red    (idx==4)
    /// 10-11: # total yellow -> red
    /// </summary>
    static const uint16_t fieldToNum[128] = {
        0x0003,0x0406,0x0412,0x0442,0x0502,0x0406,0x0809,0x0815,0x0845,0x0905,0x0412,0x0815,0x0821,0x0851,0x0911,0x0442,
        0x0845,0x0851,0x0881,0x0941,0x0502,0x0905,0x0911,0x0941,0x0A01,0x0406,0x0809,0x0815,0x0845,0x0905,0x0809,0x0C0C,
        0x0C18,0x0C48,0x0D08,0x0815,0x0C18,0x0C24,0x0C54,0x0D14,0x0845,0x0C48,0x0C54,0x0C84,0x0D44,0x0905,0x0D08,0x0D14,
        0x0D44,0x0E04,0x0412,0x0815,0x0821,0x0851,0x0911,0x0815,0x0C18,0x0C24,0x0C54,0x0D14,0x0821,0x0C24,0x0C30,0x0C60,
        0x0D20,0x0851,0x0C54,0x0C60,0x0C90,0x0D50,0x0911,0x0D14,0x0D20,0x0D50,0x0E10,0x0442,0x0845,0x0851,0x0881,0x0941,
        0x0845,0x0C48,0x0C54,0x0C84,0x0D44,0x0851,0x0C54,0x0C60,0x0C90,0x0D50,0x0881,0x0C84,0x0C90,0x0CC0,0x0D80,0x0941,
        0x0D44,0x0D50,0x0D80,0x0E40,0x0502,0x0905,0x0911,0x0941,0x0A01,0x0905,0x0D08,0x0D14,0x0D44,0x0E04,0x0911,0x0D14,
        0x0D20,0x0D50,0x0E10,0x0941,0x0D44,0x0D50,0x0D80,0x0E40,0x0A01,0x0E04,0x0E10,0x0E40,0x0F00,0x0003,0x0003,0x0003
    };    
    size_t Field::num() const
    {
        return (fieldToNum[state]>>10)&0x3;
    }
    size_t Field::num(uint8_t vIdx) const
    {
        return fieldToColors[state][vIdx].idx > 0 ? 1 : 0;
    }

    size_t Field::num(const Color& c) const
    {
        return (fieldToNum[state] >> (c.idx*2))&0x3;
    }

    size_t Field::num(uint8_t vIdx,const Color& c) const
    {
        return fieldToColors[state][vIdx].idx == c.idx ? 1 : 0;        
    }


}
