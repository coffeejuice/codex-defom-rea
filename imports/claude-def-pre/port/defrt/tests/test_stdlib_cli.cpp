// Tier-1: the command-line helpers, reference count and CRT exit emulation in defrt/stdlib.
// ctest-labels: p1-01 cli
#include <defrt/stdlib/cli_helpers.hpp>
#include <defrt/stdlib/crt_emulation.hpp>
#include <defrt/stdlib/refcount.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

namespace {

std::string read_all(std::FILE* f)
{
    std::rewind(f);
    std::string s;
    char buf[512];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof buf, f)) > 0) {
        s.append(buf, n);
    }
    return s;
}

struct ExitSeen {
    std::uint32_t status;
};
void throwing_hook(std::uint32_t status) { throw ExitSeen{status}; }

std::string stripped(const char* in)
{
    char buf[2048];
    std::strcpy(buf, in);
    def::rt::strip_quotes(buf);
    return buf;
}

// One 0x28-byte directory entry: +0x04 step, +0x14 operation, +0x1c dimension, +0x24 stored.
void put(std::vector<unsigned char>& recs, int i, std::int32_t step, std::int32_t op,
         std::int32_t dim, std::int32_t stored)
{
    unsigned char* r = recs.data() + i * 0x28;
    std::memcpy(r + 0x04, &step, 4);
    std::memcpy(r + 0x14, &op, 4);
    std::memcpy(r + 0x1c, &dim, 4);
    std::memcpy(r + 0x24, &stored, 4);
}

} // namespace

TEST_CASE(help_screen_text)
{
    // Strings of FUN_14001b4e0, in call order (DEF_PRE_64.exe .rdata).
    const std::string expected =
        "\n"
        "*** DEF_PRE.EXE - help ***\n"
        "\n"
        "\tUsage\n"
        "\t  o. Interactive mode\n"
        "\t        DEF_PRE.EXE\n"
        "\t        - OR -  \n"
        "\t  o. Batch mode\n"
        "\t        DEF_PRE.EXE [-h]\n"
        "\t                    [-m {header|stepList|lastStepNo|simStepList}]\n"
        "\t                    [-d <db name>]\n"
        "\t                    [-o <file name>]\n"
        "\n"
        "\tExample\n"
        "\t  o. This help screen\n"
        "\t        DEF_PRE.EXE -h\n"
        "\t  o. Get header information\n"
        "\t        DEF_PRE.EXE -m header -d a.db -o header.txt\n"
        "\t  o. Get step number list\n"
        "\t        DEF_PRE.EXE -m stepList -d a.db -o stepList.txt\n"
        "\t  o. Get the last step number\n"
        "\t        DEF_PRE.EXE -m lastStepNo -d a.db -o lastStepNo.txt\n"
        "\t  o. Get the simulation step number list\n"
        "\t        DEF_PRE.EXE -m simStepList -d a.db -o simStepList.txt\n"
        "\n";
    std::FILE* f = std::tmpfile();
    REQUIRE(f != nullptr);
    def::rt::print_help(f);
    CHECK_EQ(read_all(f), expected);
    std::fclose(f);
}

TEST_CASE(strip_quotes_cases)
{
    CHECK_EQ(stripped("\"a.db\""), std::string("a.dbb")); // terminated at len - 1, not len - 2
    CHECK_EQ(stripped("\"a.db"), std::string("a.dbb"));
    CHECK_EQ(stripped("a.db\""), std::string("a.db"));
    CHECK_EQ(stripped("a.db"), std::string("a.db"));
    CHECK_EQ(stripped("\""), std::string("")); // end 0 < start 1: nothing copied, s[0] = 0
}

TEST_CASE(strip_quotes_long_value_ends_like_the_cookie_failure)
{
    def::rt::set_abnormal_exit_hook(throwing_hook);
    std::string big(1024, 'x');
    std::uint32_t seen = 0;
    try {
        std::vector<char> buf(big.begin(), big.end());
        buf.push_back('\0');
        def::rt::strip_quotes(buf.data());
    } catch (const ExitSeen& e) {
        seen = e.status;
    }
    CHECK_EQ(seen, def::rt::kStatusStackBufferOverrun);
    // 1023 characters still fit
    std::vector<char> ok(1023, 'y');
    ok.push_back('\0');
    def::rt::strip_quotes(ok.data());
    CHECK_EQ(std::strlen(ok.data()), std::size_t{1023});
    def::rt::set_abnormal_exit_hook(nullptr);
}

TEST_CASE(split_tokens_with_strtok)
{
    char s[] = "C:/PROBLEM//TEST_REA/a.DB";
    char* table[8] = {};
    std::int32_t n = -1;
    def::rt::split_tokens(s, "/", table, &n);
    REQUIRE(n == 4);
    CHECK_EQ(std::string(table[0]), std::string("C:"));
    CHECK_EQ(std::string(table[1]), std::string("PROBLEM"));
    CHECK_EQ(std::string(table[2]), std::string("TEST_REA"));
    CHECK_EQ(std::string(table[3]), std::string("a.DB"));
    CHECK(table[4] == nullptr);
}

TEST_CASE(operation_steps_first_and_last)
{
    std::vector<unsigned char> recs(6 * 0x28, 0);
    put(recs, 0, 0, 1, 2, 1);
    put(recs, 1, 10, 1, 2, 1);
    put(recs, 2, 20, 1, 2, 0); // not stored
    put(recs, 3, 30, 2, 3, 1);
    put(recs, 4, 40, 1, 2, 1);
    put(recs, 5, 50, 2, 3, 1);
    const std::int32_t nrec = 6;
    std::int32_t dim = -1, b = -1, c = -1;
    std::int32_t op = 1;
    CHECK(def::rt::operation_steps(&op, recs.data(), &nrec, &dim, &b, &c));
    CHECK_EQ(dim, 2);
    CHECK_EQ(b, 0);
    CHECK_EQ(c, 40);
    op = 2;
    CHECK(def::rt::operation_steps(&op, recs.data(), &nrec, &dim, &b, &c));
    CHECK_EQ(dim, 3);
    CHECK_EQ(b, 30);
    CHECK_EQ(c, 50);
    op = 3; // no entry: nothing written
    CHECK(!def::rt::operation_steps(&op, recs.data(), &nrec, &dim, &b, &c));
    CHECK_EQ(b, 30);
    put(recs, 0, 7, 4, 2, 1); // a single stored entry leaves the last step unchanged
    op = 4;
    c = 99;
    CHECK(def::rt::operation_steps(&op, recs.data(), &nrec, &dim, &b, &c));
    CHECK_EQ(b, 7);
    CHECK_EQ(c, 99);
}

TEST_CASE(reference_count_and_dimension)
{
    std::int32_t refs = 42;
    def::rt::ref_init(&refs);
    CHECK_EQ(refs, 1);
    refs = 2;
    CHECK(!def::rt::ref_release(&refs));
    CHECK(def::rt::ref_release(&refs));
    CHECK_EQ(refs, 0);

    unsigned char obj[0x20] = {};
    std::int32_t two = 2, three = 3;
    std::memcpy(obj + 0x1c, &two, 4);
    CHECK(def::rt::dimension_is_2(obj));
    CHECK(!def::rt::dimension_is_3(obj));
    std::memcpy(obj + 0x1c, &three, 4);
    CHECK(def::rt::dimension_is_3(obj));
}

int main() { return testkit::run_all(); }
