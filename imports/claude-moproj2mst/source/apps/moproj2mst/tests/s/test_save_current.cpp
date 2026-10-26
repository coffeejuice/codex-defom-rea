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

#include <algorithm>
#include <array>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <vector>
#include <set>
#include <string>
#include <utility>

#include <defmodel/llobj/object_list.hpp>
#include <defmodel/window/inter_object_ops.hpp>
#include <defmodel/material/material_ctor.hpp>
#include <defmodel/mesh2d/material_ops.hpp>

#include "../../src/engine/engine_internal.hpp"
#include "../../src/ops/express.hpp"
#include "m2m/data/common.hpp"
#include "m2m/props.hpp"

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "input_views.hpp"

namespace m2m::engine {
std::vector<ContactPair> baseDefaultPairs(Project&, Operation&);
bool saveInterObject(Project&, Operation&, FileEffects&);
void saveBcc(Project&, Operation&, FileEffects&);
}

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
std::set<std::string> guiStep2(const fs::path& dir)
{
    fs::path moproj;
    for (const auto& e : fs::directory_iterator(dir))
        if (e.path().extension() == ".moproj") moproj = e.path();
    const auto t0 = fs::last_write_time(moproj);
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
        if (!engine::saveProject(p, ctx, views, open, se) || !se.current) return Result::Fail;
    } catch (const Error& e) {
        if (e.code != M2M_E_UNSUPPORTED) throw;
        std::printf("SKIP %s: %s\n", caseB, e.what());
        return Result::Skip;
    }
    const engine::FileEffects& save = se.step2;
    std::set<std::string> want = guiStep2(ob);
    fs::file_time_type t0{};
    for (const auto& e : fs::directory_iterator(ob))
        if (e.path().extension() == ".moproj") t0 = fs::last_write_time(e.path());
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

// Synthetic internal-helper mechanics on a real loaded operation. These inputs and calls
// establish neither public Save admission/dispatch nor native equivalence.
TEST_CASE(base_default_pairs_step_scope)
{
    const fs::path filePath = m2m_test::configuredPath("M2M_CORPUS") /
        "46_MO2_Boolean_Oprations" / "MO2_Boolean_Oprations.moproj";
    REQUIRE(fs::is_regular_file(filePath));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    const qt::String file{filePath.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx));
    REQUIRE(p.openedOk);
    activateSubProject(p, 0, ctx);
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    REQUIRE(op->classKey.utf8() == "SfOprHeatTransferExpress2D");
    REQUIRE(op->props != nullptr && op->props->common != nullptr);
    mst::installLoaderSlots();
    mst::OpClass* ob = opBase(*op);
    REQUIRE(ob != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();
    ob->current = op;
    engine::FileEffects fx;
    ob->loadOperation(p, *op, ctx, fx);
    REQUIRE(m->step != nullptr);
    REQUIRE(m->objectCount() == 2);
    REQUIRE(*engine::at<std::int32_t>(m->step.get(), 0x1c) == 2);
    auto* first = m->objectAt(1);
    auto* second = m->objectAt(2);
    REQUIRE(first != nullptr && second != nullptr);
    engine::DB_STEP_DATA* originalStep = m->step.get();

    // Private synthetic fields: two defined objects, exactly one candidate (ID 1).
    // The project is discarded at case exit and these changes are never saved.
    *engine::at<std::int32_t>(first, 8) = 1;
    *engine::at<std::int32_t>(first, 0xc) = 1;
    *engine::at<std::int32_t>(second, 8) = 2;
    *engine::at<std::int32_t>(second, 0xc) = 0;
    op->props->common->friction = 0.375;
    op->props->common->ihtc = qt::L("11.0 N/sec/mm/C");
    const double expectedIhtc = propsIhtc(*op->props);
    REQUIRE(expectedIhtc > 0.0);
    REQUIRE(propsFriction(*op->props) == 0.375);
    REQUIRE(def::model::pair_append(&m->step->inter_obj) != nullptr);
    REQUIRE(m->step->inter_obj.count > 0);
    const auto pairs = engine::baseDefaultPairs(p, *op);
    REQUIRE(pairs.size() == 2);
    for (int i = 0; i < 2; ++i) {
        CHECK_EQ(pairs[i].master, i + 1);
        CHECK_EQ(pairs[i].slave, 1);
        CHECK_EQ(pairs[i].lub, 0);
        CHECK_EQ(pairs[i].f19, 1);
        CHECK_EQ(pairs[i].frc, 0.375);
        CHECK_EQ(pairs[i].ihtc, expectedIhtc);
    }
    CHECK_EQ(m->step->inter_obj.count, 0);
    CHECK(engine::opModel(p, *op) == m);
    CHECK(m->step.get() == originalStep);
    CHECK(m->objectAt(1) == first && m->objectAt(2) == second);

    struct DetachedStep {
        std::unique_ptr<engine::DB_STEP_DATA>& owner;
        std::unique_ptr<engine::DB_STEP_DATA> saved;
        explicit DetachedStep(engine::GuiModel& model)
            : owner(model.step), saved(std::move(model.step)) {}
        DetachedStep(const DetachedStep&) = delete;
        DetachedStep& operator=(const DetachedStep&) = delete;
        ~DetachedStep() noexcept { owner = std::move(saved); }
    };
    REQUIRE(def::model::pair_append(&m->step->inter_obj) != nullptr);
    const auto savedCount = m->step->inter_obj.count;
    REQUIRE(savedCount > 0);
    {
        DetachedStep guard(*m);
        REQUIRE(m->step == nullptr);
        REQUIRE(guard.saved.get() == originalStep);
        CHECK(engine::baseDefaultPairs(p, *op).empty());
        CHECK_EQ(guard.saved->inter_obj.count, savedCount);
    }
    CHECK(engine::opModel(p, *op) == m);
    REQUIRE(m->step.get() == originalStep);
    CHECK(m->objectAt(1) == first && m->objectAt(2) == second);
    CHECK_EQ(m->step->inter_obj.count, savedCount);

    // Exercise exceptional scope exit without manufacturing a failing assertion.
    struct DetachProbe {};
    try {
        DetachedStep guard(*m);
        throw DetachProbe{};
    } catch (const DetachProbe&) {
    }
    CHECK(engine::opModel(p, *op) == m);
    REQUIRE(m->step.get() == originalStep);
    CHECK(m->objectAt(1) == first && m->objectAt(2) == second);
    CHECK_EQ(m->step->inter_obj.count, savedCount);
    CHECK(fx.writes.empty() && fx.deletes.empty());
}

