// Packet p8-08: the model copy 1401d7390 (simulation control, materials, phase pairs) that the
// DEFINT/DEFFRZ read-back runs on its temporary model, and its member copies, also called by
// CNVT3D (p9-01: 140146560, 14029b6c0) and the KEY reader (1401b6c80, 1401b7250).
// Decompile deform-rea-work/out/p8mg/p8-08/all.c. Every copy follows the binary field by field
// at the recovered offsets (the layout headers keep the binary's offsets), including two raw
// copies the layout calls owned pointers: Material+0x7c8 (EXPAND's DYNARY+0x68, 14028d433) and
// the GRNDAT main record's +0x1e8 (14029dea0). The original copies them as plain words too.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/interp.hpp>

#include <cstdlib>
#include <cstring>

#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/cls_14028b850.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/material_list.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/layout/phase_pair_list.hpp>
#include <defmodel/linked/accessors.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/material/state_remap.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::cmd {

namespace {

using model::DYNARY;

std::byte* bp(void* p) { return static_cast<std::byte*>(p); }
const std::byte* bp(const void* p) { return static_cast<const std::byte*>(p); }

void cp1(void* d, const void* s, std::size_t o) { std::memcpy(bp(d) + o, bp(s) + o, 1); }
void cp4(void* d, const void* s, std::size_t o) { std::memcpy(bp(d) + o, bp(s) + o, 4); }
void cp8(void* d, const void* s, std::size_t o) { std::memcpy(bp(d) + o, bp(s) + o, 8); }
void cpn(void* d, const void* s, std::size_t o, std::size_t n) { std::memcpy(bp(d) + o, bp(s) + o, n); }
void str(void* d, const void* s, std::size_t o)
{
    std::strcpy(reinterpret_cast<char*>(bp(d) + o), reinterpret_cast<const char*>(bp(s) + o));  // 140484f90
}
DYNARY* dyn(void* p, std::size_t o) { return reinterpret_cast<DYNARY*>(bp(p) + o); }
const DYNARY* dyn(const void* p, std::size_t o) { return reinterpret_cast<const DYNARY*>(bp(p) + o); }
// 14012eff0 (DYNARY copy) and 14019a500 (the same, returning dst)
void dy(void* d, const void* s, std::size_t o) { model::dynary_copy(dyn(d, o), dyn(s, o)); }

// owner model of the simulation control (+0xbc0): 1400015e0 dimension == 2
bool sim_is_2d(const model::Cls_1401e7b10* s) { return rt::dimension_is_2(s->owner); }
bool sim_is_3d(const model::Cls_1401e7b10* s) { return rt::dimension_is_3(s->owner); }

// ---- simulation control (Cls_1401e7b10, byte offsets) ---------------------------------------

// @src 1401e8040  names, run ids, TRANS/STNOUT/ELMNOD blocks
void sim_copy_names(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s)
{
    str(d, s, 0x0);
    str(d, s, 0x51);
    str(d, s, 0x92);
    cp4(d, s, 0xe8);
    cp4(d, s, 0xec);
    cp4(d, s, 0xfc);
    cp4(d, s, 0xf4);
    cp4(d, s, 0xf8);
    cp4(d, s, 0x100);
    cpn(d, s, 0x104, 0x18);
    cpn(d, s, 0x11c, 0x1c);
    cpn(d, s, 0x138, 0x28);
}

// @src 1401e84f0  NSTART .. NSTEP
void sim_copy_steps(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s)
{
    cp4(d, s, 0x160);
    cp4(d, s, 0x16c);
    cp4(d, s, 0x164);
    cp4(d, s, 0x168);
}

// @src 1401e87f0  step control
void sim_copy_step_ctrl(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s)
{
    cp4(d, s, 0x1c8);
    model::linked::set_dsmax(d, s->dsmax);  // 1401f4390(d, *(s+0x1e0))
    cp8(d, s, 0x1d0);
    cp8(d, s, 0x1d8);
    cp8(d, s, 0x268);
    cp8(d, s, 0x270);
    cp8(d, s, 0x278);
    cp8(d, s, 0x280);
    cp8(d, s, 0x260);
    cpn(d, s, 0x2a0, 0x10);
    if (sim_is_2d(d)) cpn(d, s, 0x2b0, 0x10);
    cp8(d, s, 0x2c0);
    if (sim_is_3d(d)) cp8(d, s, 0x2c8);
    dy(d, s, 0x1e8);
}

// @src 1401e90a0  stopping limits
void sim_copy_limits(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s)
{
    cp8(d, s, 0x178);
    cpn(d, s, 0x180, 0x18);
    cpn(d, s, 0x198, 0x18);
    cpn(d, s, 0x1b0, 0x18);
    cp4(d, s, 0x288);
    cp4(d, s, 0x28c);
    cp4(d, s, 0x290);
    cp8(d, s, 0x298);
    cp8(d, s, 0x170);
    cp8(d, s, 0xbc8);
}

// @src 1401e9950  iteration control (the 2D and 3D branches copy the same fields)
void sim_copy_iteration(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s)
{
    cp8(d, s, 0x2d0);
    cp8(d, s, 0x2d8);
    cp4(d, s, 0x2e4);
    cp4(d, s, 0x2e8);
    cp8(d, s, 0x2f0);
    cp8(d, s, 0x2f8);
    cp8(d, s, 0x300);
    cp8(d, s, 0x308);
    cp4(d, s, 0x318);
    cp4(d, s, 0x328);
    cp4(d, s, 0x32c);
    dy(d, s, 0x330);
    cp4(d, s, 0x2e0);
    cp4(d, s, 0x310);
    cp4(d, s, 0x314);
    cp4(d, s, 0x31c);
    cp4(d, s, 0x320);
    cp4(d, s, 0x324);
}

// @src 140375550  furnace model (Cls_1403753f0): rebuild the curve list with the source's count,
// copy each curve, then the name
void furnace_copy(model::Cls_1403753f0* d, model::Cls_1403753f0* s)
{
    std::int32_t n = s->curves.size();  // 14013c1b0(s+0x40)
    d->clear_curves();                  // 140375520
    d->setup_curves(&n);                // 140375600
    for (std::int32_t i = 1; i <= n; i = i + 1) {
        DYNARY* dc = d->curve(&i);      // 140375980
        model::dynary_copy(dc, s->curves.at(i));  // 140136860, 14012eff0
    }
    std::strcpy(d->name, s->name);
}

// @src 1401ea900  environment / heat transfer
void sim_copy_environment(model::Cls_1401e7b10* d, model::Cls_1401e7b10* s)
{
    cp8(d, s, 0x3a8);
    cp8(d, s, 0x3b8);
    dy(d, s, 0x3c8);
    furnace_copy(&d->furnace, &s->furnace);
    dy(d, s, 0x4c0);
    dy(d, s, 0x538);
    dy(d, s, 0x5b0);
    dy(d, s, 0xa58);
    dy(d, s, 0xad0);
    dy(d, s, 0xb48);
    cp8(d, s, 0x3b0);
    cp8(d, s, 0x4b8);
    cp8(d, s, 0x628);
    cp8(d, s, 0x630);
    cp8(d, s, 0x638);
}

// @src 1401eaf80  time, tolerances, user values, 2D/3D specifics
void sim_copy_misc(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s)
{
    cp8(d, s, 0x640);
    cp8(d, s, 0x648);
    cp4(d, s, 0x650);
    cp8(d, s, 0x658);
    cpn(d, s, 0x660, 0x20);
    if (sim_is_2d(d)) cp8(d, s, 0x680);
    for (std::size_t i = 0; i < 10; ++i) str(d, s, 0x688 + i * 0x51);
    cp4(d, s, 0x9b4);
    if (!sim_is_2d(d)) {
        cp4(d, s, 0x9d8);
        cp4(d, s, 0x9dc);
        dy(d, s, 0x9e0);  // 14019a500
    } else {
        cp4(d, s, 0x9b8);
        cp4(d, s, 0x9bc);
        cp4(d, s, 0x9c0);
        cp4(d, s, 0x9c4);
        cp4(d, s, 0x9c8);
        cp8(d, s, 0x9d0);
        cp4(d, s, 0x9d8);
    }
    dy(d, s, 0xa58);
    dy(d, s, 0xad0);
    dy(d, s, 0xb48);
}

// ---- material (Material, byte offsets = the decompile's int index x 4) ----------------------

// @src 14029d010  FSTRES block (Material+0x50)
void fstres_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    for (std::size_t o = 0x8; o <= 0x20; o += 8) cp8(d, s, o);
    dy(d, s, 0x28);
    for (std::size_t o = 0xa0; o <= 0xe0; o += 8) cp8(d, s, o);
    dy(d, s, 0xe8);
    dy(d, s, 0x160);
    dy(d, s, 0x1d8);
    for (std::size_t o = 0x250; o <= 0x330; o += 8) cp8(d, s, o);
    dy(d, s, 0x338);
    dy(d, s, 0x3b0);
    cp4(d, s, 0x428);
    dy(d, s, 0x430);
    dy(d, s, 0x4a8);
    dy(d, s, 0x520);
    dy(d, s, 0x598);
    cp4(d, s, 0x610);
}

