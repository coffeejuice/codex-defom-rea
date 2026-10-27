// CP448/CP456: enabled-builder prefix and exact-capacity normal return.
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

void fillCapacity(Fixture& fixture)
{
    for (int i = 0; i < 128; ++i)
        REQUIRE(fixture.post(firstRow, 1, boundsBoundary));
    CHECK_EQ(fixture.snapshot().allocatedSlotCount, 128);
}

void checkMatricesAndExtentUnchanged(const engine::detail::MirrorPlaneOwnerSnapshot& before,
    const engine::detail::MirrorPlaneOwnerSnapshot& after)
{
    CHECK(after.viewIdentity == before.viewIdentity);
    CHECK_EQ(after.allocatedSlotCount, before.allocatedSlotCount);
    for (std::size_t i = 0; i < before.records.size(); ++i) {
        CHECK(after.records[i].matrixDouble == before.records[i].matrixDouble);
        CHECK(after.records[i].matrixFloat == before.records[i].matrixFloat);
        CHECK_EQ(after.records[i].mode, before.records[i].mode);
        CHECK_EQ(after.records[i].c0[3], before.records[i].c0[3]);
    }
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
        REQUIRE(fixture.post(row, 1, "parent"));
        const auto owner = fixture.snapshot();
        CHECK_EQ(owner.allocatedSlotCount, 0);
        CHECK(!owner.records[0].matrixDouble && !owner.records[0].matrixFloat);
        CHECK_EQ(owner.records[0].mode, 1);
        CHECK(!owner.records[0].active);
        checkUnwrittenCallerFields(owner.records[0]);
    }
}

TEST_CASE(copymirror_enabled_known_parent_composes_reflection_as_r_times_parent)
{
    Fixture fixture;
    // Parent matrix P is a non-unit x reflection with scale -7 and x translation 24.
    REQUIRE(fixture.post("0 40 41 3 0 0 2 0 0 0\n", 1, boundsBoundary));
    REQUIRE(fixture.post("1 0 0 1 2 0 1 1 0 1\n", 1, boundsBoundary));
    const auto owner = fixture.snapshot();
    CHECK_EQ(owner.allocatedSlotCount, 2);
    REQUIRE(owner.records[0].matrixDouble && owner.records[0].matrixFloat &&
        owner.records[1].matrixDouble && owner.records[1].matrixFloat);
    const auto& composed = *owner.records[1].matrixDouble;
    // R from the child plane and parent P do not commute; the native storage
    // convention composes R*P, while P*R has different first-row/translation values.
    CHECK_EQ(composed[0], 7.0);
    CHECK_EQ(composed[1], -2.0);
    CHECK_EQ(composed[4], 14.0);
    CHECK_EQ(composed[5], -1.0);
    CHECK_EQ(composed[12], -18.0);
    CHECK_EQ(composed[13], 6.0);
    CHECK_EQ(owner.records[1].mode, 1); // parent.mode was zero
    CHECK(owner.records[1].active);
    checkUnwrittenCallerFields(owner.records[1]);
    for (std::size_t i = 0; i < composed.size(); ++i)
        CHECK_EQ(std::bit_cast<std::uint32_t>((*owner.records[1].matrixFloat)[i]),
            std::bit_cast<std::uint32_t>(static_cast<float>(composed[i])));
    REQUIRE(fixture.post("2 2 1 0 1 0 0 0 1 1\n", 1, boundsBoundary));
    const auto toggled = fixture.snapshot();
    CHECK_EQ(toggled.records[2].mode, 0); // parent mode was one
}

