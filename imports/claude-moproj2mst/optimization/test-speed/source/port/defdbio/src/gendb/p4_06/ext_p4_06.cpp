// Local copies and hook dispatch of defdbio/gendb/ext_p4_06.hpp (packet p4-06). Each body is
// ported from the decompile of the owner's function; the merge replaces it with the owner's call.
#include <defdbio/gendb/ext_p4_06.hpp>

#include <bit>

#include <defdbio/gendb/counts_frame.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defkey/gendb_deps.hpp>
#include <defkey/kw_g4a.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/window_list.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb3d/obj3d.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include "../../gendb3d/bclists.hpp"

#include <cstdio>
#include <vector>

#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/dynary_init.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::dbio::gendb::ext {

namespace {

using model::Cls_14017a6c0;
std::int32_t I(std::uint64_t v) { return static_cast<std::int32_t>(v); }
template <class T> T* P(std::uint64_t v) { return reinterpret_cast<T*>(static_cast<std::uintptr_t>(v)); }
double D(std::uint64_t v) { return std::bit_cast<double>(v); }
std::uint64_t B(bool b) { return b ? 1u : 0u; }

// The owners of the hooked calls (program build); g_hook replaces them in tests.
std::uint64_t owner_call(std::uint64_t addr, const std::uint64_t* a)
{
    using namespace def::dbio::g3d;
    switch (addr) {
    case 0x1401982c0: return u(model::geo_kw::curve(*P<LLOBJ_DATA>(a[0]), *P<const std::int32_t>(a[1])));
    case 0x140198310: return u(model::geo_kw::curve_count(*P<LLOBJ_DATA>(a[0])));
    case 0x140160cc0: return B(::def::dbio::gendb::object_has_geometry(P<LLOBJ_DATA>(a[0])));
    case 0x140160d20: return B(model::geo_kw::geometry_has_edges(*P<LLOBJ_DATA>(a[0])));
    case 0x1401808d0: return u(objcheck::node_block_coords(P<void>(a[0])));
    case 0x140183cb0: return u(element_connectivity(P<void>(a[0])));
    case 0x140165230: return u(bc_function_capacity(P<LLOBJ_DATA>(a[0])));
    case 0x140166e60:
        return B(nearest_loop_node(P<LLOBJ_DATA>(a[0]), D(a[1]), D(a[2]), P<std::int32_t>(a[3]),
                                   P<std::int32_t>(a[4]), P<double>(a[5]), P<double>(a[6])));
    case 0x140203980: return B(object_has_geometry(P<LLOBJ_DATA>(a[0])));
    case 0x14016f8e0:
        write_node_info(P<LLOBJ_DATA>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                        P<std::int32_t>(a[4]), P<std::int32_t>(a[5]));
        return 0;
    case 0x140174cd0:
        write_elem_int_info(P<LLOBJ_DATA>(a[0]), P<std::int32_t>(a[1]), P<std::int32_t>(a[2]),
                            P<std::int32_t>(a[3]), P<std::int32_t>(a[4]), P<std::int32_t>(a[5]));
        return 0;
    case 0x1401774f0:
        write_edge_face_info(P<LLOBJ_DATA>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                             P<std::int32_t>(a[4]), P<std::int32_t>(a[5]), P<std::int32_t>(a[6]),
                             P<std::int32_t>(a[7]), P<std::uint32_t>(a[8]));
        return 0;
    case 0x140172c90:
        write_elem_info(P<LLOBJ_DATA>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                        P<std::int32_t>(a[4]), P<std::int32_t>(a[5]), P<std::int32_t>(a[6]));
        return 0;
    case 0x1401707e0:
        write_node_int_info(P<LLOBJ_DATA>(a[0]), P<std::int32_t>(a[1]), P<std::int32_t>(a[2]),
                            P<std::int32_t>(a[3]), P<std::int32_t>(a[4]), P<std::int32_t>(a[5]),
                            P<std::int32_t>(a[6]), P<std::int32_t>(a[7]), P<std::uint32_t>(a[8]),
                            P<std::int32_t>(a[9]));
        return 0;
    case 0x140178460:
        write_bc_functions(P<LLOBJ_DATA>(a[0]), P<std::int32_t>(a[1]), P<std::int32_t>(a[2]),
                           P<std::int32_t>(a[3]), P<std::int32_t>(a[4]), P<std::int32_t>(a[5]),
                           P<std::int32_t>(a[6]), P<double>(a[7]), P<std::int32_t>(a[8]),
                           P<std::int32_t>(a[9]), P<std::uint32_t>(a[10]), P<std::int32_t>(a[11]),
                           P<std::uint32_t>(a[12]));
        return 0;
    case 0x14016cf10:
    case 0x14016d200: {
        auto f = addr == 0x14016cf10 ? &write_general_info : &write_general_info2;
        f(P<LLOBJ_DATA>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), I(a[3]), I(a[4]), I(a[5]),
          P<std::int32_t>(a[6]), P<double>(a[7]), P<std::int32_t>(a[8]), P<const char>(a[9]));
        return 0;
    }
    case 0x14018a760:
        write_obj_2d10(P<void>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                       P<double>(a[4]), P<double>(a[5]), I(a[6]), P<double>(a[7]), P<const char>(a[8]),
                       P<DB_STEP_DATA>(a[9]), P<std::int32_t>(a[10]));
        return 0;
    case 0x140196840:
        write_obj_2e68(P<void>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                       P<double>(a[4]), P<double>(a[5]), I(a[6]), P<double>(a[7]), P<const char>(a[8]),
                       P<const char>(a[9]), P<const char>(a[10]), P<DB_STEP_DATA>(a[11]),
                       P<std::int32_t>(a[12]));
        return 0;
    case 0x140187a60:
        write_obj_1ea0(P<void>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                       P<double>(a[4]), P<DB_STEP_DATA>(a[5]), P<std::int32_t>(a[6]));
        return 0;
    case 0x14017e190:
        convert_contact_codes(P<Cls_14017a6c0>(a[0]), P<double>(a[1]), P<std::int32_t>(a[2]), P<double>(a[3]),
                              P<std::int32_t>(a[4]));
        return 0;
    case 0x1401912a0:
        write_bnd_movement(P<void>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                           P<double>(a[4]), P<DB_STEP_DATA>(a[5]), P<std::int32_t>(a[6]));
        return 0;
    case 0x140260ea0:
        windef_write_to_db_arrays(P<void>(a[0]), P<double>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                                  P<double>(a[4]), P<std::int32_t>(a[5]), P<std::int32_t>(a[6]),
                                  P<DB_STEP_DATA>(a[7]), P<std::int32_t>(a[8]), I(a[9]));
        return 0;
    case 0x14017b370:
        write_objects_3d(P<Cls_14017a6c0>(a[0]), P<std::int32_t>(a[1]), P<double>(a[2]), P<std::int32_t>(a[3]),
                         P<std::int32_t>(a[4]), P<std::int32_t>(a[5]), P<double>(a[6]), P<double>(a[7]),
                         P<double>(a[8]));
        return 0;
    default:
        DEF_NOT_PORTED(addr);
    }
}

} // namespace

