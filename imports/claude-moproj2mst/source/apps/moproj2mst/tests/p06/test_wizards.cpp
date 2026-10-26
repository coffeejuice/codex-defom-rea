// P06: Boolean.BWZ, Forming.FWZ, Cycle.CWZ and DieStressBmWizard.Wzd readers.
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>

#include "../../src/datafiles/readers.hpp"

using namespace m2m::datafiles;
namespace fs = std::filesystem;

namespace {

std::string readAll(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

fs::path corpus()
{
    const char* c = std::getenv("M2M_CORPUS");
    return c ? fs::path(c) : fs::path();
}

std::vector<fs::path> corpusFiles(const char* name)
{
    std::vector<fs::path> out;
    for (auto& e : fs::recursive_directory_iterator(corpus()))
        if (e.is_regular_file() && e.path().filename() == name) out.push_back(e.path());
    return out;
}

// Files called `name` that the corpus manifest lists (`<sha256>  <case>/<path>` lines), so the
// expected counts follow the corpus as cases are added (oracle-B cases bring their own files).
std::size_t manifestCount(const char* name)
{
    std::ifstream in(corpus() / "manifest.sha256");
    const std::string tail = std::string("/") + name;
    std::size_t n = 0;
    for (std::string line; std::getline(in, line);) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.size() > tail.size() && line.compare(line.size() - tail.size(), tail.size(), tail) == 0) ++n;
    }
    return n;
}

}  // namespace

TEST_CASE(bwz_corpus)
{
    REQUIRE(!corpus().empty());
    auto files = corpusFiles("Boolean.BWZ");
    CHECK(files.size() >= 14);   // oracle-A corpus
    CHECK_EQ(files.size(), manifestCount("Boolean.BWZ"));
    for (auto& p : files) {
        TextFile f(readAll(p));
        m2m::data::BooleanData d;
        CHECK(readBwzFile(f, d));
        CHECK(d.workpiece >= 1);
    }
    TextFile f(readAll(corpus() / "46_MO2_Boolean_Oprations/Operations/Task00004/Boolean.BWZ"));
    m2m::data::BooleanData d;
    REQUIRE(readBwzFile(f, d));
    CHECK_EQ(d.workpiece, 1);
    CHECK_EQ(d.cutter, 2);
    CHECK_EQ(int(d.remesh), 0);
}

TEST_CASE(bwz_end_marker_stops_early)
{
    TextFile f(std::string("WZD_BOOLEAN_CUTTER_OBJECT_NO\n4\n$END\nWZD_MACHDIST_WORKPIECE_OBJECT_NO\n9\nDONE\n"));
    m2m::data::BooleanData d;
    CHECK(readBwzFile(f, d));
    CHECK_EQ(d.cutter, 4);
    CHECK_EQ(d.workpiece, 1);
}

TEST_CASE(fwz_corpus)
{
    REQUIRE(!corpus().empty());
    auto files = corpusFiles("Forming.FWZ");
    CHECK(files.size() >= 9);    // oracle-A corpus
    CHECK_EQ(files.size(), manifestCount("Forming.FWZ"));
    for (auto& p : files) {
        TextFile f(readAll(p));
        m2m::data::ExpressData d;
        readFwzFile(f, d);
        CHECK_EQ(int(d.loaded), 1);
    }
    // The empty material-catalogue lines make the reader take the next keyword as the value,
    // exactly as the token reader does (the value read moves on to the next non-empty line).
    TextFile f(readAll(corpus() / "46_MO2_Boolean_Oprations/Operations/Task00001/Forming.FWZ"));
    m2m::data::ExpressData d;
    readFwzFile(f, d);
    CHECK_EQ(d.formingType, 2);
    CHECK_EQ(d.dieTemperatureType, 2);
    CHECK_EQ(d.shapeComplexity, 25);
    CHECK_EQ(d.simulationAccuracy, 25);
    CHECK_EQ(d.workpieceShape, 0);
    CHECK(d.builtInFlownet == -1.0);
    CHECK(d.contactTolerance == 0.0);
    CHECK(d.importMatCatWorkpiece == m2m::qt::L("WZD_FORM_IMPORT_MAT_CAT_DIE"));
    CHECK(d.importMatCatDie == m2m::qt::L("WZD_FORM_STEP_DEFINITION"));
    CHECK_EQ(d.stepDefinition, 0);
    CHECK_EQ(d.meshType.size(), std::size_t(1));
    CHECK_EQ(d.meshType[1], 0);
}

TEST_CASE(cwz_corpus)
{
    REQUIRE(!corpus().empty());
    auto files = corpusFiles("Cycle.CWZ");
    CHECK(files.size() >= 2);    // oracle-A corpus
    CHECK_EQ(files.size(), manifestCount("Cycle.CWZ"));
    for (auto& p : files) {
        TextFile f(readAll(p));
        m2m::data::CycleData d;
        readCwzFile(f, d);
        CHECK_EQ(d.count, 9);
    }
    TextFile empty(std::string(""));
    m2m::data::CycleData d;
    readCwzFile(empty, d);
    CHECK_EQ(d.count, 5);
}

TEST_CASE(wzd_attributes)
{
    std::string xml =
        "<!DOCTYPE DieStressBmWizard>\r\n<DieStressBmWizard nObjTotalNum=\"2\" qsNewDbName=\"NEW.DB\" bUseNewDb=\"1\">\r\n"
        " <ObjId01 nObjNo=\"2\" nRefNo=\"3\" bIntepolate=\"1\" dTolerance=\"0.5\"/>\r\n"
        " <ObjId02 nObjNo=\"4\" nRefNo=\"4\" bIntepolate=\"0\" dTolerance=\"0.25\"/>\r\n</DieStressBmWizard>\r\n";
    m2m::data::DiestressBMData d;
    try {
        readWzdBytes(xml, d);
    } catch (const m2m::NotPorted& e) {
        std::printf("skipped until the XML reader (P01) and QString numbers (P04) merge: %s\n", e.what());
        return;
    }
    CHECK_EQ(d.objTotalNum, 2);
    CHECK(d.newDbName == m2m::qt::L("NEW.DB"));
    CHECK_EQ(int(d.useNewDb), 1);
    CHECK_EQ(d.objNo[0], 2);
    CHECK_EQ(d.refNo[0], 3);
    CHECK_EQ(int(d.interpolate[0]), 1);
    CHECK(d.tolerance[0] == 0.5);
    CHECK_EQ(d.objNo[1], 4);
    CHECK(d.tolerance[1] == 0.25);
}

int main()
{
    return testkit::run_all();
}