TEST_CASE(copymirror_known_parent_is_snapshotted_before_inactive_slot_reuse)
{
    Fixture fixture;
    REQUIRE(fixture.post("0 40 41 3 0 0 2 0 0 0\n", 1, boundsBoundary));
    fixture.setGate(false);
    CHECK(!fixture.post("9 0 0 0 1 0 0 0 0 0\n"));
    const auto disabled = fixture.snapshot();
    CHECK(!disabled.records[0].active);
    fixture.setGate(true);
    REQUIRE(fixture.post("1 0 0 1 2 0 1 1 0 1\n", 1, boundsBoundary));
    const auto owner = fixture.snapshot();
    CHECK_EQ(owner.allocatedSlotCount, 1);
    REQUIRE(owner.records[0].matrixDouble);
    CHECK_EQ((*owner.records[0].matrixDouble)[0], 7.0);
    CHECK_EQ((*owner.records[0].matrixDouble)[1], -2.0);
    CHECK_EQ((*owner.records[0].matrixDouble)[4], 14.0);
    CHECK_EQ((*owner.records[0].matrixDouble)[5], -1.0);
    CHECK_EQ((*owner.records[0].matrixDouble)[12], -18.0);
    CHECK_EQ((*owner.records[0].matrixDouble)[13], 6.0);
    CHECK_EQ(owner.records[0].mode, 1);
    CHECK(owner.records[0].active);
    CHECK(owner.records[0].a == disabled.records[0].a);
    CHECK(owner.records[0].e0 == disabled.records[0].e0);
    CHECK(owner.records[0].c0 == disabled.records[0].c0);
    CHECK(owner.records[0].links == disabled.records[0].links);
}

TEST_CASE(copymirror_enabled_capacity_return_precedes_reuse_and_fills_root_links)
{
    Fixture fixture;
    fillCapacity(fixture);
    fixture.setGate(false);
    CHECK(!fixture.post("67 0 0 8 9 10 11 12 13 0\n"));
    const auto before = fixture.snapshot();
    CHECK(!before.records[0].active);
    fixture.setGate(true);
    for (int call = 0; call < 3; ++call) {
        CHECK(!fixture.post("0 90 91 5 6 7 2 3 4 0\n"));
        const auto after = fixture.snapshot();
        CHECK((after.links == std::array<int, 2>{0, call == 0 ? -1 : 0}));
        checkMatricesAndExtentUnchanged(before, after);
        // A normal return reaches fixed row 0 and the deferred parsed disable.
        CHECK(!after.records[0].active);
        REQUIRE(after.records[0].a && after.records[0].e0);
        CHECK_EQ(*after.records[0].a, 0);
        CHECK((*after.records[0].e0 == std::array<double, 3>{5.0, 6.0, 7.0}));
        CHECK((after.records[0].c0 == std::array<double, 4>{2.0, 3.0, 4.0, 0.0}));
        // Only default record links are reachable here; this does not prove
        // preservation/clearing of nondefault record links.
        CHECK((after.records[0].links == std::array<int, 2>{-1, -1}));
        for (std::size_t i = 1; i < before.records.size(); ++i) {
            CHECK_EQ(after.records[i].active, before.records[i].active);
            CHECK(after.records[i].a == before.records[i].a);
            CHECK(after.records[i].e0 == before.records[i].e0);
            CHECK(after.records[i].c0 == before.records[i].c0);
            CHECK(after.records[i].links == before.records[i].links);
        }
    }
}

TEST_CASE(copymirror_known_parent_capacity_return_links_zero_to_parent)
{
    Fixture fixture;
    fillCapacity(fixture);
    const auto before = fixture.snapshot();
    CHECK((before.records[0].links == std::array<int, 2>{-1, -1}));
    for (int call = 0; call < 4; ++call) {
        // Parent 1 reaches the private actual-View owner and its parent-child
        // link writes; flag 1 verifies caller completion preserves enabled.
        CHECK(!fixture.post("1 0 0 1 2 0 1 1 0 1\n", 1));
        const auto after = fixture.snapshot();
        CHECK_EQ(after.allocatedSlotCount, 128);
        CHECK(after.links == before.links); // root links are untouched
        const std::array<int, 2> expected = call == 0
            ? std::array<int, 2>{0, -1}
            : std::array<int, 2>{0, 0};
        CHECK(after.records[0].links == expected);
        CHECK(after.records[0].matrixDouble == before.records[0].matrixDouble);
        CHECK(after.records[0].matrixFloat == before.records[0].matrixFloat);
        CHECK_EQ(after.records[0].mode, before.records[0].mode);
        CHECK(after.records[0].active);
        REQUIRE(after.records[0].a && after.records[0].e0);
        CHECK_EQ(*after.records[0].a, 1);
    }
    fixture.setGate(false);
    CHECK(!fixture.post("0 0 0 4 5 6 7 8 9 0\n"));
    const auto disabled = fixture.snapshot();
    CHECK(disabled.links == before.links); // deferred disabled clear is record-local
    CHECK((disabled.records[0].links == std::array<int, 2>{-1, -1}));
    CHECK(!disabled.records[0].active);
}

