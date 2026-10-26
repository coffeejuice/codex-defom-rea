// Defaults of ObjCountExt (packet p4-02a): interim copies of small callees owned by other
// packets that are not on claude/p1-gate. They follow the decompile of the named address but
// carry no @src tag; the merge points each entry at the owner's port.
#include <defmodel/geo_kw/curve_model.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/geo_kw/misc_model.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/window/window_list.hpp>
#include <defdbio/gendb/gate_ports.hpp>
#include <defdbio/gendb/obj_counts.hpp>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <defmodel/dynary_init.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include "raw.hpp"

namespace def::dbio::gendb {

using raw::at;

namespace {

// 140130200 / 140130160 / 14012fff0 / 140130290: the Phase 4 gate ports (gendb/gate_ports.hpp)

// 14012eff0: the owner port (defmodel/window/inter_object_ops.hpp)

// interim 14012fd30
void d_dynary_init_n0(DYNARY* t, std::int32_t type, std::int32_t n0)
{
    model::dynary_init(t, type);  // 14012d2a0
    t->n[0] = n0;
    model::dynary_alloc(t);  // 14012fad0
}

// 140194d40: the Phase 4 gate port (gendb/gate_ports.hpp); its result (1) is not used here
void d_matrix(const void* self, double* m) { movement_frame(self, m); }

// interim 1402416d0 (left-to-right sums, as the ADDSD sequence)
void d_mat3_mul_vec(const double* v, double* w, const double* m)
{
    w[0] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2];
    w[1] = m[3] * v[0] + m[4] * v[1] + m[5] * v[2];
    w[2] = m[6] * v[0] + m[7] * v[1] + m[8] * v[2];
}

// 140198310 / 140304e40: the kw-g2geo ports (defmodel/geo_kw)
std::int32_t d_geo2d_count(model::LLOBJ_DATA* o) { return model::geo_kw::curve_count(*o); }
bool d_drop_bad_geo2d(model::LLOBJ_DATA* o, std::int32_t remove)
{
    return model::geo_kw::remove_degenerate_curves(*o, remove != 0) != 0;
}

// 140259b60 / 140269900: the owner ports (defmodel/geo_kw/misc_model.hpp, window/window_list.hpp)
void* d_record_e8_nth(void* list, std::int32_t n)
{
    return model::geo_kw::wpaxis_nth(*static_cast<model::linked::Cls_140258580List*>(list), n);
}
void* d_window_nth(void* list, std::int32_t n)
{
    return model::winlist_nth(static_cast<const model::Cls_140269250*>(list), n);
}

// 140239a30: the port in window_orient.cpp
double d_vec3_length(const double* v) { return vec3_norm(v); }

// interim 140257150 (str 1405e5098 "%s Error when allocating %s memory ...": the message
// helper 14039d3a0 is not on this branch)
double* d_alloc_doubles(std::int32_t n)
{
    double* p;
    if (n == 0) {
        p = nullptr;
    } else {
        p = static_cast<double*>(std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 8));
        if (p == nullptr)
            DEF_NOT_PORTED(0x14039d3a0);
    }
    return p;
}

// interim 1402feeb0 / 1402ff000: the COW handle *list points at {refs, std::list*}; refs > 1
// detaches first (1402ff710 / 1402ff820, not on this branch); element i of the list, + 0x10.
void* cow_list_item(void* list, std::int64_t i, std::uint64_t detach)
{
    void* h = *static_cast<void**>(list);
    if (1 < at<std::uint32_t>(h, 0))
        DEF_NOT_PORTED(detach);
    void* node = **static_cast<void***>(static_cast<void*>(static_cast<char*>(h) + 8));
    for (std::uint64_t k = 0; k < static_cast<std::uint64_t>(i); ++k)
        node = *static_cast<void**>(node);
    return static_cast<char*>(node) + 0x10;
}
void* d_surface_set_a(void* list, std::int64_t i) { return cow_list_item(list, i, 0x1402ff710); }
void* d_surface_set_b(void* list, std::int64_t i) { return cow_list_item(list, i, 0x1402ff820); }

// interim 14019a820 / 14019a840 / 14019a800 / 140159950: *(*(void**)p + 0x10)
std::int32_t handle_size(void* p) { return at<std::int32_t>(*static_cast<void**>(p), 0x10); }
std::int32_t d_list_size(void* list) { return handle_size(list); }
// interim 14019a000 / 14019a060 (set + 0x10)
std::int32_t d_set_count(void* set) { return handle_size(static_cast<char*>(set) + 0x10); }
// interim 1402b7470 / 1402b74d0
std::int32_t d_set_kind(void* set) { return at<std::int32_t>(set, 4); }

// 1404b5ef4: list-directed WRITE puts one blank before the text
void d_debug_write(std::int32_t, const char* text, std::int32_t len)
{
    def::rt::crt::fprintf(stdout, " %.*s\n", len, text);
}

ObjCountExt defaults()
{
    ObjCountExt e{};
    e.dynary_words_1d = gendb::dynary_words_1d;
    e.dynary_words_2d = gendb::dynary_words_2d;
    e.dynary_words_3d = gendb::dynary_words_3d;
    e.dynary_words = gendb::dynary_words;
    e.dynary_copy = model::dynary_copy;
    e.dynary_init_n0 = d_dynary_init_n0;
    e.cls_14018b220_matrix = d_matrix;
    e.mat3_mul_vec = d_mat3_mul_vec;
    e.obj_geo2d_count = d_geo2d_count;
    e.obj_drop_bad_geo2d = d_drop_bad_geo2d;
    e.record_e8_nth = d_record_e8_nth;
    e.window_nth = d_window_nth;
    e.vec3_length = d_vec3_length;
    e.alloc_doubles = d_alloc_doubles;
    e.surface_set_a = d_surface_set_a;
    e.surface_set_b = d_surface_set_b;
    e.surface_list_a_size = d_list_size;
    e.surface_list_b_size = d_list_size;
    e.surface_set_a_count = d_set_count;
    e.surface_set_b_count = d_set_count;
    e.surface_set_a_kind = d_set_kind;
    e.surface_set_b_kind = d_set_kind;
    e.ftn_debug_write = d_debug_write;
    return e;
}

ObjCountExt g_ext = defaults();

} // namespace

ObjCountExt& obj_count_ext() { return g_ext; }
void reset_obj_count_ext() { g_ext = defaults(); }

} // namespace def::dbio::gendb
