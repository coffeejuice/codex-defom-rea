#include "../support/test_paths.hpp"
// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <process.h>
#include <string>
#include <windows.h>
#include "m2m/engine.hpp"
#include "m2m/mst/writer.hpp"
#include "../../src/datafiles/readers.hpp"
#include "../../src/datafiles/cogging_file.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {
fs::path freshDir()
{
    static unsigned serial = 0;
    const auto path = m2m_test::scratchOutput(m2m_test::scratchRoot() /
        ("cogging_io_" + std::to_string(_getpid())) / std::to_string(++serial));
    fs::create_directories(path);
    return path;
}
std::string readAll(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
struct Handle {
    HANDLE value = INVALID_HANDLE_VALUE;
    ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
};
struct Fixture {
    fs::path root = freshDir();
    Project project;
    Operation operation;
    std::shared_ptr<CoggingProps> props = std::make_shared<CoggingProps>();
    Fixture()
    {
        project.path = String((root / "project.moproj").u16string());
        operation.project = &project;
        operation.taskDir = String(u"Task");
        operation.classKey = String(u"SfOprCogging");
        operation.props = props;
        props->owner = &operation;
        props->model = std::make_shared<data::CogModel>();
        datafiles::cogModelDefaults(*props->model, 1, project.p06Cog);
        fs::create_directories(root / "Task");
    }
    fs::path path() const { return root / "Task" / "Cogging.MDT"; }
};
constexpr auto header = "DEFORM-2D/3D 11.0";
}

TEST_CASE(cogging_native_matches_recording_with_existing_reader_no_delete_share)
{
    Fixture f;
    { std::ofstream seed(f.path(), std::ios::binary); seed << "old"; }
    auto globals = f.project.p06Cog;
    const auto expected = datafiles::writeMdtBytes(*f.props->model, globals, header);
    Handle reader{CreateFileW(f.path().c_str(), GENERIC_READ,
                              FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, nullptr)};
    REQUIRE(reader.value != INVALID_HANDLE_VALUE);
    f.project.p06Cog.noDies = 7;
    CHECK(datafiles::saveCoggingMdt(f.project, f.operation, header, true, true, nullptr));
    CHECK_EQ(readAll(f.path()), expected); // direct truncation succeeds without FILE_SHARE_DELETE
    CHECK_EQ(f.project.p06Cog.noDies, globals.noDies);
}

TEST_CASE(cogging_failed_open_skips_globals_and_later_operation_still_writes)
{
    Fixture f;
    { std::ofstream seed(f.path(), std::ios::binary); seed << "preserved"; }
    Handle locked{CreateFileW(f.path().c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr,
                              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)};
    REQUIRE(locked.value != INVALID_HANDLE_VALUE);
    f.project.p06Cog.noDies = 7;
    CHECK(datafiles::saveCoggingMdt(f.project, f.operation, header, true, true, nullptr));
    CHECK_EQ(f.project.p06Cog.noDies, 7); // fopen precedes masterWrite/DIE global mutation
    f.operation.taskDir = String(u"Later");
    fs::create_directory(f.root / "Later");
    CHECK(datafiles::saveCoggingMdt(f.project, f.operation, header, true, true, nullptr));
    CHECK_EQ(f.project.p06Cog.noDies, 2);
    CHECK(fs::file_size(f.root / "Later" / "Cogging.MDT") > 0);
    CloseHandle(locked.value);
    locked.value = INVALID_HANDLE_VALUE;
    CHECK_EQ(readAll(f.path()), std::string("preserved"));
}

