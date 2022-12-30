#pragma once

#if defined(_WIN32)
#  if !defined(ticTacToe3DLib_SHARED)
#    define TTT_API
#  elif defined(ticTacToe3DLib_EXPORTS)
#    define TTT_API __declspec(dllexport)
#  else
#    define TTT_API __declspec(dllimport)
#  endif
#else // non windows
#  define TTT_API
#endif

namespace ttt {
    TTT_API const char * version();
}