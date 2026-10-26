#include "../support/test_paths.hpp"
// Full converter tests against recovered native file-order contracts, using
// disposable copies. They do not launch the original or claim fresh parity.
// ctest-labels: m2m-s
#include <testkit/check.hpp>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <defkey/item_writers.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/convert.hpp"
#include "m2m/blob.hpp"
#include "m2m/data/express.hpp"
#include "m2m/data/distortion.hpp"
#include "m2m/data/diestress.hpp"
#include "m2m/data/diestressbm.hpp"
#include "m2m/data/heattreat.hpp"
#include "m2m/engine.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

namespace fs = std::filesystem;
using namespace m2m;
namespace {
bool timingEnabled()
{
    static const bool enabled = [] {
        const char* value = std::getenv("M2M_SAVE_IO_TIMING");
        return value && std::string(value) == "1";
    }();
    return enabled;
}

using Clock = std::chrono::steady_clock;
void reportTiming(const char* phase, const char* scenario, Clock::time_point start)
{
    if (timingEnabled()) {
        const auto ms = std::chrono::duration<double, std::milli>(Clock::now() - start).count();
        std::fprintf(stderr, "[save-io-time] scenario=%s phase=%s ms=%.3f\n", scenario, phase, ms);
    }
}

// Optional diagnostic pass that times the fixture's files individually without
// changing the normal recursive copy or any test-owned fixture contents.
void profileFixtureFiles(const fs::path& input, const fs::path& scratch)
{
    const char* value = std::getenv("M2M_SAVE_IO_COPY_DETAIL");
    if (!value || std::string(value) != "1") return;

    const auto profileRoot = scratch.parent_path() /
        (scratch.filename().wstring() + L"-copy-profile");
    std::error_code ec;
    fs::create_directories(profileRoot, ec);
    if (ec) {
        std::fprintf(stderr, "[save-io-copy] profile setup failed error=%d\n", ec.value());
        return;
    }
    for (fs::recursive_directory_iterator it(input, ec), end; !ec && it != end; it.increment(ec)) {
        const auto relative = fs::relative(it->path(), input, ec);
        if (ec) break;
        const auto target = profileRoot / relative;
        if (it->is_directory(ec)) {
            fs::create_directories(target, ec);
            if (ec) break;
            continue;
        }
        if (!it->is_regular_file(ec)) {
            if (ec) break;
            continue;
        }
        fs::create_directories(target.parent_path(), ec);
        if (ec) break;
        const auto start = Clock::now();
        fs::copy_file(it->path(), target, fs::copy_options::none, ec);
        const auto ms = std::chrono::duration<double, std::milli>(Clock::now() - start).count();
        const auto size = it->file_size(ec);
        if (ec) break;
        const auto rel = relative.generic_string();
        std::fprintf(stderr, "[save-io-copy] file=%s bytes=%llu ms=%.3f\n", rel.c_str(),
                     static_cast<unsigned long long>(size), ms);
    }
    if (ec) std::fprintf(stderr, "[save-io-copy] profile traversal/copy failed error=%d\n", ec.value());
    fs::remove_all(profileRoot, ec);
    if (ec) std::fprintf(stderr, "[save-io-copy] profile cleanup failed error=%d\n", ec.value());
}

struct ScenarioTimer {
    const char* name;
    Clock::time_point start = Clock::now();
    explicit ScenarioTimer(const char* scenario) : name(scenario) {}
    ~ScenarioTimer() { reportTiming("total", name, start); }
};

const char* currentScenario = "unknown";
struct CurrentScenario {
    const char* previous;
    explicit CurrentScenario(const char* name) : previous(currentScenario) { currentScenario = name; }
    ~CurrentScenario() { currentScenario = previous; }
};

std::string read(const fs::path& p)
{
    std::ifstream f(p, std::ios::binary);
    return {std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>()};
}
void write(const fs::path& p, const std::string& bytes)
{
    std::ofstream f(p, std::ios::binary | std::ios::trunc);
    REQUIRE(f.is_open());
    f << bytes;
}
struct Handle {
    HANDLE value = INVALID_HANDLE_VALUE;
    Handle(const fs::path& path, DWORD share, DWORD access = GENERIC_READ)
    {
        value = CreateFileW(path.c_str(), access, share, nullptr, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, nullptr);
        REQUIRE(value != INVALID_HANDLE_VALUE);
    }
    ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
    Handle(const Handle&) = delete;
};
struct Rig {
    fs::path root, project, task, bwz, ini, mst;
    Rig(bool express = false, const char* distortion = nullptr)
    {
        static unsigned serial = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("save-io-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root));
        const auto input = distortion ? m2m_test::configuredPath("M2M_ORACLE_B") / distortion
                          : express ? m2m_test::configuredPath("M2M_CORPUS") / "49_MO2_Boolean_Oprations"
                                   : m2m_test::configuredPath("M2M_ORACLE_B") / "133_B_DBSTEP_BOOL3_before";
        const auto phaseStart = Clock::now();
        fs::copy(input,
                 root, fs::copy_options::recursive);
        reportTiming("fixture-copy", currentScenario, phaseStart);
        profileFixtureFiles(input, root);
        project = root / (express ? "MO2_Boolean_Oprations.moproj" : "COG_P1_B1.moproj");
        if (distortion)
            for (const auto& file : fs::directory_iterator(root))
                if (file.path().extension() == ".moproj") project = file.path();
        task = root / "Operations" / (express || distortion ? "Task00001" : "Task00002");
        fs::create_directories(task);
        bwz = task / "Boolean.BWZ";
        ini = task / "DEF_MACHDIST_CTRL2.INI";
        mst = root / "native-output.MST";
    }
    Result convert(const fs::path& output = {})
    {
        const auto phaseStart = Clock::now();
        auto result = m2m::convert(project.u16string(), (output.empty() ? mst : output).u16string(),
                                   M2M_FLAG_WRITE_SAVE, 1252);
        reportTiming("convert", currentScenario, phaseStart);
        return result;
    }
};

struct DistortionRig {
    Rig files;
    Project project;
    Operation op;
    std::shared_ptr<data::DistortionData> data = std::make_shared<data::DistortionData>();
    Ctx ctx;
    engine::FileEffects effects;
    DistortionRig()
    {
        project.path = qt::String(files.project.u16string());
        op.project = &project;
        op.taskDir = qt::L("Operations/Task00002");
        auto props = std::make_shared<DistortionProps>();
        props->data = data;
        setOprProps(op, props);
        ctx.nativePropertyFiles = true;
    }
    fs::path ini(int i) const { return files.task / ("DEF_MACHDIST_CTRL2.INI_" + std::to_string(i)); }
    bool save() { return engine::l6PropsSave(project, op, effects, ctx); }
};
}

// Keep default testkit registration and output intact. With M2M_SAVE_IO_TIMING=1,
// each scenario also reports total wall time and its Rig copy/convert phase timings.
#undef TEST_CASE
#define TEST_CASE(name) \
    static void name(); \
    static void name##_timed() { CurrentScenario scenario(#name); ScenarioTimer timer(#name); name(); } \
    static ::testkit::Registrar name##_registrar(#name, &name##_timed); \
    static void name()

TEST_CASE(save_failed_mst_open_keeps_prior_save_effects_but_skips_boolean_writers)
{
    Rig r;
    const auto oldProject = read(r.project);
    REQUIRE(!fs::exists(r.bwz) && !fs::exists(r.ini));
    write(r.mst, "retained MST");
    {
        Handle denied(r.mst, FILE_SHARE_READ);
        const auto result = r.convert();
        CHECK(result.code == M2M_E_WRITE); // adapter mapping of original internal false
        CHECK(!fs::exists(r.bwz) && !fs::exists(r.ini));
    }
    CHECK(read(r.mst) == "retained MST");
    CHECK(read(r.project) != oldProject); // earlier Save is retained, not rolled back
}

TEST_CASE(save_missing_mst_parent_does_not_create_it_or_run_boolean_writers)
{
    Rig r;
    const auto missing = r.root / "missing" / "output.MST";
    CHECK(r.convert(missing).code == M2M_E_WRITE);
    CHECK(!fs::exists(missing.parent_path()));
    CHECK(!fs::exists(r.bwz) && !fs::exists(r.ini));
}

TEST_CASE(save_direct_mst_write_succeeds_without_delete_sharing_and_closes_handle)
{
    Rig r;
    write(r.mst, "old MST");
    {
        Handle reader(r.mst, FILE_SHARE_READ | FILE_SHARE_WRITE);
        const auto result = r.convert();
        CHECK(result.code == M2M_OK);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
        CHECK(fs::exists(r.bwz) && fs::exists(r.ini));
    }
    // An exclusive read proves the conversion did not leave its output handle open.
    Handle exclusive(r.mst, 0);
}

TEST_CASE(save_bwz_open_failure_still_attempts_ini_and_finishes_mst)
{
    Rig r;
    const auto fixture = m2m_test::configuredPath("M2M_ORACLE_B") /
        "133_B_DBSTEP_BOOL3" / "Operations" / "Task00002" / "Boolean.BWZ";
    write(r.bwz, read(fixture));
    const auto prior = read(r.bwz);
    {
        Handle denied(r.bwz, FILE_SHARE_READ);
        const auto result = r.convert();
        CHECK(result.code == M2M_OK);
        CHECK(read(r.ini).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
    }
    CHECK(read(r.bwz) == prior);
}

TEST_CASE(save_ini_open_failure_keeps_bwz_and_finishes_mst)
{
    Rig r;
    write(r.ini, "prior INI");
    {
        Handle denied(r.ini, FILE_SHARE_READ);
        const auto result = r.convert();
        CHECK(result.code == M2M_OK);
        CHECK(read(r.bwz).find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
    }
    CHECK(read(r.ini) == "prior INI");
}

TEST_CASE(save_boolean_destination_alias_is_rejected_before_mutation)
{
    Rig r;
    const auto original = read(r.project);
    CHECK(r.convert(r.bwz).code == M2M_E_ARG);
    CHECK(!fs::exists(r.bwz));
    CHECK(read(r.project) == original);
}

TEST_CASE(save_project_open_failure_preserves_project_but_continues_boolean_and_mst)
{
    Rig r;
    const auto original = read(r.project);
    {
        Handle denied(r.project, FILE_SHARE_READ);
        const auto result = r.convert();
        CHECK(result.code == M2M_E_WRITE); // adapter reports after permitted later stages
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
        CHECK(read(r.bwz).find("WZD_BOOLEAN_CUTTER_OBJECT_NO") != std::string::npos);
        CHECK(read(r.ini).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
    }
    CHECK(read(r.project) == original);
}

TEST_CASE(save_project_direct_write_does_not_require_delete_sharing)
{
    Rig r;
    const auto original = read(r.project);
    {
        Handle reader(r.project, FILE_SHARE_READ | FILE_SHARE_WRITE);
        CHECK(r.convert().code == M2M_OK);
        CHECK(read(r.project) != original);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
    }
    Handle exclusive(r.project, 0);
}

TEST_CASE(project_native_open_failure_precedes_dom_mutation)
{
    Rig r;
    Project project;
    project.path = qt::String(r.project.u16string());
    project.title = qt::L("new title");
    project.dom = std::make_shared<xml::Document>();
    project.dom->root = xml::createElement(qt::L("ProjectFile"));
    const auto root = project.dom->root;
    const auto original = xml::toString(*project.dom, 1);
    const auto views = moproj::guiViews(project);
    Handle denied(r.project, FILE_SHARE_READ);
    CHECK(!moproj::saveProjectFileNative(project, false, *views, {}));
    CHECK(project.dom->root == root);
    CHECK(xml::toString(*project.dom, 1) == original);
}

TEST_CASE(save_forming_wizard_open_failure_keeps_file_and_continues_mst)
{
    Rig r(true);
    const auto fwz = r.task / "Forming.FWZ";
    write(fwz, "DONE\n");
    {
        Handle denied(fwz, FILE_SHARE_READ);
        CHECK(r.convert().code == M2M_OK);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
    }
    CHECK(read(fwz) == "DONE\n");
}

TEST_CASE(save_forming_wizard_direct_write_does_not_require_delete_sharing)
{
    Rig r(true);
    const auto fwz = r.task / "Forming.FWZ";
    write(fwz, "DONE\n");
    {
        Handle reader(fwz, FILE_SHARE_READ | FILE_SHARE_WRITE);
        CHECK(r.convert().code == M2M_OK);
        CHECK(read(fwz).find("WZD_FORM_FORMING_TYPE") != std::string::npos);
    }
    Handle exclusive(fwz, 0);
}

TEST_CASE(save_forming_wizard_destination_alias_is_rejected_before_mutation)
{
    Rig r(true);
    const auto fwz = r.task / "Forming.FWZ";
    const auto original = read(r.project);
    REQUIRE(fs::exists(fwz)); // the imported Express fixture already owns this side file
    const auto originalWizard = read(fwz);
    CHECK(r.convert(fwz).code == M2M_E_ARG);
    CHECK(read(fwz) == originalWizard);
    CHECK(read(r.project) == original);
}

TEST_CASE(save_forming_wizard_stream_failure_retains_prefix_and_continues_mst)
{
    Rig r(true);
    const auto fwz = r.task / "Forming.FWZ";
    write(fwz, "DONE\n");
    const std::string prefix = "WZD_FORM_FORMING_TYPE\n";
    {
        Handle lock(fwz, FILE_SHARE_READ | FILE_SHARE_WRITE, GENERIC_READ | GENERIC_WRITE);
        OVERLAPPED range{};
        range.Offset = static_cast<DWORD>(prefix.size());
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                          0, 1, 0, &range));
        CHECK(r.convert().code == M2M_OK);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
        REQUIRE(UnlockFileEx(lock.value, 0, 1, 0, &range));
    }
    CHECK(read(fwz) == prefix); // endl flushes first line; later failure stays sticky
}

TEST_CASE(forming_wizard_missing_parent_and_locale_text_follow_native_writer)
{
    Rig r;
    Project p;
    p.path = qt::String(r.project.u16string());
    Operation op;
    op.project = &p;
    op.taskDir = qt::L("not-created");
    auto props = std::make_shared<ExpressProps>();
    props->data = std::make_shared<data::ExpressData>();
    props->data->importMatCatWorkpiece = qt::String(u"caf\u00e9\nsecond line");
    props->data->meshType = {{3, 2}, {1, 4}};
    setOprProps(op, props);
    const auto dir = r.root / "not-created";
    const auto fwz = dir / "Forming.FWZ";
    Ctx ctx;
    ctx.nativePropertyFiles = true;
    engine::FileEffects fx;
    CHECK(engine::expressPropsSave(p, op, fx, ctx));
    CHECK(!fs::exists(dir) && fx.writes.empty());
    fs::create_directory(dir);
    CHECK(engine::expressPropsSave(p, op, fx, ctx));
    CHECK(read(fwz).find("caf\xe9\nsecond line\n") != std::string::npos);
    CHECK(read(fwz).find("2\n1\n4\n3\n2\n\nDONE\n\n") != std::string::npos);
    CHECK(fx.writes.empty());
    ctx.nativePropertyFiles = false;
    CHECK(engine::expressPropsSave(p, op, fx, ctx));
    REQUIRE(fx.writes.size() == 1);
    CHECK(fx.writes[0].bytes == read(fwz));
}

TEST_CASE(distortion_failed_ini_open_skips_that_file_state_but_attempts_later_file)
{
    DistortionRig r;
    r.data->freeDist.f91 = 7;
    write(r.ini(0), "retained zero");
    {
        Handle denied(r.ini(0), FILE_SHARE_READ);
        CHECK(r.save());
        CHECK(read(r.ini(1)).find("BCC_INIT    \t7\n") != std::string::npos);
        CHECK(r.data->freeDist.f91 == 0); // successful later file resets it
    }
    CHECK(read(r.ini(0)) == "retained zero");
    CHECK(r.effects.writes.empty());
}

TEST_CASE(distortion_all_ini_opens_fail_preserves_writer_state)
{
    DistortionRig r;
    r.data->freeDist.f91 = 7;
    write(r.ini(0), "retained zero");
    write(r.ini(1), "retained one");
    {
        Handle first(r.ini(0), FILE_SHARE_READ), second(r.ini(1), FILE_SHARE_READ);
        CHECK(r.save());
    }
    CHECK(r.data->freeDist.f91 == 7);
    CHECK(read(r.ini(0)) == "retained zero" && read(r.ini(1)) == "retained one");
    CHECK(read(r.files.task / "MachDistortion.MDWZ").find("BCC_INIT    \t7\n") != std::string::npos);
}

TEST_CASE(distortion_stream_failure_keeps_later_state_updates_and_next_file)
{
    DistortionRig r;
    r.data->freeDist.f91 = 7;
    write(r.ini(0), "");
    {
        Handle lock(r.ini(0), FILE_SHARE_READ | FILE_SHARE_WRITE, GENERIC_READ | GENERIC_WRITE);
        OVERLAPPED range{};
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 4096, 0, &range));
        CHECK(r.save());
        CHECK(r.data->freeDist.f91 == 0); // open succeeded, so body effects still run
        REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
    }
    CHECK(read(r.ini(0)).empty());
    CHECK(read(r.ini(1)).find("BCC_INIT    \t0\n") != std::string::npos);
}

