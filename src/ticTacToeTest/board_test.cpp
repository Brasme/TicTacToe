#include "gtest/gtest.h"
#include "ticTacToe3D/board.h"

TEST(board,init) {
    ttt::Board b;
    EXPECT_EQ(b.state,0);
    EXPECT_EQ(b.ToStr(),
    "---,---,---\n"
    "---,---,---\n"
    "---,---,---\n");
}

TEST(board,set) {
    ttt::Board b;
    b.Set(0,0,ttt::FieldColors(1,2,3));
    EXPECT_EQ(b.state,0);
    EXPECT_EQ(b.ToStr(),
    "---,---,---\n"
    "---,---,---\n"
    "---,---,---\n");
}

