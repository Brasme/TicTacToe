#include "gtest/gtest.h"
#include "ticTacToe3D/board.h"

TEST(board,init) {
    ttt::Board b;
    EXPECT_EQ(b.state,0);
}
