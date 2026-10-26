#include "../support/test_paths.hpp"
// MDT L1 part B: the cogging geometry step of File > Save (0x140762ec0, spec_L1B.md) on corpus
// 42_vdie's cogging operation (Id 4), against the GUI's re-save of that project (oracle B,
// deform-rea-work/gui_mo/oracle_b/MDT8_VDIE42, read only):
//   - Operations/Task00004/Cogging.MDT: the billet takes its geometry from the viewer box of object 1
//     (GeoTyp -1, Length = x extent, Width = y extent, Height = Dia = z extent of the float box);
//     every other line, die and manipulator fields included, is the file as read. Hand check of the
//     billet (corpus Object00001.KEY extremes +-556.27976216 / +-202.16, rounded to float):
//     2 * 556.2797851562 = 1112.5595703125 -> 1.1125596E+003, 2 * 202.1600036621 = 404.3200073242
//     -> 4.0432001E+002;
//   - Objects/Object00010..13.KEY (written by the same Save from objects 4..7): surface nodes and
//     CNTRAX of the manipulators after the placement 0x140762f30.
// Needs packet V (engine::viewerBox); before it lands every case that reaches a box read fails with
// M2M_E_UNSUPPORTED from the viewer placeholder.
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>

#include "m2m/datafiles.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/datafiles/readers.hpp"
#include "../../src/engine/cogging_save.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

const fs::path kOracleB = m2m_test::configuredPath("M2M_ORACLE_B") / "MDT8_VDIE42";

Project* openCorpus(Project& p, Ctx& ctx, const char* caseDir, const char* moproj)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus) return nullptr;
    const fs::path f = fs::path(corpus) / caseDir / moproj;
    const qt::String file{f.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return nullptr;
    activateSubProject(p, 0, ctx);
    return &p;
}

std::string readAll(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

// the line of the MDT that starts with key (CR LF kept out)
std::string mdtLine(const std::string& text, const std::string& section, const std::string& key)
{
    std::size_t at = text.find("* " + section + "\r\n");
    if (at == std::string::npos) return {};
    at = text.find("\r\n" + key + " ", at);
    if (at == std::string::npos) return {};
    at += 2;
    return text.substr(at, text.find("\r\n", at) - at);
}

int firstDiff(const std::string& a, const std::string& b)
{
    int line = 1;
    for (std::size_t i = 0; i < a.size() && i < b.size(); ++i) {
        if (a[i] != b[i]) return line;
        if (a[i] == '\n') ++line;
    }
    return a.size() == b.size() ? 0 : line;
}

std::string e10(double v)
{
    char b[64];
    std::snprintf(b, sizeof b, "%.10E", v);
    return b;
}

// surface-0 nodes and CNTRAX of object no in the model, printed as the KEY writer's %.10E
std::string objectPrint(def::model::LLOBJ_DATA* o)
{
    std::string s;
    def::model::OBJ_GEO3D* g = def::model::geo_kw::surface(*o, 0);
    const int n = g ? g->node_count() : 0;
    s += std::to_string(n) + "\n";
    for (int i = 0; i < 3 * n; ++i) s += e10(g->coords[static_cast<std::size_t>(i)]) + (i % 3 == 2 ? "\n" : " ");
    for (int k = 0; k < 3; ++k) s += e10(*engine::at<double>(o, 0x2160 + 0x630 + 8 * k)) + " ";
    return s;
}

}  // namespace