// @src 14029d5b0  DIFCOE block (Material+0x950)
void difcoe_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    dy(d, s, 0x8);
    dy(d, s, 0x80);
    dy(d, s, 0xf8);
    cp4(d, s, 0x170);
}

// @src 14029d640  CREEP block (Material+0xac8)
void creep_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    dy(d, s, 0x8);
    dy(d, s, 0x80);
    dy(d, s, 0xf8);
    dy(d, s, 0x170);
    dy(d, s, 0x1e8);
    dy(d, s, 0x260);
    cp4(d, s, 0x2d8);
    cp4(d, s, 0x2dc);
    dy(d, s, 0x2e0);
    dy(d, s, 0x358);
    dy(d, s, 0x3d0);
}

// @src 14029dbe0  GRNDAT type-1 part (4 flags, 9 DYNARYs, g2, g1)
void grndat1_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    cp4(d, s, 0x4);
    cp4(d, s, 0x8);
    cp4(d, s, 0xc);
    for (std::size_t k = 0; k < 9; ++k) dy(d, s, 0x10 + k * 0x78);
    cp8(d, s, 0x448);
    cp8(d, s, 0x450);
}

// @src 14028ae00  GRNDAT record (Cls_14028a4c0) without its list link
void grndat_record_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    for (std::size_t i = 0; i < 12; ++i) cp8(d, s, 0x8 + i * 8);
    cp8(d, s, 0x68);
    cp4(d, s, 0x70);
    dy(d, s, 0x78);
    dy(d, s, 0xf0);
    dy(d, s, 0x168);
    cp1(d, s, 0x1e0);
}

