#include "../support/test_paths.hpp"
// MDT L4 (DB-5): "Keep Mesh of Dies?" (0x1413fbd10) against oracle B. 130_B_DBSTEP_FE is
// SfOprFormingExpress at OprFlag 0 with meshed dies in the project DB; the GUI asked Setup Type and
// "Keep Mesh of Dies?" at open and Return answered both (Yes). 130n_B_DBSTEP_FE_no is the same
// project answered No. Each case is copied to a scratch folder and converted with M2M_FLAG_WRITE_SAVE
// (130n with mst::g_keepMeshOfDiesNo); the files the Save wrote must equal the GUI's, byte for byte.
// The .moproj's view parts follow the GUI window (packet moproj), so it is not compared here.
// ctest-labels: m2m-mdt m2m-mdt-l4
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "m2m/convert.hpp"
#include "m2m/mst/loader_l4.hpp"

namespace fs = std::filesystem;

namespace {

fs::path oracleB()
{
    if (const char* e = std::getenv("M2M_ORACLE_B")) return fs::path(e);
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

const char* const kFiles[] = {
    "COG_P1_B1.MST",
    "Equipments/Equipment00005.KEY",
    "Operations/Task00002/Forming.FWZ",
    "Operations/Task00002/InterObject.KEY",
    "Operations/Task00002/RestartStep.KEY",
    "Operations/Task00002/SimCtrl.KEY",
};

int runCase(const char* name, bool answerNo)
{
    const fs::path in = oracleB() / (std::string(name) + "_before");
    const fs::path want = oracleB() / name;
    if (!fs::exists(in) || !fs::exists(want)) {
        std::printf("SKIP %s: no oracle B\n", name);
        return 0;
    }
    const fs::path work = m2m_test::scratchRoot() / "m2m_db5_keepmesh" / name;
    std::error_code ec;
    fs::remove_all(work, ec);
    fs::create_directories(work.parent_path());
    fs::copy(in, work, fs::copy_options::recursive);
    m2m::mst::g_keepMeshOfDiesNo = answerNo;
    const m2m::Result r = m2m::convert((work / "COG_P1_B1.moproj").u16string(), u"", M2M_FLAG_WRITE_SAVE, 1252);
    m2m::mst::g_keepMeshOfDiesNo = false;
    if (r.code != M2M_OK) {
        std::printf("FAIL %s: convert %d %s\n", name, r.code, r.message.c_str());
        return 1;
    }
    int bad = 0;
    for (const char* f : kFiles) {
        const std::string got = readAll(work / f), exp = readAll(want / f);
        const bool same = got == exp;
        std::printf("%s %s %s (%zu vs %zu bytes)\n", same ? "SAME" : "DIFF", name, f, got.size(), exp.size());
        if (!same) ++bad;
    }
    fs::remove_all(work, ec);
    return bad;
}

}  // namespace

// Yes: the dialog's default (Return)
TEST_CASE(keep_mesh_yes_130)
{
    CHECK_EQ(runCase("130_B_DBSTEP_FE", false), 0);
}

// No: vt+0x2c0(OB, 0) deletes the die meshes; the setting stays
TEST_CASE(keep_mesh_no_130n)
{
    CHECK_EQ(runCase("130n_B_DBSTEP_FE_no", true), 0);
}

int main()
{
    return testkit::run_all();
}
