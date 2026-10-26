// Output comparator for the DEF_PRE_64 port (port plan, Phase 0.2).
//
// Compares what the original program wrote (the "expected" side, usually from the
// oracle cache) with what the port wrote ("actual"):
//   * KEY files byte for byte; a mismatch names the first differing line and the
//     keyword it belongs to;
//   * .DB files record by record with a mask list of documented volatile words
//     (dates, ISTART, header-only fields); a mismatch names the first differing
//     step, region and keyword, decoded through deformdb;
//   * stdout transcripts after masking dates, times and paths.
// Every comparison digests both sides first (XXH64 over the masked or normalised
// content) and only diffs on a digest mismatch. Expected digests can be stored in
// the oracle cache, so a passing comparison need not read the expected file.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <regex>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace defcmp {

namespace fs = std::filesystem;

// ------------------------------------------------------------------ digest

using Digest = std::uint64_t;

// Streaming XXH64 (seed 0).
class Hasher {
public:
    Hasher();
    void update(const void* data, std::size_t bytes);
    Digest finish() const;

private:
    std::uint64_t v_[4];
    std::uint64_t total_ = 0;
    unsigned char buf_[32];
    std::size_t buffered_ = 0;
};

Digest hash_bytes(std::string_view data);
std::string digest_hex(Digest d);                   // 16 lowercase hex digits
std::optional<Digest> parse_digest(std::string_view hex);

// ------------------------------------------------------------------ result

enum class Kind { Key, Db, Transcript, Bytes };
const char* kind_name(Kind k);
std::optional<Kind> parse_kind(std::string_view s);
// .KEY -> Key, .DB -> Db, stdout.txt / stderr.txt -> Transcript, anything else -> Bytes.
// Other .txt outputs (-m header, stepList, ...) are program output, compared byte for byte.
Kind kind_from_path(const fs::path& p);

struct Result {
    bool equal = false;
    bool by_digest = false;   // decided by digests alone, no diff was needed
    Digest expected_digest = 0;
    Digest actual_digest = 0;
    std::string where;        // e.g. "line 812, keyword RZ 2" or "step 25 (slot 14), A region LLCRZ"
    std::string expected;     // differing text / value on each side
    std::string actual;
    std::string error;        // set when a file could not be read

    std::string summary() const;  // one or a few lines for humans
};

// ------------------------------------------------------------------ KEY

// Keyword at the start of a KEY line (registry names, docs/re/keyword_registry.tsv).
bool is_registry_keyword(std::string_view token);

Digest key_digest(const fs::path& file);
Result compare_key_text(std::string_view expected, std::string_view actual);
Result compare_key(const fs::path& expected, const fs::path& actual,
                   std::optional<Digest> expected_digest = {});

// ------------------------------------------------------------------ transcript

struct TranscriptMasks {
    bool dates = true;   // 10/21/26, 2026-10-21, 21-Oct-2026, Oct 21 2026, Wed Oct 21 ...
    bool times = true;   // 09:31, 09:31:50, 09:31:50.123, with optional AM/PM
    bool paths = true;   // remaining absolute Windows/UNC paths -> <PATH>
    // Directory prefixes replaced by a name before path masking, so the rest of
    // the path is still compared: {"C:\\scratch\\case1", "<CASE>"}. Case-insensitive,
    // either slash.
    std::vector<std::pair<std::string, std::string>> roots;
    std::vector<std::regex> extra;   // further patterns, each match -> <MASK>
};

std::string normalize_transcript(std::string_view text, const TranscriptMasks& m);
Digest transcript_digest(const fs::path& file, const TranscriptMasks& m);
Result compare_transcript_text(std::string_view expected, std::string_view actual,
                               const TranscriptMasks& m);
Result compare_transcript(const fs::path& expected, const fs::path& actual, const TranscriptMasks& m,
                          std::optional<Digest> expected_digest = {});

// ------------------------------------------------------------------ DB

// Volatile words of a .DB file (docs/db-format-spec.md §3, §4.2,
// docs/task-dbmanager-purge-merge.md §6):
//   smi2      record 2 bytes 88-95, release date of the writing program
//   smi3      record 2 bytes 96-103, date the simulation was first written
//   smi4      record 2 bytes 104-111, date of the last write
//   istart    record 2 word 29, directory entry last written by the pre-processor
//   ho-dir    directory words 10 and 13 of header-only entries (stale record numbers)
//   ho-h7h8   step-header reals H7/H8 of header-only entries (stale array sizes)
// Groups: dates = smi3,smi4; default = dates,istart; header-only = ho-dir,ho-h7h8;
// all = every name above; none.
struct ByteRange {
    std::uint64_t offset = 0;
    std::uint64_t length = 0;
};

struct DbMasks {
    bool smi2 = false, smi3 = true, smi4 = true, istart = true;
    bool ho_dir = false, ho_h7h8 = false;
    std::vector<ByteRange> raw;   // "bytes:OFFSET:LEN", file offsets

    static DbMasks none();
    // Spec: comma-separated items applied left to right to the defaults. An item is
    // a name or group (replaces the set when it is the first item and has no sign),
    // "+name" (add), "-name" (remove) or "bytes:OFFSET:LEN". Examples:
    // "default", "+header-only", "none,+smi4", "all,-smi2", "+bytes:1024:8".
    static std::optional<DbMasks> parse(std::string_view spec, std::string* error = nullptr);
    std::string canonical() const;   // e.g. "smi3,smi4,istart"
};

// Byte ranges masked in `file` (computed from its own directory).
std::vector<ByteRange> db_mask_ranges(const fs::path& file, const DbMasks& m);
Digest db_digest(const fs::path& file, const DbMasks& m);
Result compare_db(const fs::path& expected, const fs::path& actual, const DbMasks& m,
                  std::optional<Digest> expected_digest = {});

// ------------------------------------------------------------------ bytes / dispatch

Digest bytes_digest(const fs::path& file);
Result compare_bytes(const fs::path& expected, const fs::path& actual,
                     std::optional<Digest> expected_digest = {});

struct Options {
    DbMasks db;
    TranscriptMasks transcript;
};

Digest digest_file(Kind k, const fs::path& file, const Options& o);
Result compare_files(Kind k, const fs::path& expected, const fs::path& actual, const Options& o,
                     std::optional<Digest> expected_digest = {});

// Compares two output directories (oracle run vs. port run): file sets by
// case-insensitive relative path, then each file by its kind (kind_from_path).
struct DirEntryResult {
    std::string name;
    Kind kind = Kind::Bytes;
    bool missing_expected = false;   // only in actual
    bool missing_actual = false;     // only in expected
    bool skipped = false;            // cannot be compared (e.g. oracle kept only a sha256)
    Result result;

    bool ok() const { return !skipped && !missing_actual && !missing_expected && result.error.empty() && result.equal; }
};
std::vector<DirEntryResult> compare_dirs(const fs::path& expected, const fs::path& actual, const Options& o);

// Compares an oracle cache entry (tools/oracle/def_pre_oracle.py: result.json,
// stdout.txt, stderr.txt, files/) with a port run laid out the same way. The
// actual side needs files/ (or holds the files at its top level) and may carry
// result.json or a one-line exit_code file. Rows: "exit code", stdout.txt,
// stderr.txt, then every output file. Files the oracle deleted must be absent;
// files it recorded by sha256 only (over 2 GiB) are reported as skipped.
std::vector<DirEntryResult> compare_oracle_entry(const fs::path& entry, const fs::path& actual, const Options& o);

}  // namespace defcmp