// Owner command writes exactly the bounded regions and two integers, retaining all other bytes.
TEST_CASE(store_operation_simulation_names_owner)
{
    engine::GuiModel model(nullptr, 3);
    REQUIRE(model.step);
    auto* const step = model.step.get();
    auto& sc = step->sim_ctrl;
    for (const auto lengths : {std::pair<std::size_t, std::size_t>{7, 9},
                              {0x40, 0x50}, {0x49, 0x5b}}) {
        std::fill(std::begin(sc.oprnam), std::end(sc.oprnam), 'O');
        std::fill(std::begin(sc.simnam), std::end(sc.simnam), 'S');
        sc.cursim[0] = -501; sc.cursim[1] = -502;
        const qt::String operationName{std::u16string(lengths.first, u'A')};
        const qt::String simulationName{std::u16string(lengths.second, u'B')};
        std::vector<unsigned char> expected(sizeof *step);
        std::memcpy(expected.data(), step, expected.size());
        const auto expectedRegion = [&](const void* destination, std::size_t bound,
                                        std::size_t length, unsigned char value) {
            const auto offset = static_cast<const unsigned char*>(destination) -
                reinterpret_cast<const unsigned char*>(step);
            for (std::size_t i = 0; i < bound; ++i)
                expected[offset + i] = i < length ? value : 0;
        };
        expectedRegion(sc.oprnam, 0x40, lengths.first, 'A');
        expectedRegion(sc.simnam, 0x50, lengths.second, 'B');
        const int flow = 31, simulation = 47;
        const auto expectedInteger = [&](const void* destination, const int& value) {
            const auto offset = static_cast<const unsigned char*>(destination) -
                reinterpret_cast<const unsigned char*>(step);
            std::memcpy(expected.data() + offset, &value, sizeof value);
        };
        expectedInteger(&sc.cursim[1], flow);
        expectedInteger(&sc.cursim[0], simulation);
        model.storeOperationSimulationNames(operationName, flow, simulationName, simulation);
        CHECK(model.step.get() == step);
        CHECK(std::memcmp(expected.data(), step, expected.size()) == 0);
        CHECK_EQ(sc.oprnam[0x40], 'O'); CHECK_EQ(sc.simnam[0x50], 'S');
    }
    auto detached = std::move(model.step);
    std::vector<unsigned char> before(sizeof *step);
    std::memcpy(before.data(), step, before.size());
    model.storeOperationSimulationNames(qt::L("absent-op"), 99, qt::L("absent-sim"), 101);
    CHECK(model.step == nullptr);
    CHECK(std::memcmp(before.data(), detached.get(), before.size()) == 0);
    model.step = std::move(detached);
}

// Direct storeSimNames mechanics only; no public Save dispatch or native byte claim.
TEST_CASE(store_sim_names_step_scope)
{
    const fs::path filePath = m2m_test::configuredPath("M2M_CORPUS") /
        "46_MO2_Boolean_Oprations" / "MO2_Boolean_Oprations.moproj";
    REQUIRE(fs::is_regular_file(filePath));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    const qt::String file{filePath.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx));
    REQUIRE(p.openedOk);
    activateSubProject(p, 0, ctx);
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    REQUIRE(op->classKey.utf8() == "SfOprHeatTransferExpress2D");
    mst::installLoaderSlots();
    mst::OpClass* ob = opBase(*op);
    REQUIRE(ob != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();
    ob->current = op;
    engine::FileEffects load;
    ob->loadOperation(p, *op, ctx, load);
    REQUIRE(m->step != nullptr);
    auto* const originalStep = m->step.get();
    auto& sc = originalStep->sim_ctrl;
    SubProject* sub = activeSubProject(p);
    REQUIRE(sub != nullptr);

    const auto expectedEntry = [&]() -> mst::SimEntry {
        const int no = flowNumber(*op);
        const auto entries = mst::simEntries(p, *sub);
        const mst::SimEntry* first = nullptr;
        for (const auto& entry : entries)
            if (entry.pos == no) { first = &entry; break; }
        REQUIRE(first != nullptr);
        REQUIRE(first->named);
        return *first;
    };
    const auto seed = [&]() {
        for (char& c : sc.oprnam) c = 'O';
        for (char& c : sc.simnam) c = 'S';
        sc.cursim[0] = -701;
        sc.cursim[1] = -702;
    };
    const auto checkStored = [&]() {
        // Independently admit the first named entry before calling the helper.
        const auto entry = expectedEntry();
        const int expectedFlow = flowNumber(*op);
        const std::string expectedName = engine::local8(op->name);
        const std::string expectedSimName = engine::local8(entry.name);
        REQUIRE(expectedName.size() < 0x40);
        REQUIRE(expectedSimName.size() < 0x50);
        std::string expectedOprnam(0x40, '\0');
        expectedOprnam.replace(0, expectedName.size(), expectedName);
        std::string expectedSimnam(0x50, '\0');
        expectedSimnam.replace(0, expectedSimName.size(), expectedSimName);
        seed();
        engine::storeSimNames(p, *op);
        CHECK_EQ(std::string(sc.oprnam, 0x40), expectedOprnam);
        CHECK_EQ(sc.oprnam[0x40], 'O'); // strncpy does not append a terminator here.
        CHECK_EQ(sc.cursim[1], expectedFlow);
        CHECK_EQ(std::string(sc.simnam, 0x50), expectedSimnam);
        CHECK_EQ(sc.simnam[0x50], 'S');
        CHECK_EQ(sc.cursim[0], entry.sim);
        CHECK(engine::opModel(p, *op) == m);
        CHECK(m->step.get() == originalStep);
    };
    op->name = qt::L("NamesProbeA");
    checkStored();
    op->name = qt::L("NamesProbeB");
    checkStored();

    // Null SimControl is admitted only through this direct helper, with a
    // matching named entry already established and the detached owner retained.
    (void)expectedEntry();
    seed();
    const std::string savedOprnam(sc.oprnam, sizeof(sc.oprnam));
    const std::string savedSimnam(sc.simnam, sizeof(sc.simnam));
    struct DetachedStep {
        std::unique_ptr<engine::DB_STEP_DATA>& owner;
        std::unique_ptr<engine::DB_STEP_DATA> saved;
        explicit DetachedStep(engine::GuiModel& model)
            : owner(model.step), saved(std::move(model.step)) {}
        DetachedStep(const DetachedStep&) = delete;
        DetachedStep& operator=(const DetachedStep&) = delete;
        ~DetachedStep() noexcept { owner = std::move(saved); }
    };
    {
        DetachedStep guard(*m);
        REQUIRE(m->step == nullptr);
        REQUIRE(guard.saved.get() == originalStep);
        engine::storeSimNames(p, *op); // An exception fails this direct helper test.
        CHECK(engine::opModel(p, *op) == m);
        CHECK(m->step == nullptr);
        CHECK_EQ(std::string(sc.oprnam, sizeof(sc.oprnam)), savedOprnam);
        CHECK_EQ(std::string(sc.simnam, sizeof(sc.simnam)), savedSimnam);
        CHECK_EQ(sc.cursim[0], -701);
        CHECK_EQ(sc.cursim[1], -702);
    }
    CHECK(engine::opModel(p, *op) == m);
    REQUIRE(m->step.get() == originalStep);
    checkStored();
}

