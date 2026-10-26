// ctest-labels: m2m-s internal-extra3d-page
// Native file IO plus bounded page projection, not C ABI/full Session/native GUI acceptance.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <testkit/check.hpp>
#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/common.hpp"
#include "../support/test_paths.hpp"
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <limits>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace fs = std::filesystem;
namespace {
constexpr std::string_view inputRow = "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n";
// Native QStringList order and TAB after every non-LF item; binary QFile output.
constexpr std::string_view expected = "1\t\n10\t\n0.5\t1.5\t20\t0\t\n1\t0\t\n\r\n";
constexpr std::string_view twoRows =
    "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n";
constexpr std::string_view expectedTwo =
    "2\t\n10\t\n0.5\t1.5\t20\t0\t\n1\t0\t\n30\t\n2.5\t3.5\t40\t1\t\n0\t1\t\n\r\n";
// Synthetic parsed-ID projections, not observed native combo population.
constexpr std::array firstIds{0, 10};
constexpr std::array secondIds{0, 20};
constexpr std::array twoFirstIds{0, 10, 30};
constexpr std::array twoSecondIds{0, 20, 40};
constexpr std::string_view generalRows =
    "3\r\n17 1.23456789 -2.34567891 19 0.123456789 -3.987654321 0.03125\r\n"
    "-7 -0.125 42.123456789 111 9.87654321 0 1000\r\n"
    "17 2.5 3.5 19 1 0 1\r\nIGNORED ASCII SUFFIX";
constexpr std::string_view expectedGeneral =
    "3\t\n17\t\n1.23457\t-2.34568\t19\t0.123457\t\n-3.98765\t0.03125\t\n"
    "-7\t\n-0.125\t42.1235\t111\t9.87654\t\n0\t1000\t\n"
    "17\t\n2.5\t3.5\t19\t1\t\n0\t1\t\n\r\n";
constexpr std::array generalFirstIds{0, 17};
constexpr std::array generalSecondIds{0, 19};

struct ProjectFiles {
    fs::path base, root, diagnostics;
    std::unique_ptr<Extra3DIo> io;
    int warnings = 0;
    bool cleaned = false;
    ProjectFiles(std::string_view initialBytes = inputRow, std::string_view expectedBytes = expected) {
        const fs::path requestedBase = L"C:/PROBLEM";
        const DWORD attrs = GetFileAttributesW(requestedBase.c_str());
        REQUIRE(attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY));
        REQUIRE(!(attrs & FILE_ATTRIBUTE_REPARSE_POINT));
        base = fs::canonical(requestedBase);
        static unsigned sequence = 0;
        const auto name = "sp115_" + std::to_string(GetCurrentProcessId()) + "_" +
            std::to_string(GetTickCount64()) + "_" + std::to_string(++sequence);
        root = base / name;
        // Check the complete possible sidecar path set before creating the project leaf.
        for (const char* file : {"SPRING.INI", "SPRING2.INI", "NBC.DAT", "SW2SP.DAT",
                 "STRETCH.DAT", "B_WEIGHT.DAT", "STRAIN_DST.DAT"})
            REQUIRE((root / file).u16string().size() < 100);
        REQUIRE(root.parent_path() == base); REQUIRE(!fs::exists(root));
        diagnostics = m2m_test::scratchOutput(m2m_test::scratchRoot() / name);
        REQUIRE(fs::create_directory(diagnostics));
        REQUIRE(fs::create_directory(root));
        REQUIRE(fs::canonical(root) == root);
        io = makeNativeExtra3DIo(qt::String(root.u16string()), 1252, [this] { ++warnings; });
        retain("scope.txt", "internal-native-Extra3DIo/page;test-qualified-combo-IDs;"
            "not-full-Session;public-mode300-palette-guard-retained;not-original-runtime\n");
        retain("project-path.txt", root.string());
        retain("input-main.bin", initialBytes); retain("expected-main.bin", expectedBytes);
    }
    ~ProjectFiles() { if (!cleaned) (void)cleanup(); }
    fs::path path(const char* name) const {
        REQUIRE(std::string_view(name) == "SPRING.INI" || std::string_view(name) == "SPRING2.INI");
        return root / name;
    }
    void retain(const char* name, std::string_view bytes) const {
        std::ofstream file(m2m_test::scratchOutput(diagnostics / name), std::ios::binary);
        REQUIRE(file.is_open()); file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        file.close(); REQUIRE(!file.fail());
    }
    void write(const char* name, std::string_view bytes) {
        REQUIRE(fs::canonical(root) == root);
        const auto target = path(name);
        const DWORD attrs = GetFileAttributesW(target.c_str());
        REQUIRE(attrs == INVALID_FILE_ATTRIBUTES || !(attrs & FILE_ATTRIBUTE_REPARSE_POINT));
        std::ofstream file(target, std::ios::binary | std::ios::trunc);
        REQUIRE(file.is_open()); file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        file.close(); REQUIRE(!file.fail());
    }
    std::string read(const char* name) const {
        std::ifstream file(path(name), std::ios::binary); REQUIRE(file.is_open());
        return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }
    bool cleanup() noexcept {
        io.reset();
        bool success = true;
        try {
            // No recursive removal: only these two known sidecars and the empty
            // verified owned leaf. Unexpected files/reparse points retain the leaf.
            if (root.parent_path() != base || fs::canonical(root) != root) success = false;
            const DWORD rootAttrs = GetFileAttributesW(root.c_str());
            if (rootAttrs == INVALID_FILE_ATTRIBUTES || (rootAttrs & FILE_ATTRIBUTE_REPARSE_POINT)) success = false;
            if (success) {
                for (const char* name : {"SPRING.INI", "SPRING2.INI"}) {
                    const auto target = root / name;
                    const DWORD attrs = GetFileAttributesW(target.c_str());
                    if (attrs == INVALID_FILE_ATTRIBUTES) continue;
                    if ((attrs & (FILE_ATTRIBUTE_REPARSE_POINT | FILE_ATTRIBUTE_DIRECTORY)) ||
                        !DeleteFileW(target.c_str())) success = false;
                }
                if (success && !RemoveDirectoryW(root.c_str())) success = false;
            }
            retain("cleanup.txt", success ? "owned leaf removed\n" : "cleanup failed; owned leaf retained\n");
        } catch (...) { success = false; }
        cleaned = success;
        return success;
    }
};
void checkRow(const SpringRecord& row)
{
    CHECK_EQ(row.firstObject, 10); CHECK_EQ(row.secondObject, 20);
    CHECK(row.firstValue == 0.5); CHECK(row.secondValue == 1.5);
    CHECK((row.direction == std::array{0.0, 1.0, 0.0})); CHECK_EQ(row.flag, 0);
}
void checkSecondRow(const SpringRecord& row)
{
    CHECK_EQ(row.firstObject, 30); CHECK_EQ(row.secondObject, 40);
    CHECK(row.firstValue == 2.5); CHECK(row.secondValue == 3.5);
    CHECK((row.direction == std::array{1.0, 0.0, 1.0})); CHECK_EQ(row.flag, 0);
}
struct SpringWriteTrace final : Extra3DIo {
    Extra3DIo& source;
    std::vector<std::string> events;
    bool failPrimaryAfterOpen = false;
    bool recordAllOperations = false;
    bool denyPrimaryRemoval = false;
    bool failNbcOpen = false;
    unsigned primaryWriteCalls = 0, primaryCloseCalls = 0;
    struct FailedDevice final : mst::OutputDevice {
        SpringWriteTrace& owner;
        bool open = true;
        explicit FailedDevice(SpringWriteTrace& value) : owner(value) {}
        std::int64_t write(std::string_view) override { ++owner.primaryWriteCalls; return -1; }
        bool flush() override { return false; }
        void close() override {
            notifyAboutToClose(); ++owner.primaryCloseCalls;
            owner.events.push_back("close:SPRING.INI"); open = false;
        }
        bool opened() const override { return open; }
    };
    explicit SpringWriteTrace(Extra3DIo& value) : Extra3DIo(1252), source(value) {}
    bool readDoubles(const qt::String& path, std::span<double> values) override {
        return source.readDoubles(path, values);
    }
    bool readInts(const qt::String& path, std::span<int> values) override {
        return source.readInts(path, values);
    }
    bool readSprings(std::vector<SpringRecord>& rows) override { return source.readSprings(rows); }
    bool removeIfReadable(const qt::String& path) override {
        if (recordAllOperations || path == qt::L("SPRING2.INI"))
            events.push_back("remove:" + path.utf8());
        if (denyPrimaryRemoval && path == qt::L("SPRING.INI")) return false;
        return source.removeIfReadable(path);
    }
    void nbcRemoveWarning() override { source.nbcRemoveWarning(); }
    std::unique_ptr<mst::OutputDevice> openWrite(const qt::String& path) override {
        if (recordAllOperations || path == qt::L("SPRING.INI") || path == qt::L("SPRING2.INI"))
            events.push_back("write:" + path.utf8());
        if (failNbcOpen && path == qt::L("NBC.DAT")) return {};
        std::unique_ptr<mst::OutputDevice> file;
        if (failPrimaryAfterOpen && path == qt::L("SPRING.INI")) file = std::make_unique<FailedDevice>(*this);
        else file = source.openWrite(path);
        if (path == qt::L("SPRING.INI")) events.push_back(file && file->opened() ? "opened:1" : "opened:0");
        return file;
    }
};
struct DenyPrimaryWrite {
    HANDLE handle;
    explicit DenyPrimaryWrite(const fs::path& path)
        : handle(CreateFileW(path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)) {
        REQUIRE(handle != INVALID_HANDLE_VALUE);
    }
    ~DenyPrimaryWrite() { CloseHandle(handle); }
};
template<class F> bool unsupported(F&& action)
{
    try { action(); } catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
}

