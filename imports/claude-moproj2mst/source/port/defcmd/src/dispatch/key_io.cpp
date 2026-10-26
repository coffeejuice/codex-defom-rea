// KEY write driver (KFWRIT / menu 8), the model side of the KEY loop and the post-read clean-up
// (packet p5-01). Header: defcmd/dispatch.hpp. Decompiles: deform-rea-work/out/p501/d1.c, d2.c.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/dispatch.hpp>

#include <cstdlib>
#include <cstring>

#include <defkey/group1.hpp>
#include <defkey/key_writer.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g3/kw_g3.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/program_info.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

#if __has_include(<defdbio/gendb/pre_steps.hpp>)
#include <defdbio/gendb/pre_steps.hpp>
#define P501_HAVE_PRE_STEPS 1
#endif

// P5 gate markers (harness/gate/p5_gate.py): defined only while the interim copies are compiled
#ifndef P501_HAVE_PRE_STEPS
extern "C" const char p501_interim_1401d7f40 = 1;
extern "C" const char p501_interim_1401d6250 = 1;
extern "C" const char p501_interim_1401d63c0 = 1;
#endif

namespace def::cmd {

using model::DB_STEP_DATA;
using model::field;

// ---- KEY loop hooks ---------------------------------------------------------------------------

bool ModelKeyHooks::has_sub_model() { return model::companion_model(m_) != nullptr; } // 1401d8120

void ModelKeyHooks::set_model_dimension(int dim)
{
    const std::int32_t d = dim;
    m_->set_dimension(&d); // vtable slot 0
}

void ModelKeyHooks::set_sub_model_dimension(int dim)
{
    const std::int32_t d = dim;
    model::companion_model(m_)->set_dimension(&d);
}

// part of @src 1401b6520  the group handlers: 14039aa90, 14038b8a0, 140388fb0, 140384a50,
// 140383120, 140378e00 on 1401d81b0(model), 14037e530 on the model
int ModelKeyHooks::parse_keyword(int group, std::FILE* f, int id)
{
    if (group == 7) return key::g57::read_dim_keyword(m_, f, id);
    DB_STEP_DATA* t = model::keyword_target_model(m_);
    switch (group) {
    case 1: return key::read_group1(t, f, id);
    case 2: return key::g2obj::read_object_keyword(*t, f, id);
    case 3: return key::g3::read_keyword(t, f, id);
    case 4: return key::g4::read_keyword(*t, f, id);
    case 5: return key::g57::read_phase_keyword(t, f, id);
    case 6: return dispatch(t, f, id);
    default: return 3;
    }
}

// ---- KEY writer -------------------------------------------------------------------------------

// @src 1401c5810
std::int32_t write_key_file(DB_STEP_DATA* m, const char* name, const std::uint32_t* flags)
{
    std::int32_t status = 0;
    if (name == nullptr || std::strlen(name) == 0) return 1;
    if (m->u1b5a == 0) { // 1401c6e40
        m->u1744 = static_cast<std::int32_t>(*flags);
        std::FILE* f = _fsopen(name, "wt", 0x40); // 14047b934, str 1405284b4
        if (f == nullptr) return 1;
        if ((*flags & 0x80000000u) == 0)
            status = write_key_stream(m, f);
        else
            status = write_key_stream_short(m, f);
        std::fclose(f);
        return status;
    }
    // the model only holds the DB position: read that step into a temporary model and write it
    DB_STEP_DATA* tmp = new DB_STEP_DATA(3);
    tmp->u1b58 = 1; // 1401c6e00
    tmp->u1b59 = 1; // 1401c6e20
    const int rc = dispatch_hooks().db_read_step(tmp, m->db, m->u744, 0, 0);
    if (rc == 0) {
        std::FILE* f = _fsopen(name, "wt", 0x40); // str 1405283bc
        status = write_key_stream(tmp, f);
        std::fclose(f);
    } else {
        status = 2;
    }
    delete tmp; // 1401cb2c0 (the original's model lives on the stack)
    return status;
}

// @src 1401c5af0
int write_key_stream(DB_STEP_DATA* m, std::FILE* f)
{
    // @src 1401c5a20  the banner with dim_option 0: product of the model's dimension, version
    key::write_file_banner(f, 0, model::product_name_for_dim(m->dimension), model::version());
    int rc = key::write_group1(m, f);
    if (rc != 0) return rc;
    if (m->n_materials == 0) std::fprintf(f, "*\n*  %s\n*\n", "Property Data of Material");
    for (std::int32_t i = 0; i < m->n_materials; ++i) {
        rc = key::g4::write_material(*m, f, model::material_list_nth(&m->materials, i + 1), nullptr);
        if (rc != 0) return rc;
    }
    std::fprintf(f, "*\n*  %s\n*\n", "Inter-Material Data");
    for (std::int32_t i = 0; i < model::phase_pair_count(&m->phase_pairs); ++i) {
        rc = key::g57::write_phase_pair(m, f, model::phase_pair_nth(&m->phase_pairs, i + 1), i + 1);
        if (rc != 0) return rc;
    }
    for (std::int32_t i = 0; i < m->u1378; ++i) {
        rc = key::g2obj::write_object(*m, f, m->objects.at(i + 1), 0, false);
        if (rc != 0) return rc;
    }
    std::fprintf(f, "*\n*  %s\n*\n", "Inter-Object Data");
    return key::g3::write_inter_objects(m, f);
}

// @src 1401c5d20
int write_key_stream_short(DB_STEP_DATA* m, std::FILE* f)
{
    if (rt::dimension_is_2(m))
        std::fprintf(f, "%-6s %7i\n", key::keyword_label(0x1c), field<std::int32_t>(m, 0x850));
    for (std::int32_t i = 0; i < m->u1378; ++i) {
        const int rc = dispatch_hooks().write_object_short(m, f, m->objects.at(i + 1), 0);
        if (rc != 0) return rc;
    }
    return 0;
}

// ---- after a KEY read ---------------------------------------------------------------------------

// 1401d7f40: the P4 gate's def::dbio::gendb::gendb_command_pre_steps when its header is in the
// tree; until then an interim copy (owner P4 gate, no @src tag here), with 1401d6250 / 1401d63c0
void finish_key_read(DB_STEP_DATA* m)
{
#ifdef P501_HAVE_PRE_STEPS
    def::dbio::gendb::gendb_command_pre_steps(m);
#else
#pragma message("P501-INTERIM 1401d7f40 compiled (owner P4 gate, defdbio/gendb/pre_steps.hpp absent)")

    model::sim_ctrl_update_u240(m->sim_ctrl); // 1401ee510
    reset_linked_materials(m);
    update_object_surfaces(m, false);
    if (m->u2f88 != 0) {
        std::int32_t& nstart = field<std::int32_t>(m, 0x8b0);
        if (nstart > -1) nstart = -(nstart + 1);
        m->u744 = nstart;
    }
#endif
}

#ifndef P501_HAVE_PRE_STEPS
#pragma message("P501-INTERIM 1401d6250 1401d63c0 compiled (owner P4 gate, defdbio/gendb/pre_steps.hpp absent)")

// part of 1401d7f40 interim copy (owner P4 gate): 1401d6250
void reset_linked_materials(DB_STEP_DATA* m)
{
    for (std::int32_t i = 0; i < m->n_materials; ++i) {
        model::Material* a = model::material_list_nth(&m->materials, i + 1);
        if (a == nullptr || field<std::int32_t>(a, 0x1cc8) != 1) continue;
        for (std::int32_t j = 0; j < field<std::int32_t>(a, 0x1ccc); ++j) {
            const std::int32_t no = field<std::int32_t*>(a, 0x1cd0)[j];
            model::Material* b = model::material_list_find(&m->materials, no);
            if (b == nullptr || field<std::int32_t>(b, 0x1cc8) == 2) continue;
            if (field<std::int32_t>(b, 0x1ccc) > 0) {
                if (field<std::int32_t*>(b, 0x1cd0) != nullptr) std::free(field<std::int32_t*>(b, 0x1cd0));
                field<std::int32_t*>(b, 0x1cd0) = nullptr;
                field<std::int32_t>(b, 0x1ccc) = 0;
            }
            field<std::int32_t>(b, 0x1cc8) = 2;
        }
    }
}

// part of 1401d7f40 interim copy (owner P4 gate): 1401d63c0
void update_object_surfaces(DB_STEP_DATA* m, bool flag)
{
    for (std::int32_t i = 0; i < m->u1378; ++i) {
        model::LLOBJ_DATA* o = m->objects.at(i + 1);
        if (o != nullptr && field<std::int32_t>(o, 0xc) != 7)
            dispatch_hooks().object_surface_update(o, flag);
    }
}

#endif

// ---- DB step lookup (DBREAD) --------------------------------------------------------------------

// @src 1401e3d40
std::int32_t db_step_compare(std::int32_t a, std::int32_t b)
{
    if (a == b) return 0;
    double x;
    double y;
    if (a < 1)
        x = -1.0 * static_cast<double>(a) - 0.5; // DAT_140522fc8, DAT_14051f240
    else
        x = static_cast<double>(a);
    if (b < 1)
        y = -1.0 * static_cast<double>(b) - 0.5;
    else
        y = static_cast<double>(b);
    return x <= y ? -1 : 1;
}

// @src 1401e3b60
std::int32_t db_find_step(const model::Cls_1401dbfe0* db, std::int32_t step)
{
    const auto* dir = static_cast<const std::byte*>(db->uad8);
    for (std::int32_t i = db->uac8 - 1; i >= 0; --i) {
        const std::size_t e = static_cast<std::size_t>(i) * 0x28;
        const std::int32_t no = field<std::int32_t>(dir, e + 4);
        if (db_step_compare(no, step) <= 0 && field<std::int32_t>(dir, e + 0x24) != 0) return no;
    }
    return 0;
}

} // namespace def::cmd
