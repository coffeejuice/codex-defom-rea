// Purge and merge of DEFORM databases, following DEF_GUI23's
// "Tool > DB Manager > Step List" (see docs/task-dbmanager-purge-merge.md).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <span>
#include <string>

namespace deformdb {

struct ManagerOptions {
    bool overwrite = false;   // replace an existing destination file
    std::string today;        // date for the new header ("MM/DD/YY"); empty = current date
    // Called after every written entry; return false to stop. A stopped run
    // leaves a valid database holding the entries written so far.
    std::function<bool(std::size_t done, std::size_t total)> progress;
};

struct ManagerResult {
    std::size_t entries = 0;      // directory entries written
    std::size_t header_only = 0;  // of which stored without MAR/A data
    bool stopped = false;         // progress callback asked to stop
};

// Creates `dst` from `src`, keeping the steps listed in `keep_steps` (DEFORM step
// numbers, e.g. 25 or -25). As in DEF_GUI23: every entry whose |step| lies between
// |first kept step| and |last kept step| (directory order) is copied, kept steps
// with their arrays and all others as header-only entries; entries outside that
// range are dropped. Throws if no listed step exists in `src`.
ManagerResult purge(const std::filesystem::path& src, const std::filesystem::path& dst,
                    std::span<const std::int32_t> keep_steps, const ManagerOptions& opt = {});

// Joins two databases into `dst`. One of them must continue the other: its first
// step is -(last step of the other + 1), i.e. the restart step DEFORM writes when
// a run is continued. Every entry of the earlier database, then every entry of the
// later one is copied; header-only entries stay header-only. Throws otherwise.
ManagerResult merge(const std::filesystem::path& db1, const std::filesystem::path& db2,
                    const std::filesystem::path& dst, const ManagerOptions& opt = {});

}  // namespace deformdb
