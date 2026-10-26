// m2m/data/cogging.hpp: Cogging process data held by CoggingProps::model (s1-03 3).
//
// Owner: P06 (Cogging.MDT reader 0x140761c90 fills it). P11 (writer 0x1407520c0) reads it.
// Layout proposed by P11 and adopted by P06: field names follow the MDT keys of s1-03 3.1/3.2;
// offsets are the original's. Fields after the "P06:" markers are the rest of what the reader
// stores (the writer does not read them).
// Byte effects of the positional reader (s1-03 5: %d into bytes, Cycle_Time low byte) are applied
// by the reader, so the writer reads these fields as they end up in memory.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace m2m::data {

struct CogHeat {                   // HEAT (0x80, ctor 0x140797a10, reader 0x140797ef0)
    int active = 0;                // +0x00
    int nstep = 0;                 // +0x04
    int stpinc = 0;                // +0x08
    int solmtt = 1;                // +0x0c (never read from the MDT)
    double tmax = 0.0;             // +0x10 (float widened)
    double dtpmax = 0.0;           // +0x18 (float widened)
    double ihtcof = 0.0;           // +0x20
    double ihtcof2 = 0.0;          // +0x28
    std::string userKey;           // +0x30 char[0x4f] ("---" -> " ")
};

struct CogBite {                   // BITE (0x140, ctor 0x140790700, reader 0x140790ca0/0x140790d80)
    int nstep = 0;                 // +0x00
    int stpinc = 0;                // +0x04
    int pdie = 0;                  // +0x08
    double smax = 0.0;             // +0x10
    double lmax = 0.0;             // +0x18
    double dtsub = 0.0;            // +0x20
    int stepMod = 0;               // +0x28
    double dsmax = 0.0;            // +0x30
    double dtmax = 0.0;            // +0x38
    double clampForce = 0.0;       // +0x40
    int fricTyp = 0;               // +0x48
    double frcfac = 0.0;           // +0x50
    double ihtcof = 0.0;           // +0x58
    double ihtcof2 = 0.0;          // +0x60
    std::string userKey;           // +0x68 (text after the last '/')
    double nomBite = 0.0;          // +0xb8
    CogHeat ht;                    // +0xc0 bite heat transfer
    int f0c = 1;                   // +0x0c P06: ctor 0x140790700 sets 1, never read from the MDT
};

struct CogPass {                   // PASS (0x108, ctor 0x140791580, reader 0x140792810)
    int autoBite = 0;              // +0x00
    int noBite = 1;                // +0x04 (0 read -> 1)
    double rotAng = 0.0;           // +0x08
    double biteAngle = 0.0;        // +0x10
    double minDieDis = 0.0;        // +0x18
    std::vector<CogBite> bites;    // +0x20 (noBite entries, or 1 with autoBite)
    CogHeat ht;                    // +0x28 pass heat transfer
    int diePosMethod = 0;          // +0xa8
    double dieStrtPos = 0.0;       // +0xb0
    double dieStopPos = 0.0;       // +0xb8
    int dieStrtNod = 0;            // +0xc0
    int dieStopNod = 0;            // +0xc4
    int dieMovDir = -1;            // +0xc8
    double leftManDis = 0.0;       // +0xd0
    double rightManDis = 0.0;      // +0xd8
    double horizDieShift = 0.0;    // +0xe8
    int bitesToSkip = 0;           // +0xf0
    double radialFeedPerBite = 0.0;  // +0xf8
    double dwellTime = 0.0;        // +0x100
    int fe0 = 0;                   // +0xe0 P06: reset to 0 by 0x140791580
};

struct CogReheat {                 // REHEAT (400 bytes)
    int nPass = 0;                 // +0x00
    std::vector<CogPass> passes;   // +0x08
    CogHeat heat[3];               // +0x10 / +0x90 / +0x110 (not used by the writer)
};

