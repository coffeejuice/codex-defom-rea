// ctest-labels: m2m-mdt
#include <testkit/check.hpp>
#include "m2m/engine/simcontrol300_remesh.hpp"
#include <bit>
#include <map>
#include "../support/test_paths.hpp"
#include <functional>
#include <fstream>
#include <iterator>

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace {
std::uint64_t bits(double value) { return std::bit_cast<std::uint64_t>(value); }
struct MemoryIo final : RemeshIo {
    std::map<std::string, std::string> files;
    std::vector<std::string> events;
    struct File final : RemeshReadFile {
        MemoryIo& io; std::string name, raw; datafiles::TextFile text;
        bool closed = false;
        File(MemoryIo& io, std::string name, std::string raw)
            : io(io), name(std::move(name)), raw(std::move(raw)), text(this->raw) {}
        datafiles::TextFile& tokens() override { return text; }
        void rewind() override { io.events.push_back("rewind:" + name); text = datafiles::TextFile(raw); }
        void close() override { CHECK(!closed); closed = true; io.events.push_back("close:" + name); }
        ~File() override { CHECK(closed); }
    };
    std::unique_ptr<RemeshReadFile> openRead(std::string_view name) override
    {
        events.push_back("open:" + std::string(name));
        auto it = files.find(std::string(name));
        if (it == files.end()) return {};
        return std::make_unique<File>(*this, it->first, it->second);
    }
};
void fallback(MemoryIo& io, const std::string& bytes) { io.files["LOC_DEN_CTRL.DAT"] = bytes; }
struct DiskScratch {
    std::filesystem::path dir;
    DiskScratch() {
        static unsigned sequence = 0;
        dir = m2m_test::scratchOutput(m2m_test::scratchRoot() / ("remesh" + std::to_string(++sequence)));
        std::filesystem::create_directories(dir);
    }
    ~DiskScratch() { std::error_code ec; std::filesystem::remove_all(m2m_test::scratchOutput(dir), ec); }
    std::filesystem::path path(const char* name) const { return dir / name; }
    void write(const char* name, const std::string& bytes) {
        HANDLE file = CreateFileW(path(name).c_str(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                  nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        REQUIRE(file != INVALID_HANDLE_VALUE);
        DWORD written = 0;
        const bool ok = WriteFile(file, bytes.data(), static_cast<DWORD>(bytes.size()), &written, nullptr) != 0;
        CloseHandle(file);
        CHECK(ok); CHECK_EQ(written, bytes.size());
    }
    std::unique_ptr<RemeshIo> io() const { return makeNativeRemeshIo(dir.string()); }
};
bool unsupported(const std::function<void()>& action) {
    try { action(); } catch (const Error& e) { return e.code == M2M_E_UNSUPPORTED; }
    return false;
}
std::string readBytes(const std::filesystem::path& path) {
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}
}

TEST_CASE(remesh_raw_prefix_preserves_double_suffix_and_other_fields_without_numeric_parse)
{
    RemeshState state; state.uniformInternalEdge = 1.25; state.improveContact = true;
    state.method = 4; state.filterDistance = 2.5; state.scalingFactor = 3.5; state.enabled = true;
    CHECK(applyRemeshUniformEdgeToken(state, "1") == ApplyResult::Completed);
    CHECK_EQ(bits(state.uniformInternalEdge), 0x3ff4000000000031ULL);
    CHECK(state.improveContact); CHECK_EQ(state.method, 4); CHECK(state.filterDistance == 2.5);
    CHECK(state.scalingFactor == 3.5); CHECK(state.goodInternalChangeable);
    CHECK(state.goodSurfaceChangeable); CHECK(state.enabled);
    state.uniformInternalEdge = std::bit_cast<double>(0x7ff8000000000031ULL);
    CHECK(applyRemeshUniformEdgeToken(state, "") == ApplyResult::Completed);
    CHECK_EQ(bits(state.uniformInternalEdge), 0x7ff8000000000000ULL); // No floating arithmetic.
}

TEST_CASE(remesh_eight_character_token_clears_contact_then_duplicate_preserves_new_suffix)
{
    RemeshState state; state.uniformInternalEdge = 1.25; state.improveContact = true;
    CHECK(applyRemeshUniformEdgeToken(state, "0.000000") == ApplyResult::Completed);
    CHECK_EQ(bits(state.uniformInternalEdge), 0x3030303030302e30ULL);
    CHECK(!state.improveContact); // Original terminating NUL writes known byte+18.
    CHECK(applyRemeshUniformEdgeToken(state, "1") == ApplyResult::Completed);
    CHECK_EQ(bits(state.uniformInternalEdge), 0x3030303030300031ULL);
    CHECK(!state.improveContact);
}

TEST_CASE(remesh_wider_raw_spill_and_non_source_embedded_nul_refuse_before_helper_writes)
{
    RemeshState state; state.uniformInternalEdge = 1.25; state.improveContact = true;
    state.contactToScalingBytes[2] = std::byte{0x77}; state.goodSurfaceChangeable = 53;
    state.enabled = 1;
    CHECK(applyRemeshUniformEdgeToken(state, "12345678901234567890123456") == ApplyResult::UnsupportedSnapshot);
    CHECK(applyRemeshUniformEdgeToken(state, std::string_view("1\0X", 3)) == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(bits(state.uniformInternalEdge), 0x3ff4000000000000ULL); CHECK(state.improveContact);
    CHECK(state.contactToScalingBytes[2] == std::byte{0x77});
    CHECK_EQ(bits(state.scalingFactor), 0x3ff3333333333333ULL);
    CHECK_EQ(state.goodSurfaceChangeable, 53); CHECK_EQ(state.enabled, 1);
}

TEST_CASE(remesh_nine_character_token_preserves_raw_contact_and_untouched_intervening_bytes)
{
    RemeshState state; state.uniformInternalEdge = 1.25; state.improveContact = 1; state.enabled = 1;
    CHECK((state.contactToScalingBytes == std::array<std::byte, 7>{}));
    state.contactToScalingBytes = {std::byte{0x11}, std::byte{0x22}, std::byte{0x33},
        std::byte{0x44}, std::byte{0x55}, std::byte{0x66}, std::byte{0x77}};
    REQUIRE(applyRemeshUniformEdgeToken(state, "12.500000") == ApplyResult::Completed);
    CHECK_EQ(bits(state.uniformInternalEdge), 0x30303030352e3231ULL);
    CHECK_EQ(state.improveContact, 48); // Original Save uses movzx, not Boolean normalization.
    CHECK((state.contactToScalingBytes == std::array{std::byte{0}, std::byte{0x22},
        std::byte{0x33}, std::byte{0x44}, std::byte{0x55}, std::byte{0x66}, std::byte{0x77}}));
    CHECK_EQ(bits(state.scalingFactor), 0x3ff3333333333333ULL);
    CHECK_EQ(state.goodSurfaceChangeable, 1); CHECK_EQ(state.enabled, 1);
    REQUIRE(applyRemeshUniformEdgeToken(state, "1") == ApplyResult::Completed);
    CHECK_EQ(bits(state.uniformInternalEdge), 0x30303030352e0031ULL);
    CHECK_EQ(state.improveContact, 48); CHECK(state.contactToScalingBytes[6] == std::byte{0x77});
}

TEST_CASE(remesh_raw_span_reaches_scaling_surface_and_enabled_in_original_byte_order)
{
    struct Vector { const char* token; std::uint64_t scaling; unsigned surface, enabled; };
    const Vector vectors[]{
        {"1234567890123456", 0x3ff3333333333300ULL, 1, 1},
        {"123456789012345678901234", 0x3433323130393837ULL, 0, 1},
        {"1234567890123456789012345", 0x3433323130393837ULL, 53, 0}};
    for (const auto& vector : vectors) {
        RemeshState state; state.uniformInternalEdge = 1.25; state.improveContact = 1; state.enabled = 1;
        REQUIRE(applyRemeshUniformEdgeToken(state, vector.token) == ApplyResult::Completed);
        CHECK_EQ(bits(state.uniformInternalEdge), 0x3837363534333231ULL);
        CHECK_EQ(state.improveContact, 57);
        CHECK((state.contactToScalingBytes == std::array{std::byte{0x30}, std::byte{0x31},
            std::byte{0x32}, std::byte{0x33}, std::byte{0x34}, std::byte{0x35}, std::byte{0x36}}));
        CHECK_EQ(bits(state.scalingFactor), vector.scaling);
        CHECK_EQ(state.goodSurfaceChangeable, vector.surface); CHECK_EQ(state.enabled, vector.enabled);
    }
}

TEST_CASE(remesh_reader_retains_raw_flags_until_later_keywords_and_open_assigns_enabled_last)
{
    MemoryIo io; fallback(io, "UNIFORM_INTERNAL_EDGE_LENGTH 1234567890123456789012345\nDONE\n");
    RemeshState direct; direct.enabled = 1;
    auto report = readRemesh(direct, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK_EQ(direct.improveContact, 57); CHECK_EQ(direct.goodSurfaceChangeable, 53); CHECK_EQ(direct.enabled, 0);
    RemeshState opened;
    report = loadRemeshAtProjectOpen(opened, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK_EQ(opened.improveContact, 57); CHECK_EQ(opened.goodSurfaceChangeable, 53); CHECK_EQ(opened.enabled, 1);
    fallback(io, "UNIFORM_INTERNAL_EDGE_LENGTH 1234567890123456789012345\n"
        "FURTHER_IMPROVE_CONTACT_ELEMENTS -2\nGOOD_SURFACE_ELEMENT_CHANGEABLE 0\nDONE\n");
    report = loadRemeshAtProjectOpen(opened, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK_EQ(opened.improveContact, 1); CHECK_EQ(opened.goodSurfaceChangeable, 0); CHECK_EQ(opened.enabled, 1);
}

TEST_CASE(remesh_missing_files_return_false_and_project_open_assigns_enabled_after_return)
{
    RemeshState state; state.enabled = true; state.method = 4; MemoryIo io;
    auto read = readRemesh(state, io, NumericLocale::KnownC);
    CHECK(read.outcome == ApplyResult::Completed); REQUIRE(read.loaded); CHECK(!*read.loaded);
    CHECK(state.enabled); CHECK_EQ(state.method, 4); // Reader itself does not assign enabled.
    CHECK((io.events == std::vector<std::string>{"open:LOCAL_REMESH.DAT", "open:LOC_DEN_CTRL.DAT"}));
    io.events.clear();
    auto open = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    REQUIRE(open.loaded); CHECK(!*open.loaded); CHECK(!state.enabled); CHECK_EQ(state.method, 4);
}

TEST_CASE(remesh_marker_probe_rewinds_unmarked_local_before_empty_fallback_success)
{
    RemeshState state; state.method = 4; MemoryIo io;
    io.files["LOCAL_REMESH.DAT"] = "FILTER_DISTANCE 99\r\nDONE\r\n";
    fallback(io, "");
    const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK(state.enabled); CHECK_EQ(state.method, 4); CHECK(state.filterDistance == 0.0);
    CHECK((io.events == std::vector<std::string>{"open:LOCAL_REMESH.DAT", "rewind:LOCAL_REMESH.DAT",
        "close:LOCAL_REMESH.DAT", "open:LOC_DEN_CTRL.DAT", "close:LOC_DEN_CTRL.DAT"}));
}

TEST_CASE(remesh_keyword_marker_selects_primary_then_reader_replays_earlier_lines_and_scalar_order)
{
    RemeshState state; MemoryIo io;
    io.files["LOCAL_REMESH.DAT"] = "filter_distance 1.25D-3\r\nKEYWORD_DRIVEN\r\n"
        "* skip\r\ninternal_element_size_control_method 99\r\nGOOD_INTERNAL_ELEMENT_CHANGEABLE -2\r\n"
        "FURTHER_IMPROVE_CONTACT_ELEMENTS 3tail\r\nSCALING_FACTOR_EACH_LAYER_INWARD 20\r\n"
        "GOOD_SURFACE_ELEMENT_CHANGEABLE 0\r\nDONE\r\nFILTER_DISTANCE 77\r\n";
    fallback(io, "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 0\nDONE\n");
    const auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK_EQ(state.method, 5); CHECK(state.filterDistance == 0.00125); CHECK(state.goodInternalChangeable);
    CHECK(state.improveContact); CHECK(state.scalingFactor == 10.0); CHECK(!state.goodSurfaceChangeable);
    CHECK((io.events == std::vector<std::string>{"open:LOCAL_REMESH.DAT", "rewind:LOCAL_REMESH.DAT", "close:LOCAL_REMESH.DAT"}));
}

TEST_CASE(remesh_raw_uses_next_current_token_or_whole_fresh_raw_line)
{
    RemeshState sameLine, freshLine; MemoryIo a, b;
    fallback(a, "UNIFORM_INTERNAL_EDGE_LENGTH 1 2\nDONE\n");
    fallback(b, "UNIFORM_INTERNAL_EDGE_LENGTH\n1 2\nDONE\n");
    REQUIRE(readRemesh(sameLine, a, NumericLocale::KnownC).loaded);
    REQUIRE(readRemesh(freshLine, b, NumericLocale::KnownC).loaded);
    CHECK_EQ(bits(sameLine.uniformInternalEdge), 0x31ULL);
    CHECK_EQ(bits(freshLine.uniformInternalEdge), 0x322031ULL);
}

TEST_CASE(remesh_late_unsupported_token_preserves_prior_stores_and_does_not_set_enabled)
{
    RemeshState state; state.goodSurfaceChangeable = true; MemoryIo io;
    fallback(io, "UNIFORM_INTERNAL_EDGE_LENGTH 12.500000\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\n"
        "UNIFORM_INTERNAL_EDGE_LENGTH 12345678901234567890123456\n"
        "GOOD_SURFACE_ELEMENT_CHANGEABLE 0\nDONE\n");
    auto report = loadRemeshAtProjectOpen(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!report.loaded);
    CHECK_EQ(state.method, 4); CHECK_EQ(bits(state.uniformInternalEdge), 0x30303030352e3231ULL);
    CHECK_EQ(state.improveContact, 48);
    CHECK(state.goodSurfaceChangeable); CHECK(!state.enabled);
    CHECK_EQ(io.events.back(), "close:LOC_DEN_CTRL.DAT");
}

TEST_CASE(remesh_missing_integer_uses_existing_shared_local_but_refuses_its_first_indeterminate_use)
{
    RemeshState state; state.goodInternalChangeable = false; MemoryIo io;
    fallback(io, "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\nGOOD_INTERNAL_ELEMENT_CHANGEABLE");
    auto report = readRemesh(state, io, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.loaded); CHECK(*report.loaded);
    CHECK_EQ(state.method, 4); CHECK(state.goodInternalChangeable);
    RemeshState absent; absent.goodInternalChangeable = false; MemoryIo missing;
    fallback(missing, "GOOD_INTERNAL_ELEMENT_CHANGEABLE");
    report = readRemesh(absent, missing, NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!report.loaded);
    CHECK(!absent.goodInternalChangeable);
}

TEST_CASE(remesh_missing_real_keeps_destination_before_original_scaling_clamp)
{
    RemeshState state; state.filterDistance = 3.5; MemoryIo io;
    fallback(io, "FILTER_DISTANCE");
    REQUIRE(readRemesh(state, io, NumericLocale::KnownC).loaded);
    CHECK(state.filterDistance == 3.5);
    RemeshState scaling; scaling.scalingFactor = 20.0; MemoryIo missing;
    fallback(missing, "SCALING_FACTOR_EACH_LAYER_INWARD");
    REQUIRE(readRemesh(scaling, missing, NumericLocale::KnownC).loaded);
    CHECK(scaling.scalingFactor == 10.0);
    MemoryIo low; fallback(low, "FILTER_DISTANCE invalid\nSCALING_FACTOR_EACH_LAYER_INWARD 0.5\nDONE\n");
    REQUIRE(readRemesh(scaling, low, NumericLocale::KnownC).loaded);
    CHECK(scaling.filterDistance == 0.0); CHECK(scaling.scalingFactor == 1.0);
}

TEST_CASE(remesh_unrecovered_numeric_profiles_refuse_after_prefix_and_unknown_locale_opens_nothing)
{
    for (const std::string bad : {"2147483648", "-2147483649"}) {
        RemeshState state; MemoryIo io;
        fallback(io, "FILTER_DISTANCE 3\nINTERNAL_ELEMENT_SIZE_CONTROL_METHOD " + bad + "\nDONE\n");
        const auto report = readRemesh(state, io, NumericLocale::KnownC);
        CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); CHECK(state.filterDistance == 3.0);
        CHECK_EQ(state.method, 1);
    }
    for (const std::string bad : {"0x1p2", "INF", "1e9999"}) {
        RemeshState state; MemoryIo io; fallback(io, "FILTER_DISTANCE " + bad + "\nDONE\n");
        CHECK(readRemesh(state, io, NumericLocale::KnownC).outcome == ApplyResult::UnsupportedSnapshot);
        CHECK(state.filterDistance == 0.0);
    }
    RemeshState state; MemoryIo io;
    CHECK(readRemesh(state, io, NumericLocale::Unrecovered).outcome == ApplyResult::UnsupportedSnapshot);
    CHECK(io.events.empty());
}

TEST_CASE(remesh_borrowed_textfile_delegates_fgets_and_refuses_unavailable_cursor_operations)
{
    int calls = 0;
    auto text = datafiles::TextFile::fromLineReader([&](std::string& out, int n) {
        ++calls; CHECK_EQ(n, 5); if (calls > 1) return false; out = "live"; return true;
    });
    std::string out = "before";
    CHECK(text.fgets(out, 5)); CHECK_EQ(out, "live");
    CHECK(!text.fgets(out, 5)); CHECK_EQ(out, "live");
    CHECK(unsupported([&] { (void)text.eof(); }));
    CHECK(unsupported([&] { (void)text.getc(); }));
    CHECK(unsupported([&] { text.ungetc(); }));
    CHECK(unsupported([&] { (void)text.pos(); }));
    CHECK(unsupported([&] { (void)text.data(); }));
    CHECK_EQ(calls, 2);
}

TEST_CASE(remesh_native_missing_empty_fallback_and_marked_primary_match_memory)
{
    DiskScratch disk; auto io = disk.io(); RemeshState missing;
    const auto absent = loadRemeshAtProjectOpen(missing, *io, NumericLocale::KnownC);
    REQUIRE(absent.loaded); CHECK(!*absent.loaded); CHECK(!missing.enabled);
    disk.write("LOC_DEN_CTRL.DAT", "");
    REQUIRE(loadRemeshAtProjectOpen(missing, *io, NumericLocale::KnownC).loaded);
    CHECK(missing.enabled);
    const std::string bytes = "FILTER_DISTANCE 3.25D-2\r\nKEYWORD_DRIVEN\r\n"
        "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\r\nUNIFORM_INTERNAL_EDGE_LENGTH 1\r\n"
        "GOOD_SURFACE_ELEMENT_CHANGEABLE 0\r\nSCALING_FACTOR_EACH_LAYER_INWARD 12\r\nDONE\r\n";
    disk.write("LOCAL_REMESH.DAT", bytes);
    MemoryIo memory; memory.files["LOCAL_REMESH.DAT"] = bytes;
    RemeshState native, expected;
    REQUIRE(loadRemeshAtProjectOpen(native, *io, NumericLocale::KnownC).loaded);
    REQUIRE(loadRemeshAtProjectOpen(expected, memory, NumericLocale::KnownC).loaded);
    CHECK_EQ(native.method, expected.method); CHECK(native.filterDistance == expected.filterDistance);
    CHECK_EQ(bits(native.uniformInternalEdge), bits(expected.uniformInternalEdge));
    CHECK_EQ(native.goodSurfaceChangeable, expected.goodSurfaceChangeable);
    CHECK(native.scalingFactor == expected.scalingFactor); CHECK(native.enabled);
}

TEST_CASE(remesh_native_fgets_text_translation_nul_consumption_eof_and_rewind)
{
    DiskScratch disk;
    disk.write("LOCAL_REMESH.DAT", std::string("A\r\nB\rC\nD\0hidden\nE\x1a" "after", 23));
    auto io = disk.io(); auto file = io->openRead("LOCAL_REMESH.DAT"); REQUIRE(file);
    std::string out;
    CHECK(file->tokens().fgets(out, 2000)); CHECK_EQ(out, "A\n");
    CHECK(file->tokens().fgets(out, 2000)); CHECK_EQ(out, "B\rC\n");
    CHECK(file->tokens().fgets(out, 2000)); CHECK_EQ(out, "D");
    CHECK(file->tokens().fgets(out, 2000)); CHECK_EQ(out, "E");
    out = "sentinel"; CHECK(!file->tokens().fgets(out, 2000)); CHECK_EQ(out, "sentinel");
    file->rewind(); CHECK(file->tokens().fgets(out, 2000)); CHECK_EQ(out, "A\n");
    file->close(); CHECK(unsupported([&] { file->rewind(); }));
}

TEST_CASE(remesh_native_open_does_not_preread_and_fgets_retains_remaining_chunk)
{
    DiskScratch disk; disk.write("LOCAL_REMESH.DAT", "OLD\n");
    auto io = disk.io(); auto file = io->openRead("LOCAL_REMESH.DAT"); REQUIRE(file);
    disk.write("LOCAL_REMESH.DAT", "NEW\n" + std::string(2001, 'a') + "\n");
    std::string out;
    CHECK(file->tokens().fgets(out, 4)); CHECK_EQ(out, "NEW");
    CHECK(file->tokens().fgets(out, 4)); CHECK_EQ(out, "\n");
    CHECK(file->tokens().fgets(out, 2000)); CHECK_EQ(out, std::string(1999, 'a'));
    CHECK(file->tokens().fgets(out, 2000)); CHECK_EQ(out, "aa\n");
    CHECK(!file->tokens().fgets(out, 2000));
}

TEST_CASE(remesh_native_reader_shares_writes_denies_delete_and_releases_handle)
{
    DiskScratch disk; disk.write("LOCAL_REMESH.DAT", "DONE\n"); auto io = disk.io();
    auto file = io->openRead("LOCAL_REMESH.DAT"); REQUIRE(file);
    HANDLE writer = CreateFileW(disk.path("LOCAL_REMESH.DAT").c_str(), GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    CHECK(writer != INVALID_HANDLE_VALUE); if (writer != INVALID_HANDLE_VALUE) CloseHandle(writer);
    CHECK(!DeleteFileW(disk.path("LOCAL_REMESH.DAT").c_str()));
    CHECK_EQ(GetLastError(), static_cast<DWORD>(ERROR_SHARING_VIOLATION));
    file.reset(); CHECK(DeleteFileW(disk.path("LOCAL_REMESH.DAT").c_str()));
}

TEST_CASE(remesh_native_denied_primary_falls_back_and_unmarked_primary_closes_before_fallback)
{
    DiskScratch disk; disk.write("LOCAL_REMESH.DAT", "DONE\n");
    disk.write("LOC_DEN_CTRL.DAT", "FILTER_DISTANCE 7\nDONE\n"); auto io = disk.io();
    HANDLE deny = CreateFileW(disk.path("LOCAL_REMESH.DAT").c_str(), GENERIC_READ, 0,
                              nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    REQUIRE(deny != INVALID_HANDLE_VALUE);
    RemeshState state; const auto result = readRemesh(state, *io, NumericLocale::KnownC);
    CloseHandle(deny); REQUIRE(result.loaded); CHECK(state.filterDistance == 7.0);
    struct ObserveFallback final : RemeshIo {
        RemeshIo& delegate; DiskScratch& disk; bool observed = false;
        ObserveFallback(RemeshIo& delegate, DiskScratch& disk) : delegate(delegate), disk(disk) {}
        std::unique_ptr<RemeshReadFile> openRead(std::string_view name) override {
            if (name == "LOC_DEN_CTRL.DAT") {
                observed = true;
                CHECK(MoveFileW(disk.path("LOCAL_REMESH.DAT").c_str(), disk.path("closed.DAT").c_str()));
            }
            return delegate.openRead(name);
        }
    } observed(*io, disk);
    REQUIRE(readRemesh(state, observed, NumericLocale::KnownC).loaded); CHECK(observed.observed);
}

TEST_CASE(remesh_reached_chunk_guards_keep_partial_effects_and_do_not_scan_beyond_done)
{
    DiskScratch disk; auto io = disk.io();
    const std::string prefix = "INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4\n";
    disk.write("LOC_DEN_CTRL.DAT", prefix + "\x80\n");
    RemeshState state;
    const auto result = loadRemeshAtProjectOpen(state, *io, NumericLocale::KnownC);
    CHECK(result.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!result.loaded);
    CHECK_EQ(state.method, 4); CHECK(!state.enabled);
    disk.write("LOC_DEN_CTRL.DAT", prefix + "DONE\n\x80\n");
    REQUIRE(loadRemeshAtProjectOpen(state, *io, NumericLocale::KnownC).loaded); CHECK(state.enabled);
    std::string tokens;
    for (int i = 0; i < 128; ++i) tokens += "x ";
    MemoryIo safe; fallback(safe, tokens + "\nDONE\n");
    REQUIRE(readRemesh(state, safe, NumericLocale::KnownC).loaded);
    disk.write("LOC_DEN_CTRL.DAT", tokens + "\nDONE\n");
    REQUIRE(readRemesh(state, *io, NumericLocale::KnownC).loaded);
    MemoryIo excess; fallback(excess, prefix + tokens + "x\n");
    state.method = 1;
    CHECK(readRemesh(state, excess, NumericLocale::KnownC).outcome == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(state.method, 4); CHECK_EQ(excess.events.back(), "close:LOC_DEN_CTRL.DAT");
    disk.write("LOC_DEN_CTRL.DAT", prefix + tokens + "x\n"); state.method = 1;
    CHECK(readRemesh(state, *io, NumericLocale::KnownC).outcome == ApplyResult::UnsupportedSnapshot);
    CHECK_EQ(state.method, 4);
}

TEST_CASE(remesh_native_path_and_fixed_name_profile_fail_before_unqualified_open)
{
    for (const std::string path : {"relative", "\\\\server\\share", "\\\\?\\C:\\test", "C:\\bad\x80", "C:\\bad?"})
        CHECK(unsupported([&] { (void)makeNativeRemeshIo(path); }));
    CHECK(unsupported([&] { (void)makeNativeRemeshIo("C:\\" + std::string(250, 'x')); }));
    DiskScratch disk; auto io = disk.io();
    bool rejected = false;
    try { (void)io->openRead("unrelated.txt"); } catch (const Error& e) { rejected = e.code == M2M_E_ARG; }
    CHECK(rejected);
}

TEST_CASE(remesh_native_save_matches_original_literal_derived_default_bytes_and_closes)
{
    DiskScratch disk; disk.write("LOCAL_REMESH.DAT", std::string(3000, 'x'));
    RemeshState owner; owner.enabled = true;
    const auto report = saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::KnownC);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.opened); CHECK(*report.opened);
    CHECK(!report.removalSucceeded); CHECK(!report.invalidParameterReached);
    const auto expectedPath = testkit::repo_root() / "apps/moproj2mst/tests/mdt/remesh_save_default.txt";
    REQUIRE(std::filesystem::exists(expectedPath));
    CHECK_EQ(readBytes(disk.path("LOCAL_REMESH.DAT")), readBytes(expectedPath));
    CHECK(MoveFileW(disk.path("LOCAL_REMESH.DAT").c_str(), disk.path("closed.DAT").c_str()));
    CHECK(owner.enabled); CHECK_EQ(owner.method, 1); CHECK(owner.uniformInternalEdge == 0.0);
}

TEST_CASE(remesh_native_save_uses_current_state_and_legacy_fixed_numeric_format)
{
    DiskScratch disk; RemeshState owner; owner.enabled = true; owner.method = 5;
    owner.goodInternalChangeable = false; owner.filterDistance = 1e23;
    owner.uniformInternalEdge = 12.5; owner.improveContact = true; owner.scalingFactor = -0.0;
    owner.goodSurfaceChangeable = false;
    REQUIRE(saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::KnownC).opened);
    const auto bytes = readBytes(disk.path("LOCAL_REMESH.DAT"));
    CHECK(bytes.find("INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 5\r\n") != std::string::npos);
    CHECK(bytes.find("GOOD_INTERNAL_ELEMENT_CHANGEABLE 0\r\n") != std::string::npos);
    CHECK(bytes.find("FILTER_DISTANCE 99999999999999992000000.000000\r\n") != std::string::npos);
    CHECK(bytes.find("UNIFORM_INTERNAL_EDGE_LENGTH 12.500000\r\n") != std::string::npos);
    CHECK(bytes.find("FURTHER_IMPROVE_CONTACT_ELEMENTS 1\r\n") != std::string::npos);
    CHECK(bytes.find("SCALING_FACTOR_EACH_LAYER_INWARD -0.000000\r\n") != std::string::npos);
    CHECK(bytes.find("GOOD_SURFACE_ELEMENT_CHANGEABLE 0\r\n") != std::string::npos);
    CHECK(bytes.find("DONE") == std::string::npos);
    // The nine-byte writer output uses the recovered raw span. The later
    // contact keyword replaces the temporary ASCII 48 with normalized 1.
    auto io = disk.io(); RemeshState reopened;
    const auto read = loadRemeshAtProjectOpen(reopened, *io, NumericLocale::KnownC);
    CHECK(read.outcome == ApplyResult::Completed); REQUIRE(read.loaded); CHECK(*read.loaded);
    CHECK_EQ(reopened.method, 5); CHECK_EQ(bits(reopened.uniformInternalEdge), 0x30303030352e3231ULL);
    CHECK_EQ(reopened.improveContact, 1); CHECK_EQ(reopened.goodSurfaceChangeable, 0);
    CHECK(reopened.scalingFactor == 1.0); CHECK(reopened.enabled);
}

TEST_CASE(remesh_native_save_prints_overwritten_contact_and_surface_bytes_without_normalizing)
{
    DiskScratch disk; MemoryIo io;
    fallback(io, "UNIFORM_INTERNAL_EDGE_LENGTH 1234567890123456789012345\n"
        "UNIFORM_INTERNAL_EDGE_LENGTH 12.500000\nDONE\n");
    RemeshState owner;
    const auto opened = loadRemeshAtProjectOpen(owner, io, NumericLocale::KnownC);
    REQUIRE(opened.loaded); REQUIRE(*opened.loaded); CHECK(owner.enabled);
    REQUIRE(owner.improveContact == 48); REQUIRE(owner.goodSurfaceChangeable == 53);
    const auto saved = saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::KnownC);
    CHECK(saved.outcome == ApplyResult::Completed); REQUIRE(saved.opened); CHECK(*saved.opened);
    const auto bytes = readBytes(disk.path("LOCAL_REMESH.DAT"));
    CHECK(bytes.find("FURTHER_IMPROVE_CONTACT_ELEMENTS 48\r\n") != std::string::npos);
    CHECK(bytes.find("GOOD_SURFACE_ELEMENT_CHANGEABLE 53\r\n") != std::string::npos);
    CHECK_EQ(owner.improveContact, 48); CHECK_EQ(owner.goodSurfaceChangeable, 53);
}

TEST_CASE(remesh_native_disabled_remove_always_attempts_and_ignores_missing_or_denied)
{
    DiskScratch disk; RemeshState owner;
    auto report = saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::Unrecovered);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.removalSucceeded); CHECK(!*report.removalSucceeded);
    disk.write("LOCAL_REMESH.DAT", "KEEP");
    HANDLE held = CreateFileW(disk.path("LOCAL_REMESH.DAT").c_str(), GENERIC_READ,
        FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    REQUIRE(held != INVALID_HANDLE_VALUE);
    report = saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::KnownC);
    CloseHandle(held);
    CHECK(report.outcome == ApplyResult::Completed); REQUIRE(report.removalSucceeded); CHECK(!*report.removalSucceeded);
    CHECK_EQ(readBytes(disk.path("LOCAL_REMESH.DAT")), "KEEP"); CHECK(!report.opened);
    report = saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::KnownC);
    REQUIRE(report.removalSucceeded); CHECK(*report.removalSucceeded);
    CHECK(!std::filesystem::exists(disk.path("LOCAL_REMESH.DAT")));
}

TEST_CASE(remesh_native_failed_write_open_preserves_file_and_marks_invalid_parameter_boundary)
{
    DiskScratch disk; disk.write("LOCAL_REMESH.DAT", "KEEP"); RemeshState owner; owner.enabled = true;
    HANDLE held = CreateFileW(disk.path("LOCAL_REMESH.DAT").c_str(), GENERIC_READ,
        FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    REQUIRE(held != INVALID_HANDLE_VALUE);
    const auto report = saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::KnownC);
    CloseHandle(held);
    CHECK(report.outcome == ApplyResult::UnsupportedSnapshot); REQUIRE(report.opened); CHECK(!*report.opened);
    CHECK(report.invalidParameterReached); CHECK(!report.removalSucceeded);
    CHECK_EQ(readBytes(disk.path("LOCAL_REMESH.DAT")), "KEEP");
    const auto unknown = saveRemeshAtProjectSave(owner, disk.dir.string(), NumericLocale::Unrecovered);
    CHECK(unknown.outcome == ApplyResult::UnsupportedSnapshot); CHECK(!unknown.opened);
    CHECK(!unknown.invalidParameterReached); CHECK_EQ(readBytes(disk.path("LOCAL_REMESH.DAT")), "KEEP");
}

int main() { return testkit::run_all(); }
