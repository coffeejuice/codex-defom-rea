// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <process.h>
#include <defdbio/db_query.hpp>
#include <defdbio/step/db_info_read.hpp>
#include "m2m/engine.hpp"
#include "m2m/engine/simcontrol300_lifecycle.hpp"
#include "m2m/mst/writer.hpp"
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/state_var_node_post.hpp>
#include <defmodel/llobj/cls_14017ef20.hpp>
#include <defmodel/llobj/ext.hpp>
#include <defmodel/bc/boundary_topology.hpp>
#include <defmodel/step/db_step.hpp>
#include <defmodel/step/db_directory_receipt.hpp>
#include <defmodel/sdctl/step_timer.hpp>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
using Variables = def::model::CowList<def::model::StateVarBase*>;
template<class T> T& field(def::model::LLOBJ_DATA* object, std::size_t offset)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(object) + offset);
}
struct Globals {
    const std::int32_t dimension = def::model::g_index_tables_dim;
    std::array<std::int32_t, 53> mar{};
    std::array<std::int32_t, 129> arrays{};
    const std::unique_ptr<def::model::StepTimer> nodeTimer =
        std::make_unique<def::model::StepTimer>(def::model::g_timer_read_obj_node_state_variable);
    const std::unique_ptr<def::model::StepTimer> arraysTimer =
        std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
    Globals()
    {
        std::copy_n(def::model::g_mar_index, mar.size(), mar.begin());
        std::copy_n(def::model::g_a_index, arrays.size(), arrays.begin());
    }
    ~Globals()
    {
        std::copy(mar.begin(), mar.end(), def::model::g_mar_index);
        std::copy(arrays.begin(), arrays.end(), def::model::g_a_index);
        def::model::g_index_tables_dim = dimension;
        def::model::g_timer_read_obj_node_state_variable = *nodeTimer;
        def::model::g_timer_read_arrays = *arraysTimer;
    }
    void timersUnchanged() const
    {
        CHECK(std::memcmp(&def::model::g_timer_read_obj_node_state_variable,
            nodeTimer.get(), sizeof(def::model::StepTimer)) == 0);
        CHECK(std::memcmp(&def::model::g_timer_read_arrays,
            arraysTimer.get(), sizeof(def::model::StepTimer)) == 0);
    }
};
struct Fixture {
    engine::GuiModel model{nullptr, 3};
    SimControl& sc = *model.simCtl();
    def::model::LLOBJ_DATA* object = model.objectWrap(1, true, false);
    def::model::Cls_14017ef20& nodes = field<def::model::Cls_14017ef20>(object, 0x8a0);
    explicit Fixture(engine::DbHandle* handle = nullptr) : model(handle, 3)
    {
        field<int>(object, 0x1ec8) = 2; field<int>(object, 0x1ee8) = 1;
        field<int>(object, 0x87c) = 1; field<int>(object, 0x880) = 1;
        field<double>(object, 0x1ed0) = 1.2345678;
        field<double>(object, 0x1ed8) = -0.0001; field<double>(object, 0x1ee0) = -0.0;
        nodes.n = 1; model.step->u15f8[0] = 3;
        model.step->u1b58 = 0; model.step->step_version = 6000;
        model.step->u26c4[11] = 1;
        sc.mdsobj[0] = 1; sc.mdsobj[1] = 0;
    }
    StoppingPage page() const
    {
        StoppingPage result{};
        result.widgets.labelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
        result.widgets.references[0].coordinates[0] = qt::L("old coordinate");
        result.mdsThreshold.text = qt::L("old threshold");
        result.rotationVector[0] = qt::L("old rotation");
        return result;
    }
};
struct Dispatch {
    const void* before;
    const void* after;
    std::int32_t dimensionBefore, dimensionAfter;
    std::int32_t count, key;
    const void* dimensions;
    std::int32_t versionBefore, versionAfter, timerBefore, timerAfter;
};
struct Trace {
    def::model::Cls_14017ef20* nodes;
    std::vector<Dispatch> calls;
};
Trace* activeTrace = nullptr;
std::uint64_t forward(std::uint64_t address, const std::uint64_t* args, int nargs)
{
    namespace lx = def::model::llobj::ext;
    std::array<std::uint64_t, 4> copied{};
    for (int i = 0; i < nargs && i < 4; ++i) copied[static_cast<std::size_t>(i)] = args[i];
    const bool record = activeTrace && address == 0x140329e60 && copied[0] == lx::u(activeTrace->nodes);
    const auto* variables = record ? reinterpret_cast<const Variables*>(activeTrace->nodes) : nullptr;
    const void* before = variables ? variables->body() : nullptr;
    const auto beforeDimension = def::model::g_index_tables_dim;
    const auto* step = record ? static_cast<const def::model::DB_STEP_DATA*>(activeTrace->nodes->u130) : nullptr;
    const auto beforeVersion = step ? step->step_version : 0;
    const auto beforeTimer = def::model::g_timer_read_obj_node_state_variable.count;
    struct RestoreHook {
        lx::Hook saved = lx::g_hook;
        RestoreHook() { lx::g_hook = nullptr; }
        ~RestoreHook() { lx::g_hook = saved; }
    } restore;
    const auto result = lx::call(address, copied[0], copied[1], copied[2], copied[3]);
    if (record) activeTrace->calls.push_back({before, variables->body(), beforeDimension,
        def::model::g_index_tables_dim, static_cast<std::int32_t>(copied[1]),
        static_cast<std::int32_t>(copied[3]), reinterpret_cast<const void*>(copied[2]),
        beforeVersion, step ? step->step_version : 0, beforeTimer,
        def::model::g_timer_read_obj_node_state_variable.count});
    return result;
}
struct ScopedTrace {
    explicit ScopedTrace(Trace& trace)
    {
        REQUIRE(activeTrace == nullptr); REQUIRE(def::model::llobj::ext::g_hook == nullptr);
        activeTrace = &trace; def::model::llobj::ext::g_hook = forward;
    }
    ~ScopedTrace() { def::model::llobj::ext::g_hook = nullptr; activeTrace = nullptr; }
};
struct Device final : mst::OutputDevice {
    bool live = true;
    std::int64_t write(std::string_view value) override { return static_cast<std::int64_t>(value.size()); }
    bool flush() override { return true; }
    void close() override { notifyAboutToClose(); live = false; }
    bool opened() const override { return live; }
};
struct Io final : Extra3DIo {
    Io() : Extra3DIo(1252) {}
    unsigned effects = 0;
    bool readDoubles(const qt::String&, std::span<double> output) override
    { std::fill(output.begin(), output.end(), 0.0); return false; }
    bool readInts(const qt::String&, std::span<int> output) override
    { std::fill(output.begin(), output.end(), 0); return false; }
    bool readSprings(std::vector<SpringRecord>& output) override { output.clear(); return false; }
    bool removeIfReadable(const qt::String&) override { ++effects; return true; }
    void nbcRemoveWarning() override { ++effects; }
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String&) override
    { ++effects; return std::make_unique<Device>(); }
};
void initialize(SimControl& sc)
{
    std::strcpy(sc.title, "OPEN TITLE"); std::strcpy(sc.oprnam, "OPEN OPERATION");
    sc.meshno = 30000; sc.unit = 1; sc.stype = 1;
    sc.nstart = 7; sc.nstep = 23; sc.stpinc = 3;
    sc.solmtd = 2; sc.u2e8 = 4;
    sc.solmtd_p[0] = 1.9; sc.solmtd_p[1] = 2.9;
    sc.solmtd_p[2] = -3.9; sc.solmtd_p[3] = 5.9;
    sc.tmax = 1.2345678; sc.dsmax = 2.3456789;
    sc.dttype.u58 = 0; sc.stpdef = 1;
    sc.tnow1 = 3.4567891; sc.tnow2 = 4.5678912;
    sc.htmthd = 3; sc.envmpr = 5.6789123; sc.envmpt = 6.7891234;
    sc.envtmp.value = 7.8912345; sc.vfactr.value = 8.9123456;
}