struct CogBillet {                 // cog+0xd8 (reader 0x140796f90)
    int geoTyp = 0;                // +0x408
    double length = 0.0;           // +0x410
    double dia = 0.0;              // +0x418
    double inDia = 0.0;            // +0x420
    double height = 0.0;           // +0x428
    double width = 0.0;            // +0x430
    double radius = 0.0;           // +0x438
    double axialSub = 0.0;         // +0xc88
    int axialType = 0;             // +0xc80
    int doBilStraight = 0;         // +0x1090
    // P06:
    int geoMod = 0;                // +0x000 (ver > 0x27d7)
    std::string geoFile;           // +0x004 char[0x3ff] fgets, trailing newline removed
    double layers = 0.0;           // +0x440
    int mgnelm = 0;                // +0x858
    double mgsizr = 0.0;           // +0x860
    int mgtelm = 0;                // +0x868
    double tmpbil = 0.0;           // +0xc70
    int bccMod = 0;                // +0xc78
    int mergeL = 0;                // +0xc7c (also global 0x1437e3a7c)
    std::string keyBil;            // +0xc90 (also global 0x143db1600)
    int martMod = 0;               // +0x454 (ver > 0x27e1)
    int meshMod = 0;               // +0x450 (ver > 0x27eb)
};

struct CogDie {                    // cog+0x1170 (reader 0x140795760)
    double length = 0.0;           // +0x410
    int movType = 0;               // +0xc78
    double cycles = 0.0;           // +0xca0
    double totStroke = 0.0;        // +0xc90
    std::uint8_t four = 0;         // +0x14a8 (NoDies == 4; the reader stores an int 0/1)
    // P06:
    int geoTyp = 0;                // +0x408
    int geoMod = 0;                // +0x000 (ver > 0x27d7)
    std::string geoFile;           // +0x004
    double width = 0.0;            // +0x418
    double height = 0.0;           // +0x420
    double radius = 0.0;           // +0x428
    double layers = 0.0;           // +0x430
    int mgnelm = 0;                // +0x858
    double mgsizr = 0.0;           // +0x860
    int mgtelm = 0;                // +0x868
    int dieMov = 0;                // +0xc6c (copied from global 0x1437e3444)
    double tmpdie = 0.0;           // +0xc80
    double spddie = 0.0;           // +0xc88
    double rodLength = 0.0;        // +0xc98
    std::string keyDie[4];         // +0xca8 +0x10a8 +0x14ac +0x18ac
    int refPos = 0;                // +0xc70 (ver > 2999)
    double overlap = 0.0;          // +0x438 (ver > 0xc1b)
    double startPos = 0.0;         // +0x440 (ver > 0xc1d)
    int movDir = 0;                // +0xc74 (ver > 0xc1e)
    double stopPos = 0.0;          // +0x448 (ver > 0xc1f)
    int martMod = 0;               // +0x454
    int meshMod = 0;               // +0x450
};

struct CogManip {                  // cog+0x2e20 (reader 0x140793cc0, ver > 0xc1c)
    double length = 0.0;           // +0x410
    double startPos = 0.0;         // +0x458
    double leftDis = 0.0;          // +0x460
    double rightDis = 0.0;         // +0x468
    int useSpring = 0;             // +0x8b4
    // P06:
    int geoTyp = 0;                // +0x408
    int geoMod = 0;                // +0x000
    std::string geoFile;           // +0x004
    double width = 0.0;            // +0x418
    double height = 0.0;           // +0x420
    double radius3 = 0.0;          // +0x428
    double layers = 0.0;           // +0x450
    int mgnelm = 0;                // +0x8a0
    double mgsizr = 0.0;           // +0x8a8
    int mgtelm = 0;                // +0x8b0
    double tmpdie = 0.0;           // +0xcb8
    std::string keyMan[4];         // +0xcc0 +0x10c0 +0x14c0 +0x18c0
    int useMstMan = 0;             // +0x470 (read %f, stored (int))
    double stiffness = 0.0;        // +0x478
    double preload = 0.0;          // +0x480
    double maxDisp = 0.0;          // +0x488
    double width1 = 0.0;           // +0x440
    double height1 = 0.0;          // +0x448
    double radius1 = 0.0;          // +0x430
    double radius2 = 0.0;          // +0x438
    double f490 = 45.0;            // +0x490 (ctor default, never read from the MDT; primitive parameter 8)
    int martMod = 0;               // +0x49c
    int meshMod = 0;               // +0x498
};

