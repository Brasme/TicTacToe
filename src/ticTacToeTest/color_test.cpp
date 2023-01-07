#include "gtest/gtest.h"
#include "ticTacToe3D/color.h"

using namespace ttt;
TEST(color, init)
{
    Color c;
    EXPECT_EQ(c.idx, 0);
    EXPECT_EQ(c.toChar(), '-');

    c = Color(1);  EXPECT_EQ(c.idx, 1); EXPECT_EQ(c.toChar(), 'y');
    c = Color(2);  EXPECT_EQ(c.idx, 2); EXPECT_EQ(c.toChar(), 'g');
    c = Color(3);  EXPECT_EQ(c.idx, 3); EXPECT_EQ(c.toChar(), 'b');
    c = Color(4);  EXPECT_EQ(c.idx, 4); EXPECT_EQ(c.toChar(), 'r');
    c = Color(-1); EXPECT_EQ(c.idx, 0); EXPECT_EQ(c.toChar(), '-');
    c = Color(5);  EXPECT_EQ(c.idx, 0); EXPECT_EQ(c.toChar(), '-');

    c = ColorName::Yellow; EXPECT_EQ(c.idx, 1); EXPECT_EQ(c.toChar(), 'y');
    c = ColorName::Green;  EXPECT_EQ(c.idx, 2); EXPECT_EQ(c.toChar(), 'g');
    c = ColorName::Blue;   EXPECT_EQ(c.idx, 3); EXPECT_EQ(c.toChar(), 'b');
    c = ColorName::Red;    EXPECT_EQ(c.idx, 4); EXPECT_EQ(c.toChar(), 'r');
    c = ColorName::None;   EXPECT_EQ(c.idx, 0); EXPECT_EQ(c.toChar(), '-');

    c.idx = 0xff; EXPECT_EQ(c.idx, 0xff); EXPECT_EQ(c.toChar(), '-');
}

TEST(color, stream)
{
    std::stringstream ss;
    ss << ttt::Color(0) << ttt::Color(1) << ttt::Color(2) << ttt::Color(3) << ttt::Color(4) << ttt::Color(5);
    EXPECT_EQ(ss.str(), "-ygbr-");
}

TEST(colors, init) {
    ttt::Colors fc;
    EXPECT_EQ(fc.colors[0].idx, 0);
    EXPECT_EQ(fc.toStr(), "---");
}

TEST(colors, stream) {
    std::stringstream ss;
    ss << Colors(1, 2, 3) << '|' << Colors(ColorName::Red, ColorName::Green, ColorName::Blue);
    EXPECT_EQ(ss.str(), "ygb|rgb");
}

TEST(color, nameToIdx) {
    EXPECT_EQ(Color::nameToIdx(ColorName::None), 0);
    EXPECT_EQ(Color::nameToIdx(ColorName::Yellow), 1);
    EXPECT_EQ(Color::nameToIdx(ColorName::Green), 2);
    EXPECT_EQ(Color::nameToIdx(ColorName::Blue), 3);
    EXPECT_EQ(Color::nameToIdx(ColorName::Red), 4);
}