// Direct helper mechanics only: a real loaded operation plus a valid-shaped pair
// when needed. This does not establish public Save dispatch or native byte equality.
TEST_CASE(save_interobject_step_scope)
{
    const fs::path filePath = m2m_test::configuredPath("M2M_CORPUS") /
        "46_MO2_Boolean_Oprations" / "MO2_Boolean_Oprations.moproj";
    REQUIRE(fs::is_regular_file(filePath));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    const qt::String file{filePath.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx));
    REQUIRE(p.openedOk);
    activateSubProject(p, 0, ctx);
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    REQUIRE(op->classKey.utf8() == "SfOprHeatTransferExpress2D");
    mst::installLoaderSlots();
    mst::OpClass* ob = opBase(*op);
    REQUIRE(ob != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();
    ob->current = op;
    engine::FileEffects load;
    ob->loadOperation(p, *op, ctx, load);
    REQUIRE(m->step != nullptr);
    REQUIRE(m->objectCount() == 2);
    auto* const originalStep = m->step.get();
    auto& list = originalStep->inter_obj;
    op->lubSystem = false; // retain the seeded/list input for this writer probe
    op->icb.enable = 1;
    if (list.count == 0) {
        auto* master = m->objectAt(1);
        auto* slave = m->objectAt(2);
        REQUIRE(master != nullptr && slave != nullptr);
        const auto masterId = *engine::at<std::int32_t>(master, 8);
        const auto slaveId = *engine::at<std::int32_t>(slave, 8);
        REQUIRE(masterId > 0 && masterId <= static_cast<int>(op->icb.flag.size()));
        REQUIRE(slaveId > 0 && slaveId <= static_cast<int>(op->icb.flag.size()));
        REQUIRE(masterId != slaveId);
        // Constructor-initialized records, actual loaded object IDs, kind 2 and
        // its reversed kind-1 mirror. Synthetic changes die with this project.
        auto* pair = def::model::pair_append(&list);
        REQUIRE(pair != nullptr);
        *engine::at<std::int32_t>(pair, 0x168) = masterId;
        *engine::at<std::int32_t>(pair, 0x16c) = slaveId;
        *engine::at<std::int32_t>(pair, 0x170) = 2;
        auto* mirror = def::model::pair_append(&list);
        REQUIRE(mirror != nullptr);
        def::model::pair_copy(mirror, pair);
        *engine::at<std::int32_t>(mirror, 0x168) = slaveId;
        *engine::at<std::int32_t>(mirror, 0x16c) = masterId;
        *engine::at<std::int32_t>(mirror, 0x170) = 1;
    }
    REQUIRE(list.count > 0);
    std::array<std::uint8_t, 100> expectedFlags{};
    bool hasContact = false;
    for (std::int32_t i = 1; i <= list.count; ++i) {
        auto* pair = def::model::pair_nth(&list, i);
        REQUIRE(pair != nullptr);
        const auto kind = *engine::at<std::int32_t>(pair, 0x170);
        REQUIRE(kind == 1 || kind == 2);
        const auto masterId = *engine::at<std::int32_t>(pair, 0x168);
        const auto slaveId = *engine::at<std::int32_t>(pair, 0x16c);
        REQUIRE(masterId > 0 && masterId <= static_cast<int>(expectedFlags.size()));
        REQUIRE(slaveId > 0 && slaveId <= static_cast<int>(expectedFlags.size()));
        REQUIRE(def::model::object_list_find(&originalStep->objects, masterId) != nullptr);
        REQUIRE(def::model::object_list_find(&originalStep->objects, slaveId) != nullptr);
        expectedFlags[static_cast<std::size_t>((kind == 2 ? slaveId : masterId) - 1)] = 1;
        hasContact = true;
    }
    REQUIRE(hasContact);
    const auto savedCount = list.count;
    // Seed every flag to prove the enabled pair path clears stale entries.
    op->icb.flag.fill(7);
    engine::FileEffects saved;
    REQUIRE(engine::saveInterObject(p, *op, saved));
    REQUIRE(saved.writes.size() == 1);
    CHECK(saved.writes[0].path == interObjectPath(*op));
    CHECK(!saved.writes[0].bytes.empty());
    CHECK(saved.deletes.empty());
    CHECK(op->icb.flag == expectedFlags);
    CHECK_EQ(list.count, savedCount);
    CHECK(engine::opModel(p, *op) == m);
    CHECK(m->step.get() == originalStep);

    struct DetachedStep {
        std::unique_ptr<engine::DB_STEP_DATA>& owner;
        std::unique_ptr<engine::DB_STEP_DATA> saved;
        explicit DetachedStep(engine::GuiModel& model)
            : owner(model.step), saved(std::move(model.step)) {}
        DetachedStep(const DetachedStep&) = delete;
        DetachedStep& operator=(const DetachedStep&) = delete;
        ~DetachedStep() noexcept { owner = std::move(saved); }
    };
    // Exercise null refusal with the system-lubricant branch enabled. The
    // detached contacts and existing contact flags must remain untouched.
    op->lubSystem = true;
    op->icb.flag.fill(7);
    const auto unchangedFlags = op->icb.flag;
    engine::FileEffects refused;
    {
        DetachedStep guard(*m);
        REQUIRE(m->step == nullptr);
        REQUIRE(guard.saved.get() == originalStep);
        CHECK(!engine::saveInterObject(p, *op, refused));
        CHECK(refused.writes.empty() && refused.deletes.empty());
        CHECK(op->icb.flag == unchangedFlags);
        CHECK_EQ(guard.saved->inter_obj.count, savedCount);
    }
    CHECK(engine::opModel(p, *op) == m);
    REQUIRE(m->step.get() == originalStep);
    CHECK_EQ(list.count, savedCount);

    struct DetachProbe {};
    try {
        DetachedStep guard(*m);
        CHECK(!engine::saveInterObject(p, *op, refused));
        CHECK_EQ(guard.saved->inter_obj.count, savedCount);
        CHECK(op->icb.flag == unchangedFlags);
        CHECK(refused.writes.empty() && refused.deletes.empty());
        throw DetachProbe{};
    } catch (const DetachProbe&) {
    }
    CHECK(engine::opModel(p, *op) == m);
    REQUIRE(m->step.get() == originalStep);
    CHECK_EQ(list.count, savedCount);
    CHECK(op->icb.flag == unchangedFlags);
    CHECK(refused.writes.empty() && refused.deletes.empty());

    // The RAII-restored step still drives the positive contact-flags path.
    // Keep system rebuild off so the original pair input remains the oracle.
    op->lubSystem = false;
    engine::FileEffects restored;
    REQUIRE(engine::saveInterObject(p, *op, restored));
    REQUIRE(restored.writes.size() == 1);
    CHECK(restored.writes[0].path == interObjectPath(*op));
    CHECK(restored.writes[0].bytes == saved.writes[0].bytes);
    CHECK(restored.deletes.empty());
    CHECK(op->icb.flag == expectedFlags);
    CHECK_EQ(list.count, savedCount);
    CHECK(engine::opModel(p, *op) == m);
    CHECK(m->step.get() == originalStep);
}

