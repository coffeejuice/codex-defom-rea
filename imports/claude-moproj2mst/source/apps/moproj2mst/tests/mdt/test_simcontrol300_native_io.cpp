// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <cmath>
#include <fstream>
#include <iterator>
#include <string_view>
#include <string>
#include <utility>
#include "m2m/common.hpp"
#include "m2m/engine/simcontrol300_io.hpp"
#include "../../src/engine/simcontrol300_qt_int_parser.hpp"

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace fs = std::filesystem;
namespace {
struct Rig {
    fs::path root;
    int warnings = 0;
    std::unique_ptr<Extra3DIo> io;
    Rig()
    {
        static unsigned serial = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("sim300-native-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root)); fs::create_directory(root);
        io = makeNativeExtra3DIo(qt::String(root.u16string()), 1252, [this] { ++warnings; });
    }
    void write(const char* name, const std::string& bytes)
    {
        std::ofstream file(m2m_test::scratchOutput(root / name), std::ios::binary);
        REQUIRE(file.is_open()); file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    }
    std::string read(const char* name)
    {
        std::ifstream file(root / name, std::ios::binary);
        return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    }
};
struct Handle {
    HANDLE value;
    Handle(const fs::path& path, DWORD share, DWORD access = GENERIC_READ)
        : value(CreateFileW(path.c_str(), access, share, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr))
    {
        REQUIRE(value != INVALID_HANDLE_VALUE);
    }
    ~Handle() { if (value != INVALID_HANDLE_VALUE) CloseHandle(value); }
    Handle(const Handle&) = delete;
};
template <class F> bool unsupported(F action)
{
    try { action(); } catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
}

TEST_CASE(native_extra3d_empty_files_enable_and_zero_count_probe_does_not_decode)
{
    Rig rig;
    for (const char* name : {"NBC.DAT", "SW2SP.DAT", "STRETCH.DAT", "B_WEIGHT.DAT"}) rig.write(name, "");
    rig.write("STRAIN_DST.DAT", std::string("\xff\xfe\0\x80", 4));
    auto form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
    CHECK(form->nbcEnabled && form->switchToSparseEnabled && form->stretchEnabled);
    CHECK(form->weightEnabled && form->strainDistributionEnabled);
    CHECK(form->nbc[0] == qt::L("0")); CHECK(form->switchToSparse == qt::L("0"));
    constructorRefreshExtra3D(*form);
    CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
    CHECK(rig.read("NBC.DAT") == "75\t\t\t105\t\r\n");
    CHECK(rig.read("SW2SP.DAT") == "0\t\r\n");
    CHECK(rig.read("STRAIN_DST.DAT") == "\r\n"); CHECK_EQ(rig.warnings, 0);
}

TEST_CASE(native_extra3d_real_reader_preserves_prefix_pushback_and_failure_continuation)
{
    Rig rig;
    rig.write("real", " \t1.25-2.5 3e+1x 99");
    std::array<double, 5> values{};
    REQUIRE(rig.io->readDoubles(qt::L("real"), values));
    CHECK(values[0] == 1.25 && values[1] == -2.5 && values[2] == 30.0);
    CHECK(values[3] == 0.0 && values[4] == 0.0); // bad x is repeatedly ungotten
    rig.write("real", "1e+ 4");
    REQUIRE(rig.io->readDoubles(qt::L("real"), values));
    CHECK(values[0] == 0.0 && values[1] == 4.0); // sticky status does not block later extraction
    CHECK(values[2] == 0.0);
    rig.write("real", "-0 .5 1.");
    REQUIRE(rig.io->readDoubles(qt::L("real"), values));
    CHECK(values[0] == 0.0 && std::signbit(values[0])); CHECK(values[1] == 0.5 && values[2] == 1.0);
}

TEST_CASE(native_extra3d_b_weight_one_double_accepts_utf8_bom_ascii_payload)
{
    Rig rig;
    const std::string utf8BomAsciiNumber("\xef\xbb\xbf" "1.5\r\n", 8);
    rig.write("B_WEIGHT.DAT", utf8BomAsciiNumber);
    std::array<double, 1> value{};
    REQUIRE(rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value));
    CHECK(value[0] == 1.5);