TEST_CASE(vdie42_cogging_save_geometry)
{
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    REQUIRE(openCorpus(p, ctx, "42_vdie", "vdie.moproj") != nullptr);
    Operation* op = operationById(p, 4);
    REQUIRE(op != nullptr);
    auto* props = dynamic_cast<CoggingProps*>(op->props.get());
    REQUIRE(props != nullptr);
    datafiles::readCoggingMdt(p, *op, *props, ctx);       // Operations/Task00004/Cogging.MDT
    REQUIRE(props->model != nullptr);
    // the GUI's activation load of the active diagram item (operation 4): KEY load plus the cogging
    // class slots (L1 part A, 0x140766740 writes the primitive parameters the step reads back)
    engine::FileEffects fx;
    Operation* act = engine::openActiveOperation(p, ctx, fx);
    REQUIRE(act == op);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);

    int code = 0;
    std::string what;
    try {
        engine::coggingGeometryStep(p, *op, *props->model, true, fx);
    } catch (const Error& e) {
        code = e.code;
        what = e.what();
    }
    if (code) std::printf("  step refused: %s\n", what.c_str());
    REQUIRE(code == 0);

    const data::CogBillet& B = props->model->cog.billet;
    CHECK_EQ(B.geoTyp, -1);
    CHECK(B.length == 1112.5595703125);                   // (double)556.2797851562f * 2
    CHECK(B.width == 404.32000732421875);
    CHECK(B.height == 404.32000732421875);
    CHECK(B.dia == 404.32000732421875);

    const std::string orig = readAll(kOracleB / "Operations" / "Task00004" / "Cogging.MDT");
    REQUIRE(!orig.empty());
    const std::string out = datafiles::writeMdtBytes(*props->model, p.p06Cog, datafiles::mdtHeaderText({}));
    CHECK_EQ(mdtLine(out, "Billet", "Length"), std::string("Length      1.1125596E+003"));
    CHECK_EQ(mdtLine(out, "Billet", "Dia"), std::string("Dia         4.0432001E+002"));
    CHECK_EQ(mdtLine(out, "Billet", "Height"), std::string("Height      4.0432001E+002"));
    CHECK_EQ(mdtLine(out, "Billet", "Width"), std::string("Width       4.0432001E+002"));
    for (const char* k : {"Length", "Width", "Height", "Radius", "SPDDIE", "CYCLES", "TOTSTROKE", "MovType"})
        CHECK_EQ(mdtLine(out, "Die", k), mdtLine(orig, "Die", k));
    for (const char* k : {"Length", "Width", "Height", "Radius3", "Width1", "Height1", "Radius1", "Radius2"})
        CHECK_EQ(mdtLine(out, "Manipulator", k), mdtLine(orig, "Manipulator", k));
    const int d = firstDiff(out, orig);                   // the whole file
    CHECK_EQ(d, 0);
    if (d) std::printf("  Cogging.MDT: first difference at line %d\n", d);

    // the manipulators against the object files the same Save wrote (slot codes 10..13)
    for (int k = 4; k <= 7; ++k) {
        char name[32];
        std::snprintf(name, sizeof name, "Object%05d.KEY", 6 + k);
        engine::GuiModel ref(nullptr, 3);
        REQUIRE(ref.readKey(qt::String{(kOracleB / "Objects" / name).u16string()}));
        def::model::LLOBJ_DATA* r = ref.object(k);
        def::model::LLOBJ_DATA* o = m->object(k);
        REQUIRE(r != nullptr);
        REQUIRE(o != nullptr);
        CHECK(def::model::geo_kw::surface(*o, 0) != nullptr && def::model::geo_kw::surface(*o, 0)->node_count() > 0);
        const std::string a = objectPrint(o), b = objectPrint(r);
        const int dd = firstDiff(a, b);
        CHECK_EQ(dd, 0);
        if (dd) std::printf("  object %d: first difference at node line %d\n", k, dd);
    }
}

TEST_CASE(delete_object_repicks_pdie)
{
    engine::GuiModel w(nullptr, 3);
    def::model::DB_STEP_DATA* e = w.step.get();
    def::model::llobj_ctor::get_or_create(e, 3);
    *engine::at<std::int32_t>(e->objects.at(1), 0xc) = 1;   // deformable
    *engine::at<std::int32_t>(e->objects.at(2), 0xc) = 0;   // rigid
    *engine::at<std::int32_t>(e->objects.at(3), 0xc) = 0;
    w.setPdie(3);
    engine::deleteObject(w, 0);                            // n < 1: nothing
    CHECK_EQ(e->u1378, 3);
    engine::deleteObject(w, 5);                            // missing: the engine finds nothing
    CHECK_EQ(e->u1378, 3);
    CHECK_EQ(w.pdie(), 3);
    engine::deleteObject(w, 3);                            // 14019cdc0 empties and trims object 3
    CHECK_EQ(e->u1378, 2);
    CHECK(w.object(3) == nullptr);
    // two objects left (0x140c7c1e0 == 2): the first rigid object (1401cad40) = 2
    CHECK_EQ(w.pdie(), 2);
    engine::deleteObject(w, 2);
    CHECK_EQ(e->u1378, 1);
    // one object left: the first real object (1401cabc0) = 1
    CHECK_EQ(w.pdie(), 1);
    *engine::at<std::int32_t>(e->objects.at(1), 0xc) = 7;  // no counted object: nothing happens
    w.setPdie(1);
    engine::deleteObject(w, 1);
    CHECK_EQ(e->u1378, 1);
    CHECK_EQ(w.pdie(), 1);
}

int main()
{
    return testkit::run_all();
}
