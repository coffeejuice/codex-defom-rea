#include "../support/test_paths.hpp"
// MDT L6S/L6T: save current operation 0x140052ec0 for the L6 classes against oracle B. The inputs
// are the cases' _before folders (made by oracle_b/_tools/mkl6.py: the L6 operation first and
// active, its properties empty). Every file the port's save records must equal the file the GUI
// wrote in Save step 2 (mtime in the second before the saved .moproj), and the set of files must be
// the same (as test_save_l2.cpp).
// ctest-labels: m2m-mdt mdt-l6
#include <process.h>

#include <testkit/check.hpp>

#include <chrono>
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

std::string lower(std::string s)
{
    for (char& c : s)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    return s;
}

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

[[maybe_unused]] void replaceFirst(std::string& s, std::size_t from, std::size_t to, const std::string& open, const std::string& close,
                  const std::string& value)
{
    const std::size_t a = s.find(open, from);
    if (a == std::string::npos || a >= to) return;
    const std::size_t b = s.find(close, a);
    s.replace(a + open.size(), b - a - open.size(), value);
}

fs::path guiMoproj(const fs::path& dir)
{
    for (const auto& e : fs::directory_iterator(dir))
        if (e.path().extension() == ".moproj") return e.path();
    return {};
}

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
        const std::string rel = lower(fs::relative(e.path(), dir).generic_string());
        if (rel == "opened.png") continue;                 // the runner's screenshot
        if (rel == "view_state.json") continue;            // the runner's window state
        if (t < t0 && t0 - t < std::chrono::seconds(1)) out.insert(rel);
    }
    return out;
}

