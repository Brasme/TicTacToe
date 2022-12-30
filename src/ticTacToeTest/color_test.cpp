#include "gtest/gtest.h"
#include "ticTacToe3D/color.h"

using namespace ttt;
TEST(color, init)
{
    Color c;
    EXPECT_EQ(c.idx, 0);
    EXPECT_EQ(c.ToChar(), '-');

    c = Color(1);  EXPECT_EQ(c.idx, 1); EXPECT_EQ(c.ToChar(), 'y');
    c = Color(2);  EXPECT_EQ(c.idx, 2); EXPECT_EQ(c.ToChar(), 'g');
    c = Color(3);  EXPECT_EQ(c.idx, 3); EXPECT_EQ(c.ToChar(), 'b');
    c = Color(4);  EXPECT_EQ(c.idx, 4); EXPECT_EQ(c.ToChar(), 'r');
    c = Color(-1); EXPECT_EQ(c.idx, 0); EXPECT_EQ(c.ToChar(), '-');
    c = Color(5);  EXPECT_EQ(c.idx, 0); EXPECT_EQ(c.ToChar(), '-');

    c = ColorName::Yellow; EXPECT_EQ(c.idx, 1); EXPECT_EQ(c.ToChar(), 'y');
    c = ColorName::Green;  EXPECT_EQ(c.idx, 2); EXPECT_EQ(c.ToChar(), 'g');
    c = ColorName::Blue;   EXPECT_EQ(c.idx, 3); EXPECT_EQ(c.ToChar(), 'b');
    c = ColorName::Red;    EXPECT_EQ(c.idx, 4); EXPECT_EQ(c.ToChar(), 'r');
    c = ColorName::None;   EXPECT_EQ(c.idx, 0); EXPECT_EQ(c.ToChar(), '-');

    c.idx = 0xff; EXPECT_EQ(c.idx, 0xff); EXPECT_EQ(c.ToChar(), '-');
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
    EXPECT_EQ(fc.ToStr(), "---");
}

TEST(colors, stream) {
    std::stringstream ss;
    ss << Colors(1, 2, 3) << '|' << Colors(ColorName::Red, ColorName::Green, ColorName::Blue);
    EXPECT_EQ(ss.str(), "ygb|rgb");
}

TEST(color, nameToIdx) {
    EXPECT_EQ(Color::NameToIdx(ColorName::None), 0);
    EXPECT_EQ(Color::NameToIdx(ColorName::Yellow), 1);
    EXPECT_EQ(Color::NameToIdx(ColorName::Green), 2);
    EXPECT_EQ(Color::NameToIdx(ColorName::Blue), 3);
    EXPECT_EQ(Color::NameToIdx(ColorName::Red), 4);
}