TEST_CASE(save_distortion_mdwz_failure_still_writes_ini_and_mst)
{
    Rig r(false, "101_B_DIS3F_before");
    const auto mdwz = r.task / "MachDistortion.MDWZ";
    REQUIRE(!fs::exists(mdwz)); // inspected original fixture has no wizard file
    write(mdwz, "DONE\n");
    {
        Handle denied(mdwz, FILE_SHARE_READ);
        CHECK(r.convert().code == M2M_OK);
        CHECK(read(r.task / "DEF_MACHDIST_CTRL2.INI_0").find("WZD_MACHDIST_OPERATION_KEEP_WORKPIECE_ONLY") != std::string::npos);
        CHECK(read(r.task / "DEF_MACHDIST_CTRL2.INI_1").find("WZD_MACHDIST_OPERATION_UNLOAD") != std::string::npos);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
    }
    CHECK(read(mdwz) == "DONE\n");
}

TEST_CASE(save_distortion_ini_alias_is_rejected_before_save_effects)
{
    Rig r(false, "101_B_DIS3F_before");
    const auto output = r.task / "DEF_MACHDIST_CTRL2.INI_0";
    REQUIRE(!fs::exists(output));
    const auto original = read(r.project);
    CHECK(r.convert(output).code == M2M_E_ARG);
    CHECK(!fs::exists(output));
    CHECK(read(r.project) == original);
}

