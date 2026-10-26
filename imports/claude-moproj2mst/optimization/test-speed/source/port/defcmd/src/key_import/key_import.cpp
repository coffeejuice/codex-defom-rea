// The KEY import entries 1401b6c80 .. 1401b7de0 (packet mdt-keyimport). Decompile:
// deform-rea-work/out/mdt1/f0.c; scratch-model offsets checked in the listing (1401b7250: scratch
// at RSP+0x30, so [RSP+0x16fc] is scratch+0x16cc).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/key_import.hpp>

#include <cstdlib>
#include <cstring>
#include <new>

#include <defcmd/interp.hpp>
#include <defcmd/key_file.hpp>
#include <defdbio/db_query.hpp>
#include <defmodel/gui_twins.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/step/material_select.hpp>
#include <defmodel/step/name_arrays.hpp>

namespace def::cmd {

// 14029c3c0, p8-07's (src/meshgen/meshgen_2d.cpp, declared in src/meshgen/mg_internal.hpp)
std::int32_t first_free_material(void* materials);

namespace {

// The original's DB_STEP_DATA lives on the caller's stack: 1401c8950(&local, 3) .. 1401cb2c0(&local).
class Scratch {
public:
    Scratch()
    {
        void* mem = std::calloc(1, sizeof(DB_STEP_DATA));
        if (mem == nullptr) throw std::bad_alloc();
        m_ = new (mem) DB_STEP_DATA(3);  // 1401c8950
    }
    ~Scratch()
    {
        m_->~DB_STEP_DATA();  // 1401cb2c0
        std::free(m_);
    }
    Scratch(const Scratch&) = delete;
    Scratch& operator=(const Scratch&) = delete;
    DB_STEP_DATA* get() const { return m_; }

private:
    DB_STEP_DATA* m_ = nullptr;
};

// 1401b5f00(scratch, name, &1, &0)
int read_into(DB_STEP_DATA* s, const char* name)
{
    const std::int32_t flags = 0;
    const char reset = 1;
    return read_keyword_file(s, name, &reset, &flags);
}

// model vt[0](&dim) with dim = 140158e90(scratch)
void take_dimension(DB_STEP_DATA* m, DB_STEP_DATA* s)
{
    const std::int32_t dim = model::llobj::int_at_1c(s);  // 140158e90
    m->set_dimension(&dim);                                // vtable slot 0 (1401c8ba0)
}

// first object of the scratch model that is not kind 7 (1401cabc0, 14017aaf0 on +0x1380)
model::LLOBJ_DATA* first_real_object(DB_STEP_DATA* s)
{
    return model::object_list_find(&s->objects, model::first_real_object_number(*s));
}

// strncpy(buf, path, 0x3ff), 140323d50, 140323ce0, then strncpy(dst, buf, min(strlen, limit))
void name_from_path(char* dst, const char* path, std::size_t limit)
{
    char buf[0x400] = {};  // stack buffer in the original (uninitialised; terminated for paths < 0x3ff)
    std::strncpy(buf, path, 0x3ff);
    path_strip_directory(buf);              // 140323d50
    dbio::query::strip_extension(buf);      // 140323ce0
    std::size_t n = std::strlen(buf) < limit ? std::strlen(buf) : limit;
    std::strncpy(dst, buf, static_cast<std::size_t>(static_cast<std::int32_t>(n)));
}

} // namespace

// @src 1401b6c80
int key_import_sim_ctrl(DB_STEP_DATA* m, const char* name, bool copy_names)
{
    Scratch sc;
    DB_STEP_DATA* s = sc.get();
    const int rc = read_into(s, name);
    const std::uint8_t nstart_given = step_nstart_given(s);  // 1401d7e90
    if (rc != 0) return rc;
    take_dimension(m, s);
    std::int32_t nstart = 0;
    if (nstart_given == 0) nstart = m->sim_ctrl.nstart;  // +0x8b0
    sim_ctrl_copy(&m->sim_ctrl, &s->sim_ctrl);           // 1401f3660
    if (nstart_given == 0) m->sim_ctrl.nstart = nstart;
    if (copy_names) step_copy_name_arrays(m, s);         // 1401d7430
    return rc;
}

// @src 1401b7530
bool key_import_inter_object(DB_STEP_DATA* m, const char* name)
{
    Scratch sc;
    DB_STEP_DATA* s = sc.get();
    if (read_into(s, name) != 0) return false;
    take_dimension(m, s);
    inter_object_list_copy(&m->inter_obj, &s->inter_obj);  // 1401fa0c0
    return true;
}

// @src 1401b6ea0
model::Material* key_import_material(DB_STEP_DATA* m, const char* name, std::int32_t number,
                                     bool convert_units)
{
    const std::int32_t model_unit = m->sim_ctrl.unit;  // +0x84c
    Scratch sc;
    DB_STEP_DATA* s = sc.get();
    s->sim_ctrl.unit = 0;
    if (read_into(s, name) != 0) return nullptr;
    const std::int32_t key_unit = s->sim_ctrl.unit;
    model::Material* src = model::material_list_nth(&s->materials, 1);  // 14029c260
    for (std::int32_t i = 1; i < s->materials.count; i = i + 1) {      // 140199c50: +0x10
        model::Material* p = model::material_list_nth(&s->materials, i + 1);
        if (p != nullptr && p->mstmtr_flag == 1) {
            src = p;
            break;
        }
    }
    if (src == nullptr) return nullptr;
    std::int32_t no = number;
    if (number < 1) no = first_free_material(&m->materials);         // 14029c3c0
    model::Material* dst = model::step_material_get_or_add(*m, no);  // 14019cfb0
    if (src->mstmtr_flag == 1) {
        material_import_mixture(m, dst, src, s);                                         // 14019d6b0
        if (convert_units) material_convert_units_mixture(m, dst, key_unit, model_unit);  // 14019df50
    } else {
        material_copy_keep_number(dst, src);                                // 14028d9a0
        if (convert_units) material_convert_units(dst, key_unit, model_unit);  // 14033adb0
    }
    model::call_1401d0fc0(m, 0);  // 1401d0fc0(model, 0)
    if (std::strlen(dst->name) == 0) name_from_path(dst->name, name, 0x40);
    return dst;
}

// @src 1401b7250
model::LLOBJ_DATA* key_import_object(DB_STEP_DATA* m, const char* name, std::int32_t number)
{
    Scratch sc;
    DB_STEP_DATA* s = sc.get();
    sim_ctrl_copy(&s->sim_ctrl, &m->sim_ctrl);  // 1401f3660(scratch, model)
    if (read_into(s, name) != 0) return nullptr;
    model::LLOBJ_DATA* src = first_real_object(s);
    if (src == nullptr) return nullptr;
    take_dimension(m, s);
    std::int32_t no = number;
    if (number < 1) no = model::first_free_object_number(&m->objects);  // 14017ac20
    model::LLOBJ_DATA* dst = model::llobj_ctor::get_or_create(m, no);   // 14019ccc0
    model::llobj_copy_keep_number(dst, src);                            // 140161770
    if (std::strlen(dst->name) == 0) name_from_path(dst->name, name, 0x50);
    if (0 < s->u15f8[0x35]) {                                           // scratch+0x16cc
        m->u15f8[0x35] = s->u15f8[0x35];                                // model+0x16cc
        m->sim_ctrl.trans[4] = 1;                                       // model+0x864 (TRANS 5)
    }
    return dst;
}

// @src 1401b7c00
bool key_import_mesh_settings(DB_STEP_DATA* m, const char* name, std::int32_t number)
{
    Scratch sc;
    DB_STEP_DATA* s = sc.get();
    if (read_into(s, name) != 0) return false;
    model::LLOBJ_DATA* src = first_real_object(s);
    if (src == nullptr) return false;
    take_dimension(m, s);
    model::LLOBJ_DATA* dst = model::llobj_ctor::get_or_create(m, number);  // 14019ccc0
    const char by_dim = 1;
    const char shapes = 1;
    model::llobj_copy::meshgen_copy(dst->s2d10, src->s2d10, &shapes, &by_dim);  // 140189b70
    return true;
}

// @src 1401b79f0
bool key_import_obj_properties(DB_STEP_DATA* m, const char* name, std::int32_t number)
{
    Scratch sc;
    DB_STEP_DATA* s = sc.get();
    if (read_into(s, name) != 0) return false;
    model::LLOBJ_DATA* src = first_real_object(s);
    if (src == nullptr) return false;
    model::LLOBJ_DATA* dst = model::llobj_ctor::get_or_create(m, number);  // 14019ccc0
    std::byte* d = reinterpret_cast<std::byte*>(dst);
    const std::byte* p = reinterpret_cast<const std::byte*>(src);
    std::memcpy(d + 0x1eec, p + 0x1eec, 4);
    std::memcpy(d + 0x1ef0, p + 0x1ef0, 4);
    std::memcpy(d + 0x1ef8, p + 0x1ef8, 8);
    std::memcpy(d + 0x1f00, p + 0x1f00, 8);
    std::memcpy(d + 0x1f08, p + 0x1f08, 4);
    model::llobj_copy::sf_list_copy(reinterpret_cast<model::SfLinkedList*>(d + 0x20b8),
                                    reinterpret_cast<const model::SfLinkedList*>(p + 0x20b8));  // 14025a730
    return true;
}

// @src 1401b7de0
bool key_import_geo_convt23(DB_STEP_DATA* m, const char* name, std::int32_t number)
{
    Scratch sc;
    DB_STEP_DATA* s = sc.get();
    if (read_into(s, name) != 0) return false;
    model::LLOBJ_DATA* src = first_real_object(s);
    if (src == nullptr) return false;
    take_dimension(m, s);
    model::LLOBJ_DATA* dst = model::llobj_ctor::get_or_create(m, number);  // 14019ccc0
    model::cls_1401889a0_assign(dst->s2be8, src->s2be8);                    // 140188c90
    return true;
}

} // namespace def::cmd
