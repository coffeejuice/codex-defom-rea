#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt
// Native result algebra and disposable candidate I/O; no original timing claim.
#include <testkit/check.hpp>
#include <process.h>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>
#include "m2m/data/boolean.hpp"
#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/boolean_side_file_result.hpp"
#include "m2m/mst/loader_l5.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/props.hpp"

namespace m2m::mst { void installLoaderSlots(); }
using namespace m2m;
namespace fs = std::filesystem;
using Attempt = mst::BooleanFileAttemptResult;
namespace {
template<class F> void refused(F&& action)
{
    bool caught = false;
    try { action(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    Project project;
    std::shared_ptr<data::BooleanData> data = std::make_shared<data::BooleanData>();
    std::shared_ptr<BooleanProps> properties = std::make_shared<BooleanProps>();
    Operation* operation = nullptr;
    std::vector<std::string> calls;
    Fixture()
    {
        auto value = std::make_unique<Operation>(); operation = value.get();
        operation->project = &project; operation->props = properties;
        properties->owner = operation; properties->data = data;
        project.operations.push_back(std::move(value));
    }
    mst::BooleanSideFileAttempts attempts(Attempt bwz = Attempt::OpenedPath,
                                        Attempt ini = Attempt::OpenedPath)
    {
        return {[this, bwz] { calls.push_back("BWZ"); return bwz; },
                [this, ini] { calls.push_back("INI"); return ini; }};
    }
    int run(const mst::BooleanSideFileAttempts& a)
    { return mst::nativeBooleanSideFileSequence(project, *operation, *data, a); }
};
struct DiskFixture : Fixture {
    fs::path root, task, bwz, ini;
    Ctx context;
    explicit DiskFixture(bool createParent = true)
    {
        static unsigned serial = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("b89_" + std::to_string(_getpid()) + "_" +
             std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
             "_" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root));
        fs::create_directories(root);
        project.path = qt::String((root / "test.moproj").u16string());
        operation->taskDir = qt::L("Task");
        operation->classKey = qt::L("SfOprBoolean2D");
        makeRegistry(project);
        task = root / "Task"; bwz = task / "Boolean.BWZ"; ini = task / "DEF_MACHDIST_CTRL2.INI";
        if (createParent) fs::create_directories(task);
        context.nativeMstSideFiles = true;
        data->workpiece = 7; data->cutter = 9; data->remesh = 1;
    }
    int write() { return mst::booleanSideFilesNativeResult(project, *operation, *data, context); }
};
std::string read(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
}

TEST_CASE(boolean_native_result_all_attempt_pairs_preserve_bwz_result_and_order)
{
    for (const auto bwz : {Attempt::OpenFailed, Attempt::OpenedPath})
        for (const auto ini : {Attempt::OpenFailed, Attempt::OpenedPath}) {
            Fixture f;
            CHECK_EQ(f.run(f.attempts(bwz, ini)), static_cast<int>(bwz));
            CHECK(f.calls == std::vector<std::string>({"BWZ", "INI"}));
        }
}

TEST_CASE(boolean_native_result_missing_properties_and_null_owner_return_minus_one_without_attempts)
{
    Fixture missing; missing.operation->props.reset();
    CHECK_EQ(missing.run({}), -1);
    CHECK(missing.calls.empty());
    Fixture null; null.properties->owner = nullptr;
    CHECK_EQ(null.run(null.attempts()), -1);
    CHECK(null.calls.empty());
}

TEST_CASE(boolean_native_result_expired_actual_opi_guard_returns_minus_one_without_attempts)
{
    Fixture f;
    CHECK(f.operation->commitOperationInfoQObjectBoundary("synthetic qualified QObject boundary only"));
    CHECK_EQ(f.run(f.attempts()), -1);
    CHECK(f.calls.empty());
    CHECK(f.properties->owner == f.operation); // raw adapter pointer is not guard liveness
}

TEST_CASE(boolean_native_result_foreign_project_and_properties_owner_refuse_before_attempts)
{
    Fixture f, other;
    refused([&] { mst::nativeBooleanSideFileSequence(other.project, *f.operation, *f.data, f.attempts()); });
    CHECK(f.calls.empty());
    f.properties->owner = other.operation;
    refused([&] { f.run(f.attempts()); });
    CHECK(f.calls.empty());
    f.properties->owner = f.operation; f.operation->project = nullptr;
    refused([&] { f.run(f.attempts()); });
    CHECK(f.calls.empty());
}

TEST_CASE(boolean_native_result_wrong_or_unavailable_data_and_properties_do_not_become_null_owner)
{
    Fixture f;
    data::BooleanData different;
    refused([&] { mst::nativeBooleanSideFileSequence(f.project, *f.operation, different, f.attempts()); });
    f.properties->data.reset(); refused([&] { f.run(f.attempts()); });
    f.operation->props = std::make_shared<Props>(); refused([&] { f.run(f.attempts()); });
    CHECK(f.calls.empty());
}

TEST_CASE(boolean_native_result_unavailable_writer_refuses_before_any_attempt)
{
    Fixture f;
    auto attempts = f.attempts(); attempts.ini = {};
    refused([&] { f.run(attempts); });
    attempts = f.attempts(); attempts.bwz = {};
    refused([&] { f.run(attempts); });
    CHECK(f.calls.empty());
}

TEST_CASE(boolean_native_result_does_not_turn_cpp_exception_into_returned_open_failure)
{
    Fixture first;
    auto attempts = first.attempts();
    attempts.bwz = [&]() -> Attempt { first.calls.push_back("BWZ"); throw std::runtime_error("injected"); };
    bool caught = false;
    try { (void)first.run(attempts); } catch (const std::runtime_error&) { caught = true; }
    CHECK(caught && first.calls == std::vector<std::string>({"BWZ"}));

    Fixture second; attempts = second.attempts(Attempt::OpenFailed);
    attempts.ini = [&]() -> Attempt { second.calls.push_back("INI"); throw std::runtime_error("injected"); };
    caught = false;
    try { (void)second.run(attempts); } catch (const std::runtime_error&) { caught = true; }
    CHECK(caught && second.calls == std::vector<std::string>({"BWZ", "INI"}));
}

TEST_CASE(boolean_native_result_opened_path_result_is_not_stream_flush_success)
{
    struct FailedDevice : mst::OutputDevice {
        bool open = true;
        std::int64_t write(std::string_view) override { return -1; }
        bool flush() override { return false; }
        void close() override { notifyAboutToClose(); open = false; }
        bool opened() const override { return open; }
    } device;
    Fixture f;
    auto attempts = f.attempts();
    attempts.bwz = [&] {
        f.calls.push_back("BWZ");
        mst::Stream stream(1252, &device); stream << "prefix" << mst::endl;
        CHECK(stream.writeFailed()); device.close();
        return Attempt::OpenedPath; // native selected close path returns1
    };
    CHECK_EQ(f.run(attempts), 1);
    CHECK(f.calls == std::vector<std::string>({"BWZ", "INI"}));
}

TEST_CASE(boolean_native_physical_result_failed_bwz_open_still_writes_ini)
{
    DiskFixture f; fs::create_directory(f.bwz);
    CHECK_EQ(f.write(), 0);
    CHECK(fs::is_directory(f.bwz));
    CHECK(read(f.ini).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
}

TEST_CASE(boolean_native_physical_result_failed_ini_open_keeps_bwz_result_one)
{
    DiskFixture f; fs::create_directory(f.ini);
    CHECK_EQ(f.write(), 1);
    CHECK(read(f.bwz).find("WZD_BOOLEAN_CUTTER_OBJECT_NO\n  * OBJECT # FOR CUTTER \n9") != std::string::npos);
    CHECK(fs::is_directory(f.ini));
}

TEST_CASE(boolean_native_physical_result_both_failed_or_missing_parent_create_no_success)
{
    DiskFixture both; fs::create_directory(both.bwz); fs::create_directory(both.ini);
    CHECK_EQ(both.write(), 0);
    DiskFixture missing(false);
    CHECK_EQ(missing.write(), 0);
    CHECK(!fs::exists(missing.task)); // writer does not create task directory
}

TEST_CASE(boolean_native_result_void_slot_and_staged_mode_remain_separate)
{
    DiskFixture native;
    mst::installLoaderSlots();
    engine::FileEffects effects;
    mst::l5Slot(mst::g_l5Slots.booleanSideFiles)(
        native.project, *native.operation, *native.data, native.context, effects);
    CHECK(read(native.bwz).find("WZD_BOOLEAN_REMESH") != std::string::npos);
    CHECK(read(native.ini).find("WZD_MACHDIST_OPERATION_BOOLEAN") != std::string::npos);
    CHECK(effects.writes.empty());

    DiskFixture staged; staged.context.nativeMstSideFiles = false;
    refused([&] { staged.write(); });
    CHECK(!fs::exists(staged.bwz) && !fs::exists(staged.ini));
    mst::l5Slot(mst::g_l5Slots.booleanSideFiles)(
        staged.project, *staged.operation, *staged.data, staged.context, effects);
    CHECK(effects.writes.size() == 2u && staged.context.pendingWrites.size() == 2u);
    CHECK(!fs::exists(staged.bwz) && !fs::exists(staged.ini));
}

int main() { return testkit::run_all(); }