TEST_CASE(distortion_ini_input_ascii_conversion_differs_from_output_qstring)
{
    DistortionRig r;
    r.op.taskDir = qt::String(u"Operations/\u20ac");
    r.ctx.nativePropertyFiles = false;
    r.ctx.codepage = 65001;
    CHECK(r.save());
    REQUIRE(r.effects.writes.size() == 3);
    const auto& ini = r.effects.writes[1].bytes;
    CHECK(ini.find("Operations/?/Step0.KEY\n") != std::string::npos);
    CHECK(ini.find("Operations/\xe2\x82\xac/DEF_MACHDIST_OUT0.KEY\n") != std::string::npos);
}

TEST_CASE(diestress_xml_native_matches_recording_without_delete_sharing)
{
    DistortionRig r; // shared disposable directory/model shell, replace the properties
    auto props = std::make_shared<DiestressProps>();
    props->data = std::make_shared<data::DiestressData>();
    props->data->stepNumber = 17;
    setOprProps(r.op, props);
    r.ctx.nativePropertyFiles = false;
    CHECK(r.save());
    REQUIRE(r.effects.writes.size() == 1);
    const auto expected = r.effects.writes[0].bytes;
    r.effects.writes.clear();
    const auto path = r.files.task / "DieStress.Wzd";
    write(path, "long prior content");
    {
        Handle reader(path, FILE_SHARE_READ | FILE_SHARE_WRITE);
        r.ctx.nativePropertyFiles = true;
        CHECK(r.save());
    }
    CHECK_EQ(read(path), expected);
    CHECK(r.effects.writes.empty());
    Handle exclusive(path, 0);
}

TEST_CASE(bm_xml_failed_open_skips_block_reads_and_leaves_no_replay)
{
    DistortionRig r;
    auto props = std::make_shared<DiestressBMProps>();
    props->data = std::make_shared<data::DiestressBMData>();
    props->data->objTotalNum = 16; // successful-open branch remains outside represented fixed block
    setOprProps(r.op, props);
    const auto path = r.files.task / "DieStressBmWizard.Wzd";
    write(path, "retained");
    {
        Handle denied(path, FILE_SHARE_READ);
        CHECK(r.save()); // original open gate skips all block access
    }
    CHECK_EQ(read(path), std::string("retained"));
    CHECK(r.effects.writes.empty());
}

TEST_CASE(bm_xml_large_native_document_retains_unicode_and_recording_bytes)
{
    DistortionRig r;
    auto props = std::make_shared<DiestressBMProps>();
    props->data = std::make_shared<data::DiestressBMData>();
    props->data->newDbName = qt::String(std::u16string(17000, u'\u20ac') + u"&\n");
    setOprProps(r.op, props);
    r.ctx.nativePropertyFiles = false;
    CHECK(r.save());
    REQUIRE(r.effects.writes.size() == 1);
    const auto expected = r.effects.writes[0].bytes;
    r.effects.writes.clear();
    r.ctx.nativePropertyFiles = true;
    CHECK(r.save());
    CHECK_EQ(read(r.files.task / "DieStressBmWizard.Wzd"), expected);
    CHECK(r.effects.writes.empty());
}

TEST_CASE(save_diestress_xml_open_failure_retains_file_and_continues_mst)
{
    Rig r(false, "98_B_DS3F_before");
    const auto path = r.task / "DieStress.Wzd";
    REQUIRE(!fs::exists(path));
    const std::string prior = "<!DOCTYPE DieStressWizard>\n<DieStressWizard nStepNumber=\"3\"/>\n";
    write(path, prior);
    {
        Handle denied(path, FILE_SHARE_READ);
        CHECK_EQ(r.convert().code, M2M_OK);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
    }
    CHECK_EQ(read(path), prior);
}

TEST_CASE(save_diestress_xml_stream_failure_keeps_later_mst)
{
    Rig r(false, "98_B_DS3F_before");
    const auto path = r.task / "DieStress.Wzd";
    REQUIRE(!fs::exists(path));
    write(path, "");
    {
        Handle lock(path, FILE_SHARE_READ | FILE_SHARE_WRITE, GENERIC_READ | GENERIC_WRITE);
        OVERLAPPED range{};
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 4096, 0, &range));
        CHECK_EQ(r.convert().code, M2M_OK);
        REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
    }
    CHECK(read(path).empty());
    CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
}

TEST_CASE(save_xml_alias_is_rejected_before_project_and_wizard_mutation)
{
    for (const auto& item : {std::pair{"98_B_DS3F_before", "DieStress.Wzd"},
                             std::pair{"100_B_BM3F_before", "DieStressBmWizard.Wzd"}}) {
        Rig r(false, item.first);
        const auto path = r.task / item.second;
        REQUIRE(!fs::exists(path));
        const auto original = read(r.project);
        CHECK_EQ(r.convert(path).code, M2M_E_ARG);
        CHECK(!fs::exists(path));
        CHECK_EQ(read(r.project), original);
    }
}

