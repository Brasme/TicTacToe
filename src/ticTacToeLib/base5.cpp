#include "ticTacToe3D/base5.h"
#include <sstream>

namespace ttt {
    typedef DecimalsUInt8b5 Decimals4;
    static const Decimals4 uint8ToUint8b5decimals[256] = {
        Decimals4({0,0,0,0}),Decimals4({0,0,0,1}),Decimals4({0,0,0,2}),Decimals4({0,0,0,3}),Decimals4({0,0,0,4}),Decimals4({0,0,1,0}),Decimals4({0,0,1,1}),Decimals4({0,0,1,2}),
        Decimals4({0,0,1,3}),Decimals4({0,0,1,4}),Decimals4({0,0,2,0}),Decimals4({0,0,2,1}),Decimals4({0,0,2,2}),Decimals4({0,0,2,3}),Decimals4({0,0,2,4}),Decimals4({0,0,3,0}),
        Decimals4({0,0,3,1}),Decimals4({0,0,3,2}),Decimals4({0,0,3,3}),Decimals4({0,0,3,4}),Decimals4({0,0,4,0}),Decimals4({0,0,4,1}),Decimals4({0,0,4,2}),Decimals4({0,0,4,3}),
        Decimals4({0,0,4,4}),Decimals4({0,1,0,0}),Decimals4({0,1,0,1}),Decimals4({0,1,0,2}),Decimals4({0,1,0,3}),Decimals4({0,1,0,4}),Decimals4({0,1,1,0}),Decimals4({0,1,1,1}),
        Decimals4({0,1,1,2}),Decimals4({0,1,1,3}),Decimals4({0,1,1,4}),Decimals4({0,1,2,0}),Decimals4({0,1,2,1}),Decimals4({0,1,2,2}),Decimals4({0,1,2,3}),Decimals4({0,1,2,4}),
        Decimals4({0,1,3,0}),Decimals4({0,1,3,1}),Decimals4({0,1,3,2}),Decimals4({0,1,3,3}),Decimals4({0,1,3,4}),Decimals4({0,1,4,0}),Decimals4({0,1,4,1}),Decimals4({0,1,4,2}),
        Decimals4({0,1,4,3}),Decimals4({0,1,4,4}),Decimals4({0,2,0,0}),Decimals4({0,2,0,1}),Decimals4({0,2,0,2}),Decimals4({0,2,0,3}),Decimals4({0,2,0,4}),Decimals4({0,2,1,0}),
        Decimals4({0,2,1,1}),Decimals4({0,2,1,2}),Decimals4({0,2,1,3}),Decimals4({0,2,1,4}),Decimals4({0,2,2,0}),Decimals4({0,2,2,1}),Decimals4({0,2,2,2}),Decimals4({0,2,2,3}),
        Decimals4({0,2,2,4}),Decimals4({0,2,3,0}),Decimals4({0,2,3,1}),Decimals4({0,2,3,2}),Decimals4({0,2,3,3}),Decimals4({0,2,3,4}),Decimals4({0,2,4,0}),Decimals4({0,2,4,1}),
        Decimals4({0,2,4,2}),Decimals4({0,2,4,3}),Decimals4({0,2,4,4}),Decimals4({0,3,0,0}),Decimals4({0,3,0,1}),Decimals4({0,3,0,2}),Decimals4({0,3,0,3}),Decimals4({0,3,0,4}),
        Decimals4({0,3,1,0}),Decimals4({0,3,1,1}),Decimals4({0,3,1,2}),Decimals4({0,3,1,3}),Decimals4({0,3,1,4}),Decimals4({0,3,2,0}),Decimals4({0,3,2,1}),Decimals4({0,3,2,2}),
        Decimals4({0,3,2,3}),Decimals4({0,3,2,4}),Decimals4({0,3,3,0}),Decimals4({0,3,3,1}),Decimals4({0,3,3,2}),Decimals4({0,3,3,3}),Decimals4({0,3,3,4}),Decimals4({0,3,4,0}),
        Decimals4({0,3,4,1}),Decimals4({0,3,4,2}),Decimals4({0,3,4,3}),Decimals4({0,3,4,4}),Decimals4({0,4,0,0}),Decimals4({0,4,0,1}),Decimals4({0,4,0,2}),Decimals4({0,4,0,3}),
        Decimals4({0,4,0,4}),Decimals4({0,4,1,0}),Decimals4({0,4,1,1}),Decimals4({0,4,1,2}),Decimals4({0,4,1,3}),Decimals4({0,4,1,4}),Decimals4({0,4,2,0}),Decimals4({0,4,2,1}),
        Decimals4({0,4,2,2}),Decimals4({0,4,2,3}),Decimals4({0,4,2,4}),Decimals4({0,4,3,0}),Decimals4({0,4,3,1}),Decimals4({0,4,3,2}),Decimals4({0,4,3,3}),Decimals4({0,4,3,4}),
        Decimals4({0,4,4,0}),Decimals4({0,4,4,1}),Decimals4({0,4,4,2}),Decimals4({0,4,4,3}),Decimals4({0,4,4,4}),Decimals4({1,0,0,0}),Decimals4({1,0,0,1}),Decimals4({1,0,0,2}),
        Decimals4({1,0,0,3}),Decimals4({1,0,0,4}),Decimals4({1,0,1,0}),Decimals4({1,0,1,1}),Decimals4({1,0,1,2}),Decimals4({1,0,1,3}),Decimals4({1,0,1,4}),Decimals4({1,0,2,0}),
        Decimals4({1,0,2,1}),Decimals4({1,0,2,2}),Decimals4({1,0,2,3}),Decimals4({1,0,2,4}),Decimals4({1,0,3,0}),Decimals4({1,0,3,1}),Decimals4({1,0,3,2}),Decimals4({1,0,3,3}),
        Decimals4({1,0,3,4}),Decimals4({1,0,4,0}),Decimals4({1,0,4,1}),Decimals4({1,0,4,2}),Decimals4({1,0,4,3}),Decimals4({1,0,4,4}),Decimals4({1,1,0,0}),Decimals4({1,1,0,1}),
        Decimals4({1,1,0,2}),Decimals4({1,1,0,3}),Decimals4({1,1,0,4}),Decimals4({1,1,1,0}),Decimals4({1,1,1,1}),Decimals4({1,1,1,2}),Decimals4({1,1,1,3}),Decimals4({1,1,1,4}),
        Decimals4({1,1,2,0}),Decimals4({1,1,2,1}),Decimals4({1,1,2,2}),Decimals4({1,1,2,3}),Decimals4({1,1,2,4}),Decimals4({1,1,3,0}),Decimals4({1,1,3,1}),Decimals4({1,1,3,2}),
        Decimals4({1,1,3,3}),Decimals4({1,1,3,4}),Decimals4({1,1,4,0}),Decimals4({1,1,4,1}),Decimals4({1,1,4,2}),Decimals4({1,1,4,3}),Decimals4({1,1,4,4}),Decimals4({1,2,0,0}),
        Decimals4({1,2,0,1}),Decimals4({1,2,0,2}),Decimals4({1,2,0,3}),Decimals4({1,2,0,4}),Decimals4({1,2,1,0}),Decimals4({1,2,1,1}),Decimals4({1,2,1,2}),Decimals4({1,2,1,3}),
        Decimals4({1,2,1,4}),Decimals4({1,2,2,0}),Decimals4({1,2,2,1}),Decimals4({1,2,2,2}),Decimals4({1,2,2,3}),Decimals4({1,2,2,4}),Decimals4({1,2,3,0}),Decimals4({1,2,3,1}),
        Decimals4({1,2,3,2}),Decimals4({1,2,3,3}),Decimals4({1,2,3,4}),Decimals4({1,2,4,0}),Decimals4({1,2,4,1}),Decimals4({1,2,4,2}),Decimals4({1,2,4,3}),Decimals4({1,2,4,4}),
        Decimals4({1,3,0,0}),Decimals4({1,3,0,1}),Decimals4({1,3,0,2}),Decimals4({1,3,0,3}),Decimals4({1,3,0,4}),Decimals4({1,3,1,0}),Decimals4({1,3,1,1}),Decimals4({1,3,1,2}),
        Decimals4({1,3,1,3}),Decimals4({1,3,1,4}),Decimals4({1,3,2,0}),Decimals4({1,3,2,1}),Decimals4({1,3,2,2}),Decimals4({1,3,2,3}),Decimals4({1,3,2,4}),Decimals4({1,3,3,0}),
        Decimals4({1,3,3,1}),Decimals4({1,3,3,2}),Decimals4({1,3,3,3}),Decimals4({1,3,3,4}),Decimals4({1,3,4,0}),Decimals4({1,3,4,1}),Decimals4({1,3,4,2}),Decimals4({1,3,4,3}),
        Decimals4({1,3,4,4}),Decimals4({1,4,0,0}),Decimals4({1,4,0,1}),Decimals4({1,4,0,2}),Decimals4({1,4,0,3}),Decimals4({1,4,0,4}),Decimals4({1,4,1,0}),Decimals4({1,4,1,1}),
        Decimals4({1,4,1,2}),Decimals4({1,4,1,3}),Decimals4({1,4,1,4}),Decimals4({1,4,2,0}),Decimals4({1,4,2,1}),Decimals4({1,4,2,2}),Decimals4({1,4,2,3}),Decimals4({1,4,2,4}),
        Decimals4({1,4,3,0}),Decimals4({1,4,3,1}),Decimals4({1,4,3,2}),Decimals4({1,4,3,3}),Decimals4({1,4,3,4}),Decimals4({1,4,4,0}),Decimals4({1,4,4,1}),Decimals4({1,4,4,2}),
        Decimals4({1,4,4,3}),Decimals4({1,4,4,4}),Decimals4({2,0,0,0}),Decimals4({2,0,0,1}),Decimals4({2,0,0,2}),Decimals4({2,0,0,3}),Decimals4({2,0,0,4}),Decimals4({2,0,1,0})
    };


    std::ostream& operator<<(std::ostream& os,const IDecimals& d)
    {
        static const char *v2c="0123456789abcdefghijhlmnopqrstuv";
        const uint8_t *dd=d.decimals();
        const size_t num=d.num();
        for (size_t n=0;n<num;++n) {
            os << v2c[dd[n]&0x1f];
        }
        os << ":b" << (int)d.base();
        return os;
    }

    std::ostream &operator<<(std::ostream &os, const UInt8b5 &c)
    {
        os << c.getDecimals();
        return os;
    }
    
    const DecimalsUInt8b5 &UInt8b5::getDecimals() const
    {
        return uint8ToUint8b5decimals[value];
    }

    std::string UInt8b5::ToStr() const { 
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }

    UInt8b5& UInt8b5::setDecimal(size_t idx, const uint8_t& v)
    {
        if (idx>=4) return *this;
        Decimals4 d=getDecimals();
        d.v[3-idx]=V(v);
        value=0;
        size_t i=4;
        while (i>0) {
            i--;
            value=value*5+d.v[i];
        }
        return *this;
    }
        
}
