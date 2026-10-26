// DEFORM .DB writer: the C++ counterpart of the "write step" operation (op code 4)
// of DEFORM's datarm23 module, as DEF_GUI23's DB Manager uses it. The layout is
// described in docs/db-format-spec.md and docs/task-dbmanager-purge-merge.md.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "deformdb/database.hpp"
#include "deformdb/file.hpp"

namespace deformdb {

// Record 2 fields that do not come from the steps themselves.
struct WriterHeader {
    std::string title;         // TITLE, up to 80 chars
    std::string release_date;  // SMI2, 8 chars
    std::int32_t ismtyp = 1;   // ISMTYP for 2D steps; 3D steps always write 1
    std::string today;         // SMI3/SMI4 date "MM/DD/YY"; empty = current local date
};

// Header fields taken over from an existing database (title, release date, ISMTYP).
WriterHeader writer_header_from(const FileHeader& source);

// Writes a new combined-family (2D/3D) database step by step. Every append
// leaves a complete, readable file behind, as DEFORM's writer does.
class Writer {
public:
    // Creates `path`, replacing any existing file.
    static std::unique_ptr<Writer> create(const std::filesystem::path& path, WriterHeader header);
    ~Writer();

    // Appends `step` as directory entry entry_count() + 1. With `header_only`
    // (or when the step has no arrays) only the step header is stored and the
    // entry gets MARSize = ASize = 0. `step` must carry the header and, unless
    // header-only, the full MAR/A arrays plus region tables for 3D steps of
    // version >= 6000, as Database::read_step returns them.
    StepEntry append_step(const StepData& step, bool header_only = false);

    std::int32_t entry_count() const noexcept { return static_cast<std::int32_t>(dir_.size()); }
    std::int32_t next_record() const noexcept { return nxtrec_; }
    void flush();

private:
    Writer(std::unique_ptr<WritableFile> f, WriterHeader h);

    std::int32_t write_i32(std::int32_t record, const std::vector<std::int32_t>& words);
    std::int32_t write_f64(std::int32_t record, const std::vector<double>& words);
    void write_region_array_i32(const StepData& s, std::array<std::int32_t, 13>& e);
    void write_region_array_f64(const StepData& s, std::array<std::int32_t, 13>& e);
    void write_dynamic(const StepData& s, std::array<std::int32_t, 13>& e);
    void write_signature(int dim);
    void write_file_header(const StepEntry& last);
    void write_index_records();
    void write_directory_chunk(std::int32_t slot);
    void allocate_directory_chunk(std::int32_t slot);

    std::unique_ptr<WritableFile> file_;
    WriterHeader hdr_;
    std::string created_;                                // SMI3
    std::vector<std::array<std::int32_t, 13>> dir_;      // directory entries, slot = index + 1
    std::vector<std::array<std::int32_t, 128>> index_;   // index records (first one is record 3)
    std::int32_t nxtrec_ = 17;                           // NXTREC, next free record
};

}  // namespace deformdb