TEST_CASE(ht_native_open_failure_preserves_preopen_model_fill)
{
    DistortionRig r;
    makeRegistry(r.project);
    r.op.classKey = qt::L("SfOprHeatTreat");
    r.op.opClass = findClass(r.project, r.op.classKey);
    REQUIRE(r.op.opClass != nullptr);
    r.op.engineModel = std::make_shared<engine::GuiModel>(nullptr, 2);
    auto props = std::make_shared<HeatTreatProps>();
    props->data = std::make_shared<data::HeatTreatData>();
    setOprProps(r.op, props);
    auto& schedule = props->data->schedule;
    schedule.owner = props.get();
    schedule.ectmfn.push_back(std::make_unique<data::HTNodeEntry>());
    schedule.ectmfn[0]->media = -1;
    const auto path = r.files.task / "HeatTreatWizard.HTWZ";
    write(path, "retained");
    {
        Handle denied(path, FILE_SHARE_READ);
        CHECK(r.save());
    }
    CHECK(schedule.ectmfn[0]->object == 1 && schedule.ectmfn[0]->media == 0);
    REQUIRE(schedule.ecc.size() == 1);
    CHECK(schedule.ecc[0]->object == 1);
    CHECK_EQ(read(path), std::string("retained"));
    CHECK(r.effects.writes.empty());
}

TEST_CASE(ht_native_sharing_write_uses_locale_codec_for_media_and_latin1_for_user_key)
{
    DistortionRig r;
    auto props = std::make_shared<HeatTreatProps>();
    props->data = std::make_shared<data::HeatTreatData>();
    setOprProps(r.op, props);
    auto& schedule = props->data->schedule;
    schedule.owner = props.get();
    schedule.media.push_back(std::make_unique<data::HTMedia>());
    schedule.media[0]->name = qt::String(u"\u20ac");
    schedule.steps.push_back(std::make_unique<data::HTStep>());
    schedule.steps[0]->userKey = qt::String(u"\u20ac");
    r.ctx.codepage = 65001;
    const auto path = r.files.task / "HeatTreatWizard.HTWZ";
    write(path, "old");
    {
        Handle reader(path, FILE_SHARE_READ | FILE_SHARE_WRITE);
        CHECK(r.save());
    }
    const auto bytes = read(path);
    CHECK(bytes.find("MEDIA    1\r\n\xe2\x82\xac\r\n") != std::string::npos);
    CHECK(bytes.find("  ?   0\r\n") != std::string::npos);
    CHECK(bytes.find("START_OPTNO    1\r\n") != std::string::npos);
    CHECK(r.effects.writes.empty());
    Handle exclusive(path, 0);
}

TEST_CASE(ht_native_failed_flush_is_ignored_with_no_atomic_replay)
{
    DistortionRig r;
    auto props = std::make_shared<HeatTreatProps>();
    props->data = std::make_shared<data::HeatTreatData>();
    setOprProps(r.op, props);
    props->data->schedule.owner = props.get();
    const auto path = r.files.task / "HeatTreatWizard.HTWZ";
    write(path, "");
    {
        Handle lock(path, FILE_SHARE_READ | FILE_SHARE_WRITE, GENERIC_READ | GENERIC_WRITE);
        OVERLAPPED range{};
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 4096, 0, &range));
        CHECK(r.save());
        REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
    }
    CHECK(read(path).empty());
    CHECK(r.effects.writes.empty());
}

TEST_CASE(save_ht_wizard_alias_is_rejected_before_project_mutation)
{
    Rig r(false, "103_B_HT3F_before");
    const auto path = r.task / "HeatTreatWizard.HTWZ";
    const auto wizardBefore = read(path);
    const auto original = read(r.project);
    CHECK_EQ(r.convert(path).code, M2M_E_ARG);
    CHECK_EQ(read(r.project), original);
    CHECK_EQ(read(path), wizardBefore);
}

TEST_CASE(native_save_continues_after_unused_project_directory_cannot_be_created)
{
    Rig files;
    const auto target = files.root / "Lubricants";
    if (fs::exists(target)) fs::rename(target, files.root / "preserved-lubricants");
    write(target, "blocking file retained");
    const auto result = files.convert();
    CHECK_EQ(result.code, M2M_OK);
    CHECK(fs::exists(files.mst));
    CHECK_EQ(read(target), std::string("blocking file retained"));
}

TEST_CASE(native_simctrl_failed_open_skips_model_flag_mutation)
{
    Rig files;
    Project project;
    initProject(project, qt::String(files.root.u16string()), qt::L("FlagObservation"));
    makeRegistry(project);
    auto* op = createOperation(project, qt::L("Boolean"), qt::L("SfOprBoolean3D"), {}, 1);
    REQUIRE(op != nullptr);
    op->taskDir = qt::L("Operations/Task00002");
    opBase(*op)->current = op;
    op->engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
    auto& step = *op->engineModel->step;
    def::key::set_model_unit(step, 1, true);
    step.u1744 = 0x123456;
    const auto key = files.task / "SimCtrl.KEY";
    const auto before = read(key);
    engine::FileEffects effects; effects.nativeSimCtrl = true;
    {
        Handle locked(key, FILE_SHARE_READ);
        CHECK(!engine::saveSimCtrl(project, *op, effects));
        CHECK_EQ(step.u1744, 0x123456);
        CHECK_EQ(read(key), before);
    }
    project.overlay[qt::cleanPath(simCtrlPath(*op))] = "stale imported blob";
    CHECK(engine::saveSimCtrl(project, *op, effects));
    CHECK_EQ(step.u1744, 0xf);
    CHECK(project.overlay.find(qt::cleanPath(simCtrlPath(*op))) == project.overlay.end());
    CHECK(effects.writes.empty());
    CHECK(!read(key).empty());
    {
        Handle lock(key, FILE_SHARE_READ | FILE_SHARE_WRITE);
        OVERLAPPED range{};
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                           0, 4096, 0, &range));
        step.u1744 = 0x654321;
        CHECK(engine::saveSimCtrl(project, *op, effects));
        CHECK_EQ(step.u1744, 0xf); // open succeeded; ignored failed flush cannot roll this back
        REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
        CHECK(read(key).empty());
    }
}

TEST_CASE(native_save_continues_after_simctrl_open_failure)
{
    Rig files;
    const auto key = files.task / "SimCtrl.KEY";
    const auto before = read(key);
    Handle locked(key, FILE_SHARE_READ);
    const auto result = files.convert();
    CHECK_EQ(result.code, M2M_OK);
    CHECK_EQ(read(key), before);
    CHECK(fs::exists(files.mst));
}

TEST_CASE(native_simctrl_does_not_require_delete_sharing_and_alias_is_rejected)
{
    Rig files;
    const auto key = files.task / "SimCtrl.KEY";
    Handle locked(key, FILE_SHARE_READ | FILE_SHARE_WRITE);
    CHECK_EQ(files.convert().code, M2M_OK);
    const auto before = read(key);
    CHECK_EQ(files.convert(key).code, M2M_E_ARG);
    CHECK_EQ(read(key), before);
}

namespace {
struct TaskKeyRig {
    Rig files{true}; // active file-backed object; DB Boolean fixture has only negative object IDs
    Project project;
    ProjectScope scope{project};
    Ctx ctx;
    Operation* op = nullptr;
    engine::FileEffects native;
    TaskKeyRig()
    {
        initProject(project, qt::String(files.root.u16string()), qt::L("COG_P1_B1"));
        makeRegistry(project);
        REQUIRE(openProject(project, qt::String(files.project.u16string()), ctx));
        activateSubProject(project, 0, ctx);
        engine::FileEffects opened;
        op = engine::openActiveOperation(project, ctx, opened);
        REQUIRE(op != nullptr);
        native.nativeDirectories = native.nativeSimCtrl = native.nativeTaskKeys = native.nativeRemovals = true;
    }
    int firstObjectSlot()
    {
        for (int s = 0; s < objSlots(*op); ++s)
            if (objId(*op, s) > 0) return s;
        REQUIRE(false);
        return -1;
    }
    fs::path objectPath(int slot)
    {
        const auto* item = project.objects.find(objId(*op, slot));
        REQUIRE(item != nullptr);
        return fs::path(qt::cleanPath(item->keyPath).u16());
    }
};
}

