// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <limits>

#include "m2m/engine.hpp"
#include "m2m/engine/simctrl_ppv_save_file.hpp"
#include "m2m/model.hpp"
#include "m2m/qt4path.hpp"
#include "../../src/engine/engine_internal.hpp"

using namespace m2m;
using namespace m2m::engine::simctrl_ppv;
namespace fs = std::filesystem;

namespace {
struct Fixture {
    Project project;
    ProjectScope scope{project};
    Ctx context;
    Operation* active = nullptr;

    Fixture()
    {
        const char* configured = std::getenv("M2M_ORACLE_B");
        const fs::path oracle = configured
            ? fs::path(configured)
            : m2m_test::configuredPath("M2M_ORACLE_B");
        const fs::path input = oracle / "93_B_MB3ACT_before" / "Gear_blank.moproj";
        REQUIRE(fs::is_regular_file(input));
        const qt::String file(input.u16string());
        const qt::String db = app::changeSuffix(file, qt::L(".DB"));
        initProject(project, qt::absolutePath(db), qt::completeBaseName(db));
        makeRegistry(project);
        REQUIRE(openProject(project, file, context) && project.openedOk);
        activateSubProject(project, 0, context);
        engine::FileEffects opened;
        active = engine::openActiveOperation(project, context, opened);
        REQUIRE(active != nullptr);
    }

    OperationSaveInput qualified(SaveOpenOutcome open) const
    {
        return {SavePathQualification::OperationTaskPath,
                SaveCodecQualification::KnownAsciiCompatible, 1252, open};
    }
};

bool sameEntries(const std::vector<RecoveryEntry>& a,
                 const std::vector<RecoveryEntry>& b)
{
    if (a.size() != b.size()) return false;
    for (std::size_t i = 0; i < a.size(); ++i) {
        if (a[i].type != b[i].type || a[i].value != b[i].value ||
            a[i].ppvSecondInt != b[i].ppvSecondInt ||
            a[i].ppvFirstDouble != b[i].ppvFirstDouble ||
            a[i].ppvSecondDouble != b[i].ppvSecondDouble) return false;
    }
    return true;
}
} // namespace

TEST_CASE(ppv_save_adapter_local_empty_skips_before_root_path_codec_and_open)
{
    Fixture f;
    REQUIRE(engine::prevOp(f.project, *f.active) != nullptr);
    auto* root = recoveryListA(*f.active);
    REQUIRE(root != nullptr);
    REQUIRE(f.active->recoveryB != nullptr);
    f.active->recoveryB->clear();
    root->clear();
    root->push_back({7, 71.0, 8, 9.5, 10.5});
    f.active->recoveryFlags = std::make_shared<std::vector<bool>>(1, true);

    const OperationSaveResult result = writePpvForOperation(*f.active, {});
    CHECK(result.status == OperationSaveStatus::LocalListEmpty);
    CHECK(!result.openAttempted);
    CHECK(!result.writerReturned);
    CHECK(result.path.isEmpty());
    CHECK(result.bytes.empty());
    CHECK(f.active->recoveryB->empty());
    REQUIRE(root->size() == 1u);
    CHECK_EQ(root->front().value, 71.0);
    REQUIRE(f.active->recoveryFlags != nullptr);
    REQUIRE(f.active->recoveryFlags->size() == 1u);
    CHECK(f.active->recoveryFlags->front());
}

TEST_CASE(ppv_save_adapter_preserves_local_empty_root_only_gate)
{
    Fixture f;
    REQUIRE(engine::prevOp(f.project, *f.active) != nullptr);
    auto* root = recoveryListA(*f.active);
    REQUIRE(root != nullptr);
    f.active->recoveryB->clear();
    root->clear();
    root->push_back({7, 71.0, 8, 9.5, 10.5});

    const OperationSaveResult result = writePpvForOperation(*f.active,
        f.qualified(SaveOpenOutcome::Succeeded));
    // Native 0x140d4afe0 gates only on the local QList before QFile open.
    CHECK(result.status == OperationSaveStatus::LocalListEmpty);
    CHECK(!result.openAttempted);
    CHECK(!result.writerReturned);
    CHECK(result.bytes.empty());
    CHECK_EQ(root->size(), 1u);
}

TEST_CASE(ppv_save_adapter_preserves_qfile_open_failure_order)
{
    Fixture f;
    REQUIRE(engine::prevOp(f.project, *f.active) != nullptr);
    auto* root = recoveryListA(*f.active);
    REQUIRE(root != nullptr);
    root->clear();
    root->push_back({7, 71.0, 8, 9.5, 10.5});
    f.active->recoveryB->clear();
    f.active->recoveryB->push_back({4, 41.0, 5, 1.25, 2.5});
    f.active->recoveryFlags = std::make_shared<std::vector<bool>>(1, true);

    const auto beforeLocal = *f.active->recoveryB;
    const auto beforeRoot = *root;
    const auto beforeFlags = *f.active->recoveryFlags;
    const OperationSaveResult result = writePpvForOperation(*f.active,
        f.qualified(SaveOpenOutcome::Failed));
    CHECK(result.status == OperationSaveStatus::OpenFailed);
    CHECK(!result.writerReturned);
    CHECK(result.openAttempted);
    CHECK(result.bytes.empty());
    CHECK(!result.path.isEmpty());
    CHECK(sameEntries(*f.active->recoveryB, beforeLocal));
    CHECK(sameEntries(*root, beforeRoot));
    CHECK(*f.active->recoveryFlags == beforeFlags);
}