// @src 14029dea0  GRNDAT main record: as 14028ae00, plus the word at +0x1e8
void grndat_main_record_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    for (std::size_t i = 0; i < 12; ++i) cp8(d, s, 0x8 + i * 8);
    cp8(d, s, 0x68);
    cp4(d, s, 0x70);
    dy(d, s, 0x78);
    dy(d, s, 0xf0);
    dy(d, s, 0x168);
    cp1(d, s, 0x1e0);
    cp8(d, s, 0x1e8);
}

// part of @src 14028aff0 (owner P2, not exported; same walk as p4-06's local helper): record n
// (1-based) of a record list, the head for n < 2, null past the end
model::Cls_14028a4c0* record_nth(const model::Cls_14028b640::RecordList* l, std::int32_t n)
{
    std::int32_t i = 0;
    model::Cls_14028a4c0* r = l->head;
    while (r != nullptr && (i = i + 1, i < n)) {
        r = *reinterpret_cast<model::Cls_14028a4c0* const*>(bp(r) + 0x1e8);
    }
    return r;
}

// @src 14028af60  record list copy
// @src 14029de40  (thunk: 14028af60, returns dst)
void grndat_record_list_copy(model::Cls_14028b640::RecordList* d, const model::Cls_14028b640::RecordList* s)
{
    model::grndat_record_list_clear(d);  // 14028b240
    for (std::int32_t i = 0; i < model::grndat_record_list_count(s); i = i + 1) {  // 14029cd50
        model::Cls_14028a4c0* r = model::grndat_record_list_add(d);                 // 14028b050
        grndat_record_copy(r, record_nth(s, i + 1));
        *reinterpret_cast<void**>(bp(r) + 0x1e8) = nullptr;
    }
}

// @src 14029dd70  GRNDAT type-2 part: c, main record, five record lists
void grndat2_copy(model::Cls_14028b640* d, const model::Cls_14028b640* s)
{
    cp4(d, s, 0x0);
    grndat_main_record_copy(&d->main, &s->main);
    grndat_record_list_copy(&d->l3, &s->l3);  // +0x1f8
    grndat_record_list_copy(&d->l2, &s->l2);  // +0x210
    grndat_record_list_copy(&d->l1, &s->l1);  // +0x228
    grndat_record_list_copy(&d->l4, &s->l4);  // +0x240
    grndat_record_list_copy(&d->l5, &s->l5);  // +0x258
}

