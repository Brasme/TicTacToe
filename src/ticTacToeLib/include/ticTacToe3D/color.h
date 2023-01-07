#ifndef TTT_COLOR_H_
#define TTT_COLOR_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API Field;
    struct TTT_API Color;
    struct TTT_API Colors;
}

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    enum class ColorName {
        None,
        Yellow,
        Green,
        Blue,
        Red
    };
    
    /// @brief Color 0='-' 1='y'(yellow), 2='g'(green), 3='b'(blue), 4='r'(red)
    struct TTT_API Color {
        Color() : idx(0) {}
        Color(const ColorName& c) : idx(nameToIdx(c)) {} // Let this pass as non-explicit to allow direct conversion
        explicit Color(int i) : idx(0) {
            switch (i) {
            case 0:
            case '-':
                idx = 0; break;
            case 1:
            case 'y':
                idx = 1; break;
            case 2:
            case 'g':
                idx = 2; break;
            case 3:
            case 'b':
                idx = 3; break;
            case 4:
            case 'r':
                idx = 4; break;
            default:
                idx = 0;
            }            
        }
        uint8_t idx;        
        char toChar() const;
        static uint8_t nameToIdx(const ColorName& c);
        operator ColorName() const;
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const Color& c);

    struct TTT_API Colors {
        Colors() {}
        explicit Colors(const std::string& colors);
        explicit Colors(uint8_t i0, uint8_t i1, uint8_t i2) : colors{ ttt::Color(i0),ttt::Color(i1),ttt::Color(i2) } {}
        explicit Colors(const ttt::Color &c0,const ttt::Color &c1,const ttt::Color &c2) : colors{c0,c1,c2 } {}
        explicit Colors(const ttt::Field& f);

        Colors& set(uint8_t idx, const Color& c) { if (idx <= 2) { colors[idx] = c; } return *this; }
        const Color& operator[](uint8_t idx) const { return colors[idx]; }
        Color& operator[](uint8_t idx) { return colors[idx]; }

        Color colors[3];
        std::string toStr() const;
    };
    TTT_API std::ostream& operator<<(std::ostream& os, const Colors& c);
}

#include "ticTacToe3D/field.h"

#endif
