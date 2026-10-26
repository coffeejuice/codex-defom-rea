// ctest-labels: m2m-mdt
#include "../support/test_paths.hpp"
#include <testkit/check.hpp>
#include <array>
#include <cstdint>
#include <fstream>
#include <initializer_list>
#include <iterator>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include "m2m/common.hpp"
#include "m2m/engine/simcontrol300_io.hpp"

using namespace m2m;
using namespace m2m::engine::simcontrol300;
namespace fs = std::filesystem;

namespace {
struct Rig {
    fs::path root;
    std::unique_ptr<Extra3DIo> io;

    Rig()
    {
        static unsigned serial = 0;
        root = m2m_test::scratchOutput(m2m_test::scratchRoot() /
            ("sim300-spring-reader-" + std::to_string(GetCurrentProcessId()) + "-" +
             std::to_string(GetTickCount64()) + "-" + std::to_string(++serial)));
        REQUIRE(!fs::exists(root));
        fs::create_directory(root);
        io = makeNativeExtra3DIo(qt::String(root.u16string()), 1252, [] {});
    }

    ~Rig()
    {
        std::error_code error;
        fs::remove_all(root, error);
    }

    void write(const char* name, const std::string& bytes)
    {
        std::ofstream file(m2m_test::scratchOutput(root / name), std::ios::binary);
        REQUIRE(file.is_open());
        file.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
        REQUIRE(file.good());
    }

    std::string read(const char* name) const
    {
        std::ifstream file(m2m_test::scratchOutput(root / name), std::ios::binary);
        REQUIRE(file.is_open());
        return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    }
};

template <class F> bool unsupported(F action)
{
    try { action(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}

std::string unicodeBomAscii(const std::string& payload, int width, bool bigEndian)
{
    std::string result;
    if (width == 1) result.assign("\xef\xbb\xbf", 3);
    else if (width == 2 && bigEndian) result.assign("\xfe\xff", 2);
    else if (width == 2) result.assign("\xff\xfe", 2);
    else if (bigEndian) result.assign("\x00\x00\xfe\xff", 4);
    else result.assign("\xff\xfe\x00\x00", 4);

    for (unsigned char value : payload) {
        if (width == 1) result.push_back(static_cast<char>(value));
        else if (bigEndian) {
            for (int byte = width - 1; byte >= 0; --byte)
                result.push_back(static_cast<char>((value >> (8 * byte)) & 0xff));
        } else {
            for (int byte = 0; byte < width; ++byte)
                result.push_back(static_cast<char>((value >> (8 * byte)) & 0xff));
        }
    }
    return result;
}
}

TEST_CASE(native_spring_reader_parses_bounded_complete_ascii_rows)
{
    Rig rig;
    rig.write("SPRING.INI", "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n");

    std::vector<SpringRecord> rows(1);
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].firstObject == 10);
    CHECK(rows[0].firstValue == 0.5);
    CHECK(rows[0].secondValue == 1.5);
    CHECK(rows[0].secondObject == 20);
    CHECK(rows[0].direction[0] == 0.0);
    CHECK(rows[0].direction[1] == 1.0);
    CHECK(rows[0].direction[2] == 0.0);
    CHECK(rows[0].flag == 0);
    CHECK(!fs::exists(rig.root / "SPRING2.INI"));