struct CogData {                   // SfOprCogData (ctor 0x140767a20)
    std::string prbid = "COGGING"; // +0x000
    std::string title = "COGGING"; // +0x064
    std::uint8_t useManip = 1;     // +0x0c8
    int nReheats = 1;              // +0x0cc (reheats.size() == max(nReheats, 0))
    std::vector<CogReheat> reheats;  // +0x0d0
    CogBillet billet;              // +0x0d8
    CogDie die;                    // +0x1170
    CogManip manip;                // +0x2e20
};

struct CogModel {                  // 0x550 bytes, ctor 0x140761310, defaults 0x1407613b0/0x1407614e0
    int processType = 0;           // +0x000 Process_Type
    int temperatureType = 1;       // +0x004 Temperaure_Type (0x1407614e0 sets 1)
    std::uint8_t useMandrel = 0;   // +0x008 Use_Mandrel
    std::uint8_t rotationalSymmetry = 0;  // +0x015 RotationalSymmetry (byte as read)
    std::uint8_t useAdaptiveReheat = 0;   // +0x4cc
    int checkingMethod = 0;        // +0x4d0
    double tempRange[2] = {0.0, 0.0};     // +0x4d8 / +0x4e0
    double adaptiveHeatingTime = 0.0;     // +0x4e8
    double swagingStroke = 0.0;    // +0x4f8
    int numStepsDsmax = 4;         // +0x500
    std::uint8_t heatTransfer = 0; // +0x504
    std::uint8_t remainingCycleTimeDwell = 0;  // +0x505 (byte as read)
    double cycleTime = 0.0;        // +0x508 (low byte overwritten by the reader, s1-03 5)
    double dwellTime = 0.0;        // +0x510
    double decompressionTime = 0.0;  // +0x518
    int bottomFixed = 0;           // +0x520
    std::uint8_t byte538 = 0;      // +0x538 (never set; DSMAX_NSTEP gate)
    CogData cog;                   // +0x540
    // P06: the rest of the scalar block as the reader leaves it (ints decoded from its bytes)
    int useMandrelInt = 0;         // +0x008 as int (%d writes 4 bytes; useMandrel is its low byte)
    int f0c = 0;                   // +0x00c
    int f10 = 0;                   // +0x010
    int useDifferentDies = 0;      // +0x014 as int, after RotationalSymmetry's %d overwrote +0x15..
    int f18 = 1;                   // +0x018 (low byte overwritten by RotationalSymmetry bits 24..31)
    int solverType = 0;            // +0x4f0 Solver_Type
    double clampForce = 0.0;       // +0x528 Clamp_Force
    double clampSpeed = 0.0;       // +0x530 Clamp_Speed
    int mdtVersion = 0;            // "* MASTER DATA FILE %d" of the last read (not in the original)
};

// Process globals of the cogging module that the Cogging.MDT writer 0x1407617e0 reads (packet MDT,
// docs/re/moproj2mst/mdt/spec.md section 2). The reader and the SfOprCogData ctor set them; with
// several cogging operations every MDT the Save writes gets the values of the last read.
struct CogProcess {
    CogHeat slot[5];               // 0x143db1a20 + k*0x80 (writer: slots 0..2)
    CogPass defaultPass;           // 0x143da7e00
    CogBite defaultBite;           // 0x143da7cc0
    double poserr = 0.001;         // 0x1437e4040 (.data)
    double interr = 0.001;         // 0x1437e4048 (.data)
    double cvgerr[2] = {0.0, 0.0}; // 0x143da6dd0 / dd8 (.bss; ctor 0x14076b3d0: 0.005 / 0.05)
    double avgstr = 0.0;           // 0x143da6df8 (.bss; ctor: 1.0)
    double lmtstr = 0.0;           // 0x143da6df0 (.bss; ctor: 0.01)
    double ctlpct = 50.0;          // 0x1437e3a80 (.data)
};

}  // namespace m2m::data
