// OBJ_GEO3D = RTTI .?AV_OBJ_GEO3D@@ (_OBJ_GEO3D, base OBJ_GEO_BASE): a 3D surface mesh: node
// coordinates (3 doubles each) and 4-node elements (triangles repeat a node), with per-element
// flags and ids. Embedded twice in LLOBJ_DATA (0x3328, 0x3458).
//
//   vtable   0x140548d18 (12 slots)
//   ctor     1402f9020 (P2 in the ledger; ported here, listed as an extra address)
//   dtor     1402f91f0; slot 0 scalar deleting dtor 1402fe990
//   size     0x130 (the two LLOBJ_DATA instances are 0x130 apart)
//   open     roles of 0x28 (p1-06 point list), 0x40, 0x48, 0x50/0x58 (p1-09 lists), 0x60,
//            0x70, 0x78, 0x80 (p1-14 tree); element types of 0x50/0x58.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/obj_geo_base.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {

class OBJ_GEO3D : public OBJ_GEO_BASE {
public:
    // @src 1402f9020  ctor
    OBJ_GEO3D();
    OBJ_GEO3D(const OBJ_GEO3D&) = delete;
    OBJ_GEO3D& operator=(const OBJ_GEO3D&) = delete;

    // @src 1402fe990  slot 0: scalar deleting dtor (dtor body 1402f91f0)
    ~OBJ_GEO3D() override;
    // @src 1402fe970  slot 1: "_OBJ_GEO3D"
    const char* class_name() override;
    // @src 1402f92b0  slot 2: clear nodes and elements, flags pointer 0 (not freed), ...
    void reset() override;
    // slot 3: clear_data, OBJ_GEO_BASE's no-op (14027f300)
    // @src 1402f9dc0  slot 4: -> assign 1402f9b60 (no null check)
    void copy_from(OBJ_GEO_BASE* other) override;
    // @src 1402fce60  slot 5: -> reverse_elements 1402fbf60
    void reverse() override;
    // @src 1402fb3a0  slot 6
    void scale(const double* factor) override;
    // @src 1402fb480  slot 7
    void translate(const double* offset) override;
    // @src 1402fb550  slot 8: angle in degrees about the axis through center
    void rotate(const double* center, const void* axis, double angle) override;
    // @src 1402fb120  slot 9: bounding-box diagonal (0 without nodes or elements)
    double char_length() override;
    // @src 1402fb690  slot 10: no-op
    void notify() override;
    // @src 1402fe980  slot 11: 100
    std::int32_t is_extension() override;

    // @src 1400034d0  node count: coords size / 3
    std::int32_t node_count() const;
    // @src 140003500  element count: elems size / 4
    std::int32_t elem_count() const;
    // @src 1402f9340  -> clear_all 1402fcde0
    void clear();
    // @src 1402fcde0  clear every member
    void clear_all();
    // @src 1402f9600  coords.resize(n * 3, 0); returns the first coordinate (0 when n < 1)
    double* resize_nodes(const std::int32_t* n);
    // @src 1402f9670  coords.clear()
    void clear_nodes();
    // @src 1402f96a0  elems.resize(n << 2, 0); returns the first entry (0 when n < 1)
    std::int32_t* resize_elems(const std::int32_t* n);
    // @src 1402f9710  elems.clear()
    void clear_elems();
    // @src 1402f9970  new zeroed elem_count() ints at 0x20 (unchanged when there are none)
    std::int32_t* alloc_elem_flags();
    // @src 1402f9a00  free 0x20
    void free_elem_flags();
    // @src 1402f9a50  ids.resize(elem_count(), 0); returns the first id (0 when none)
    std::int32_t* resize_elem_ids();
    // @src 1402f9ac0  ids.clear()
    void clear_elem_ids();
    // @src 1402f9b60  copy nodes, elements, flags, ids and the point list
    void assign(OBJ_GEO3D* other);
    // @src 1402fbf60  reverse each element's node order (a negative 4th node stays last)
    void reverse_elements();
    // @src 1402fbb50  0x48 = *v
    void set_u48(const std::int32_t* v);
    // @src 1402fbba0  0x60 = *v
    void set_u60(const std::int32_t* v);

    // vptr, OBJ_GEO_BASE 0x08..0x0f
    CowVector<double> coords;         // 0x010 x, y, z per node
    CowVector<std::int32_t> elems;    // 0x018 4 node numbers per element
    std::int32_t* elem_flags;         // 0x020 elem_count() ints (operator new / free)
    std::byte u28[0x18];              // 0x028 point list (p1-06: ctor 140257bf0)
    CowList<std::int32_t> u40;        // 0x040 int list (ctor 140159860, p1-04)
    std::int32_t u48;                 // 0x048
    std::byte pad_04c[4];             // 0x04c
    void* u50;                        // 0x050 list handle (p1-09: ctor 1402fedd0)
    void* u58;                        // 0x058 list handle (p1-09: ctor 1402fef20)
    std::int32_t u60;                 // 0x060
    std::byte pad_064[4];             // 0x064
    CowVector<std::int32_t> elem_ids; // 0x068 one per element
    bool u70;                         // 0x070
    std::byte pad_071[7];             // 0x071
    void* u78;                        // 0x078
    std::byte u80[0xb0];              // 0x080 Cls_14042a870 (p1-14: ctor 14042a7e0)
};

static_assert(sizeof(OBJ_GEO3D) == 0x130);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, coords, 0x10);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, elems, 0x18);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, elem_flags, 0x20);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u28, 0x28);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u40, 0x40);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u48, 0x48);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u50, 0x50);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u60, 0x60);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, elem_ids, 0x68);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u70, 0x70);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u78, 0x78);
DEF_LAYOUT_OFFSET(OBJ_GEO3D, u80, 0x80);

} // namespace def::model