namespace receipts = def::model::directory_receipts;
namespace fs = std::filesystem;
struct DirectoryFixture {
    Project project;
    std::unique_ptr<ProjectScope> scope;
    fs::path directory, file;
    explicit DirectoryFixture(const char* tag)
    {
        directory = m2m_test::scratchRoot() /
            ("stopping_directory_" + std::to_string(_getpid()) + "_" + tag);
        fs::create_directories(directory);
        file = directory / "X.DB";
        const auto input = m2m_test::configuredPath("M2M_DB_FIXTURE") / "COG_P1_B1.DB";
        REQUIRE(fs::is_regular_file(input));
        fs::copy_file(input, file, fs::copy_options::overwrite_existing);
        scope = std::make_unique<ProjectScope>(project);
        auto* db = engine::projectDb(project);
        REQUIRE(db); REQUIRE(db->reader);
        REQUIRE(def::dbio::load_db_info(*db->reader, file.string().c_str(), false) == 0);
        const auto receipt = receipts::current(db, *db->reader);
        REQUIRE(receipt); REQUIRE(receipt->elementCount == std::size_t{32});
    }
    ~DirectoryFixture()
    {
        scope.reset();
        project.dataDb.reset();
        std::error_code error;
        fs::remove_all(directory, error);
    }
    engine::DbHandle* handle() { return project.dataDb.get(); }
    def::model::Cls_1401dbfe0& reader() { return *handle()->reader; }
    def::model::DbDirRecord* records()
    { return static_cast<def::model::DbDirRecord*>(reader().uad8); }
};
struct DirectoryCandidate : Fixture {
    explicit DirectoryCandidate(engine::DbHandle* handle) : Fixture(handle)
    {
        model.step->db = handle->reader.get();
        model.step->u740 = 0; model.step->u26c4[11] = 0;
        // Generic CP161 refusal cases remain before the new producer gate.
        model.step->u1b68 = 0; model.step->u1b6c = 0;
        model.step->u2b24 = 23; model.step->u2b28[11] = 29;
    }
    ~DirectoryCandidate()
    {
        // Borrowed opaque/expired regression identities must not reach teardown.
        model.db = nullptr; model.step->db = nullptr;
    }
};