    // The admission is only for this reachable one-value sidecar and a valid
    // ASCII numeric payload; it does not claim malformed UTF-8 conversion.
    rig.write("B_WEIGHT.DAT", std::string("\xef\xbb\xbf" "\xc3\x28", 5));
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value); }));
    rig.write("B_WEIGHT.DAT", utf8BomAsciiNumber);
    std::array<double, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), twoValues); }));
    rig.write("other-real", utf8BomAsciiNumber);
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("other-real"), value); }));
}

TEST_CASE(native_extra3d_b_weight_one_double_accepts_utf16le_and_utf32le_bom_ascii_payload)
{
    Rig rig;
    const std::string utf16leAsciiNumber("\xff\xfe" "1\0.\0" "5\0\r\0\n\0", 12);
    rig.write("B_WEIGHT.DAT", utf16leAsciiNumber);
    std::array<double, 1> value{};
    REQUIRE(rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value));
    CHECK(value[0] == 1.5);

    // A zero-count read opens but does not decode; multi-value and other-path
    // reads retain their existing decoder gate.
    CHECK(rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), {}));
    std::array<double, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), twoValues); }));
    rig.write("other-real", utf16leAsciiNumber);
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("other-real"), value); }));

    rig.write("B_WEIGHT.DAT", std::string("\xff\xfe" "1\0", 3));
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value); }));
    rig.write("B_WEIGHT.DAT", std::string("\xff\xfe\xe9\0", 4));
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value); }));
    const unsigned char utf32leBytes[] = {
        0xff, 0xfe, 0x00, 0x00,
        '1', 0x00, 0x00, 0x00, '.', 0x00, 0x00, 0x00,
        '5', 0x00, 0x00, 0x00, '\r', 0x00, 0x00, 0x00,
        '\n', 0x00, 0x00, 0x00,
    };
    rig.write("B_WEIGHT.DAT", std::string(
        reinterpret_cast<const char*>(utf32leBytes), sizeof(utf32leBytes)));
    REQUIRE(rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value));
    CHECK(value[0] == 1.5);
}

TEST_CASE(native_extra3d_b_weight_one_double_accepts_other_qt_unicode_bom_ascii_payloads)
{
    Rig rig;
    std::array<double, 1> value{};
    const unsigned char utf16beBytes[] = {
        0xfe, 0xff, 0x00, '1', 0x00, '.', 0x00, '5', 0x00, '\r', 0x00, '\n',
    };
    rig.write("B_WEIGHT.DAT", std::string(
        reinterpret_cast<const char*>(utf16beBytes), sizeof(utf16beBytes)));
    REQUIRE(rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value));
    CHECK(value[0] == 1.5);

    const unsigned char utf32beBytes[] = {
        0x00, 0x00, 0xfe, 0xff,
        0x00, 0x00, 0x00, '1', 0x00, 0x00, 0x00, '.',
        0x00, 0x00, 0x00, '5', 0x00, 0x00, 0x00, '\r',
        0x00, 0x00, 0x00, '\n',
    };
    rig.write("B_WEIGHT.DAT", std::string(
        reinterpret_cast<const char*>(utf32beBytes), sizeof(utf32beBytes)));
    REQUIRE(rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value));
    CHECK(value[0] == 1.5);

    const unsigned char truncatedUtf32le[] = {0xff, 0xfe, 0x00, 0x00, '1'};
    rig.write("B_WEIGHT.DAT", std::string(
        reinterpret_cast<const char*>(truncatedUtf32le), sizeof(truncatedUtf32le)));
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("B_WEIGHT.DAT"), value); }));
}