std::uint64_t call(std::uint64_t addr, std::initializer_list<std::uint64_t> args)
{
    const std::vector<std::uint64_t> a(args);
    if (g_hook != nullptr) {
        return g_hook(addr, a.data(), static_cast<int>(a.size()));
    }
    return owner_call(addr, a.data());
}

// 14017aaf0: the owner's port
LLOBJ_DATA* f14017aaf0(const model::Cls_14017a6c0* l, std::int32_t n) { return model::object_list_find(l, n); }

// 1401facf0: the owner's port
void* f1401facf0(const model::Cls_1401fa020* l, std::int32_t a, std::int32_t b)
{
    return model::pair_find(const_cast<model::Cls_1401fa020*>(l), a, b);
}

// 140269900: the owner's port
void* f140269900(const void* list, std::int32_t n) { return model::winlist_nth(static_cast<const model::Cls_140269250*>(list), n); }

// 14028aff0: the owner's port
void* f14028aff0(const void* list, std::int32_t n)
{
    return key::g4::grndat_record_nth(static_cast<const model::Cls_14028b640::RecordList*>(list), n);
}

// 14029c410: the owner's port
std::int32_t f14029c410(const MaterialList* l) { return material_list_max_number(const_cast<MaterialList*>(l)); }

// 14029ba10: the owner's port
std::int32_t f14029ba10(const MaterialList* l, std::int32_t* table, const std::int32_t* n)
{
    return g3d::material_positions(const_cast<MaterialList*>(l), table, n);
}