TEST_CASE(native_object_refresh_removes_file_before_row_without_deferred_replay)
{
    TaskKeyRig r;
    const int id = engine::newObjectItem(r.project);
    const auto path = itemKeyPath(r.project, ItemKind::Object, id);
    write(fs::path(path.u16()), "unused object");
    bool rowPresentAtRemoval = false;
    r.native.beforeNativeRemoval = [&](const qt::String& target) {
        if (target == path) rowPresentAtRemoval = r.project.objects.find(id) != nullptr;
    };
    engine::refreshObjManager(r.project, r.native);
    CHECK(rowPresentAtRemoval);
    CHECK(!fs::exists(fs::path(path.u16())));
    CHECK(r.project.objects.find(id) == nullptr);
    CHECK(r.native.deletes.empty());
    REQUIRE(r.native.removalAttempts.size() == 1);
    CHECK(r.native.removalAttempts[0].succeeded);
    engine::refreshObjManager(r.project, r.native);
    CHECK(r.native.removalAttempts.size() == 1); // row was removed, no retry
}

namespace {
struct EquipmentRemovalRig {
    TaskKeyRig base;
    std::shared_ptr<doe::SessionNotificationState> notifier =
        std::make_shared<doe::SessionNotificationState>();
    ObjEntry* record = nullptr;
    fs::path file;
    EquipmentRemovalRig()
    {
        auto& p = base.project;
        auto& op = *base.op;
        auto* model = op.engineModel.get();
        REQUIRE(model != nullptr && model->objectCount() > 0);
        model->simCtl()->pdie1 = 0; // all objects take the non-primary MultiBlow path
        for (auto& entry : op.objs) if (entry) entry->equipmentId = -1;
        record = objEntryN(op, 1);
        REQUIRE(record != nullptr);
        record->equipmentId = 7;
        p.equipment.rows.clear();
        file = base.files.root / "Equipments" / "Equipment00007.KEY";
        fs::create_directories(file.parent_path());
        write(file, "retained equipment");
        auto item = std::make_unique<Item>();
        item->kind = ItemKind::Equipment;
        item->id = 7;
        item->keyPath = qt::String(file.u16string());
        p.equipment.rows.push_back(std::move(item));
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindSessionNotificationState(notifier);
    }
    void sync() { engine::equipmentSync(base.project, *base.op, true, base.native); }
};
}

TEST_CASE(native_equipment_removal_attempt_precedes_row_and_notifier_even_when_denied)
{
    for (bool deny : {false, true}) {
        EquipmentRemovalRig r;
        std::unique_ptr<Handle> locked;
        if (deny) locked = std::make_unique<Handle>(r.file, FILE_SHARE_READ | FILE_SHARE_WRITE);
        bool before = false;
        r.notifier->flag90 = true;
        r.base.native.beforeNativeRemoval = [&](const qt::String&) {
            before = r.base.project.equipment.find(7) != nullptr &&
                     r.record->equipmentId == 7 && r.notifier->attempts.empty();
        };
        r.sync();
        CHECK(before);
        CHECK(r.base.project.equipment.find(7) == nullptr);
        CHECK_EQ(r.record->equipmentId, -1);
        CHECK_EQ(fs::exists(r.file), deny);
        REQUIRE(r.base.native.removalAttempts.size() == 1);
        CHECK_EQ(r.base.native.removalAttempts[0].succeeded, !deny);
        CHECK(r.base.native.deletes.empty());
        REQUIRE(r.notifier->attempts.size() == 1);
        CHECK_EQ(r.notifier->attempts[0].signal, std::string("2signalMoEquipManagerChanged()"));
        REQUIRE(r.notifier->attempts[0].argument.has_value());
        CHECK_EQ(*r.notifier->attempts[0].argument, 6);
        CHECK(!r.notifier->flag90);
        r.sync(); // negative record now skips the wrapper entirely
        CHECK_EQ(r.notifier->attempts.size(), 1u);
    }
}

TEST_CASE(native_equipment_missing_row_still_notifies_without_removing_orphan_file)
{
    EquipmentRemovalRig r;
    r.base.project.equipment.rows.clear();
    r.sync();
    CHECK(fs::exists(r.file));
    CHECK(r.base.native.removalAttempts.empty());
    REQUIRE(r.notifier->attempts.size() == 1);
    CHECK_EQ(*r.notifier->attempts[0].argument, 6);
    CHECK_EQ(r.record->equipmentId, -1);
}

TEST_CASE(native_equipment_notifier_boundary_keeps_prior_removal_and_uncommitted_record)
{
    EquipmentRemovalRig r;
    r.base.project.doeIo.reset();
    bool rejected = false;
    try { r.sync(); }
    catch (const Error& e) { rejected = e.code == M2M_E_UNSUPPORTED; }
    CHECK(rejected);
    CHECK(!fs::exists(r.file));
    CHECK(r.base.project.equipment.find(7) == nullptr);
    CHECK_EQ(r.record->equipmentId, 7); // caller assignment is after wrapper notification
    CHECK(r.notifier->attempts.empty());
}

TEST_CASE(native_object_refresh_ignores_denied_delete_and_still_removes_row)
{
    TaskKeyRig r;
    const int id = engine::newObjectItem(r.project);
    const auto path = itemKeyPath(r.project, ItemKind::Object, id);
    write(fs::path(path.u16()), "retained object");
    Handle denied(fs::path(path.u16()), FILE_SHARE_READ | FILE_SHARE_WRITE);
    engine::refreshObjManager(r.project, r.native);
    CHECK_EQ(read(fs::path(path.u16())), std::string("retained object"));
    CHECK(r.project.objects.find(id) == nullptr);
    CHECK(r.native.deletes.empty());
    REQUIRE(r.native.removalAttempts.size() == 1);
    CHECK(!r.native.removalAttempts[0].succeeded);
}

TEST_CASE(recorded_object_refresh_preserves_file_until_effect_application)
{
    TaskKeyRig r;
    const int id = engine::newObjectItem(r.project);
    const auto path = itemKeyPath(r.project, ItemKind::Object, id);
    write(fs::path(path.u16()), "recorded object");
    engine::FileEffects recorded;
    engine::refreshObjManager(r.project, recorded);
    CHECK_EQ(read(fs::path(path.u16())), std::string("recorded object"));
    CHECK(r.project.objects.find(id) == nullptr);
    REQUIRE(recorded.deletes.size() == 1);
    CHECK(recorded.deletes[0] == path);
    CHECK(recorded.removalAttempts.empty());
}

TEST_CASE(native_removal_alias_guard_precedes_file_and_row_mutation)
{
    TaskKeyRig r;
    const int id = engine::newObjectItem(r.project);
    const auto path = itemKeyPath(r.project, ItemKind::Object, id);
    write(fs::path(path.u16()), "protected alias");
    r.native.beforeNativeRemoval = [&](const qt::String& target) {
        if (target == path) throw Error(M2M_E_ARG, "test output alias");
    };
    bool rejected = false;
    try { engine::refreshObjManager(r.project, r.native); }
    catch (const Error& error) { rejected = error.code == M2M_E_ARG; }
    CHECK(rejected);
    CHECK(r.project.objects.find(id) != nullptr);
    CHECK_EQ(read(fs::path(path.u16())), std::string("protected alias"));
    CHECK(r.native.deletes.empty() && r.native.removalAttempts.empty());
}

