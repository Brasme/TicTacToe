#ifndef TTT_BOARD_H_
#define TTT_BOARD_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API Board;    
}

#include "ticTacToe3D/field.h"

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    
    struct TTT_API Board {
        struct Row {
            Row() {}
            // explicit Row(const std::string& state) { FromState(state); }
            Field column[3];
            Field& operator[](uint8_t col) { return column[col]; }
            const Field& operator[](uint8_t col) const { return column[col]; }
            // std::string ToState() const;
            // std::ostream& ToState(std::ostream &os) const;
            // Row& FromState(const std::string& state);
            Row& Clr() { column[0].state = 0; column[1].state = 0; column[2].state = 0; return *this; }
        };

        Board() : playerIdx(0) {}
        explicit Board(const std::string& state) : playerIdx(0) {}
        explicit Board(const uint64_t& state) : playerIdx(0) {}
        Row     rows[3];
        uint8_t playerIdx;

        uint64_t state() const;
    
        Field& At(uint8_t row, uint8_t col) { return rows[row][col]; }
        const Field& At(uint8_t row, uint8_t col) const { return rows[row][col]; }

        Row& operator[](uint8_t row) { return rows[row]; }
        const Row& operator[](uint8_t row) const { return rows[row]; }

        Board& Set(uint8_t row, uint8_t col, const Field& field) { rows[row][col] = field; return *this; }
        Board& Clr(uint8_t row, uint8_t col) { rows[row][col].state = 0; return *this; }
        Board& Clr() { rows[0].Clr(); rows[1].Clr(); rows[2].Clr(); return *this; }

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