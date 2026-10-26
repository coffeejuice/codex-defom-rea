// FUN_1401d0fc0 (state-variable remap after a material-set change) and FUN_1401d0320 (the
// state-variable size table at DB_STEP_DATA+0x15f8), packet kw-g4b. Offsets the layouts do not
// name yet are read with field<T>(p, off), as the decompile does.
#include <defmodel/material/state_remap.hpp>

#include <cstddef>
#include <cstdint>
#include <new>

#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/material/material_ctor.hpp>

#include <defmodel/llobj_ctor/llobj_ctor.hpp> // kw-g2obj: list_add 14017a880, list_count 14019a280

#include "object_deps.hpp"

namespace def::model {

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}

// 1400015e0 / 1400015f0 (p1-01, inlined as in p1-03's sim_ctrl.cpp): dimension word == 2 / 3
bool model_dim_is_2d(const DB_STEP_DATA* s) { return s->dimension == 2; }
bool model_dim_is_3d(const DB_STEP_DATA* s) { return s->dimension == 3; }

// state-variable table entry k (DB_STEP_DATA+0x15f8 + 4k)
std::int32_t& sv(DB_STEP_DATA& s, std::size_t off) { return at<std::int32_t>(&s, off); }

} // namespace

// @src 1401d0320  state-variable size table for the current dimension, material count and
// simulation-control settings
void step_material_state_sizes(DB_STEP_DATA& s)
{
    const std::int32_t dim = s.dimension; // 140158e90 (+0x1c)
    for (std::int32_t k = 0; k < 0x4f; k = k + 1) {
        s.u15f8[k] = 1;
    }
    if (model_dim_is_3d(&s)) { // 1400015f0
        sv(s, 0x16b0) = 0;
    }
    sv(s, 0x15f8) = dim;
    sv(s, 0x15fc) = dim;
    sv(s, 0x1600) = dim;
    sv(s, 0x1604) = dim;
    sv(s, 0x1608) = dim;
    sv(s, 0x160c) = dim;
    sv(s, 0x1610) = dim;
    sv(s, 0x1640) = dim;
    sv(s, 0x1644) = dim;
    sv(s, 0x165c) = dim;
    if (!step_has_array(s, 0x18, nullptr)) {
        sv(s, 0x1658) = 0;
    }
    else {
        sv(s, 0x1658) = 5;
    }
    sv(s, 0x1690) = dim << 1;
    sv(s, 0x1660) = dim << 1;
    sv(s, 0x1694) = dim << 1;
    std::int32_t sum = 0;
    for (std::int32_t k = 0; k < 5; k = k + 1) {
        sum = sum + at<std::int32_t>(&s, 0x86c + static_cast<std::size_t>(k) * 4);
    }
    const std::int32_t width = sum < 2 ? 1 : sum;
    const std::int32_t base = at<std::int32_t>(&s, 0x880) + at<std::int32_t>(&s, 0x884);
    sv(s, 0x1698) = base + width * dim * 2;
    sv(s, 0x16dc) = dim << 1;
    sv(s, 0x16b4) = 2;
    if (!step_has_array(s, 0x31, nullptr)) {
        sv(s, 0x16bc) = 0;
        sv(s, 0x16c0) = 0;
        sv(s, 0x16c4) = 0;
        sv(s, 0x16c8) = 0;
    }
    else {
        sv(s, 0x16bc) = s.n_materials;  // +0x1320
        sv(s, 0x16c0) = s.n_materials;
        sv(s, 0x16c4) = s.n_phase_on;   // +0x13f4
        sv(s, 0x16c8) = s.n_phase_on;
    }
    std::int32_t size = 0;
    sv(s, 0x16cc) = step_has_array(s, 0x35, &size) ? size : 0;
    sv(s, 0x166c) = step_has_array(s, 0x1d, &size) ? size : 0;
    sv(s, 0x16d0) = step_has_array(s, 0x36, &size) ? size : 0;
    sv(s, 0x16d8) = 2;
    sv(s, 0x16e0) = 2;
    sv(s, 0x16e4) = model_dim_is_3d(&s) ? 3 : 2;
    sv(s, 0x16d4) = step_has_array(s, 0x37, &size) ? size : 0;
    sv(s, 0x163c) = s.u3398;
    sv(s, 0x16e8) = s.u33a8;
    if (!model_dim_is_2d(&s)) { // 1400015e0
        if (model_dim_is_3d(&s)) {
            sv(s, 0x168c) = 0;
            if (!step_has_array(s, 0x2e, nullptr)) {
                sv(s, 0x16b0) = 0;
            }
            else {
                sv(s, 0x16b0) = 9;
            }
            sv(s, 0x16f0) = dim;
        }
    }
    else {
        sv(s, 0x168c) = 4;
        sv(s, 0x16f0) = dim;
        if (at<std::int32_t>(&s, 0x850) == 3) {
            sv(s, 0x1600) = 3;
            sv(s, 0x1604) = 3;
            sv(s, 0x1608) = 3;
            sv(s, 0x1640) = 3;
            sv(s, 0x1644) = 3;
            sv(s, 0x1690) = 6;
            sv(s, 0x1660) = 6;
            sv(s, 0x1694) = 6;
            sv(s, 0x1698) = width * 6 + base;
            sv(s, 0x16dc) = 6;
        }
    }
    if (at<std::int32_t>(&s, 0x888) == 0 || at<std::int32_t>(&s, 0x888) == 3) {
        sv(s, 0x1668) = 0;
    }
    else {
        sv(s, 0x1668) = s.u33c8;
    }
    if (at<std::int32_t>(&s, 0x88c) == 0 || at<std::int32_t>(&s, 0x88c) == 3) {
        sv(s, 0x1664) = 0;
    }
    if (at<std::int32_t>(&s, 0x890) == 0 || at<std::int32_t>(&s, 0x890) == 3) {
        sv(s, 0x1660) = 0;
    }
    sv(s, 0x16ac) = s.u33d8;
    sv(s, 0x1680) = at<std::int32_t>(&s, 0x11c4);
    sv(s, 0x16f4) = at<std::int32_t>(&s, 0x123c);
    sv(s, 0x1684) = at<std::int32_t>(&s, 0x12b4);
}

