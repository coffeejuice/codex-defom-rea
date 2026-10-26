#include "../support/test_paths.hpp"
// MDT L4S: save current operation 0x140052ec0 of an active MultiBlow operation (vt+0xd8 0x140602780,
// vt+0x80 0x140602770) against oracle B 93_B_MB3ACT (75F, Active = 2, SfOprMultiBlow3D) and
// 94_B_MB2ACT (92_B_MB2REL, Active = 3, SfOprMultiBlow2D): <case>_before is opened, the active
// operation loaded and saved; every KEY file the port records must equal the file DEF_GUI_MO_64
// wrote in Save step 2 (<case>, files whose mtime lies in the second before the saved .moproj; the
// method of tests/s/test_save_current.cpp).
// ctest-labels: m2m-mdt-l4
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <set>
#include <string>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

fs::path oracleB()
{
    if (const char* e = std::getenv("M2M_ORACLE_B")) return e;
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

std::string lower(std::string s)
{
    for (char& c : s)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    return s;
}

// Relative paths (lower case, '/') of the files the GUI wrote in Save step 2.
std::set<std::string> guiStep2(const fs::path& dir)
{
    fs::path moproj;
    for (const auto& e : fs::directory_iterator(dir))
        if (e.path().extension() == ".moproj") moproj = e.path();
    const auto t0 = fs::last_write_time(moproj);
    std::set<std::string> out;
    for (const auto& e : fs::recursive_directory_iterator(dir)) {
        if (!e.is_regular_file()) continue;
        const auto t = e.last_write_time();
        if (t < t0 && t0 - t < std::chrono::seconds(1))
            out.insert(lower(fs::relative(e.path(), dir).generic_string()));
    }
    return out;
}

enum class Result { Pass, Fail, Skip };

// Known differences, with their cause (each path is lower case, relative to the project): none since
// GuiModel runs the GUI's sim-control init 0x1408d3d10 (93's SOLMTD 2 4).
// opened.png is the runner's screenshot, not a GUI file.
Result runCase(const char* caseB, const char* moproj, const std::set<std::string>& known)
{
    const fs::path ob = oracleB() / caseB;
    const fs::path dirA = oracleB() / (std::string(caseB) + "_before");
    if (!fs::exists(ob) || !fs::exists(dirA)) {
        std::printf("SKIP %s: no oracle B\n", caseB);
        return Result::Skip;
    }
    const qt::String file{(dirA / moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return Result::Fail;
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    engine::FileEffects open, save;
    Operation* op = nullptr;
    try {
        op = engine::openActiveOperation(p, ctx, open);
        if (!op) return Result::Fail;
        engine::saveCurrentOperation(p, *op, ctx, save);
    } catch (const Error& e) {
        if (e.code != M2M_E_UNSUPPORTED) throw;
        std::printf("SKIP %s: %s\n", caseB, e.what());
        return Result::Skip;
    }
    std::set<std::string> want = guiStep2(ob);
    want.erase("opened.png");
    std::set<std::string> got;
    bool ok = true;
    for (const auto& w : save.writes) {
        const std::string rel = lower(m2m_test::effectRelative(fs::path(w.path.u16()), dirA).generic_string());
        got.insert(rel);
        const fs::path exp = ob / rel;
        const std::string e = fs::exists(exp) ? readAll(exp) : std::string("<missing>");
        if (e != w.bytes) {
            const bool k = known.count(rel) != 0;
            if (k) std::printf("KNOWN ");
            else ok = false;
            if (const char* dump = m2m_test::scratchSetting("M2M_S_DUMP")) {   // port output for inspection
                const fs::path d = fs::path(dump) / caseB / rel;
                fs::create_directories(m2m_test::scratchOutput(d).parent_path());
                std::ofstream(m2m_test::scratchOutput(d), std::ios::binary) << w.bytes;
            }
            std::printf("DIFF %s %s (%zu vs %zu bytes)\n", caseB, rel.c_str(), w.bytes.size(), e.size());
        }
    }
    for (const auto& r : want)
        if (!got.count(r)) {
            ok = false;
            std::printf("MISSING %s %s (the GUI wrote it)\n", caseB, r.c_str());
        }
    for (const auto& r : got)
        if (!want.count(r)) {
            if (known.count(r)) {
                std::printf("KNOWN EXTRA %s %s\n", caseB, r.c_str());
                continue;
            }
            ok = false;
            std::printf("EXTRA %s %s (the GUI did not write it in step 2)\n", caseB, r.c_str());
        }
    // files the GUI removed (in <case>_before, not in <case>) = the deletes of the open (views step)
    std::set<std::string> removed, deleted;
    for (const auto& e : fs::recursive_directory_iterator(dirA)) {
        if (!e.is_regular_file()) continue;
        const fs::path rel = fs::relative(e.path(), dirA);
        if (!fs::exists(ob / rel)) removed.insert(lower(rel.generic_string()));
    }
    for (const auto& d : open.deletes)
        deleted.insert(lower(m2m_test::effectRelative(fs::path(d.u16()), dirA).generic_string()));
    if (removed != deleted) {
        ok = false;
        for (const auto& r : removed)
            if (!deleted.count(r)) std::printf("NOT DELETED %s %s\n", caseB, r.c_str());
        for (const auto& r : deleted)
            if (!removed.count(r)) std::printf("EXTRA DELETE %s %s\n", caseB, r.c_str());
    }
    std::printf("%s %s: %zu files, %zu deleted\n", ok ? "PASS" : "FAIL", caseB, got.size(), deleted.size());
    return ok ? Result::Pass : Result::Fail;
}

}  // namespace

TEST_CASE(save_multiblow3d_93)
{
    CHECK(runCase("93_B_MB3ACT", "Gear_blank.moproj",
                  {}) != Result::Fail);
}

// 93 with Dwell "0.2 sec" / "2.0 N/sec/mm/C" before the open (oracle B probe 93D)
TEST_CASE(save_multiblow3d_93d)
{
    CHECK(runCase("93D_B_MB3ACT_DWELL", "Gear_blank.moproj", {}) != Result::Fail);
}

TEST_CASE(save_multiblow2d_94)
{
    CHECK(runCase("94_B_MB2ACT", "MO2_Operation_Cycles2.moproj",
                  {}) != Result::Fail);
}

int main()
{
    return testkit::run_all();
}
