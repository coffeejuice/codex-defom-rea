// Unit-conversion helpers. See defrt/ftn/units.hpp.
#include <defrt/ftn/units.hpp>

#include <bit>
#include <cmath>
#include <cstring>

namespace def::rt::ftn {

void reset_unit_globals() { std::memset(&g_units, 0, sizeof g_units); }

// @src 140235c10  unit system from DAT_1406fe778
std::int32_t unit_system() { return g_units.unit_flag == 0 ? 1 : 2; }

namespace {
// @data 140531490  conversion factors, two per quantity (bit patterns from the binary)
constexpr std::uint64_t kUnitTable[44][2] = {
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, //  0: 1.0, 1.0
    {0x3fa42850ac14f154ULL, 0x4039666666666666ULL}, //  1: 0.03937008, 25.4
    {0x3fa42850ac14f154ULL, 0x4039666666666666ULL}, //  2: 0.03937008, 25.4
    {0x3f59652f0c659c9bULL, 0x40b933999999999aULL}, //  3: 0.001550003, 6451.6
    {0x3f0ffe77bd9a6084ULL, 0x40d000c3d70a3d71ULL}, //  4: 6.102374e-05, 16387.06
    {0x3f2d77094bd91c66ULL, 0x40b1606666666666ULL}, //  5: 0.0002248, 4448.4
    {0x3ee28fb544871a11ULL, 0x40fb958ccccccccdULL}, //  6: 8.850746e-06, 112984.8
    {0x3fc29098befe2d17ULL, 0x401b943b2dd377e2ULL}, //  7: 0.14503774, 6.894757
    {0x3eafcda7b561d100ULL, 0x4130195400000000ULL}, //  8: 9.478134e-07, 1055060.0
    {0x3ffccccccccccccdULL, 0x3fe1c71c71cd3832ULL}, //  9: 1.8, 0.5555555556
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 10
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 11
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 12
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 13
    {0x3ee28fb544871a11ULL, 0x40fb958ccccccccdULL}, // 14: 8.850746e-06, 112984.8
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 15
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 16
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 17
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 18
    {0x3f0e9393f10e1fa8ULL, 0x40d0bf8000000000ULL}, // 19: 5.832e-05, 17150.0
    {0x3f3644e5e31e47eaULL, 0x40a6fe0000000000ULL}, // 20: 0.0003398, 2943.0
    {0x3fa42850ac14f154ULL, 0x4039666666666666ULL}, // 21: 0.03937008, 25.4
    {0x3fe1c71c71cd3832ULL, 0x3ffccccccccccccdULL}, // 22: 0.5555555556, 1.8
    {0x3eec0ca600b02928ULL, 0x40f240c000000000ULL}, // 23: 1.3375e-05, 74764.0
    {0x3f81ac8d92245a52ULL, 0x405cf8f5c28f5c29ULL}, // 24: 0.0086299, 115.89
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 25
    {0x3f59652bd3c36113ULL, 0x40842947ae147ae1ULL}, // 26: 0.00155, 645.16
    {0x3fa42850ac14f154ULL, 0x4039666666666666ULL}, // 27: 0.03937008, 25.4
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 28
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 29
    {0x3f8fce3150dae3e7ULL, 0x40501851eb851eb8ULL}, // 30: 0.01553, 64.38
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 31
    {0x3f3644e5e31e47eaULL, 0x40a6fe0000000000ULL}, // 32: 0.0003398, 2943.0
    {0x3f59652bd3c36113ULL, 0x40842947ae147ae1ULL}, // 33: 0.00155, 645.16
    {0x3ee28fb544871a11ULL, 0x40fb958ccccccccdULL}, // 34: 8.850746e-06, 112984.8
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 35
    {0x3fe1c71c71cd3832ULL, 0x3ffccccccccccccdULL}, // 36: 0.5555555556, 1.8
    {0x3ff0000000000000ULL, 0x3ff0000000000000ULL}, // 37
    {0x3f77638657e07629ULL, 0x4065e40ebedfa440ULL}, // 38: 0.0057101486, 175.1268
    {0x3eafcda7b561d100ULL, 0x4130195400000000ULL}, // 39: 9.478134e-07, 1055060.0
    {0x3eafcda7b561d100ULL, 0x4130195400000000ULL}, // 40: 9.478134e-07, 1055060.0
    {0x4001a3112f6538e3ULL, 0x3fdd07a84ab75e51ULL}, // 41: 2.2046226218, 0.45359237
    {0x40e1a3e96be9c886ULL, 0x3efd064508cd0c5eULL}, // 42: 36127.294423, 2.76799115e-05
    {0x3fe1c71c71cd3832ULL, 0x3ffccccccccccccdULL}, // 43: 0.5555555556, 1.8
};
} // namespace

// @src 140235d00  unit factor lookup. The code does not bound q; rows past 43 are other data in
// the original and are not reproduced (callers pass quantity numbers inside the table).
double unit_factor(std::int32_t q, std::int32_t mode)
{
    if (mode == 1) {
        return std::bit_cast<double>(kUnitTable[q][0]);
    }
    if (mode == 2) {
        return std::bit_cast<double>(kUnitTable[q][1]);
    }
    if (mode == 3) {
        return reinterpret_cast<const double*>(&g_units)[q];
    }
    return 1.0; // DAT_140509c90
}

// @src 140235d80  temperature conversion; FUN_140484558 is fabs
double convert_temperature(std::int32_t mode, double t)
{
    const double k18 = std::bit_cast<double>(0x3ffccccccccccccdULL); // DAT_140531748 = 1.8
    const double k32 = 32.0;                                           // DAT_140522fb0
    const double k59 = std::bit_cast<double>(0x3fe1c71c71cd3832ULL); // DAT_140531740
    const double eps = std::bit_cast<double>(0x3f50624dd2f1a9fcULL); // DAT_14051f1e8 = 0.001
    double r = t; // other modes return an uninitialised local in the original
    if (mode == 1) {
        r = t * k18 + k32;
    } else if (mode == 2) {
        r = (t - k32) / k18;
    } else if (mode == 3) {
        if (std::fabs(g_units.temp_factor - k18) < eps) {
            r = t * k18 + k32;
        } else {
            r = t;
            if (std::fabs(g_units.temp_factor - k59) < eps) {
                r = (t - k32) / k18;
            }
        }
    }
    return r;
}

} // namespace def::rt::ftn