// @src 1401d8fa0  MaterialList: material number array (+0x18)
std::int32_t* material_list_numbers(const MaterialList* l) { return l->numbers; }

// @src 1401d8fb0  MaterialList: number -> index array (+0x20)
std::int32_t* material_list_index_of(const MaterialList* l) { return l->index_of; }

// @src 1401d0fc0  remap the per-material state after a material-set change
void step_remap_material_state(DB_STEP_DATA& s, bool remap)
{
    std::int32_t old_sizes[0x50];
    for (std::int32_t k = 0; k < 0x4f; k = k + 1) {
        old_sizes[k] = s.u15f8[k];
    }
    Cls_14017a6c0 saved(nullptr); // 14017a6c0(tmp, 0)
    saved.reset();                // 14017a700
    objects_set_owner(saved, &s); // 1401d8340
    std::int32_t* old_numbers = nullptr;
    std::int32_t* old_index = nullptr;
    if (remap) {
        old_numbers = material_list_numbers(&s.materials);  // 1401d8fa0
        old_index = material_list_index_of(&s.materials);   // 1401d8fb0
    }
    // keep a copy of every non-kind-7 object as it was (id included)
    for (std::int32_t i = 0; i < llobj_ctor::list_count(&s.objects); i = i + 1) { // 14019a280
        LLOBJ_DATA* o = s.objects.at(i + 1);                                 // 14017aa80
        if (remap && o != nullptr && llobj_not_kind7(o) && old_numbers != nullptr &&
            old_index != nullptr) {
            LLOBJ_DATA* copy = llobj_ctor::list_add(&saved); // 14017a880 (kw-g2obj)
            bool a = true, b = true, c = true, d = true, e = false, g = true;
            llobj_copy_from(copy, o, s.u15f8, &a, &b, &c, &d, &e, &g); // 1401613a0
            at<std::int32_t>(copy, 8) = at<std::int32_t>(o, 8);
        }
    }
    step_material_state_sizes(s); // 1401d0320
    for (std::int32_t i = 0; i < llobj_ctor::list_count(&s.objects); i = i + 1) {
        LLOBJ_DATA* o = s.objects.at(i + 1);
        if (o == nullptr || !llobj_not_kind7(o)) {
            continue;
        }
        llobj_remap_state(o, s.u15f8, old_sizes); // 140199650
        if (!remap || old_numbers == nullptr || old_index == nullptr) {
            continue;
        }
        LLOBJ_DATA* was = object_list_find(&saved, at<std::int32_t>(o, 8)); // 14017aaf0
        if (was == nullptr || !step_has_array(s, 0x31, nullptr)) {
            continue;
        }
        const std::int32_t width = sv(s, 0x16bc);
        const std::int32_t n_elem = at<std::int32_t>(o, 0x884);
        double* a58 = at<double*>(o, 0xa58);
        double* a60 = at<double*>(o, 0xa60);
        for (std::int32_t col = 0; col < width; col = col + 1) {
            for (std::int32_t e = 0; e < n_elem; e = e + 1) {
                a58[e * width + col] = 0.0;
                a60[e * width + col] = 0.0;
            }
        }
        for (std::int32_t k = 0; k < old_sizes[0x31]; k = k + 1) {
            const std::int32_t number = old_numbers[k];
            const std::int32_t old_col = old_index[k];
            const std::int32_t new_col = material_list_index_of(&s.materials, number);
            if (-1 < material_list_position(&s.materials, number)) { // 14029c7f0
                const double* w58 = at<double*>(was, 0xa58);
                const double* w60 = at<double*>(was, 0xa60);
                for (std::int32_t e = 0; e < at<std::int32_t>(o, 0x884); e = e + 1) {
                    a58[e * sv(s, 0x16bc) + new_col] = w58[e * old_sizes[0x31] + old_col];
                    a60[e * sv(s, 0x16bc) + new_col] = w60[e * old_sizes[0x31] + old_col];
                }
            }
        }
    }
    saved.clear(); // 14017ab70; dtor 14017a740 at scope end
}

// @src 14029c7f0  1-based position of a material number, -1 when absent
std::int32_t material_list_position(const MaterialList* list, std::int32_t number)
{
    std::int32_t pos = 1;
    for (const Material* m = list->head; m != nullptr && m->number != number; m = m->next) {
        pos = pos + 1;
    }
    if (list->count < pos) {
        pos = -1;
    }
    return pos;
}

// @src 14017aaf0  object with id `id` (+8), or nullptr
LLOBJ_DATA* object_list_find(const Cls_14017a6c0* l, std::int32_t id)
{
    LLOBJ_DATA* o = l->head;
    while (o != nullptr && at<std::int32_t>(o, 8) != id) {
        o = o->next;
    }
    return o;
}


// @src 140160f10  object kind (+0xc) is not 7
bool llobj_not_kind7(const LLOBJ_DATA* o) { return at<std::int32_t>(const_cast<LLOBJ_DATA*>(o), 0xc) != 7; }


} // namespace def::model