TEST_CASE(native_extra3d_nbc_two_doubles_preserve_valid_row_and_eof_zero_tail)
{
    Rig rig;
    std::array<double, 2> values{};
    rig.write("NBC.DAT", "1.5\r\n2.5\r\n");
    REQUIRE(rig.io->readDoubles(qt::L("NBC.DAT"), values));
    CHECK(values[0] == 1.5);
    CHECK(values[1] == 2.5);

    // The original wrapper initializes all slots, then attempts extraction
    // while not atEnd and below count; a missing second value stays zero.
    rig.write("NBC.DAT", "1.5\r\n");
    values = {99.0, 99.0};
    REQUIRE(rig.io->readDoubles(qt::L("NBC.DAT"), values));
    CHECK(values[0] == 1.5);
    CHECK(values[1] == 0.0);

    const std::string bomRow("\xef\xbb\xbf" "1.5\r\n2.5\r\n", 13);
    rig.write("NBC.DAT", bomRow);
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("NBC.DAT"), values); }));

    std::array<double, 3> stretchValues{};
    rig.write("STRETCH.DAT", "\xef\xbb\xbf" "1.5\r\n2.5\r\n3.5\r\n");
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("STRETCH.DAT"), stretchValues); }));
}

TEST_CASE(native_extra3d_stretch_three_doubles_follow_separate_integer_open)
{
    Rig rig;
    rig.write("STRETCH.DAT", "1\r\n0.25\r\n0.5\r\n");
    auto full = fillExtra3D(*rig.io, NumericLocale::KnownC);
    REQUIRE(full.has_value());
    CHECK(full->stretchEnabled);
    CHECK(full->stretchKind == qt::L("1"));
    CHECK(full->stretch[0] == qt::L("0.25"));
    CHECK(full->stretch[1] == qt::L("0.5"));

    // The integer extraction opens the file independently; the double read
    // restarts at byte zero and leaves the missing third value zero at EOF.
    rig.write("STRETCH.DAT", "1\r\n0.25\r\n");
    auto partial = fillExtra3D(*rig.io, NumericLocale::KnownC);
    REQUIRE(partial.has_value());
    CHECK(partial->stretchEnabled);
    CHECK(partial->stretchKind == qt::L("1"));
    CHECK(partial->stretch[0] == qt::L("0.25"));
    CHECK(partial->stretch[1] == qt::L("0"));

    std::array<double, 3> threeValues{};
    rig.write("STRETCH.DAT", "\xef\xbb\xbf" "1\r\n0.25\r\n0.5\r\n");
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("STRETCH.DAT"), threeValues); }));
}

TEST_CASE(native_extra3d_integer_and_decoder_profiles_are_explicit)
{
    Rig rig;
    rig.write("integer", "-2147483648 2147483647 +7 0");
    std::array<int, 4> values{};
    REQUIRE(rig.io->readInts(qt::L("integer"), values));
    CHECK_EQ(values[0], (-2147483647 - 1)); CHECK_EQ(values[1], 2147483647);
    CHECK_EQ(values[2], 7); CHECK_EQ(values[3], 0);
    for (const char* outside : {"011", "0b10", "2147483648", "2x"}) {
        rig.write("integer", outside);
        CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), values); }));
    }
    rig.write("real", std::string("\xef\xbb\xbf" "1.5", 6));
    std::array<double, 1> real{};
    CHECK(unsupported([&] { rig.io->readDoubles(qt::L("real"), real); }));
    rig.write("SPRING.INI", "0\r\n");
    std::vector<SpringRecord> springs;
    CHECK(!rig.io->readSprings(springs));
    rig.write("SPRING.INI", "1\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(springs); }));
}