    // Exact whitespace emitted by the source-qualified one-record writer.
    rig.write("SPRING.INI", "1\t\n10\t\n0.5\t1.5\t20\t0\t\n1\t0\t\n\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].firstObject == 10);
    CHECK(rows[0].firstValue == 0.5);
    CHECK(rows[0].secondValue == 1.5);
    CHECK(rows[0].secondObject == 20);
    CHECK(rows[0].direction[0] == 0.0);
    CHECK(rows[0].direction[1] == 1.0);
    CHECK(rows[0].direction[2] == 0.0);
    CHECK(rows[0].flag == 0);

    rig.write("SPRING.INI",
        "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].firstObject == 10);
    CHECK(rows[0].firstValue == 0.5);
    CHECK(rows[0].secondValue == 1.5);
    CHECK(rows[0].secondObject == 20);
    CHECK(rows[0].direction[0] == 0.0);
    CHECK(rows[0].direction[1] == 1.0);
    CHECK(rows[0].direction[2] == 0.0);
    CHECK(rows[0].flag == 0);
    CHECK(rows[1].firstObject == 30);
    CHECK(rows[1].firstValue == 2.5);
    CHECK(rows[1].secondValue == 3.5);
    CHECK(rows[1].secondObject == 40);
    CHECK(rows[1].direction[0] == 1.0);
    CHECK(rows[1].direction[1] == 0.0);
    CHECK(rows[1].direction[2] == 1.0);
    CHECK(rows[1].flag == 0);

    // Exact count-two bytes emitted by the retained Save writer for these rows.
    rig.write("SPRING.INI",
        "2\t\n10\t\n0.5\t1.5\t20\t0\t\n1\t0\t\n"
        "30\t\n2.5\t3.5\t40\t1\t\n0\t1\t\n\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].firstObject == 10 && rows[0].secondObject == 20);
    CHECK(rows[0].firstValue == 0.5 && rows[0].secondValue == 1.5);
    CHECK(rows[0].direction[0] == 0.0 && rows[0].direction[1] == 1.0 && rows[0].direction[2] == 0.0);
    CHECK(rows[1].firstObject == 30 && rows[1].secondObject == 40);
    CHECK(rows[1].firstValue == 2.5 && rows[1].secondValue == 3.5);
    CHECK(rows[1].direction[0] == 1.0 && rows[1].direction[1] == 0.0 && rows[1].direction[2] == 1.0);
    CHECK(rows[1].flag == 0);

    // The row grammar is value-based, not a literal fixture allowlist.
    rig.write("SPRING.INI",
        "2\t\n10\t\n0.5\t1.5\t20\t0\t\n1\t0\t\n"
        "30\t\n2.5\t3.5\t40\t1\t\n0\t2\t\n\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[1].direction[2] == 2.0);

    rig.write("SPRING.INI", "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());

    rig.write("SPRING.INI",
        "3\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n"
        "50\r\n3.5\r\n4.5\r\n60\r\n0\r\n1\r\n0\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 3);
    CHECK(rows[2].firstObject == 50);
    CHECK(rows[2].firstValue == 3.5);
    CHECK(rows[2].secondValue == 4.5);
    CHECK(rows[2].secondObject == 60);
    CHECK(rows[2].direction[0] == 0.0 && rows[2].direction[1] == 1.0 && rows[2].direction[2] == 0.0);
    CHECK(rows[2].flag == 0);

    // Tokens follow Qt-like ASCII whitespace, and decimal exponent notation
    // stays within the explicitly admitted finite C-locale grammar.
    rig.write("SPRING.INI", "1\t+10 5e-1\r\n.5 +20\t-0 1.25 0 trailing 99\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].firstObject == 10);
    CHECK(rows[0].firstValue == 0.5);
    CHECK(rows[0].secondValue == 0.5);
    CHECK(rows[0].secondObject == 20);
    CHECK(rows[0].direction[0] == 0.0);
    CHECK(rows[0].direction[1] == 1.25);
    CHECK(rows[0].direction[2] == 0.0);

    // The source loop stops after count rows and does not extract trailing
    // tokens; ASCII suffixes therefore remain ignored.
    rig.write("SPRING.INI", "1 10 .5 1.5 20 0 1 0 999 trailing\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].firstObject == 10);
    CHECK(rows[0].direction[1] == 1.0);

    std::string maxRows = "256\n";
    for (int row = 0; row < 256; ++row) {
        maxRows += std::to_string(row + 1) + " 0.5 1.5 " + std::to_string(row + 101) + " 0 1 0\n";
    }
    rig.write("SPRING.INI", maxRows);
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 256);
    CHECK(rows.front().firstObject == 1);
    CHECK(rows.back().firstObject == 256);
    CHECK(rows.back().secondObject == 356);

    rig.write("SPRING.INI", "257\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());

    for (const std::string malformed : {
             "1 010 0.5 1.5 20 0 1 0",       // leading-zero radix ambiguity
             "1 10 1,234 1.5 20 0 1 0",     // grouping separator not admitted
             "1 10 nan 1.5 20 0 1 0",       // non-finite spelling
             "1 10 1e+ 1.5 20 0 1 0",       // incomplete exponent
             "1 10 1e9999 1.5 20 0 1 0",    // non-finite overflow
             "1 10 0.5 1.5 20 0 1"}) {     // incomplete final row
        rig.write("SPRING.INI", malformed);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
    }

    std::string nonAscii = "1 10 0.5 1.5 20 0 1 0 ";
    nonAscii.push_back(static_cast<char>(0x80));
    rig.write("SPRING.INI", nonAscii);
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());

    rig.write("SPRING.INI", "0\r\n");
    CHECK(!rig.io->readSprings(rows));
    CHECK(rows.empty());
    rig.write("SPRING.INI", "x\r\n");
    CHECK(!rig.io->readSprings(rows));
    CHECK(rows.empty());
}

TEST_CASE(native_spring_reader_maps_present_readable_ascii_companion_flags)
{
    Rig rig;
    const std::string twoRows =
        "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n";
    rig.write("SPRING.INI", twoRows);
    rig.write("SPRING2.INI", "1\t-2\t\r\n99\r\n");
    const auto originalSpring = rig.read("SPRING.INI");
    const auto originalFlags = rig.read("SPRING2.INI");

    std::vector<SpringRecord> rows;
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == -2);
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == originalFlags);

    // EOF after one complete flag leaves the pre-zeroed suffix at zero.
    rig.write("SPRING2.INI", "7\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 7);
    CHECK(rows[1].flag == 0);

    // A present but empty readable file is the all-zero flag profile.
    rig.write("SPRING2.INI", "");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 0);
    CHECK(rows[1].flag == 0);

    // A valid prefix is returned and the stopping x remains unread. With the
    // second requested extraction, that same x fails again and leaves zero.
    rig.write("SPRING2.INI", "1x\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == "1x\r\n");

    // Exact uppercase-X companion spelling follows the same two extraction
    // order: valid prefix 1, then a failed retry at the still-unread X.
    rig.write("SPRING2.INI", "1X\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == "1X\r\n");

    for (const auto& adjacentUpperX : {std::string("1X 2\r\n"), std::string("1XX\r\n")}) {
        rig.write("SPRING2.INI", adjacentUpperX);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == originalSpring);
        CHECK(rig.read("SPRING2.INI") == adjacentUpperX);
    }
    rig.write("SPRING.INI", "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n");
    rig.write("SPRING2.INI", "1X\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING2.INI") == "1X\r\n");
    rig.write("SPRING.INI",
        "3\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n"
        "50\r\n3.5\r\n4.5\r\n60\r\n0\r\n1\r\n0\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING2.INI") == "1X\r\n");
    rig.write("SPRING.INI", originalSpring);
    // Restore the earlier lowercase fixture expected by the following
    // one-row case; uppercase X is intentionally count-two-only.
    rig.write("SPRING2.INI", "1x\r\n");

    rig.write("SPRING.INI", "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 1);
    CHECK(rig.read("SPRING2.INI") == "1x\r\n");

    rig.write("SPRING.INI", twoRows);
    rig.write("SPRING2.INI", "x 2\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 0);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == "x 2\r\n");

    // C-locale grouping punctuation is ungot after a valid decimal prefix.
    // Once the next extraction starts at that comma, repeated requested flags
    // are zero because the same comma remains unread.
    const std::string oneRow = "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n";
    rig.write("SPRING.INI", oneRow);
    const auto originalOneRow = rig.read("SPRING.INI");
    rig.write("SPRING2.INI", "1,234\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 1);
    CHECK(rig.read("SPRING.INI") == originalOneRow);
    CHECK(rig.read("SPRING2.INI") == "1,234\r\n");

    rig.write("SPRING.INI", twoRows);
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == "1,234\r\n");

    const std::string threeRows =
        "3\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n"
        "50\r\n3.5\r\n4.5\r\n60\r\n0\r\n1\r\n0\r\n";
    rig.write("SPRING.INI", threeRows);
    rig.write("SPRING2.INI", "1,234\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 3);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == 0);
    CHECK(rows[2].flag == 0);
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "1,234\r\n");

    // The QTextStream getter succeeds in the wider positive range and the int
    // operator stores its low 32 bits.
    rig.write("SPRING.INI", oneRow);
    rig.write("SPRING2.INI", "2147483648\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == (-2147483647 - 1));
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "2147483648\r\n");

    rig.write("SPRING.INI", twoRows);
    rig.write("SPRING2.INI", "2147483648\t4294967295\t\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == (-2147483647 - 1));
    CHECK(rows[1].flag == -1);
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == "2147483648\t4294967295\t\r\n");

    rig.write("SPRING.INI", oneRow);
    rig.write("SPRING2.INI", "4294967296\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "4294967296\r\n");

    // Nonnegative, non-leading-zero decimal extraction accumulates through
    // uint64 wrap in Qt, then the int operator narrows to its low dword.
    for (const auto& [token, expected] : std::array<std::pair<const char*, int>, 3>{{
             {"4294967297\r\n", 1},
             {"18446744073709551616\r\n", 0},
             {"18446744073709551617\r\n", 1}}}) {
        rig.write("SPRING2.INI", token);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expected);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == token);
    }

    rig.write("SPRING2.INI", "18446744073709551617");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 1);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "18446744073709551617");

    rig.write("SPRING2.INI", "-2147483649\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 2147483647);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "-2147483649\r\n");

    for (const std::string hexToken : {"0xA\r\n", "0XA\r\n"}) {
        rig.write("SPRING2.INI", hexToken);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == 10);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == hexToken);
    }

    rig.write("SPRING2.INI", "0xA0\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 160);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0xA0\r\n");

    for (const auto& [hexBytes, expectedFlag] : std::array{
             std::pair{"0x1\r\n", 1}, std::pair{"0xA1\r\n", 161},
             std::pair{"0XA0", 160}, std::pair{"0XfF\t", 255},
             std::pair{"0x80000000\r\n", (-2147483647 - 1)},
             std::pair{"0xFFFFFFFF", -1}, std::pair{"0x100000001\r\n", 1},
             std::pair{"0xA00\r\n", 2560}}) {
        rig.write("SPRING2.INI", hexBytes);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expectedFlag);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == hexBytes);
    }

