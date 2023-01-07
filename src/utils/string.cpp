#include "utils/string.h"

#include <stdio.h>
#include <algorithm>
#include <locale>
#include <sstream>

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

}