void checkDirectoryOutcome(DirectoryCandidate& f, std::int32_t expectedVersion)
{
    auto& step = *f.model.step;
    const auto initialVersion = step.step_version;
    const auto headerLength = step.u1b68, arrayLength = step.u1b6c;
    const auto bodyCount = step.u2b24, storedLength = step.u2b28[11];
    const auto* header = step.u1b80; const auto* mar = step.u1b88; const auto* arrays = step.u1b78;
    const auto* companion = step.db; const auto* handle = f.model.db;
    const bool alreadyAllocated = f.nodes.u008 != nullptr;
    const std::array<double, 3> objectCoordinates{field<double>(f.object, 0x1ed0),
        field<double>(f.object, 0x1ed8), field<double>(f.object, 0x1ee0)};
    auto beforeArrays = std::make_unique<def::model::StepTimer>(def::model::g_timer_read_arrays);
    auto& timer = def::model::g_timer_read_obj_node_state_variable;
    timer.count = 7;
    def::model::g_index_tables_dim = 0;
    auto& variables = *reinterpret_cast<Variables*>(&f.nodes);
    const auto shared = variables; const auto* oldBody = shared.body();
    Trace trace{&f.nodes, {}};
    for (int invocation = 0; invocation < 2; ++invocation) {
        auto page = f.page(); // Fresh delivered page; do not bypass spin suppression.
        StoppingFillReport report;
        { ScopedTrace scope(trace); report = fillStopping3DInto(page, f.sc, f.model, NumericLocale::KnownC); }
        CHECK(!report.complete()); CHECK(report.issue == StoppingFillIssue::CoordinateSync);
        CHECK(report.stage == StoppingFillStage::NodeCallback);
        CHECK_EQ(report.coordinateGetterCalls, 1u); CHECK_EQ(report.stateVariableDispatches, 1u);
        CHECK_EQ(report.coordinateAllocationAttempts, invocation == 0 && !alreadyAllocated ? 1u : 0u);
        REQUIRE(trace.calls.size() == static_cast<std::size_t>(invocation + 1));
        const auto& call = trace.calls.back();
        CHECK_EQ(call.count, 1); CHECK_EQ(call.key, 0); CHECK(call.dimensions == step.u15f8);
        CHECK(call.before == (invocation == 0 ? oldBody : variables.body()));
        CHECK(call.after == variables.body()); CHECK(call.after != oldBody);
        CHECK_EQ(call.dimensionBefore, invocation == 0 ? 0 : 3);
        CHECK_EQ(call.dimensionAfter, call.dimensionBefore);
        CHECK_EQ(call.versionBefore, invocation == 0 ? initialVersion : expectedVersion);
        CHECK_EQ(call.versionAfter, call.versionBefore);
        CHECK_EQ(call.timerBefore, invocation == 0 ? 7 : 2); CHECK_EQ(call.timerAfter, call.timerBefore);
        CHECK_EQ(step.step_version, expectedVersion); CHECK_EQ(step.u26c4[11], 0);
        CHECK_EQ(step.u1b68, headerLength); CHECK_EQ(step.u1b6c, arrayLength);
        CHECK_EQ(step.u2b24, bodyCount); CHECK_EQ(step.u2b28[11], storedLength);
        CHECK(step.u1b80 == header); CHECK(step.u1b88 == mar); CHECK(step.u1b78 == arrays);
        CHECK(step.db == companion); CHECK(f.model.db == handle); CHECK(f.nodes.u130 == &step);
        REQUIRE(f.nodes.u008); CHECK_EQ(f.nodes.u008[0], 0.0);
        CHECK_EQ(f.nodes.u008[1], 0.0); CHECK_EQ(f.nodes.u008[2], 0.0);
        for (std::size_t axis = 0; axis < objectCoordinates.size(); ++axis)
            CHECK_BITS(field<double>(f.object, 0x1ed0 + axis * sizeof(double)), objectCoordinates[axis]);
        CHECK_EQ(timer.count, 2); CHECK(std::string(timer.labels[0]) == "Start");
        CHECK(std::string(timer.labels[1]) == "ReadSingleAArrayFromDB");
        CHECK(timer.subs[0] == nullptr); CHECK(timer.subs[1] == nullptr);
        CHECK(std::memcmp(&def::model::g_timer_read_arrays, beforeArrays.get(), sizeof *beforeArrays) == 0);
        CHECK(page.widgets.references[0].coordinates[0] == qt::L("old coordinate"));
        CHECK(page.mdsThreshold.text == qt::L("old threshold"));
        CHECK(page.rotationVector[0] == qt::L("old rotation"));
        CHECK(!InitialEditorSession::completeForPublicSave);
    }
    CHECK_EQ(variables.use_count(), 1); CHECK_EQ(shared.use_count(), 1);
}
}

TEST_CASE(stopping_directory_query_reads_first_last_high_versions_and_refuses_after_version_effect)
{
    Globals globals;
    DirectoryFixture fixture("high");
    for (const auto index : {0, 31})
    for (const auto oldVersion : {6000, 11000})
    for (const auto recordVersion : {6000, 11000, (std::numeric_limits<int>::max)()}) {
        // Controlled body inputs use the real, ready loader allocation.
        auto& record = fixture.records()[index];
        record.hsize = -101; record.mar_size = 103; record.a_size = -107;
        record.version = recordVersion;
        DirectoryCandidate candidate(fixture.handle());
        candidate.model.step->u740 = index; candidate.model.step->step_version = oldVersion;
        checkDirectoryOutcome(candidate, recordVersion);
        const auto current = receipts::current(fixture.handle(), fixture.reader());
        REQUIRE(current); CHECK(receipts::isCurrent(*current));
        CHECK_EQ(current->elementCount, std::size_t{32});
    }
}