TEST_CASE(native_stale_restart_removal_is_immediate_and_denial_does_not_stop_load)
{
    for (const bool denied : {false, true}) {
        TaskKeyRig r;
        Operation* target = nullptr;
        for (const auto& op : r.project.operations)
            if (op && !isChainHead(*op) && opBase(*op)->restartCapable()) { target = op.get(); break; }
        REQUIRE(target != nullptr);
        setRestartGiven(*target, false, false);
        setRestartKey(*target, qt::L("stale-removal.KEY"));
        target->oprFlag = 1; // already configured: no setup-type dialog branch
        const auto path = restartKeyPath(*target, true);
        fs::create_directories(fs::path(path.u16()).parent_path());
        write(fs::path(path.u16()), "stale restart");
        std::unique_ptr<Handle> lock;
        if (denied) lock = std::make_unique<Handle>(fs::path(path.u16()), FILE_SHARE_READ | FILE_SHARE_WRITE);
        engine::decideAndLoad(r.project, *target, false, r.ctx, r.native);
        CHECK(fs::exists(fs::path(path.u16())) == denied);
        CHECK(!target->restartGiven);
        CHECK(r.native.deletes.empty());
        const auto attempt = std::find_if(r.native.removalAttempts.begin(), r.native.removalAttempts.end(),
            [&](const auto& item) { return item.path == path; });
        REQUIRE(attempt != r.native.removalAttempts.end());
        CHECK(attempt->succeeded == !denied);
    }
}

TEST_CASE(native_task_writers_match_recorded_bytes_and_leave_no_deferred_replay)
{
    TaskKeyRig r;
    const int slot = r.firstObjectSlot();
    auto* entry = objEntry(*r.op, slot);
    REQUIRE(entry != nullptr);
    entry->meshSetting = true;
    engine::FileEffects recorded;
    CHECK(engine::syncOperationFiles(r.project, *r.op, recorded));
    const auto has = [&](const char* part) {
        return std::any_of(recorded.writes.begin(), recorded.writes.end(), [&](const auto& w) {
            return w.path.utf8().find(part) != std::string::npos;
        });
    };
    REQUIRE(has("InterObject") && has("MeshSettings") && has("ObjProperties") && has("Material"));
    REQUIRE(!recorded.writes.empty());
    std::vector<qt::String> attempted;
    r.native.beforeNativeKeyWrite = [&](const qt::String& path) { attempted.push_back(path); };
    CHECK(engine::syncOperationFiles(r.project, *r.op, r.native));
    CHECK(r.native.writes.empty());
    for (const auto& written : recorded.writes) {
        CHECK_EQ(read(fs::path(written.path.u16())), written.bytes);
        if (qt::fileName(written.path) != qt::L("SimCtrl.KEY"))
            CHECK(std::find(attempted.begin(), attempted.end(), written.path) != attempted.end());
    }
}

TEST_CASE(native_object_open_controls_flag_mutation_and_keeps_previous_file_on_denial)
{
    TaskKeyRig r;
    const int slot = r.firstObjectSlot();
    const auto target = r.objectPath(slot);
    const auto prior = read(target);
    auto* model = engine::opModel(r.project, *r.op);
    REQUIRE(model && model->step);
    model->step->u1744 = 0x7365;
    {
        Handle denied(target, FILE_SHARE_READ);
        CHECK(!engine::saveObject(r.project, *r.op, slot, r.native));
        CHECK_EQ(model->step->u1744, 0x7365);
        CHECK_EQ(read(target), prior);
    }
    const qt::String path(target.u16string());
    r.project.overlay[qt::cleanPath(path)] = "stale object";
    CHECK(engine::saveObject(r.project, *r.op, slot, r.native));
    CHECK_EQ(model->step->u1744, 0x3000);
    CHECK(r.project.overlay.find(qt::cleanPath(path)) == r.project.overlay.end());
    CHECK(r.native.writes.empty());
}

TEST_CASE(native_whole_model_key_changes_flags_before_denied_open_but_not_empty_name)
{
    TaskKeyRig r;
    auto* model = engine::opModel(r.project, *r.op);
    REQUIRE(model && model->step && model->step->u1b5a == 0);
    auto& step = *model->step;
    const auto target = r.files.root / "restart-order.KEY";
    const qt::String path(target.u16string());
    write(target, "retained");
    step.u1744 = 0x1357;
    CHECK_EQ(engine::saveWholeModelKey(r.project, step, {}, 0x2468, r.native), 1);
    CHECK_EQ(step.u1744, 0x1357);
    r.project.overlay[qt::cleanPath(path)] = "retained overlay";
    {
        Handle denied(target, FILE_SHARE_READ);
        CHECK_EQ(engine::saveWholeModelKey(r.project, step, path, 0x2468, r.native), 1);
        CHECK_EQ(step.u1744, 0x2468);
        CHECK_EQ(read(target), std::string("retained"));
        CHECK_EQ(r.project.overlay.at(qt::cleanPath(path)), std::string("retained overlay"));
    }
    CHECK(r.native.writes.empty());
}

TEST_CASE(native_restart_key_runs_at_the_operation_save_slot_without_replay)
{
    TaskKeyRig r;
    setRestartGiven(*r.op, true, false);
    setRestartKey(*r.op, qt::L("restart-native.KEY"));
    const auto path = restartKeyPath(*r.op, true);
    engine::FileEffects recorded;
    CHECK(engine::syncOperationFiles(r.project, *r.op, recorded));
    const auto expected = std::find_if(recorded.writes.begin(), recorded.writes.end(),
        [&](const auto& item) { return item.path == path; });
    REQUIRE(expected != recorded.writes.end());
    REQUIRE(!expected->bytes.empty());
    std::vector<qt::String> attempted;
    r.native.beforeNativeKeyWrite = [&](const qt::String& file) { attempted.push_back(file); };
    r.project.overlay[qt::cleanPath(path)] = "stale restart";
    CHECK(engine::syncOperationFiles(r.project, *r.op, r.native));
    REQUIRE(!attempted.empty());
    CHECK(attempted.back() == path); // material/equipment/object writes precede Restart
    CHECK_EQ(read(fs::path(path.u16())), expected->bytes);
    CHECK(r.project.overlay.find(qt::cleanPath(path)) == r.project.overlay.end());
    CHECK(r.native.writes.empty());
    {
        Handle denied(fs::path(path.u16()), FILE_SHARE_READ);
        CHECK(engine::syncOperationFiles(r.project, *r.op, r.native)); // status is ignored
        CHECK_EQ(read(fs::path(path.u16())), expected->bytes);
    }
}

TEST_CASE(native_whole_model_key_short_branch_and_ignored_flush_close)
{
    TaskKeyRig r;
    engine::GuiModel empty(nullptr, 3);
    const auto shortTarget = r.files.root / "short-model.KEY";
    write(shortTarget, "old");
    CHECK_EQ(engine::saveWholeModelKey(r.project, *empty.step,
        qt::String(shortTarget.u16string()), 0x80000000u, r.native), 0);
    CHECK_EQ(static_cast<std::uint32_t>(empty.step->u1744), 0x80000000u);
    CHECK(read(shortTarget).empty()); // 3D + no objects: short writer has no blocks/banner
    auto* model = engine::opModel(r.project, *r.op);
    REQUIRE(model && model->step);
    const auto target = r.files.root / "whole-flush.KEY";
    write(target, "old");
    {
        Handle lock(target, FILE_SHARE_READ | FILE_SHARE_WRITE);
        OVERLAPPED range{};
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                           0, MAXDWORD, 0, &range));
        CHECK_EQ(engine::saveWholeModelKey(r.project, *model->step,
            qt::String(target.u16string()), 0, r.native), 0);
        REQUIRE(UnlockFileEx(lock.value, 0, MAXDWORD, 0, &range));
        CHECK(read(target).empty());
    }
    CHECK(r.native.writes.empty());
    Handle closed(target, 0); // failed flush still closes the actual CRT handle
}

TEST_CASE(native_whole_model_deferred_db_branch_stays_guarded_before_target_effects)
{
    TaskKeyRig r;
    auto* model = engine::opModel(r.project, *r.op);
    REQUIRE(model && model->step);
    const auto target = r.files.root / "deferred-whole.KEY";
    write(target, "retained");
    model->step->u1b5a = 1;
    model->step->u1744 = 0x1234;
    bool refused = false;
    try {
        engine::saveWholeModelKey(r.project, *model->step,
            qt::String(target.u16string()), 0, r.native);
    } catch (const Error& error) {
        refused = std::string(error.what()).find("deferred DB-step temporary ownership") != std::string::npos;
    }
    model->step->u1b5a = 0;
    CHECK(refused);
    CHECK_EQ(model->step->u1744, 0x1234);
    CHECK_EQ(read(target), std::string("retained"));
    CHECK(r.native.writes.empty());
}

