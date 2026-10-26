// DEFORM .DB reader: container, step directory and raw step payload.
// The layout follows docs/db-format-spec.md. Nothing here depends on DEFORM
// being installed.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "deformdb/file.hpp"

namespace deformdb {

// Database family, from the 20-character signature in record 1.
enum class Family : int {
    Unknown = 0,
    Legacy2D = 2,    // SFTC-DEFORM-2D-DB450 / DB720
    Legacy3D = 3,    // SFTC-DEFORM-3D-DB230
    Combined = 23,   // SFTC-DEFORM-23-DB100, SFTC-DEF-2D-DB100-00, SFTC-DEF-3D-DB100-00
};

struct FileHeader {
    std::string signature;                 // record 1, 20 chars
    Family family = Family::Unknown;
    std::string title;                     // record 2, 80 chars
    std::string version_text;              // e.g. "11.00"
    int version = 0;                       // e.g. 11000
    std::array<std::string, 3> dates;      // three 8-char date fields after the version
    std::array<std::int32_t, 6> words{};   // record 2 int words 29..34
    std::int32_t entry_count = 0;          // directory entries in use (word 31)
};

// One step-directory entry (13 int32 words).
struct StepEntry {
    std::int32_t slot = 0;        // 1-based position in the directory
    std::int32_t mesh_no = 0;     // w1
    std::int32_t step = 0;        // w2, negative = input/restart step
    std::int32_t header_size = 0; // w3, real*8 words in the step header
    std::int32_t mar_size = 0;    // w4, MAR int words (or region-table words, see spec)
    std::int32_t a_size = 0;      // w5, A real words (or region-table words, see spec)
    std::int32_t sim_no = 0;      // w6
    std::int32_t sim_mode = 0;    // w7
    std::int32_t dim = 0;         // w8, 2 or 3
    std::int32_t version = 0;     // w9, e.g. 11000
    std::int32_t rec_dynamic = 0; // w10, record of the dynamic-array descriptor (0 = none)
    std::int32_t rec_header = 0;  // w11
    std::int32_t rec_mar = 0;     // w12
    std::int32_t rec_a = 0;       // w13
};

// Placement of one MAR or A region (3D steps, version >= 6000).
struct Region {
    int index = 0;               // 1-based region number
    std::string name;            // DEFORM address name (IINOD, LLCRZ, ...), may be empty
    std::int32_t start = 0;      // 1-based start address in the full array
    std::int32_t length = 0;     // decoded length in words
    std::int32_t stored = 0;     // stored words (0 = region absent)
    std::int32_t record = 0;     // first record of the stored data
};

struct DynamicIntArray {
    int slot = 0;
    std::array<std::int32_t, 10> descriptor{};
    std::vector<std::int32_t> values;
};

struct DynamicRealArray {
    int slot = 0;
    std::array<std::int32_t, 10> descriptor{};
    std::vector<double> values;
};

struct StepData {
    StepEntry entry;
    bool compressed = false;             // RLE applied to MAR/A
    std::vector<double> header;          // header_size words
    std::vector<std::int32_t> mar;       // full integer work array
    std::vector<double> a;               // full real work array
    std::vector<Region> mar_regions;     // filled when the step stores a region table
    std::vector<Region> a_regions;
    std::vector<std::int32_t> mar_table; // raw region tables as stored (MARSize / ASize words)
    std::vector<std::int32_t> a_table;
    std::array<std::int32_t, 20> dynamic_descriptor{};
    std::vector<std::array<std::int32_t, 10>> dynamic_int_rows;   // raw table rows 1..NI
    std::vector<std::array<std::int32_t, 10>> dynamic_real_rows;  // raw table rows 1..NR
    std::vector<DynamicIntArray> dynamic_ints;
    std::vector<DynamicRealArray> dynamic_reals;

    // True when the directory entry carries MAR and A data (not a header-only step).
    bool has_arrays() const noexcept { return entry.mar_size > 0 && entry.a_size > 0; }

    // Region lookup by DEFORM address name; empty span if absent.
    std::span<const std::int32_t> mar_region(std::string_view name) const;
    std::span<const double> a_region(std::string_view name) const;
};

struct ReadOptions {
    bool header = true;
    bool arrays = true;      // MAR + A
    bool dynamic = true;     // dynamic arrays (version >= 11000)
};

class Database {
public:
    static std::unique_ptr<Database> open(const std::filesystem::path& path);

    const FileHeader& header() const noexcept { return header_; }
    const std::vector<StepEntry>& entries() const noexcept { return entries_; }
    std::uint64_t file_size() const noexcept { return file_->size(); }

    // Entry for a DEFORM step number (e.g. 25 or -25); first match, as DEFORM does.
    std::optional<StepEntry> find_step(std::int32_t step) const;

    // Thread-safe: reads only through positional I/O.
    StepData read_step(const StepEntry& entry, const ReadOptions& opt = {}) const;

    // Reads steps on `threads` workers (0 = hardware concurrency) and hands each
    // finished step to `sink`, which may be called concurrently.
    void read_steps(std::span<const StepEntry> entries, const ReadOptions& opt, unsigned threads,
                    const std::function<void(StepData&&)>& sink) const;

    // Re-reads record 2 and the directory, picking up steps appended since open().
    void refresh();

private:
    explicit Database(std::unique_ptr<RandomAccessFile> f);
    void read_file_header();
    void read_directory();
    std::vector<std::int32_t> read_i32(std::int32_t record, std::size_t count) const;
    std::vector<double> read_f64(std::int32_t record, std::size_t count) const;
    void read_region_array_i32(StepData& s) const;
    void read_region_array_f64(StepData& s) const;
    void read_dynamic(StepData& s) const;

    std::unique_ptr<RandomAccessFile> file_;
    FileHeader header_;
    std::vector<StepEntry> entries_;
};

// Version number DEFORM assigns to a version string for a given family.
int version_from_text(Family family, std::string_view text);

// Region name tables (index order) for a step of the given dimension/version.
std::span<const char* const> mar_region_names(int dim, int version);
std::span<const char* const> a_region_names(int dim, int version);

}  // namespace deformdb
