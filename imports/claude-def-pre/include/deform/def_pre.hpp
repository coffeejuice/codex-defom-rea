#pragma once

// Driving DEFORM's own text pre-processor (DEF_PRE_64.exe):
//  * one-shot batch queries: DEF_PRE -m header|stepList|lastStepNo|simStepList -d a.db -o out.txt
//  * a persistent interactive session that runs keyword programs
//    "DBREAD <step> / <db> / KFWRIT / <file>" to export steps as KEY text.
//
// Windows only. Each session runs in its own working directory because
// DEF_PRE writes scratch files (NEWEQ.MSG ...) into its current directory.

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace dk {

struct DefPreOptions {
    std::filesystem::path executable;   // empty: discover_def_pre()
    std::filesystem::path work_root;    // empty: %TEMP%\deform_key
    std::chrono::milliseconds start_timeout{30000};
    std::chrono::milliseconds query_timeout{120000};
    std::chrono::milliseconds step_timeout{900000};
    bool keep_key_files = false;        // keep exported KEY files on disk
};

class DefPreError : public std::runtime_error {
public:
    DefPreError(std::string message, std::string log = {})
        : std::runtime_error(std::move(message)), log_(std::move(log))
    {
    }
    [[nodiscard]] const std::string &log() const noexcept { return log_; }

private:
    std::string log_;
};

// Default: DEF_PRE_64.exe next to DEF_PRE.EXE in the newest
// C:\Program Files\SFTC\DEFORM\v*\3D folder, or DEFORM_DEF_PRE env var.
[[nodiscard]] std::filesystem::path discover_def_pre();

struct DatabaseHeader {
    int version = 0;
    std::string release_date;
    std::string initial_date;
    std::string current_date;
    int simulation_type = 0;
    int record_count = 0;
    int stored_step_count = 0;
    std::string raw;
};

struct SimulationGroup {
    int index = 0;           // 1-based group index
    int metadata = 0;        // second number on the group line
    std::string label;       // "Operation N" as printed
    std::vector<int> boundary_values; // numbers printed after the label (raw)
};

[[nodiscard]] DatabaseHeader parse_header(std::string_view text);
[[nodiscard]] std::vector<int> parse_step_list(std::string_view text);
[[nodiscard]] std::vector<SimulationGroup> parse_sim_step_list(std::string_view text);

// Runs "DEF_PRE -m <mode> -d <db> -o <tmp>" and returns the output file text.
[[nodiscard]] std::string run_query(const DefPreOptions &options, std::string_view mode,
                                    const std::filesystem::path &database);

struct ExportResult {
    int requested = 0;                  // 0 = last step
    std::optional<int> resolved;        // step DEF_PRE reported reading
    bool substituted = false;           // DEF_PRE read another step ("<- n")
    std::filesystem::path key_file;
    std::string log;                    // DEF_PRE console output for this command
};

// A persistent DEF_PRE process. Not thread-safe; use one per thread.
class DefPreSession {
public:
    explicit DefPreSession(DefPreOptions options = {});
    ~DefPreSession();
    DefPreSession(const DefPreSession &) = delete;
    DefPreSession &operator=(const DefPreSession &) = delete;

    // Exports one step to `key_file` (relative paths are inside the session
    // directory). Blocking.
    ExportResult export_step(const std::filesystem::path &database, int step,
                             const std::filesystem::path &key_file);

    // Pipelined form: begin() queues the command, finish() waits for it.
    // At most one command may be outstanding.
    void begin_export(const std::filesystem::path &database, int step,
                      const std::filesystem::path &key_file);
    ExportResult finish_export();

    [[nodiscard]] const std::filesystem::path &directory() const noexcept;
    [[nodiscard]] std::uint32_t process_id() const noexcept;
    [[nodiscard]] bool alive() const noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

// Path form DEF_PRE (Fortran, ANSI code page) can read: the 8.3 short path
// when the path is long or not plain ASCII.
[[nodiscard]] std::string fortran_path(const std::filesystem::path &p);

} // namespace dk
