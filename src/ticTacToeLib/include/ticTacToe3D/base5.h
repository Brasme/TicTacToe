#ifndef TTT_BASE5_H_
#define TTT_BASE5_H_

#include "ticTacToe3D/lib.h"

namespace ttt {
    struct TTT_API IDecimals;
    struct TTT_API DecimalsUInt8b5;
    struct TTT_API UInt8b5;
}

#include <stdint.h>
#include <string>
#include <ostream>

namespace ttt {
    struct TTT_API IDecimals {
        virtual const uint8_t *decimals() const = 0;
        virtual size_t num() const = 0;
        virtual size_t base() const = 0;
        uint8_t operator[](size_t idx) const { return decimals()[idx]; }
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const IDecimals& d);

    struct TTT_API DecimalsUInt8b5 : IDecimals {
        DecimalsUInt8b5() : v{0,0,0,0} {}
        DecimalsUInt8b5(const uint8_t(&d)[4]) : v{d[0],d[1],d[2],d[3]} {}
        uint8_t v[4];
        const uint8_t *decimals() const override { return v; }
        size_t num() const override { return 4; }
        size_t base() const override { return 5; }
    };    

    struct TTT_API UInt8b5 {
        uint8_t value;
        UInt8b5() : value(0) {}
        UInt8b5(const uint8_t &v) : value(v) {}   // Accept as non-explicit cast conversion
        UInt8b5(const IDecimals &d) : value(0) {  // Accept as non-explicit cast conversion
            setDecimals(d);
        }
        
        operator uint8_t() const { return value; } // Accept as non-explicit cast conversion
        bool operator ==(const UInt8b5& o) const { return value==value; }
        bool operator !=(const UInt8b5& o) const { return value!=value; }
        
        const DecimalsUInt8b5& getDecimals() const;
        std::string ToStr() const;

        UInt8b5& setDecimal(size_t idx, const uint8_t& v);
        UInt8b5& setDecimals(const IDecimals &d) {  // Accept as non-explicit cast conversion
            value=0;
            size_t i=d.num();
            const uint8_t *dd=d.decimals();
            while (i>0) {
                i--;
                value=value*5+V(dd[i]);
            }            
            return *this;
        }
        
        constexpr uint8_t V(uint8_t i) { return i >= 5 ? 0 : i; }        
    };
    TTT_API std::ostream& operator<<(std::ostream& os,const UInt8b5& c);
}

#endif