// @src 14029d7a0  GRNDAT block (Material+0x1078)
// @src 14029de70  (thunk: 14019a500 on +0x6c8)
void grndat_copy(model::Cls_14028b850* d, const model::Cls_14028b850* s)
{
    grndat1_copy(&d->type1, &s->type1);
    grndat2_copy(&d->type2, &s->type2);
    dy(d, s, 0x6c8);
    cpn(d, s, 0x740, 0x30);  // via a 0x30-byte stack copy
    cp4(d, s, 0x770);
    cp4(d, s, 0x774);
}

// @src 14029d8f0  BURGRS / ALPHA / NDISFM / RECVRY (Material+0x18f0)
void disloc_copy(void* d, const void* s)
{
    dy(d, s, 0x0);
    dy(d, s, 0x78);
    dy(d, s, 0xf0);
    dy(d, s, 0x168);
}

// @src 14029d970  FRCMOD / UTSDAT block (Material+0x1ad0)
void frcmod_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    for (std::size_t o = 0x8; o <= 0x30; o += 8) cp8(d, s, o);
    cp4(d, s, 0x38);
    dy(d, s, 0x40);
    cp8(d, s, 0xb8);
    dy(d, s, 0xc0);
    dy(d, s, 0x138);
    for (std::size_t o = 0x1b0; o <= 0x1f0; o += 8) cp8(d, s, o);
}

// @src 14029db80  ANISO block (Material+0x1e40)
void aniso_copy(void* d, const void* s)
{
    cp4(d, s, 0x0);
    dy(d, s, 0x8);
    cp4(d, s, 0x80);
}

// @src 14028d370  one material
void material_copy(model::Material* d, const model::Material* s)
{
    cp4(d, s, 0x0);
    str(d, s, 0xc);
    fstres_copy(bp(d) + 0x50, bp(s) + 0x50);
    cp4(d, s, 0x668);
    dy(d, s, 0x670);
    dy(d, s, 0x6e8);
    dy(d, s, 0x760);
    cp8(d, s, 0x7c8);  // 14028d433
    cp1(d, s, 0x7d0);
    cp8(d, s, 0x7d8);
    dy(d, s, 0x7e0);
    dy(d, s, 0x858);
    cp4(d, s, 0x8d0);
    dy(d, s, 0x1fe0);
    dy(d, s, 0x8d8);
    dy(d, s, 0x2058);
    difcoe_copy(bp(d) + 0x950, bp(s) + 0x950);
    creep_copy(bp(d) + 0xac8, bp(s) + 0xac8);
    dy(d, s, 0xf10);
    dy(d, s, 0xf88);
    dy(d, s, 0x1000);
    grndat_copy(&d->grndat, &s->grndat);
    d->mstmtr_flag = s->mstmtr_flag;
    if (d->mstmtr_flag == 1) {
        d->mstmtr_count = s->mstmtr_count;
        if (d->mstmtr_list != nullptr) std::free(d->mstmtr_list);
        d->mstmtr_list = nullptr;
        d->mstmtr_list = static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(d->mstmtr_count), 4));
        std::memcpy(d->mstmtr_list, s->mstmtr_list, static_cast<std::size_t>(static_cast<std::int64_t>(d->mstmtr_count) << 2));
    }
    disloc_copy(bp(d) + 0x18f0, bp(s) + 0x18f0);
    frcmod_copy(bp(d) + 0x1ad0, bp(s) + 0x1ad0);
    dy(d, s, 0x1cd8);
    dy(d, s, 0x1d50);
    dy(d, s, 0x1dc8);
    aniso_copy(bp(d) + 0x1e40, bp(s) + 0x1e40);
    cp8(d, s, 0x1ec8);
    cp8(d, s, 0x1ed0);
    cp8(d, s, 0x1ed8);
    cp8(d, s, 0x1ee0);
    cp8(d, s, 0x1ee8);
    dy(d, s, 0x1ef0);
    dy(d, s, 0x1f68);
    dy(d, s, 0x2160);
    cp1(d, s, 0x2210);
    cp4(d, s, 0x20d0);
    dy(d, s, 0x20d8);
    cp4(d, s, 0x2150);
    cpn(d, s, 0x2154, 0xc);  // via a 12-byte stack copy
    cp4(d, s, 0x21d8);
    cp8(d, s, 0x21e0);
    cp8(d, s, 0x21e8);
    cp8(d, s, 0x21f0);
    cp8(d, s, 0x21f8);
    dy(d, s, 0x17f0);
    cp8(d, s, 0x1868);
    cp8(d, s, 0x1870);
    dy(d, s, 0x1878);
}