TEST_CASE(spring_native_page_one_row_normalizes_and_roundtrips_with_absent_companion)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    CHECK(!fs::exists(project.path("SPRING2.INI")));
    std::vector<SpringRecord> parsed;
    REQUIRE(project.io->readSprings(parsed)); REQUIRE(parsed.size() == 1); checkRow(parsed[0]);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, firstIds, secondIds);
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 1);
    const auto& spring = *form->spring;
    CHECK(spring.pageChecked); CHECK_EQ(spring.currentRow, 0);
    CHECK_EQ(spring.firstSelectedId, 10); CHECK_EQ(spring.secondSelectedId, 20);
    CHECK(!spring.recordFlagChecked); checkRow(spring.items[0]);
    const std::array texts{qt::L("0.5"), qt::L("1.5"), qt::L("0"), qt::L("1"), qt::L("0")};
    CHECK(spring.visibleNumbers == texts);
    constructorRefreshExtra3D(*form);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    const auto first = project.read("SPRING.INI"); project.retain("actual-first.bin", first);
    CHECK(first == expected); CHECK(first != inputRow);
    CHECK(!fs::exists(project.path("SPRING2.INI")));
    auto reopened = fillExtra3D(*project.io, NumericLocale::KnownC, firstIds, secondIds);
    REQUIRE(reopened); REQUIRE(reopened->spring); REQUIRE(reopened->spring->items.size() == 1);
    checkRow(reopened->spring->items[0]);
    CHECK(applyExtra3D(*project.io, *reopened) == ApplyResult::Completed);
    const auto second = project.read("SPRING.INI"); project.retain("actual-second.bin", second);
    CHECK(second == expected); CHECK_EQ(project.warnings, 0);
    CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_flag_zero_removes_companion_created_after_absent_open)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    REQUIRE(!fs::exists(project.path("SPRING2.INI")));
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, firstIds, secondIds);
    REQUIRE(form); REQUIRE(form->spring); CHECK(!form->spring->recordFlagChecked);
    // Exercises all-zero Save-time deletion of a companion created after Fill.
    constexpr std::string_view lateCompanion = "late readable companion\r\n";
    project.write("SPRING2.INI", lateCompanion); project.retain("late-companion.bin", lateCompanion);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    const auto actual = project.read("SPRING.INI"); project.retain("actual-main.bin", actual);
    CHECK(actual == expected); CHECK(!fs::exists(project.path("SPRING2.INI")));
    CHECK_EQ(project.warnings, 0); CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_retains_missing_combo_locale_and_reader_guards)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    CHECK(!fillExtra3D(*project.io, NumericLocale::KnownC)); // No qualified combo context.
    const std::array missingFirst{0, 99};
    CHECK(!fillExtra3D(*project.io, NumericLocale::KnownC, missingFirst, secondIds));
    CHECK(!fillExtra3D(*project.io, NumericLocale::Unrecovered, firstIds, secondIds));
    CHECK(project.read("SPRING.INI") == inputRow);
    auto edited = fillExtra3D(*project.io, NumericLocale::KnownC, firstIds, secondIds);
    REQUIRE(edited); REQUIRE(edited->spring);
    edited->spring->visibleNumbers[0] = qt::L("not-a-number");
    CHECK(applyExtra3D(*project.io, *edited) == ApplyResult::UnsupportedSnapshot);
    CHECK(project.read("SPRING.INI") == inputRow);
    project.write("SPRING2.INI", "bad\n");
    CHECK(unsupported([&] { (void)fillExtra3D(*project.io, NumericLocale::KnownC, firstIds, secondIds); }));
    CHECK(project.read("SPRING.INI") == inputRow); CHECK(project.read("SPRING2.INI") == "bad\n");
    REQUIRE(DeleteFileW(project.path("SPRING2.INI").c_str()));
    constexpr std::string_view partialTwo = "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n";
    project.write("SPRING.INI", partialTwo); project.retain("unsupported-partial-two.bin", partialTwo);
    CHECK(unsupported([&] { (void)fillExtra3D(*project.io, NumericLocale::KnownC, firstIds, secondIds); }));
    CHECK(project.read("SPRING.INI") == partialTwo); CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_two_ordered_rows_roundtrip_without_changing_unselected_payload)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, twoFirstIds, twoSecondIds);
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 2);
    CHECK_EQ(form->spring->currentRow, 0); CHECK_EQ(form->spring->firstSelectedId, 10);
    CHECK_EQ(form->spring->secondSelectedId, 20); CHECK(form->spring->pageChecked);
    CHECK(!form->spring->recordFlagChecked);
    checkRow(form->spring->items[0]); checkSecondRow(form->spring->items[1]);
    const std::array visible{qt::L("0.5"), qt::L("1.5"), qt::L("0"), qt::L("1"), qt::L("0")};
    CHECK(form->spring->visibleNumbers == visible);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    const auto first = project.read("SPRING.INI"); project.retain("actual-first.bin", first);
    CHECK(first == expectedTwo); CHECK(first != twoRows);
    checkRow(form->spring->items[0]); checkSecondRow(form->spring->items[1]);
    CHECK(!fs::exists(project.path("SPRING2.INI")));
    auto reopened = fillExtra3D(*project.io, NumericLocale::KnownC, twoFirstIds, twoSecondIds);
    REQUIRE(reopened); REQUIRE(reopened->spring); REQUIRE(reopened->spring->items.size() == 2);
    checkRow(reopened->spring->items[0]); checkSecondRow(reopened->spring->items[1]);
    CHECK_EQ(reopened->spring->currentRow, 0); CHECK(reopened->spring->visibleNumbers == visible);
    CHECK(applyExtra3D(*project.io, *reopened) == ApplyResult::Completed);
    const auto second = project.read("SPRING.INI"); project.retain("actual-second.bin", second);
    CHECK(second == expectedTwo); CHECK_EQ(project.warnings, 0); CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_two_rows_primary_open_failure_still_removes_companion)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, twoFirstIds, twoSecondIds);
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 2);
    constexpr std::string_view lateCompanion = "late readable two-row companion\r\n";
    project.write("SPRING2.INI", lateCompanion); project.retain("late-companion.bin", lateCompanion);
    {
        DenyPrimaryWrite denial(project.path("SPRING.INI"));
        SpringWriteTrace traced(*project.io);
        CHECK(applyExtra3D(traced, *form) == ApplyResult::Completed);
        CHECK((traced.events == std::vector<std::string>{"write:SPRING.INI", "opened:0", "remove:SPRING2.INI"}));
        const auto actual = project.read("SPRING.INI"); project.retain("actual-denied-main.bin", actual);
        project.retain("expected-denied-main.bin", twoRows);
        CHECK(actual == twoRows); CHECK(!fs::exists(project.path("SPRING2.INI")));
        checkSecondRow(form->spring->items[1]);
    }
    CHECK_EQ(project.warnings, 0); CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_two_rows_retain_selection_order_id_and_edit_guards)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    const std::array missingSecond{0, 40}; // Row0's selected ID20 is absent.
    CHECK(!fillExtra3D(*project.io, NumericLocale::KnownC, twoFirstIds, missingSecond));
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, twoFirstIds, twoSecondIds);
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 2);
    auto changed = *form;
    changed.spring->currentRow = -2;
    CHECK(applyExtra3D(*project.io, changed) == ApplyResult::UnsupportedSnapshot);
    changed = *form; changed.spring->comboLabels->firstCurrentIndex = 99;
    CHECK(applyExtra3D(*project.io, changed) == ApplyResult::UnsupportedSnapshot);
    changed = *form; changed.spring->comboLabels.reset();
    CHECK(applyExtra3D(*project.io, changed) == ApplyResult::UnsupportedSnapshot);
    changed = *form; changed.spring->items[1].firstValue = (std::numeric_limits<double>::quiet_NaN)();
    CHECK(applyExtra3D(*project.io, changed) == ApplyResult::UnsupportedSnapshot);
    changed = *form; changed.spring->visibleNumbers[0] = qt::L("1e9999");
    CHECK(applyExtra3D(*project.io, changed) == ApplyResult::UnsupportedSnapshot);
    CHECK(project.read("SPRING.INI") == twoRows);
    const auto countThree = std::string("3") + std::string(twoRows.substr(1));
    project.write("SPRING.INI", countThree); project.retain("unsupported-partial-three.bin", countThree);
    CHECK(unsupported([&] { (void)fillExtra3D(*project.io, NumericLocale::KnownC, twoFirstIds, twoSecondIds); }));
    CHECK(project.read("SPRING.INI") == countThree); CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_general_records_preserve_unselected_payload_and_normalize_selected_row)
{
    ProjectFiles project(generalRows, expectedGeneral); project.write("SPRING.INI", generalRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, generalFirstIds, generalSecondIds);
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 3);
    const auto& selected = *form->spring;
    CHECK_EQ(selected.currentRow, 0); CHECK_EQ(selected.firstSelectedId, 17); CHECK_EQ(selected.secondSelectedId, 19);
    CHECK(selected.items[0].firstValue == 1.23456789);
    const std::array visible{qt::L("1.23457"), qt::L("-2.34568"), qt::L("0.123457"), qt::L("-3.98765"), qt::L("0.03125")};
    CHECK(selected.visibleNumbers == visible);
    // These unselected foreign/repeated IDs are never looked up on no-click Save.
    CHECK_EQ(selected.items[1].firstObject, -7); CHECK_EQ(selected.items[1].secondObject, 111);
    CHECK_EQ(selected.items[2].firstObject, 17); CHECK_EQ(selected.items[2].secondObject, 19);
    const auto unselectedScalar = std::bit_cast<std::uint64_t>(selected.items[1].secondValue);
    const auto unselectedDirection = std::bit_cast<std::uint64_t>(selected.items[1].direction[0]);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    const auto first = project.read("SPRING.INI"); project.retain("actual-general.bin", first);
    CHECK(first == expectedGeneral); CHECK(first != generalRows);
    CHECK_EQ(std::bit_cast<std::uint64_t>(selected.items[1].secondValue), unselectedScalar);
    CHECK_EQ(std::bit_cast<std::uint64_t>(selected.items[1].direction[0]), unselectedDirection);
    auto reopened = fillExtra3D(*project.io, NumericLocale::KnownC, generalFirstIds, generalSecondIds);
    REQUIRE(reopened); REQUIRE(reopened->spring); REQUIRE(reopened->spring->items.size() == 3);
    const auto& rows = reopened->spring->items;
    CHECK(rows[0].firstValue == 1.23457); CHECK(rows[0].secondValue == -2.34568);
    CHECK_EQ(rows[1].firstObject, -7); CHECK_EQ(rows[1].secondObject, 111);
    CHECK(rows[1].secondValue == 42.1235); CHECK(rows[1].direction[0] == 9.87654);
    CHECK_EQ(rows[2].firstObject, 17); CHECK(rows[2].firstValue == 2.5);
    CHECK(applyExtra3D(*project.io, *reopened) == ApplyResult::Completed);
    const auto second = project.read("SPRING.INI"); project.retain("actual-general-second.bin", second);
    CHECK(second == expectedGeneral); CHECK(!fs::exists(project.path("SPRING2.INI"))); CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_candidate_count_cap_handles_256_without_count_fixture_admission)
{
    std::string input = "256\n", output = "256\t\n";
    for (int i = 0; i < 256; ++i) {
        input += "1 0.5 1.5 2 0 1 0\n";
        output += "1\t\n0.5\t1.5\t2\t0\t\n1\t0\t\n";
    }
    output += "\r\n";
    ProjectFiles project(input, output); project.write("SPRING.INI", input);
    const std::array ids{0, 1, 2};
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, ids, ids);
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 256);
    CHECK_EQ(form->spring->currentRow, 0);
    CHECK_EQ(form->spring->items.front().firstObject, 1); CHECK_EQ(form->spring->items.back().secondObject, 2);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    const auto actual = project.read("SPRING.INI"); project.retain("actual-cap.bin", actual);
    CHECK(actual == output);
    auto tooLarge = *form; tooLarge.spring->items.push_back(tooLarge.spring->items.back());
    CHECK(applyExtra3D(*project.io, tooLarge) == ApplyResult::UnsupportedSnapshot);
    CHECK(project.read("SPRING.INI") == output);
    const auto overCapInput = std::string("257") + input.substr(3);
    project.write("SPRING.INI", overCapInput); project.retain("unsupported-over-cap.bin", overCapInput);
    CHECK(unsupported([&] { (void)fillExtra3D(*project.io, NumericLocale::KnownC, ids, ids); }));
    CHECK(project.read("SPRING.INI") == overCapInput); CHECK(project.cleanup());
}

