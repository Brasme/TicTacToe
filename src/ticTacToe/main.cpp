
#include <stdio.h>
#include <iostream>
#include <string>

#include "ticTacToe3D/board.h"
#include "ticTacToe3D/color.h"
#include "ticTacToe3D/field.h"

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
            inline Field ToField() const { return Field(VIdx(), ToColor()); }
            inline Color ToColor() const { return Color(pieceInfo & 0x7); }
            inline uint8_t VIdx() const { return (pieceInfo & 0x18) >> 3; }
            inline uint8_t Num() const { return (pieceInfo & 0xe0) >> 5; }
            inline bool Take() { 
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

    Player& GetPlayer() { return player[playerTurn]; }
    const Player& GetPlayer() const { return player[playerTurn]; }

    State& Clear(const Board& b,uint8_t turn = 0) {
        board = b;
        playerTurn = turn;
        for (uint8_t playerIdx = 0; playerIdx < 4; ++playerIdx) {
            Player& p = player[playerIdx];
            const Color color(playerIdx + 1);
            for (uint8_t vIdx = 0; vIdx < 3; ++vIdx) {
                const size_t n = board.Num(vIdx, color);
                p.piece[vIdx] = Player::Piece(color,vIdx,n > 3 ? 0 : (uint8_t)(3 - n));
            }
        }
        return *this;
    }

    State& Clear(uint8_t playerTurn = 0) {  return Clear(Board(), playerTurn); }

    State(uint8_t playerTurn = 0) { Clear(playerTurn); }
    State(const Board& board, uint8_t playerTurn = 0) { Clear(board, playerTurn); }
};


std::ostream& operator<<(std::ostream& os, const State::Player::Piece& piece) {
    os << (int)piece.Num() << "*\"" << piece.ToField() << '\"';
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
    state.board.ToStr(os,true);
    for (uint8_t pIdx = 0; pIdx < 4; ++pIdx) {
        os << "P#" << (int)pIdx << (pIdx == state.playerTurn ? " -> : " : "    : ");
        os << state.player[pIdx] << " : ";
        os << "#=" << state.board.Num(Color(1 + pIdx)) << ", solved=" << state.board.Solved(Color(1 + pIdx));
        os << "\n";
    }
    return os;
}

#include <vector>
#include <sstream>
#include <algorithm>
#include <locale>
#include "main.h"

namespace utils {
    std::string& ltrim(std::string& str)
    {
        auto it2 = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
        str.erase(str.begin(), it2);
        return str;
    }

    std::string& rtrim(std::string& str)
    {
        auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
        str.erase(it1.base(), str.end());
        return str;
    }

    std::string& trim(std::string& str)
    {
        size_t len1 = str.length();
        std::string::iterator i = std::unique(str.begin(), str.end(), [](auto lhs, auto rhs) { return lhs == rhs && lhs == ' '; });
        str.erase(i, str.end());
        ltrim(str);
        rtrim(str);
        return str;
    }

    struct Tokens : std::vector<std::string> {
        char delim;
        Tokens(char delimiter = ' ') : delim(delimiter) {}
        Tokens(const std::string &str,char delimiter = ' ') : delim(delimiter) { tokenize(str); }

        Tokens& tokenize(const std::string& str)
        {
            clear();
            std::string s = str;
            trim(s);
            std::stringstream ss(s);
            while (std::getline(ss, s, delim)) {
                push_back(s);
            }
            return *this;
        }
        std::ostream& toStream(std::ostream& os) const {
            os << '{';
            for (auto it = begin(); it != end(); ++it) {
                os << (it!=begin()? ",\"" : "\"") << *it << '\"';
            }
            os << '}';
            return os;
        }
        std::string toStr() const {
            std::stringstream ss;
            toStream(ss);
            return ss.str();
        }
        Tokens& toLower() {
            for (auto it = begin(); it != end(); ++it) {
                std::string& str = *it;
                std::transform(str.begin(), str.end(), str.begin(),
                    [](unsigned char c) { return std::tolower(c); });
            }
            return *this;
        }
        Tokens& toUpper() {
            for (auto it = begin(); it != end(); ++it) {
                std::string& str = *it;
                std::transform(str.begin(), str.end(), str.begin(),
                    [](unsigned char c) { return std::toupper(c); });
            }
            return *this;
        }
    };
}

std::ostream& operator<<(std::ostream& os, const utils::Tokens& t) { return t.toStream(os); }

std::ostream& help(std::ostream& os)
{
    os << 
        "Available commands:\n"
        "  q[uit] : Quit\n"
        "  u[ndo] : Undo last move\n"
        "  <c> <r> <p> : Set piece at column (<c>=0..2),row (<r>=0..2),piece(<p>=0..2)\n";
    return os;
}

#include <string.h>
#include <stdio.h>

