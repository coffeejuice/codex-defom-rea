// P06: KEY token reader 0x1409068f0 and the text-mode file.
#include <testkit/check.hpp>

#include "m2m/datafiles.hpp"

using m2m::datafiles::TextFile;
using m2m::datafiles::TokenReader;

TEST_CASE(text_mode_crlf_and_ctrl_z)
{
    TextFile f(std::string("a\r\nb\rc\n\x1a" "after"));
    CHECK(f.data() == std::string("a\nb\rc\n"));
    std::string l;
    CHECK(f.fgets(l, 100));
    CHECK(l == "a\n");
    CHECK(f.fgets(l, 3));
    CHECK(l == "b\r");
    CHECK(f.fgets(l, 100));
    CHECK(l == "c\n");
    CHECK(!f.fgets(l, 100));
    CHECK(l == "c\n");
}

TEST_CASE(bwz_style_keywords)
{
    TextFile f(std::string("WZD_MACHDIST_WORKPIECE_OBJECT_NO\n  * OBJECT # FOR WORKPIECE \n3\n\n"
                           "wzd_boolean_remesh // comment\n  * c\n1\n\nDONE\n\n"));
    TokenReader r;
    std::string w;
    int v = 0;
    std::uint8_t b = 0;
    CHECK_EQ(r.read(f, TokenReader::Upper, &w, TokenReader::kNewLine), 1);
    CHECK(w == "WZD_MACHDIST_WORKPIECE_OBJECT_NO");
    CHECK_EQ(r.read(f, TokenReader::Int, &v), 0);        // the '*' line is skipped
    CHECK_EQ(v, 3);
    CHECK_EQ(r.read(f, TokenReader::Upper, &w, TokenReader::kNewLine), 1);
    CHECK(w == "WZD_BOOLEAN_REMESH");
    CHECK_EQ(r.read(f, TokenReader::Bool, &b), 0);
    CHECK_EQ(int(b), 1);
    CHECK_EQ(r.read(f, TokenReader::Upper, &w, TokenReader::kNewLine), 1);
    CHECK(w == "DONE");
    CHECK_EQ(r.read(f, TokenReader::Upper, &w, TokenReader::kNewLine), -2);
    CHECK(w == "DONE");
}

TEST_CASE(numbers_and_positions)
{
    TextFile f(std::string("KEY 1.5D2, 7 = 2.5\nNEXT\n"));
    TokenReader r;
    std::string w;
    double d = 0;
    float fl = 0;
    int i = 0;
    CHECK_EQ(r.read(f, TokenReader::Upper, &w, TokenReader::kNewLine), 4);
    CHECK_EQ(r.read(f, TokenReader::Double, &d), 0);
    CHECK(d == 150.0);
    CHECK_EQ(r.read(f, TokenReader::Int, &i), 0);
    CHECK_EQ(i, 7);
    CHECK_EQ(r.read(f, TokenReader::Float, &fl), 0);
    CHECK(fl == 2.5f);
    CHECK_EQ(r.read(f, TokenReader::Int, &i, 3), 0);     // token 3 of the line again
    CHECK_EQ(i, 7);
    CHECK_EQ(r.read(f, TokenReader::Int, &i, 5), -1);
    CHECK_EQ(r.read(f, TokenReader::Text, &w), 0);        // used up: next line
    CHECK(w == "NEXT");
}

int main()
{
    return testkit::run_all();
}
