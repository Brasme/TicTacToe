#pragma once

#if defined(_WIN32)
#  if !defined(utils_SHARED)
#    define UTILS_API
#  elif defined(utils_EXPORTS)
#    define UTILS_API __declspec(dllexport)
#  else
#    define UTILS_API __declspec(dllimport)
#  endif
#else // non windows
#  define UTILS_API
#endif

namespace utils {
    UTILS_API const char *version();
}