TEST_CASE(ppv_save_adapter_writes_shared_then_local_from_actual_owners)
{
    Fixture f;
    REQUIRE(engine::prevOp(f.project, *f.active) != nullptr);
    auto* root = recoveryListA(*f.active);
    REQUIRE(root != nullptr);
    root->clear();
    root->push_back({7, 71.0, 8, 9.5, 10.5});
    f.active->recoveryB->clear();
    f.active->recoveryB->push_back({4, 41.0, 5, 1.25, 2.5});
    f.active->recoveryFlags = std::make_shared<std::vector<bool>>(1, true);

    const auto beforeLocal = *f.active->recoveryB;
    const auto beforeRoot = *root;
    const auto beforeFlags = *f.active->recoveryFlags;
    const OperationSaveResult result = writePpvForOperation(*f.active,
        f.qualified(SaveOpenOutcome::Succeeded));
    CHECK(result.status == OperationSaveStatus::PayloadReady);
    CHECK(result.openAttempted);
    CHECK(result.writerReturned);
    CHECK_EQ(result.localCount, 1);
    CHECK_EQ(result.sharedCount, 1);
    CHECK(result.path == qt::cleanPath(taskDirAbs(*f.active) + qt::kSep +
                                       qt::L("SimCtrl.PPV")));
    CHECK_EQ(result.bytes, std::string("2\r\n"
        "0\ttrue\t7\t8\t9.5\t10.5\t1\r\n"
        "0\tfalse\t4\t5\t1.25\t2.5\t0\r\n"));
    CHECK(sameEntries(*f.active->recoveryB, beforeLocal));
    CHECK(sameEntries(*root, beforeRoot));
    CHECK(*f.active->recoveryFlags == beforeFlags);
}

TEST_CASE(ppv_save_adapter_guards_flags_path_codec_and_unknown_open)
{
    Fixture f;
    REQUIRE(engine::prevOp(f.project, *f.active) != nullptr);
    auto* root = recoveryListA(*f.active);
    REQUIRE(root != nullptr);
    root->clear();
    root->push_back({7, 71.0, 8, 9.5, 10.5});
    f.active->recoveryB->clear();
    f.active->recoveryB->push_back({4, 41.0, 5, 1.25, 2.5});

    f.active->recoveryFlags = std::make_shared<std::vector<bool>>();
    auto result = writePpvForOperation(*f.active, f.qualified(SaveOpenOutcome::Succeeded));
    CHECK(result.status == OperationSaveStatus::FlagsTooShort);
    CHECK(!result.openAttempted);
    CHECK(result.bytes.empty());

    f.active->recoveryFlags.reset();
    OperationSaveInput input = f.qualified(SaveOpenOutcome::Succeeded);
    input.path = SavePathQualification::Unknown;
    result = writePpvForOperation(*f.active, input);
    CHECK(result.status == OperationSaveStatus::PathUnqualified);
    CHECK(!result.openAttempted);

    input = f.qualified(SaveOpenOutcome::Succeeded);
    input.codec = SaveCodecQualification::Unknown;
    result = writePpvForOperation(*f.active, input);
    CHECK(result.status == OperationSaveStatus::CodecUnqualified);
    CHECK(!result.openAttempted);

    result = writePpvForOperation(*f.active, f.qualified(SaveOpenOutcome::Unknown));
    CHECK(result.status == OperationSaveStatus::OpenOutcomeUnknown);
    CHECK(result.openAttempted);
    CHECK(!result.writerReturned);
    CHECK(result.bytes.empty());
}

TEST_CASE(ppv_save_adapter_keeps_no_predecessor_local_route)
{
    Fixture f;
    Operation* first = operationById(f.project, 1);
    REQUIRE(first != nullptr);
    CHECK(engine::prevOp(f.project, *first) == nullptr);
    first->recoveryB->clear();
    first->recoveryB->push_back({3, 31.0, 4, 5.5, 6.5});
    first->recoveryFlags = std::make_shared<std::vector<bool>>(1, true); // Not passed without predecessor.

    const OperationSaveResult result = writePpvForOperation(*first,
        f.qualified(SaveOpenOutcome::Succeeded));
    CHECK(result.status == OperationSaveStatus::PayloadReady);
    CHECK_EQ(result.localCount, 1);
    CHECK_EQ(result.sharedCount, 0);
    CHECK_EQ(result.bytes, std::string("1\r\n"
        "0\tfalse\t3\t4\t5.5\t6.5\t0\r\n"));
}

TEST_CASE(ppv_save_adapter_checks_signed_native_count_range_without_allocation)
{
    const auto max = static_cast<std::size_t>((std::numeric_limits<int>::max)());
    CHECK(ppvCountsRepresentable(max, 0));
    CHECK(ppvCountsRepresentable(0, max));
    CHECK(!ppvCountsRepresentable(max, 1));
    CHECK(!ppvCountsRepresentable(max + 1, 0));
}

int main() { return testkit::run_all(); }