TEST_CASE(native_save_continues_after_interobject_properties_or_material_open_denial)
{
    for (const auto* relative : {"Operations/Task00002/InterObject.KEY",
                                 "Operations/Task00002/ObjProperties00001.KEY",
                                 "Materials/Material00001.KEY"}) {
        Rig r;
        const auto target = r.root / relative;
        REQUIRE(fs::exists(target));
        const auto prior = read(target);
        Handle denied(target, FILE_SHARE_READ);
        const auto result = r.convert();
        if (result.code != M2M_OK) std::printf("Native failure case %s: %s\n", relative, result.message.c_str());
        CHECK_EQ(result.code, M2M_OK);
        CHECK_EQ(read(target), prior);
        CHECK(read(r.mst).find("END OF MASTER FILE") != std::string::npos);
    }
}

TEST_CASE(native_interobject_denial_preserves_contact_updates_before_open)
{
    TaskKeyRig r;
    const auto target = r.files.task / "InterObject.KEY";
    const auto prior = read(target);
    r.op->icb.flag.fill(9);
    Handle denied(target, FILE_SHARE_READ);
    CHECK(engine::syncOperationFiles(r.project, *r.op, r.native));
    CHECK(std::any_of(r.op->icb.flag.begin(), r.op->icb.flag.end(),
                       [](auto flag) { return flag != 9; }));
    CHECK_EQ(read(target), prior);
    CHECK(r.native.writes.empty());
}

TEST_CASE(native_equipment_denial_and_failed_flush_are_not_replayed)
{
    TaskKeyRig r;
    auto* model = engine::opModel(r.project, *r.op);
    REQUIRE(model && model->step);
    auto* object = model->object(r.firstObjectSlot() + 1);
    REQUIRE(object != nullptr);
    const auto target = r.files.root / "equipment-test.KEY";
    const qt::String path(target.u16string());
    engine::FileEffects recorded;
    engine::saveEquipment(r.project, *model->step, object, path, recorded);
    REQUIRE(recorded.writes.size() == 1);
    write(target, "retained");
    {
        Handle denied(target, FILE_SHARE_READ);
        engine::saveEquipment(r.project, *model->step, object, path, r.native);
        CHECK_EQ(read(target), std::string("retained"));
    }
    engine::saveEquipment(r.project, *model->step, object, path, r.native);
    CHECK_EQ(read(target), recorded.writes[0].bytes);
    {
        Handle lock(target, FILE_SHARE_READ | FILE_SHARE_WRITE);
        OVERLAPPED range{};
        REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                           0, 4096, 0, &range));
        engine::saveEquipment(r.project, *model->step, object, path, r.native);
        REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
        CHECK(read(target).empty());
    }
    CHECK(r.native.writes.empty());
    Handle closed(target, 0);
}

TEST_CASE(material_lookup_failure_precedes_truncation)
{
    engine::GuiModel model(nullptr, 3);
    Rig r;
    const auto target = r.root / "absent-material.KEY";
    write(target, "retained");
    CHECK_EQ(def::key::write_material_key_file(*model.step, target.string().c_str(), 999,
                                              false, nullptr, nullptr), 3);
    CHECK_EQ(read(target), std::string("retained"));
}

TEST_CASE(native_task_key_output_alias_is_rejected_before_target_truncation)
{
    Rig r;
    const auto target = r.task / "InterObject.KEY";
    const auto prior = read(target);
    const auto projectBefore = read(r.project);
    CHECK_EQ(r.convert(target).code, M2M_E_ARG);
    CHECK_EQ(read(target), prior);
    CHECK_EQ(read(r.project), projectBefore);
}

TEST_CASE(native_operation_blobs_write_each_data_child_in_order_as_binary)
{
    Rig files;
    Project project;
    Ctx context;
    context.nativeOperationBlobs = true;
    const auto target = files.root / "blob-subdir" / "blob.KEY";
    const std::string first = "first\n";
    const std::string second("next\0\r\n", 7);
    xml::Document doc;
    REQUIRE(xml::setContent(doc, "<Key><Data>" + qt::toHex(qt::qCompress(first)) +
        "</Data><Data>" + qt::toHex(qt::qCompress(second)) + "</Data></Key>", nullptr, nullptr, nullptr));
    std::vector<std::string> seenBefore;
    context.beforeNativeOperationBlob = [&](const std::u16string& path) {
        seenBefore.push_back(read(fs::path(path)));
    };
    storeBlob(project, *doc.root, qt::String(target.u16string()), context);
    REQUIRE(seenBefore.size() == 2);
    CHECK(seenBefore[0].empty());
    CHECK_EQ(seenBefore[1], first);
    CHECK_EQ(read(target), second);
    CHECK(project.overlay.empty());
    REQUIRE(context.blobDirectoryAttempts.size() == 2);
    CHECK(context.blobDirectoryAttempts[0].succeeded && context.blobDirectoryAttempts[1].succeeded);
}

TEST_CASE(native_operation_blob_failed_open_does_not_supply_decoded_overlay)
{
    Rig files;
    Project project;
    Ctx context;
    context.nativeOperationBlobs = true;
    const auto target = files.root / "blob.KEY";
    write(target, "retained");
    xml::Document doc;
    REQUIRE(xml::setContent(doc, "<Key><Data>" + qt::toHex(qt::qCompress("new\n")) +
        "</Data><Data>" + qt::toHex(qt::qCompress("later\n")) + "</Data></Key>", nullptr, nullptr, nullptr));
    Handle denied(target, FILE_SHARE_READ);
    storeBlob(project, *doc.root, qt::String(target.u16string()), context);
    CHECK_EQ(read(target), std::string("retained"));
    CHECK(project.overlay.empty());
    CHECK_EQ(context.blobDirectoryAttempts.size(), std::size_t{2});
}

TEST_CASE(native_operation_blob_directory_failure_keeps_scanning_and_ignores_failed_flush)
{
    Rig files;
    Project project;
    Ctx context;
    context.nativeOperationBlobs = true;
    xml::Document doc;
    REQUIRE(xml::setContent(doc, "<Key><Data>" + qt::toHex(qt::qCompress("bytes")) +
        "</Data><Data>" + qt::toHex(qt::qCompress("last")) + "</Data></Key>", nullptr, nullptr, nullptr));
    const auto blocker = files.root / "blocked-directory";
    write(blocker, "file");
    storeBlob(project, *doc.root, qt::String((blocker / "blob.KEY").u16string()), context);
    REQUIRE(context.blobDirectoryAttempts.size() == 2);
    CHECK(!context.blobDirectoryAttempts[0].succeeded && !context.blobDirectoryAttempts[1].succeeded);
    CHECK_EQ(read(blocker), std::string("file"));
    const auto target = files.root / "flush-blob.KEY";
    write(target, "");
    Handle lock(target, FILE_SHARE_READ | FILE_SHARE_WRITE);
    OVERLAPPED range{};
    REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 4096, 0, &range));
    storeBlob(project, *doc.root, qt::String(target.u16string()), context);
    REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
    CHECK(read(target).empty());
    CHECK(project.overlay.empty());
}

namespace {
std::string itemXml(const std::string& name, int id, const std::string& children)
{
    return "<MoItem><Name>" + name + "</Name><Id>" + std::to_string(id) +
        "</Id>" + children + "</MoItem>";
}
std::string dataXml(const std::string& bytes)
{
    return "<Data>" + qt::toHex(qt::qCompress(bytes)) + "</Data>";
}
void parseItemXml(Project& p, Ctx& ctx, const std::string& bytes)
{
    xml::Document doc;
    REQUIRE(xml::setContent(doc, bytes, nullptr, nullptr, nullptr));
    parseMoItem(p, *doc.root, p.objects, ctx);
}
}