TEST_CASE(stopping_directory_query_publishes_fresh_a_then_refuses_before_selected_length)
{
    Globals globals;
    for (const auto recordVersion : {6000, 11000, (std::numeric_limits<int>::max)()})
    for (const auto bodyCount : {0, 1, 4})
    for (const auto selectedLength : {0, 7}) {
        DirectoryFixture fixture("a_publish");
        fixture.records()[0].version = recordVersion;
        DirectoryCandidate candidate(fixture.handle());
        auto& step = *candidate.model.step;
        step.u1b68 = 17; step.u1b6c = 19; step.u2b24 = bodyCount;
        step.u2b28[11] = selectedLength;
        step.u1b74 = 23;
        const auto oldVersion = step.step_version;
        const auto* const oldHeader = step.u1b80;
        const auto* const oldMar = step.u1b88;
        const auto bodyLength = step.u2b24;
        const auto selectedArrayLength = step.u2b28[11];
        const std::array<double, 3> objectCoordinates{field<double>(candidate.object, 0x1ed0),
            field<double>(candidate.object, 0x1ed8), field<double>(candidate.object, 0x1ee0)};
        auto page = candidate.page();
        const auto arraysTimer = def::model::g_timer_read_arrays;
        Trace trace{&candidate.nodes, {}};
        StoppingFillReport report;
        { ScopedTrace scope(trace); report = fillStopping3DInto(page, candidate.sc, candidate.model, NumericLocale::KnownC); }
        CHECK(!report.complete()); CHECK(report.issue == StoppingFillIssue::CoordinateSync);
        CHECK_EQ(report.coordinateGetterCalls, 1u); CHECK_EQ(report.stateVariableDispatches, 1u);
        REQUIRE(trace.calls.size() == 1);
        CHECK_EQ(trace.calls[0].versionBefore, oldVersion); CHECK_EQ(trace.calls[0].versionAfter, oldVersion);
        CHECK_EQ(step.step_version, recordVersion > oldVersion ? recordVersion : oldVersion);
        REQUIRE(step.u1b78);
        const auto* const publishedValues = static_cast<const double*>(step.u1b78);
        const auto count = static_cast<std::size_t>(bodyCount + 2);
        for (std::size_t i = 0; i < count; ++i) CHECK_EQ(publishedValues[i], 0.0);
        auto* const published = step.u1b78;
        CHECK_EQ(step.u1b74, 23); CHECK_EQ(step.u1b68, 17); CHECK_EQ(step.u1b6c, 19);
        CHECK_EQ(step.u2b24, bodyLength); CHECK_EQ(step.u2b28[11], selectedArrayLength);
        CHECK(step.u1b80 == oldHeader); CHECK(step.u1b88 == oldMar);
        CHECK_EQ(step.u26c4[11], 0);
        CHECK(std::memcmp(&def::model::g_timer_read_arrays, &arraysTimer, sizeof arraysTimer) == 0);
        auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        CHECK_EQ(nodeTimer.count, 2); CHECK(std::string(nodeTimer.labels[0]) == "Start");
        CHECK(std::string(nodeTimer.labels[1]) == "ReadSingleAArrayFromDB");
        CHECK(nodeTimer.subs[0] == nullptr); CHECK(nodeTimer.subs[1] == nullptr);
        CHECK(page.widgets.references[0].coordinates[0] == qt::L("old coordinate"));
        CHECK(page.mdsThreshold.text == qt::L("old threshold"));
        for (std::size_t axis = 0; axis < objectCoordinates.size(); ++axis)
            CHECK_BITS(field<double>(candidate.object, 0x1ed0 + axis * sizeof(double)), objectCoordinates[axis]);

        auto retryPage = candidate.page();
        StoppingFillReport retry;
        { ScopedTrace scope(trace); retry = fillStopping3DInto(retryPage, candidate.sc, candidate.model, NumericLocale::KnownC); }
        CHECK(!retry.complete()); CHECK(retry.issue == StoppingFillIssue::CoordinateSync);
        CHECK_EQ(retry.coordinateGetterCalls, 1u); CHECK_EQ(retry.stateVariableDispatches, 1u);
        CHECK_EQ(trace.calls.size(), 2u);
        CHECK(step.u1b78 == published);
        for (std::size_t i = 0; i < count; ++i) CHECK_EQ(publishedValues[i], 0.0);
        CHECK_EQ(step.u26c4[11], 0);
        CHECK_EQ(nodeTimer.count, 2); CHECK(std::string(nodeTimer.labels[0]) == "Start");
        CHECK(std::string(nodeTimer.labels[1]) == "ReadSingleAArrayFromDB");
        CHECK(nodeTimer.subs[0] == nullptr); CHECK(nodeTimer.subs[1] == nullptr);
    }
}

TEST_CASE(stopping_directory_query_keeps_a_guarded_for_invalid_producer_inputs)
{
    Globals globals;
    for (int mode = 0; mode < 10; ++mode) {
        DirectoryFixture fixture("a_guard");
        fixture.records()[0].version = 11000;
        DirectoryCandidate candidate(fixture.handle());
        auto& step = *candidate.model.step;
        step.u1b68 = 17; step.u1b6c = 19; step.u2b24 = 0;
        if (mode == 0) step.u1b68 = 0;
        if (mode == 1) step.u1b6c = 0;
        if (mode == 2) step.u1b68 = -1;
        if (mode == 3) step.u1b6c = -1;
        if (mode == 4) step.u2b24 = -1;
        if (mode == 5) step.u2b24 = -2;
        if (mode == 6) step.u2b24 = (std::numeric_limits<std::int32_t>::min)();
        if (mode == 7) step.u2b24 = (std::numeric_limits<std::int32_t>::max)() - 1;
        if (mode == 8) step.u2b24 = (std::numeric_limits<std::int32_t>::max)();
        auto* const sentinel = mode == 9 ? def::model::bc::calloc_doubles(2) : nullptr;
        if (sentinel) step.u1b78 = sentinel;
        auto page = candidate.page();
        const auto arraysTimer = def::model::g_timer_read_arrays;
        const auto report = fillStopping3DInto(page, candidate.sc, candidate.model, NumericLocale::KnownC);
        CHECK(!report.complete()); CHECK(report.issue == StoppingFillIssue::CoordinateSync);
        CHECK_EQ(report.coordinateGetterCalls, 1u); CHECK_EQ(report.stateVariableDispatches, 1u);
        CHECK_EQ(step.step_version, 11000);
        CHECK(step.u1b78 == sentinel);
        CHECK_EQ(step.u26c4[11], 0);
        CHECK(std::memcmp(&def::model::g_timer_read_arrays, &arraysTimer, sizeof arraysTimer) == 0);
        const auto& nodeTimer = def::model::g_timer_read_obj_node_state_variable;
        CHECK_EQ(nodeTimer.count, 2); CHECK(std::string(nodeTimer.labels[0]) == "Start");
        CHECK(std::string(nodeTimer.labels[1]) == "ReadSingleAArrayFromDB");
        CHECK(nodeTimer.subs[0] == nullptr); CHECK(nodeTimer.subs[1] == nullptr);
    }
}

