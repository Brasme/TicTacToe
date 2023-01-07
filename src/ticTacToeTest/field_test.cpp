#include "gtest/gtest.h"
#include "ticTacToe3D/field.h"

#include <sstream>

using namespace ttt;
TEST(field, init) {
    Field f;
    EXPECT_EQ(f.state, 0);
    EXPECT_EQ(f.toStr(), "---");
}

TEST(field, construct)
{
    Field f(1, 2, 3);
    EXPECT_EQ(f.state, 1 + 2 * 5 + 3 * 25);
    EXPECT_EQ(f.toStr(), "ygb");

    f = Field(2, 3, 4);
    EXPECT_EQ(f.state, 2 + 3 * 5 + 4 * 25);
    EXPECT_EQ(f.toStr(), "gbr");

    Field f0 = Field(0,ColorName::Blue);
    EXPECT_EQ(f0.toStr(), "b--");

    Field f1 = Field(1,ColorName::Red);
    EXPECT_EQ(f1.toStr(), "-r-");

    Field f2 = Field(2,ColorName::Green);
    EXPECT_EQ(f2.toStr(), "--g");

    f.add(f1).add(f2);
    EXPECT_EQ(f.toStr(), "grg");
}

TEST(field, stream)
{
    Field f(ColorName::Red, ColorName::Green, ColorName::Blue);
    std::stringstream ss;
    ss << f;
    EXPECT_EQ(ss.str(), "rgb");
}

TEST(field,set) 
{
    Field f;
    f = Field(ColorName::Red, ColorName::Green, ColorName::Blue);
    EXPECT_EQ(f.state, 4 + 2 * 5 + 3 * 25);
    EXPECT_EQ(f.toStr(), "rgb");
}

