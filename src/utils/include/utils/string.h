#ifndef UTILS_STRING_H_
#define UTILS_STRING_H_

#include "utils/lib.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
} // extern "C"	

#include <string>

namespace utils {
    UTILS_API std::string& ltrim(std::string& str);

    UTILS_API std::string& rtrim(std::string& str);

    UTILS_API std::string& trim(std::string& str);

}


#endif // __cplusplus


#endif