// @src 1403357a0  one phase pair (PhasePair, byte offsets = int index x 4)
void phase_pair_copy(model::PhasePair* d, const model::PhasePair* s)
{
    for (std::size_t o = 0x0; o <= 0x14; o += 4) cp4(d, s, o);
    str(d, s, 0x18);
    cp4(d, s, 0x6c);
    cp8(d, s, 0x70);
    dy(d, s, 0x78);
    dy(d, s, 0xf8);
    cp8(d, s, 0x170);
    cp8(d, s, 0x178);
    dy(d, s, 0x180);
    cp8(d, s, 0x3e8);
    cp8(d, s, 0x3f0);
    dy(d, s, 0x1f8);
    for (std::size_t o = 0x270; o <= 0x2c0; o += 8) cp8(d, s, o);
    dy(d, s, 0x2c8);
    for (std::size_t o = 0x340; o <= 0x3e0; o += 8) cp8(d, s, o);
    dy(d, s, 0x3f8);
    cp4(d, s, 0x470);
    dy(d, s, 0x478);
    dy(d, s, 0x4f0);
    dy(d, s, 0x568);
    cp4(d, s, 0x5e0);
    cp4(d, s, 0x5e4);
    dy(d, s, 0x5e8);
    cp4(d, s, 0x660);
    cp8(d, s, 0x668);
    cp8(d, s, 0x670);
    cp4(d, s, 0x768);
    dy(d, s, 0x6f0);
}

} // namespace

void material_record_copy(model::Material* d, const model::Material* s) { material_copy(d, s); }
void phase_pair_record_copy(model::PhasePair* d, const model::PhasePair* s) { phase_pair_copy(d, s); }
void furnace_model_copy(model::Cls_1403753f0* d, model::Cls_1403753f0* s) { furnace_copy(d, s); }

void sim_ctrl_copy_names(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s) { sim_copy_names(d, s); }
void sim_ctrl_copy_steps(model::Cls_1401e7b10* d, const model::Cls_1401e7b10* s) { sim_copy_steps(d, s); }
void sim_ctrl_copy_environment(model::Cls_1401e7b10* d, model::Cls_1401e7b10* s) { sim_copy_environment(d, s); }

// @src 1401f3660
void sim_ctrl_copy(model::Cls_1401e7b10* dst, const model::Cls_1401e7b10* src)
{
    // 1401ea900 reads the source furnace's list through non-const accessors only
    auto* s = const_cast<model::Cls_1401e7b10*>(src);
    sim_copy_names(dst, src);
    sim_copy_steps(dst, src);
    sim_copy_step_ctrl(dst, src);
    sim_copy_limits(dst, src);
    sim_copy_iteration(dst, src);
    sim_copy_environment(dst, s);
    sim_copy_misc(dst, src);
}

// @src 14029b630
void material_list_copy(model::MaterialList* dst, model::MaterialList* src)
{
    model::material_list_clear(dst);  // 14029c310
    for (std::int32_t i = 0; i < src->count; i = i + 1) {  // 140199c50: +0x10
        model::Material* m = model::material_list_add(dst);  // 14029c060
        material_copy(m, model::material_list_nth(src, i + 1));  // 14029c260
        m->next = nullptr;  // +0x2200
    }
}

// @src 140339d30
void phase_pair_list_copy(model::PhasePairList* dst, model::PhasePairList* src)
{
    dst->clear();  // 14033a120
    for (std::int32_t i = 0; i < model::phase_pair_count(src); i = i + 1) {  // 1401b05f0
        model::PhasePair* p = model::phase_pair_append(dst);                 // 140339df0
        phase_pair_copy(p, model::phase_pair_nth(src, i + 1));               // 140339fe0
        *reinterpret_cast<void**>(bp(p) + 0x770) = nullptr;
    }
}

// @src 1401d7390
void model_copy_data(model::DB_STEP_DATA* dst, const model::DB_STEP_DATA* src)
{
    auto* s = const_cast<model::DB_STEP_DATA*>(src);
    sim_ctrl_copy(&dst->sim_ctrl, &src->sim_ctrl);
    dst->n_materials = src->n_materials;
    material_list_copy(&dst->materials, &s->materials);
    phase_pair_list_copy(&dst->phase_pairs, &s->phase_pairs);
    model::count_active_phase_pairs(dst);          // 1401d6450
    model::step_remap_material_state(*dst, false); // 1401d0fc0(dst, 0)
}

// @src 1401d7320
void model_copy_layout_table(const model::DB_STEP_DATA* m, std::int32_t* out)
{
    if (out == nullptr) return;
    for (std::int32_t i = 0; i < 0x4f; i = i + 1) out[i] = m->u15f8[i];
}

} // namespace def::cmd
