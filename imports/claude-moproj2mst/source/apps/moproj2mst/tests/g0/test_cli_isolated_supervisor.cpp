// ctest-labels: m2m-g0 isolated-cli
// The successful acceptance case exercises a normal Returned path only; it does not execute parent terminal relay.
#include <testkit/check.hpp>
#include "../support/test_paths.hpp"
#include "../../src/cli/isolated_save.hpp"
#include "moproj2mst.h"

#include <atomic>
#include <filesystem>
#include <string>
#include <string_view>

namespace fs = std::filesystem;

namespace {
struct Scratch {
    fs::path root;
    Scratch()
    {
        static std::atomic<unsigned> serial{0};
        const auto path = m2m_test::scratchRoot() / ("isolated-supervisor-" +
            std::to_string(GetCurrentProcessId()) + "-" + std::to_string(GetTickCount64()) + "-" +
            std::to_string(++serial));
        REQUIRE(fs::create_directory(path));
        root = path;
    }
    ~Scratch()
    {
        std::error_code ignored;
        fs::remove_all(root, ignored);
    }
};

m2m::cli::IsolatedSaveRequest requestFor(const fs::path& root)
{
    fs::create_directory(root / "deform");
    fs::create_directory(root / "project");
    return {(root / "project" / "missing.moproj").wstring(), (root / "result.MST").wstring(),
            (root / "deform").wstring(), (root / "receipt.json").wstring(),
            M2M_FLAG_WRITE_SAVE, 1252};
}
}

TEST_CASE(cli_isolated_save_rejects_receipt_output_alias_before_launch)
{
    Scratch scratch; auto request = requestFor(scratch.root);
    request.receipt = request.output;
    CHECK_EQ(m2m::cli::runIsolatedSave(request), M2M_E_ARG);
    CHECK(!fs::exists(scratch.root / "receipt.json"));
}

TEST_CASE(cli_isolated_save_rejects_receipt_inside_project_tree_before_worker_launch)
{
    Scratch scratch; auto request = requestFor(scratch.root);
    fs::create_directory(scratch.root / "project" / "diagnostics");
    request.receipt = (scratch.root / "project" / "diagnostics" / "receipt.json").wstring();
    // A launched worker would return M2M_E_PARSE for this missing project.
    // M2M_E_ARG proves the nested receipt is rejected in request preflight.
    CHECK_EQ(m2m::cli::runIsolatedSave(request), M2M_E_ARG);
    CHECK(!fs::exists(scratch.root / "project" / "diagnostics" / "receipt.json"));
    CHECK(!fs::exists(scratch.root / "result.MST"));
}

TEST_CASE(cli_isolated_save_returns_worker_adapter_code_only_after_supervised_normal_exit)
{
    Scratch scratch; const auto request = requestFor(scratch.root);
    // WRITE_SAVE selects the already-selected-path Open/Save lifecycle, which
    // skips the legacy read preflight; a missing .moproj is rejected by the
    // project reader as M2M_E_PARSE. The worker returns that adapter code
    // normally, rather than treating it as a supervisor/transport failure.
    CHECK_EQ(m2m::cli::runIsolatedSave(request), M2M_E_PARSE);
    CHECK(!fs::exists(scratch.root / "receipt.json"));
    CHECK(!fs::exists(scratch.root / "result.MST"));
}

int wmain(int argc, wchar_t** argv)
{
    if (argc > 1 && argv && argv[1] && std::wstring_view(argv[1]) == L"--m2m-isolated-save-worker-v1")
        return m2m::cli::isolatedSaveWorkerMain(argc, argv);
    return testkit::run_all();
}
