#pragma once

#if defined(_WIN32)
#  if defined(EXPORTING_ticTacToeLib)
#    define TTT_API __declspec(dllexport)
#  else
#    define TTT_API __declspec(dllimport)
#  endif
#else // non windows
#  define TTT_API
#endif

namespace ttt {
    const char * TTT_API version();
}