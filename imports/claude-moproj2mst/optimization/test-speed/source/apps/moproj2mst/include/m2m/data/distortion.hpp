// m2m/data/distortion.hpp: DistortionPassData (SfOprDistortionProperties+0x40, 0x5e0 bytes,
// ctor 0x1413690c0). Owner: P13 (reader 0x141365c60 -> 0x1413680e0, writer 0x141362f60).
// The values come from Operations/TaskNNNNN/MachDistortion.MDWZ (s1-04 6, P13 notes).
#pragma once

#include <array>
#include <cstdint>

namespace m2m::data {

// Free-distortion BCC block (+0x18 of DistortionPassData, 0x98 bytes, ctor 0x1411631e0; reader
// 0x141163cc0 between BEGIN_FREE_DIST_BCC_INFO and END_FREE_DIST_BCC_INFO). The MST writer does
// not read it; it is kept so the reader consumes the same tokens as the original.
struct FreeDistBcc {
    double fixedCoord[3] = {};     // +0x00 FIXEDCOORD
    double fixedZ[3] = {};         // +0x18 FIXED_Z_COORD
    double fixedX[3] = {};         // +0x30 FIXED_X_COORD
    double fixedY[3] = {};         // +0x48 FIXED_Y_COORD
    std::int32_t fixedNode = 0;    // +0x60 FIXEDNODE
    std::int32_t fixedZNode = 0;   // +0x64 FIXEDZNODE
    double fixedZAngle = 0.0;      // +0x68 FIXEDZANGLE
    std::int32_t fixedXNode = 0;   // +0x70 FIXEDXNODE
    double fixedXAngle = 0.0;      // +0x78 FIXEDXANGLE
    std::int32_t fixedYNode = 0;   // +0x80 FIXEDYNODE
    double fixedYAngle = 0.0;      // +0x88 FIXEDYANGLE
    std::uint8_t autoFix = 0;      // +0x90 AUTO
    std::uint8_t f91 = 0;          // +0x91 (ctor sets 0; 0x1411e2450 sets 1 for Boolean)
    // The ctor also clears fields through setters 0x141344d20, 0x1411632e0, 0x1404ce5b0,
    // 0x141163300, 0x141163320, 0x14010f1e0, 0x140bb1000, 0x140eae910 (not decompiled; they write
    // zeros, which the value-initialised members above already are).
};

struct DistortionData {
    std::int32_t mode = 0;         // +0x00 token-reader base (0x140906740)
    std::int32_t fixtureCount = 0; // +0x04 WZD_MACHDIST_NUM_FIXTURES (0x14142ff20)
    std::int32_t passCount = 0;    // +0x08 WZD_MACHDIST_NUM_PASSES (0x140cbc690)
    std::int32_t firstPass = 0;    // +0x0c first pass the writer writes (0x140cbc680); the MDWZ
                                   //       reader never sets it
    std::uint8_t initVelBcc = 0;   // +0x10 INIT_VEL_BCC (0x140f8a540)
    std::uint8_t f11 = 0;          // +0x11
    FreeDistBcc freeDist;          // +0x18
    std::int32_t fb0 = 0;          // +0xb0
    std::int32_t workpiece = 1;    // +0xb4 WZD_MACHDIST_WORKPIECE_OBJECT_NO
    std::array<std::int32_t, 100> fixtures{};    // +0xb8 object number per fixture
    std::array<std::int32_t, 100> passObjects{}; // +0x3d8 object number per pass (0x1413651f0)
    std::array<std::uint8_t, 100> f568{};        // +0x568
    std::uint8_t f5cc = 0, f5ce = 0, f5cf = 0, f5d0 = 0;   // +0x5cc +0x5ce +0x5cf +0x5d0
    // +0x5d8 owner (SfOprDistortionProperties*): DistortionProps holds this object.

    // @src 0x141366710  reset before each read: +4, +8, +0xc = 0, +0xb4 = 1, the three arrays 0
    void resetCounts()
    {
        fixtureCount = 0;
        passCount = 0;
        firstPass = 0;
        workpiece = 1;
        fixtures.fill(0);
        passObjects.fill(0);
        f568.fill(0);
    }
};

}  // namespace m2m::data
