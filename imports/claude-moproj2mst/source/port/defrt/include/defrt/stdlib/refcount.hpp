// defrt/stdlib/refcount.hpp: small helpers placed in DEF_PRE_64's runtime range that many model
// classes call (packet p1-01).
//
// The copy-on-write array handles that use the reference count are ported by p1-05
// (def::model::CowVector / CowList in defmodel/sdctl/cow.hpp).
#pragma once

#include <cstdint>

namespace def::rt {

// @src 140001000  intrusive reference count: initialise to 1 (35 callers in Phase 1)
inline std::int32_t* ref_init(std::int32_t* refs)
{
    *refs = 1;
    return refs;
}

// @src 140001010  intrusive reference count: drop one reference; true when none is left
inline bool ref_release(std::int32_t* refs)
{
    *refs = *refs - 1;
    return *refs == 0;
}

// @src 1400015e0  the dimension word at +0x1c of an object is 2 (called on DB_STEP_DATA's +0xbc0)
inline bool dimension_is_2(const void* obj)
{
    return *reinterpret_cast<const std::int32_t*>(static_cast<const char*>(obj) + 0x1c) == 2;
}

// @src 1400015f0  ... is 3
inline bool dimension_is_3(const void* obj)
{
    return *reinterpret_cast<const std::int32_t*>(static_cast<const char*>(obj) + 0x1c) == 3;
}

} // namespace def::rt