TEST_CASE(native_extra3d_spring_invalid_count_maps_to_empty_spring_result)
{
    Rig rig;
    std::vector<SpringRecord> springs;
    springs.push_back({});
    rig.write("SPRING.INI", "x\r\n");
    CHECK(!rig.io->readSprings(springs));
    CHECK(springs.empty());

    // The public Extra3D fill ignores a false/empty spring result and keeps
    // the scalar page, matching the original caller's count<=0 path.
    auto form = fillExtra3D(*rig.io, NumericLocale::KnownC);
    REQUIRE(form.has_value());

    // Keep the compatibility slice exact. Other invalid payloads and positive
    // counts remain guarded; no spring allocation or row parsing is admitted.
    rig.write("SPRING.INI", "x");
    CHECK(unsupported([&] { rig.io->readSprings(springs); }));
    rig.write("SPRING.INI", "?\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(springs); }));
    rig.write("SPRING.INI", "1\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(springs); }));
}

TEST_CASE(native_extra3d_sw2sp_qt4_base0_parser_matches_source_table)
{
    Rig rig;
    struct OracleRow {
        std::string_view bytes;
        int value;
        bool getterSucceeded;
        std::size_t nextByte;
        int operatorStatus;
    };
    // Numeric rows use the independent PE-verified table in
    // analysis/resume83-sw2sp-parser-independent-review.md. CP486/487 qualify
    // the finite malformed classes: first M has getter error 2; sign then Q
    // has getter error 1. The helper reports only failure/cursor/status.
    const OracleRow rows[] = {
        {"0xA\r\n", 10, true, 3, 0},
        {"0XA\r\n", 10, true, 3, 0},
        {"0X10\r\n", 16, true, 4, 0},
        {"0B10", 2, true, 4, 0},
        {"08\r\n", 8, true, 2, 0},
        {"09\r\n", 9, true, 2, 0},
        {"018\r\n", 1, true, 2, 0},
        {"0b12\r\n", 1, true, 3, 0},
        {"0b2\r\n", 0, false, 0, 2},
        {"0b", 0, false, 0, 2},
        {"0xG\r\n", 0, false, 2, 2},
        {"0x", 0, false, 2, 1},
        {"+0x10", 0, true, 2, 0},
        {"-010", -10, true, 4, 0},
        {"0", 0, true, 1, 0},
        {"+", 0, false, 1, 1},
        {"1,234", 1, true, 1, 0},
        {"18446744073709551616", 0, true, 20, 0},
        {"-9223372036854775809", 1, true, 20, 0},
        {"-18446744073709551615", -1, true, 21, 0},
        {"word\r\n", 0, false, 0, 2},
        {"!\r\n", 0, false, 0, 2},
        {"-x10\r\n", 0, false, 1, 2},
        {"+x\r\n", 0, false, 1, 2},
        {"- \r\n", 0, false, 1, 2},
    };

    std::array<int, 1> value{};
    for (const auto& row : rows) {
        const bool malformedOptIn = row.bytes == "word\r\n" || row.bytes == "!\r\n" ||
                                    row.bytes == "-x10\r\n" || row.bytes == "+x\r\n" || row.bytes == "- \r\n";
        const auto parsed = detail::parseQt4Base0Int(row.bytes, 0, malformedOptIn);
        CHECK(parsed.sourceQualified);
        CHECK_EQ(parsed.getterSucceeded, row.getterSucceeded);
        CHECK_EQ(parsed.nextByte, row.nextByte);
        const auto mapped = detail::applyQt4IntOperatorToFreshStream(parsed, row.bytes.size());
        CHECK_EQ(mapped.value, row.value);
        CHECK_EQ(mapped.freshStreamStatus, row.operatorStatus);

        rig.write("SW2SP.DAT", std::string(row.bytes));
        value[0] = 123;
        REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
        CHECK_EQ(value[0], row.value);
    }

    const auto minusXDefault = detail::parseQt4Base0Int("-x10\r\n");
    CHECK(!minusXDefault.sourceQualified);
    const auto minusXOffset = detail::parseQt4Base0Int(" -x10\r\n", 1, true);
    CHECK(!minusXOffset.sourceQualified);

    // Exhaust the finite first-marker M set (printable ASCII excluding digits
    // and signs), then every sign followed by printable nondigit Q.
    std::size_t firstClassRows = 0;
    for (unsigned char marker = 0x21; marker <= 0x7e; ++marker) {
        if ((marker >= '0' && marker <= '9') || marker == '+' || marker == '-') continue;
        const std::string bytes(1, static_cast<char>(marker));
        const auto parsed = detail::parseQt4Base0Int(bytes, 0, true);
        CHECK(parsed.sourceQualified);
        CHECK(!parsed.getterSucceeded);
        CHECK_EQ(parsed.nextByte, 0u);
        const auto mapped = detail::applyQt4IntOperatorToFreshStream(parsed, bytes.size());
        CHECK_EQ(mapped.value, 0);
        CHECK_EQ(mapped.freshStreamStatus, 2);
        ++firstClassRows;
    }
    CHECK_EQ(firstClassRows, 82u);
    std::size_t signClassRows = 0;
    for (char sign : {'+', '-'}) {
        for (unsigned char marker = 0x21; marker <= 0x7e; ++marker) {
            if (marker >= '0' && marker <= '9') continue;
            const std::string bytes{sign, static_cast<char>(marker)};
            const auto parsed = detail::parseQt4Base0Int(bytes, 0, true);
            CHECK(parsed.sourceQualified);
            CHECK(!parsed.getterSucceeded);
            CHECK_EQ(parsed.nextByte, 1u);
            const auto mapped = detail::applyQt4IntOperatorToFreshStream(parsed, bytes.size());
            CHECK_EQ(mapped.value, 0);
            CHECK_EQ(mapped.freshStreamStatus, 2);
            ++signClassRows;
        }
        for (unsigned char whitespace : {9, 10, 11, 12, 13, 32}) {
            const std::string bytes{sign, static_cast<char>(whitespace)};
            const auto parsed = detail::parseQt4Base0Int(bytes, 0, true);
            CHECK(parsed.sourceQualified);
            CHECK(!parsed.getterSucceeded);
            CHECK_EQ(parsed.nextByte, 1u);
            const auto mapped = detail::applyQt4IntOperatorToFreshStream(parsed, bytes.size());
            CHECK_EQ(mapped.value, 0);
            CHECK_EQ(mapped.freshStreamStatus, 2);
            ++signClassRows;
        }
        const std::string bytes{sign};
        const auto parsed = detail::parseQt4Base0Int(bytes, 0, true);
        CHECK(parsed.sourceQualified);
        CHECK(!parsed.getterSucceeded);
        CHECK_EQ(parsed.nextByte, 1u);
        CHECK_EQ(detail::applyQt4IntOperatorToFreshStream(parsed, bytes.size()).freshStreamStatus, 1);
    }
    CHECK_EQ(signClassRows, 180u);
    // Whitespace belongs to Q after a consumed sign, while control/non-ASCII
    // bytes and an offset/non-opt-in parser call remain out of scope.
    for (const std::string bytes : {std::string("!\x01", 2), std::string("!\x80", 2)})
        CHECK(!detail::parseQt4Base0Int(bytes, 0, true).sourceQualified);
    CHECK(!detail::parseQt4Base0Int("!", 0).sourceQualified);
    CHECK(!detail::parseQt4Base0Int(" !", 1, true).sourceQualified);
    for (const auto& [bytes, next] : {std::pair<std::string, std::size_t>{"\t !", 2}, {"\t -x", 3}}) {
        const auto parsed = detail::parseQt4Base0Int(bytes, 0, true);
        CHECK(parsed.sourceQualified);
        CHECK(!parsed.getterSucceeded);
        CHECK_EQ(parsed.nextByte, next);
        CHECK_EQ(detail::applyQt4IntOperatorToFreshStream(parsed, bytes.size()).freshStreamStatus, 2);
    }

    // Outside names, shared STRETCH, and multi-value SW2SP remain guarded.
    for (const char* admitted : {"words\r\n", "-x11\r\n", "-X10\r\n", "+ \r\n", "+", "-", "\t !", "\t -x"}) {
        rig.write("SW2SP.DAT", admitted);
        value[0] = 456;
        REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
        CHECK_EQ(value[0], 0);
        if (std::string_view(admitted) == "+" || std::string_view(admitted) == "-")
            CHECK_EQ(detail::applyQt4IntOperatorToFreshStream(
                         detail::parseQt4Base0Int(admitted, 0, true), std::string_view(admitted).size())
                         .freshStreamStatus,
                     1);
    }
    for (const char* outside : {"word\x01", "word\x80"}) {
        rig.write("SW2SP.DAT", outside);
        CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), value); }));
    }
    rig.write("integer", "word\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("STRETCH.DAT", "word\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("STRETCH.DAT"), value); }));
    rig.write("STRETCH.DAT", "-x10\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("STRETCH.DAT"), value); }));
    rig.write("integer", "-x10\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "word\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
    rig.write("SW2SP.DAT", "-x10\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_stretch_one_integer_uses_qt4_base0_profile)
{
    Rig rig;
    std::array<int, 1> value{};
    rig.write("STRETCH.DAT", "0X10\r\n");
    REQUIRE(rig.io->readInts(qt::L("STRETCH.DAT"), value));
    CHECK_EQ(value[0], 16);

    // The additional parser route remains limited to the identified sidecar
    // and one requested integer; legacy behavior remains on other paths.
    rig.write("integer", "0X10\r\n");
    REQUIRE(rig.io->readInts(qt::L("integer"), value));
    CHECK_EQ(value[0], 0);

    rig.write("STRETCH.DAT", "0X10\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("STRETCH.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_reads_the_qualified_base0_hex_profile)
{
    Rig rig;
    std::array<int, 1> value{};
    struct HexCase { const char* bytes; int expected; };
    const HexCase valid[] = {
        {"0x0\r\n", 0},
        {"0x2\r\n", 2},
        {"0xa\r\n", 10},
        {"0xA\r\n", 10},
        {"0xf\r\n", 15},
        {"0x1", 1}, // EOF terminates a valid digit sequence.
        {"0x10\r\n", 16},
        {"0x20\r\n", 32},
        {"0xdeadbeef\r\n", -559038737},
        {"0x80000000\r\n", (-2147483647 - 1)},
        {"0xffffffff\r\n", -1},
        {"0x100000000\r\n", 0},
        {"0x10x", 16}, // getter ungets the non-hex suffix; this read requests one int
    };
    for (const auto& test : valid) {
        rig.write("SW2SP.DAT", test.bytes);
        REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
        CHECK_EQ(value[0], test.expected);
    }

    // A recognized zero-digit hex prefix returns adapter success/value zero;
    // case-folded A is a successful digit even at EOF.
    for (const char* failed : {"0x", "0xg"}) {
        rig.write("SW2SP.DAT", failed);
        value[0] = 99;
        REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
        CHECK_EQ(value[0], 0);
    }
    rig.write("SW2SP.DAT", "0xA");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 10);
    rig.write("integer", "0xA\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "0xA\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_base0_prefix_and_signed_fallback_reads_one_sidecar_integer)
{
    Rig rig;
    std::array<int, 1> value{99};
    // The adapter retains the original QFile/open distinction: absent sidecar
    // returns false and leaves the pre-zeroed destination at zero.
    CHECK(!rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 0);

    struct FallbackCase { const char* bytes; int expected; };
    const FallbackCase cases[] = {
        {"0X10\r\n", 16},
        {"+0x10\r\n", 0}, {"-0x10\r\n", 0},
        {"+0X10\r\n", 0}, {"-0X10\r\n", 0},
    };
    for (const auto& test : cases) {
        rig.write("SW2SP.DAT", test.bytes);
        value[0] = 99;
        REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
        CHECK_EQ(value[0], test.expected);
    }

    // Multi-value continuation remains outside this one-value SW2SP profile.
    rig.write("SW2SP.DAT", "0X10 7\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
    CHECK_EQ(twoValues[0], 0);
}

TEST_CASE(native_extra3d_sw2sp_one_int_low32_overflow_is_narrowly_qualified)
{
    Rig rig;
    std::array<int, 1> value{};
    rig.write("SW2SP.DAT", "18446744073709551616\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 0);

    rig.write("SW2SP.DAT", "2147483648\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], (-2147483647 - 1));

    rig.write("SW2SP.DAT", "-2147483649\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 2147483647);

    // The adjacent in-range edge continues through the ordinary decimal path.
    rig.write("SW2SP.DAT", "-2147483648\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], (-2147483647 - 1));

    // Overflow arithmetic stays confined to the one-value SW2SP profile.
    rig.write("integer", "18446744073709551616\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("integer", "-2147483649\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    std::vector<SpringRecord> springs;
    for (const char* springCount : {"-2147483649\r\n", "18446744073709551616\r\n"}) {
        rig.write("SPRING.INI", springCount);
        CHECK(unsupported([&] { rig.io->readSprings(springs); }));
    }

    rig.write("SW2SP.DAT", "-2147483649\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));

    rig.write("SW2SP.DAT", "2147483648\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));

    rig.write("SW2SP.DAT", "18446744073709551616\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_reads_qualified_base0_radix_tokens)
{
    Rig rig;
    rig.write("SW2SP.DAT", "010\r\n");
    std::array<int, 1> value{};
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 8);
    rig.write("SW2SP.DAT", "02\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 2);
    rig.write("SW2SP.DAT", "0b10\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 2);
    rig.write("SW2SP.DAT", "0b12\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 1);
    rig.write("SW2SP.DAT", "0b2\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 0);
    struct RadixCase { const char* bytes; int expected; };
    const RadixCase additional[] = {
        {"011\r\n", 9}, {"03\r\n", 3}, {"0b11\r\n", 3},
        {"0b3\r\n", 0}, {"0B10", 2}, {"0b2", 0},
    };
    for (const auto& test : additional) {
        rig.write("SW2SP.DAT", test.bytes);
        value[0] = 77;
        REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
        CHECK_EQ(value[0], test.expected);
    }

    rig.write("integer", "0b12\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "0b12\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
    rig.write("SW2SP.DAT", "0b10\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_leading_zero_08_falls_back_to_decimal)
{
    Rig rig;
    std::array<int, 1> value{};
    rig.write("SW2SP.DAT", "08\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 8);
    rig.write("SW2SP.DAT", "09\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 9);

    // The added result is limited to the reachable one-value SW2SP call.
    rig.write("integer", "08\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "08\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_octal_prefix_before_invalid_digit_succeeds)
{
    Rig rig;
    std::array<int, 1> value{};
    rig.write("SW2SP.DAT", "018\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 1);

    rig.write("integer", "018\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "018\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_octal_2_to_32_narrows_to_low_dword)
{
    Rig rig;
    std::array<int, 1> value{};
    rig.write("SW2SP.DAT", "040000000000\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 0);

    rig.write("integer", "040000000000\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "040000000000\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_signed_leading_zero_is_decimal_in_one_value_profile)
{
    Rig rig;
    struct SignedDecimal { const char* bytes; int expected; };
    const SignedDecimal valid[] = {
        {"-010\r\n", -10},
        {"+010\r\n", 10},
        {"-011\r\n", -11},
        {"+008\r\n", 8},
        {"011\r\n", 9},
        {"-010x", -10},
        {"+0002147483649\r\n", -2147483647},
    };
    std::array<int, 1> value{};
    for (const auto& test : valid) {
        rig.write("SW2SP.DAT", test.bytes);
        REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
        CHECK_EQ(value[0], test.expected);
    }

    // Other paths and multi-value behavior remain on their separate guards.
    rig.write("integer", "-010\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "-010 7\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_sw2sp_c_locale_comma_is_unconsumed_after_exact_decimal_prefix)
{
    Rig rig;
    std::array<int, 1> value{};
    rig.write("SW2SP.DAT", "1,234\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 1);
    rig.write("SW2SP.DAT", "2,345\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 2);

    // A single extraction stops at comma, leaving both it and following
    // bytes unread; this exercises the adjacent-suffix cursor behavior.
    rig.write("SW2SP.DAT", "1,234x\r\n");
    REQUIRE(rig.io->readInts(qt::L("SW2SP.DAT"), value));
    CHECK_EQ(value[0], 1);
    rig.write("integer", "1,234\r\n");
    CHECK(unsupported([&] { rig.io->readInts(qt::L("integer"), value); }));
    rig.write("SW2SP.DAT", "1,234\r\n");
    std::array<int, 2> twoValues{};
    CHECK(unsupported([&] { rig.io->readInts(qt::L("SW2SP.DAT"), twoValues); }));
}

TEST_CASE(native_extra3d_write_denial_keeps_existing_prefix_and_continues_other_files)
{
    Rig rig;
    auto form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
    form->nbcEnabled = form->switchToSparseEnabled = true;
    rig.write("NBC.DAT", "existing NBC"); rig.write("SW2SP.DAT", "old");
    {
        Handle held(rig.root / "NBC.DAT", FILE_SHARE_READ);
        CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
        CHECK(rig.read("NBC.DAT") == "existing NBC");
        CHECK(rig.read("SW2SP.DAT") == "140000\t\r\n");
        CHECK_EQ(rig.warnings, 0);
    }
}

TEST_CASE(native_extra3d_readable_delete_failure_warns_only_nbc_and_continues)
{
    Rig rig;
    auto form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
    rig.write("NBC.DAT", "keep"); rig.write("SW2SP.DAT", "delete");
    {
        Handle held(rig.root / "NBC.DAT", FILE_SHARE_READ | FILE_SHARE_WRITE);
        CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
        CHECK(rig.read("NBC.DAT") == "keep"); CHECK(!fs::exists(rig.root / "SW2SP.DAT"));
        CHECK_EQ(rig.warnings, 1);
    }
    CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
    CHECK(!fs::exists(rig.root / "NBC.DAT")); CHECK_EQ(rig.warnings, 1);
}

TEST_CASE(native_extra3d_read_open_denial_is_remembered_as_disabled)
{
    Rig rig; rig.write("NBC.DAT", "75 105");
    std::optional<Extra3DPage> form;
    {
        Handle held(rig.root / "NBC.DAT", FILE_SHARE_WRITE, GENERIC_WRITE);
        form = fillExtra3D(*rig.io, NumericLocale::KnownC); REQUIRE(form.has_value());
        CHECK(!form->nbcEnabled); CHECK(form->nbc[0] == qt::L("75.0"));
    }
    // The control state is remembered even though the file is readable now.
    CHECK(applyExtra3D(*rig.io, *form) == ApplyResult::Completed);
    CHECK(!fs::exists(rig.root / "NBC.DAT"));
}

TEST_CASE(native_extra3d_missing_parents_are_not_created_and_directory_is_explicit)
{
    Rig rig;
    std::array<double, 1> value{99.0};
    CHECK(!rig.io->readDoubles(qt::L("missing/data"), value)); CHECK(value[0] == 0.0);
    CHECK(!rig.io->writeStringList(qt::L("missing/data"), {qt::L("text")}));
    CHECK(rig.io->removeIfReadable(qt::L("missing/data")));
    CHECK(!fs::exists(rig.root / "missing"));
    for (const char* relative : {"relative", "C:relative", "/drive-relative"}) {
        bool argument = false;
        try { makeNativeExtra3DIo(qt::L(relative), 1252, [] {}); }
        catch (const Error& error) { argument = error.code == M2M_E_ARG; }
        CHECK(argument);
    }
}

TEST_CASE(native_extra3d_directory_and_delete_path_profiles_are_explicit)
{
    Rig rig;
    const auto directory = rig.root.u16string();
    CHECK(unsupported([&] {
        makeNativeExtra3DIo(qt::String(directory + u"/" + std::u16string(MAX_PATH, u'x')),
            1252, [] {});
    }));
    CHECK(unsupported([&] {
        makeNativeExtra3DIo(qt::String(u"\\\\?\\" + directory), 1252, [] {});
    }));
    CHECK(unsupported([&] { rig.io->removeIfReadable(qt::String(u"\u4e2d")); }));
    // A guarded path never becomes a successful missing-file delete probe.
    CHECK(!fs::exists(rig.root / fs::path(u"\u4e2d")));
}

TEST_CASE(native_extra3d_qfile_read_write_accepts_unicode_directory_but_delete_stays_guarded)
{
    Rig rig;
    const fs::path unicodeRoot = rig.root / fs::path(u"Caf\u00e9");
    REQUIRE(fs::create_directory(unicodeRoot));
    {
        std::ofstream file(unicodeRoot / "NBC.DAT", std::ios::binary);
        REQUIRE(file.is_open()); file << "1.25 2.5";
    }

    auto io = makeNativeExtra3DIo(qt::String(unicodeRoot.u16string()), 1252, [] {});
    std::array<double, 2> values{};
    REQUIRE(io->readDoubles(qt::L("NBC.DAT"), values));
    CHECK(values[0] == 1.25 && values[1] == 2.5);
    CHECK(io->writeStringList(qt::L("written.DAT"), {qt::L("3.5")}));
    std::ifstream written(unicodeRoot / "written.DAT", std::ios::binary);
    REQUIRE(written.is_open());
    const std::string bytes{std::istreambuf_iterator<char>(written), std::istreambuf_iterator<char>()};
    CHECK_EQ(bytes, "3.5\t\r\n");

    CHECK(unsupported([&] { io->removeIfReadable(qt::L("NBC.DAT")); }));
    CHECK(fs::exists(unicodeRoot / "NBC.DAT"));
}

int main() { return testkit::run_all(); }