TEST_CASE(spring_native_page_generic_post_open_write_failure_closes_then_removes_companion)
{
    ProjectFiles project(generalRows, expectedGeneral); project.write("SPRING.INI", generalRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, generalFirstIds, generalSecondIds);
    REQUIRE(form); REQUIRE(form->spring);
    constexpr std::string_view lateCompanion = "late generic companion\r\n";
    project.write("SPRING2.INI", lateCompanion); project.retain("late-companion.bin", lateCompanion);
    SpringWriteTrace traced(*project.io); traced.failPrimaryAfterOpen = true;
    CHECK(applyExtra3D(traced, *form) == ApplyResult::Completed);
    CHECK((traced.events == std::vector<std::string>{"write:SPRING.INI", "opened:1", "close:SPRING.INI", "remove:SPRING2.INI"}));
    CHECK(traced.primaryWriteCalls > 0); CHECK_EQ(traced.primaryCloseCalls, 1U);
    CHECK(!fs::exists(project.path("SPRING2.INI")));
    // The injected device has no primary backing file: no native failure residue is asserted.
    CHECK(project.cleanup());
}

namespace {
SpringComboLabels canonicalLabels()
{
    return {{qt::L("0   - None"), qt::L("10  - Top Die"), qt::L("30  - Lower Die")},
        {qt::L("0   - None"), qt::L("20  - Workpiece"), qt::L("40  - Other")}, 0, 0};
}
}