    rig.write("SPRING2.INI", "0xG\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0xG\r\n");

    rig.write("SPRING2.INI", "0x");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0x");

    rig.write("SPRING2.INI", "0x\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0x\r\n");

    rig.write("SPRING2.INI", "0X\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0X\r\n");

    rig.write("SPRING2.INI", "0X");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0X");

    rig.write("SPRING2.INI", "018\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 1);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "018\r\n");

    rig.write("SPRING2.INI", "019\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 1);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "019\r\n");

    for (const auto& [octalBytes, expectedFlag] : std::array{
             std::pair{"02\r\n", 2}, std::pair{"010\r\n", 8},
             std::pair{"017\r\n", 15}, std::pair{"010", 8},
             std::pair{"017777777777\r\n", 2147483647}, std::pair{"011\t", 9}}) {
        rig.write("SPRING2.INI", octalBytes);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expectedFlag);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == octalBytes);
    }

    for (const auto& [negativeBytes, expectedFlag] : std::array{
             std::pair{"-2147483650\r\n", 2147483646},
             std::pair{"-4294967297\r\n", -1},
             std::pair{" -4294967297\r\n", -1},
             std::pair{"-9223372036854775807", 1},
             std::pair{"-9223372036854775808\r\n", 0},
             std::pair{"-9223372036854775809\r\n", 1},
             std::pair{"-18446744073709551615\r\n", -1},
             std::pair{"-18446744073709551616", 0},
             std::pair{"-18446744073709551617\t", -1}}) {
        rig.write("SPRING2.INI", negativeBytes);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expectedFlag);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == negativeBytes);
    }

    for (const auto& [positiveBytes, expectedFlag] : std::array{
             std::pair{"+2147483648\t", (-2147483647 - 1)},
             std::pair{"+4294967297\r\n", 1},
             std::pair{"+9223372036854775807", -1},
             std::pair{"+9223372036854775808\r\n", 0},
             std::pair{"+18446744073709551616", 0},
             std::pair{"+18446744073709551617\r\n", 1},
             std::pair{"+36893488147419103233\t", 1}}) {
        rig.write("SPRING2.INI", positiveBytes);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expectedFlag);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == positiveBytes);
    }

    for (const auto& [signedZeroBytes, expectedFlag] : std::array{
             std::pair{"-010\r\n", -10}, std::pair{"+010\t", 10},
             std::pair{" -0012", -12}, std::pair{"+000\r\n", 0}}) {
        rig.write("SPRING2.INI", signedZeroBytes);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expectedFlag);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == signedZeroBytes);
    }

    rig.write("SPRING2.INI", "09\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 9);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "09\r\n");

    for (const auto& [decimalFallbackBytes, expectedFlag] : std::array{
             std::pair{"08\r\n", 8}, std::pair{"089\t", 89},
             std::pair{"090", 90}, std::pair{"09223372036854775807\r\n", -1}}) {
        rig.write("SPRING2.INI", decimalFallbackBytes);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expectedFlag);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == decimalFallbackBytes);
    }

    rig.write("SPRING2.INI", "0B10\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 2);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0B10\r\n");

    rig.write("SPRING2.INI", "0B2\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0B2\r\n");

    rig.write("SPRING2.INI", "0b12\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 1);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "0b12\r\n");

    for (const auto& [binaryBytes, expectedFlag] : std::array{
             std::pair{"0b1\r\n", 1}, std::pair{"0b10\r\n", 2},
             std::pair{"0B1010\t", 10},
             std::pair{"0b10000000000000000000000000000000", (-2147483647 - 1)},
             std::pair{"0B11111111111111111111111111111111\r\n", -1}}) {
        rig.write("SPRING2.INI", binaryBytes);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == expectedFlag);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == binaryBytes);
    }

    const std::string binaryOverflow = "0B1" + std::string(31, '0') + "1";
    rig.write("SPRING2.INI", binaryOverflow);
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 1);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == binaryOverflow);

    for (const std::string unsupportedOneRow : {
             "0190\r\n", "019 \r\n", " 019\r\n", "019\r\n ",
             "0180\r\n", "08x\r\n", "089x\r\n", "0B\r\n",
             " 010\r\n", "010x\r\n", "037777777777\r\n",
             "-4294967297x\r\n", "+4294967297x\r\n",
             "-010x\r\n", "+010x\r\n",
             "-18446744073709551617x\r\n",
             "0B3\r\n", "+0B10\r\n", "0b2\r\n",
             "0b13\r\n", "0b10x\r\n", "0b12x\r\n", "0x \r\n", "0XG\r\n", "0X \r\n",
             "0xA0G\r\n"}) {
        rig.write("SPRING2.INI", unsupportedOneRow);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == unsupportedOneRow);
    }

    rig.write("SPRING.INI", twoRows);
    rig.write("SPRING2.INI", "0B2\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 0);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == twoRows);
    CHECK(rig.read("SPRING2.INI") == "0B2\r\n");

    rig.write("SPRING2.INI", "018\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == 8);
    CHECK(rig.read("SPRING.INI") == twoRows);
    CHECK(rig.read("SPRING2.INI") == "018\r\n");

    for (const std::string broaderOctalRows : {
             "1 018\r\n", "018 1\r\n", "018 018\r\n", " 018\r\n", "018\r\n ",
             "019\r\n", "019 0\r\n", " 019\r\n", "019\r\n ",
             "010\r\n", "010 1\r\n"}) {
        rig.write("SPRING2.INI", broaderOctalRows);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == broaderOctalRows);
    }

    rig.write("SPRING2.INI", "09\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 9);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == twoRows);
    CHECK(rig.read("SPRING2.INI") == "09\r\n");

    for (const std::string broader09Rows : {"09 0\r\n", " 09\r\n", "09\r\n "}) {
        rig.write("SPRING2.INI", broader09Rows);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == broader09Rows);
    }

    rig.write("SPRING2.INI", "0b12\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == 2);
    CHECK(rig.read("SPRING.INI") == twoRows);
    CHECK(rig.read("SPRING2.INI") == "0b12\r\n");

    for (const std::string broaderBinaryRows : {
             "1 0b12\r\n", "0b12 1\r\n", "0b12 0b12\r\n", " 0b12\r\n", "0b12\r\n "}) {
        rig.write("SPRING2.INI", broaderBinaryRows);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == broaderBinaryRows);
    }

    rig.write("SPRING.INI", threeRows);
    rig.write("SPRING2.INI", "0b12\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "0b12\r\n");

    rig.write("SPRING.INI", twoRows);

    rig.write("SPRING2.INI", "0xG\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 0);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == twoRows);
    CHECK(rig.read("SPRING2.INI") == "0xG\r\n");

    for (const std::string broaderHexFailure : {
             "1 0xG\r\n", "0xG 1\r\n", "0xG 0xG\r\n", " 0xG\r\n", "0xG\r\n "}) {
        rig.write("SPRING2.INI", broaderHexFailure);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == broaderHexFailure);
    }

    for (const std::string broaderTwoRow : {
             "1 0B2\r\n", "0B2 1\r\n", "0B2 0B2\r\n", " 0B2\r\n", "0B2\r\n "}) {
        rig.write("SPRING2.INI", broaderTwoRow);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == broaderTwoRow);
    }

    rig.write("SPRING.INI", threeRows);
    rig.write("SPRING2.INI", "0B2\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "0B2\r\n");

    rig.write("SPRING2.INI", "0xG\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "0xG\r\n");

    rig.write("SPRING2.INI", "018\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "018\r\n");

    rig.write("SPRING2.INI", "09\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "09\r\n");

    rig.write("SPRING.INI", twoRows);
    // Keep the admitted one-row malformed-hex witness exact; retry behavior
    // and neighboring malformed or broader hexadecimal forms remain guarded.
    for (const std::string unsupportedToken : {
             "bad\r\n", "0x1\r\n", "-2147483650\r\n", "+2147483648\r\n",
             "+4294967297\r\n", "+18446744073709551617\r\n",
             "-4294967297\r\n", "-9223372036854775809\r\n",
             "-18446744073709551617\r\n", "-010\r\n", "+010\r\n",
             "0b10\r\n", "0B1010\t",
             "08\r\n", "089\r\n", "090", "04294967297\r\n",
             "4294967297,4\r\n", "0x", "0x\r\n", "0xH\r\n", "0XA1\r\n", "0X", "0X\r\n",
             "0B10\r\n",
             "0xG0\r\n", "0xA0\r\n", "0Xa\r\n",
             ",234\r\n", "1,,2\r\n"}) {
        rig.write("SPRING2.INI", unsupportedToken);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == originalSpring);
        CHECK(rig.read("SPRING2.INI") == unsupportedToken);
    }

    std::string nonAscii = "1 ";
    nonAscii.push_back(static_cast<char>(0x80));
    rig.write("SPRING2.INI", nonAscii);
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == nonAscii);

    for (const auto [width, bigEndian] : std::array{
             std::pair{1, false}, std::pair{2, false}, std::pair{2, true},
             std::pair{4, false}, std::pair{4, true}}) {
        const auto bomFlags = unicodeBomAscii("1\t-2\t\r\n", width, bigEndian);
        rig.write("SPRING2.INI", bomFlags);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 2);
        CHECK(rows[0].flag == 1);
        CHECK(rows[1].flag == -2);
        CHECK(rig.read("SPRING.INI") == originalSpring);
        CHECK(rig.read("SPRING2.INI") == bomFlags);
    }

    // A readable BOM-bearing companion that ends after one complete flag
    // leaves the pre-zeroed second row at zero.
    const auto utf16Short = unicodeBomAscii("1", 2, false);
    rig.write("SPRING2.INI", utf16Short);
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 2);
    CHECK(rows[0].flag == 1);
    CHECK(rows[1].flag == 0);
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == utf16Short);

    // Truncated code units and decoded non-ASCII scalars remain outside the
    // ASCII-only profile, with source files left untouched.
    const std::string truncatedUtf16 = std::string("\xff\xfe", 2) + "1";
    rig.write("SPRING2.INI", truncatedUtf16);
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == truncatedUtf16);

    const std::string utf16NonAscii = std::string("\xff\xfe", 2) +
        std::string("1\0\t\0\x80\0", 6);
    rig.write("SPRING2.INI", utf16NonAscii);
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == originalSpring);
    CHECK(rig.read("SPRING2.INI") == utf16NonAscii);
}

