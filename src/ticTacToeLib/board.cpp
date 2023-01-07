#include "ticTacToe3D/board.h"
#include "ticTacToe3D/field.h"
#include "ticTacToe3D/color.h"
#include "ticTacToe3D/field.h"

#include <chrono>  // chrono::system_clock
#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string
#include <time.h>

namespace ttt {

    Board::Board(const std::string& state) : playerIdx(0) {
        stateStr(state);
    }

    Board::Board(const uint64_t& s): playerIdx(0)
    {
        state(s);
    }
    
    Board& Board::stateStr(const std::string& state) {
        
        std::size_t pos = state.find(",P:");
        if (pos != std::string::npos) {
            std::size_t end = state.find(",",pos+3);
            if (end != std::string::npos) {
                std::string player = state.substr(pos + 3, end - pos - 3);
                playerIdx = std::stoi(player);
            }
        }
        
        pos = state.find(",L:");
        if (pos != std::string::npos) {
            std::size_t end = state.find(",", pos + 3);
            std::string board = state.substr(pos + 3, end - pos - 3);
            
            uint8_t field = 0;
            pos = board.find('{');
            while (pos != std::string::npos && field<9) {
                pos = board.find('{',pos+1);
                end = board.find('}',pos+1);
                if (pos != std::string::npos && end != std::string::npos) {
                    std::string fieldStr = board.substr(pos + 1, end - pos - 1);
                    rows[field / 3][field % 3] = Field(Colors(fieldStr));
                }
            }
            uint8_t row = 0;

        }
        return *this;
    }

    std::string Board::stateStr() const
    {
        auto now = std::chrono::system_clock::now();
        auto t = std::chrono::system_clock::to_time_t(now);

        struct tm tmBuf;
        localtime_s(&tmBuf, &t);
        std::stringstream ss;
        ss << "B";
        ss << ",T:" << std::put_time(&tmBuf, "%Y-%m-%d %X");
        ss << ",P:" << (int)playerIdx;
        
        ss << ",L:";
        for (uint8_t row = 0; row < 3; ++row) {
            ss << (row == 0 ? '{' : ',');
            const Row& r = rows[row];
            ss << '{' << r[0] << ',' << r[1] << ',' << r[1] << '}';
        }
        ss << '}';
        return ss.str();
    }

    uint64_t Board::state() const
    {
        uint64_t s = 0;
        uint8_t idx = 9;
        while (idx > 0) {
            idx--;
            const Field& f = rows[idx/3][idx%3];
            // s = s * 125ull + (uint64_t)f.state;            
            s = s<<7 | (uint64_t)f.state;
        }
        return s;
    }

    Board& Board::state(const uint64_t &state)
    {
        uint64_t s = state;
        for (uint8_t idx = 0; idx < 9;++idx) {
            Field& f = rows[idx / 3][idx % 3];
            // uint64_t ns = s / 125ull;
            // f.state = (uint8_t)(s - ns * 125ull);
            // s = ns; 
            f.state = (uint8_t)(s & 0x7f);
            s = s >> 7;                        
        }
        return *this;
    }

    std::string Board::toStr(bool showIndex) const
    {
        std::stringstream ss;
        toStr(ss,showIndex);
        return ss.str();
    }

    std::ostream& Board::toStr(std::ostream& os,bool showIndex) const 
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
            os << r[0] << ',' << r[1] << ',' << r[2] << "\n";
        }
        return os;
    }

    size_t Board::solved() const
    {
        return solved(ColorName::Yellow)+ solved(ColorName::Green)+ solved(ColorName::Blue)+ solved(ColorName::Red);
         
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

    size_t Board::solved(const Color& color) const
    {
        const uint8_t i = color.idx;
        if (i == 0 || i > 4)
            return 0;

        size_t n=0;

        // Check along columns
        for (uint8_t row = 0; row < 3; ++row) {
            n += numSolved(rows[row][0].toColors(), rows[row][1].toColors(), rows[row][2].toColors(), i);
        };
        
        // Check along rows
        for (uint8_t col = 0; col < 3; ++col) {
            n += numSolved(rows[0][col].toColors(), rows[1][col].toColors(), rows[2][col].toColors(), i);
        };

        // Check vertically
        for (uint8_t row = 0; row < 3; ++row) {
            for (uint8_t col = 0; col < 3; ++col) {
                n += numSolved(rows[row][col].toColors(), i);
            }
        }

        // Check along diagonals
        n += numSolved(rows[0][0].toColors(), rows[1][1].toColors(), rows[2][2].toColors(), i);
        n += numSolved(rows[2][0].toColors(), rows[1][1].toColors(), rows[0][2].toColors(), i);
        
        return n;
    }
    
    
    size_t Board::num() const
    {
        size_t n = 0;
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row]; 
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.num();
            }
        }            
        return n;
    }

    size_t Board::num(uint8_t vIdx) const
    {
        if (vIdx > 2)
            return 0;
        size_t n = 0;        
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row];
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.num(vIdx);
            }
        }

        return n;
    }

    size_t Board::num(const Color& c) const
    {
        if (c.idx > 4)
            return 0;
        size_t n = 0;        
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row];
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.num(c);
            }
        }
        return n;
    }

    size_t Board::num(uint8_t vIdx,const Color& c) const
    {
        if (c.idx > 4)
            return 0;
        size_t n = 0;        
        for (uint8_t row = 0; row < 3; ++row) {
            const Row& r = rows[row];
            for (uint8_t col = 0; col < 3; ++col) {
                const Field& f = r[col];
                n += f.num(vIdx,c);
            }
        }
        return n;
    }
}