// Positive system-lubricant rebuild on a copied, real RS39 operation. The
// serialized oracle and the source-derived in-memory list are independent:
// the writer omits kind-2 pairs whose slave precedes their master.
TEST_CASE(system_lubricant_rebuild_real_template)
{
    const fs::path oracle = oracleB();
    const fs::path before = oracle / "RS_39_brick_mesh_01_before";
    const fs::path expectedFile = oracle / "RS_39_brick_mesh_01" / "Operations" /
        "Task00003" / "InterObject.KEY";
    REQUIRE(fs::is_regular_file(before / "brick_mesh_01.moproj"));
    REQUIRE(fs::is_regular_file(expectedFile));
    const fs::path copied = m2m_test::scratchOutput(
        m2m_test::scratchRoot() / "system-lubricant-rebuild-real-template");
    fs::remove_all(copied);
    fs::copy(before, copied, fs::copy_options::recursive);

    const fs::path projectFile = copied / "brick_mesh_01.moproj";
    const qt::String file{projectFile.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx));
    REQUIRE(p.openedOk);
    // The fixture has one parsed sub-project; its XML Active="2" selects the
    // operation chain entry, not sub-project index 2.
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");

    Operation* op = operationById(p, 3);
    REQUIRE(op != nullptr);
    REQUIRE(op->classKey.utf8() == "SfOprForming");
    REQUIRE(op->lubSystem);
    REQUIRE(op->lubricantId == 1);
    const Item* lubricant = p.lubricants.find(1);
    REQUIRE(lubricant != nullptr);
    const fs::path lubricantFile(engine::local8(lubricant->keyPath));
    REQUIRE(fs::is_regular_file(lubricantFile));
    const std::string lubricantBytes = readAll(lubricantFile);
    REQUIRE(!lubricantBytes.empty());

    mst::installLoaderSlots();
    mst::OpClass* ob = opBase(*op);
    REQUIRE(ob != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();
    ob->current = op;
    engine::FileEffects loaded;
    ob->loadOperation(p, *op, ctx, loaded);
    REQUIRE(m->step != nullptr);
    engine::DB_STEP_DATA* const originalStep = m->step.get();
    REQUIRE(*engine::at<std::int32_t>(originalStep, 0x1c) == 3);

    // Independently admit the literal slot/object mapping. The in-memory table
    // below is asserted only when the source-derived master-candidate premise
    // is established from the loaded fields themselves.
    bool admitted = m->objectCount() == 3 && objSlots(*op) == 3 && op->objs.size() == 3;
    std::array<int, 3> kinds{};
    std::array<int, 3> f08{};
    std::array<bool, 3> candidate{};
    int candidateCount = 0;
    int candidatePosition = 0;
    for (int i = 1; i <= 3 && op->objs.size() == 3; ++i) {
        auto* object = m->objectAt(i);
        ObjEntry* slot = objEntryN(*op, i);
        if (!object || !slot) {
            admitted = false;
            continue;
        }
        const int id = *engine::at<std::int32_t>(object, 8);
        kinds[static_cast<std::size_t>(i - 1)] = *engine::at<std::int32_t>(object, 0xc);
        f08[static_cast<std::size_t>(i - 1)] = slot->f08;
        admitted = admitted && id == i && kinds[static_cast<std::size_t>(i - 1)] != 7;
        const int kind = kinds[static_cast<std::size_t>(i - 1)];
        candidate[static_cast<std::size_t>(i - 1)] =
            kind == 5 ? (slot->f08 != 0 && slot->f08 != 3) : (kind != 0 && kind != 3);
        if (candidate[static_cast<std::size_t>(i - 1)]) {
            ++candidateCount;
            candidatePosition = i;
        }
    }
    admitted = admitted && candidateCount == 1 && candidatePosition == 1;

    const auto originalObjectAt = [&](int i) { return m->objectAt(i); };
    std::array<std::vector<std::int32_t>, 3> bccBefore;
    const auto captureBcc = [&](int i, std::vector<std::int32_t>& out) {
        const auto* object = originalObjectAt(i);
        if (!object) return false;
        const auto nodes = *engine::at<std::int32_t>(object, 0x880);
        const auto width = *engine::at<std::int32_t>(originalStep, 0x1640);
        const auto* data = *engine::at<std::int32_t*>(object, 0x990);
        if (nodes < 0 || width < 0 || (nodes != 0 && width != 0 && !data)) return false;
        const auto count = static_cast<std::size_t>(nodes) * static_cast<std::size_t>(width);
        if (count == 0) out.clear();
        else out.assign(data, data + count);
        return true;
    };
    if (admitted)
        for (int i = 1; i <= 3; ++i)
            REQUIRE(captureBcc(i, bccBefore[static_cast<std::size_t>(i - 1)]));

    engine::FileEffects saved;
    REQUIRE(engine::saveInterObject(p, *op, saved));
    REQUIRE(saved.writes.size() == 1);
    CHECK(saved.writes[0].path == interObjectPath(*op));
    const fs::path actualPath(engine::local8(saved.writes[0].path));
    CHECK(actualPath.lexically_normal() ==
          (copied / "Operations" / "Task00003" / "InterObject.KEY").lexically_normal());
    CHECK(saved.deletes.empty());
    CHECK(engine::opModel(p, *op) == m);
    CHECK(m->step.get() == originalStep);

    const std::string expectedBytes = readAll(expectedFile);
    REQUIRE(expectedBytes.size() == 631u);
    CHECK_EQ(saved.writes[0].bytes.size(), 631u);
    CHECK_EQ(saved.writes[0].bytes, expectedBytes);
    const std::string contact = "CNTACT";
    const auto firstContact = expectedBytes.find(contact);
    REQUIRE(firstContact != std::string::npos);
    const auto secondContact = expectedBytes.find(contact, firstContact + contact.size());
    REQUIRE(secondContact != std::string::npos);
    CHECK(expectedBytes.find(contact, secondContact + contact.size()) == std::string::npos);

    if (!admitted) {
        std::printf("NOT ADMITTED system_lubricant_rebuild_real_template in-memory table: "
                    "loaded slots/IDs/kinds/f08 do not establish exactly candidate position 1 "
                    "(count=%d, candidates=%d, position=%d; kind/f08=[%d/%d,%d/%d,%d/%d])\n",
                    m->objectCount(), candidateCount, candidatePosition,
                    kinds[0], f08[0], kinds[1], f08[1], kinds[2], f08[2]);
    } else {
        const auto& list = originalStep->inter_obj;
        REQUIRE(list.count == 4);
        const std::array<std::array<int, 3>, 4> expected{{
            {{2, 1, 2}}, {{1, 2, 1}}, {{3, 1, 2}}, {{1, 3, 1}}
        }};
        for (int i = 1; i <= 4; ++i) {
            auto* pair = def::model::pair_nth(&originalStep->inter_obj, i);
            REQUIRE(pair != nullptr);
            const auto& row = expected[static_cast<std::size_t>(i - 1)];
            CHECK_EQ(*engine::at<std::int32_t>(pair, 0x168), row[0]);
            CHECK_EQ(*engine::at<std::int32_t>(pair, 0x16c), row[1]);
            CHECK_EQ(*engine::at<std::int32_t>(pair, 0x170), row[2]);
            CHECK_EQ(*engine::at<std::uint32_t>(pair, 0x174), 1u);
            CHECK_EQ(*engine::at<double>(pair, 0x20), 1.0);
            CHECK_EQ(*engine::at<double>(pair, 0x1c0), 5.0);
        }

        // The literal rebuilt list has masters 2 and 3 only. The qualified
        // bccdef_lubricant contract writes the object passed for each master,
        // so object 1 (which appears only as a slave) must remain unchanged.
        // No derived expected transition is asserted for the targeted objects.
        std::vector<std::int32_t> objectOneAfter;
        REQUIRE(captureBcc(1, objectOneAfter));
        CHECK(objectOneAfter == bccBefore[0]);
    }
}