TEST_CASE(spring_label_text_prefix_first_match_and_missing_id_preserve_current_index)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    auto labels = canonicalLabels();
    labels.first = {qt::L("bad"), qt::L("+010suffix"), qt::L("10 duplicate")};
    labels.second = {qt::L("0 - None"), qt::L("99suffix")};
    labels.firstCurrentIndex = 2; labels.secondCurrentIndex = 1;
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, labels);
    REQUIRE(form); REQUIRE(form->spring); auto& state = *form->spring;
    CHECK_EQ(state.comboLabels->firstCurrentIndex, 1); // First parsed match, not last or item data.
    CHECK_EQ(state.comboLabels->secondCurrentIndex, 1); // ID20 missing: retain selection99.
    CHECK_EQ(state.firstSelectedId, 10); CHECK_EQ(state.secondSelectedId, 99);
    state.comboLabels->firstCurrentIndex = 0; // Nonempty sscanf failure is initialized zero.
    auto saved = transitionSpringItem(state, 0, SpringItemEvent::SaveCurrentItem);
    CHECK(saved.outcome == ApplyResult::Completed); CHECK(saved.oldItemWritten); CHECK(saved.sameRow);
    CHECK_EQ(state.items[0].firstObject, 0); CHECK_EQ(state.items[0].secondObject, 99);
    state.comboLabels->first[0] = qt::L("");
    CHECK(transitionSpringItem(state, 0, SpringItemEvent::SaveCurrentItem).outcome == ApplyResult::Completed);
    CHECK_EQ(state.items[0].firstObject, 0);
    CHECK(project.cleanup());
}