TEST_CASE(native_spring_reader_preserves_bounded_q_question_retry_cursor)
{
    Rig rig;
    const std::string oneRow = "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n";
    const std::string twoRows =
        "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n";
    std::vector<SpringRecord> rows;

    rig.write("SPRING.INI", oneRow);
    for (const std::string flags : {std::string("1q\r\n"), std::string("1?\r\n")}) {
        rig.write("SPRING2.INI", flags);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 1);
        CHECK(rows[0].flag == 1);
        CHECK(rig.read("SPRING.INI") == oneRow);
        CHECK(rig.read("SPRING2.INI") == flags);
    }

    rig.write("SPRING.INI", twoRows);
    for (const auto& [flags, first, second] : std::array{
             std::tuple{"1q\r\n", 1, 0}, std::tuple{"1?\r\n", 1, 0},
             std::tuple{"1q 2\r\n", 1, 0}, std::tuple{"1? 2\r\n", 1, 0},
             std::tuple{"1??\r\n", 1, 0}, std::tuple{"q 2\r\n", 0, 0}}) {
        rig.write("SPRING2.INI", flags);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 2);
        CHECK(rows[0].flag == first);
        CHECK(rows[1].flag == second);
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == flags);
    }

    for (const std::string flags : {std::string("1z\r\n"), std::string("1z?\r\n"),
                                    std::string("1x?\r\n")}) {
        rig.write("SPRING2.INI", flags);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == flags);
    }

    rig.write("SPRING.INI", oneRow);
    rig.write("SPRING2.INI", "q 2\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "q 2\r\n");

    const std::string threeRows =
        "3\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n"
        "50\r\n3.5\r\n4.5\r\n60\r\n0\r\n1\r\n0\r\n";
    rig.write("SPRING.INI", threeRows);
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "q 2\r\n");
}

