// m2m/data/l6_blocks.hpp: props blocks that the MDT L6 loader slots write (owner MDT L6).
//
//   HTPropsBlock ......... SfOprHeatTreatProperties+0x58, written by props vt+0x68 (0x1411c0730) and
//                          0x1411bfd60, read back by props vt+0x70 (0x1411c05a0) and vt+0x60
//                          (0x1411bfa20) on the save path
//   DistortionCounts ..... SfOprDistortionProperties+0x38 (0x18 bytes, ctor 0x141369120), written by
//                          props vt+0x58 (0x1413665c0)
#pragma once

#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::data {

// The HeatTreat props ctor (0x1411bd570) that makes this block is not decompiled; on the load path
// 0x1411c0730 writes every scalar below before 0x1411bfd60 reads one, and 0x1411c0980 empties the
// lists, so the loader never depends on the ctor values (it refuses when the sim control is missing).
struct HTPropsBlock {
    bool b0 = false;                   // +0x00 SimCtrl+0x108 != 0 (0x1408dfd80)
    bool b1 = false;                   // +0x01 SimCtrl+0x110 != 0 (0x1408dfdc0); gates the +0xc0 list
    bool b2 = false;                   // +0x02 SimCtrl+0x10c != 0 (0x1408dfda0)
    int stepCount = 0;                 // +0x04 schedule steps (0x1411bfd60); NO_MEDIA
    // the defaults below: the block ctor 0x1411c2ec0 (L2)
    int stpdef = 1;                    // +0x08 SimCtrl+0x1c8 == 1 ? 0 : 1; STEPDEF
    int stepIncr = 10;                 // +0x0c SimCtrl+0x164; STPINC
    double dtmax = 0.001;              // +0x10 SimCtrl+0x1d0; DTMAX (0x1421b5050)
    double dtpmax1 = 2.0;              // +0x18 SimCtrl+0x270; DTEMPMAX (0x142101520)
    double dtpmax2 = 0.001;            // +0x20 SimCtrl+0x2a0; DTEMPLMT0 (= +0x10)
    double f28 = 10.0;                 // +0x28 SimCtrl+0x2a8; DTEMPLMT1 (0x142142078)
    // Per-step values of the property tree (QList members), one entry per schedule step.
    std::vector<double> time;          // +0xa0 step+0x00
    std::vector<qt::String> media;     // +0xa8 media record name
    std::vector<double> t3;            // +0xb0 step+0x10; ENVTMP
    std::vector<double> cnvcof;        // +0xb8 CNVCOF (filled by setNO_MEDIA 0x1411c0c30 only; L2)
    std::vector<double> t4;            // +0xc0 step+0x18 (only when b1); ENVATM
    std::vector<double> acvcof;        // +0xc8 ACVCOF (setNO_MEDIA only, when b1; L2)
    std::vector<bool> mediaFlag;       // +0xd0 media record +0x08
};

struct DistortionCounts {
    int fixtures = 0;                  // +0x10 DistortionPassData+0x04
    int passes = 0;                    // +0x14 DistortionPassData+0x08
};

}  // namespace m2m::data