TEST_CASE(spring_click_writes_edits_before_switch_and_same_row_save_does_not_reload)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); auto& state = *form->spring;
    state.visibleNumbers[0] = qt::L("1.23456789"); state.recordFlagChecked = true;
    state.comboLabels->firstCurrentIndex = 2;
    REQUIRE(setSpringListCurrentRow(state, 1));
    CHECK_EQ(state.currentRow, 0); CHECK(state.items[0].firstValue == 0.5); // Programmatic move only.
    const auto clicked = transitionSpringItem(state, 1, SpringItemEvent::ItemClicked);
    CHECK(clicked.outcome == ApplyResult::Completed); CHECK(clicked.oldItemWritten); CHECK(!clicked.sameRow);
    CHECK_EQ(state.currentRow, 1); CHECK(state.items[0].firstValue == 1.23456789);
    CHECK_EQ(state.items[0].firstObject, 30); CHECK_EQ(state.items[0].flag, 1);
    CHECK(state.visibleNumbers[0] == qt::L("2.5")); CHECK(!state.recordFlagChecked);
    state.visibleNumbers[1] = qt::L("-6.25"); state.recordFlagChecked = true;
    REQUIRE(setSpringListCurrentRow(state, 0));
    CHECK(transitionSpringItem(state, 0, SpringItemEvent::ItemClicked).outcome == ApplyResult::Completed);
    CHECK(state.items[1].secondValue == -6.25); CHECK_EQ(state.items[1].flag, 1);
    CHECK(state.visibleNumbers[0] == qt::L("1.23457")); CHECK(state.recordFlagChecked);
    state.visibleNumbers[0] = qt::L("8.7654321"); state.recordFlagChecked = false;
    const auto same = transitionSpringItem(state, 0, SpringItemEvent::SaveCurrentItem);
    CHECK(same.outcome == ApplyResult::Completed); CHECK(same.sameRow); CHECK(same.oldItemWritten);
    CHECK(state.items[0].firstValue == 8.7654321); CHECK_EQ(state.items[0].flag, 0);
    CHECK(state.visibleNumbers[0] == qt::L("8.7654321")); // Same-row returns before reload/rounding.
    const auto nullClick = transitionSpringItem(state, std::nullopt, SpringItemEvent::ItemClicked);
    CHECK(nullClick.outcome == ApplyResult::Completed); CHECK(!nullClick.oldItemWritten);
    CHECK(state.items[0].firstValue == 8.7654321); CHECK(project.cleanup());
}

TEST_CASE(spring_programmatic_list_move_save_commits_old_row_then_snapshots_all_items)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring);
    form->spring->visibleNumbers[0] = qt::L("6.25");
    REQUIRE(setSpringListCurrentRow(*form->spring, 1));
    CHECK_EQ(form->spring->currentRow, 0);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    const std::string expectedEdited =
        "2\t\n10\t\n6.25\t1.5\t20\t0\t\n1\t0\t\n30\t\n2.5\t3.5\t40\t1\t\n0\t1\t\n\r\n";
    CHECK(project.read("SPRING.INI") == expectedEdited);
    auto reopened = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(reopened); REQUIRE(reopened->spring);
    CHECK(reopened->spring->items[0].firstValue == 6.25); checkSecondRow(reopened->spring->items[1]);
    CHECK(project.cleanup());
}

