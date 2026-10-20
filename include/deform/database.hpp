#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>

namespace deform {

// Original numeric status codes are retained when a recovered branch supplies
// one. Checked C++ boundary failures use code 0 and are not original statuses.
class database_error : public std::runtime_error {
public:
    database_error(std::string message, int original_code = 0)
        : std::runtime_error(std::move(message)), original_code_(original_code) {}
    [[nodiscard]] int original_code() const noexcept { return original_code_; }
private:
    int original_code_;
};

struct database_header {
    std::string signature;
    std::string title;             // Exactly 80 bytes; trailing spaces retained.
    std::string version_text;      // Exactly 8 bytes.
    std::string release_date;
    std::string initial_date;
    std::string current_date;
    std::array<std::int32_t, 6> words{};
    std::int32_t dimension{};      // 2, 3, or 23 (unified database).
    std::int32_t version{};
    [[nodiscard]] std::int32_t simulation_type() const { return words[1]; }
    [[nodiscard]] std::int32_t record_count() const { return words[2]; }
};

struct step_record {
    // All thirteen original table words, including the auxiliary pointer, survive.
    std::array<std::int32_t, 13> words{};
    // Legacy files obtain these from the database header, without altering words.
    std::int32_t effective_dimension{};
    std::int32_t effective_version{};
    [[nodiscard]] std::int32_t number() const { return words[1]; }
    [[nodiscard]] std::int32_t dimension() const { return effective_dimension; }
    [[nodiscard]] std::int32_t version() const { return effective_version; }
    [[nodiscard]] bool has_arrays() const { return words[3] > 0 && words[4] > 0; }
};

struct array_block {
    std::int32_t logical_capacity{};
    std::int32_t destination{};    // Original one-based array index.
    std::int32_t stored_count{};
    std::int32_t record{};         // Original one-based 512-byte record number.
};

template<class T>
struct auxiliary_array {
    std::array<std::int32_t, 10> words{}; // Original on-disk descriptor.
    std::vector<T> values;
    // The reference reports a mismatch but does not assign a new error code.
    [[nodiscard]] bool size_matches() const noexcept {
        return words[0] >= 0 && values.size() == static_cast<std::size_t>(words[0]);
    }
};

struct auxiliary_data {
    bool present{};
    std::array<std::int32_t, 20> words{};
    std::vector<std::int32_t> integer_directory;
    std::vector<std::int32_t> real_directory;
    std::vector<auxiliary_array<std::int32_t>> integers;
    std::vector<auxiliary_array<double>> reals;
};

struct loaded_step {
    step_record record;
    // Real header slots may contain character bytes. Preserve their exact
    // 64-bit values; interpreting every header slot as a number is incorrect.
    std::vector<double> header;
    std::vector<std::int32_t> integers;
    std::vector<double> reals;
    std::vector<array_block> integer_blocks;
    std::vector<array_block> real_blocks;
    std::vector<std::int32_t> integer_directory; // Includes opaque header/trailing words.
    std::vector<std::int32_t> real_directory;
};

class database {
public:
    explicit database(std::filesystem::path path);
    [[nodiscard]] const database_header& header() const noexcept { return header_; }
    [[nodiscard]] const std::vector<step_record>& records() const noexcept { return records_; }
    [[nodiscard]] std::uint64_t file_size() const noexcept { return size_; }
    [[nodiscard]] std::vector<std::size_t> stored_steps() const;
    [[nodiscard]] std::size_t find_step(std::int32_t number) const;
    [[nodiscard]] std::vector<double> load_header(std::size_t record_index) const;
    [[nodiscard]] loaded_step load_step(std::size_t record_index) const;
    // Original mode 5 is separate from the mode 2/3 main-array load.
    [[nodiscard]] auxiliary_data load_auxiliary(std::size_t record_index) const;
private:
    std::filesystem::path path_;
    std::uint64_t size_{};
    database_header header_;
    std::vector<step_record> records_;
    [[nodiscard]] std::vector<std::uint8_t> read(std::uint64_t offset, std::size_t count,
                                               int original_code) const;
    [[nodiscard]] std::vector<std::int32_t> integers(std::int32_t record,
                                                    std::int32_t count, int code) const;
    [[nodiscard]] std::vector<double> reals(std::int32_t record,
                                           std::int32_t count, int code) const;
    [[nodiscard]] static std::vector<array_block> directory(
        const std::vector<std::int32_t>& words);
};

} // namespace deform
