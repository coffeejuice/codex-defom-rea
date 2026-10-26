#pragma once
// Keyword registry record, as laid out at 0x140544400 in DEF_PRE_64.exe (0x30 bytes per record).

#include <cstdint>

namespace def::model {

inline constexpr int kKeywordCount = 0x130;  // 304 records; loop bound in FUN_1402f78d0/FUN_1402f7950

// Group code (+0x10). FUN_1401b6520 dispatches on group - 1 to one parser per group; any other
// value is a "Data Error" (return 3).
enum class KeywordGroup : std::int32_t {
    SimulationControl = 1,    // FUN_14039aa90
    Object = 2,               // FUN_14038b8a0
    InterObject = 3,          // FUN_140388fb0
    Material = 4,             // FUN_140384a50
    PhaseTransformation = 5,  // FUN_140383120
    Command = 6,              // FUN_140378e00
    Dimension = 7,            // FUN_14037e530
};

struct KeywordRecord {
    std::int32_t id;      // +0x00; ids 0..303, a permutation of the table positions
    const char* name;     // +0x08; compared with strcmp (case-sensitive, exact)
    KeywordGroup group;   // +0x10
    std::int32_t flag;    // +0x14; read only by FUN_1402f77e0, which nothing calls
    // Handler slots, kept as the original's addresses until the handlers are ported.
    std::uint64_t handler1;  // +0x18; FUN_1402f7590 in every record (returns 0); read by FUN_1402f7830
    std::uint64_t handler2;  // +0x20; KEY write handler, called by FUN_1402f79c0; default FUN_1402f75d0
    std::uint64_t handler3;  // +0x28; axis label handler, called by FUN_1402f7a70; default FUN_1402f7610;
                             // 0 (no handler) for ZSTR
};

inline constexpr std::uint64_t kDefaultHandler1 = 0x1402F7590;  // returns 0
inline constexpr std::uint64_t kDefaultHandler2 = 0x1402F75D0;  // returns 0
inline constexpr std::uint64_t kDefaultHandler3 = 0x1402F7610;  // returns -1

}  // namespace def::model
