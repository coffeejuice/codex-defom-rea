// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <stdexcept>
#include <string>
#include <vector>

#include "m2m/datafiles.hpp"
#include "../../src/doe/object_color_table_values.hpp"

using namespace m2m;
using namespace m2m::doe;
namespace {
ObjectColorTableValues filled(std::uint8_t value)
{
    ObjectColorTableValues result;
    for (auto& table : result.tables) for (auto& color : table) color.fill(value);
    return result;
}
std::string row(int id, int components, int first = 1)
{
    std::string out = std::to_string(id);
    for (int i = 0; i < components; ++i) out += " " + std::to_string(first + i);
    return out + "\r\n";
}
struct AdapterStop : std::runtime_error { AdapterStop() : std::runtime_error("isolated read stop") {} };
bool stopped(const auto& work)
{
    try { work(); }
    catch (const AdapterStop&) { return true; }
    return false;
}
}

TEST_CASE(object_color_open_failure_preserves_values_outputs_and_reader_position)
{
    auto values = filled(73);
    const auto before = values.tables;
    datafiles::TokenReader reader;
    datafiles::TextFile otherStream("7 8\r\n");
    std::int32_t previous = 0, first = 91, second = 92;
    CHECK(reader.read(otherStream, datafiles::TokenReader::Int, &previous,
        datafiles::TokenReader::kNewLine) == 2 && previous == 7);
    CHECK(!readIsolatedObjectColorTableValues(nullptr, reader, values, first, second));
    CHECK(values.tables == before && first == 91 && second == 92);
    CHECK(reader.read(otherStream, datafiles::TokenReader::Int, &previous, 0) == 0 && previous == 8);
}

TEST_CASE(object_color_rgb_100_rows_preserve_alpha_and_last_tables_then_read_two_tail_tokens)
{
    std::string bytes;
    for (int id = 1; id <= 100; ++id) bytes += row(id, 12);
    bytes += "42 43 99\r\n";
    datafiles::TextFile stream(bytes);
    datafiles::TokenReader reader;
    auto values = filled(77);
    std::int32_t first = -1, second = -2;
    CHECK(readIsolatedObjectColorTableValues(&stream, reader, values, first, second));
    const std::array<ObjectColorTableValues::Color, 6> expected = {{
        {1, 2, 3, 77}, {4, 5, 6, 77}, {7, 8, 9, 77}, {10, 11, 12, 77},
        {77, 77, 77, 77}, {77, 77, 77, 77}}};
    for (std::size_t table = 0; table < 6; ++table)
        for (const auto& color : values.tables[table]) CHECK(color == expected[table]);
    CHECK(first == 42 && second == 43);
    std::int32_t remaining = 0;
    CHECK(reader.read(stream, datafiles::TokenReader::Int, &remaining, 0) == 0 && remaining == 99);
}