TEST_CASE(spring_label_guards_retain_old_write_and_reached_combo_stage)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring);
    for (const auto& bad : {qt::String(std::u16string(64, u'x')), qt::L("2147483648suffix"), qt::L("30 - \u00e9")}) {
        auto state = *form->spring;
        state.visibleNumbers[0] = qt::L("9.25");
        state.comboLabels->second[2] = bad;
        REQUIRE(setSpringListCurrentRow(state, 1));
        const auto refused = transitionSpringItem(state, 1, SpringItemEvent::ItemClicked);
        CHECK(refused.outcome == ApplyResult::UnsupportedSnapshot);
        CHECK(refused.oldItemWritten); CHECK(refused.stage == SpringTransitionStage::FirstComboApplied);
        CHECK(state.items[0].firstValue == 9.25); CHECK_EQ(state.currentRow, 0);
        CHECK_EQ(state.comboLabels->firstCurrentIndex, 2); // First search already committed.
    }
    auto longest = canonicalLabels(); longest.first[1] = qt::String(u"10" + std::u16string(61, u'x'));
    CHECK(fillExtra3D(*project.io, NumericLocale::KnownC, longest).has_value()); // 63 + NUL.
    auto invalid = canonicalLabels(); invalid.first[1] = qt::String(std::u16string(64, u'x'));
    CHECK(!fillExtra3D(*project.io, NumericLocale::KnownC, invalid));
    CHECK(project.read("SPRING.INI") == twoRows); CHECK(project.cleanup());
}

TEST_CASE(spring_selected_flag_exact_one_normalizes_and_unselected_raw_flags_survive)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    project.write("SPRING2.INI", "2 -7");
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); CHECK(!form->spring->recordFlagChecked);
    CHECK_EQ(form->spring->items[0].flag, 2); CHECK_EQ(form->spring->items[1].flag, -7);
    SpringWriteTrace trace(*project.io);
    CHECK(applyExtra3D(trace, *form) == ApplyResult::Completed);
    CHECK(project.read("SPRING.INI") == expectedTwo); CHECK(project.read("SPRING2.INI") == "0\t-7\t\r\n");
    CHECK((trace.events == std::vector<std::string>{"write:SPRING.INI", "opened:1", "write:SPRING2.INI"}));
    auto reopened = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(reopened); REQUIRE(reopened->spring); CHECK_EQ(reopened->spring->items[1].flag, -7);
    project.write("SPRING2.INI", "1 0");
    form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); CHECK(form->spring->recordFlagChecked);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    CHECK(project.read("SPRING2.INI") == "1\t0\t\r\n");
    project.write("SPRING2.INI", "-1 0");
    form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); CHECK(!form->spring->recordFlagChecked);
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    CHECK(!fs::exists(project.path("SPRING2.INI"))); // any-nonzero is tested after selected normalization.
    CHECK(project.cleanup());
}

TEST_CASE(spring_nonzero_flags_primary_denied_skips_companion_then_companion_denied_does_not_rollback)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    project.write("SPRING2.INI", "0 2");
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring);
    {
        DenyPrimaryWrite denial(project.path("SPRING.INI")); SpringWriteTrace trace(*project.io);
        CHECK(applyExtra3D(trace, *form) == ApplyResult::Completed);
        CHECK((trace.events == std::vector<std::string>{"write:SPRING.INI", "opened:0"}));
        CHECK(project.read("SPRING.INI") == twoRows); CHECK(project.read("SPRING2.INI") == "0 2");
    }
    {
        DenyPrimaryWrite denial(project.path("SPRING2.INI")); SpringWriteTrace trace(*project.io);
        CHECK(applyExtra3D(trace, *form) == ApplyResult::Completed);
        CHECK((trace.events == std::vector<std::string>{"write:SPRING.INI", "opened:1", "write:SPRING2.INI"}));
        CHECK(project.read("SPRING.INI") == expectedTwo); CHECK(project.read("SPRING2.INI") == "0 2");
    }
    CHECK(project.cleanup());
}

namespace {
const std::vector<std::string> scalarAndSpringRemovals{
    "remove:NBC.DAT", "remove:SW2SP.DAT", "remove:STRETCH.DAT", "remove:B_WEIGHT.DAT",
    "remove:STRAIN_DST.DAT", "remove:SPRING.INI", "remove:SPRING2.INI"};
}

TEST_CASE(spring_unchecked_page_skips_poisoned_controls_and_preserves_payload)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring);
    project.write("SPRING2.INI", "late nonzero companion");
    auto& state = *form->spring;
    REQUIRE(state.items.size() == 2);
    state.pageChecked = false; state.comboLabels.reset();
    state.currentRow = -77; state.listCurrentRow = 99;
    state.visibleNumbers[0] = qt::L("not-a-number"); state.recordFlagChecked = true;
    state.items[0].flag = 2;
    state.items[0].firstValue = (std::numeric_limits<double>::quiet_NaN)();
    const auto payloadBits = std::bit_cast<std::uint64_t>(state.items[0].firstValue);
    // These values remain excluded when the positive-count record path is reached.
    auto rechecked = *form; rechecked.spring->pageChecked = true;
    SpringWriteTrace refused(*project.io); refused.recordAllOperations = true;
    CHECK(applyExtra3D(refused, rechecked) == ApplyResult::UnsupportedSnapshot);
    CHECK(refused.events.empty()); CHECK(project.read("SPRING.INI") == twoRows);
    CHECK(project.read("SPRING2.INI") == "late nonzero companion");
    SpringWriteTrace trace(*project.io); trace.recordAllOperations = true;
    CHECK(applyExtra3D(trace, *form) == ApplyResult::Completed);
    CHECK(trace.events == scalarAndSpringRemovals);
    CHECK(!fs::exists(project.path("SPRING.INI"))); CHECK(!fs::exists(project.path("SPRING2.INI")));
    CHECK(!state.pageChecked); CHECK(!state.comboLabels); CHECK_EQ(state.currentRow, -77);
    CHECK_EQ(state.listCurrentRow, 99); CHECK(state.visibleNumbers[0] == qt::L("not-a-number"));
    CHECK(state.recordFlagChecked); REQUIRE(state.items.size() == 2);
    CHECK_EQ(std::bit_cast<std::uint64_t>(state.items[0].firstValue), payloadBits);
    CHECK_EQ(state.items[0].flag, 2); checkSecondRow(state.items[1]);
    CHECK_EQ(project.warnings, 0); CHECK(project.cleanup());
}

