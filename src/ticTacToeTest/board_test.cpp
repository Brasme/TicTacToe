#include "gtest/gtest.h"
#include "ticTacToe3D/board.h"
#include "ticTacToe3D/field.h"
#include "ticTacToe3D/color.h"

using namespace ttt;
TEST(board,init) {
    Board b;
    EXPECT_EQ(b.state(),0);
    EXPECT_EQ(b.toStr(),
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
    b.set(0, 0, f00);
    EXPECT_EQ(b.state(), 44ull);
    EXPECT_EQ(b.toStr(),
        "rby,---,---\n"
        "---,---,---\n"
        "---,---,---\n");

    const Field fYGB(ColorName::Yellow, ColorName::Green, ColorName::Blue);
    const Field f_GB(ColorName::None, ColorName::Green, ColorName::Blue);
    b.set(1, 1, fYGB).set(2, 0, f_GB);
    uint64_t expectedState = 44ull + ((uint64_t)fYGB.state << (7 * 4)) + ((uint64_t)f_GB.state << (7 * 6));
    EXPECT_EQ(b.state(), expectedState);
    EXPECT_EQ(b.toStr(),
        "rby,---,---\n"
        "---,ygb,---\n"
        "-gb,---,---\n");

    EXPECT_EQ(b[1][1].state, 1 + 2 * 5 + 3 * 25);
    EXPECT_EQ(b[1][1], fYGB);

    const Field fYYY(1, 1, 1);
    EXPECT_EQ(fYYY.toStr(), "yyy");
    b.set(2, 2, fYYY).set(1, 2, b[1][1]).clr(1, 1);

    expectedState = 44ull + ((uint64_t)fYGB.state << (7 * 5)) + ((uint64_t)f_GB.state << (7 * 6)) + ((uint64_t)(1 + 5 + 25) << (7 * 8));
    EXPECT_EQ(b.state(), expectedState);
    EXPECT_EQ(b.toStr(),
        "rby,---,---\n"
        "---,---,ygb\n"
        "-gb,---,yyy\n");

    EXPECT_EQ(ToHexStr(b.state()), "0x1f0156b00000002c");
    EXPECT_EQ(b.state(), 0x1f0156b00000002cull);
}

using namespace std;

void combinationsAnyOf(size_t n, size_t m,const char ch, vector<string> &c,const string &pad="")
{
    c.push_back(pad + string(m, '-')); // All empty            
    if (n == 0)
        return;    
    for (size_t i = 0; i < m; ++i) {
        combinationsAnyOf(n - 1, m -i -1, ch, c, pad + string(i,'-') + ch);
    }
}


void combinationsAllOf(size_t n, size_t m, const char ch, vector<string>& c, const string& pad = "")
{
    if (n == 0) {
        c.push_back(pad + string(m, '-'));
        return;
    }
    for (size_t i = 0; i < m; ++i) {
        combinationsAllOf(n - 1, m - i - 1, ch, c, pad + string(i, '-') + ch);
    }
}

struct Combinations : vector<string> {
    string toStr() const {
        stringstream ss;
        for (const string& s : *this)
            ss << s << ',';
        return ss.str();
    }
};
TEST(board, state) 
{
    Board b;
    for (uint8_t r = 0; r < 3; r++) for (uint8_t c = 0; c < 3; c++) {
        b[r][c].state = 124; // = 444 base 5 (4+5*4+25*4)
    }

    uint64_t maxState = b.state();
    EXPECT_EQ(maxState, 0x7cf9f3e7cf9f3e7cull); // Only 1 bit left ... not enough to store playerIdx => store it separatly
    
    
    // Or: 
    //    <placed pieces p#0>0..3 (2 bit)  => *4
    //      <pos0> 0..8                    => *9 (p1)
    //      ...
    //    <placed pieces p#1>0..3 (2 bit)  => *4
    //      <pos1> 0..7                    => *8 (p2)
    //      <pos2> 0..6                    => *7 (p3)
    //      <pos3> 0..5                    => *6 (p4)
    //    <placed pieces p#2>0..3 (2 bit)  => *4
    //      <pos1> 0..4                    => *5 (p5)
    //      <pos2> 0..3                    => *4 (p6)
    //      <pos3> 0..2                    => *3 (p7)
    //    <placed pieces p#3>0..3 (2 bit)  => *4
    //      <pos1> 0..1                    => *5 (p8)
    //      .. not needed <pos0> 0         => *1 (p9)
    
    uint64_t v = 4 * 9 * 4 * 8 * 7 * 6 * 4 * 5 * 4 * 3 * 4 * 5;
    EXPECT_EQ(v, 0x000000000dd7c000ull); // Which requires too many bits => not use
    
    // What about possible combinations?
    
    Combinations c;
    combinationsAnyOf(3, 9,'1',c);
    EXPECT_EQ(c.size(), 130);
    EXPECT_EQ(c.toStr(), 
        "---------,1--------,11-------,111------,11-1-----,11--1----,11---1---,11----1--,11-----1-,11------1,1-1------,1-11-----,1-1-1----,"
        "1-1--1---,1-1---1--,1-1----1-,1-1-----1,1--1-----,1--11----,1--1-1---,1--1--1--,1--1---1-,1--1----1,1---1----,1---11---,1---1-1--,"
        "1---1--1-,1---1---1,1----1---,1----11--,1----1-1-,1----1--1,1-----1--,1-----11-,1-----1-1,1------1-,1------11,1-------1,-1-------,"
        "-11------,-111-----,-11-1----,-11--1---,-11---1--,-11----1-,-11-----1,-1-1-----,-1-11----,-1-1-1---,-1-1--1--,-1-1---1-,-1-1----1,"
        "-1--1----,-1--11---,-1--1-1--,-1--1--1-,-1--1---1,-1---1---,-1---11--,-1---1-1-,-1---1--1,-1----1--,-1----11-,-1----1-1,-1-----1-,"
        "-1-----11,-1------1,--1------,--11-----,--111----,--11-1---,--11--1--,--11---1-,--11----1,--1-1----,--1-11---,--1-1-1--,--1-1--1-,"
        "--1-1---1,--1--1---,--1--11--,--1--1-1-,--1--1--1,--1---1--,--1---11-,--1---1-1,--1----1-,--1----11,--1-----1,---1-----,---11----,"
        "---111---,---11-1--,---11--1-,---11---1,---1-1---,---1-11--,---1-1-1-,---1-1--1,---1--1--,---1--11-,---1--1-1,---1---1-,---1---11,"
        "---1----1,----1----,----11---,----111--,----11-1-,----11--1,----1-1--,----1-11-,----1-1-1,----1--1-,----1--11,----1---1,-----1---,"
        "-----11--,-----111-,-----11-1,-----1-1-,-----1-11,-----1--1,------1--,------11-,------111,------1-1,-------1-,-------11,--------1,");

    c.clear();
    combinationsAnyOf(3, 8, '1', c);
    EXPECT_EQ(c.size(), 93);
    EXPECT_EQ(c.toStr(), 
        "--------,1-------,11------,111-----,11-1----,11--1---,11---1--,11----1-,11-----1,1-1-----,1-11----,1-1-1---,1-1--1--,1-1---1-,"
        "1-1----1,1--1----,1--11---,1--1-1--,1--1--1-,1--1---1,1---1---,1---11--,1---1-1-,1---1--1,1----1--,1----11-,1----1-1,1-----1-,"
        "1-----11,1------1,-1------,-11-----,-111----,-11-1---,-11--1--,-11---1-,-11----1,-1-1----,-1-11---,-1-1-1--,-1-1--1-,-1-1---1,"
        "-1--1---,-1--11--,-1--1-1-,-1--1--1,-1---1--,-1---11-,-1---1-1,-1----1-,-1----11,-1-----1,--1-----,--11----,--111---,--11-1--,"
        "--11--1-,--11---1,--1-1---,--1-11--,--1-1-1-,--1-1--1,--1--1--,--1--11-,--1--1-1,--1---1-,--1---11,--1----1,---1----,---11---,"
        "---111--,---11-1-,---11--1,---1-1--,---1-11-,---1-1-1,---1--1-,---1--11,---1---1,----1---,----11--,----111-,----11-1,----1-1-,"
        "----1-11,----1--1,-----1--,-----11-,-----111,-----1-1,------1-,------11,-------1,");

    c.clear();
    combinationsAnyOf(3, 7, '1', c);
    EXPECT_EQ(c.size(), 64);
    EXPECT_EQ(c.toStr(), 
        "-------,1------,11-----,111----,11-1---,11--1--,11---1-,11----1,1-1----,1-11---,1-1-1--,1-1--1-,1-1---1,1--1---,1--11--,1--1-1-,"
        "1--1--1,1---1--,1---11-,1---1-1,1----1-,1----11,1-----1,-1-----,-11----,-111---,-11-1--,-11--1-,-11---1,-1-1---,-1-11--,-1-1-1-,"
        "-1-1--1,-1--1--,-1--11-,-1--1-1,-1---1-,-1---11,-1----1,--1----,--11---,--111--,--11-1-,--11--1,--1-1--,--1-11-,--1-1-1,--1--1-,"
        "--1--11,--1---1,---1---,---11--,---111-,---11-1,---1-1-,---1-11,---1--1,----1--,----11-,----111,----1-1,-----1-,-----11,------1,");

    c.clear();
    combinationsAnyOf(3, 6, '1', c);
    EXPECT_EQ(c.size(), 42);
    EXPECT_EQ(c.toStr(), 
        "------,1-----,11----,111---,11-1--,11--1-,11---1,1-1---,1-11--,1-1-1-,1-1--1,1--1--,1--11-,1--1-1,1---1-,1---11,1----1,-1----,"
        "-11---,-111--,-11-1-,-11--1,-1-1--,-1-11-,-1-1-1,-1--1-,-1--11,-1---1,--1---,--11--,--111-,--11-1,--1-1-,--1-11,--1--1,---1--,"
        "---11-,---111,---1-1,----1-,----11,-----1,");

    c.clear();
    combinationsAnyOf(3, 5, '1', c);
    EXPECT_EQ(c.size(), 26);
    EXPECT_EQ(c.toStr(), 
        "-----,1----,11---,111--,11-1-,11--1,1-1--,1-11-,1-1-1,1--1-,1--11,1---1,-1---,-11--,-111-,-11-1,"
        "-1-1-,-1-11,-1--1,--1--,--11-,--111,--1-1,---1-,---11,----1,");

    c.clear();
    combinationsAnyOf(3, 4, '1', c);
    EXPECT_EQ(c.size(), 15);
    EXPECT_EQ(c.toStr(), "----,1---,11--,111-,11-1,1-1-,1-11,1--1,-1--,-11-,-111,-1-1,--1-,--11,---1,");

    c.clear();
    combinationsAnyOf(3, 3, '1', c);
    EXPECT_EQ(c.size(), 8);
    EXPECT_EQ(c.toStr(), "---,1--,11-,111,1-1,-1-,-11,--1,");

    c.clear();
    combinationsAnyOf(3, 2, '1', c);
    EXPECT_EQ(c.size(), 4);
    EXPECT_EQ(c.toStr(), "--,1-,11,-1,");

    c.clear();
    combinationsAnyOf(3, 1, '1', c);
    EXPECT_EQ(c.size(), 2);
    EXPECT_EQ(c.toStr(), "-,1,");

    // => playerIdx=[0:1]
    // => represent "layer" as 9 bit value => 130 combinations => all empty (worst case) layer = 130^4 combinations for each layer, 3 layers => 
    uint64_t combScale = 130 * 130 * 130 * 130;
    EXPECT_EQ(combScale, 0x0000000011061010ull);

    combScale = combScale * combScale * combScale; // 3 layers max combinations (alle empty) => too large
    EXPECT_EQ(combScale, 0xb9a87f2742301000ull);

    // but.. what if encode 2 bits = # of placed 0..3, then 8 bits represent "number of placed pieces" for layer - and placement is encoded as: 
    //  0: no bits for placed => scale=1
    //  1: 9 places (combination lookup) => scale=9
    c.clear(); combinationsAllOf(1, 9, '1', c); EXPECT_EQ(c.size(), 9); 
    EXPECT_EQ(c.toStr(), "1--------,-1-------,--1------,---1-----,----1----,-----1---,------1--,-------1-,--------1,");
    //  2: 36 combinations => scale =36;
    c.clear(); combinationsAllOf(2, 9, '1', c); EXPECT_EQ(c.size(), 36);
    EXPECT_EQ(c.toStr(), "11-------,1-1------,1--1-----,1---1----,1----1---,1-----1--,1------1-,1-------1,-11------,"
        "-1-1-----,-1--1----,-1---1---,-1----1--,-1-----1-,-1------1,--11-----,--1-1----,--1--1---,--1---1--,--1----1-,"
        "--1-----1,---11----,---1-1---,---1--1--,---1---1-,---1----1,----11---,----1-1--,----1--1-,----1---1,-----11--,"
        "-----1-1-,-----1--1,------11-,------1-1,-------11,");
    // 3: 130 combinations

    // For next player: remaning placements is indexed with 9-"occupied by first", and hence first may 

 
}


constexpr uint8_t MAX(uint8_t v, uint8_t m) { return v > m ? m : v; }

ostream& create_decode_lookup(ostream& os,const std::string &symName,uint8_t maxVal,const std::string tName)
{
    os << "namespace tttnums {\n";
    uint16_t idx = 0;
    uint8_t n0range = MAX(10,maxVal);
    for (uint8_t n0 = 0; n0 < n0range; ++n0) {
        const uint8_t n1range = MAX(10 - n0,maxVal);
        for (uint8_t n1 = 0; n1 < n1range; ++n1) {
            const uint8_t n2range = MAX(10 - n1 - n0,maxVal);
            for (uint8_t n2 = 0; n2 < n2range; ++n2) {
                const uint8_t n3range = MAX(10 - n2 - n1 - n0,maxVal);
                os << "   static const " << tName << "     " << symName << (int)n0 << (int)n1 << (int)n2 << "_[] = {";
                for (uint8_t n3 = 0; n3 < n3range; ++n3) {
                    if (n3 != 0) os << ", ";
                    os << (int)idx;
                    idx++;
                }
                os << " };\n";
            }
            os << "   static const " << tName << "    *" << symName << (int)n0 << (int)n1 << "__[] = {";
            for (uint8_t n2 = 0; n2 < n2range; ++n2) {
                if (n2 != 0) os << ", ";
                os << symName << (int)n0 << (int)n1 << (int)n2 << '_';
            }
            os << " };\n";
        }
        os << "   static const " << tName << "   **" << symName << (int)n0 << "___[] = {";
        for (uint8_t n1 = 0; n1 < n1range; ++n1) {
            if (n1 != 0) os << ", ";
            os << symName << (int)n0 << (int)n1 << "__";
        }
        os << " };\n";
    }
    os << "   static const " << tName << "  ***" << symName << "____[] = {";
    for (uint8_t n0 = 0; n0 < n0range; ++n0) {
        if (n0 != 0) os << ", ";
        os << symName << (int)n0 << "___";

    }
    os << " };\n";

    os << "} // namespace tttnums\n";
    return os;
}

ostream& create_decode_lookup_lkup_3(ostream& os, const std::string& symName)
{
    std::string s = "{\n      ";
    os << "namespace tttnums {\n";
    os << "   static const uint8_t " << symName << "[] = ";
    int lineLen = 0;
    uint16_t idx = 0;
    for (uint8_t n0 = 0; n0 < 4; ++n0) {
        if (lineLen >= 100) { s = ",\n      "; lineLen = 0;  }
        for (uint8_t n1 = 0; n1 < 4; ++n1) {
            for (uint8_t n2 = 0; n2 < 4; ++n2) {
                for (uint8_t n3 = 0; n3 < 4; ++n3) {
                    if ((n0 + n1 + n2 + n3) <= 9) {
                        os << s << (int)idx;
                        idx++;
                        lineLen += 2 + (idx>=100?3:idx>=10?2:1);
                    } else {
                        os << s << 0;
                        lineLen += 3;
                    }
                    s = ", ";                    
                }                
            }            
        }        
    }
    os << "\n   };\n";

    os << "} // namespace tttnums\n";
    return os;
}

#include <iostream>
#include <fstream>

TEST(board, create_decode_lookup_code) {

    std::ofstream ss;
    ss.open("board_num_encoded.txt", ios::trunc);
    create_decode_lookup(ss, "enc9x4_", 10,"uint16_t");

    create_decode_lookup(ss, "enc3x4_", 4,"uint8_t");

    create_decode_lookup_lkup_3(ss, "enc3_");
    ss.close();
}


//TEST(board, encode_number) {
//
//    // sequence
//    Combinations c;
//    for (uint32_t i = 0; i < 715; ++i) {
//        uint32_t v = i;
//        uint32_t r0 = 10;
//        uint32_t n0 = v % r0;
//        uint32_t r1 = (n0 + 1);
//        v /= r1;
//        uint32_t n1 = v / r0;
//        uint32_t n1 = n % (n0 + 1);
//
//    }
//
//    uint8_t n0range = 10;
//    for (uint8_t n0 = 0; n0 < n0range; ++n0) {
//        const uint8_t n1range = 10 - n0;
//        for (uint8_t n1 = 0; n1 < n1range; ++n1) {
//            const uint8_t n2range = 10 - n1 - n0;
//            for (uint8_t n2 = 0; n2 < n2range; ++n2) {
//                const uint8_t n3range = 10 - n2 - n1 - n0;
//                for (uint8_t n3 = 0; n3 < n3range; ++n3) {
//                    std::stringstream ss;
//                    ss << (int)n0 << (int)n1 << (int)n2 << (int)n3;
//                    c.push_back(ss.str());
//                }
//            }
//        }
//    }
//
//
//    uint8_t r0 = 9;  size_t n0 = cap(v[0], &r0);
//    uint8_t r1 = 10; size_t n1 = cap(v[1], &maxSum);
//    uint8_t r2 = 10;
//    uint8_t r3 = 10;
//
//    size_t n2 = cap(v[2], &maxSum);
//    size_t n3 = cap(v[3], &maxSum);
//
//    return n3 + n2 * (n3 + 1) + n1 * (n3 + 1) * (n2 + 1) + n0 * (n3 + 1) * (n2 + 1) * (n1 + 1);
//}

uint16_t sum3decode(const uint8_t(&v)[4]);
uint16_t sum9decode(const uint8_t(&v)[4]);
TEST(board, encode_number_0_9)
{
    Combinations c;
    uint8_t n0range = 10;
    for (uint8_t n0 = 0; n0 < n0range; ++n0) {
        const uint8_t n1range = 10 - n0;
        for (uint8_t n1 = 0; n1 < n1range; ++n1) {
            const uint8_t n2range = 10 - n1 - n0;
            for (uint8_t n2 = 0; n2 < n2range; ++n2) {
                const uint8_t n3range = 10 - n2 - n1 - n0;
                for (uint8_t n3 = 0; n3 < n3range; ++n3) {
                    std::stringstream ss;
                    ss << (int)n0 << (int)n1 << (int)n2 << (int)n3;
                    c.push_back(ss.str());
                }
            }
        }
    }
    EXPECT_EQ(8 * 8 * 8 * 8, 4096); // If 3 bits each (allowing for <0..3>^4
    EXPECT_EQ(c.size(), 715); // All legal combinations of <0..9> ^ 4 
    EXPECT_EQ(c.toStr(),        
        "0000,0001,0002,0003,0004,0005,0006,0007,0008,0009,0010,0011,0012,0013,0014,0015,0016,0017,0018,"
        "0020,0021,0022,0023,0024,0025,0026,0027,0030,0031,0032,0033,0034,0035,0036,"
        "0040,0041,0042,0043,0044,0045,0050,0051,0052,0053,0054,0060,0061,0062,0063,0070,0071,0072,"
        "0080,0081,0090,0100,0101,0102,0103,0104,0105,0106,0107,0108,0110,0111,0112,0113,0114,0115,0116,0117,"
        "0120,0121,0122,0123,0124,0125,0126,0130,0131,0132,0133,0134,0135,0140,0141,0142,0143,0144,0150,0151,0152,0153,"
        "0160,0161,0162,0170,0171,0180,0200,0201,0202,0203,0204,0205,0206,0207,0210,0211,0212,0213,0214,0215,0216,"
        "0220,0221,0222,0223,0224,0225,0230,0231,0232,0233,0234,0240,0241,0242,0243,0250,0251,0252,0260,0261,0270,"
        "0300,0301,0302,0303,0304,0305,0306,0310,0311,0312,0313,0314,0315,0320,0321,0322,0323,0324,0330,0331,0332,0333,0340,0341,0342,0350,0351,0360,0400,0401,0402,0403,0404,0405,0410,0411,0412,0413,0414,0420,0421,0422,0423,0430,0431,0432,0440,0441,0450,0500,0501,0502,0503,0504,0510,0511,0512,0513,0520,0521,0522,0530,0531,0540,0600,0601,0602,0603,0610,0611,0612,0620,0621,0630,0700,0701,0702,0710,0711,0720,0800,0801,0810,0900,1000,1001,1002,1003,1004,1005,1006,1007,1008,1010,1011,1012,1013,1014,1015,1016,1017,1020,1021,1022,1023,1024,1025,1026,1030,1031,1032,1033,1034,1035,1040,1041,1042,1043,1044,1050,1051,1052,1053,1060,1061,1062,1070,1071,1080,1100,1101,1102,1103,1104,1105,1106,1107,1110,1111,1112,1113,1114,1115,1116,1120,1121,1122,1123,1124,1125,1130,1131,1132,1133,1134,1140,1141,1142,1143,1150,1151,1152,1160,1161,1170,1200,1201,1202,1203,1204,1205,1206,1210,1211,1212,1213,1214,1215,1220,1221,1222,1223,1224,1230,1231,1232,1233,1240,1241,1242,1250,1251,1260,1300,1301,1302,1303,1304,1305,1310,1311,1312,1313,1314,1320,1321,1322,1323,1330,1331,1332,1340,1341,1350,1400,1401,1402,1403,1404,1410,1411,1412,1413,1420,1421,1422,1430,1431,1440,1500,1501,1502,1503,1510,1511,1512,1520,1521,1530,1600,1601,1602,1610,1611,1620,1700,1701,1710,1800,2000,2001,2002,2003,2004,2005,2006,2007,2010,2011,2012,2013,2014,2015,2016,2020,2021,2022,2023,2024,2025,2030,2031,2032,2033,2034,2040,2041,2042,2043,2050,2051,2052,2060,2061,2070,2100,2101,2102,2103,2104,2105,2106,2110,2111,2112,2113,2114,2115,2120,2121,2122,2123,2124,2130,2131,2132,2133,2140,2141,2142,2150,2151,2160,2200,2201,2202,2203,2204,2205,2210,2211,2212,2213,2214,2220,2221,2222,2223,2230,2231,2232,2240,2241,2250,2300,2301,2302,2303,2304,2310,2311,2312,2313,2320,2321,2322,2330,2331,2340,2400,2401,2402,2403,2410,2411,2412,2420,2421,2430,2500,2501,2502,2510,2511,2520,2600,2601,2610,2700,3000,3001,3002,3003,3004,3005,3006,3010,3011,3012,3013,3014,3015,3020,3021,3022,3023,3024,3030,3031,3032,3033,3040,3041,3042,3050,3051,3060,3100,3101,3102,3103,3104,3105,3110,3111,3112,3113,3114,3120,3121,3122,3123,3130,3131,3132,3140,3141,3150,3200,3201,3202,3203,3204,3210,3211,3212,3213,3220,3221,3222,3230,3231,3240,3300,3301,3302,3303,3310,3311,3312,3320,3321,3330,3400,3401,3402,3410,3411,3420,3500,3501,3510,3600,4000,4001,4002,4003,4004,4005,4010,4011,4012,4013,4014,4020,4021,4022,4023,4030,4031,4032,4040,4041,4050,4100,4101,4102,4103,4104,4110,4111,4112,4113,4120,4121,4122,4130,4131,4140,4200,4201,4202,4203,4210,4211,4212,4220,4221,4230,4300,4301,4302,4310,4311,4320,4400,4401,4410,4500,5000,5001,5002,5003,5004,5010,5011,5012,5013,5020,5021,5022,5030,5031,5040,5100,5101,5102,5103,5110,5111,5112,5120,5121,5130,5200,5201,5202,5210,5211,5220,5300,5301,5310,5400,6000,6001,6002,6003,6010,6011,6012,6020,6021,6030,6100,6101,6102,6110,6111,6120,6200,6201,6210,6300,7000,7001,7002,7010,7011,7020,7100,7101,7110,7200,8000,8001,8010,8100,9000,");

    EXPECT_EQ(c[sum9decode({ 1,1,0,7 })], "1107");
    EXPECT_EQ(c[sum9decode({ 0,2,5,3 })], "0252"); // last cut (sum <=9)
    EXPECT_EQ(c[sum9decode({ 2,3,4,5 })], "2340"); // last cut (sum <=9)
    EXPECT_EQ(c[sum9decode({ 9,3,3,3 })], "9000"); // last cut (sum <=9)
    EXPECT_EQ(c[sum9decode({ 5,0,3,0 })], "5030");
    EXPECT_EQ(c[sum9decode({ 1,2,3,3 })], "1233");
    EXPECT_EQ(c[sum9decode({ 9,0,0,0 })], "9000");

    // Example:   0013 : 0=>10, 0=>10, 1=>9   : 1*9+3=12
    /*size_t v = encode({ 0, 0, 1, 9 });
    EXPECT_EQ(c[v], "0019");


    EXPECT_EQ(c[encode({ 0,0,0,0 })], "0000");
    EXPECT_EQ(c[encode({ 0,0,0,9 })], "0009");
    EXPECT_EQ(c[encode({ 0,0,1,0 })], "0010");*/

    //            0152 => 2+(10)*
    // Value 3,2,1,2 should give 2+1+2+3  +9*1+ 1234 represents
}


TEST(board, encode_number_0_3)
{
    Combinations c;
    uint8_t n0range = 4;
    for (uint8_t n0 = 0; n0 < n0range; ++n0) {
        const uint8_t n1range = MAX(10 - n0,4);
        for (uint8_t n1 = 0; n1 < n1range; ++n1) {
            const uint8_t n2range = MAX(10 - n1 - n0,4);
            for (uint8_t n2 = 0; n2 < n2range; ++n2) {
                const uint8_t n3range = MAX(10 - n2 - n1 - n0,4);
                for (uint8_t n3 = 0; n3 < n3range; ++n3) {
                    std::stringstream ss;
                    ss << (int)n0 << (int)n1 << (int)n2 << (int)n3;
                    c.push_back(ss.str());
                }
            }
        }
    }
    EXPECT_EQ(8 * 8 * 8 * 8, 4096); // If 3 bits each (allowing for <0..7>^4
    EXPECT_EQ(c.size(), 241); // All legal combinations of <0..9> ^ 4 
    EXPECT_EQ(c.toStr(),
        "0000,0001,0002,0003,0010,0011,0012,0013,0020,0021,0022,0023,0030,0031,0032,0033,0100,0101,0102,0103,0110,0111,0112,0113,0120,0121,0122,0123,"
        "0130,0131,0132,0133,0200,0201,0202,0203,0210,0211,0212,0213,0220,0221,0222,0223,0230,0231,0232,0233,0300,0301,0302,0303,0310,0311,0312,0313,"
        "0320,0321,0322,0323,0330,0331,0332,0333,1000,1001,1002,1003,1010,1011,1012,1013,1020,1021,1022,1023,1030,1031,1032,1033,1100,1101,1102,1103,"
        "1110,1111,1112,1113,1120,1121,1122,1123,1130,1131,1132,1133,1200,1201,1202,1203,1210,1211,1212,1213,1220,1221,1222,1223,1230,1231,1232,1233,"
        "1300,1301,1302,1303,1310,1311,1312,1313,1320,1321,1322,1323,1330,1331,1332,2000,2001,2002,2003,2010,2011,2012,2013,2020,2021,2022,2023,2030,"
        "2031,2032,2033,2100,2101,2102,2103,2110,2111,2112,2113,2120,2121,2122,2123,2130,2131,2132,2133,2200,2201,2202,2203,2210,2211,2212,2213,2220,"
        "2221,2222,2223,2230,2231,2232,2300,2301,2302,2303,2310,2311,2312,2313,2320,2321,2322,2330,2331,3000,3001,3002,3003,3010,3011,3012,3013,3020,"
        "3021,3022,3023,3030,3031,3032,3033,3100,3101,3102,3103,3110,3111,3112,3113,3120,3121,3122,3123,3130,3131,3132,3200,3201,3202,3203,3210,3211,"
        "3212,3213,3220,3221,3222,3230,3231,3300,3301,3302,3303,3310,3311,3312,3320,3321,3330,");

    EXPECT_EQ(c[sum3decode({ 1,1,0,1 })], "1101");
    EXPECT_EQ(c[sum3decode({ 0,2,1,3 })], "0213"); // last cut (sum <=9)
    EXPECT_EQ(c[sum3decode({ 2,3,4,5 })], "2331");
    EXPECT_EQ(c[sum3decode({ 1,3,3,3 })], "1332"); // last cut (sum <=9)
    EXPECT_EQ(c[sum3decode({ 5,0,3,0 })], "3030");
    EXPECT_EQ(c[sum3decode({ 1,2,3,3 })], "1233");
    EXPECT_EQ(c[sum3decode({ 3,0,0,0 })], "3000");

    // Example:   0013 : 0=>10, 0=>10, 1=>9   : 1*9+3=12
    /*size_t v = encode({ 0, 0, 1, 9 });
    EXPECT_EQ(c[v], "0019");


    EXPECT_EQ(c[encode({ 0,0,0,0 })], "0000");
    EXPECT_EQ(c[encode({ 0,0,0,9 })], "0009");
    EXPECT_EQ(c[encode({ 0,0,1,0 })], "0010");*/

    //            0152 => 2+(10)*
    // Value 3,2,1,2 should give 2+1+2+3  +9*1+ 1234 represents
}

TEST(board, solved)
{
    Board b(0x1f0156b00000002cull);

    EXPECT_EQ(b.solved(), 1);
    EXPECT_EQ(b.solved(ColorName::Yellow), 1);
    
    b.set(1, 1, Colors(0, 1, 0));
    EXPECT_EQ(b.toStr(),
        "rby,---,---\n"
        "---,-y-,ygb\n"
        "-gb,---,yyy\n");
    EXPECT_EQ(b.solved(), 2);
    EXPECT_EQ(b.solved(ColorName::Yellow), 2);

    b.set(1, 1, Colors(0, 1, 0));
    EXPECT_EQ(b.toStr(),
        "rby,---,---\n"
        "---,-y-,ygb\n"
        "-gb,---,yyy\n");
    EXPECT_EQ(b.solved(), 2);
    EXPECT_EQ(b.solved(ColorName::Yellow), 2);

    b.clr();
    EXPECT_EQ(b.solved(), 0);
    EXPECT_EQ(b.solved(ColorName::Green), 0);
    
    const Colors cGGG(ColorName::Green, ColorName::Green, ColorName::Green);
    for (uint8_t col = 0; col < 3; ++col)
        b.set(0, col, cGGG);
    EXPECT_EQ(b.solved(), 8);
    EXPECT_EQ(b.solved(ColorName::Yellow), 0);
    EXPECT_EQ(b.solved(ColorName::Green), 8);
    EXPECT_EQ(b.solved(ColorName::Blue), 0);
    EXPECT_EQ(b.solved(ColorName::Red), 0);
    EXPECT_EQ(b.toStr(),
        "ggg,ggg,ggg\n"
        "---,---,---\n"
        "---,---,---\n");

    for (uint8_t col = 0; col < 3; ++col)
        b.set(1, col, cGGG);
    EXPECT_EQ(b.solved(), 16);
    EXPECT_EQ(b.solved(ColorName::Yellow), 0);
    EXPECT_EQ(b.solved(ColorName::Green), 16);
    EXPECT_EQ(b.solved(ColorName::Blue), 0);
    EXPECT_EQ(b.solved(ColorName::Red), 0);
    EXPECT_EQ(b.toStr(),        
        "ggg,ggg,ggg\n"
        "ggg,ggg,ggg\n"
        "---,---,---\n");

    for (uint8_t col = 0; col < 3; ++col)
        b.set(2, col, cGGG);
    EXPECT_EQ(b.solved(), 24 +5*5);
    EXPECT_EQ(b.solved(ColorName::Yellow), 0);
    EXPECT_EQ(b.solved(ColorName::Green), 49);
    EXPECT_EQ(b.solved(ColorName::Blue), 0);
    EXPECT_EQ(b.solved(ColorName::Red), 0);
    EXPECT_EQ(b.toStr(),
        "ggg,ggg,ggg\n"
        "ggg,ggg,ggg\n"
        "ggg,ggg,ggg\n");

}

TEST(board, num)
{
    Board b(0x1f0156b00000002cull);
    EXPECT_EQ(b.toStr(),
        "rby,---,---\n"
        "---,---,ygb\n"
        "-gb,---,yyy\n");

    EXPECT_EQ(b.num(), 11);
    EXPECT_EQ(b.num(0), 3); // 3 at level 0
    EXPECT_EQ(b.num(1), 4); // 4 at level 1
    EXPECT_EQ(b.num(2), 4); // 4 at level 2
    EXPECT_EQ(b.num(ColorName::Yellow), 5); // total 5 yellow placed
    EXPECT_EQ(b.num(0, ColorName::Yellow), 2); // total 5 yellow placed at level 0
    EXPECT_EQ(b.num(1, ColorName::Yellow), 1); // total 5 yellow placed at level 1
    EXPECT_EQ(b.num(2, ColorName::Yellow), 2); // total 5 yellow placed at level 2
    EXPECT_EQ(b.num(ColorName::None), 16); // 16 left
    EXPECT_EQ(b.num(0, ColorName::None), 6); // 6 left at level 0
    EXPECT_EQ(b.num(1, Color()), 5); // 5 left at level 1
    EXPECT_EQ(b.num(2, Color()), 5); // 5 left at level 2

    b.clr();
    const Colors cRGB(ColorName::Red,  ColorName::Green, ColorName::Blue);
    const Colors cGBY(ColorName::Green,ColorName::Blue,  ColorName::Yellow);
    const Colors cBYR(ColorName::Blue, ColorName::Yellow,ColorName::Red);
    for (uint8_t col = 0; col < 3; ++col) {
        b.set(0, col, cRGB).set(1, col, cGBY).set(2, col, cBYR);
    }

    b.clr(1, 1);
    EXPECT_EQ(b.toStr(),
        "rgb,rgb,rgb\n"
        "gby,---,gby\n"
        "byr,byr,byr\n");

    EXPECT_EQ(ToHexStr(b.state()), "0x6cd9b15005566cd9");
    EXPECT_EQ(b.state(), 0x6cd9b15005566cd9ull);

    EXPECT_EQ(b.num(), 24);
    EXPECT_EQ(b.num(0), 8); // 4 at level 0
    EXPECT_EQ(b.num(1), 8); // 4 at level 1
    EXPECT_EQ(b.num(2), 8); // 3 at level 2
    EXPECT_EQ(b.num(ColorName::None),   3); 
    EXPECT_EQ(b.num(ColorName::Yellow), 5); 
    EXPECT_EQ(b.num(ColorName::Green),  5); 
    EXPECT_EQ(b.num(ColorName::Blue),   8); 
    EXPECT_EQ(b.num(ColorName::Red),    6); 
    
    EXPECT_EQ(b.num(0, ColorName::None), 1); 
    EXPECT_EQ(b.num(0, ColorName::Yellow), 0);
    EXPECT_EQ(b.num(0, ColorName::Green), 2);
    EXPECT_EQ(b.num(0, ColorName::Blue), 3);
    EXPECT_EQ(b.num(0, ColorName::Red), 3);

    EXPECT_EQ(b.num(1, ColorName::None), 1);
    EXPECT_EQ(b.num(1, ColorName::Yellow), 3);
    EXPECT_EQ(b.num(1, ColorName::Green), 3);
    EXPECT_EQ(b.num(1, ColorName::Blue), 2);
    EXPECT_EQ(b.num(1, ColorName::Red), 0);

    EXPECT_EQ(b.num(2, ColorName::None), 1);
    EXPECT_EQ(b.num(2, ColorName::Yellow), 2);
    EXPECT_EQ(b.num(2, ColorName::Green), 0);
    EXPECT_EQ(b.num(2, ColorName::Blue), 3);
    EXPECT_EQ(b.num(2, ColorName::Red), 3);


}