TEST_CASE(native_spring_reader_consumes_recognized_ascii_sign_without_digits)
{
    Rig rig;
    const std::string oneRow = "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n";
    const std::string twoRows =
        "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n";
    std::vector<SpringRecord> rows;

    rig.write("SPRING.INI", oneRow);
    for (const std::string sign : {std::string("+"), std::string("-")}) {
        for (const std::string ending : {std::string{}, std::string("\r\n")}) {
            const auto flags = sign + ending;
            rig.write("SPRING2.INI", flags);
            REQUIRE(rig.io->readSprings(rows));
            REQUIRE(rows.size() == 1);
            CHECK(rows[0].flag == 0);
            CHECK(rig.read("SPRING.INI") == oneRow);
            CHECK(rig.read("SPRING2.INI") == flags);
        }
    }
    rig.write("SPRING2.INI", "++1\r\n");
    REQUIRE(rig.io->readSprings(rows));
    REQUIRE(rows.size() == 1);
    CHECK(rows[0].flag == 0);
    CHECK(rig.read("SPRING.INI") == oneRow);
    CHECK(rig.read("SPRING2.INI") == "++1\r\n");

    rig.write("SPRING.INI", twoRows);
    for (const char first : {'+', '-'}) {
        for (const char second : {'+', '-'}) {
            for (int value = 1; value <= 9; ++value) {
                for (const std::string ending : {std::string{}, std::string("\r\n")}) {
                    const auto flags = std::string{first, second} + std::to_string(value) + ending;
                    rig.write("SPRING2.INI", flags);
                    REQUIRE(rig.io->readSprings(rows));
                    REQUIRE(rows.size() == 2);
                    CHECK(rows[0].flag == 0);
                    CHECK(rows[1].flag == (second == '-' ? -value : value));
                    CHECK(rig.read("SPRING.INI") == twoRows);
                    CHECK(rig.read("SPRING2.INI") == flags);
                }
            }
        }
    }

    for (const std::string flags : {std::string("+++2\r\n"), std::string("---2\r\n")}) {
        rig.write("SPRING2.INI", flags);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 2);
        CHECK(rows[0].flag == 0);
        CHECK(rows[1].flag == 0);
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == flags);
    }

    const std::string threeRows =
        "3\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n"
        "30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n"
        "50\r\n3.5\r\n4.5\r\n60\r\n0\r\n1\r\n0\r\n";
    rig.write("SPRING.INI", threeRows);
    rig.write("SPRING2.INI", "++2\r\n");
    CHECK(unsupported([&] { rig.io->readSprings(rows); }));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING.INI") == threeRows);
    CHECK(rig.read("SPRING2.INI") == "++2\r\n");
    rig.write("SPRING.INI", twoRows);

    for (const std::string flags : {std::string("+\t2\r\n"), std::string("-\t2\r\n")}) {
        rig.write("SPRING2.INI", flags);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size() == 2);
        CHECK(rows[0].flag == 0);
        CHECK(rows[1].flag == 2);
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == flags);
    }

    // Comma has a distinct C-locale grouping transition and remains outside
    // the sign/no-digit admission. These are candidate refusal controls only.
    for (const std::string flags : {std::string("+,2\r\n"), std::string("-,2\r\n")}) {
        rig.write("SPRING2.INI", flags);
        CHECK(unsupported([&] { rig.io->readSprings(rows); }));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI") == twoRows);
        CHECK(rig.read("SPRING2.INI") == flags);
    }

    rig.write("SPRING.INI", oneRow);
    for (const auto [width, bigEndian] : std::array{
             std::pair{2, false}, std::pair{2, true}, std::pair{4, false}, std::pair{4, true}}) {
        for (const auto [sign, scalar] : std::array{
                 std::pair{std::string("minus"), 0x2212u},
                 std::pair{std::string("fullwidth-plus"), 0xff0bu},
                 std::pair{std::string("fullwidth-minus"), 0xff0du}}) {
            (void)sign;
            std::string flags;
            if (width == 2 && bigEndian) flags.assign("\xfe\xff", 2);
            else if (width == 2) flags.assign("\xff\xfe", 2);
            else if (bigEndian) flags.assign("\x00\x00\xfe\xff", 4);
            else flags.assign("\xff\xfe\x00\x00", 4);
            for (const auto codepoint : {scalar, 0x32u}) {
                for (int byte = 0; byte < width; ++byte) {
                    const auto shift = bigEndian ? 8 * (width - byte - 1) : 8 * byte;
                    flags.push_back(static_cast<char>((codepoint >> shift) & 0xff));
                }
            }
            rig.write("SPRING2.INI", flags);
            CHECK(unsupported([&] { rig.io->readSprings(rows); }));
            CHECK(rows.empty());
            CHECK(rig.read("SPRING.INI") == oneRow);
            CHECK(rig.read("SPRING2.INI") == flags);
        }
    }
}

