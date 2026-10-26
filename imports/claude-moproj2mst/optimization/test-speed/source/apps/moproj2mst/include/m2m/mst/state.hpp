// m2m/mst/state.hpp: the MST writers' process globals as per-call state (s1-02 8) (owner: G0).
// The original never resets them at the end of a file; a conversion starts from the .bss values.
#pragma once

#include <map>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::doe { class Model; }

namespace m2m::mst {
using qt::String;

struct State {
    int cycleIdx = 0;              // 0x143d80250  CYCL<n>
    int cycleCursim = 0;           // 0x143d80254  CURSIM middle value in cycle mode
    bool doeMode = false;          // 0x143d80258
    bool kfwrit = false;           // 0x143d80259  GENDB -> KFWRIT (never set on Save)
    bool cycleMode = false;        // 0x143d8025a
    doe::Model* doeModel = nullptr;   // 0x143d80260
    int doeOpId = 0;               // 0x143d80268
    std::vector<void*> doeVarList; // 0x143d80270
    String kfwritPath;             // 0x143d80278
    String cycleOprName;           // 0x143d80280
    std::vector<char> objMaskCur;  // 0x143d80288
    std::vector<char> objMaskPrev; // 0x143d80290
    std::map<int, int> doeObjMap;  // 0x143d80298

    // @src 0x140131390  clear both object masks (start of each file)
    void clearMasks()
    {
        objMaskCur.clear();
        objMaskPrev.clear();
    }
    // @src 0x140136af0  (ord, name): cycle context on (s1-04 2)
    void setCycleMode(int ord, const String& name)
    {
        cycleCursim = ord;
        cycleOprName = name;
        cycleMode = true;
    }
    // @src 0x140136b30  leave cycle mode
    void clearCycleMode() { cycleMode = false; }
    // @src 0x140136b40  cycle index for SIMNAM "CYCL<n>-"
    void setCycleIdx(int n) { cycleIdx = n; }
    // @src 0x140136ab0 / 0x140136ae0  KFWRIT on/off (not on the Save path)
    void setKfwrit(const String& path)
    {
        kfwrit = true;
        kfwritPath = path;
    }
    void clearKfwrit() { kfwrit = false; }
};

}  // namespace m2m::mst
