#include "gtest/gtest.h"
#include "ticTacToe3D/board.h"
#include "ticTacToe3D/field.h"
#include "ticTacToe3D/color.h"

using namespace ttt;
TEST(board,init) {
    Board b;
    EXPECT_EQ(b.state,0);
    EXPECT_EQ(b.ToStr(),
    "---,---,---\n"
    "---,---,---\n"
    "---,---,---\n");
}

static std::string ToHexStr(uint64_t v) {
    std::stringstream ss;
    ss << "0x";
    ss << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>((v >> 32) & 0xffffffffu);
    ss << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(v & 0xffffffffu);
    return ss.str();
}
TEST(board, set) {
    Board b;
    Field f00(ColorName::Red, ColorName::Blue, ColorName::Yellow);
    b.Set(0, 0, f00);
    EXPECT_EQ(b.state, 44ull);
    EXPECT_EQ(b.ToStr(),
        "rby,---,---\n"
        "---,---,---\n"
        "---,---,---\n");

    const Field fYGB(ColorName::Yellow, ColorName::Green, ColorName::Blue);
    const Field f_GB(ColorName::None, ColorName::Green, ColorName::Blue);
    b.Set(1, 1, fYGB).Set(2, 0, f_GB);
    uint64_t expectedState = 44ull + ((uint64_t)fYGB.state << (7 * 4)) + ((uint64_t)f_GB.state << (7 * 6));
    EXPECT_EQ(b.state, expectedState);
    EXPECT_EQ(b.ToStr(),
        "rby,---,---\n"
        "---,ygb,---\n"
        "-gb,---,---\n");

    EXPECT_EQ(b.Get(1, 1).state, 1 + 2 * 5 + 3 * 25);
    EXPECT_EQ(b.Get(1, 1), fYGB);

    const Field fYYY(1, 1, 1);
    EXPECT_EQ(fYYY.ToStr(), "yyy");
    b.Set(2, 2, fYYY).Set(1, 2, b.Get(1, 1)).Clr(1, 1);

    expectedState = 44ull + ((uint64_t)fYGB.state << (7 * 5)) + ((uint64_t)f_GB.state << (7 * 6)) + ((uint64_t)(1 + 5 + 25) << (7 * 8));
    EXPECT_EQ(b.state, expectedState);
    EXPECT_EQ(b.ToStr(),
        "rby,---,---\n"
        "---,---,ygb\n"
        "-gb,---,yyy\n");

    EXPECT_EQ(ToHexStr(b.state), "0x1f0156b00000002c");
    EXPECT_EQ(b.state, 0x1f0156b00000002cull);
}

TEST(board, solved) 
{
    Board b(0x1f0156b00000002cull);

    EXPECT_EQ(b.Solved(), 1);
    EXPECT_EQ(b.Solved(ColorName::Yellow), 1);
    
    b.Set(1, 1, Colors(0, 1, 0));
    EXPECT_EQ(b.ToStr(),
        "rby,---,---\n"
        "---,-y-,ygb\n"
        "-gb,---,yyy\n");
    EXPECT_EQ(b.Solved(), 2);
    EXPECT_EQ(b.Solved(ColorName::Yellow), 2);

    b.Set(1, 1, Colors(0, 1, 0));
    EXPECT_EQ(b.ToStr(),
        "rby,---,---\n"
        "---,-y-,ygb\n"
        "-gb,---,yyy\n");
    EXPECT_EQ(b.Solved(), 2);
    EXPECT_EQ(b.Solved(ColorName::Yellow), 2);

    b.Clr();
    EXPECT_EQ(b.Solved(), 0);
    EXPECT_EQ(b.Solved(ColorName::Green), 0);
    
    const Colors cGGG(ColorName::Green, ColorName::Green, ColorName::Green);
    for (uint8_t col = 0; col < 3; ++col)
        b.Set(0, col, cGGG);
    EXPECT_EQ(b.Solved(), 8);
    EXPECT_EQ(b.Solved(ColorName::Yellow), 0);
    EXPECT_EQ(b.Solved(ColorName::Green), 8);
    EXPECT_EQ(b.Solved(ColorName::Blue), 0);
    EXPECT_EQ(b.Solved(ColorName::Red), 0);
    EXPECT_EQ(b.ToStr(),
        "ggg,ggg,ggg\n"
        "---,---,---\n"
        "---,---,---\n");

    for (uint8_t col = 0; col < 3; ++col)
        b.Set(1, col, cGGG);
    EXPECT_EQ(b.Solved(), 16);
    EXPECT_EQ(b.Solved(ColorName::Yellow), 0);
    EXPECT_EQ(b.Solved(ColorName::Green), 16);
    EXPECT_EQ(b.Solved(ColorName::Blue), 0);
    EXPECT_EQ(b.Solved(ColorName::Red), 0);
    EXPECT_EQ(b.ToStr(),        
        "ggg,ggg,ggg\n"
        "ggg,ggg,ggg\n"
        "---,---,---\n");

    for (uint8_t col = 0; col < 3; ++col)
        b.Set(2, col, cGGG);
    EXPECT_EQ(b.Solved(), 24 +5*5);
    EXPECT_EQ(b.Solved(ColorName::Yellow), 0);
    EXPECT_EQ(b.Solved(ColorName::Green), 49);
    EXPECT_EQ(b.Solved(ColorName::Blue), 0);
    EXPECT_EQ(b.Solved(ColorName::Red), 0);
    EXPECT_EQ(b.ToStr(),
        "ggg,ggg,ggg\n"
        "ggg,ggg,ggg\n"
        "ggg,ggg,ggg\n");

}

