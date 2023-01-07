#ifndef UTILS_TOKENS_H_
#define UTILS_TOKENS_H_

#include "utils/lib.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} // extern "C"	

#include <string>
#include <ostream>
#include <vector>

namespace utils {
    UTILS_API std::string& ltrim(std::string& str);

    UTILS_API std::string& rtrim(std::string& str);

    UTILS_API std::string& trim(std::string& str);

    struct UTILS_API Tokens : std::vector<std::string> {
        char delim;
        Tokens(char delimiter = ' ');        
        Tokens(const std::string& str, char delimiter = ' ');
        
        Tokens& tokenize(const std::string& str);

        std::ostream& toStream(std::ostream& os) const;
        std::string toStr() const;
        Tokens& toLower();
        Tokens& toUpper();
    };
}

UTILS_API std::ostream& operator<<(std::ostream& os, const utils::Tokens& t);

#endif // __cplusplus


#endif
