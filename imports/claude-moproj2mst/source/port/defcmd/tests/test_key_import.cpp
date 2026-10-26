// ctest-labels: mdt key_import
// KEY import family 1401b6c80 .. 1401b7de0 (defcmd/key_import.hpp). The entries run on corpus KEY
// files of DEF_GUI_MO_64 projects (read only; $M2M_CORPUS or the default path; skipped when
// absent) and on small KEY files written to %TEMP%. Expected values are read off the KEY text
// (keyword specs in docs/re/keywords/) or computed in the test with the listing's operation order
// from the unit table helpers (defrt unit_factor / convert_temperature, verified in defrt tests).
// The single-record copies 14028d370 / 1403357a0 (p8-08, not exported yet) are replaced by a test
// double through g_key_import_ext where a test needs them.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/key_import.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <new>
#include <string>

#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_1401f46e0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/layout/phase_pair.hpp>
#include <defmodel/llobj/object_list.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/material_select.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defrt/ftn/units.hpp>
#include <defrt/stdlib/libm.hpp>
#include <testkit/check.hpp>

using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;
using def::model::Material;
using def::model::PhasePair;
namespace kc = def::cmd;
namespace fs = std::filesystem;

namespace {

fs::path corpus()
{
    const char* env = std::getenv("M2M_CORPUS");
    return env ? fs::path(env) : fs::path("C:/Users/alext/Projects/deform-rea-work/gui_mo/s1/corpus");
}

bool have(const fs::path& p)
{
    if (fs::exists(p)) return true;
    std::printf("SKIP: %s not found\n", p.string().c_str());
    return false;
}

DB_STEP_DATA* new_model()
{
    void* mem = std::calloc(1, sizeof(DB_STEP_DATA));
    return new (mem) DB_STEP_DATA(3);  // leaked, like the other defcmd tests
}

std::string temp_key(const char* name, const char* text)
{
    const fs::path p = fs::temp_directory_path() / name;
    std::FILE* f = std::fopen(p.string().c_str(), "wb");
    std::fputs(text, f);
    std::fclose(f);
    return p.string();
}

template <class T> T rd(const void* p, std::size_t off)
{
    T v;
    std::memcpy(&v, static_cast<const std::byte*>(p) + off, sizeof v);
    return v;
}
double& dref(void* p, std::size_t off) { return *reinterpret_cast<double*>(static_cast<std::byte*>(p) + off); }

double F(int k, int mode) { return def::rt::ftn::unit_factor(k, mode); }
double T(int mode, double t) { return def::rt::ftn::convert_temperature(mode, t); }

// Test double for 14028d370: the fields the import logic reads (number, name, MSTMTR).
bool g_copy_name = true;
void fake_material_copy(Material* d, const Material* s)
{
    d->number = s->number;
    if (g_copy_name) std::memcpy(d->name, s->name, sizeof d->name);
    d->mstmtr_flag = s->mstmtr_flag;
    d->mstmtr_count = s->mstmtr_count;
}

} // namespace

// ---- helpers -----------------------------------------------------------------------------------

TEST_CASE(path_helpers)
{
    char a[64] = "C:\\dir/sub\\Material00001.KEY";
    kc::path_strip_directory(a);
    CHECK_EQ(std::string(a), std::string("Material00001.KEY"));
    char b[64] = "no_sep.a.b";
    kc::path_strip_directory(b);  // no separator: unchanged
    CHECK_EQ(std::string(b), std::string("no_sep.a.b"));
    char c[64] = "dir/";
    kc::path_strip_directory(c);
    CHECK_EQ(std::string(c), std::string(""));
}

TEST_CASE(dynary_multi_kind_27)
{
    DYNARY t;
    def::model::dynary_init(&t, 0);
    t.ndim = 1;
    t.var[0] = 3;  // kVarUnitKind[3] = 9: temperature axis
    t.n[0] = 2;
    t.ncomp = 2;
    def::model::dynary_alloc(&t);
    t.axis[0][0] = 100.0;
    t.axis[0][1] = 0.0;
    for (int i = 0; i < 4; ++i) t.values[i] = 1.5 + i;
    kc::dynary_convert_units_multi(&t, 0x27, 1);
    CHECK_BITS(t.axis[0][0], T(1, 100.0));
    CHECK_BITS(t.axis[0][1], T(1, 0.0));
    CHECK_BITS(t.values[0], 1.5 * F(0x1a, 1));
    CHECK_BITS(t.values[1], 2.5 * F(0xb, 1));
    CHECK_BITS(t.values[2], 3.5 * F(0x1a, 1));
    CHECK_BITS(t.values[3], 4.5 * F(0xb, 1));
    // ncomp 1: untouched
    t.ncomp = 1;
    kc::dynary_convert_units_multi(&t, 0x27, 1);
    CHECK_BITS(t.values[0], 1.5 * F(0x1a, 1));
}

