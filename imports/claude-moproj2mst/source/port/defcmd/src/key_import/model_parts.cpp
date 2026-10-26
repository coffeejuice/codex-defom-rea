// Helpers of the KEY import family (packet mdt-keyimport): model part copies, the mixture
// material import and the directory strip. Decompiles: deform-rea-work/out/mdt1/f0.c, f1.c, f2.c.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/key_import.hpp>

#include <cstddef>
#include <cstring>

#include <defcmd/cnvt3d_sweep_b.hpp>
#include <defcmd/interp.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/cls_1401fa020.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/material_select.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/port.hpp>

namespace def::cmd {

// 14029c3c0, p8-07's (src/meshgen/meshgen_2d.cpp, declared in src/meshgen/mg_internal.hpp)
std::int32_t first_free_material(void* materials);

namespace {

std::byte* bp(void* p) { return static_cast<std::byte*>(p); }
const std::byte* bp(const void* p) { return static_cast<const std::byte*>(p); }

template <class T> T rd(const void* p, std::size_t off)
{
    T v;
    std::memcpy(&v, bp(p) + off, sizeof v);
    return v;
}

// 14028d370 / 1403357a0 are p8-08's (defcmd/interp.hpp); a test may replace them.
void material_copy(model::Material* d, const model::Material* s)
{
    if (g_key_import_ext.material_copy != nullptr) g_key_import_ext.material_copy(d, s);
    else material_record_copy(d, s);
}

void phase_pair_copy(model::PhasePair* d, const model::PhasePair* s)
{
    if (g_key_import_ext.phase_pair_copy != nullptr) g_key_import_ext.phase_pair_copy(d, s);
    else phase_pair_record_copy(d, s);
}

} // namespace

// @src 1401d7e90
std::uint8_t step_nstart_given(const DB_STEP_DATA* m)
{
    return m->u2f88;
}

// @src 1401d7430
void step_copy_name_arrays(DB_STEP_DATA* m, DB_STEP_DATA* src)
{
    std::int32_t i = 0;
    if (0 < model::node_name_count(src)) {                                    // 1401cbbc0
        model::resize_node_names(m, model::node_name_count(src));              // 1401d21c0
        for (i = 0; i < model::node_name_count(src); i = i + 1)
            cnvt3d_sweep::fun_1401cbd40(m, &i, cnvt3d_sweep::fun_1401cbc20(src)[i]);
    }
    if (0 < model::element_name_count(src)) {                                 // 1401cbc80
        model::resize_element_names(m, model::element_name_count(src));        // 1401d2090
        for (i = 0; i < model::element_name_count(src); i = i + 1)
            cnvt3d_sweep::fun_1401cbd90(m, &i, cnvt3d_sweep::fun_1401cbce0(src)[i]);
    }
}

// @src 1401fa0c0
void inter_object_list_copy(model::Cls_1401fa020* dst, model::Cls_1401fa020* src)
{
    dst->clear();  // 1401fade0
    for (std::int32_t i = 0; i < model::pair_count(src); i = i + 1) {  // 1400015d0
        model::Cls_1401f46e0* p = model::pair_append(dst);           // 1401faaa0
        model::pair_copy(p, model::pair_nth(src, i + 1));            // 1401fac90, 1401f9900
        p->next = nullptr;                                           // +0x390
    }
}

// @src 140323d50
void path_strip_directory(char* s)
{
    std::int32_t i = static_cast<std::int32_t>(std::strlen(s));
    std::int32_t k;
    do {
        k = i;
        i = k - 1;
        if (i < 0) return;
    } while (s[i] != '\\' && s[i] != '/');
    // 140484f90 strcpy(s, s + k): a forward copy onto itself
    std::memmove(s, s + k, std::strlen(s + k) + 1);
}

// @src 14028d9a0
bool material_copy_keep_number(model::Material* dst, const model::Material* src)
{
    const std::int32_t no = dst->number;
    material_copy(dst, src);  // 14028d370
    dst->number = no;
    return true;
}

// @src 14019d6b0
std::int32_t material_import_mixture(DB_STEP_DATA* m, model::Material* dst,
                                     const model::Material* src, DB_STEP_DATA* scratch)
{
    if (dst == nullptr) return -1;
    material_copy_keep_number(dst, src);  // 14028d9a0
    if (dst->mstmtr_flag != 1 || !(1 < scratch->materials.count)) return 0;  // 140199c50: +0x10
    for (std::int32_t i = 0; i < dst->mstmtr_count; i = i + 1) {
        const model::Material* comp = model::material_list_find(&scratch->materials, src->mstmtr_list[i]);  // 14029c2c0
        if (comp != nullptr) {
            const std::int32_t no = first_free_material(&m->materials);           // 14029c3c0
            model::Material* nm = model::step_material_get_or_add(*m, no);         // 14019cfb0
            material_copy_keep_number(nm, comp);                                   // 14028d9a0
            dst->mstmtr_list[i] = nm->number;
        }
    }
    // FSTRES block type (+0x50) 10 / 12: material numbers in column 2 of the +0x4c8 table
    const std::int32_t ft = rd<std::int32_t>(src, 0x50);
    if (ft == 10 || ft == 0xc) {
        for (std::int32_t i = 0; i < rd<std::int32_t>(src, 0x478); i = i + 1) {
            for (std::int32_t j = 0; j < src->mstmtr_count; j = j + 1) {
                const double* sv = rd<const double*>(src, 0x4c8);
                if (static_cast<std::int32_t>(sv[i * 2 + 1]) == src->mstmtr_list[j]) {
                    double* dv = rd<double*>(dst, 0x4c8);
                    dv[i * 2 + 1] = static_cast<double>(dst->mstmtr_list[j]);
                    break;
                }
            }
        }
    }
    // CREEP block type (+0xac8) 7 / 8: the same in the +0xdf0 table
    const std::int32_t ct = rd<std::int32_t>(src, 0xac8);
    if (ct == 7 || ct == 8) {
        for (std::int32_t i = 0; i < rd<std::int32_t>(src, 0xda4); i = i + 1) {
            for (std::int32_t j = 0; j < src->mstmtr_count; j = j + 1) {
                const double* sv = rd<const double*>(src, 0xdf0);
                if (static_cast<std::int32_t>(sv[i * 2 + 1]) == src->mstmtr_list[j]) {
                    double* dv = rd<double*>(dst, 0xdf0);
                    dv[i * 2 + 1] = static_cast<double>(dst->mstmtr_list[j]);
                    break;
                }
            }
        }
    }
    for (std::int32_t i = 0; i < model::phase_pair_count(&scratch->phase_pairs); i = i + 1) {  // 1401b05f0
        model::PhasePair* pp = model::phase_pair_nth(&scratch->phase_pairs, i + 1);           // 140339fe0
        if (model::phase_pair_in_mixture(pp, src)) {                                          // 140339ba0
            model::PhasePair* np = model::phase_pair_append(&m->phase_pairs);                 // 140339df0
            phase_pair_copy(np, pp);                                                          // 1403357a0
            if (np->phastf_on != 0) m->n_phase_on = m->n_phase_on + 1;
            for (std::int32_t j = 0; j < src->mstmtr_count; j = j + 1) {
                if (np->from == src->mstmtr_list[j]) {
                    np->from = dst->mstmtr_list[j];
                    break;
                }
            }
            for (std::int32_t j = 0; j < src->mstmtr_count; j = j + 1) {
                if (np->to == src->mstmtr_list[j]) {
                    np->to = dst->mstmtr_list[j];
                    break;
                }
            }
        }
    }
    m->n_materials = m->materials.count;  // 140199c50
    return 0;
}

} // namespace def::cmd