TEST_CASE(object_color_rgba_100_rows_map_six_tables_and_narrow_bytes_without_clamping)
{
    const std::vector<int> components = {-1, 256, 511, -258, 128, 127, 0, 255,
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    std::string bytes;
    for (int id = 1; id <= 100; ++id) {
        bytes += std::to_string(id);
        for (int value : components) bytes += " " + std::to_string(value);
        bytes += "\r\n";
    }
    bytes += "-9 10\r\n";
    datafiles::TextFile stream(bytes);
    datafiles::TokenReader reader;
    auto values = filled(77);
    std::int32_t first = 0, second = 0;
    CHECK(readIsolatedObjectColorTableValues(&stream, reader, values, first, second));
    const std::array<ObjectColorTableValues::Color, 6> expected = {{
        {255, 0, 255, 254}, {128, 127, 0, 255}, {1, 2, 3, 4},
        {5, 6, 7, 8}, {9, 10, 11, 12}, {13, 14, 15, 16}}};
    for (std::size_t table = 0; table < 6; ++table)
        for (const auto& color : values.tables[table]) CHECK(color == expected[table]);
    CHECK(first == -9 && second == 10);
}

TEST_CASE(object_color_duplicate_rows_overwrite_and_invalid_ids_skip_payload_reads)
{
    std::string bytes = row(1, 12) + row(1, 24, 41);
    for (int i = 2; i < 100; ++i) bytes += row(i % 3 == 0 ? 0 : (i % 3 == 1 ? 101 : -1), 3, 999);
    bytes += "5 6\r\n";
    datafiles::TextFile stream(bytes);
    datafiles::TokenReader reader;
    auto values = filled(77);
    std::int32_t first = 0, second = 0;
    CHECK(readIsolatedObjectColorTableValues(&stream, reader, values, first, second));
    CHECK(values.tables[0][0] == ObjectColorTableValues::Color({41, 42, 43, 44}));
    CHECK(values.tables[5][0] == ObjectColorTableValues::Color({61, 62, 63, 64}));
    for (const auto& table : values.tables)
        for (std::size_t index = 1; index < 100; ++index)
            CHECK(table[index] == ObjectColorTableValues::Color({77, 77, 77, 77}));
    CHECK(first == 5 && second == 6);
}

TEST_CASE(object_color_short_rgb_file_reuses_prior_id_components_and_initial_alpha_at_eof)
{
    datafiles::TextFile stream(row(7, 12));
    datafiles::TokenReader reader;
    auto values = filled(77);
    std::int32_t first = 900, second = 901;
    CHECK(readIsolatedObjectColorTableValues(&stream, reader, values, first, second));
    // Later EOF reads preserve ID7 and final RGB10/11/12, choose RGBA because
    // return-2 !=13, and retain the initially zero local alpha destination.
    for (const auto& table : values.tables) {
        CHECK(table[6] == ObjectColorTableValues::Color({10, 11, 12, 0}));
        CHECK(table[0] == ObjectColorTableValues::Color({77, 77, 77, 77}));
        CHECK(table[99] == ObjectColorTableValues::Color({77, 77, 77, 77}));
    }
    CHECK(first == 900 && second == 901);
}

TEST_CASE(object_color_open_empty_stream_returns_true_without_changing_destinations)
{
    datafiles::TextFile stream("");
    datafiles::TokenReader reader;
    auto values = filled(77);
    const auto before = values.tables;
    std::int32_t first = 900, second = 901;
    CHECK(readIsolatedObjectColorTableValues(&stream, reader, values, first, second));
    CHECK(values.tables == before && first == 900 && second == 901);
}

TEST_CASE(object_color_adapter_stop_before_first_alpha_retains_already_stored_rgb)
{
    int lines = 0;
    auto stream = datafiles::TextFile::fromLineReader([&](std::string& line, int) {
        if (++lines > 1) throw AdapterStop();
        line = "1 101 102 103\n";
        return true;
    });
    datafiles::TokenReader reader;
    auto values = filled(9);
    std::int32_t first = 71, second = 72;
    CHECK(stopped([&] { readIsolatedObjectColorTableValues(&stream, reader, values, first, second); }));
    CHECK(values.tables[0][0] == ObjectColorTableValues::Color({101, 102, 103, 9}));
    CHECK(values.tables[1][0] == ObjectColorTableValues::Color({9, 9, 9, 9}));
    CHECK(first == 71 && second == 72 && lines == 2);
}

TEST_CASE(object_color_adapter_stop_before_fifth_alpha_retains_whole_fifth_color)
{
    int lines = 0;
    auto stream = datafiles::TextFile::fromLineReader([&](std::string& line, int) {
        if (++lines > 1) throw AdapterStop();
        line = row(1, 19);
        return true;
    });
    datafiles::TokenReader reader;
    auto values = filled(9);
    std::int32_t first = 71, second = 72;
    CHECK(stopped([&] { readIsolatedObjectColorTableValues(&stream, reader, values, first, second); }));
    CHECK(values.tables[3][0] == ObjectColorTableValues::Color({13, 14, 15, 16}));
    CHECK(values.tables[4][0] == ObjectColorTableValues::Color({9, 9, 9, 9}));
    CHECK(values.tables[5][0] == ObjectColorTableValues::Color({9, 9, 9, 9}));
    CHECK(first == 71 && second == 72 && lines == 2);
}

TEST_CASE(object_color_adapter_stop_on_second_tail_preserves_first_tail_commit)
{
    int lines = 0;
    auto stream = datafiles::TextFile::fromLineReader([&](std::string& line, int) {
        ++lines;
        if (lines > 101) throw AdapterStop();
        line = lines <= 100 ? "0\n" : "71\n";
        return true;
    });
    datafiles::TokenReader reader;
    auto values = filled(9);
    const auto before = values.tables;
    std::int32_t first = 70, second = 72;
    CHECK(stopped([&] { readIsolatedObjectColorTableValues(&stream, reader, values, first, second); }));
    CHECK(first == 71 && second == 72 && lines == 102);
    CHECK(values.tables == before);
    // Adapter-stop tests check partial writes only, not native CRT exceptions.
}

int main() { return testkit::run_all(); }