TEST_CASE(spring_checked_empty_page_deletes_both_without_combo_or_selection_context)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); project.write("SPRING2.INI", "1");
    auto& state = *form->spring;
    state.items.clear(); state.pageChecked = true; state.comboLabels.reset();
    state.currentRow = 25; state.listCurrentRow = 25;
    state.visibleNumbers[0] = qt::L("1e9999"); state.recordFlagChecked = true;
    SpringWriteTrace trace(*project.io); trace.recordAllOperations = true;
    CHECK(applyExtra3D(trace, *form) == ApplyResult::Completed);
    CHECK(trace.events == scalarAndSpringRemovals);
    CHECK(!fs::exists(project.path("SPRING.INI"))); CHECK(!fs::exists(project.path("SPRING2.INI")));
    CHECK(state.pageChecked); CHECK(state.items.empty()); CHECK(!state.comboLabels);
    CHECK_EQ(state.currentRow, 25); CHECK_EQ(state.listCurrentRow, 25);
    CHECK(state.visibleNumbers[0] == qt::L("1e9999")); CHECK(state.recordFlagChecked);
    CHECK_EQ(project.warnings, 0); CHECK(project.cleanup());
}

TEST_CASE(spring_unchecked_primary_remove_failure_still_attempts_companion_without_rollback)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); form->spring->pageChecked = false;
    project.write("SPRING2.INI", "1");
    SpringWriteTrace trace(*project.io); trace.recordAllOperations = true; trace.denyPrimaryRemoval = true;
    CHECK(applyExtra3D(trace, *form) == ApplyResult::Completed);
    CHECK(trace.events == scalarAndSpringRemovals);
    CHECK(project.read("SPRING.INI") == inputRow); CHECK(!fs::exists(project.path("SPRING2.INI")));
    CHECK_EQ(project.warnings, 0); // Spring removal failure has no NBC warning route.
    checkRow(form->spring->items[0]); CHECK(project.cleanup());
}

TEST_CASE(spring_unchecked_gate_runs_after_failed_scalar_write_then_ordered_removals)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); form->spring->pageChecked = false;
    form->nbcEnabled = true; project.write("SPRING2.INI", "1");
    SpringWriteTrace trace(*project.io); trace.recordAllOperations = true; trace.failNbcOpen = true;
    CHECK(applyExtra3D(trace, *form) == ApplyResult::Completed);
    const std::vector<std::string> expectedOrder{"write:NBC.DAT", "remove:SW2SP.DAT", "remove:STRETCH.DAT",
        "remove:B_WEIGHT.DAT", "remove:STRAIN_DST.DAT", "remove:SPRING.INI", "remove:SPRING2.INI"};
    CHECK(trace.events == expectedOrder);
    CHECK(!fs::exists(project.path("SPRING.INI"))); CHECK(!fs::exists(project.path("SPRING2.INI")));
    CHECK_EQ(project.warnings, 0); checkRow(form->spring->items[0]); CHECK(project.cleanup());
}

TEST_CASE(spring_empty_and_null_numeric_edits_commit_positive_zero_in_each_field)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring);
    const std::array emptyTexts{qt::L(""), qt::String()};
    for (const auto& empty : emptyTexts) {
        for (std::size_t field = 0; field < 5; ++field) {
            auto state = *form->spring;
            state.visibleNumbers = {qt::L("2"), qt::L("3"), qt::L("4"), qt::L("5"), qt::L("6")};
            state.visibleNumbers[field] = empty; state.recordFlagChecked = true;
            const auto result = transitionSpringItem(state, 0, SpringItemEvent::SaveCurrentItem);
            CHECK(result.outcome == ApplyResult::Completed); CHECK(result.oldItemWritten); CHECK(result.sameRow);
            CHECK(result.stage == SpringTransitionStage::Complete);
            REQUIRE(state.items.size() == 1);
            const auto& row = state.items.front();
            const std::array values{row.firstValue, row.secondValue, row.direction[0], row.direction[1], row.direction[2]};
            for (std::size_t i = 0; i < values.size(); ++i) {
                if (i == field) CHECK_EQ(std::bit_cast<std::uint64_t>(values[i]), std::uint64_t{0});
                else CHECK(values[i] == static_cast<double>(i + 2));
            }
            CHECK_EQ(row.flag, 1); CHECK_EQ(row.firstObject, 10); CHECK_EQ(row.secondObject, 20);
            CHECK(state.visibleNumbers[field].isEmpty());
            CHECK(state.visibleNumbers[field].isNull() == empty.isNull()); // Same-row does not reload0.
        }
    }
    CHECK(project.read("SPRING.INI") == inputRow); CHECK(project.cleanup());
}

