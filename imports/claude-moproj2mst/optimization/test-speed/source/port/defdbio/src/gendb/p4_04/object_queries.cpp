// Object queries used by the simulation control checks (packet p4-04). They read LLOBJ_DATA
// fields inside sub-objects the layout keeps opaque; the offsets are the decompile's:
//   +0x008 object number (1401d6ea0 looks objects up by it)    +0x00c object type (0 rigid,
//   1 plastic, 2 porous, 3 elastic, 4 elasto-plastic, 7 skipped by 140160f10)
//   +0x87c mesh present   +0x880 element count   +0x884 element-material count
//   +0x890 mesh kind      +0x990 BCC codes (int*, element-major, s.u15f8[18] per element)
//   +0x9f8 element material numbers (int*)   +0x2154 mesh type   +0x21c8 movement type
//   +0x22c0 followed object (position)   +0x22c8 follow ratio   +0x2788 / +0x28b8 rotation types
#include <cstdlib>
#include <vector>

#include <defdbio/gendb/checks_p4_04.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "ext_p4_04.hpp"

namespace def::dbio::gendb {

namespace {
using def::model::field;
std::int32_t i32(const LLOBJ_DATA* o, std::size_t off) { return field<std::int32_t>(o, off); }
LLOBJ_DATA* object_at(DB_STEP_DATA& s, std::int32_t n) // 1401d6ed0
{
    return static_cast<LLOBJ_DATA*>(def::model::step_object_at(s, n));
}
bool usable(const LLOBJ_DATA* o) { return o != nullptr && ext::fun_140160f10(o); }
} // namespace

// @src 1401bed60
std::int32_t count_elasto_plastic(DB_STEP_DATA& s)
{
    std::int32_t n = 0;
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && i32(o, 0xc) == 4) {
            n = n + 1;
        }
    }
    return n;
}

// @src 1401bee00
bool any_movement_type_5(DB_STEP_DATA& s)
{
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && i32(o, 0x21c8) == 5) return true;
    }
    return false;
}

// @src 1401bee90
bool any_force_movement(DB_STEP_DATA& s)
{
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && i32(o, 0x21c8) == 0 && ext::fun_1401792d0(o)) return true;
    }
    return false;
}

// @src 1401bef30
bool any_torque_movement(DB_STEP_DATA& s)
{
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o)) {
            if (i32(o, 0x2788) == 0 && ext::fun_1401797e0(o)) return true;
            if (def::rt::dimension_is_3(&s) && i32(o, 0x28b8) == 0 && ext::fun_1401797e0(o)) return true;
        }
    }
    return false;
}

// @src 1401bf230
void inter_obj_and_rigid_movement(DB_STEP_DATA& s, std::uint8_t* any_relation, std::uint8_t* rigid_moving)
{
    *any_relation = 0;
    *rigid_moving = 0;
    if (ext::fun_1400015d0(&s.inter_obj) != 0) {
        for (std::int32_t i = 0; i < ext::fun_1400015d0(&s.inter_obj); ++i) {
            Cls_1401f46e0* r = ext::fun_1401fac90(&s.inter_obj, i + 1);
            if (r != nullptr && r->u170 != 0) {
                *any_relation = 1;
                break;
            }
        }
        for (std::int32_t i = 0; i < s.u1378; ++i) {
            LLOBJ_DATA* o = object_at(s, i + 1);
            if (usable(o) && i32(o, 0xc) == 0 && ext::fun_140179280(o)) {
                *rigid_moving = 1;
                return;
            }
        }
    }
}

// @src 1401bf380  (s + 0x8b8 is the simulation control's PDIE 1)
bool primary_die_moves(DB_STEP_DATA& s)
{
    const std::int32_t pdie = s.sim_ctrl.pdie1;
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        if (pdie == i + 1) {
            LLOBJ_DATA* o = object_at(s, pdie);
            if (o == nullptr) return false;
            if (!ext::fun_140160f10(o)) return false;
            if (ext::fun_1401792d0(o)) return true;
        }
    }
    return false;
}