TEST_CASE(native_item_blob_retains_last_data_before_one_binary_write_and_row_append)
{
    Rig files;
    Project project;
    project.path = qt::String(files.project.u16string());
    Ctx context;
    context.nativeItemBlobs = true;
    const auto path = itemKeyPath(project, ItemKind::Object, 91);
    const std::string bytes("last\0\r\n", 7);
    unsigned attempts = 0;
    context.beforeNativeItemBlob = [&](const auto& target) {
        ++attempts;
        CHECK(target == path.u16());
        CHECK(project.objects.rows.empty());
    };
    parseItemXml(project, context, itemXml("item", 91,
        dataXml("first") + dataXml(bytes) +
        "<ObjectInfo ObjectType=\"2\" HasGeo=\"1\" HasMesh=\"0\" MatId=\"3 3 4\"/>"));
    CHECK_EQ(attempts, 1u);
    CHECK_EQ(read(fs::path(path.u16())), bytes);
    REQUIRE(project.objects.rows.size() == 1);
    CHECK_EQ(project.objects.rows[0]->objectType, 2);
    CHECK(project.objects.rows[0]->hasGeo && !project.objects.rows[0]->hasMesh);
    CHECK(project.overlay.empty());
    CHECK_EQ(context.blobDirectoryAttempts.size(), std::size_t{1});
}

TEST_CASE(native_item_blob_failed_open_omits_row_and_later_items_continue)
{
    Rig files;
    Project project;
    project.path = qt::String(files.project.u16string());
    Ctx context;
    context.nativeItemBlobs = true;
    const auto path = fs::path(itemKeyPath(project, ItemKind::Object, 91).u16());
    fs::create_directories(path.parent_path());
    write(path, "retained");
    Handle denied(path, FILE_SHARE_READ);
    xml::Document doc;
    REQUIRE(xml::setContent(doc, "<Object_List>" + itemXml("denied", 91, dataXml("new")) +
        itemXml("later", 92, dataXml("later")) + "</Object_List>", nullptr, nullptr, nullptr));
    parseItemList(project, *doc.root, project.objects, context);
    CHECK_EQ(read(path), std::string("retained"));
    REQUIRE(project.objects.rows.size() == 1);
    CHECK_EQ(project.objects.rows[0]->id, 92);
    CHECK_EQ(read(fs::path(itemKeyPath(project, ItemKind::Object, 92).u16())), std::string("later"));
    CHECK(project.overlay.empty());
}

TEST_CASE(native_item_without_data_appends_without_file_attempt_and_empty_name_skips)
{
    Rig files;
    Project project;
    project.path = qt::String(files.project.u16string());
    Ctx context;
    context.nativeItemBlobs = true;
    unsigned attempts = 0;
    context.beforeNativeItemBlob = [&](const auto&) { ++attempts; };
    parseItemXml(project, context, itemXml("external", 91, "<Path>ignored.KEY</Path>"));
    parseItemXml(project, context, itemXml("", 92, dataXml("ignored")));
    REQUIRE(project.objects.rows.size() == 1);
    CHECK_EQ(project.objects.rows[0]->id, 91);
    CHECK_EQ(attempts, 0u);
    CHECK(context.blobDirectoryAttempts.empty());
}

TEST_CASE(native_item_blob_ignored_flush_failure_still_appends_row)
{
    Rig files;
    Project project;
    project.path = qt::String(files.project.u16string());
    Ctx context;
    context.nativeItemBlobs = true;
    const auto path = fs::path(itemKeyPath(project, ItemKind::Object, 91).u16());
    fs::create_directories(path.parent_path());
    write(path, "");
    Handle lock(path, FILE_SHARE_READ | FILE_SHARE_WRITE);
    OVERLAPPED range{};
    REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, 4096, 0, &range));
    parseItemXml(project, context, itemXml("item", 91, dataXml("new")));
    REQUIRE(UnlockFileEx(lock.value, 0, 4096, 0, &range));
    CHECK(read(path).empty());
    REQUIRE(project.objects.rows.size() == 1);
    CHECK_EQ(project.objects.rows[0]->id, 91);
    CHECK(project.overlay.empty());
}

TEST_CASE(native_item_blob_directory_failure_omits_row_but_does_not_throw)
{
    Rig files;
    Project project;
    project.path = qt::String((files.root / "fresh-items" / "project.moproj").u16string());
    fs::create_directories(files.root / "fresh-items");
    Ctx context;
    context.nativeItemBlobs = true;
    const auto path = fs::path(itemKeyPath(project, ItemKind::Object, 91).u16());
    write(path.parent_path(), "blocker");
    parseItemXml(project, context, itemXml("item", 91, dataXml("new")));
    CHECK(project.objects.rows.empty());
    REQUIRE(context.blobDirectoryAttempts.size() == 1);
    CHECK(!context.blobDirectoryAttempts[0].succeeded);
    CHECK_EQ(read(path.parent_path()), std::string("blocker"));
}

TEST_CASE(native_full_save_writes_same_opened_remesh_state_after_mst)
{
    Rig files;
    const auto fallback = files.root / "LOC_DEN_CTRL.DAT";
    const auto remesh = files.root / "LOCAL_REMESH.DAT";
    REQUIRE(!fs::exists(remesh));
    write(fallback, "FILTER_DISTANCE 7\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nDONE\n");
    const auto result = files.convert();
    CHECK_EQ(result.code, M2M_OK);
    const auto bytes = read(remesh);
    CHECK(bytes.find("KEYWORD_DRIVEN") != std::string::npos);
    CHECK(bytes.find("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4") != std::string::npos);
    CHECK(bytes.find("FILTER_DISTANCE 7") != std::string::npos);
    CHECK(read(files.mst).find("END OF MASTER FILE") != std::string::npos);
    CHECK_EQ(read(fallback), std::string("FILTER_DISTANCE 7\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nDONE\n"));
}
TEST_CASE(native_full_save_remesh_tail_runs_after_project_or_mst_open_failure)
{
    for (bool failMst : {false, true}) {
        Rig files;
        const auto remesh = files.root / "LOCAL_REMESH.DAT";
        write(files.root / "LOC_DEN_CTRL.DAT", "FILTER_DISTANCE 7\nDONE\n");
        if (failMst) write(files.mst, "retained MST");
        const auto denied = failMst ? files.mst : files.project;
        const auto before = read(denied);
        Handle lock(denied, FILE_SHARE_READ);
        const auto result = files.convert();
        CHECK_EQ(result.code, M2M_E_WRITE);
        CHECK_EQ(read(denied), before);
        CHECK(read(remesh).find("KEYWORD_DRIVEN") != std::string::npos);
        if (!failMst) CHECK(read(files.mst).find("END OF MASTER FILE") != std::string::npos);
    }
}
TEST_CASE(native_full_save_disabled_remesh_removes_unmarked_primary_and_ignores_denial)
{
    for (bool denyRemoval : {false, true}) {
        Rig files;
        const auto remesh = files.root / "LOCAL_REMESH.DAT";
        REQUIRE(!fs::exists(files.root / "LOC_DEN_CTRL.DAT"));
        write(remesh, "DONE\n"); // no marker, fallback absent -> enabledfalse
        std::unique_ptr<Handle> lock;
        if (denyRemoval) lock = std::make_unique<Handle>(remesh, FILE_SHARE_READ | FILE_SHARE_WRITE);
        CHECK_EQ(files.convert().code, M2M_OK);
        CHECK(fs::exists(remesh) == denyRemoval);
        if (denyRemoval) CHECK_EQ(read(remesh), std::string("DONE\n"));
        CHECK(read(files.mst).find("END OF MASTER FILE") != std::string::npos);
    }
}
TEST_CASE(native_full_save_remesh_output_alias_is_rejected_before_project_save)
{
    Rig files;
    const auto remesh = files.root / "LOCAL_REMESH.DAT";
    const auto before = read(files.project);
    CHECK_EQ(files.convert(remesh).code, M2M_E_ARG);
    CHECK(!fs::exists(remesh));
    CHECK_EQ(read(files.project), before);
}
int main() { return testkit::run_all(); }
