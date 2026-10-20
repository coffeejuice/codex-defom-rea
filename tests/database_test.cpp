#include "deform/database.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <winioctl.h>
#endif

namespace {
using bytes = std::vector<std::uint8_t>;
constexpr std::size_t record_bytes = 512;
constexpr std::uint64_t nan_bits = 0x7ff8000000001234ULL;
constexpr std::uint64_t negative_zero = 0x8000000000000000ULL;
constexpr std::uint64_t infinity_bits = 0x7ff0000000000000ULL;

void require(bool condition, const char* message) {
    if (!condition) throw std::runtime_error(message);
}

// The source-backed layout uses one-based 512-byte physical records,
// a linked root with 127 page pointers + next-root pointer, and 128 rows of
// thirteen int32 words per page (REA 0x1403b92bc). The fixture below is deliberately
// small and its bytes are authored independently of database.cpp.
void word(bytes& out, std::size_t offset, std::int32_t value) {
    const auto bits = std::bit_cast<std::uint32_t>(value);
    for (std::size_t i = 0; i < 4; ++i) {
        out.at(offset + i) = static_cast<std::uint8_t>(bits >> (24 - i * 8));
    }
}
void real_bits(bytes& out, std::size_t offset, std::uint64_t value) {
    for (std::size_t i = 0; i < 8; ++i) {
        out.at(offset + i) = static_cast<std::uint8_t>(value >> (56 - i * 8));
    }
}
void real(bytes& out, std::size_t offset, double value) {
    real_bits(out, offset, std::bit_cast<std::uint64_t>(value));
}
void ascii(bytes& out, std::size_t offset, const std::string& text) {
    std::copy(text.begin(), text.end(), out.begin() + static_cast<std::ptrdiff_t>(offset));
}
constexpr std::size_t physical(std::int32_t record) {
    return (static_cast<std::size_t>(record) - 1) * record_bytes;
}
void row(bytes& out, std::int32_t page_record, std::size_t index,
         const std::array<std::int32_t, 13>& values) {
    for (std::size_t i = 0; i < values.size(); ++i)
        word(out, physical(page_record) + index * 13 * 4 + i * 4, values[i]);
}
const std::array<std::int32_t, 13> first_row{
    0x12345678, -7, 8, 8, 9, 4, 1, 3, 11000, -83, 17, 18, 19};
const std::array<std::int32_t, 13> header_only_row{
    -33, 8, 8, 0, 0, 4, 1, 3, 11000, 87, 22, 0, 0};

void step_header(bytes& out, std::size_t offset) {
    // Slots may contain character bytes, IEEE special values, or numeric sizes.
    real_bits(out, offset, 0x4142434445464748ULL);
    real_bits(out, offset + 8, negative_zero);
    real_bits(out, offset + 16, nan_bits);
    real(out, offset + 24, 17.5);
    real(out, offset + 32, -16.25);
    real(out, offset + 40, 0);
    real(out, offset + 48, 3);
    real(out, offset + 56, 3);
}
bytes fixture() {
    bytes out(32 * record_bytes);
    ascii(out, 0, "SFTC-DEF-3D-DB100-00");
    auto title = std::string("Independent synthetic fixture");
    title.resize(80, ' ');
    ascii(out, 512, title);
    ascii(out, 512 + 80, "11.00   ");
    ascii(out, 512 + 88, "06/30/14");
    ascii(out, 512 + 96, "10/01/26");
    ascii(out, 512 + 104, "10/20/26");
    const std::array<std::int32_t, 6> header_words{1, 1, 2, 100, 0, -77};
    for (std::size_t i = 0; i < header_words.size(); ++i)
        word(out, 512 + 112 + i * 4, header_words[i]);
    word(out, physical(3), 4);
    row(out, 4, 0, first_row);
    row(out, 4, 1, header_only_row);
    step_header(out, physical(17));
    step_header(out, physical(22));

    // One block per directory: capacity, destination, stored count, record.
    // Directory second words are deliberately nonzero and must not be mistaken
    // for block counts. Payloads have explicit packed-array metadata.
    const std::array<std::int32_t, 6> integer_directory{1, 123, 3, 1, 5, 20};
    const std::array<std::int32_t, 6> real_directory{1, -42, 3, 1, 5, 21};
    for (std::size_t i = 0; i < 6; ++i) {
        word(out, physical(18) + i * 4, integer_directory[i]);
        word(out, physical(19) + i * 4, real_directory[i]);
    }
    word(out, physical(18) + 24, 0x12345678);
    word(out, physical(18) + 28, -89);
    word(out, physical(19) + 24, -99);
    word(out, physical(19) + 28, 77);
    word(out, physical(19) + 32, std::numeric_limits<std::int32_t>::min());
    const std::array<std::int32_t, 5> integer_payload{
        -4, 17, std::numeric_limits<std::int32_t>::min(), 3, 0};
    for (std::size_t i = 0; i < 5; ++i)
        word(out, physical(20) + i * 4, integer_payload[i]);
    real_bits(out, physical(21), nan_bits);
    real_bits(out, physical(21) + 8, negative_zero);
    real_bits(out, physical(21) + 16, infinity_bits);
    real(out, physical(21) + 24, 3);
    real(out, physical(21) + 32, 0);
    return out;
}

class temporary_file {
public:
    temporary_file() {
        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();
        path = std::filesystem::temp_directory_path() /
            ("deform-db-test-" + std::to_string(stamp) + "-" +
             std::to_string(std::random_device{}()) + ".db");
    }
    ~temporary_file() {
        std::error_code ignored;
        std::filesystem::remove(path, ignored); // One owned file; never recursive.
    }
    void write(const bytes& data) const {
        std::ofstream stream(path, std::ios::binary | std::ios::trunc);
        stream.write(reinterpret_cast<const char*>(data.data()),
                     static_cast<std::streamsize>(data.size()));
        if (!stream) throw std::runtime_error("Cannot write test fixture");
    }
    std::filesystem::path path;
};

template <class F>
void original_error(int code, F operation) {
    bool caught = false;
    try { operation(); }
    catch (const deform::database_error& error) {
        caught = true;
        require(error.original_code() == code, "Unexpected original error code");
    }
    require(caught, "Expected original error branch was not taken");
}

void test_fields_and_absent_arrays() {
    temporary_file file;
    file.write(fixture());
    const deform::database db(file.path);
    require(db.header().version == 11000 && db.header().dimension == 23,
            "Unified version and dimension");
    require(db.header().title.size() == 80 && db.header().title.back() == ' ',
            "Fixed-width title should retain padding");
    require(db.header().words == std::array<std::int32_t, 6>{1, 1, 2, 100, 0, -77},
            "All global header words retained");
    require(db.records().size() == 2 && db.records()[0].words == first_row &&
            db.records()[1].words == header_only_row, "All 13 step table words retained");
    require(db.stored_steps() == std::vector<std::size_t>{0}, "Stored-step filtering");
    require(db.find_step(-7) == 0 && db.find_step(8) == 1, "Signed step-number lookup");
    original_error(201, [&] { (void)db.find_step(77); });
    original_error(202, [&] { (void)db.load_step(1); });
    const auto header = db.load_header(1);
    require(header.size() == 8, "Header-only rows remain readable");
    require(std::bit_cast<std::uint64_t>(header[0]) == 0x4142434445464748ULL &&
            std::bit_cast<std::uint64_t>(header[1]) == negative_zero &&
            std::bit_cast<std::uint64_t>(header[2]) == nan_bits,
            "Step header retains character and IEEE bit patterns");
    const auto loaded = db.load_step(0);
    require(loaded.record.words == first_row, "Loaded step preserves row words");
    require(loaded.integers ==
            std::vector<std::int32_t>{-4, 17, std::numeric_limits<std::int32_t>::min()},
            "Integer payload byte order and packing");
    require(loaded.reals.size() == 3 &&
            std::bit_cast<std::uint64_t>(loaded.reals[0]) == nan_bits &&
            std::bit_cast<std::uint64_t>(loaded.reals[1]) == negative_zero &&
            std::bit_cast<std::uint64_t>(loaded.reals[2]) == infinity_bits,
            "Real payload byte order and exact bits");
    const auto& block = loaded.integer_blocks.at(0);
    require(block.logical_capacity == 3 && block.destination == 1 &&
            block.stored_count == 5 && block.record == 20, "Integer block descriptor");
    require(loaded.real_blocks.at(0).record == 21, "Real block descriptor");
    require(loaded.integer_directory ==
            std::vector<std::int32_t>{1, 123, 3, 1, 5, 20, 0x12345678, -89},
            "Integer directory preserves opaque prefix and trailing words");
    require(loaded.real_directory ==
            std::vector<std::int32_t>{1, -42, 3, 1, 5, 21, -99, 77,
                                     std::numeric_limits<std::int32_t>::min()},
            "Real directory preserves opaque prefix and trailing words");
}

// Auxiliary fixtures follow the static schema in analysis/auxiliary-schema.json:
// 20 metadata words, two groups of ten; each directory uses ten-word descriptors
// with no prefix. The original invokes packing unconditionally for these arrays.
// These are independent authored fixtures, not a dynamic oracle for mode 5.
const std::array<std::int32_t, 20> auxiliary_words{
    2, 777, 24, 22, -11, 12, -13, 14, -15, 16,
    2, -888, 25, 23, 21, -22, 23, -24, 25, -26};
const std::array<std::int32_t, 10> auxiliary_integer_one{
    7, -123, 7, 26, 41, -42, 43, -44, 45, -46};
const std::array<std::int32_t, 10> auxiliary_integer_two{
    6, 234, 6, 27, -51, 52, -53, 54, -55, 56};
const std::array<std::int32_t, 10> auxiliary_real_one{
    5, -345, 6, 28, 61, -62, 63, -64, 65, -66};
const std::array<std::int32_t, 10> auxiliary_real_two{
    4, 456, 4, 29, -71, 72, -73, 74, -75, 76};

bytes auxiliary_fixture() {
    auto out = fixture();
    ascii(out, 512 + 80, "10.00   "); // Active step version 11000 enables mode 5.
    auto with_auxiliary = first_row;
    with_auxiliary[9] = 23;
    row(out, 4, 0, with_auxiliary);
    auto without_main_arrays = header_only_row;
    without_main_arrays[9] = 23;
    row(out, 4, 1, without_main_arrays);
    for (std::size_t i = 0; i < auxiliary_words.size(); ++i)
        word(out, physical(23) + i * 4, auxiliary_words[i]);
    for (std::size_t i = 0; i < 10; ++i) {
        word(out, physical(24) + i * 4, auxiliary_integer_one[i]);
        word(out, physical(24) + (10 + i) * 4, auxiliary_integer_two[i]);
        word(out, physical(25) + i * 4, auxiliary_real_one[i]);
        word(out, physical(25) + (10 + i) * 4, auxiliary_real_two[i]);
    }
    word(out, physical(24) + 80, 0x12345678);
    word(out, physical(24) + 84, -909);
    word(out, physical(25) + 80, -919);
    word(out, physical(25) + 84, 0x76543210);
    word(out, physical(25) + 88, -929);

    // One linked run and surrounding literals decode to seven integers.
    const std::array<std::int32_t, 7> integers_one{1, -7, 5, 0, 2, 7, 4};
    const std::array<std::int32_t, 6> integers_two{-3, 5, 4, 2, 4, 0};
    for (std::size_t i = 0; i < integers_one.size(); ++i)
        word(out, physical(26) + i * 4, integers_one[i]);
    for (std::size_t i = 0; i < integers_two.size(); ++i)
        word(out, physical(27) + i * 4, integers_two[i]);

    // A four-NaN run and a negative-zero literal decode to five doubles.
    real_bits(out, physical(28), nan_bits);
    real(out, physical(28) + 8, 4);
    real(out, physical(28) + 16, 0);
    real_bits(out, physical(28) + 24, negative_zero);
    real(out, physical(28) + 32, 5);
    real(out, physical(28) + 40, 3);
    real_bits(out, physical(29), infinity_bits);
    real(out, physical(29) + 8, 2.25);
    real(out, physical(29) + 16, 2);
    real(out, physical(29) + 24, 0);
    return out;
}

void test_auxiliary_fields_and_packing() {
    temporary_file file;
    file.write(auxiliary_fixture());
    const deform::database db(file.path);
    require(db.header().version == 10000 && db.records()[0].version() == 11000,
            "Auxiliary fixture distinguishes database and effective step versions");
    const auto loaded = db.load_auxiliary(0);
    require(loaded.present && loaded.words == auxiliary_words,
            "All auxiliary metadata preserved, including overwritten-in-original fields");
    require(loaded.integers.size() == 2 && loaded.reals.size() == 2,
            "Auxiliary entries have no directory prefix");
    require(loaded.integers[0].words == auxiliary_integer_one &&
            loaded.integers[1].words == auxiliary_integer_two &&
            loaded.reals[0].words == auxiliary_real_one &&
            loaded.reals[1].words == auxiliary_real_two,
            "All auxiliary descriptor words retained");
    auto expected_integer_directory = std::vector<std::int32_t>(
        auxiliary_integer_one.begin(), auxiliary_integer_one.end());
    expected_integer_directory.insert(expected_integer_directory.end(),
        auxiliary_integer_two.begin(), auxiliary_integer_two.end());
    expected_integer_directory.insert(expected_integer_directory.end(), {0x12345678, -909});
    auto expected_real_directory = std::vector<std::int32_t>(
        auxiliary_real_one.begin(), auxiliary_real_one.end());
    expected_real_directory.insert(expected_real_directory.end(),
        auxiliary_real_two.begin(), auxiliary_real_two.end());
    expected_real_directory.insert(expected_real_directory.end(), {-919, 0x76543210, -929});
    require(loaded.integer_directory == expected_integer_directory &&
            loaded.real_directory == expected_real_directory,
            "Auxiliary raw directories preserve trailing words");
    require(loaded.integers[0].values == std::vector<std::int32_t>{1, -7, -7, -7, -7, -7, 2} &&
            loaded.integers[0].size_matches(), "Auxiliary integer run decode");
    require(loaded.integers[1].values == std::vector<std::int32_t>{-3, 5, 4, 2} &&
            !loaded.integers[1].size_matches(),
            "Auxiliary integer size mismatch is reported without inventing an error");
    require(loaded.reals[0].size_matches() && loaded.reals[0].values.size() == 5,
            "Auxiliary real run length");
    for (std::size_t i = 0; i < 4; ++i)
        require(std::bit_cast<std::uint64_t>(loaded.reals[0].values[i]) == nan_bits,
                "Auxiliary real runs preserve NaN payloads");
    require(std::bit_cast<std::uint64_t>(loaded.reals[0].values[4]) == negative_zero,
            "Auxiliary real literal preserves signed zero");
    require(!loaded.reals[1].size_matches() && loaded.reals[1].values.size() == 2 &&
            std::bit_cast<std::uint64_t>(loaded.reals[1].values[0]) == infinity_bits &&
            loaded.reals[1].values[1] == 2.25,
            "Auxiliary real size mismatch preserves decoded payload");
    require(!db.records()[1].has_arrays() && db.load_auxiliary(1).integers.size() == 2,
            "Auxiliary loading is independent of main array availability");
}

void test_auxiliary_absent_and_version_gate() {
    temporary_file absent_file;
    auto absent = fixture();
    auto absent_row = first_row;
    absent_row[9] = 0;
    row(absent, 4, 0, absent_row);
    absent_file.write(absent);
    const auto empty = deform::database(absent_file.path).load_auxiliary(0);
    require(!empty.present && empty.words == std::array<std::int32_t, 20>{} &&
            empty.integer_directory.empty() && empty.real_directory.empty() &&
            empty.integers.empty() && empty.reals.empty(),
            "Zero auxiliary pointer returns absent without reading");

    temporary_file gated_file;
    auto gated = fixture();
    auto gated_row = first_row;
    gated_row[8] = 10000;
    gated_row[9] = std::numeric_limits<std::int32_t>::max(); // Invalid if read.
    row(gated, 4, 0, gated_row);
    gated_file.write(gated);
    const deform::database db(gated_file.path);
    require(db.header().version == 11000 && db.records()[0].version() == 10000 &&
            !db.load_auxiliary(0).present,
            "Effective step version gate precedes pointer access regardless of database version");

    temporary_file no_entries_file;
    auto no_entries = auxiliary_fixture();
    auto metadata = auxiliary_words;
    metadata[0] = metadata[10] = 0;
    metadata[2] = metadata[12] = -123; // Ignored when entry count is zero.
    metadata[3] = metadata[13] = -456;
    for (std::size_t i = 0; i < metadata.size(); ++i)
        word(no_entries, physical(23) + i * 4, metadata[i]);
    no_entries_file.write(no_entries);
    const auto present_empty = deform::database(no_entries_file.path).load_auxiliary(0);
    require(present_empty.present && present_empty.words == metadata &&
            present_empty.integer_directory.empty() && present_empty.real_directory.empty() &&
            present_empty.integers.empty() && present_empty.reals.empty(),
            "Zero entry counts retain metadata while skipping directories and payloads");
}

void test_legacy_effective_versions_preserve_raw_words() {
    // Legacy files derive dimension/version from the database header. Sentinel
    // row words must remain raw even though loading uses the effective values.
    for (const int dimension : {2, 3}) {
        temporary_file file;
        auto data = fixture();
        ascii(data, 0, dimension == 2 ? "SFTC-DEFORM-2D-DB450" : "SFTC-DEFORM-3D-DB230");
        ascii(data, 512 + 80, dimension == 2 ? " 5.0    " : " 6.0    ");
        auto legacy_row = first_row;
        legacy_row[7] = -777;
        legacy_row[8] = 12000; // Would wrongly enable auxiliary reading if used.
        legacy_row[9] = std::numeric_limits<std::int32_t>::max();
        if (dimension == 2) {
            // The legacy 2D path addresses packed arrays directly, not block
            // directories. Records 20/21 already carry the authored payloads.
            legacy_row[3] = legacy_row[4] = 5;
            legacy_row[11] = 20;
            legacy_row[12] = 21;
        }
        row(data, 4, 0, legacy_row);
        file.write(data);
        const deform::database db(file.path);
        const auto& record = db.records()[0];
        const int expected_version = dimension == 2 ? 5000 : 6000;
        require(db.header().dimension == dimension && db.header().version == expected_version,
                "Legacy header dimension/version decoding");
        require(record.words == legacy_row && record.words[7] == -777 &&
                record.words[8] == 12000,
                "Legacy dimension/version sentinels remain in raw row words");
        require(record.dimension() == dimension && record.version() == expected_version,
                "Legacy effective values come from the database header");
        const auto loaded = db.load_step(0);
        require(loaded.record.words == legacy_row &&
                loaded.record.dimension() == dimension &&
                loaded.record.version() == expected_version,
                "Loaded legacy record retains raw and effective fields separately");
        require(loaded.integers ==
                std::vector<std::int32_t>{-4, 17, std::numeric_limits<std::int32_t>::min()},
                "Legacy loader dispatch uses effective dimension/version");
        require(!db.load_auxiliary(0).present,
                "Legacy effective version gates invalid auxiliary pointer despite raw version word");
    }
}

void test_page_and_root_boundaries() {
    temporary_file file;
    auto data = fixture();
    // 127 pages * 128 rows fill the first root. The final row is on root two.
    constexpr int count = 127 * 128 + 1;
    constexpr int second_root = 4 + 127 * 13;
    constexpr int last_page = second_root + 1;
    data.resize(static_cast<std::size_t>(last_page + 13) * record_bytes);
    word(data, 512 + 120, count);
    for (int i = 0; i < 127; ++i)
        word(data, physical(3) + static_cast<std::size_t>(i) * 4, 4 + i * 13);
    word(data, physical(3) + 127 * 4, second_root);
    word(data, physical(second_root), last_page);
    auto final = header_only_row;
    final[0] = 0x76543210;
    final[1] = -123456;
    row(data, last_page, 0, final);
    file.write(data);
    const deform::database db(file.path);
    require(db.records().size() == count, "Linked root/page counts");
    require(db.records().back().words == final, "Linked root/page traversal preserves final row");
    require(db.find_step(-123456) == count - 1, "Lookup beyond first index root");
}

void mark_sparse(const std::filesystem::path& path) {
#ifdef _WIN32
    const auto handle = CreateFileW(path.c_str(), GENERIC_WRITE,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (handle == INVALID_HANDLE_VALUE) throw std::runtime_error("Cannot open sparse test file");
    DWORD returned{};
    const bool success = DeviceIoControl(handle, FSCTL_SET_SPARSE, nullptr, 0,
                                        nullptr, 0, &returned, nullptr) != 0;
    CloseHandle(handle);
    if (!success) throw std::runtime_error("Cannot mark test file sparse");
#else
    (void)path; // Seek/write creates a sparse extent on normal POSIX filesystems.
#endif
}

void test_offset_beyond_four_gib() {
    temporary_file file;
    auto data = fixture();
    constexpr std::int32_t distant_record = 8388620;
    constexpr std::uint64_t offset = (std::uint64_t{distant_record} - 1) * 512;
    static_assert(offset > (std::uint64_t{1} << 32));
    auto distant_row = first_row;
    distant_row[10] = distant_record;
    row(data, 4, 0, distant_row);
    file.write(data);
    mark_sparse(file.path);
    bytes header(64);
    step_header(header, 0);
    {
        std::fstream stream(file.path, std::ios::binary | std::ios::in | std::ios::out);
        stream.seekp(static_cast<std::streamoff>(offset));
        stream.write(reinterpret_cast<const char*>(header.data()),
                     static_cast<std::streamsize>(header.size()));
        require(static_cast<bool>(stream), "Write sparse distant header");
    }
    const deform::database db(file.path);
    require(db.file_size() == offset + 64, "64-bit file-size reporting");
    const auto decoded = db.load_header(0);
    require(decoded.size() == 8 &&
            std::bit_cast<std::uint64_t>(decoded[0]) == 0x4142434445464748ULL,
            "Read header beyond four GiB");
    require(db.load_step(0).integers.size() == 3, "Full step with distant header");
}

} // namespace

int main() {
    try {
        test_fields_and_absent_arrays();
        test_auxiliary_fields_and_packing();
        test_auxiliary_absent_and_version_gate();
        test_legacy_effective_versions_preserve_raw_words();
        test_page_and_root_boundaries();
        test_offset_beyond_four_gib();
        std::cout << "database tests passed\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
