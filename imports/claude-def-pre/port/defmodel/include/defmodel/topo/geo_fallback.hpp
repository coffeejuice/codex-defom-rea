// defmodel/topo/geo_fallback.hpp: OBJ_GEO_BASE and _OBJ_GEO2D as the topology code needs them.
//
// Both classes belong to p1-07 (layout/obj_geo_base.hpp, layout/obj_geo2d.hpp). When those headers
// exist they are used and nothing here is compiled. Until then this header declares the minimum
// the p1-11 code calls, at the binary's offsets, so that p1-11 builds and its tier-1 tests run;
// the bodies (src/topo/geo_fallback.cpp) abort in DEF_NOT_PORTED except trivial ones.
//
// Names p1-11 relies on (p1-07 is asked to use them, or to rename here when merging):
//   OBJ_GEO_BASE slots: 0 dtor, 1 class_name, 2 reset, 3 clear_data, 4 copy_from, 5 reverse,
//     6 scale, 7 translate, 8 rotate, 9 char_length, 10 notify, 11 is_extension
//     (vtable 0x140537aa8; _OBJ_GEO2D 0x140537a38; OBJ_GEO2D_DIGITIZED 0x1405681d8)
//   OBJ_GEO2D: curve_type (+0x10), n_points (+0x14), points (+0x18), ext (+0x20), flag28 (+0x28);
//     clear_all 140275e90, set_curve_type 1402763d0, fun_1402765e0, append 140276e30,
//     is_ccw 1402789f0, contains_bbox 140278ed0, set_flag28 14027f410, get_flag28 14041ce20
//     (p1-11 function), copy_from body 140276230, reverse body 1402567c0.
#pragma once

#include <cstddef>
#include <cstdint>

#if __has_include(<defmodel/layout/obj_geo_base.hpp>)
#include <defmodel/layout/obj_geo_base.hpp>
#define DEF_P1_11_GEO_BASE_FALLBACK 0
#else
#define DEF_P1_11_GEO_BASE_FALLBACK 1
#endif

#if __has_include(<defmodel/layout/obj_geo2d.hpp>)
#include <defmodel/layout/obj_geo2d.hpp>
#define DEF_P1_11_GEO2D_FALLBACK 0
#else
#define DEF_P1_11_GEO2D_FALLBACK 1
#endif

#include <defmodel/layout/layout_check.hpp>

namespace def::model {

#if DEF_P1_11_GEO_BASE_FALLBACK
// fallback of OBJ_GEO_BASE (p1-07): vtable 0x140537aa8, ctor 14027f200, dtor via 14027f320
class OBJ_GEO_BASE {
public:
    OBJ_GEO_BASE() = default;                                  // 14027f200: u0c = 0
    virtual ~OBJ_GEO_BASE() = default;                         // slot 0
    virtual const char* class_name() { return "OBJ_GEO_BASE"; } // slot 1 (14027f310)
    virtual void reset() {}                                    // slot 2 (14027f2f0, empty)
    virtual void clear_data() {}                               // slot 3 (14027f300, empty)
    virtual void copy_from(OBJ_GEO_BASE* other) = 0;           // slot 4 (pure)
    virtual void reverse() = 0;                                // slot 5
    virtual void scale(const double* f) = 0;                   // slot 6
    virtual void translate(const double* d) = 0;               // slot 7
    virtual void rotate(const double* center, const void* unused, double angle) = 0; // slot 8
    virtual double char_length() = 0;                          // slot 9
    virtual void notify() = 0;                                 // slot 10
    virtual std::int32_t is_extension() = 0;                   // slot 11

    std::int32_t u08 = 0;                                      // 0x08 (not written by the ctor)
    std::int32_t u0c = 0;                                      // 0x0c =0
};
static_assert(sizeof(OBJ_GEO_BASE) == 0x10);
#endif

#if DEF_P1_11_GEO2D_FALLBACK
class OBJ_GEO2D_DIGITIZED;

// fallback of _OBJ_GEO2D (p1-07): vtable 0x140537a38, size 0x30 (1404126d0 keeps two on its stack)
class OBJ_GEO2D : public OBJ_GEO_BASE {
public:
    OBJ_GEO2D();                                               // 140275cd0
    ~OBJ_GEO2D() override;                                     // 140275e00
    const char* class_name() override { return "_OBJ_GEO2D"; } // 14027f380
    void reset() override;                                     // 140275e50 (slot 2)
    void clear_data() override;                                // 14027f360 (slot 3)
    void copy_from(OBJ_GEO_BASE* other) override;              // 140276230
    void reverse() override;                                   // 1402567c0
    void scale(const double* f) override;                      // 140277a20
    void translate(const double* d) override;                  // 140277b00
    void rotate(const double* center, const void* unused, double angle) override; // 140277c50
    double char_length() override;                             // 140278f70
    void notify() override;                                    // 140277970
    std::int32_t is_extension() override { return 0; }         // 14027f390

    void clear_all();                                          // 140275e90
    std::int64_t set_curve_type(std::int32_t t);               // 1402763d0
    void fun_1402765e0();                                      // 1402765e0
    void append(OBJ_GEO2D* other);                             // 140276e30
    bool is_ccw();                                             // 1402789f0
    bool contains_bbox(OBJ_GEO2D* other);                      // 140278ed0: this box inside other
    void set_flag28(bool v) { flag28 = v ? 1 : 0; }            // 14027f410
    bool get_flag28() const;                                   // 14041ce20 (p1-11)

    std::int32_t curve_type = 0;                               // 0x10
    std::int32_t n_points = 0;                                 // 0x14
    double* points = nullptr;                                  // 0x18
    OBJ_GEO2D_DIGITIZED* ext = nullptr;                        // 0x20
    std::uint8_t flag28 = 0;                                   // 0x28
};
static_assert(sizeof(OBJ_GEO2D) == 0x30);
DEF_LAYOUT_OFFSET(OBJ_GEO2D, curve_type, 0x10);
DEF_LAYOUT_OFFSET(OBJ_GEO2D, flag28, 0x28);
#endif

} // namespace def::model