extern "C" {
    /* Source: Linux source code : https://codebrowser.dev/glibc/glibc/posix/getopt.c.html
    *  Modifications: Bent Gramdal - Using a structure instead of globals form internal attributes
    */
    // Getopt for GNU.
    // Copyright (C) 1987-2022 Free Software Foundation, Inc.
    // This file is part of the GNU C Library and is also part of gnulib.
    // Patches to this file should be submitted to both projects.
    // The GNU C Library is free software; you can redistribute it and/or
    // modify it under the terms of the GNU Lesser General Public
    // License as published by the Free Software Foundation; either
    // version 2.1 of the License, or (at your option) any later version.
    // The GNU C Library is distributed in the hope that it will be useful,
    // but WITHOUT ANY WARRANTY; without even the implied warranty of
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    // Lesser General Public License for more details.
    // You should have received a copy of the GNU Lesser General Public
    // License along with the GNU C Library; if not, see
    // <https://www.gnu.org/licenses/>.  */
    
    struct getopt_t {
        int   argc;
        char **argv;

        int opterr;  /* printf of errors*/
        int optind;  /* index into parent argv vector */
        int optopt;  /* character checked for validity */
        int optreset; /* reset getopt */
        char *optarg; /* argument associated with option */
        const char *ostr;
        int BADCH;
        int BADARG;
        char *EMSG;
        char EMSGCHAR;

    };

    int getopt_init(struct getopt_t *opt,int argc,char *argv[],const char *ostr) {
        if (!opt || argc<=0 || !argv) 
            return -1;
        opt->argc=argc;
        opt->argv=argv;
        opt->opterr=1;
        opt->optind=1;
        opt->optopt=0;
        opt->optreset=0;
        opt->optarg=NULL;
        opt->ostr=ostr;
        opt->BADCH=(int)'?';
        opt->BADARG= (int)':';
        opt->EMSG=&opt->EMSGCHAR;
        opt->EMSGCHAR='\0';
        return 0;
    }

    int getopt_parse(struct getopt_t *opt)
    {
        if (!opt)
            return -1;
        static char *place = opt->EMSG;   /* option letter processing */
        const char *oli;                  /* option letter list index */

        if (opt->optreset || !*place) {              /* update scanning pointer */
            opt->optreset = 0;
            if (opt->optind >= opt->argc || *(place = opt->argv[opt->optind]) != '-') {
                place = opt->EMSG;
                return (-1);
            }
            if (place[1] && *++place == '-') {      /* found "--" */
                ++(opt->optind);
                place = opt->EMSG;
                return (-1);
            }
        }                                       /* option letter okay? */
        if ((opt->optopt = (int)*place++) == (int)':' ||
            !(oli = strchr(opt->ostr, opt->optopt))) { /* if the user didn't specify '-' as an option, assume it means -1. */
            if (opt->optopt == (int)'-')
                return (-1);
            if (!*place)
                ++(opt->optind);
            if (opt->opterr && *opt->ostr != ':')
                (void)printf("illegal option -- %c\n", opt->optopt);
                return (opt->BADCH);
        }
        if (*++oli != ':') {                    /* don't need argument */
            opt->optarg = NULL;
            if (!*place)
                ++(opt->optind);
        }
        else {                                  /* need an argument */
            if (*place)                     /* no white space */
                opt->optarg = place;
            else if (opt->argc <= ++(opt->optind)) {   /* no arg */
                place = opt->EMSG;
                if (*(opt->ostr) == ':')
                    return (opt->BADARG);
                if (opt->opterr)
                    (void)printf("option requires an argument -- %c\n", opt->optopt);
                return (opt->BADCH);
            }
            else /* white space */
                opt->optarg = opt->argv[opt->optind];
            place = opt->EMSG;
            ++(opt->optind);
        }
        return (opt->optopt); /* dump back option letter */
    }
}

int parse_args_getopt_example(int argc,char *argv[])
{
    struct getopt_t opt;    
    // put ':' at the starting of the string so compiler can distinguish between '?' and ':'
    if (getopt_init(&opt,argc,argv,":if:lrx")!=0)
        return -1; // Failed to init
    
    int option;
    while((option = getopt_parse(&opt)) != -1){ //get option from the getopt() method
      switch(option){
         //For option i, r, l, print that these are options
         case 'i':
         case 'l':
         case 'r':
            printf("Given Option: %c\n", option);
            break;
         case 'f': //here f is used for some file name
            printf("Given File: %s\n", opt.optarg);
            break;
         case ':':
            printf("option needs a value\n");
            break;
         case '?': //used for some unknown options
            printf("unknown option: %c\n", opt.optopt);
            break;
      }
   }
   for(; opt.optind < argc; opt.optind++){ //when some extra arguments are passed
      printf("Given extra arguments: %s\n", argv[opt.optind]);
   }
   return 0;
}

int main(int argc,char *argv[])
{
    parse_args_getopt_example(argc,argv);

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
                    ok = player.piece[piece].Num() > 0;
                    if (!ok) std::cout << "Error: Player has no more piece " << piece << "\n";
                }
                Field field;
                if (ok) {
                    field = state.board[row][col];
                    ok = field.Num(piece) == 0;
                    if (!ok) std::cout << "Error: There is already a piece on c/r = " << col << '/' << row << "\n";
                }
                if (ok) {
                    moves++;
                    
                    State &newState = states[moves];
                    newState = state;
                    newState.GetPlayer().piece[piece].Take();
                    newState.board.Set(row, col, field.Set(piece, newState.GetPlayer().piece[piece].ToColor()));
                    
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