TEST_CASE(native_spring_reader_accepts_bounded_qt_bmp_decimal_digits)
{
    Rig rig;
    const std::string primary = "1\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n";
    rig.write("SPRING.INI", primary);
    const auto encoded = [](std::initializer_list<std::uint32_t> units, int width, bool be) {
        std::string out;
        if(width==2 && be) out="\xfe\xff";
        else if(width==2) out="\xff\xfe";
        else if(be) out=std::string("\x00\x00\xfe\xff",4);
        else out=std::string("\xff\xfe\x00\x00",4);
        for(auto cp:units) for(int j=0;j<width;++j) {
            const int shift=8*(be ? width-j-1 : j);
            out.push_back(static_cast<char>((cp>>shift)&0xff));
        }
        return out;
    };
    std::vector<SpringRecord> rows;
    constexpr std::uint32_t starts[] = {0x0660,0x06f0,0x07c0,0x0966,0x09e6,0x0a66,0x0ae6,0x0b66,0x0be6,0x0c66,0x0ce6,0x0d66,0x0e50,0x0ed0,0x0f20,0x1040,0x17e0,0x1810,0x1946,0x19d0,0x1b50,0xff10};
    for(const auto [width,be] : std::array{std::pair{2,false},std::pair{2,true},std::pair{4,false},std::pair{4,true}}) {
        rig.write("SPRING.INI",primary);
        for(const auto base : starts) for(std::uint32_t digit=0;digit<10;++digit) {
            const auto bytes=encoded({base+digit},width,be);
            rig.write("SPRING2.INI",bytes);
            REQUIRE(rig.io->readSprings(rows));
            REQUIRE(rows.size()==1);
            CHECK(rows[0].flag==static_cast<int>(digit));
            CHECK(rig.read("SPRING.INI")==primary);
            CHECK(rig.read("SPRING2.INI")==bytes);
        }
        rig.write("SPRING.INI", "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n");
        for(const auto base : starts) for(std::uint32_t digit=0;digit<10;++digit) {
            const auto bytes=encoded({base+digit},width,be);
            rig.write("SPRING2.INI",bytes);
            REQUIRE(rig.io->readSprings(rows));
            REQUIRE(rows.size()==2);
            CHECK(rows[0].flag==static_cast<int>(digit));
            CHECK(rows[1].flag==0);
            CHECK(rig.read("SPRING.INI")=="2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n");
            CHECK(rig.read("SPRING2.INI")==bytes);
        }
    }
    for(const auto [width,be] : std::array{std::pair{2,false},std::pair{2,true},std::pair{4,false},std::pair{4,true}}) {
        rig.write("SPRING.INI",primary);
        const auto flags=encoded({0x0660,0x0661,0x0660},width,be);
        rig.write("SPRING2.INI",flags);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size()==1);
        CHECK(rows[0].flag==10);
        CHECK(rig.read("SPRING.INI")==primary);
        CHECK(rig.read("SPRING2.INI")==flags);

        const auto mixed=encoded({0x0661,0xff12},width,be);
        rig.write("SPRING2.INI",mixed);
        REQUIRE(rig.io->readSprings(rows));
        CHECK(rows[0].flag==12);

        // Two-row extraction is exercised with a matching supported primary.
        rig.write("SPRING.INI", "2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n");
        rig.write("SPRING2.INI", encoded({0x0661,0x0020,'-',0x0968},width,be));
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size()==2);
        CHECK(rows[0].flag==1);
        CHECK(rows[1].flag==-2);
        CHECK(rig.read("SPRING2.INI")==encoded({0x0661,0x0020,'-',0x0968},width,be));
        const auto asciiOctal=encoded({'0','1','0'},width,be);
        rig.write("SPRING.INI",primary);
        rig.write("SPRING2.INI",asciiOctal);
        REQUIRE(rig.io->readSprings(rows));
        REQUIRE(rows.size()==1);
        CHECK(rows[0].flag==8);
    }
    // Representative category boundary negatives: superscript two has a
    // digit value in Qt but is not in the recovered isDigit category.
    for(const auto [width,be] : std::array{std::pair{2,false},std::pair{2,true},std::pair{4,false},std::pair{4,true}}) {
        for(const auto cp : {0x00b2u,0x2160u,0x2212u,0xd800u,0x1d7d9u}) {
            rig.write("SPRING.INI",primary);
            auto bad=encoded({cp},width,be);
            rig.write("SPRING2.INI",bad);
            CHECK(unsupported([&]{rig.io->readSprings(rows);}));
            CHECK(rows.empty());
            CHECK(rig.read("SPRING.INI")==primary);
            CHECK(rig.read("SPRING2.INI")==bad);
        }
    }
    rig.write("SPRING.INI",primary);
    rig.write("SPRING2.INI",encoded({0x0661,'q'},2,false));
    CHECK(unsupported([&]{rig.io->readSprings(rows);}));
    CHECK(rows.empty());
    for(const auto excluded : {encoded({'0',0x0661},2,false), encoded({0x0661,0x00a0,0x0662},2,false)}) {
        rig.write("SPRING2.INI",excluded);
        CHECK(unsupported([&]{rig.io->readSprings(rows);}));
        CHECK(rows.empty());
        CHECK(rig.read("SPRING.INI")==primary);
        CHECK(rig.read("SPRING2.INI")==excluded);
    }
    rig.write("SPRING.INI", "3\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n50\r\n3.5\r\n4.5\r\n60\r\n0\r\n1\r\n0\r\n");
    const auto countThreeFlags=encoded({0x0661},2,false);
    rig.write("SPRING2.INI",countThreeFlags);
    CHECK(unsupported([&]{rig.io->readSprings(rows);}));
    CHECK(rows.empty());
    CHECK(rig.read("SPRING2.INI")==countThreeFlags);
}

int main() { return testkit::run_all(); }