TEST_CASE(dynary_multi_kind_47_pow)
{
    DYNARY t;
    def::model::dynary_init(&t, 0);
    t.ndim = 1;
    t.var[0] = 1;  // kVarUnitKind[1] = 0
    t.n[0] = 1;
    t.ncomp = 5;
    def::model::dynary_alloc(&t);
    t.axis[0][0] = 2.0;
    const double v[5] = {3.0, 2.0, 7.0, 5.0, -1.0};
    for (int i = 0; i < 5; ++i) t.values[i] = v[i];
    kc::dynary_convert_units_multi(&t, 0x47, 2);
    CHECK_BITS(t.axis[0][0], 2.0 * F(0, 2));
    CHECK_BITS(t.values[0], 3.0 * def::rt::libm::crt_pow(F(7, 2), -2.0));
    CHECK_BITS(t.values[3], 5.0 * def::rt::libm::crt_pow(F(7, 2), 1.0));
    CHECK_BITS(t.values[1], 2.0);
    CHECK_BITS(t.values[2], 7.0);
}

TEST_CASE(material_and_phase_pair_conversion)
{
    DB_STEP_DATA* m = new_model();
    Material* mat = def::model::step_material_get_or_add(*m, 1);
    dref(mat, 0x58) = 2.0;
    dref(mat, 0xf0) = 3.0;
    dref(mat, 0x2f0) = 100.0;
    dref(mat, 0x120) = 4.0;
    dref(mat, 0x128) = 0.5;
    kc::material_convert_units(mat, 2, 2);  // same system: nothing
    CHECK_BITS(dref(mat, 0x58), 2.0);
    kc::material_convert_units(mat, 1, 2);  // mode 1
    CHECK_BITS(dref(mat, 0x58), 2.0 * F(7, 1));
    CHECK_BITS(dref(mat, 0xf0), 3.0 * (1.0 / F(7, 1)));
    CHECK_BITS(dref(mat, 0x2f0), 212.0);  // 100 C -> F
    CHECK_BITS(dref(mat, 0x120), 4.0 * def::rt::libm::crt_pow(F(7, 1), -0.5));

    PhasePair* p = def::model::phase_pair_append(&m->phase_pairs);
    dref(p, 0x360) = 1.0;
    dref(p, 0x380) = 10.0;
    kc::phase_pair_convert_mode(p, 1);
    const double c360 = 1.0 * (1.0 / F(9, 1));
    CHECK_BITS(dref(p, 0x360), c360);
    CHECK_BITS(dref(p, 0x380), 10.0 - c360 * 32.0);
    CHECK_EQ(rd<std::int32_t>(p, 0x5e8), 0x2a);
    dref(p, 0x360) = 1.0;
    dref(p, 0x380) = 10.0;
    kc::phase_pair_convert_mode(p, 2);
    const double d360 = 1.0 * (1.0 / F(9, 2));
    CHECK_BITS(dref(p, 0x380), 10.0 + (d360 * 32.0) / 1.8);
}

// ---- entries on corpus files ---------------------------------------------------------------------

TEST_CASE(sim_ctrl_from_corpus)
{
    const fs::path f = corpus() / "42_vdie/Operations/Task00004/SimCtrl.KEY";
    if (!have(f)) return;
    DB_STEP_DATA* m = new_model();
    m->sim_ctrl.nstart = 7;
    CHECK_EQ(kc::key_import_sim_ctrl(m, f.string().c_str(), true), 0);
    // CURSIM 4 4 0, SMODE 3, UNIT 1, STPINC 100000, NSTEP 100000; no NSTART line -> kept.
    // The copy 1401e8040 takes CURSIM 2 and 3 only (+0xe8, +0xec): CURSIM 1 keeps the default 1.
    CHECK_EQ(m->sim_ctrl.cursim[0], 1);
    CHECK_EQ(m->sim_ctrl.cursim[1], 4);
    CHECK_EQ(m->sim_ctrl.cursim[2], 0);
    CHECK_EQ(m->sim_ctrl.smode, 3);
    CHECK_EQ(m->sim_ctrl.unit, 1);
    CHECK_EQ(m->sim_ctrl.stpinc, 100000);
    CHECK_EQ(m->sim_ctrl.nstep, 100000);
    CHECK_EQ(m->sim_ctrl.nstart, 7);
    CHECK_EQ(m->dimension, 3);
    CHECK_EQ(kc::key_import_sim_ctrl(m, "no_such_file.KEY", false), 1);
}

