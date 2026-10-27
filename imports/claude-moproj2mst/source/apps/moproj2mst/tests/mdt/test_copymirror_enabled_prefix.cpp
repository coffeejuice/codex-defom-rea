// CP448: static enabled-builder prefix, stopped before unknown local bounds.
// These private integrated occurrences do not establish original runtime parity
// or public admission of SfOprCopyMirror.
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"

#include <array>
#include <bit>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>

#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/model.hpp"
#include "m2m/p17.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/mirror_plane_owner.hpp"

namespace m2m::mst { void installLoaderSlots(); }

namespace {
using namespace m2m;
namespace fs = std::filesystem;

xml::NodePtr parse(const std::string& text)
{
    xml::Document doc;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(doc, text, &error, &line, &column));
    return doc.root;
}

struct Fixture {
    Project project;
    ProjectScope scope{project};
    Ctx ctx;
    Operation operation;
    mst::OpClass* cls = nullptr;
    engine::GuiModel* model = nullptr;
    fs::path directory;
    qt::String ini;

    Fixture()
    {
        static unsigned occurrence = 0;
        directory = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("cm448_" + std::to_string(GetCurrentProcessId()) + "_" +
             std::to_string(GetTickCount64()) + "_" + std::to_string(++occurrence)));
        REQUIRE(fs::create_directory(directory));
        project.path = qt::String((directory / "private.moproj").u16string());
        auto klass = mst::makeCopyMirror();
        mst::classInit(*klass);
        operation.project = &project;
        operation.classKey = klass->key();
        operation.props = klass->makeProps(project.unit);
        project.registry[klass->key()] = std::move(klass);
        setTaskDirRel(operation, qt::L("task"));
        cls = opBase(operation);
        REQUIRE(cls != nullptr);
        cls->current = &operation;
        mst::installLoaderSlots();
        operation.engineModel = std::make_shared<engine::GuiModel>(nullptr, 3);
        model = engine::opModel(project, operation);
        REQUIRE(model == operation.engineModel.get());
        auto* object = model->objectWrap(1, true, false);
        REQUIRE(object != nullptr);
        engine::viewerSync(*model, object, true);
        ini = qt::cleanPath(projectDir(project) + qt::kSep + taskDirRel(operation) +
            qt::kSep + qt::L("mirrormerge.ini"));
        setGate(true);
    }

    ~Fixture()
    {
        std::error_code error;
        fs::remove(directory, error); // only an empty owned scratch directory
    }

    void setGate(bool enabled)
    {
        setGateFlags(enabled ? 1 : 0, enabled ? 1 : 0, enabled ? 1 : 0);
    }

    void setGateFlags(int objectOn, int meshOn, int geoOn)
    {
        const auto display = parse("<SfDispOpt><DispFlagList ObjectOn='" +
            std::to_string(objectOn) + "' MeshOn='" + std::to_string(meshOn) +
            "' GeoOn='" + std::to_string(geoOn) + "'/></SfDispOpt>");
        engine::readViewerDisplayOptions(*model, 0, *display);
        const auto settings = engine::viewerDisplayOptionSnapshot(*model, 0);
        REQUIRE(settings.has_value());
        CHECK_EQ(settings->flags[1], objectOn);
        CHECK_EQ(settings->flags[2], meshOn);
        CHECK_EQ(settings->flags[3], geoOn);
    }

    // All file names, including any missed overlay lookup, resolve under scratch.
    bool post(const std::string& rows, int count = 1, const char* boundary = nullptr)
    {
        project.overlay[ini] = "MIRROR_MERGE 1 " + std::to_string(count) + "\n" + rows;
        engine::FileEffects effects;
        bool stopped = false;
        try { cls->postLoad(project, operation, ctx, effects); }
        catch (const Error& error) {
            CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
            REQUIRE(boundary != nullptr);
            CHECK(std::string(error.what()).find(boundary) != std::string::npos);
            stopped = true;
        }
        CHECK(effects.writes.empty());
        CHECK(effects.deletes.empty());
        CHECK(effects.mkdirs.empty());
        CHECK(model->viewerMirrorPlanes.empty());
        return stopped;
    }

    engine::detail::MirrorPlaneOwnerSnapshot snapshot()
    {
        const auto owner = engine::detail::mirrorPlaneOwnerSnapshot(*model, 1);
        REQUIRE(owner.has_value());
        return *owner;
    }
};

constexpr const char* boundsBoundary = "unknown builder-local bounds before 0x141249e40";
constexpr const char* firstRow = "0 40 41 3 0 0 2 0 0 0\n";

