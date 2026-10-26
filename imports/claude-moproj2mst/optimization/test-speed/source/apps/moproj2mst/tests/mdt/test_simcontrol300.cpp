// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <limits>
#include "m2m/convert.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/mst/loader_l3.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/engine/engine_internal.hpp"

using namespace m2m;
namespace {
std::filesystem::path input() {
    return m2m_test::configuredPath("M2M_ORACLE_B") / "RS_55_Multiple_Operations_Example1_before" /
           "Multiple_Operations_Example1.moproj";
}
struct Fixture {
    Project p;
    Ctx ctx;
    ProjectScope scope;
    engine::FileEffects effects;
    Fixture(int mode) : scope(p) {
        const qt::String file{input().u16string()};
        const auto db = app::changeSuffix(file, qt::L(".DB"));
        initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
        p.originalSettings.moInputMode = mode;
        ctx.originalSettings = p.originalSettings;
        makeRegistry(p);
        REQUIRE(openProject(p, file, ctx) && p.openedOk);
        activateSubProject(p, 0, ctx);
        engine::openActiveOperation(p, ctx, effects);
    }
    Operation& op() { auto* op = operationById(p, 2); REQUIRE(op != nullptr); return *op; }
    def::model::Cls_1401e7b10& sc() {
        auto* model = engine::opModel(p, op()); REQUIRE(model != nullptr);
        auto* sc = model->simCtl(); REQUIRE(sc != nullptr); return *sc;
    }
};
}

TEST_CASE(default_mode_keeps_inherited_open_values)
{
    Fixture f(0);
    CHECK_EQ(f.sc().solmtd, 2);
    CHECK_EQ(f.sc().u2e8, 4);
    for (double v : f.sc().solmtd_p) CHECK(v == 0.0);
}

TEST_CASE(editor300_open_corrects_only_active_model)
{
    Fixture f(1);
    // Independent expectation: original callback0x140ed01b0 and retained RS55 Save row.
    CHECK_EQ(f.sc().solmtd, 2);
    CHECK(f.sc().solmtd_p[0] == 0.0);
    CHECK(f.sc().solmtd_p[1] == 1.0);
    CHECK(f.sc().solmtd_p[2] == 0.0);
    CHECK(f.sc().solmtd_p[3] == 1.0);
    auto* inactive = operationById(f.p, 1); REQUIRE(inactive != nullptr);
    auto* model = engine::opModel(f.p, *inactive); REQUIRE(model != nullptr);
    auto* other = model->simCtl(); REQUIRE(other != nullptr);
    other->solmtd = 2; other->u2e8 = 4;
    for (double& v : other->solmtd_p) v = 0.0;
    mst::g_loaderSlotsL3.formingSimControl300Open(f.p, f.op());
    for (double v : other->solmtd_p) CHECK(v == 0.0);
    // Creating the inactive same-class model above changes OpClass::current.
    // Restore the active editor before testing the actual Save dispatch.
    opBase(f.op())->current = &f.op();
    bool refused = false;
    try { engine::SaveEffects saved; engine::saveCurrent(f.p, f.ctx, saved); }
    catch (const NotPorted& e) { refused = e.src == 0x140eb5180; }
    CHECK(refused);
}

TEST_CASE(original_cg_parameter_boundaries)
{
    Fixture f(1);
    auto& sc = f.sc();
    auto fill = [&] { mst::g_loaderSlotsL3.formingSimControl300Open(f.p, f.op()); };
    sc.u2e8 = 0;
    fill();
    CHECK_EQ(sc.u2e8, 4);
    CHECK(sc.solmtd_p[0] == 2.0); CHECK(sc.solmtd_p[1] == 4.0);
    CHECK(sc.solmtd_p[2] == 1.0); CHECK(sc.solmtd_p[3] == 1.0);
    sc.solmtd_p[0] = -0.9; sc.solmtd_p[1] = 1.9; sc.solmtd_p[2] = -77.0; sc.solmtd_p[3] = 5.9;
    fill(); // original tests truncated integers but preserves accepted source doubles
    CHECK(sc.solmtd_p[0] == -0.9); CHECK(sc.solmtd_p[1] == 1.9);
    CHECK(sc.solmtd_p[2] == -77.0); CHECK(sc.solmtd_p[3] == 5.9);
    sc.solmtd_p[0] = 3.0; sc.solmtd_p[1] = 0.9; sc.solmtd_p[3] = 2.0;
    fill();
    CHECK(sc.solmtd_p[0] == 2.0); CHECK(sc.solmtd_p[1] == 1.0); CHECK(sc.solmtd_p[3] == 1.0);
    sc.solmtd_p[0] = (std::numeric_limits<double>::quiet_NaN)();
    sc.solmtd_p[1] = (std::numeric_limits<double>::infinity)();
    sc.solmtd_p[3] = 2147483648.0;
    fill();
    CHECK(sc.solmtd_p[0] == 2.0); CHECK(sc.solmtd_p[1] == 1.0); CHECK(sc.solmtd_p[3] == 1.0);
    sc.solmtd = 1; sc.u2e8 = 0;
    for (double& v : sc.solmtd_p) v = 0.0;
    fill();
    CHECK_EQ(sc.u2e8, 0);
    for (double v : sc.solmtd_p) CHECK(v == 0.0);
}

TEST_CASE(explicit_settings_adapter_does_not_approximate300_save)
{
    std::string mst;
    std::u16string output;
    SideBytes side;
    OriginalSettings settings; settings.moInputMode = 1;
    const auto result = convertToBytes(input().u16string(), {}, 0, 1252, mst, output, side, 0, {}, settings);
    CHECK_EQ(result.code, M2M_E_UNSUPPORTED);
    CHECK(result.message.find("0x140eb5180") != std::string::npos);
    CHECK(mst.empty()); CHECK(side.empty());
}

TEST_CASE(full_lifecycle_editor300_reaches_session_factory_and_exact_setup_guard)
{
    const auto directory = m2m_test::scratchOutput(m2m_test::scratchRoot() / "editor300-public-factory");
    std::filesystem::create_directories(directory);
    std::filesystem::copy(input().parent_path(), directory,
        std::filesystem::copy_options::recursive | std::filesystem::copy_options::overwrite_existing);
    std::string mst;
    std::u16string output;
    SideBytes side;
    OriginalSettings settings; settings.moInputMode = 1;
    const auto result = convertToBytes((directory / input().filename()).u16string(), {},
        M2M_FLAG_WRITE_SAVE, 1252, mst, output, side, 0, {}, settings);
    CHECK_EQ(result.code, M2M_E_UNSUPPORTED);
    CHECK(result.message.find("0x6504abb0") != std::string::npos);
    CHECK(mst.empty()); CHECK(side.empty());
}

int main() { return testkit::run_all(); }