TEST_CASE(sim_ctrl_nstart_given)
{
    const std::string f = temp_key("mdt_ki_nstart.KEY", "NSTART      12\nNSTEP       34\n");
    DB_STEP_DATA* m = new_model();
    m->sim_ctrl.nstart = 7;
    CHECK_EQ(kc::key_import_sim_ctrl(m, f.c_str(), false), 0);
    CHECK_EQ(m->sim_ctrl.nstart, -13);  // 1401d7f40 after the read: NSTART given and >= 0 -> -(n + 1)
    CHECK_EQ(m->sim_ctrl.nstep, 34);
}

TEST_CASE(inter_object_from_corpus)
{
    const fs::path f = corpus() / "42_vdie/Operations/Task00004/InterObject.KEY";
    if (!have(f)) return;
    DB_STEP_DATA* m = new_model();
    def::model::pair_append(&m->inter_obj);  // replaced by the import
    CHECK(kc::key_import_inter_object(m, f.string().c_str()));
    // 8 CNTACT pairs, each mirrored (CNTACT.md "Mirroring"): (1,2),(2,1),(1,3),(3,1),...
    CHECK_EQ(def::model::pair_count(&m->inter_obj), 16);
    def::model::Cls_1401f46e0* p1 = def::model::pair_nth(&m->inter_obj, 1);
    def::model::Cls_1401f46e0* p2 = def::model::pair_nth(&m->inter_obj, 2);
    CHECK_EQ(p1->u168, 1);
    CHECK_EQ(p1->u16c, 2);
    CHECK_EQ(p1->u170, 1);  // CNTACT 1 2 1 (3D d = 1 -> 1)
    CHECK_EQ(p2->u168, 2);
    CHECK_EQ(p2->u16c, 1);
    CHECK_EQ(p2->u170, 2);  // mirror of direction 1
    def::model::Cls_1401f46e0* last = def::model::pair_nth(&m->inter_obj, 16);
    CHECK_EQ(last->u168, 7);
    CHECK_EQ(last->u16c, 6);
    CHECK(last->next == nullptr);
    CHECK(!kc::key_import_inter_object(m, "no_such_file.KEY"));
}

TEST_CASE(object_from_corpus)
{
    const fs::path f = corpus() / "42_vdie/Objects/Object00006.KEY";
    if (!have(f)) return;
    DB_STEP_DATA* m = new_model();
    LLOBJ_DATA* o = kc::key_import_object(m, f.string().c_str(), 3);
    REQUIRE(o != nullptr);
    CHECK_EQ(rd<std::int32_t>(o, 8), 3);
    CHECK_EQ(std::string(o->name), std::string("Plate for dies positioning"));  // OBJNAM
    CHECK(def::model::object_list_find(&m->objects, 3) == o);
    // get_or_create(3) made 1 and 2 kind-7 placeholders, so the first free number is 4
    LLOBJ_DATA* o2 = kc::key_import_object(m, f.string().c_str(), 0);
    REQUIRE(o2 != nullptr);
    CHECK_EQ(rd<std::int32_t>(o2, 8), 4);
    CHECK(kc::key_import_object(m, "no_such_file.KEY", 1) == nullptr);
}

TEST_CASE(object_name_from_path)
{
    const std::string f = temp_key("mdt_ki_ObjNoName.KEY", "OBJTYP       1       1       0\n");
    DB_STEP_DATA* m = new_model();
    LLOBJ_DATA* o = kc::key_import_object(m, f.c_str(), 2);
    REQUIRE(o != nullptr);
    CHECK_EQ(std::string(o->name), std::string("mdt_ki_ObjNoName"));
}