// Direct helper mechanics: enable BCC on a real imported corpus mesh. This is
// a synthetic admission flag, not public Save or native/oracle byte equivalence.
TEST_CASE(save_bcc_step_scope)
{
    const fs::path filePath = m2m_test::configuredPath("M2M_CORPUS") /
        "46_MO2_Boolean_Oprations" / "MO2_Boolean_Oprations.moproj";
    REQUIRE(fs::is_regular_file(filePath));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    const qt::String file{filePath.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx));
    REQUIRE(p.openedOk);
    activateSubProject(p, 0, ctx);
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    REQUIRE(op->classKey.utf8() == "SfOprHeatTransferExpress2D");
    mst::installLoaderSlots();
    mst::OpClass* ob = opBase(*op);
    REQUIRE(ob != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();
    ob->current = op;
    engine::FileEffects load;
    ob->loadOperation(p, *op, ctx, load);
    REQUIRE(m->step != nullptr);
    auto* const originalStep = m->step.get();
    REQUIRE(objSlots(*op) >= 2);
    ObjEntry* entry = objEntry(*op, 0);
    REQUIRE(entry != nullptr);
    REQUIRE(entry->objectType == 5);
    // The inherited-operation loader strips mesh via loadObjectKeyGeometry.
    // Import the actual corpus mesh through the normal full-object loader for
    // this discarded helper fixture (RZ 1204 nodes, ELMCON 1104 elements).
    const fs::path meshPath = filePath.parent_path() / "Objects" / "Object00001.KEY";
    REQUIRE(fs::is_regular_file(meshPath));
    const qt::String meshFile{meshPath.u16string()};
    const int objectNumber = 1;
    REQUIRE(m->loadObjectKey(meshFile, &objectNumber));
    REQUIRE(m->step.get() == originalStep);
    auto* const object = m->object(1);
    REQUIRE(object != nullptr);
    REQUIRE(engine::objHasMesh(object));
    REQUIRE(*engine::at<std::int32_t>(object, 0xc) != 7);
    REQUIRE(def::model::object_list_find(&originalStep->objects, 1) == object);
    // The full-object loader constructs the mesh from real KEY records; no
    // object or mesh bytes are fabricated. Enable only this slot's BCC flag.
    for (int s = 0; s < objSlots(*op); ++s)
        if (auto* e = objEntry(*op, s)) e->defineBCC = false;
    entry->defineBCC = true;
    engine::FileEffects saved;
    engine::saveBcc(p, *op, saved);
    REQUIRE(saved.writes.size() == 2);
    CHECK(saved.writes[0].path == qt::cleanPath(taskDirAbs(*op) + qt::kSep + qt::L("RzElmCon00001.KEY")));
    CHECK(saved.writes[1].path == qt::cleanPath(taskDirAbs(*op) + qt::kSep + qt::L("BccDef00001.KEY")));
    CHECK(!saved.writes[0].bytes.empty());
    CHECK(!saved.writes[1].bytes.empty());
    CHECK(saved.deletes.empty());

    engine::FileEffects refused;
    entry->defineBCC = false;
    engine::saveBcc(p, *op, refused);
    CHECK(refused.writes.empty() && refused.deletes.empty());
    entry->defineBCC = true;
    entry->objectType = 0;
    engine::saveBcc(p, *op, refused);
    CHECK(refused.writes.empty() && refused.deletes.empty());
    entry->objectType = 5;

    struct DetachedStep {
        std::unique_ptr<engine::DB_STEP_DATA>& owner;
        std::unique_ptr<engine::DB_STEP_DATA> saved;
        explicit DetachedStep(engine::GuiModel& model)
            : owner(model.step), saved(std::move(model.step)) {}
        DetachedStep(const DetachedStep&) = delete;
        DetachedStep& operator=(const DetachedStep&) = delete;
        ~DetachedStep() noexcept { owner = std::move(saved); }
    };
    {
        DetachedStep guard(*m);
        REQUIRE(m->step == nullptr);
        REQUIRE(guard.saved.get() == originalStep);
        engine::saveBcc(p, *op, refused);
        CHECK(refused.writes.empty() && refused.deletes.empty());
    }
    CHECK(engine::opModel(p, *op) == m);
    REQUIRE(m->step.get() == originalStep);
    CHECK(m->object(1) == object);
    CHECK(engine::objHasMesh(object));
}

