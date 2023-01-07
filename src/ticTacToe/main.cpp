#include "main.h"

#include <stdio.h>
#include <iostream>
#include <string>

#include "ticTacToe3D/board.h"
#include "ticTacToe3D/color.h"
#include "ticTacToe3D/field.h"

#include "utils/getopt.h"
#include "utils/tokens.h"

using namespace ttt;

struct Turn {
    Turn(int idx=0): color(1 + idx & 0x3) {}
    int Idx() const { return color.idx - 1; }
    Color color;
    Turn& Next() { color.idx = 1+(color.idx +1)&0x3; }
};

struct State {
    Board   board; 
    struct Player {
        struct Piece {
            uint8_t pieceInfo; // bit0:2="Color.idx", bit3:4=vIdx (0..2), bit5:7=Num (0..7)
            
            Piece() : pieceInfo(0) {}
            Piece(const Color &c,uint8_t vIdx,uint8_t num) : pieceInfo(c.idx | (vIdx<<3) | (num<<5)) {}
            inline Field toField() const { return Field(vIdx(), toColor()); }
            inline Color toColor() const { return Color(pieceInfo & 0x7); }
            inline uint8_t vIdx() const { return (pieceInfo & 0x18) >> 3; }
            inline uint8_t num() const { return (pieceInfo & 0xe0) >> 5; }
            inline bool take() { 
                uint8_t num=(pieceInfo & 0xe0) >> 5;
                if (num == 0) return false;
                num--;
                pieceInfo = (pieceInfo & 0x1f) | (num << 5);
                return true;
            }
        };
        Piece piece[3];
    } player[4];

    uint8_t playerTurn;    

    Player& getPlayer() { return player[playerTurn]; }
    const Player& getPlayer() const { return player[playerTurn]; }

    State& clear(const Board& b,uint8_t turn = 0) {
        board = b;
        playerTurn = turn;
        for (uint8_t playerIdx = 0; playerIdx < 4; ++playerIdx) {
            Player& p = player[playerIdx];
            const Color color(playerIdx + 1);
            for (uint8_t vIdx = 0; vIdx < 3; ++vIdx) {
                const size_t n = board.num(vIdx, color);
                p.piece[vIdx] = Player::Piece(color,vIdx,n > 3 ? 0 : (uint8_t)(3 - n));
            }
        }
        return *this;
    }

    State& clear(uint8_t playerTurn = 0) {  return clear(Board(), playerTurn); }

    State(uint8_t playerTurn = 0) { clear(playerTurn); }
    State(const Board& board, uint8_t playerTurn = 0) { clear(board, playerTurn); }
};


std::ostream& operator<<(std::ostream& os, const State::Player::Piece& piece) {
    os << (int)piece.num() << "*\"" << piece.toField() << '\"';
    return os;
}

std::ostream& operator<<(std::ostream& os, const State::Player& player) {
    for (uint8_t i = 0; i < 3; ++i) {
        os << (i > 0?",  ":"  ") << (int)i << ": " << player.piece[i];
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const State& state)
{
    state.board.toStr(os,true);
    for (uint8_t pIdx = 0; pIdx < 4; ++pIdx) {
        os << "P#" << (int)pIdx << (pIdx == state.playerTurn ? " -> : " : "    : ");
        os << state.player[pIdx] << " : ";
        os << "#=" << state.board.num(Color(1 + pIdx)) << ", solved=" << state.board.solved(Color(1 + pIdx));
        os << "\n";
    }
    return os;
}


std::ostream& help(std::ostream& os)
{
    os << 
        "Available commands:\n"
        "  q[uit] : Quit\n"
        "  u[ndo] : Undo last move\n"
        "  <c> <r> <p> : Set piece at column (<c>=0..2),row (<r>=0..2),piece(<p>=0..2)\n";
    return os;
}

#include "utils/getopt.h"


int main(int argc,char *argv[])
{
    parse_args_getopt_example_c(argc,argv);
    parse_args_getopt_example_cpp(argc, argv);

    std::cout << "TicTacToe3D\n";

    bool done = 0;
    size_t moves = 0;
    State states[28];

    while (!done) {
        bool ok = true;
        State& state = states[moves];
        State::Player& player = state.player[state.playerTurn];

        std::cout << "--------------------------------\nMoves: " << moves << "\n" << state;
        std::cout << "%> ";
        std::string line;
        utils::Tokens tokens;
        if (!std::getline(std::cin, line)) done = 1;
        else if (!tokens.tokenize(line).toLower().empty()) {
            switch (tokens[0][0]) {
            case 'q': done = 1; break;
            case 'u': 
                if (moves == 0) {
                    std::cout << "Error: No more undo\n";
                } else {
                    std::cout << "=> Undo\n";
                    moves--;
                }
                break;
            case 'h': help(std::cout); break;
            case '0':
            case '1':
            case '2':
            {
                int row = 0, col = 0, piece = 0;
                
                if (tokens.size() >= 3) {
                    try {
                        col = std::stoi(tokens[0]);
                        row = std::stoi(tokens[1]);
                        piece = std::stoi(tokens[2]);
                        ok = (col >= 0 && col < 3) && (row >= 0 && row < 3) && (piece >= 0 && piece < 3);
                        if (!ok) std::cout << "Error: All numbers must be 0..2\n";
                    }
                    catch (const std::invalid_argument&) {
                        std::cout << "Error: Set need numbers 0..2 for all args\n";
                        ok = false;
                    }
                }
                else {
                    std::cout << "Error: Set need numbers 0..2 for <c> <r> <p> (3 numbers)\n";
                    ok = false;
                }
                if (ok) {
                    ok = player.piece[piece].num() > 0;
                    if (!ok) std::cout << "Error: Player has no more piece " << piece << "\n";
                }
                Field field;
                if (ok) {
                    field = state.board[row][col];
                    ok = field.num(piece) == 0;
                    if (!ok) std::cout << "Error: There is already a piece on c/r = " << col << '/' << row << "\n";
                }
                if (ok) {
                    moves++;
                    
                    State &newState = states[moves];
                    newState = state;
                    newState.getPlayer().piece[piece].take();
                    // newState.board.set(row, col, field.set(piece, newState.getPlayer().piece[piece].toColor()));
                    newState.board.set(row, col, player.piece[piece].toField());
                    
                    newState.playerTurn = (state.playerTurn + 1) % 4;
                    
                    std::cout << "Ok: Set c/r/p " << col << '/' << row << '/' << piece << "\n";
                }
            }
                break;
            default:
                std::cout << "Unknown command cmd=" << tokens << "\n";
            }
        }
        if (!ok) {
            if (!tokens.empty()) std::cout << "cmd=" << tokens;
            help(std::cout);
        }
    }
    return 0;
}