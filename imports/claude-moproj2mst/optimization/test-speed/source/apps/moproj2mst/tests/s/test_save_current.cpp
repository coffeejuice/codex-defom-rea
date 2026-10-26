#include "../support/test_paths.hpp"
// Packet S: "save current operation" 0x140052ec0 against oracle B. Each case opens an oracle-A
// corpus project, loads its active operation (MDT loader) and runs the save; every KEY file the
// port records must equal, byte for byte, the file DEF_GUI_MO_64 wrote when it re-saved an
// unchanged copy of that project (gui_mo/oracle_b/<case>). The GUI's own step-2 files are the ones
// whose mtime lies in the second before the saved .moproj (Save writes them just before it,
// spec section 1); the port must write exactly that set.
// Then step 3 (engine::saveProject, File > Save 0x1400980d0): the .moproj must equal the GUI's, the
// view-derived parts not ported yet taken from the input and masked.
// Cases whose class is not ported yet (loader slots of L3/L5, or save slots of this packet's
// refusal list) are reported as skipped, not as failures.
// ctest-labels: m2m-s
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <chrono>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "input_views.hpp"

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
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

std::string lower(std::string s)
{
    for (char& c : s)
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
    return s;
}

// Relative paths (lower case, '/') of the files the GUI wrote in Save step 2.
std::set<std::string> guiStep2(const fs::path& dir, fs::file_time_type* moprojTime = nullptr)
{
    fs::path moproj;
    for (const auto& e : fs::directory_iterator(dir))
        if (e.path().extension() == ".moproj") moproj = e.path();
    const auto t0 = fs::last_write_time(moproj);
    if (moprojTime) *moprojTime = t0;
    std::set<std::string> out;
    for (const auto& e : fs::recursive_directory_iterator(dir)) {
        if (!e.is_regular_file()) continue;
        if (e.path().extension() == ".png") continue;   // the runner's screenshot (opened.png)
        const auto t = e.last_write_time();
        if (e.path().filename() == "view_state.json") continue;   // the oracle runner's window state
        if (t < t0 && t0 - t < std::chrono::seconds(1))
            out.insert(lower(fs::relative(e.path(), dir).generic_string()));
    }
    return out;
}

enum class Result { Pass, Fail, Skip };