// 14033a040: the owner's port
PhasePair* f14033a040(const PhasePairList* l, std::int32_t from, std::int32_t to)
{
    return model::phase_pair_find(const_cast<PhasePairList*>(l), from, to);
}

// ---- FUNCTION_DATA lengths -----------------------------------------------------------------

namespace {
inline std::int32_t at_least_1(std::int32_t v) { return v < 1 ? 1 : v; }

// 0xa7, 0xa8, 0xaa, 0xab, 0xad, 0xb3, 0xb5: types whose ndim-0 form is a value list
inline bool is_list_type(std::int32_t t)
{
    return t == 0xa7 || t == 0xa8 || t == 0xaa || t == 0xab || t == 0xad || t == 0xb3 || t == 0xb5;
}
} // namespace

// 14012fff0: the Phase 4 gate port (gendb/gate_ports.hpp)
std::int32_t f14012fff0(const DYNARY* f) { return dynary_words_3d(f); }

// 140130090: the owner's port
std::int32_t f140130090(const DYNARY* f) { return dynary_length_3d_comp(f); }

// 140130160 / 140130200: the Phase 4 gate ports
std::int32_t f140130160(const DYNARY* f) { return dynary_words_2d(f); }
std::int32_t f140130200(const DYNARY* f) { return dynary_words_1d(f); }

// 140130230: the owner's port
std::int32_t f140130230(const DYNARY* f) { return dynary_words_1d_padded(f); }

// 140130290: the Phase 4 gate port
std::int32_t f140130290(const DYNARY* f) { return dynary_words(f); }

// 140130380: the owner's port
std::int32_t f140130380(const DYNARY* f) { return dynary_length_plus10(f); }

// 140131360: the owner's port
void f140131360(const DYNARY* f, double* out) { g3d::write_dynary_to_db_array(f, out); }

// 1401317c0: the owner's port
void f1401317c0(const DYNARY* f, double* out) { g3d::write_table_1d_to_db_array(f, out); }

// 14028a650: the owner's port
std::int32_t f14028a650(const void* rec) { return grain_record_length(const_cast<void*>(rec)); }

// 14028b990: the owner's port
std::int32_t f14028b990(const void* g) { return grain_table_length(const_cast<void*>(g)); }

// 14028e140: the owner's port
bool f14028e140(const Material* mat, std::int32_t* code, double* p) { return key::g4a::frcmod_key_values(*mat, code, p); }

// 140336300: the owner's port
std::int32_t f140336300(const PhasePair* pp) { return phase_pair_kinetics_length(const_cast<PhasePair*>(pp)); }

// 1401bb3d0: the owner's port
void f1401bb3d0(DB_STEP_DATA* m, const std::int32_t* len, std::int32_t* iifnc, const std::int32_t* attr,
                double* fid, std::int32_t* start)
{
    g3d::alloc_function(m, len, iifnc, attr, fid, start);
}

} // namespace def::dbio::gendb::ext