void checkUnwrittenCallerFields(const engine::detail::MirrorPlaneRecordSnapshot& record)
{
    CHECK(!record.a.has_value());
    CHECK(!record.e0.has_value());
    CHECK((record.c0 == std::array<double, 4>{}));
    CHECK((record.links == std::array<int, 2>{-1, -1}));
}
}

TEST_CASE(copymirror_enabled_builder_uses_exact_mixed_settings_gate)
{
    Fixture fixture;
    fixture.setGateFlags(1, 0, 0);
    CHECK(!fixture.post(firstRow));
    CHECK_EQ(fixture.snapshot().allocatedSlotCount, 0);
    fixture.setGateFlags(0, 1, 1);
    CHECK(!fixture.post(firstRow));
    CHECK_EQ(fixture.snapshot().allocatedSlotCount, 0);
    fixture.setGateFlags(1, 1, 0);
    REQUIRE(fixture.post(firstRow, 1, boundsBoundary));
    CHECK_EQ(fixture.snapshot().allocatedSlotCount, 1);
    fixture.setGateFlags(1, 0, 1);
    REQUIRE(fixture.post(firstRow, 1, boundsBoundary));
    CHECK_EQ(fixture.snapshot().allocatedSlotCount, 2);
}

TEST_CASE(copymirror_enabled_postload_retains_parent_zero_prefix_before_unknown_bounds)
{
    Fixture fixture;
    const auto before = fixture.snapshot();
    CHECK_EQ(before.allocatedSlotCount, 0);
    CHECK(!before.records[0].matrixDouble);
    CHECK(!before.records[0].matrixFloat);
    REQUIRE(fixture.post(std::string(firstRow) + "0 50 51 8 9 10 11 12 13 1\n", 2, boundsBoundary));
    const auto owner = fixture.snapshot();
    CHECK(owner.viewIdentity == before.viewIdentity);
    CHECK_EQ(owner.allocatedSlotCount, 1);
    CHECK(owner.links == before.links);
    const auto& record = owner.records[0];
    REQUIRE(record.matrixDouble && record.matrixFloat);
    // Nonunit n=(2,0,0), p=(3,0,0): no preconstruction normalization.
    const std::array<double, 16> expected{-7.0, -0.0, -0.0, 0.0,
        -0.0, 1.0, -0.0, 0.0, -0.0, -0.0, 1.0, 0.0, 24.0, 0.0, 0.0, 1.0};
    for (std::size_t i = 0; i < expected.size(); ++i) {
        CHECK(testkit::same_bits((*record.matrixDouble)[i], expected[i]));
        CHECK_EQ(std::bit_cast<std::uint32_t>((*record.matrixFloat)[i]),
            std::bit_cast<std::uint32_t>(static_cast<float>(expected[i])));
    }
    CHECK_EQ(record.mode, 0);
    CHECK(record.active); // parsed flag=0 cannot reach caller's final flag pass
    checkUnwrittenCallerFields(record);
    CHECK(!owner.records[1].matrixDouble && !owner.records[1].matrixFloat);
    CHECK_EQ(owner.records[1].mode, 1);
    CHECK(!owner.records[1].active);
    checkUnwrittenCallerFields(owner.records[1]);
}

TEST_CASE(copymirror_enabled_prefix_converts_each_double_matrix_element_to_float)
{
    Fixture fixture;
    REQUIRE(fixture.post("0 0 0 1.125 -3.5 0.2 0.1 2.25 -0.625 1\n", 1, boundsBoundary));
    const auto owner = fixture.snapshot();
    const auto& record = owner.records[0];
    REQUIRE(record.matrixDouble && record.matrixFloat);
    // Precomputed CP447 formula on the parser's binary32 operands, widened
    // before arithmetic. Literal bits also distinguish conversion rounding.
    const std::array<double, 16> expected{
        0x1.f5c28f570a3d7p-1, -0x1.cccccd4000000p-2, 0x1.0000004000000p-3, 0.0,
        -0x1.cccccd4000000p-2, -0x1.2400000000000p+3, 0x1.6800000000000p+1, 0.0,
        0x1.0000004000000p-3, 0x1.6800000000000p+1, 0x1.c000000000000p-3, 0.0,
        -0x1.93d70aa28f5c3p+0, -0x1.1bf3333350000p+5, 0x1.3b80000020000p+3, 1.0};
    const std::array<std::uint32_t, 16> floatBits{
        0x3f7ae148u, 0xbee66667u, 0x3e000000u, 0x0u,
        0xbee66667u, 0xc1120000u, 0x40340000u, 0x0u,
        0x3e000000u, 0x40340000u, 0x3e600000u, 0x0u,
        0xbfc9eb85u, 0xc20df99au, 0x411dc000u, 0x3f800000u};
    for (std::size_t i = 0; i < expected.size(); ++i) {
        CHECK(testkit::same_bits((*record.matrixDouble)[i], expected[i]));
        CHECK_EQ(std::bit_cast<std::uint32_t>((*record.matrixFloat)[i]), floatBits[i]);
    }
    checkUnwrittenCallerFields(record);
}