TEST_CASE(stopping_directory_query_low_version_preflight_cannot_open_a_later_fast_path)
{
    Globals globals;
    DirectoryFixture fixture("low");
    for (const auto oldVersion : {6000, 11000})
    for (const auto recordVersion : {5999, 0, -1, (std::numeric_limits<int>::min)()}) {
        fixture.records()[0].version = recordVersion;
        DirectoryCandidate candidate(fixture.handle()); candidate.model.step->step_version = oldVersion;
        checkDirectoryOutcome(candidate, oldVersion);
    }
}

TEST_CASE(stopping_directory_query_uses_signed_index_and_physical_extent)
{
    Globals globals;
    DirectoryFixture fixture("bounds");
    fixture.reader().uac8 = 33;
    for (const auto index : {-1, (std::numeric_limits<int>::min)(), 32, (std::numeric_limits<int>::max)()}) {
        DirectoryCandidate candidate(fixture.handle()); candidate.model.step->u740 = index;
        checkDirectoryOutcome(candidate, 6000);
    }
    fixture.reader().uac8 = 1;
    fixture.records()[31].version = 11000;
    DirectoryCandidate candidate(fixture.handle()); candidate.model.step->u740 = 31;
    checkDirectoryOutcome(candidate, 11000);
}

TEST_CASE(stopping_directory_query_requires_actual_scope_owner_reader_and_ready_allocation)
{
    Globals globals;
    for (int mode = 0; mode < 14; ++mode) {
        DirectoryFixture fixture("owners");
        fixture.records()[0].version = 11000;
        DirectoryCandidate candidate(fixture.handle());
        std::shared_ptr<engine::DbHandle> outsideLease;
        struct RestorePointer {
            def::model::Cls_1401dbfe0& reader; void* pointer;
            explicit RestorePointer(def::model::Cls_1401dbfe0& value) : reader(value), pointer(value.uad8) {}
            ~RestorePointer() { reader.uad8 = pointer; }
        };
        std::unique_ptr<RestorePointer> restorePointer;
        switch (mode) {
        case 0: fixture.scope.reset(); break;
        case 1: outsideLease = fixture.project.dataDb; fixture.project.dataDb.reset(); break;
        case 2: fixture.project.dataDb.reset(); break; // Actual owner and reader expired.
        case 3:
            outsideLease = fixture.project.dataDb;
            fixture.project.dataDb = std::make_shared<engine::DbHandle>(); break;
        case 4: candidate.model.db = reinterpret_cast<engine::DbHandle*>(std::uintptr_t{1}); break;
        case 5: candidate.model.step->db = reinterpret_cast<def::model::Cls_1401dbfe0*>(std::uintptr_t{1}); break;
        case 6:
            fixture.handle()->reader = std::make_unique<def::model::Cls_1401dbfe0>();
            candidate.model.step->db = fixture.handle()->reader.get(); break;
        case 7:
        case 8:
            restorePointer = std::make_unique<RestorePointer>(fixture.reader());
            fixture.reader().uad8 = mode == 7 ? nullptr : reinterpret_cast<void*>(std::uintptr_t{1}); break;
        case 9: def::model::db_info_reset(fixture.reader()); break; // Actual retirement.
        case 10: candidate.model.step->db = nullptr; break;
        case 11:
            // Registered real owner, never loaded: no ready allocation exists.
            fixture.project.dataDb = std::make_shared<engine::DbHandle>();
            candidate.model.db = fixture.handle(); candidate.model.step->db = fixture.handle()->reader.get(); break;
        case 12: fixture.handle()->reader.reset(); break;
        case 13:
            outsideLease = std::make_shared<engine::DbHandle>();
            candidate.model.step->db = outsideLease->reader.get(); break;
        }
        checkDirectoryOutcome(candidate, 6000);
        if (mode == 1 || mode == 2) CHECK(!fixture.project.dataDb); // No manufactured owner.
    }
}

TEST_CASE(stopping_directory_query_refreshes_receipt_after_full_reload_and_real_multipart)
{
    Globals globals;
    DirectoryFixture fixture("replacement");
    const auto first = receipts::current(fixture.handle(), fixture.reader()); REQUIRE(first);
    DirectoryCandidate candidate(fixture.handle()); candidate.model.step->u740 = 31;
    fixture.records()[31].version = 11000;
    checkDirectoryOutcome(candidate, 11000);
    REQUIRE(def::dbio::load_db_info(fixture.reader(), fixture.file.string().c_str(), false) == 0);
    const auto second = receipts::current(fixture.handle(), fixture.reader()); REQUIRE(second);
    CHECK(first->allocationGeneration != second->allocationGeneration); CHECK(!receipts::isCurrent(*first));
    fixture.records()[31].version = 12000;
    checkDirectoryOutcome(candidate, 12000);
    fs::copy_file(fixture.file, fixture.directory / "X25.DB", fs::copy_options::overwrite_existing);
    fixture.records()[0].step = 26;
    REQUIRE(def::dbio::query::setup_part_chain(fixture.reader()) == 0);
    const auto third = receipts::current(fixture.handle(), fixture.reader()); REQUIRE(third);
    CHECK_EQ(third->elementCount, std::size_t{64}); CHECK(!receipts::isCurrent(*second));
    CHECK_EQ(first->ownerGeneration, third->ownerGeneration);
    candidate.model.step->u740 = 63; candidate.model.step->step_version = 6000;
    checkDirectoryOutcome(candidate, 12000);
}

