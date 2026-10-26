// m2m/data/heattreat.hpp: SfOprHeatTreatProperties payload, the heat-treatment schedule
// (PROPS+0x68, 0x38 bytes, ctor 0x1411b3440). Owner: MDT L6 (the reader 0x1411b4770 / 0x1411b4870 /
// 0x1411b4ab0 / 0x1411b5e50 in src/ops/heattreat_htwz.cpp); P16's MST writer and P14's simulation
// list read it. Layouts from the reader and the ctors named below (docs/re/moproj2mst/mdt/L6).
#pragma once

#include <memory>
#include <vector>

#include <defmodel/layout/dynary.hpp>

#include "m2m/qt4.hpp"

namespace m2m {
struct Props;
}

namespace m2m::data {

using def::model::DYNARY;

// Node-value record of a medium (ECC +0x10 list and ECTMFN +0x18 list of the schedule; 0x10 bytes,
// ctor 0x1411c2ae0). +0x04 is not set by the ctor; every path that makes a record sets it.
struct HTNodeEntry {
    int object = 0;                    // +0x00 object number (0x140d33b00 / 0x1413ee240)
    int media = 0;                     // +0x04 medium number (0x1413fd9d0 / 0x14142ff20)
    std::vector<std::pair<int, int>> values;   // +0x08 QList of (node, value) (0x1411c3260)
};

// Quenchant medium (0x90 bytes, ctor 0x1411b3360: name "Media", radiation on, one zone).
struct HTMedia {
    HTMedia();
    qt::String name = qt::String(u"Media");    // +0x00 (0x140dfbd00)
    bool radiation = true;             // +0x08 RADIATION != 0 (0x1411c2b10)
    DYNARY acvcof;                     // +0x10 ACVCOF table, type 0x1e (0x1412c0730)
    std::vector<std::unique_ptr<DYNARY>> zones;   // +0x88 CNVCOF table per zone, type 1
};

// Process step (0x140 bytes, ctor 0x1411b3020). +0x08 (media) is not set by the ctor; 0 here.
struct HTStep {
    double time = 0.0;                 // +0x00 PROC token 1 (0x1411c2b30: kept unless >= 0.0)
    int media = 0;                     // +0x08 PROC token 2 - 1 (0x141371aa0)
    double t3 = 0.0;                   // +0x10 PROC token 3 (0x1411c2b60, also t20.value)
    double t4 = 0.0;                   // +0x18 PROC token 4 (0x1411c2b90: kept unless >= 0.0)
    DYNARY t20;                        // +0x20 table (0x140a99e10), PROC token 5 rows
    DYNARY t98;                        // +0x98 table (0x1411c24c0), PROC token 6 rows
    bool flag7 = false;                // +0x110 PROC token 7 (0x1411c2bd0)
    bool flag10 = false;               // +0x111 PROC token 10 (0x1411c2bf0)
    qt::String userKey = qt::String(u"None");   // +0x118 user keyword file (0x1411c2c10)
    // +0x120 list of 0x780-byte records (0x1409dc8b0, ctor 0x1409d6a20); the reader sets their
    // first three ints only (a, b, -1); the other members keep the ctor's defaults
    struct Pair {
        int a = 0;
        int b = 0;
        int c = -1;
    };
    std::vector<Pair> pairs;
};

struct HTSchedule {                    // PROPS+0x68 (0x38 bytes, ctor 0x1411b3440)
    bool engineDefaults = false;       // +0x00 set by 0x1411b3540 once the defaults are applied
    std::vector<std::unique_ptr<HTMedia>> media;   // +0x08 (0x140a9a450)
    std::vector<std::unique_ptr<HTNodeEntry>> ecc;      // +0x10 (0x1411c2c90)
    std::vector<std::unique_ptr<HTNodeEntry>> ectmfn;   // +0x18 (0x1411c28c0)
    int first = 1;                     // +0x20 first step to run (1-based), START_OPTNO
    std::vector<std::unique_ptr<HTStep>> steps;   // +0x28 QList<HTStep*>
    Props* owner = nullptr;            // +0x30 owner properties (their OPI gives the task folder)
};

struct HeatTreatData {
    HTSchedule schedule;               // PROPS+0x68 (0x141681de0)
};

// @src 0x1411b0a70  number of process steps (count of schedule+0x28); also used by P14's
// HeatTreat simulation lists (0x1411b2790 / 0x1411b0170)
inline int htStepCount(const HTSchedule& s)
{
    return static_cast<int>(s.steps.size());
}

}  // namespace m2m::data
