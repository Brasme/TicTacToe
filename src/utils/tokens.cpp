#include "utils/tokens.h"
#include "utils/string.h"

#include <stdio.h>
#include <algorithm>
#include <locale>
#include <sstream>


namespace utils {
    

    Tokens::Tokens(char delimiter) : delim(delimiter) {}
    Tokens::Tokens(const std::string& str, char delimiter) : delim(delimiter) { tokenize(str); }

    Tokens& Tokens::tokenize(const std::string& str)
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
    std::ostream& Tokens::toStream(std::ostream& os) const {
        os << '{';
        for (auto it = begin(); it != end(); ++it) {
            os << (it != begin() ? ",\"" : "\"") << *it << '\"';
        }
        os << '}';
        return os;
    }
    std::string Tokens::toStr() const {
        std::stringstream ss;
        toStream(ss);
        return ss.str();
    }
    Tokens& Tokens::toLower() {
        for (auto it = begin(); it != end(); ++it) {
            std::string& str = *it;
            std::transform(str.begin(), str.end(), str.begin(),
                [](unsigned char c) { return std::tolower(c); });
        }
        return *this;
    }
    Tokens& Tokens::toUpper() {
        for (auto it = begin(); it != end(); ++it) {
            std::string& str = *it;
            std::transform(str.begin(), str.end(), str.begin(),
                [](unsigned char c) { return std::toupper(c); });
        }
        return *this;
    }    
}

std::ostream& operator<<(std::ostream& os, const utils::Tokens& t) { return t.toStream(os); }