TEST_CASE(stopping_directory_query_retains_actual_old_extent_after_header_only_and_failed_reload)
{
    Globals globals;
    DirectoryFixture fixture("failed_reload");
    const auto original = receipts::current(fixture.handle(), fixture.reader()); REQUIRE(original);
    REQUIRE(def::dbio::load_db_info(fixture.reader(), fixture.file.string().c_str(), true) == 0);
    CHECK(receipts::isCurrent(*original));
    {
        std::fstream file(fixture.file, std::ios::binary | std::ios::in | std::ios::out);
        REQUIRE(file.is_open()); std::array<char, 4> word{};
        const std::array<char, 4> count32{0, 0, 0, 0x20}, chunk4{0, 0, 0, 4};
        const std::array<char, 4> count33{0, 0, 0, 0x21}, chunk0{0, 0, 0, 0};
        file.seekg(632); file.read(word.data(), 4); REQUIRE(file.good()); REQUIRE(word == count32);
        file.seekg(1024); file.read(word.data(), 4); REQUIRE(file.good()); REQUIRE(word == chunk4);
        file.seekp(632); file.write(count33.data(), 4); REQUIRE(file.good());
        file.seekp(1024); file.write(chunk0.data(), 4); file.flush(); REQUIRE(file.good());
        file.close(); REQUIRE(!file.fail());
    }
    REQUIRE(def::dbio::load_db_info(fixture.reader(), fixture.file.string().c_str(), false) == 3);
    CHECK_EQ(fixture.reader().uac8, 33); CHECK(receipts::isCurrent(*original));
    DirectoryCandidate candidate(fixture.handle()); candidate.model.step->u740 = 32;
    checkDirectoryOutcome(candidate, 6000);
    fixture.records()[31].version = 11000; candidate.model.step->u740 = 31;
    checkDirectoryOutcome(candidate, 11000);
    const auto retained = receipts::current(fixture.handle(), fixture.reader()); REQUIRE(retained);
    CHECK_EQ(retained->elementCount, std::size_t{32});
    CHECK_EQ(retained->allocationGeneration, original->allocationGeneration);
}

TEST_CASE(stopping_cached_reader_preserves_native_getter_counts_and_stored_text)
{
    Globals globals;
    for (const int version : {6000, 11000, (std::numeric_limits<int>::max)()})
    for (const int cache : {1, -1})
    for (const bool allocated : {false, true})
    for (const auto selected : std::array<std::array<int, 2>, 3>{{{1, 0}, {0, 1}, {1, 1}}}) {
        Fixture f;
        f.model.step->step_version = version; f.model.step->u26c4[11] = cache;
        REQUIRE(f.model.step->u1b88 == nullptr); REQUIRE(f.model.step->u1b78 == nullptr);
        if (allocated) {
            f.nodes.u008 = def::model::bc::calloc_doubles(3); REQUIRE(f.nodes.u008);
            f.nodes.u008[0] = 12.345678; f.nodes.u008[1] = -20.25; f.nodes.u008[2] = 30.5;
        }
        auto& variables = *reinterpret_cast<Variables*>(&f.nodes);
        const auto shared = variables; const auto* oldBody = shared.body();
        f.sc.mdsobj[0] = selected[0]; f.sc.mdsobj[1] = selected[1];
        auto page = f.page();
        def::model::g_index_tables_dim = 0;
        Trace trace{&f.nodes, {}};
        StoppingFillReport report;
        { ScopedTrace scope(trace); report = fillStopping3DInto(page, f.sc, f.model, NumericLocale::KnownC); }
        REQUIRE(report.complete()); REQUIRE(f.nodes.u008);
        const std::size_t count = selected[0] ? (selected[1] ? 20 : 8) : 4;
        CHECK_EQ(report.coordinateGetterCalls, count); CHECK_EQ(report.stateVariableDispatches, count);
        CHECK_EQ(report.coordinateAllocationAttempts, allocated ? 0u : 1u);
        REQUIRE(trace.calls.size() == count);
        CHECK(trace.calls[0].before == oldBody); CHECK(trace.calls[0].after != oldBody);
        CHECK_EQ(trace.calls[0].dimensionBefore, 0); CHECK_EQ(trace.calls[0].dimensionAfter, 0);
        for (std::size_t i = 0; i < trace.calls.size(); ++i) {
            CHECK_EQ(trace.calls[i].count, 1); CHECK_EQ(trace.calls[i].key, 0);
            CHECK(trace.calls[i].dimensions == f.model.step->u15f8);
            if (i) {
                CHECK(trace.calls[i].before == variables.body()); CHECK(trace.calls[i].after == variables.body());
                CHECK_EQ(trace.calls[i].dimensionBefore, 3); CHECK_EQ(trace.calls[i].dimensionAfter, 3);
            }
        }
        CHECK_EQ(variables.use_count(), 1); CHECK_EQ(shared.use_count(), 1);
        for (int side = 0; side < 2; ++side) if (selected[side]) {
            CHECK_EQ(page.widgets.references[side].selectedObject, 1);
            CHECK_EQ(page.widgets.references[side].node, 1);
            CHECK(page.widgets.references[side].coordinates[0] == qt::L("1.235"));
            CHECK(page.widgets.references[side].coordinates[1] == qt::L("-0.000"));
        }
        CHECK(f.nodes.u130 == f.model.step.get()); CHECK_EQ(f.model.step->u26c4[11], cache);
        CHECK(f.nodes.u008[0] == (allocated ? 12.345678 : 0.0));
        CHECK(field<double>(f.object, 0x1ed0) == 1.2345678);
        const auto repeated = fillStopping3DInto(page, f.sc, f.model, NumericLocale::KnownC);
        REQUIRE(repeated.complete()); CHECK_EQ(repeated.coordinateGetterCalls, 0u);
    }
    globals.timersUnchanged();
}

