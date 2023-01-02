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
        struct Row {
            Row() {}
            explicit Row(const std::string& state) { FromState(state); }
            Field column[3];
            Field& operator[](uint8_t col) { return column[col]; }
            const Field& operator[](uint8_t col) const { return column[col]; }
            std::string ToState() const;
            std::ostream& ToState(std::ostream &os) const;
            Row& FromState(const std::string& state);
        };
        
        Board() {}
        explicit Board(const std::string &state) {}
        Row     rows[3];
        uint8_t playerIdx;
        uint8_t pieces[4];

        Field& At(uint8_t row, uint8_t col) { return rows[row][col]; }
        const Field& At(uint8_t row, uint8_t col) const { return rows[row][col]; }

        Row& operator[](uint8_t row) { return rows[row]; }
        const Row& operator[](uint8_t row) const { return rows[row]; }

        Board& Set(uint8_t row,uint8_t col,const Field& field);
        Board& Clr(uint8_t row, uint8_t col);
        Board& Clr() { state = 0; return *this; }

        std::string ToStr(bool showIndex=false) const;
        std::ostream& ToStr(std::ostream& os, bool showIndex = false) const;

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