Result runCase(const char* caseA, const char* moproj, const char* caseB, bool project)
{
    const bool timing = std::getenv("M2M_S_TIMING") != nullptr;
    using Clock = std::chrono::steady_clock;
    const auto caseStart = Clock::now();
    auto phaseStart = caseStart;
    double setupMs = 0, loadOperationMs = 0, saveProjectMs = 0, compareMs = 0;
    const char* corpus = std::getenv("M2M_CORPUS");
    const fs::path ob = oracleB() / caseB;
    if (!corpus || !fs::exists(ob)) {
        std::printf("SKIP %s: no corpus or oracle B\n", caseB);
        return Result::Skip;
    }
    // "B:<folder>" takes the project from the oracle-B tree (a "_before" copy made for the case)
    const std::string a = caseA;
    const fs::path dirA = a.rfind("B:", 0) == 0 ? oracleB() / a.substr(2) : fs::path(corpus) / caseA;
    if (!fs::exists(dirA)) {
        std::printf("SKIP %s: no input %s\n", caseB, dirA.string().c_str());
        return Result::Skip;
    }
    const qt::String file{(dirA / moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    phaseStart = Clock::now();
    if (!openProject(p, file, ctx) || !p.openedOk) {
        // expected open failures: the GUI closed the project too, so it saved nothing (B11_CM:
        // SfOprCopyMirror is not registered, oracle_b/B11_CM/opened.png "Current project has been
        // closed.")
        static const char* const kGuiOpenFails[] = {"B11_CM"};
        for (const char* c : kGuiOpenFails)
            if (std::string(caseB) == c) {
                std::printf("PASS %s: does not open, as in the GUI\n", caseB);
                return Result::Pass;
            }
        std::printf("FAIL %s: the project does not open\n", caseB);
        for (const std::string& w : ctx.warnings) std::printf("  %s\n", w.c_str());
        return Result::Fail;
    }
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    const xml::Node* inSub = nullptr;
    int k = 0;
    for (const auto& c : p.dom->root->children)
        if (c->kind == xml::Node::Element && c->tag == qt::L("Sub_Project") && k++ == p.activeSub) inSub = c.get();
    if (!inSub) return Result::Fail;
    // the GUI's views (moproj::guiViews) refuse the parts L7 has not ported yet; the comparison takes
    // those from the input file and masks them (input_views.hpp)
    testviews::InputViews views(*inSub);
    engine::FileEffects open;
    engine::SaveEffects se;
    setupMs = std::chrono::duration<double, std::milli>(Clock::now() - caseStart).count();
    phaseStart = Clock::now();
    try {
        if (!engine::openActiveOperation(p, ctx, open)) {
            // no active operation (Sub_Project Active="-1", e.g. 127/128): the GUI's Save step 2
            // saves nothing, which holds when it wrote no step 2 file either
            if (guiStep2(ob).empty()) {
                std::printf("PASS %s: no active operation, no step 2 files\n", caseB);
                return Result::Pass;
            }
            std::printf("FAIL %s: no active operation\n", caseB);
            return Result::Fail;
        }
        loadOperationMs = std::chrono::duration<double, std::milli>(Clock::now() - phaseStart).count();
        phaseStart = Clock::now();
        if (!engine::saveProject(p, ctx, views, open, se) || !se.current) return Result::Fail;
        saveProjectMs = std::chrono::duration<double, std::milli>(Clock::now() - phaseStart).count();
    } catch (const Error& e) {
        if (e.code != M2M_E_UNSUPPORTED) throw;
        std::printf("SKIP %s: %s\n", caseB, e.what());
        return Result::Skip;
    }
    const engine::FileEffects& save = se.step2;
    phaseStart = Clock::now();
    fs::file_time_type t0{};
    std::set<std::string> want = guiStep2(ob, &t0);
    std::set<std::string> got;
    // A step-2 file the GUI writes again later in the save (props vt+0xb8 in step 3, mtime at or
    // after the .moproj's) keeps only its last version, as in test_save_l6: with the same bytes it
    // counts as a step-2 write; with other bytes only the port's step-3 write of it is compared.
    std::map<std::string, std::string> later;
    bool ok = true;
    auto dumpDiff = [&](const std::string& rel, const std::string& bytes, const std::string& exp, const char* step) {
        ok = false;
        if (const char* dump = m2m_test::scratchSetting("M2M_S_DUMP")) {   // port output for inspection
            const fs::path d = fs::path(dump) / caseB / rel;
            fs::create_directories(m2m_test::scratchOutput(d).parent_path());
            std::ofstream(m2m_test::scratchOutput(d), std::ios::binary) << bytes;
        }
        std::printf("DIFF %s%s %s (%zu vs %zu bytes)\n", caseB, step, rel.c_str(), bytes.size(), exp.size());
    };
    for (const auto& w : save.writes) {
        const std::string rel = lower(m2m_test::effectRelative(fs::path(w.path.u16()), dirA).generic_string());
        got.insert(rel);
        const fs::path exp = ob / rel;
        const std::string e = fs::exists(exp) ? readAll(exp) : std::string("<missing>");
        if (!want.count(rel) && fs::exists(exp) && fs::last_write_time(exp) >= t0) {
            if (e != w.bytes) later[rel] = e;
            else want.insert(rel);
            continue;
        }
        if (e != w.bytes) dumpDiff(rel, w.bytes, e, "");
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
    if (!later.empty()) {
        engine::FileEffects step3;
        bool ran = false;
        try {
            ran = engine::l6PropsSave(p, *se.current, step3, ctx);
        } catch (const Error& e) {
            if (e.code != M2M_E_UNSUPPORTED) throw;
            std::printf("SKIP %s step 3: %s\n", caseB, e.what());
            return Result::Skip;
        }
        std::set<std::string> got3;
        if (ran)
            for (const auto& w : step3.writes) {
                const std::string rel = lower(m2m_test::effectRelative(fs::path(w.path.u16()), dirA).generic_string());
                const auto it = later.find(rel);
                if (it == later.end()) continue;
                got3.insert(rel);
                if (it->second != w.bytes) dumpDiff(rel, w.bytes, it->second, " step 3");
            }
        for (const auto& [r, bytes] : later)
            if (!got3.count(r)) {
                ok = false;
                std::printf("UNCHECKED %s %s (written in step 2, the GUI rewrote it later, step 3 does not)\n",
                            caseB, r.c_str());
            }
    }
    // step 3: the .moproj, byte for byte outside the view parts guiViews() does not produce yet
    if (!project) {
        std::printf("   %s: .moproj not compared (no input before the GUI's edit)\n", caseB);
    } else if (se.project.writes.size() != 1) {
        ok = false;
        std::printf("FAIL %s: %zu .moproj writes\n", caseB, se.project.writes.size());
    } else {
        const std::string pg = se.project.writes[0].bytes, pw = readAll(ob / moproj);
        const bool same = pg == pw || testviews::masked(pg, views.rectFromInput) ==
                                          testviews::masked(pw, views.rectFromInput);
        if (!same) {
            ok = false;
            std::printf("DIFF %s %s (%zu vs %zu bytes)\n", caseB, moproj, pg.size(), pw.size());
            if (const char* dump = m2m_test::scratchSetting("M2M_S_DUMP"))
                std::ofstream(m2m_test::scratchOutput(fs::path(dump) / (std::string(caseB) + ".moproj")), std::ios::binary) << pg;
        }
        try {   // the first view part guiViews() still refuses (L7)
            double x, y;
            moproj::guiViews(p)->viewCentre(*activeSubProject(p), x, y);
        } catch (const Error& e) {
            std::printf("   guiViews: %s\n", e.what());
        }
    }
    std::printf("%s %s: %zu files%s\n", ok ? "PASS" : "FAIL", caseB, got.size(), project ? " + .moproj" : "");
    compareMs = std::chrono::duration<double, std::milli>(Clock::now() - phaseStart).count();
    if (timing) {
        const double totalMs = std::chrono::duration<double, std::milli>(Clock::now() - caseStart).count();
        std::fprintf(stderr, "TIMING save_current case=%s setup=%.3fms open_operation=%.3fms save_project=%.3fms compare=%.3fms total=%.3fms\n",
                     caseB, setupMs, loadOperationMs, saveProjectMs, compareMs, totalMs);
    }
    return ok ? Result::Pass : Result::Fail;
}

}  // namespace

TEST_CASE(save_current_oracle_b)
{
    // open = a target still being worked on: its result is printed, not asserted
    struct C { const char* a; const char* moproj; const char* b; bool open = false; bool project = true; };
    // project = false: the oracle-B save follows an edit in the GUI (an operation appended) or a
    // reused GUI process, with no input before it, so only step 2 is compared
    const C cases[] = {
        {"11_THEXP", "THEXP.moproj", "R11"},                                  // HeatTransfer
        {"19_XY_H01", "XY_H01.moproj", "R19"},                                // HeatTransfer
        {"46_MO2_Boolean_Oprations", "MO2_Boolean_Oprations.moproj", "R46", false, false},  // Boolean2D (L5)
        {"47_MO2_Extrusion_and_Boolean_Cycles", "MO2_Extrusion_and_Boolean_Cycles.moproj",
         "83_R47_MO2_Extrusion_and_Boolean_Cycles", false, false},            // Boolean2D (L5)
        // the fresh-process re-saves of 46 and 47 (inputs <case>_before)
        {"B:R46F_before", "MO2_Boolean_Oprations.moproj", "R46F"},
        {"B:R47F_before", "MO2_Extrusion_and_Boolean_Cycles.moproj", "R47F"},
        {"52_BEND_2D", "BEND_2D.moproj", "82_R52_BEND_2D"},                   // HTExpress2D (L3)
        // 59 is 57 with a HeatTreat appended after the active Forming: the same step-2 files
        {"57_Gear_blank", "Gear_blank.moproj", "59_B_HT3", false, false},                 // Forming (L3)
        // 60 and 62 are 51 with a HeatTreat2D / DiestressBM2D appended after the active Cycle; the
        // cycle's own vt+0xe8 is empty, so the GUI wrote no file in step 2 (MDT L7)
        {"51_MO2_Operation_Cycles2", "MO2_Operation_Cycles2.moproj", "60_B_HT2", false, false},   // Cycle (L7)
        {"51_MO2_Operation_Cycles2", "MO2_Operation_Cycles2.moproj", "62_B_BM2", false, false},   // Cycle (L7)
        // oracle-B inputs (<case>_before, an absolute path replaces the corpus folder): 57 with a
        // FormingExpress op appended and active (MDT L3S)
        {"../../oracle_b/95_B_FE3ACT_before", "Gear_blank.moproj", "95_B_FE3ACT"},   // FormingExpress (L3)
        // corpus 51 with a Convert2Dto3D operation appended and active (L5S)
        // the up axis comes from the Configuration editor SfConv23Config (vt+0xd0 -> vt+0x1d0)
        {"B:97_B_CNVACT_before", "MO2_Operation_Cycles2.moproj", "97_B_CNVACT"},
        // 57 with a MultiBlow appended and active, Dwell edited: its MST needs step 2's widget
        // write-back (updateDataInWidgetArea)
        {"B:93D_B_MB3ACT_DWELL_before", "Gear_blank.moproj", "93D_B_MB3ACT_DWELL"},
        // L4 widget-area sweep probes (oracle_b/_tools/mkl4w.py): the shown editor's vt+0x1c0 at Save
        {"B:105_B_COG7_before", "Cogging_Multiple_Ops_Lab2.moproj", "105_B_COG7"},   // Cogging
        // corpus 54, Process_Type 1, Swaging_Stroke = Cycle_Time = 1.23456789: the GUI saved
        // 1.2345700E+000, the Process editor's 'g' 6 round trip (src/engine/cogging_process.cpp)
        {"B:115_B_COG7P1_before", "Cogging_Multiple_Ops_Lab2.moproj", "115_B_COG7P1"},   // Cogging
        // corpus 54 with a new Cogging op 4, active and never set up (0x1407507b0 new-operation path; open:
        // the display's scene box of geometry-only objects, packet V, refuses for now)
        {"B:131_B_COG_NEWOP_before", "Cogging_Multiple_Ops_Lab2.moproj", "131_B_COG_NEWOP", true},   // Cogging
        {"B:106_B_F2ACT_before", "MO2_Operation_Cycles2.moproj", "106_B_F2ACT"},   // Forming2D
        {"B:108_B_SOPTA_before", "Gear_blank.moproj", "108_B_SOPTA"},             // ShapeOptim
        {"B:109_B_FE7_before", "Gear_blank.moproj", "109_B_FE7"},                 // FormingExpress
    };
    int pass = 0, skip = 0;
    for (const C& c : cases) {
        const Result r = runCase(c.a, c.moproj, c.b, c.project);
        if (!c.open) CHECK(r != Result::Fail);
        if (r == Result::Pass) ++pass;
        if (r == Result::Skip) ++skip;
    }
    std::printf("%d pass, %d skipped\n", pass, skip);
}

// Which classes still refuse in step 2 on the corpus (one open + save per project, its active
// operation). Printed, not asserted: convert moves to engine::saveProject when the list is empty.
TEST_CASE(save_current_corpus_refusals)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    if (!corpus) return;
    std::map<std::string, std::vector<std::string>> refused;
    int ok = 0, none = 0;
    for (const auto& dir : fs::directory_iterator(corpus)) {
        if (!dir.is_directory()) continue;
        for (const auto& f : fs::directory_iterator(dir.path())) {
            if (f.path().extension() != ".moproj") continue;
            const qt::String file{f.path().u16string()};
            const qt::String db = app::changeSuffix(file, qt::L(".DB"));
            Project p;
            ProjectScope scope(p);
            Ctx ctx;
            initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
            makeRegistry(p);
            const std::string name = dir.path().filename().string();
            try {
                if (!openProject(p, file, ctx) || !p.openedOk) continue;
                activateSubProject(p, 0, ctx);
                engine::setEngineVersion(u"");
                engine::FileEffects open, save;
                Operation* op = engine::openActiveOperation(p, ctx, open);
                if (!op) {
                    ++none;
                    continue;
                }
                try {
                    engine::saveCurrentOperation(p, *op, ctx, save);
                    ++ok;
                } catch (const Error& e) {
                    if (e.code != M2M_E_UNSUPPORTED) throw;
                    refused[op->classKey.utf8() + ": " + e.what()].push_back(name);
                }
            } catch (const Error& e) {
                refused[std::string("(open) ") + e.what()].push_back(name);
            }
        }
    }
    std::printf("step 2 on the corpus: %d saved, %d without an active operation\n", ok, none);
    for (const auto& [why, cases] : refused) {
        std::printf("REFUSED %zu x %s\n  ", cases.size(), why.c_str());
        for (const auto& c : cases) std::printf(" %s", c.c_str());
        std::printf("\n");
    }
}

// A sweep over a list of cases (M2M_S_CASES = a TSV of input folder, .moproj name, oracle-B case and
// an optional fourth column "0" when the .moproj is not compared (a reused GUI process, or an input
// rebuilt rather than kept), one case per line), for the whole oracle-B set; reports only, never
// fails. Without the variable it does nothing.
TEST_CASE(save_current_listed)
{
    const char* list = std::getenv("M2M_S_CASES");
    if (!list) return;
    std::ifstream in(list);
    std::string line;
    int pass = 0, fail = 0, skip = 0;
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        const std::size_t t1 = line.find('\t'), t2 = line.find('\t', t1 + 1);
        if (t1 == std::string::npos || t2 == std::string::npos) continue;
        const std::size_t t3 = line.find('\t', t2 + 1);
        const std::string a = line.substr(0, t1), m = line.substr(t1 + 1, t2 - t1 - 1),
                          b = line.substr(t2 + 1, t3 == std::string::npos ? std::string::npos : t3 - t2 - 1);
        const bool project = t3 == std::string::npos || line.substr(t3 + 1) != "0";
        // GUI-interaction cases (a click between Open and Save): not a stage 1 target
        static const char* const kInteractive[] = {"114_B_CNV_CLICK"};
        bool interactive = false;
        for (const char* c : kInteractive) interactive = interactive || b == c;
        if (interactive) {
            std::printf("SKIP %s: GUI interaction between Open and Save (not a stage 1 case)\n", b.c_str());
            ++skip;
            continue;
        }
        Result r = Result::Fail;
        try {
            r = runCase(a.c_str(), m.c_str(), b.c_str(), project);   // an absolute input folder replaces the corpus
        } catch (const std::exception& e) {
            std::printf("FAIL %s: %s\n", b.c_str(), e.what());
        }
        (r == Result::Pass ? pass : r == Result::Skip ? skip : fail)++;
    }
    std::printf("listed: %d pass, %d fail, %d skipped\n", pass, fail, skip);
}

int main()
{
    return testkit::run_all();
}