TEST_CASE(stopping_cached_reader_preserves_current_tables_and_miss_partial_prefix)
{
    Globals globals;
    for (const int initialDimension : {0, 2, 3})
    for (const int profile : {0, 1, 2, 3, 4, 5, 6}) {
        Fixture f;
        def::model::g_index_tables_dim = initialDimension;
        def::model::g_mar_index[0] = -777; def::model::g_a_index[0] = -888;
        def::model::g_mar_index[3] = profile == 2 ? -1 : profile == 3 ? 140 : 139;
        def::model::g_a_index[128] = profile == 4 ? 11 : profile == 5 ? 0 : profile == 6 ? -1 : 12;
        f.model.step->u26c4[11] = profile == 1 ? 0 : -1;
        const bool supported = profile != 1 && (initialDimension != 3 || profile == 0);
        auto& variables = *reinterpret_cast<Variables*>(&f.nodes); const auto shared = variables;
        auto page = f.page();
        const auto report = fillStopping3DInto(page, f.sc, f.model, NumericLocale::KnownC);
        CHECK(report.complete() == supported); REQUIRE(f.nodes.u008);
        CHECK(variables.body() != shared.body());
        CHECK_EQ(def::model::g_index_tables_dim, 3);
        if (initialDimension == 3) {
            CHECK_EQ(def::model::g_mar_index[0], -777); CHECK_EQ(def::model::g_a_index[0], -888);
        } else {
            CHECK(def::model::g_mar_index[0] != -777); CHECK(def::model::g_a_index[0] != -888);
        }
        if (!supported) {
            CHECK(report.issue == StoppingFillIssue::CoordinateSync);
            CHECK(report.stage == StoppingFillStage::NodeCallback);
            CHECK_EQ(report.coordinateGetterCalls, 1u); CHECK_EQ(report.stateVariableDispatches, 1u);
            CHECK_EQ(report.coordinateAllocationAttempts, 1u);
            CHECK_EQ(page.widgets.references[0].node, 1);
            CHECK(page.widgets.references[0].coordinates[0] == qt::L("old coordinate"));
            CHECK(page.mdsThreshold.text == qt::L("old threshold"));
            CHECK(page.rotationVector[0] == qt::L("old rotation"));
        }
        CHECK_EQ(f.model.step->u26c4[11], profile == 1 ? 0 : -1);
        CHECK(f.model.step->u1b88 == nullptr); CHECK(f.model.step->u1b78 == nullptr);
        if (profile == 1) {
            const auto& timer = def::model::g_timer_read_obj_node_state_variable;
            CHECK_EQ(timer.count, 2);
            CHECK(std::string(timer.labels[0]) == "Start");
            CHECK(std::string(timer.labels[1]) == "ReadSingleAArrayFromDB");
            CHECK(timer.subs[0] == nullptr); CHECK(timer.subs[1] == nullptr);
            CHECK_EQ(def::model::g_timer_read_arrays.count, globals.arraysTimer->count);
        } else globals.timersUnchanged();
        def::model::g_timer_read_obj_node_state_variable = *globals.nodeTimer;
    }
}

TEST_CASE(stopping_cached_reader_refuses_noncanonical_id_type_and_foreign_owner)
{
    Globals globals;
    for (int profile = 0; profile < 3; ++profile) {
        Fixture f; auto page = f.page();
        auto& variables = *reinterpret_cast<Variables*>(&f.nodes);
        auto* original = variables.at(0);
        struct Restore {
            Variables& variables; def::model::StateVarBase* original;
            def::model::Cls_14017ef20& nodes; void* owner; int id;
            ~Restore() { variables.mutable_at(0) = original; original->id = id; nodes.u130 = owner; }
        } restore{variables, original, f.nodes, f.nodes.u130, original->id};
        def::model::StateVarNodePost other(original->id, nullptr);
        if (profile == 0) original->id = 999;
        if (profile == 1) variables.mutable_at(0) = &other;
        if (profile == 2) f.nodes.u130 = f.object;
        def::model::g_index_tables_dim = 2;
        const auto report = fillStopping3DInto(page, f.sc, f.model, NumericLocale::KnownC);
        CHECK(!report.complete()); CHECK(report.issue == (profile == 2 ?
            StoppingFillIssue::CoordinateOwner : StoppingFillIssue::CoordinateSync));
        CHECK_EQ(report.stateVariableDispatches, profile == 2 ? 0u : 1u);
        CHECK_EQ(def::model::g_index_tables_dim, 2);
        CHECK(page.mdsThreshold.text == qt::L("old threshold"));
    }
    globals.timersUnchanged();
}

