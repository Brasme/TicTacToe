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

    struct TTT_API BoardState {
        BoardState() : playerIdx(0) { 
            for (uint8_t r = 0; r < 3; ++r)
                for (uint8_t c = 0; c < 3; ++c)
                    fields[r][c] = 0;
        }
        explicit operator uint64_t() const {
            uint64_t v=0;
            for (uint8_t r = 0; r < 3; ++r)
                for (uint8_t c = 0; c < 3; ++c)
                    v = v * 125ull + (uint64_t)fields[r][c];
        }
        uint8_t playerIdx;
        uint8_t fields[3][3];
    };

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
            Row& clr() { column[0].state = 0; column[1].state = 0; column[2].state = 0; return *this; }
        };

        Board() : playerIdx(0) {}
        explicit Board(const std::string& stateStr);
        explicit Board(const uint64_t& state);
        Row     rows[3];
        uint8_t playerIdx;

        std::string stateStr() const;
        Board& stateStr(const std::string &stateStr);

        uint64_t state() const;
        Board& state(const uint64_t& state);

        Field& at(uint8_t row, uint8_t col) { return rows[row][col]; }
        const Field& at(uint8_t row, uint8_t col) const { return rows[row][col]; }

        Row& operator[](uint8_t row) { return rows[row]; }
        const Row& operator[](uint8_t row) const { return rows[row]; }

        Board& set(uint8_t row, uint8_t col, const Field& field) { rows[row][col] = field; return *this; }
        Board& clr(uint8_t row, uint8_t col) { rows[row][col].state = 0; return *this; }
        Board& clr() { rows[0].clr(); rows[1].clr(); rows[2].clr(); return *this; }

        std::string toStr(bool showIndex=false) const;
        std::ostream& toStr(std::ostream& os, bool showIndex = false) const;

        size_t solved() const;
        size_t solved(const Color& color) const;

        // Number of pieces on board
        size_t num() const;

        // Number of pieces on board vertically at idx 0..2 (size)
        size_t num(uint8_t vIdx) const;

        // Number of pieces on board of color
        size_t num(const Color &c) const;

        // Number of pieces on board of color at idx 0..2 (size)
        size_t num(uint8_t vIdx,const Color& c) const;
    };
}

#endif