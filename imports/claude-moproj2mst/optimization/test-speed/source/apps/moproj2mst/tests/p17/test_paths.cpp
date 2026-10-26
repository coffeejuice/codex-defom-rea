// P17: DB paths (0x1400a4f60, 0x1400b9e80 family), *!SIMTYP source, DOE paths and the *!SUBPRJ block.
// Cases that reach another packet's stub (P03 project helpers, P04 stream, P05 paths) are skipped
// while that packet is a stub.
#include <cstdio>
#include <memory>

#include <testkit/check.hpp>

#include "m2m/mst/emit.hpp"
#include "m2m/mst/state.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/p17.hpp"
#include "m2m/xml.hpp"

using m2m::qt::String;
using m2m::Project;
using m2m::SubProject;

static String S(const char* s) { return String::latin1(s); }

template <class F>
static void needs(const char* what, F body)
{
    try {
        body();
    } catch (const m2m::NotPorted& e) {
        std::printf("  skipped (%s): %s\n", what, e.what());
    }
}

static SubProject& addSub(Project& p, int type, int id, const char* dbName = "")
{
    auto s = std::make_unique<SubProject>();
    s->project = &p;
    s->type = type;
    s->id = id;
    s->dbName = S(dbName);
    p.subs.push_back(std::move(s));
    return *p.subs.back();
}

TEST_CASE(sim_type_is_the_stored_class_key)
{
    m2m::Operation op;
    op.classKey = S("SfOprHeatTransferExpress2D");   // legacy AirConvect2D after the mapping
    CHECK(m2m::simTypeName(op) == S("SfOprHeatTransferExpress2D"));
}

TEST_CASE(die_stress_db_name_skips_used_names)
{
    Project p;
    addSub(p, 0, 0);
    SubProject& d = addSub(p, 4, 1);
    CHECK(m2m::dieStressDbName(d) == S("DieStress00001.DB"));
    addSub(p, 4, 2, "DieStress00001.DB");
    addSub(p, 4, 3, "DieStress00002.DB");
    CHECK(m2m::dieStressDbName(d) == S("DieStress00003.DB"));
    SubProject orphan;
    CHECK(m2m::dieStressDbName(orphan).isNull());
}

TEST_CASE(sub_project_index)
{
    Project p;
    SubProject& main = addSub(p, 0, 0);
    addSub(p, 2, 7);
    SubProject& b = addSub(p, 2, 9);
    CHECK_EQ(m2m::mst::subProjectIndex(main), 0);
    CHECK_EQ(m2m::mst::subProjectIndex(b), 2);
    SubProject stray;
    stray.project = &p;
    stray.id = 42;
    CHECK_EQ(m2m::mst::subProjectIndex(stray), -1);
    stray.id = 0;
    CHECK_EQ(m2m::mst::subProjectIndex(stray), 0);   // id 0 is index 0 without a search
    SubProject none;
    none.id = 9;
    CHECK_EQ(m2m::mst::subProjectIndex(none), -1);
}

TEST_CASE(doe_temp_path_only_for_doe_types)
{
    Project p;
    p.path = S("C:/w/proj/COG_P1.moproj");
    SubProject& main = addSub(p, 0, 0);
    SubProject& ds = addSub(p, 4, 1);
    CHECK(m2m::mst::doeTempPath(main, nullptr).isNull());
    CHECK(m2m::mst::doeTempPath(ds, nullptr).isNull());
    SubProject loose;
    loose.type = 2;
    CHECK(m2m::mst::doeTempPath(loose, nullptr).isNull());
    SubProject& doe = addSub(p, 2, 3);
    needs("P03/P04/P05 path helpers", [&] {
        CHECK(m2m::mst::doeTempPath(doe, nullptr) ==
              S("C:/w/proj/DoeStudies/DoeStudy3/Control/COG_P1.MST.TEMP"));
        String study = S("Study A");
        int dummy = 0;
        doe.doe = reinterpret_cast<m2m::doe::Model*>(&dummy);   // only tested for non-null
        CHECK(m2m::mst::doeTempPath(doe, &study) ==
              S("C:/w/proj/DoeStudies/Study A/Control/COG_P1.MST.TEMP"));
    });
}

TEST_CASE(db_paths)
{
    Project p;
    p.path = S("C:/w/proj/COG_P1.moproj");
    SubProject& main = addSub(p, 0, 0);
    SubProject& ds = addSub(p, 4, 1);
    needs("P03/P05 path helpers", [&] {
        CHECK(m2m::projectDbPath(p, true) == S("C:/w/proj/COG_P1.DB"));
        CHECK(m2m::projectDbPath(p, false) == S("COG_P1.DB"));
        CHECK(m2m::projectDbPath(p) == S("C:/w/proj/COG_P1.DB"));
        CHECK(m2m::subProjectDbPath(main, true) == S("C:/w/proj/COG_P1.DB"));
        CHECK(m2m::subProjectDbPath(p, ds) == S("C:/w/proj/DieStress/DieStress00001.DB"));
        CHECK(m2m::subProjectDbPath(p, ds, false) == S("DieStress00001.DB"));
        CHECK(m2m::subProjectDbPath(p, main, false) == S("COG_P1.DB"));
        CHECK(ds.dbName.isEmpty());                  // the const seam does not store the name
        CHECK(m2m::subProjectDbPath(ds, false) == S("DieStress00001.DB"));
        CHECK(ds.dbName == S("DieStress00001.DB"));  // 0x1400bbee0 stores it
        CHECK(m2m::subProjectDbPath(ds, true) == S("C:/w/proj/DieStress/DieStress00001.DB"));
    });
}

TEST_CASE(sub_project_block)
{
    needs("P01 XML, P03 sub-project name, P04 stream", [] {
        Project p;
        p.dom = std::make_shared<m2m::xml::Document>();
        const std::string xml = "<Project><Sub_Project Name=\"Main\"/><Sub_Project Name=\"DOE run\"/></Project>";
        REQUIRE(m2m::xml::setContent(*p.dom, xml, nullptr, nullptr, nullptr));
        m2m::mst::Stream ts(1252);
        m2m::mst::State st;
        m2m::Ctx ctx;
        m2m::mst::W w{ts, st, ctx};
        m2m::mst::writeSubBlock(w, p, 1);
        std::string exp = "\n"
            "*-------------------------------------------------------------------------*\n"
            "*                                                                         *\n"
            "* Sub-Project                                                             *\n"
            "*                                                                         *\n"
            "*-------------------------------------------------------------------------*\n"
            "*!SUBPRJ 2\n"
            "*!PRJNAM DOE run\n"
            "*\n"
            "*-------------------------------------------------------------------------*\n";
        CHECK_EQ(ts.bytes(), exp);
    });
}

int main() { return testkit::run_all(); }