// Direct helper mechanics on a loaded operation; synthetic additions below do not
// establish public Save dispatch or native/oracle byte equivalence.
TEST_CASE(save_materials_step_scope)
{
    const fs::path filePath = m2m_test::configuredPath("M2M_CORPUS") /
        "46_MO2_Boolean_Oprations" / "MO2_Boolean_Oprations.moproj";
    REQUIRE(fs::is_regular_file(filePath));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    const qt::String file{filePath.u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    REQUIRE(openProject(p, file, ctx));
    REQUIRE(p.openedOk);
    activateSubProject(p, 0, ctx);
    Operation* op = operationById(p, 2);
    REQUIRE(op != nullptr);
    REQUIRE(op->classKey.utf8() == "SfOprHeatTransferExpress2D");
    mst::installLoaderSlots();
    mst::OpClass* ob = opBase(*op);
    REQUIRE(ob != nullptr);
    engine::GuiModel* m = engine::opModel(p, *op);
    REQUIRE(m != nullptr);
    m->reset();
    ob->current = op;
    engine::FileEffects load;
    ob->loadOperation(p, *op, ctx, load);
    REQUIRE(m->step != nullptr);
    auto* const originalStep = m->step.get();
    auto& list = originalStep->materials;
    const auto originalCount = originalStep->n_materials;
    auto* const originalHead = list.head;
    auto* const originalTail = list.tail;
    REQUIRE(originalCount >= 0);
    REQUIRE(originalCount == list.count);
    REQUIRE((originalCount == 0) == (originalHead == nullptr));
    REQUIRE((originalCount == 0) == (originalTail == nullptr));
    if (originalTail) REQUIRE(originalTail->next == nullptr);
    bool eligible = false;
    std::int32_t maxNumber = 0;
    std::printf("saveMaterials loaded op2: %d materials\n", originalCount);
    for (std::int32_t i = 1; i <= originalCount; ++i) {
        auto* mat = def::model::material_list_nth(&list, i);
        if (!mat) {
            std::printf("  material[%d]: null\n", i);
            continue;
        }
        std::printf("  material[%d]: number=%d type=%d\n", i, mat->number, mat->mstmtr_flag);
        eligible = eligible || mat->mstmtr_flag != 2;
        if (mat->number > maxNumber) maxNumber = mat->number;
    }
    REQUIRE(maxNumber < std::numeric_limits<std::int32_t>::max() - 2);
    REQUIRE(originalCount < std::numeric_limits<std::int32_t>::max() - 2);

    // Append a constructed type-2 exclusion and, only if needed, one eligible
    // material. Keep the real loaded list and owner; restore links/counts before
    // the temporary nodes are destroyed, including on assertion/exception exit.
    struct MaterialFixture {
        engine::DB_STEP_DATA& step;
        def::model::Material* head;
        def::model::Material* tail;
        std::int32_t count;
        std::int32_t n;
        std::unique_ptr<def::model::Material> added;
        std::unique_ptr<def::model::Material> excluded;
        MaterialFixture(engine::DB_STEP_DATA& s, bool addEligible, std::int32_t number)
            : step(s), head(s.materials.head), tail(s.materials.tail),
              count(s.materials.count), n(s.n_materials),
              added(addEligible ? std::make_unique<def::model::Material>(&s) : nullptr),
              excluded(std::make_unique<def::model::Material>(&s))
        {
            excluded->number = number;
            excluded->mstmtr_flag = 2;
            auto* first = excluded.get();
            if (added) {
                added->number = number + 1;
                added->next = excluded.get();
                first = added.get();
            }
            if (tail) tail->next = first;
            else step.materials.head = first;
            step.materials.tail = excluded.get();
            step.materials.count += added ? 2 : 1;
            step.n_materials = step.materials.count;
        }
        MaterialFixture(const MaterialFixture&) = delete;
        MaterialFixture& operator=(const MaterialFixture&) = delete;
        ~MaterialFixture() noexcept
        {
            if (tail) tail->next = nullptr;
            step.materials.head = head;
            step.materials.tail = tail;
            step.materials.count = count;
            step.n_materials = n;
        }
    };
    {
        MaterialFixture fixture(*originalStep, !eligible, maxNumber + 1);
        std::printf("saveMaterials synthetic: eligible=%d excluded=%d\n",
                    fixture.added ? fixture.added->number : 0, fixture.excluded->number);
        std::vector<qt::String> expected;
        std::set<std::u16string> excludedPaths;
        for (std::int32_t i = 1; i <= originalStep->n_materials; ++i) {
            auto* mat = def::model::material_list_nth(&list, i);
            if (!mat) continue;
            const auto path = engine::materialDefaultFile(p, mat->number);
            if (mat->mstmtr_flag == 2) excludedPaths.insert(path.u16());
            else expected.push_back(path);
        }
        REQUIRE(!expected.empty());
        engine::FileEffects saved;
        engine::saveMaterials(p, *op, saved);
        REQUIRE(saved.writes.size() == expected.size());
        // FileEffects::writes is a vector, so it also observes list traversal order.
        for (std::size_t i = 0; i < expected.size(); ++i) {
            CHECK(saved.writes[i].path == expected[i]);
            CHECK(!saved.writes[i].bytes.empty());
            CHECK(saved.writes[i].bytes.find("DEFORM MATERIAL KEYWORD FILE") != std::string::npos);
            CHECK(excludedPaths.count(saved.writes[i].path.u16()) == 0);
        }
        CHECK(saved.deletes.empty());
        const auto seededCount = originalStep->n_materials;
        struct DetachedStep {
            std::unique_ptr<engine::DB_STEP_DATA>& owner;
            std::unique_ptr<engine::DB_STEP_DATA> saved;
            explicit DetachedStep(engine::GuiModel& model)
                : owner(model.step), saved(std::move(model.step)) {}
            DetachedStep(const DetachedStep&) = delete;
            DetachedStep& operator=(const DetachedStep&) = delete;
            ~DetachedStep() noexcept { owner = std::move(saved); }
        };
        {
            DetachedStep guard(*m);
            REQUIRE(m->step == nullptr);
            REQUIRE(guard.saved.get() == originalStep);
            engine::FileEffects absent;
            engine::saveMaterials(p, *op, absent);
            CHECK(absent.writes.empty() && absent.deletes.empty());
            CHECK_EQ(guard.saved->n_materials, seededCount);
            CHECK_EQ(guard.saved->materials.count, seededCount);
        }
        CHECK(engine::opModel(p, *op) == m);
        REQUIRE(m->step.get() == originalStep);
        CHECK_EQ(originalStep->n_materials, seededCount);
        CHECK_EQ(list.count, seededCount);
        struct DetachProbe {};
        try {
            DetachedStep guard(*m);
            throw DetachProbe{};
        } catch (const DetachProbe&) {
        }
        CHECK(engine::opModel(p, *op) == m);
        REQUIRE(m->step.get() == originalStep);
        CHECK_EQ(originalStep->n_materials, seededCount);
        CHECK_EQ(list.count, seededCount);
    }
    CHECK(engine::opModel(p, *op) == m);
    REQUIRE(m->step.get() == originalStep);
    CHECK_EQ(originalStep->n_materials, originalCount);
    CHECK_EQ(list.count, originalCount);
    CHECK(list.head == originalHead && list.tail == originalTail);
    if (originalTail) CHECK(originalTail->next == nullptr);
}

namespace {

// Synthetic helper mechanics only: OpClass already exposes classModel. Both
// model owners remain alive through every lookup and synchronous writer call.
// This fixture does not establish public C-ABI model replacement or reentry.
struct MaterialAccessorClass final : mst::OpClass {
    MaterialAccessorClass() : OpClass(qt::L("MaterialAccessorFixture")), a(nullptr, 3), b(nullptr, 3) {}
    engine::GuiModel a, b;
    std::vector<engine::GuiModel*> script;
    std::size_t calls = 0;

    engine::GuiModel* classModel(Project&, Operation&) override
    {
        const auto position = calls++;
        return position < script.size() ? script[position] : &a;
    }
};

struct MaterialAccessorRig {
    Project project;
    ProjectScope scope{project};
    Operation op;
    MaterialAccessorClass* cls = nullptr;
    fs::path root;

    explicit MaterialAccessorRig(const char* name)
        : root(m2m_test::scratchOutput(m2m_test::scratchRoot() / "material-accessors" / name))
    {
        fs::create_directories(root / "Materials");
        project.path = qt::String((root / "accessor.moproj").u16string());
        auto owner = std::make_unique<MaterialAccessorClass>();
        cls = owner.get();
        op.classKey = cls->key();
        op.project = &project;
        op.opClass = cls;
        project.registry.emplace(cls->key(), std::move(owner));
        cls->current = &op;
    }

    static void add(engine::GuiModel& model, int number, const char* name, int type = 0)
    {
        REQUIRE(model.step != nullptr);
        auto& step = *model.step;
        auto* material = def::model::material_list_add(&step.materials);
        REQUIRE(material != nullptr);
        material->number = number;
        material->mstmtr_flag = type;
        std::snprintf(material->name, sizeof material->name, "%s", name);
        step.n_materials = step.materials.count;
    }

    qt::String path(int number) const { return engine::materialDefaultFile(project, number); }
};

} // namespace

TEST_CASE(save_materials_repeated_accessors_preserve_stable_order_duplicates_and_filter)
{
    MaterialAccessorRig r("stable");
    r.add(r.cls->a, 7, "stable-seven");
    r.add(r.cls->a, 8, "excluded-eight", 2);
    r.add(r.cls->a, 7, "duplicate-seven");
    r.add(r.cls->a, 9, "stable-nine");
    engine::FileEffects effects;
    engine::saveMaterials(r.project, r.op, effects);
    CHECK_EQ(r.cls->calls, 1u + 4u + 3u);
    REQUIRE(effects.writes.size() == 3);
    const int numbers[] = {7, 7, 9};
    for (std::size_t i = 0; i < effects.writes.size(); ++i) {
        CHECK(effects.writes[i].path == r.path(numbers[i]));
        CHECK(!effects.writes[i].bytes.empty());
        CHECK(effects.writes[i].bytes.find("DEFORM MATERIAL KEYWORD FILE") != std::string::npos);
        CHECK(effects.writes[i].bytes.find("excluded-eight") == std::string::npos);
    }
    CHECK_EQ(effects.writes[0].bytes, effects.writes[1].bytes);
    CHECK(effects.deletes.empty());
}

TEST_CASE(save_materials_initial_null_step_has_one_count_lookup_and_no_writes)
{
    MaterialAccessorRig r("initial-null-step");
    r.add(r.cls->a, 17, "held-seventeen");
    auto heldStep = std::move(r.cls->a.step);
    REQUIRE(heldStep != nullptr);
    engine::FileEffects effects;
    engine::saveMaterials(r.project, r.op, effects);
    CHECK_EQ(r.cls->calls, 1u);
    CHECK(effects.writes.empty() && effects.deletes.empty());
    CHECK_EQ(heldStep->n_materials, 1);
    CHECK_EQ(heldStep->materials.count, 1);
    r.cls->a.step = std::move(heldStep);
}

TEST_CASE(save_materials_writer_reacquires_model_and_filters_only_at_traversal)
{
    MaterialAccessorRig r("writer-reacquire");
    r.add(r.cls->a, 17, "traversal-owner-A");
    r.add(r.cls->b, 17, "writer-owner-B", 2);
    r.cls->script = {&r.cls->a, &r.cls->a, &r.cls->b};
    engine::FileEffects effects;
    engine::saveMaterials(r.project, r.op, effects);
    CHECK_EQ(r.cls->calls, 3u);
    REQUIRE(effects.writes.size() == 1);
    CHECK(effects.writes[0].path == r.path(17));
    CHECK(effects.writes[0].bytes.find("writer-owner-B") != std::string::npos);
    CHECK(effects.writes[0].bytes.find("traversal-owner-A") == std::string::npos);
    CHECK(effects.deletes.empty());
}

TEST_CASE(save_materials_traversal_reacquires_model_and_keeps_initial_count)
{
    MaterialAccessorRig r("traversal-reacquire");
    r.add(r.cls->a, 11, "first-A");
    r.add(r.cls->a, 12, "second-A");
    r.add(r.cls->b, 31, "first-B");
    r.add(r.cls->b, 42, "second-B");
    r.add(r.cls->b, 53, "third-B");
    // A supplies the saved count and first position/write. B supplies the
    // second position/write, but its larger, consistent count is not reread.
    r.cls->script = {&r.cls->a, &r.cls->a, &r.cls->a, &r.cls->b, &r.cls->b};
    engine::FileEffects effects;
    engine::saveMaterials(r.project, r.op, effects);
    CHECK_EQ(r.cls->calls, 5u);
    REQUIRE(effects.writes.size() == 2);
    CHECK(effects.writes[0].path == r.path(11));
    CHECK(effects.writes[1].path == r.path(42));
    CHECK(effects.writes[0].bytes.find("first-A") != std::string::npos);
    CHECK(effects.writes[1].bytes.find("second-B") != std::string::npos);
    CHECK(effects.writes[1].bytes.find("second-A") == std::string::npos);
    CHECK_EQ(r.cls->a.step->n_materials, 2);
    CHECK_EQ(r.cls->b.step->n_materials, 3);
    CHECK(effects.deletes.empty());
}

TEST_CASE(save_materials_missing_writer_number_preserves_target_and_continues)
{
    MaterialAccessorRig r("missing-writer-number");
    r.add(r.cls->a, 11, "missing-from-B");
    r.add(r.cls->a, 12, "second-from-A");
    r.add(r.cls->b, 12, "later-writer-B");
    r.cls->script = {&r.cls->a, &r.cls->a, &r.cls->b, &r.cls->a, &r.cls->b};
    const auto missing = r.path(11);
    const auto later = r.path(12);
    const auto missingFile = m2m_test::scratchOutput(fs::path(missing.u16()));
    const auto laterFile = m2m_test::scratchOutput(fs::path(later.u16()));
    { std::ofstream out(missingFile, std::ios::binary); out << "retained missing-number sentinel"; }
    { std::ofstream out(laterFile, std::ios::binary); out << "later target sentinel"; }
    REQUIRE(readAll(missingFile) == "retained missing-number sentinel");
    r.project.overlay[qt::cleanPath(missing)] = "retained missing-number overlay";
    engine::FileEffects effects;
    effects.nativeTaskKeys = true;
    std::vector<qt::String> attempted;
    effects.beforeNativeKeyWrite = [&](const qt::String& target) { attempted.push_back(target); };
    engine::saveMaterials(r.project, r.op, effects);
    CHECK_EQ(r.cls->calls, 5u);
    REQUIRE(attempted.size() == 1);
    CHECK(attempted[0] == later);
    CHECK_EQ(readAll(missingFile), std::string("retained missing-number sentinel"));
    CHECK_EQ(r.project.overlay.at(qt::cleanPath(missing)), std::string("retained missing-number overlay"));
    CHECK(readAll(laterFile).find("later-writer-B") != std::string::npos);
    CHECK(readAll(laterFile).find("second-from-A") == std::string::npos);
    CHECK(effects.writes.empty() && effects.deletes.empty());
}

int main()
{
    return testkit::run_all();
}