TEST_CASE(cogging_native_failed_parent_and_unencodable_path_do_not_generate)
{
    Fixture f;
    f.project.p06Cog.noDies = 7;
    f.operation.taskDir = String(u"Missing/Child");
    CHECK(datafiles::saveCoggingMdt(f.project, f.operation, header, true, true, nullptr));
    CHECK_EQ(f.project.p06Cog.noDies, 7);
    CHECK(!fs::exists(f.root / "Missing"));
    f.operation.taskDir = String(u"Unicode_\u0100");
    fs::create_directory(f.root / fs::path(f.operation.taskDir.u16()));
    CHECK(datafiles::saveCoggingMdt(f.project, f.operation, header, true, true, nullptr));
    CHECK_EQ(f.project.p06Cog.noDies, 7); // Latin1 '?' cannot be a Windows filename character
    CHECK(!fs::exists(f.root / fs::path(f.operation.taskDir.u16()) / "Cogging.MDT"));
    std::vector<datafiles::SideFile> recorded;
    int code = 0;
    try { datafiles::saveCoggingMdt(f.project, f.operation, header, true, false, &recorded); }
    catch (const Error& error) { code = error.code; }
    CHECK_EQ(code, M2M_E_UNSUPPORTED); // aggregate recording profile remains explicit
}

TEST_CASE(cogging_native_missing_property_owner_skips_file_and_globals)
{
    Fixture f;
    f.project.p06Cog.noDies = 7;
    f.props->owner = nullptr;
    CHECK(datafiles::saveCoggingMdt(f.project, f.operation, header, true, true, nullptr));
    CHECK_EQ(f.project.p06Cog.noDies, 7);
    CHECK(!fs::exists(f.path()));
}

TEST_CASE(cogging_crt_buffer_boundary_text_translation_and_nul_path)
{
    const auto root = freshDir();
    const auto path = root / "buffer.txt";
    auto nulPath = path.u16string();
    nulPath.push_back(0);
    nulPath += u"ignored.txt";
    datafiles::detail::CoggingFile file{String(nulPath)};
    REQUIRE(file.get());
    const std::string a(4095, 'a');
    CHECK_EQ(std::fprintf(file.get(), "%s", a.c_str()), 4095);
    CHECK_EQ(fs::file_size(path), 0u);
    CHECK_EQ(std::fprintf(file.get(), "b"), 1);
    CHECK_EQ(fs::file_size(path), 0u);
    CHECK_EQ(std::fprintf(file.get(), "c\n"), 2);
    CHECK_EQ(fs::file_size(path), 4096u);
    file.close();
    file.close();
    CHECK_EQ(readAll(path), a + "bc\r\n");
}

TEST_CASE(cogging_crt_flush_failure_discards_buffer_but_later_printf_runs)
{
    const auto path = freshDir() / "retry.txt";
    datafiles::detail::CoggingFile file{String(path.u16string())};
    REQUIRE(file.get());
    Handle lock{CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, nullptr)};
    REQUIRE(lock.value != INVALID_HANDLE_VALUE);
    OVERLAPPED range{};
    REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                       0, 8192, 0, &range));
    CHECK_EQ(std::fprintf(file.get(), "first\n"), 6);
    CHECK_EQ(std::fflush(file.get()), EOF);
    CHECK(std::ferror(file.get()) != 0);
    CHECK_EQ(fs::file_size(path), 0u);
    REQUIRE(UnlockFileEx(lock.value, 0, 8192, 0, &range));
    CHECK_EQ(std::fprintf(file.get(), "second\n"), 7); // no QTextStream-style suppression
    CHECK(std::ferror(file.get()) != 0); // sticky diagnostic does not prohibit later writes
    file.close();
    CHECK_EQ(readAll(path), std::string("second\r\n")); // failed CRT flush discarded first buffer
}

TEST_CASE(cogging_crt_close_ignores_failed_flush)
{
    const auto path = freshDir() / "close.txt";
    datafiles::detail::CoggingFile file{String(path.u16string())};
    REQUIRE(file.get());
    Handle lock{CreateFileW(path.c_str(), GENERIC_READ | GENERIC_WRITE,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL, nullptr)};
    REQUIRE(lock.value != INVALID_HANDLE_VALUE);
    OVERLAPPED range{};
    REQUIRE(LockFileEx(lock.value, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY,
                       0, 8192, 0, &range));
    CHECK_EQ(std::fprintf(file.get(), "lost\n"), 5);
    file.close(); // no exception/status promotion; writer returns1 regardless
    CHECK(file.get() == nullptr);
    REQUIRE(UnlockFileEx(lock.value, 0, 8192, 0, &range));
    CHECK(readAll(path).empty());
}

int main() { return testkit::run_all(); }
