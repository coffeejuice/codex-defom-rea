// Fakes for the p1-07 tier-1 tests: stand-ins for the pending cross-packet calls
// (defmodel/geo/pending.hpp). Each fake records what it was given; nothing here decides an
// expected value.
#pragma once

#include <cstdint>
#include <new>
#include <vector>

#include <defmodel/geo/pending.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/obj_geo_base.hpp>

namespace geofake {

using namespace def::model;

struct Calls {
    int ext_ctor = 0;
    int ext_dtor = 0;
    int ext_copy = 0;
    int ext_notify = 0;
    std::vector<std::uint64_t> seq; // addresses in call order
};
inline Calls calls;

// Stand-in for OBJ_GEO2D_DIGITIZED (0x50 bytes, owner pointer at 0x10).
struct FakeExt : OBJ_GEO_BASE {
    OBJ_GEO2D* owner = nullptr; // 0x10
    std::byte rest[0x38];
    ~FakeExt() override { ++calls.ext_dtor; }
    const char* class_name() override { return "fake"; }
    void copy_from(OBJ_GEO_BASE*) override {}
    void reverse() override {}
    void scale(const double*) override {}
    void translate(const double*) override {}
    void rotate(const double*, const void*, double) override {}
    double char_length() override { return 0.0; }
    void notify() override { ++calls.ext_notify; }
    std::int32_t is_extension() override { return 1; }
};
static_assert(sizeof(FakeExt) == 0x50);

inline OBJ_GEO2D_DIGITIZED* ext_ctor(void* mem)
{
    ++calls.ext_ctor;
    return reinterpret_cast<OBJ_GEO2D_DIGITIZED*>(new (mem) FakeExt());
}
inline void ext_copy(OBJ_GEO2D_DIGITIZED*, OBJ_GEO2D_DIGITIZED*) { ++calls.ext_copy; }

inline void install_ext()
{
    pending::fun_14041dcc0 = &ext_ctor;
    pending::fun_14041e090 = &ext_copy;
}

inline void reset()
{
    pending::reset_pending();
    calls = Calls{};
}

} // namespace geofake