TEST_CASE(copymirror_enabled_prefix_appends_then_reuses_first_inactive_slot)
{
    Fixture fixture;
    REQUIRE(fixture.post(firstRow, 1, boundsBoundary));
    REQUIRE(fixture.post("0 0 0 4 0 0 2 0 0 1\n", 1, boundsBoundary));
    const auto appended = fixture.snapshot();
    CHECK_EQ(appended.allocatedSlotCount, 2);
    REQUIRE(appended.records[1].matrixDouble);
    CHECK_EQ((*appended.records[1].matrixDouble)[12], 32.0);

    fixture.setGate(false);
    CHECK(!fixture.post("67 0 0 8 9 10 11 12 13 0\n"));
    const auto disabled = fixture.snapshot();
    CHECK_EQ(disabled.allocatedSlotCount, 2);
    CHECK(!disabled.records[0].active);
    CHECK(disabled.records[1].active);
    fixture.setGate(true);
    REQUIRE(fixture.post("0 0 0 5 0 0 2 0 0 0\n", 1, boundsBoundary));
    const auto reused = fixture.snapshot();
    CHECK_EQ(reused.allocatedSlotCount, 2);
    CHECK(reused.viewIdentity == appended.viewIdentity);
    REQUIRE(reused.records[0].matrixDouble);
    CHECK_EQ((*reused.records[0].matrixDouble)[12], 40.0);
    CHECK_EQ(reused.records[0].mode, 0);
    CHECK(reused.records[0].active);
    // The stopped caller cannot overwrite the prior disabled-path fields.
    CHECK(reused.records[0].a == disabled.records[0].a);
    CHECK(reused.records[0].e0 == disabled.records[0].e0);
    CHECK(reused.records[0].c0 == disabled.records[0].c0);
    CHECK(reused.records[0].links == disabled.records[0].links);
    CHECK(reused.records[1].matrixDouble == appended.records[1].matrixDouble);
    CHECK(reused.records[1].matrixFloat == appended.records[1].matrixFloat);
}

TEST_CASE(copymirror_enabled_nonzero_parents_stop_before_allocation)
{
    Fixture fixture;
    for (const char* row : {"1 0 0 3 0 0 2 0 0 1\n", "-1 0 0 3 0 0 2 0 0 1\n"}) {
        REQUIRE(fixture.post(row, 1, "nonzero parent matrix composition"));
        const auto owner = fixture.snapshot();
        CHECK_EQ(owner.allocatedSlotCount, 0);
        CHECK(!owner.records[0].matrixDouble && !owner.records[0].matrixFloat);
        CHECK_EQ(owner.records[0].mode, 1);
        CHECK(!owner.records[0].active);
        checkUnwrittenCallerFields(owner.records[0]);
    }
}

TEST_CASE(copymirror_enabled_capacity_guard_precedes_inactive_slot_search)
{
    Fixture fixture;
    for (int i = 0; i < 128; ++i)
        REQUIRE(fixture.post(firstRow, 1, boundsBoundary));
    fixture.setGate(false);
    CHECK(!fixture.post("67 0 0 8 9 10 11 12 13 0\n"));
    const auto before = fixture.snapshot();
    CHECK_EQ(before.allocatedSlotCount, 128);
    CHECK(!before.records[0].active);
    fixture.setGate(true);
    REQUIRE(fixture.post("0 0 0 5 0 0 2 0 0 1\n", 1, "capacity-return continuation"));
    const auto after = fixture.snapshot();
    CHECK_EQ(after.allocatedSlotCount, 128);
    CHECK(!after.records[0].active);
    CHECK(after.records[0].matrixDouble == before.records[0].matrixDouble);
    CHECK(after.records[0].matrixFloat == before.records[0].matrixFloat);
    CHECK(after.records[0].a == before.records[0].a);
    CHECK(after.records[0].e0 == before.records[0].e0);
    CHECK(after.records[0].c0 == before.records[0].c0);
    CHECK(after.records[0].links == before.records[0].links);
}

int main() { return testkit::run_all(); }