TEST_CASE(mesh_settings_from_corpus)
{
    const fs::path f = corpus() / "46_MO2_Boolean_Oprations/Operations/Task00001/MeshSettings00001.KEY";
    if (!have(f)) return;
    DB_STEP_DATA* m = new_model();
    CHECK(kc::key_import_mesh_settings(m, f.string().c_str(), 2));
    LLOBJ_DATA* o = def::model::object_list_find(&m->objects, 2);
    REQUIRE(o != nullptr);
    // The file's "DEFORM-2D" banner makes the scratch model 2D; vt[0] passes that on to the model,
    // so 140189b70 copies the 2D field +0x70 (MGNELM.md: 2D n -> obj+0x2d80, b -> obj+0x2d8c).
    CHECK_EQ(m->dimension, 2);
    CHECK_BITS(rd<double>(o, 0x2d20), 4.0);           // MGSIZR 1 4.0
    CHECK_EQ(rd<std::int32_t>(o, 0x2d80), 1000);      // MGNELM 1 1000 32000 0
    CHECK_EQ(rd<std::int32_t>(o, 0x2d8c), 32000);
}

TEST_CASE(obj_properties)
{
    const std::string f = temp_key("mdt_ki_props.KEY", "OTPRNG       1      -4    2.5    3.5       7\n");
    DB_STEP_DATA* m = new_model();
    CHECK(kc::key_import_obj_properties(m, f.c_str(), 5));
    LLOBJ_DATA* o = def::model::object_list_find(&m->objects, 5);
    REQUIRE(o != nullptr);
    CHECK_EQ(rd<std::int32_t>(o, 0x1eec), 3);  // negative mode: 3 and |mode|
    CHECK_EQ(rd<std::int32_t>(o, 0x1ef0), 4);
    CHECK_BITS(rd<double>(o, 0x1ef8), 2.5);
    CHECK_BITS(rd<double>(o, 0x1f00), 3.5);
    CHECK_EQ(rd<std::int32_t>(o, 0x1f08), 7);
    const fs::path c = corpus() / "42_vdie/Operations/Task00004/ObjProperties00001.KEY";
    if (have(c)) {
        CHECK(kc::key_import_obj_properties(m, c.string().c_str(), 5));  // OTPRNG 1 0 0 0 0
        CHECK_EQ(rd<std::int32_t>(o, 0x1eec), 0);
        CHECK_EQ(rd<std::int32_t>(o, 0x1ef0), 0);  // mode 0 leaves the scratch default 0, which is copied
    }
}

TEST_CASE(geo_convt23)
{
    const std::string f = temp_key("mdt_ki_geo23.KEY", "GEO23        1       1       5\n 1.0 2.0 3.0\n");
    DB_STEP_DATA* m = new_model();
    CHECK(kc::key_import_geo_convt23(m, f.c_str(), 4));
    LLOBJ_DATA* o = def::model::object_list_find(&m->objects, 4);
    REQUIRE(o != nullptr);
    CHECK_EQ(rd<std::int32_t>(o, 0x2be8), 1);
    CHECK_BITS(rd<double>(o, 0x2bf0), 1.0);
    CHECK_BITS(rd<double>(o, 0x2bf8), 2.0);
    CHECK_BITS(rd<double>(o, 0x2c00), 3.0);
    CHECK_EQ(rd<std::int32_t>(o, 0x2c08), 5);
    CHECK(!kc::key_import_geo_convt23(m, "no_such_file.KEY", 4));
}

TEST_CASE(material_from_corpus)
{
    const fs::path f = corpus() / "42_vdie/Materials/Material00001.KEY";
    if (!have(f)) return;
    kc::g_key_import_ext.material_copy = fake_material_copy;
    DB_STEP_DATA* m = new_model();
    g_copy_name = true;
    Material* mat = kc::key_import_material(m, f.string().c_str(), 0, false);
    REQUIRE(mat != nullptr);
    CHECK_EQ(mat->number, 1);  // first free number, kept over the copy
    CHECK_EQ(std::string(mat->name), std::string("INC718 JMatPro"));  // MTNAME
    CHECK_EQ(m->n_materials, 1);
    g_copy_name = false;
    Material* m3 = kc::key_import_material(m, f.string().c_str(), 3, false);
    REQUIRE(m3 != nullptr);
    CHECK_EQ(m3->number, 3);
    // base name of the file: strncpy(name, base, strlen(base)) writes no terminator, as in the
    // original; the bytes after it are whatever the new material's heap block held
    CHECK_EQ(std::strncmp(m3->name, "Material00001", 13), 0);
    CHECK_EQ(m->n_materials, 2);
    CHECK(kc::key_import_material(m, "no_such_file.KEY", 1, false) == nullptr);
    kc::g_key_import_ext.material_copy = nullptr;
}

int main() { return testkit::run_all(); }