// @src 1401bf460
bool primary_die_follows(DB_STEP_DATA& s, std::int32_t* followed)
{
    const std::int32_t pdie = s.sim_ctrl.pdie1;
    *followed = 0;
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        if (pdie == i + 1) {
            LLOBJ_DATA* o = object_at(s, pdie);
            if (o == nullptr) return false;
            if (!ext::fun_140160f10(o)) return false;
            LLOBJ_DATA* f = object_at(s, i32(o, 0x22c0));
            if (f == nullptr || !ext::fun_140160f10(f)) return false;
            *followed = i32(o, 0x22c0);
            if (ext::fun_1401792d0(f) && field<double>(o, 0x22c8) != 0.0) return true; // 1401bf58f UCOMISD
        }
    }
    return false;
}

// @src 1401bf730
std::int32_t count_objects_of_type(DB_STEP_DATA& s, const std::int32_t* type)
{
    std::int32_t n = 0;
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && i32(o, 0xc) == *type) {
            n = n + 1;
        }
    }
    return n;
}

// @src 1401bf8c0
bool any_mesh_kind(DB_STEP_DATA& s, bool below_4)
{
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && i32(o, 0x87c) != 0) {
            if (!below_4) {
                if (i32(o, 0x890) == 4) return true;
            } else if (i32(o, 0x890) < 4) {
                return true;
            }
        }
    }
    return false;
}

// @src 1401bf990
bool any_elasto_plastic_mesh_type(DB_STEP_DATA& s, const std::int32_t* kind)
{
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && i32(o, 0x87c) != 0 && i32(o, 0xc) == 4 && i32(o, 0x2154) == *kind) return true;
    }
    return false;
}

// @src 1401bfbc0
bool any_rotational_symmetry_bcc(DB_STEP_DATA& s)
{
    if (def::rt::dimension_is_2(&s)) {
        return false;
    }
    const std::int32_t ncol = s.u15f8[(0x1640 - 0x15f8) / 4]; // s + 0x1640
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && i32(o, 0x87c) != 0) {
            for (std::int32_t e = 0; e < i32(o, 0x880); ++e) {
                for (std::int32_t k = 0; k < s.u15f8[(0x1640 - 0x15f8) / 4]; ++k) {
                    const std::int32_t* bcc = field<std::int32_t*>(o, 0x990);
                    const std::int32_t v = bcc[e * ncol + k];
                    if (v == 7 || v == 8) return true;
                }
            }
        }
    }
    return false;
}

// @src 1401c5500
bool any_type_4(DB_STEP_DATA& s)
{
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (o != nullptr && i32(o, 0xc) == 4) return true;
    }
    return false;
}

// @src 1401c5590
bool any_mechanical_press(DB_STEP_DATA& s)
{
    for (std::int32_t i = 0; i < s.u1378; ++i) {
        LLOBJ_DATA* o = object_at(s, i + 1);
        if (usable(o) && movement_type_4_defined(o)) return true;
    }
    return false;
}

// @src 14017a130
bool movement_type_4_defined(LLOBJ_DATA* obj)
{
    if (i32(obj, 0x21c8) == 4) {
        return ext::fun_1401792d0(obj);
    }
    return false;
}

// @src 1401fb840  The binary keeps the per-value counts in a calloc(10, 4) block that it never
// grows (only the value block is realloc'ed), and the index of the largest count it computes is
// not used; the counts are not observable, so they are not kept here.
bool object_material_ids(LLOBJ_DATA* obj, std::int32_t* n, std::int32_t* out, const std::int32_t* cap,
                         DB_STEP_DATA*)
{
    *n = 0;
    if (i32(obj, 0x87c) == 0) return false;
    const std::int32_t* ids = field<std::int32_t*>(obj, 0x9f8);
    if (ids == nullptr) return false;
    std::vector<std::int32_t> seen;
    seen.reserve(10);
    for (std::int32_t i = 0; i < i32(obj, 0x884); ++i) {
        const std::int32_t v = ids[i];
        bool found = false;
        for (std::int32_t k = 0; k < static_cast<std::int32_t>(seen.size()); ++k) {
            if (v == seen[k]) {
                found = true;
                break;
            }
        }
        if (!found) seen.push_back(v);
    }
    std::int32_t count = static_cast<std::int32_t>(seen.size());
    *n = count;
    if (*cap < count) count = *cap;
    for (std::int32_t i = 0; i < count; ++i) out[i] = seen[i];
    return true;
}

// @src 140202a00
std::int32_t object_u3980(const LLOBJ_DATA* obj) { return field<std::int32_t>(obj, 0x3980); }

} // namespace def::dbio::gendb
