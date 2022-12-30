#include "ticTacToe3D/color.h"
#include "ticTacToe3D/field.h"
#include <sstream>

namespace ttt {
    uint8_t Color::NameToIdx(const ColorName& c) {
        switch (c) {
        case ColorName::None: return 0;
        case ColorName::Yellow: return 1;
        case ColorName::Green: return 2;
        case ColorName::Blue: return 3;
        case ColorName::Red: return 4;
        }
        return 0;
    }

    char Color::ToChar() const {
        static const char* colorIdxChar = "-ygbr";
        return idx>=5?'-':colorIdxChar[idx];
    }
    
    Color::operator ColorName() const {
        static const ttt::ColorName idxToColorName[5] = { ColorName::None,ColorName::Yellow,ColorName::Green,ColorName::Blue,ColorName::Red };
        return idx>=5?ColorName::None:idxToColorName[idx];
    }

    std::ostream &operator<<(std::ostream &os, const Color &c) { os << c.ToChar(); return os; }

    Colors::Colors(const Field& f) {
        *this = f.Colors();
    }

    std::string Colors::ToStr() const
    {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    std::ostream& operator<<(std::ostream& os, const Colors& c)
    {
        os << c.colors[0] << c.colors[1] << c.colors[2];
        return os;
    }


}