TEST(board, num)
{
    Board b(0x1f0156b00000002cull);
    EXPECT_EQ(b.ToStr(),
        "rby,---,---\n"
        "---,---,ygb\n"
        "-gb,---,yyy\n");

    EXPECT_EQ(b.Num(), 11);
    EXPECT_EQ(b.Num(0), 3); // 3 at level 0
    EXPECT_EQ(b.Num(1), 4); // 4 at level 1
    EXPECT_EQ(b.Num(2), 4); // 4 at level 2
    EXPECT_EQ(b.Num(ColorName::Yellow), 5); // total 5 yellow placed
    EXPECT_EQ(b.Num(0, ColorName::Yellow), 2); // total 5 yellow placed at level 0
    EXPECT_EQ(b.Num(1, ColorName::Yellow), 1); // total 5 yellow placed at level 1
    EXPECT_EQ(b.Num(2, ColorName::Yellow), 2); // total 5 yellow placed at level 2
    EXPECT_EQ(b.Num(ColorName::None), 16); // 16 left
    EXPECT_EQ(b.Num(0, ColorName::None), 6); // 6 left at level 0
    EXPECT_EQ(b.Num(1, Color()), 5); // 5 left at level 1
    EXPECT_EQ(b.Num(2, Color()), 5); // 5 left at level 2

    b.Clr();
    const Colors cRGB(ColorName::Red,  ColorName::Green, ColorName::Blue);
    const Colors cGBY(ColorName::Green,ColorName::Blue,  ColorName::Yellow);
    const Colors cBYR(ColorName::Blue, ColorName::Yellow,ColorName::Red);
    for (uint8_t col = 0; col < 3; ++col) {
        b.Set(0, col, cRGB).Set(1, col, cGBY).Set(2, col, cBYR);
    }

    b.Clr(1, 1);
    EXPECT_EQ(b.ToStr(),
        "rgb,rgb,rgb\n"
        "gby,---,gby\n"
        "byr,byr,byr\n");

    EXPECT_EQ(ToHexStr(b.state), "0x6cd9b15005566cd9");
    EXPECT_EQ(b.state, 0x6cd9b15005566cd9ull);

    EXPECT_EQ(b.Num(), 24);
    EXPECT_EQ(b.Num(0), 8); // 4 at level 0
    EXPECT_EQ(b.Num(1), 8); // 4 at level 1
    EXPECT_EQ(b.Num(2), 8); // 3 at level 2
    EXPECT_EQ(b.Num(ColorName::None),   3); 
    EXPECT_EQ(b.Num(ColorName::Yellow), 5); 
    EXPECT_EQ(b.Num(ColorName::Green),  5); 
    EXPECT_EQ(b.Num(ColorName::Blue),   8); 
    EXPECT_EQ(b.Num(ColorName::Red),    6); 
    
    EXPECT_EQ(b.Num(0, ColorName::None), 1); 
    EXPECT_EQ(b.Num(0, ColorName::Yellow), 0);
    EXPECT_EQ(b.Num(0, ColorName::Green), 2);
    EXPECT_EQ(b.Num(0, ColorName::Blue), 3);
    EXPECT_EQ(b.Num(0, ColorName::Red), 3);

    EXPECT_EQ(b.Num(1, ColorName::None), 1);
    EXPECT_EQ(b.Num(1, ColorName::Yellow), 3);
    EXPECT_EQ(b.Num(1, ColorName::Green), 3);
    EXPECT_EQ(b.Num(1, ColorName::Blue), 2);
    EXPECT_EQ(b.Num(1, ColorName::Red), 0);

    EXPECT_EQ(b.Num(2, ColorName::None), 1);
    EXPECT_EQ(b.Num(2, ColorName::Yellow), 2);
    EXPECT_EQ(b.Num(2, ColorName::Green), 0);
    EXPECT_EQ(b.Num(2, ColorName::Blue), 3);
    EXPECT_EQ(b.Num(2, ColorName::Red), 3);


}


