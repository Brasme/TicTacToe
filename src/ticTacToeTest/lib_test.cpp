#include "gtest/gtest.h"
#include "ticTacToe3D/lib.h"

TEST(lib,version) {
    EXPECT_STREQ(ttt::version(),"1.0.0");
}