TEST_CASE(copymirror_capacity_caller_writes_fixed_input_rows_then_parsed_flags)
{
    Fixture fixture;
    fillCapacity(fixture);
    const auto before = fixture.snapshot();
    CHECK(!fixture.post("0 90 91 5 6 7 2 3 4 0\n0 92 93 8 9 10 11 12 13 1\n", 2));
    const auto after = fixture.snapshot();
    checkMatricesAndExtentUnchanged(before, after);
    CHECK((after.links == std::array<int, 2>{0, 0}));
    REQUIRE(after.records[0].e0 && after.records[1].e0);
    CHECK((*after.records[0].e0 == std::array<double, 3>{5.0, 6.0, 7.0}));
    CHECK((*after.records[1].e0 == std::array<double, 3>{8.0, 9.0, 10.0}));
    CHECK((after.records[1].c0 == std::array<double, 4>{11.0, 12.0, 13.0, 0.0}));
    REQUIRE(after.records[1].a);
    CHECK_EQ(*after.records[1].a, 0);
    CHECK(!after.records[0].active);
    CHECK(after.records[1].active);
    checkUnwrittenCallerFields(after.records[2]);
}

TEST_CASE(copymirror_capacity_later_parent_refusal_retains_first_pass_and_root)
{
    Fixture fixture;
    fillCapacity(fixture);
    const auto before = fixture.snapshot();
    REQUIRE(fixture.post("1 90 91 5 6 7 2 3 4 0\n-1 92 93 8 9 10 11 12 13 0\n",
        2, "parent"));
    const auto after = fixture.snapshot();
    checkMatricesAndExtentUnchanged(before, after);
    // The first known-parent insertion survives the later invalid-parent
    // refusal, which also suppresses the deferred disable and row 1 stores.
    CHECK(after.links == before.links);
    CHECK((after.records[0].links == std::array<int, 2>{0, -1}));
    CHECK(after.records[0].active);
    REQUIRE(after.records[0].a);
    CHECK_EQ(*after.records[0].a, 1);
    REQUIRE(after.records[0].e0);
    CHECK((*after.records[0].e0 == std::array<double, 3>{5.0, 6.0, 7.0}));
    CHECK((after.records[0].c0 == std::array<double, 4>{2.0, 3.0, 4.0, 0.0}));
    checkUnwrittenCallerFields(after.records[1]);
}

TEST_CASE(copymirror_capacity_return_keeps_fixed_row_out_of_range_refusal)
{
    Fixture fixture;
    fillCapacity(fixture);
    const auto before = fixture.snapshot();
    std::string rows;
    for (int i = 0; i < 129; ++i)
        rows += "0 90 91 " + std::to_string(i + 1) + " 6 7 2 3 4 0\n";
    REQUIRE(fixture.post(rows, 129, "first out-of-range fixed plane record"));
    const auto after = fixture.snapshot();
    checkMatricesAndExtentUnchanged(before, after);
    CHECK((after.links == std::array<int, 2>{0, 0}));
    for (std::size_t i = 0; i < after.records.size(); ++i) {
        // All 128 preceding stores survive; no deferred flag pass ran.
        CHECK(after.records[i].active);
        REQUIRE(after.records[i].a && after.records[i].e0);
        CHECK_EQ(*after.records[i].a, 0);
        CHECK_EQ((*after.records[i].e0)[0], static_cast<double>(i + 1));
        CHECK((after.records[i].c0 == std::array<double, 4>{2.0, 3.0, 4.0, 0.0}));
    }
}

int main() { return testkit::run_all(); }