// edit: what the user changed in the shown editor before the Save (oracle B clicks), or null
bool runCase(const char* caseB, const char* typ, void (*edit)(mst::OpClass&, Operation&) = nullptr)
{
    const fs::path ob = oracleB() / caseB;
    const fs::path before = oracleB() / (std::string(caseB) + "_before");
    if (!fs::exists(ob) || !fs::exists(before)) {
        std::printf("SKIP %s: no oracle B\n", caseB);
        return true;
    }
    const fs::path dirA = m2m_test::scratchRoot() / ("m2m_l6s_" + std::to_string(_getpid())) / caseB;
    std::error_code ec;
    fs::remove_all(dirA, ec);
    fs::create_directories(dirA);
    fs::copy(before, dirA, fs::copy_options::recursive, ec);
    if (ec) return false;
    fs::path mp;
    for (const auto& e : fs::directory_iterator(dirA))
        if (lower(e.path().extension().string()) == ".moproj") mp = e.path();
    const qt::String file{mp.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return false;
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    engine::FileEffects open, save;
    Operation* op = engine::openActiveOperation(p, ctx, open);
    if (!op || op->classKey.utf8() != typ) {
        std::printf("FAIL %s: active operation %s\n", caseB, op ? op->classKey.utf8().c_str() : "none");
        return false;
    }
    if (edit) {
        mst::OpClass* c = opBase(*op);
        if (!c) return false;
        edit(*c, *op);
    }
    try {
        engine::saveCurrentOperation(p, *op, ctx, save);
    } catch (const Error& e) {
        std::printf("FAIL %s: %s\n", caseB, e.what());
        return false;
    }
    std::set<std::string> want = guiStep2(ob);
    std::set<std::string> got;
    // files the GUI wrote again in step 3 (mtime at or after the .moproj's) keep only their last
    // version: the port's step-2 bytes cannot be compared, its step-3 write of them must be
    std::set<std::string> later;
    bool ok = true;
    for (const auto& w : save.writes) {
        const std::string rel = lower(m2m_test::effectRelative(fs::path(w.path.u16()), dirA).generic_string());
        got.insert(rel);
        const fs::path exp = ob / rel;
        const std::string e = fs::exists(exp) ? readAll(exp) : std::string("<missing>");
        if (!want.count(rel) && fs::exists(exp) && fs::last_write_time(exp) >= fs::last_write_time(guiMoproj(ob))) {
            if (e != w.bytes) later.insert(rel);
            else want.insert(rel);                           // same bytes: written in step 2 too
            continue;
        }
        if (e != w.bytes) {
            ok = false;
            if (const char* dump = m2m_test::scratchSetting("M2M_S_DUMP")) {
                const fs::path dd = fs::path(dump) / caseB / rel;
                fs::create_directories(m2m_test::scratchOutput(dd).parent_path());
                std::ofstream(m2m_test::scratchOutput(dd), std::ios::binary) << w.bytes;
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
        if (!want.count(r) && !later.count(r)) {
            ok = false;
            std::printf("EXTRA %s %s (the GUI did not write it in step 2)\n", caseB, r.c_str());
        }
    std::printf("%s %s: %zu files\n", ok ? "PASS" : "FAIL", caseB, got.size());
    // step 3: props vt+0xb8 after the .moproj (mtime at or after it); only the L6 data files
    engine::FileEffects step3;
    try {
        engine::l6PropsSave(p, *op, step3, ctx);   // false: no L6 props, nothing to write
    } catch (const Error& e) {
        std::printf("FAIL %s step 3: %s\n", caseB, e.what());
        return false;
    }
    std::set<std::string> want3;
    const auto t0 = fs::last_write_time(guiMoproj(ob));
    for (const auto& e : fs::recursive_directory_iterator(ob)) {
        if (!e.is_regular_file() || e.last_write_time() < t0) continue;
        const std::string rel = lower(fs::relative(e.path(), ob).generic_string());
        const std::string name = lower(e.path().filename().string());
        if (name.find(".wzd") != std::string::npos || name.find(".mdwz") != std::string::npos ||
            name.find(".htwz") != std::string::npos || name.find("def_machdist_ctrl2.ini_") == 0)
            want3.insert(rel);
    }
    std::set<std::string> got3;
    bool ok3 = true;
    for (const auto& w : step3.writes) {
        const std::string rel = lower(m2m_test::effectRelative(fs::path(w.path.u16()), dirA).generic_string());
        got3.insert(rel);
        const fs::path exp = ob / rel;
        const std::string e = fs::exists(exp) ? readAll(exp) : std::string("<missing>");
        if (!want3.count(rel)) {
            ok3 = false;
            std::printf("EXTRA %s step 3 %s\n", caseB, rel.c_str());
        } else if (e != w.bytes) {
            ok3 = false;
            if (const char* dump = m2m_test::scratchSetting("M2M_S_DUMP")) {
                const fs::path dd = fs::path(dump) / caseB / rel;
                fs::create_directories(m2m_test::scratchOutput(dd).parent_path());
                std::ofstream(m2m_test::scratchOutput(dd), std::ios::binary) << w.bytes;
            }
            std::printf("DIFF %s step 3 %s (%zu vs %zu bytes)\n", caseB, rel.c_str(), w.bytes.size(), e.size());
        }
    }
    for (const auto& r : later)
        if (!got3.count(r)) {
            ok3 = false;
            std::printf("UNCHECKED %s %s (written in step 2, the GUI rewrote it later, step 3 does not)\n",
                        caseB, r.c_str());
        }
    for (const auto& r : want3)
        if (!got3.count(r)) {
            ok3 = false;
            std::printf("MISSING %s step 3 %s\n", caseB, r.c_str());
        }
    std::printf("%s %s step 3: %zu files\n", ok3 ? "PASS" : "FAIL", caseB, got3.size());
    return ok && ok3;
}

}  // namespace

TEST_CASE(save_l6_oracle_b)
{
    CHECK(runCase("98_B_DS3F", "SfOprDiestress"));
    CHECK(runCase("99_B_BM2F", "SfOprDiestressBM2D"));
    CHECK(runCase("100_B_BM3F", "SfOprDiestressBM3D"));
    CHECK(runCase("101_B_DIS3F", "SfOprDistortion3D"));
    CHECK(runCase("102_B_DIS2F", "SfOprDistortion2D"));
    CHECK(runCase("103_B_HT3F", "SfOprHeatTreat"));
    CHECK(runCase("104_B_HT2F", "SfOprHeatTreat2D"));
}

// SfOprHTFurnace's Load Pattern editor (MDT L6T): 87 and 107 save it unmodified; 119 is 107's input
// with the first spin box (sbRows, "Workpieces in a fixture") set from 1 to 2 before the Save
TEST_CASE(save_htf_load_pattern)
{
    CHECK(runCase("87_B_HTFF", "SfOprHTFurnace"));
    CHECK(runCase("107_B_HTFF", "SfOprHTFurnace"));
    CHECK(runCase("119_B_HTF_LPEDIT", "SfOprHTFurnace", [](mst::OpClass& c, Operation& op) {
        engine::LoadPatternForm* f = c.loadPatternEditor(op);
        if (!f) throw Error(M2M_E_UNSUPPORTED, "no Load Pattern editor shown");
        f->count[0] = 2;
        f->modified = true;
    }));
}

int main()
{
    return testkit::run_all();
}