TEST_CASE(spring_empty_edit_click_roundtrip_commits_before_switch_then_reloads_zero)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); auto& state = *form->spring;
    REQUIRE(state.items.size() == 2); state.visibleNumbers[0] = qt::L("");
    REQUIRE(setSpringListCurrentRow(state, 1));
    CHECK(state.items[0].firstValue == 0.5); CHECK_EQ(state.currentRow, 0);
    const auto clicked = transitionSpringItem(state, 1, SpringItemEvent::ItemClicked);
    CHECK(clicked.outcome == ApplyResult::Completed); CHECK(clicked.oldItemWritten); CHECK(!clicked.sameRow);
    CHECK_EQ(std::bit_cast<std::uint64_t>(state.items[0].firstValue), std::uint64_t{0});
    CHECK(state.visibleNumbers[0] == qt::L("2.5")); CHECK_EQ(state.currentRow, 1);
    REQUIRE(setSpringListCurrentRow(state, 0));
    CHECK(transitionSpringItem(state, 0, SpringItemEvent::ItemClicked).outcome == ApplyResult::Completed);
    CHECK(state.visibleNumbers[0] == qt::L("0")); CHECK_EQ(state.currentRow, 0);
    checkSecondRow(state.items[1]); CHECK(project.read("SPRING.INI") == twoRows); CHECK(project.cleanup());
}

TEST_CASE(spring_empty_numeric_apply_writes_zero_bytes_and_roundtrips_selected_flag)
{
    constexpr std::string_view zeroOutput = "1\t\n10\t\n0\t0\t20\t0\t\n0\t0\t\n\r\n";
    ProjectFiles project(inputRow, zeroOutput); project.write("SPRING.INI", inputRow);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 1);
    form->spring->visibleNumbers.fill(qt::L("")); form->spring->visibleNumbers[1] = qt::String();
    form->spring->recordFlagChecked = true;
    CHECK(applyExtra3D(*project.io, *form) == ApplyResult::Completed);
    const auto actual = project.read("SPRING.INI"); project.retain("actual-empty-numeric.bin", actual);
    CHECK(actual == zeroOutput); CHECK(project.read("SPRING2.INI") == "1\t\r\n");
    checkRow(form->spring->items[0]); // Apply uses a private snapshot; caller item is unchanged.
    CHECK(form->spring->visibleNumbers[0].isEmpty()); CHECK(form->spring->visibleNumbers[1].isNull());
    auto reopened = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(reopened); REQUIRE(reopened->spring); REQUIRE(reopened->spring->items.size() == 1);
    const auto& row = reopened->spring->items[0];
    const std::array values{row.firstValue, row.secondValue, row.direction[0], row.direction[1], row.direction[2]};
    for (double value : values) CHECK_EQ(std::bit_cast<std::uint64_t>(value), std::uint64_t{0});
    CHECK_EQ(row.flag, 1); CHECK(reopened->spring->recordFlagChecked);
    for (const auto& text : reopened->spring->visibleNumbers) CHECK(text == qt::L("0"));
    CHECK(applyExtra3D(*project.io, *reopened) == ApplyResult::Completed);
    CHECK(project.read("SPRING.INI") == zeroOutput); CHECK(project.read("SPRING2.INI") == "1\t\r\n");
    CHECK(project.cleanup());
}

TEST_CASE(spring_empty_edit_does_not_admit_other_failed_or_nonfinite_numeric_text)
{
    ProjectFiles project; project.write("SPRING.INI", inputRow);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); REQUIRE(form->spring->items.size() == 1);
    const std::array invalidTexts{qt::L(" "), qt::L("\t"), qt::L("bad"), qt::L("1e9999"),
        qt::L("1e-9999"), qt::L("nan"), qt::L("inf"), qt::String(std::u16string(1, u'\0'))};
    for (const auto& invalid : invalidTexts) {
        auto changed = *form; changed.spring->visibleNumbers[0] = qt::L("");
        changed.spring->visibleNumbers[4] = invalid;
        auto state = *changed.spring;
        const auto refused = transitionSpringItem(state, 0, SpringItemEvent::SaveCurrentItem);
        CHECK(refused.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!refused.oldItemWritten);
        CHECK(refused.stage == SpringTransitionStage::Preflight); checkRow(state.items[0]);
        SpringWriteTrace trace(*project.io); trace.recordAllOperations = true;
        CHECK(applyExtra3D(trace, changed) == ApplyResult::UnsupportedSnapshot);
        CHECK(trace.events.empty()); CHECK(project.read("SPRING.INI") == inputRow);
        CHECK(!fs::exists(project.path("SPRING2.INI")));
    }
    CHECK(project.cleanup());
}

TEST_CASE(spring_empty_edit_old_write_survives_later_target_label_refusal)
{
    ProjectFiles project(twoRows, expectedTwo); project.write("SPRING.INI", twoRows);
    auto form = fillExtra3D(*project.io, NumericLocale::KnownC, canonicalLabels());
    REQUIRE(form); REQUIRE(form->spring); auto& state = *form->spring;
    REQUIRE(state.items.size() == 2);
    state.visibleNumbers[0] = qt::String();
    state.comboLabels->second[2] = qt::String(std::u16string(64, u'x'));
    REQUIRE(setSpringListCurrentRow(state, 1));
    const auto result = transitionSpringItem(state, 1, SpringItemEvent::ItemClicked);
    CHECK(result.outcome == ApplyResult::UnsupportedSnapshot); CHECK(result.oldItemWritten);
    CHECK(result.stage == SpringTransitionStage::FirstComboApplied);
    CHECK_EQ(std::bit_cast<std::uint64_t>(state.items[0].firstValue), std::uint64_t{0});
    CHECK_EQ(state.currentRow, 0); CHECK_EQ(state.comboLabels->firstCurrentIndex, 2);
    CHECK(state.visibleNumbers[0].isNull()); checkSecondRow(state.items[1]);
    CHECK(project.read("SPRING.INI") == twoRows); CHECK(project.cleanup());
}

int main() { return testkit::run_all(); }
