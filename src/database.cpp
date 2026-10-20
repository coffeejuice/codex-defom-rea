#include "deform/database.hpp"
#include "deform/packed_array.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <fstream>
#include <limits>
#include <set>
#include <span>
#include <string_view>

namespace deform {
namespace {
constexpr std::uint64_t record_bytes = 512;

std::uint32_t be32(const std::uint8_t* p) {
    return (std::uint32_t{p[0]} << 24) | (std::uint32_t{p[1]} << 16) |
           (std::uint32_t{p[2]} << 8) | p[3];
}
std::uint64_t be64(const std::uint8_t* p) {
    return (std::uint64_t{be32(p)} << 32) | be32(p + 4);
}
std::uint64_t position(std::int32_t record, int code) {
    if (record < 1) throw database_error("Invalid physical record number", code);
    return (static_cast<std::uint64_t>(record) - 1) * record_bytes;
}
std::size_t count_bytes(std::int32_t count, std::size_t width) {
    if (count < 0) throw database_error("Negative array length");
    return static_cast<std::size_t>(count) * width;
}
std::string text(std::span<const std::uint8_t> bytes, std::size_t start, std::size_t count) {
    return {reinterpret_cast<const char*>(bytes.data() + start), count};
}
std::int32_t version_number(std::string_view version, std::int32_t dimension) {
    // Literal 8-byte comparisons and fallback values from 0x1403b32b5.
    if (dimension == 23) {
        for (int v = 10; v <= 20; ++v) {
            auto expected = std::to_string(v) + ".00";
            expected.resize(8, ' ');
            if (version == expected) return v * 1000;
        }
        return 10000;
    }
    const std::pair<std::string_view, int> two[] = {
        {"5.0",5000},{"5.0 CNV",5000},{"5.1",5100},{"5.1 CNV",5100},
        {"5.2",5200},{"5.3",5300},{"6.0",6000},{"7.0",7000},{"7.10",7100},
        {"7.20",7200},{"8.00",8000},{"8.10",8100},{"8.20",8200},{"9.00",9000},{"9.01",9010}};
    const std::pair<std::string_view, int> three[] = {
        {"2.3",2300},{"2.4",3000},{"3.0",3000},{"3.1.1",3100},{"3.2",3200},
        {"3.3",3300},{"4.0",4000},{"5.0",5000},{"5.1",5100},{"6.0",6000},{"6.01",6010}};
    if (dimension == 2) {
        for (const auto& [name, value] : two) {
            auto expected = " " + std::string(name);
            expected.resize(8, ' ');
            if (version == expected) return value;
        }
        return 9100;
    }
    for (const auto& [name, value] : three) {
        auto expected = " " + std::string(name);
        expected.resize(8, ' ');
        if (version == expected) return value;
    }
    return 6100;
}
std::int32_t nint(double value) {
    if (!std::isfinite(value) || value < 0 ||
        value > static_cast<double>(std::numeric_limits<std::int32_t>::max()) - 0.5)
        throw database_error("Invalid decoded array size in step header");
    return static_cast<std::int32_t>(std::floor(value + 0.5));
}
template<class T>
void place(std::vector<T>& destination, const std::vector<T>& source, const array_block& block) {
    if (block.destination < 1) throw database_error("Invalid array destination");
    const auto offset = static_cast<std::size_t>(block.destination - 1);
    if (offset > destination.size() || source.size() > destination.size() - offset)
        throw database_error("Decoded block exceeds the step array");
    // Avoid floating point transformations of payloads containing text/NaNs.
    if (!source.empty())
        std::memcpy(destination.data() + offset, source.data(), source.size() * sizeof(T));
}
}

std::vector<std::uint8_t> database::read(std::uint64_t offset, std::size_t count, int code) const {
    if (offset > size_ || count > size_ - offset)
        throw database_error("Read exceeds database file", code);
    std::ifstream stream(path_, std::ios::binary);
    if (!stream) throw database_error("Cannot open database", 99);
    stream.seekg(static_cast<std::streamoff>(offset));
    std::vector<std::uint8_t> bytes(count);
    if (count && !stream.read(reinterpret_cast<char*>(bytes.data()),
                              static_cast<std::streamsize>(count)))
        throw database_error("Database read failed", code);
    return bytes;
}
std::vector<std::int32_t> database::integers(std::int32_t record, std::int32_t count, int code) const {
    if (count == 0) return {};
    const auto bytes = read(position(record, code), count_bytes(count, 4), code);
    std::vector<std::int32_t> result(static_cast<std::size_t>(count));
    for (std::size_t i = 0; i < result.size(); ++i)
        result[i] = std::bit_cast<std::int32_t>(be32(bytes.data() + i * 4));
    return result;
}
std::vector<double> database::reals(std::int32_t record, std::int32_t count, int code) const {
    if (count == 0) return {};
    const auto bytes = read(position(record, code), count_bytes(count, 8), code);
    std::vector<double> result(static_cast<std::size_t>(count));
    for (std::size_t i = 0; i < result.size(); ++i)
        result[i] = std::bit_cast<double>(be64(bytes.data() + i * 8));
    return result;
}

database::database(std::filesystem::path path) : path_(std::move(path)) {
    size_ = std::filesystem::file_size(path_);
    const auto first = read(0, 20, 101);
    header_.signature = text(first, 0, 20);
    const auto signature = std::string_view(header_.signature);
    if (signature == "SFTC-DEFORM-2D-DB450" ||
        signature == "SFTC-DEFORM-2D-DB720") header_.dimension = 2;
    else if (signature == "SFTC-DEFORM-3D-DB230") header_.dimension = 3;
    else if (signature == "SFTC-DEFORM-23-DB100" ||
             signature == "SFTC-DEF-2D-DB100-00" ||
             signature == "SFTC-DEF-3D-DB100-00") header_.dimension = 23;
    else throw database_error("Not a valid DEFORM database!", 101);
    const auto bytes = read(512, 136, 101);
    header_.title = text(bytes, 0, 80);
    header_.version_text = text(bytes, 80, 8);
    header_.release_date = text(bytes, 88, 8);
    header_.initial_date = text(bytes, 96, 8);
    header_.current_date = text(bytes, 104, 8);
    for (std::size_t i = 0; i < 6; ++i)
        header_.words[i] = std::bit_cast<std::int32_t>(be32(bytes.data() + 112 + i * 4));
    header_.version = version_number(header_.version_text, header_.dimension);
    const auto count = header_.record_count();
    // The original stores at most 200 roots * 127 pages * 128 records.
    if (count < 0 || count > 200 * 127 * 128)
        throw database_error("Database step count exceeds recovered table capacity");
    records_.reserve(static_cast<std::size_t>(count));
    std::int32_t root_record = 3;
    std::set<std::int32_t> visited;
    while (records_.size() < static_cast<std::size_t>(count)) {
        if (!visited.insert(root_record).second)
            throw database_error("Cyclic step index");
        const auto root = integers(root_record, 128, 101);
        for (std::size_t page = 0; page < 127 && records_.size() < static_cast<std::size_t>(count); ++page) {
            const auto words = integers(root[page], 128 * 13, 101);
            for (std::size_t row = 0; row < 128 && records_.size() < static_cast<std::size_t>(count); ++row) {
                step_record step;
                std::copy_n(words.begin() + row * 13, 13, step.words.begin());
                step.effective_dimension = header_.dimension == 23 ? step.words[7] : header_.dimension;
                step.effective_version = header_.dimension == 23 ? step.words[8] : header_.version;
                records_.push_back(step);
            }
        }
        root_record = root[127];
    }
}

std::vector<std::size_t> database::stored_steps() const {
    std::vector<std::size_t> result;
    for (std::size_t i = 0; i < records_.size(); ++i)
        if (records_[i].words[3] > 0) result.push_back(i);
    return result;
}
std::size_t database::find_step(std::int32_t number) const {
    const auto it = std::find_if(records_.begin(), records_.end(),
                                 [number](const auto& row) { return row.number() == number; });
    if (it == records_.end()) throw database_error("Step number not found", 201);
    return static_cast<std::size_t>(it - records_.begin());
}
std::vector<double> database::load_header(std::size_t index) const {
    const auto& step = records_.at(index);
    return reals(step.words[10], step.words[2], 102);
}
std::vector<array_block> database::directory(const std::vector<std::int32_t>& words) {
    if (words.size() < 2 || words[0] < 0 ||
        static_cast<std::size_t>(words[0]) > (words.size() - 2) / 4)
        throw database_error("Invalid array block directory", 102);
    std::vector<array_block> result;
    result.reserve(static_cast<std::size_t>(words[0]));
    for (std::size_t i = 0; i < static_cast<std::size_t>(words[0]); ++i) {
        const auto j = 2 + 4 * i;
        result.push_back({words[j], words[j + 1], words[j + 2], words[j + 3]});
    }
    return result;
}
loaded_step database::load_step(std::size_t index) const {
    loaded_step result;
    result.record = records_.at(index);
    const auto& row = result.record;
    if (!row.has_arrays()) throw database_error("Step has no stored array data", 202);
    if (!row.dimension() || !row.version())
        throw database_error("Missing step dimension or version", 505);
    if (row.dimension() != 2 && row.dimension() != 3)
        throw database_error("Internal error on loading a step data", 707);
    result.header = load_header(index);
    const bool packed = row.dimension() == 2 ? row.version() > 3109 : row.version() > 999;
    if (row.dimension() == 2 || row.version() < 6000) {
        auto ints = integers(row.words[11], row.words[3], 103);
        auto values = reals(row.words[12], row.words[4], 104);
        result.integers = packed ? unpack_integers(ints) : std::move(ints);
        result.reals = packed ? unpack_reals(values) : std::move(values);
        return result;
    }
    if (result.header.size() < 8) throw database_error("Step header is too short", 102);
    result.integer_directory = integers(row.words[11], row.words[3], 102);
    result.real_directory = integers(row.words[12], row.words[4], 102);
    result.integer_blocks = directory(result.integer_directory);
    result.real_blocks = directory(result.real_directory);
    result.integers.resize(static_cast<std::size_t>(nint(result.header[6])));
    result.reals.resize(static_cast<std::size_t>(nint(result.header[7])));
    for (const auto& block : result.integer_blocks) {
        if (!block.stored_count) continue;
        auto data = integers(block.record, block.stored_count, 103);
        if (packed) data = unpack_integers(data);
        place(result.integers, data, block);
    }
    for (const auto& block : result.real_blocks) {
        if (!block.stored_count) continue;
        auto data = reals(block.record, block.stored_count, 104);
        if (packed) data = unpack_reals(data);
        place(result.reals, data, block);
    }
    return result;
}

auxiliary_data database::load_auxiliary(std::size_t index) const {
    auxiliary_data result;
    const auto& row = records_.at(index);
    // The reference uses the active step version established by mode 2/3.
    // This independent API selects that step explicitly.
    if (row.version() < 11000 || row.words[9] == 0) return result;
    const auto metadata = integers(row.words[9], 20, 1);
    result.present = true;
    std::copy(metadata.begin(), metadata.end(), result.words.begin());
    const auto read_directory = [&](std::size_t group) {
        const auto count = result.words[group];
        if (count < 0) throw database_error("Negative auxiliary array count");
        if (!count) return std::vector<std::int32_t>{};
        auto words = integers(result.words[group + 2], result.words[group + 3], 1);
        if (static_cast<std::size_t>(count) > words.size() / 10)
            throw database_error("Auxiliary directory is too short");
        return words;
    };
    result.integer_directory = read_directory(0);
    result.real_directory = read_directory(10);
    for (std::size_t i = 0; i < static_cast<std::size_t>(result.words[0]); ++i) {
        auxiliary_array<std::int32_t> entry;
        std::copy_n(result.integer_directory.begin() + i * 10, 10, entry.words.begin());
        if (entry.words[0] < 0) throw database_error("Negative auxiliary decoded count");
        entry.values = unpack_integers(integers(entry.words[3], entry.words[2], 1));
        result.integers.push_back(std::move(entry));
    }
    for (std::size_t i = 0; i < static_cast<std::size_t>(result.words[10]); ++i) {
        auxiliary_array<double> entry;
        std::copy_n(result.real_directory.begin() + i * 10, 10, entry.words.begin());
        if (entry.words[0] < 0) throw database_error("Negative auxiliary decoded count");
        entry.values = unpack_reals(reals(entry.words[3], entry.words[2], 1));
        result.reals.push_back(std::move(entry));
    }
    return result;
}
} // namespace deform