TEST_CASE(stopping_cache_zero_records_only_native_timer_prefix_on_each_fresh_miss)
{
    Globals globals;
    Fixture f;
    f.model.step->u26c4[11] = 0;
    auto& timer = def::model::g_timer_read_obj_node_state_variable;
    timer.count = 7; timer.flag = 0x5a;
    std::memcpy(timer.name, "timer sentinel", 15);
    std::memcpy(timer.labels[0] + 6, "TAIL", 5);
    std::memcpy(timer.labels[1] + 26, "TAIL", 5);
    timer.labels[99][0] = 'Z'; timer.times[99] = 0x12345678;
    timer.subs[99] = reinterpret_cast<void*>(std::uintptr_t{0x1234});
    auto name = std::array<char, sizeof(timer.name)>{};
    std::memcpy(name.data(), timer.name, name.size());
    for (int invocation = 0; invocation < 2; ++invocation) {
        auto page = f.page(); // Each call models a freshly delivered callback.
        const auto report = fillStopping3DInto(page, f.sc, f.model, NumericLocale::KnownC);
        CHECK(!report.complete()); CHECK(report.issue == StoppingFillIssue::CoordinateSync);
        CHECK_EQ(report.coordinateGetterCalls, 1u); CHECK_EQ(report.stateVariableDispatches, 1u);
        CHECK_EQ(timer.count, 2);
        CHECK(std::string(timer.labels[0]) == "Start");
        CHECK(std::string(timer.labels[1]) == "ReadSingleAArrayFromDB");
        CHECK(timer.subs[0] == nullptr); CHECK(timer.subs[1] == nullptr);
        CHECK(std::memcmp(timer.name, name.data(), name.size()) == 0);
        CHECK(timer.flag == 0x5a); CHECK(std::memcmp(timer.labels[0] + 6, "TAIL", 5) == 0);
        CHECK(std::memcmp(timer.labels[1] + 26, "TAIL", 5) == 0);
        CHECK(timer.labels[99][0] == 'Z'); CHECK_EQ(timer.times[99], 0x12345678);
        CHECK(timer.subs[99] == reinterpret_cast<void*>(std::uintptr_t{0x1234}));
    }
    CHECK_EQ(def::model::g_timer_read_arrays.count, globals.arraysTimer->count);
    def::model::g_timer_read_obj_node_state_variable = *globals.nodeTimer;
}

TEST_CASE(stopping_cached_reader_completes_private_constructor_and_retained_apply)
{
    Globals globals;
    for (const bool cached : {false, true}) {
        Fixture f; initialize(f.sc); f.model.step->step_version = 11000;
        f.model.step->u26c4[11] = cached ? -1 : 0;
        RemeshState remesh; Io io; InitialEditorSession session; InitialOpenReport report;
        InitialEditorContext context{{1, 3, {true, true, true, true, true, true}, false, true},
            {1, {}}, NumericLocale::KnownC, true};
        context.stoppingLabelProfile = StoppingLabelProfile::AsciiNoActiveTranslator;
        session.pages.stopping.mdsThreshold.text = qt::L("unreached threshold");
        def::model::g_index_tables_dim = 2;
        openInitialEditorInto(session, f.model, remesh, io, context, report);
        CHECK(report.complete() == cached); REQUIRE(report.stoppingFill); REQUIRE(f.nodes.u008);
        CHECK(f.nodes.u130 == f.model.step.get()); CHECK_EQ(f.model.step->u26c4[11], cached ? -1 : 0);
        if (!cached) {
            CHECK(report.stage == InitialOpenStage::InitialFill);
            CHECK(report.reachedPage == Page::Stopping);
            CHECK(report.stoppingFill->issue == StoppingFillIssue::CoordinateSync);
            CHECK_EQ(report.stoppingFill->coordinateGetterCalls, 1u);
            CHECK(!report.temporariesCopied); CHECK_EQ(io.effects, 0u);
            CHECK(session.pages.stopping.mdsThreshold.text == qt::L("unreached threshold"));
            CHECK_EQ(def::model::g_timer_read_obj_node_state_variable.count, 2);
            CHECK(std::string(def::model::g_timer_read_obj_node_state_variable.labels[0]) == "Start");
            CHECK(std::string(def::model::g_timer_read_obj_node_state_variable.labels[1]) ==
                "ReadSingleAArrayFromDB");
            def::model::g_timer_read_obj_node_state_variable = *globals.nodeTimer;
            continue;
        }
        CHECK(report.temporariesCopied); CHECK(report.backupRestored);
        CHECK_EQ(report.stoppingFill->coordinateGetterCalls, 0u);
        CHECK(session.pages.stopping.widgets.references[0].coordinates[0] == qt::L("1.235"));
        f.nodes.u008[0] = 17.25; f.nodes.u008[1] = -3.5; f.nodes.u008[2] = 9.75;
        field<double>(f.object, 0x1ed0) = 90.0; field<int>(f.object, 0x1ee8) = 99;
        const std::vector<RecoveryEntry> empty;
        const auto applied = applyInitialEditorPageState(session, f.model, remesh, io, &empty, &empty);
        CHECK(applied.outcome == ApplyResult::Completed); REQUIRE(applied.recoveryTail);
        CHECK(*applied.recoveryTail == ApplyResult::Completed);
        CHECK_EQ(field<int>(f.object, 0x1ee8), 1); CHECK(field<double>(f.object, 0x1ed0) == 1.235);
        REQUIRE(f.nodes.u008); CHECK(f.nodes.u130 == f.model.step.get());
        CHECK(f.nodes.u008[0] == 17.25); CHECK(f.nodes.u008[1] == -3.5); CHECK(f.nodes.u008[2] == 9.75);
        CHECK(!InitialEditorSession::completeForPublicSave);
    }
    globals.timersUnchanged();
}

int main() { return testkit::run_all(); }
