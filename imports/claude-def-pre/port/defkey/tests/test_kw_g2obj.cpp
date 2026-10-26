// ctest-labels: kw-g2obj group2
// Tier-1 tests of the group 2 shells (parser switch 14038b8a0, object writer 14029e250) and the
// object-header, movement and control keywords of packet kw-g2obj. Expected text is worked out by
// hand from the decompiled format strings ("%-6s %7i %20.10E": the name padded to 6, the number
// right-aligned in 7, reals in 20 columns with the old CRT's three-digit exponent) and from the
// stores in the specs (docs/re/keywords/<KW>.md).
#include "key_test_util.hpp"

#include <bit>
#include <cstdint>
#include <cstring>
#include <new>
#include <string>

#include <defkey/kw_g2obj.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <testkit/check.hpp>

using namespace def::key::g2obj;
using def::model::DB_STEP_DATA;
using def::model::DYNARY;
using def::model::LLOBJ_DATA;
using keytest::InFile;
using keytest::OutFile;

namespace {

template <class T, class C>
T& at(C& base, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<unsigned char*>(&base) + off);
}

// Zero-filled model storage with the fields the group 2 code reads (the DB_STEP_DATA ctor is
// p1-03's; nothing here calls a virtual).
struct Model {
    void* raw;
    explicit Model(int dim)
    {
        raw = ::operator new(sizeof(DB_STEP_DATA));
        std::memset(raw, 0, sizeof(DB_STEP_DATA));
        at<std::int32_t>(m(), 0x1c) = dim;
    }
    ~Model() { ::operator delete(raw); }
    DB_STEP_DATA& m() { return *static_cast<DB_STEP_DATA*>(raw); }
};

// Reads `text` with the group 2 parser as the KEY loop would: the keyword token is consumed by
// the loop, so the parser starts after it.
int read(Model& md, int id, const std::string& text)
{
    InFile in("kw_g2obj_in.tmp", text);
    return read_object_keyword(md.m(), in.get(), id);
}

std::string key_rest(Model& md, int id, const std::string& text, int* status)
{
    InFile in("kw_g2obj_in.tmp", text);
    *status = read_object_keyword(md.m(), in.get(), id);
    return in.rest();
}

constexpr int kObjnam = 144, kObjtyp = 145, kAvgstr = 146, kZstr = 148, kPenvol = 149,
              kMovctl = 151, kStroke = 153, kSpdlmt = 155, kAngmov = 156, kObjphs = 204,
              kRefpos = 215, kVolcrg = 236, kOtprng = 240, kTrgvol = 259, kPrsnam = 257,
              kCsfreq = 235;

} // namespace

TEST_CASE(prologue_object_numbers)
{
    Model md(2);
    CHECK_EQ(read(md, kAvgstr, " 0 1.0\n"), 2);  // object number < 1
    CHECK_EQ(read(md, kAvgstr, ""), 2);         // EOF before the object number
    CHECK_EQ(read(md, kAvgstr, " 3 2.5\n"), 0);
    LLOBJ_DATA* o1 = find_object(md.m(), 1);
    LLOBJ_DATA* o3 = find_object(md.m(), 3);
    REQUIRE(o1 != nullptr && o3 != nullptr);
    CHECK_EQ(at<std::int32_t>(*o1, 0xc), 7);  // placeholder: type 7 (empty)
    CHECK_EQ(at<std::int32_t>(md.m(), 0x1378), 3);
    CHECK_EQ(at<double>(*o3, 0x1ea8), 2.5);
    // OBJPHS has no case: default -> 2 after the object number
    int st = 0;
    std::string rest = key_rest(md, kObjphs, " 1 7\n", &st);
    CHECK_EQ(st, 2);
    CHECK_EQ(rest, std::string(" 7\n"));
}

TEST_CASE(objtyp_codes_and_writer)
{
    Model md(3);
    CHECK_EQ(read(md, kObjtyp, " 1 3 5\n"), 0);
    LLOBJ_DATA* o = find_object(md.m(), 1);
    CHECK_EQ(at<std::int32_t>(*o, 0xc), 2);
    CHECK_EQ(at<std::int32_t>(*o, 0x2154), 5);
    CHECK_EQ(read(md, kObjtyp, " 1 6 9\n"), 0);  // code 6: type unchanged, aux stored
    CHECK_EQ(at<std::int32_t>(*o, 0xc), 2);
    CHECK_EQ(at<std::int32_t>(*o, 0x2154), 9);
    CHECK_EQ(read(md, kObjtyp, " 1\n"), 2);  // blank rest of line
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    CHECK_EQ(write_objtyp(md.m(), out.get(), *o, 1), 0);
    CHECK_EQ(out.text(), std::string("OBJTYP       1       3       9\n"));
}

TEST_CASE(empty_object_block)
{
    Model md(2);
    CHECK_EQ(read(md, kObjtyp, " 2 0 4\n"), 0);  // object 1 becomes a placeholder, 2 empty
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    CHECK_EQ(write_object(md.m(), out.get(), find_object(md.m(), 2), 0, false), 0);
    CHECK_EQ(out.text(), std::string("*\n*  Data for Object #     2\n*\nOBJTYP       2       0       4\n"));
    CHECK_EQ(at<std::int32_t>(*find_object(md.m(), 2), 0x3f24), 0);
}

TEST_CASE(objnam_and_prsnam)
{
    Model md(2);
    CHECK_EQ(read(md, kObjnam, " 1 ignored\n  Die top  \n"), 0);
    LLOBJ_DATA* o = find_object(md.m(), 1);
    CHECK_EQ(std::string(&at<char>(*o, 0x828)), std::string("  Die top"));
    CHECK_EQ(read(md, kPrsnam, " 1\nPress A\n"), 0);
    CHECK_EQ(std::string(&at<char>(*o, 0x2160)), std::string("Press A"));
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    write_objnam(md.m(), out.get(), o, 4);
    CHECK_EQ(out.text(), std::string("OBJNAM       4\n  Die top\n"));
}

TEST_CASE(scalars_and_renumbering)
{
    Model md(2);
    CHECK_EQ(read(md, kAvgstr, " 1 5.0E-3\n"), 0);
    CHECK_EQ(read(md, kZstr, " 1 1, 2.5\n"), 0);  // commas are separators
    CHECK_EQ(read(md, kPenvol, " 1 1.0E6 1.9\n"), 0);
    LLOBJ_DATA* o = find_object(md.m(), 1);
    CHECK_EQ(at<std::int32_t>(*o, 0x1f30), 1);
    CHECK_EQ(at<double>(*o, 0x1f28), 2.5);
    CHECK_EQ(at<std::int32_t>(*o, 0x1f20), 1);  // (int)1.9
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    KeyRef ref{1};
    at<std::int32_t>(*o, 0x3f24) = 12;  // renumbered
    write_avgstr(out.get(), md.m(), ref);
    write_zstr(out.get(), md.m(), ref);
    write_penvol(out.get(), md.m(), ref);
    CHECK_EQ(out.text(), std::string("AVGSTR      12    5.0000000000E-003\n"
                                     "ZSTR        12       1    2.5000000000E+000\n"
                                     "PENVOL      12    1.0000000000E+006       1\n"));
    // ZSTR in 3D reads nothing: the rest of the line stays for the KEY loop
    Model m3(3);
    int st = 0;
    CHECK_EQ(key_rest(m3, kZstr, " 1 0 0.0\n", &st), std::string(" 0 0.0\n"));
    CHECK_EQ(st, 0);
}

TEST_CASE(movctl_2d_speed_table)
{
    Model md(2);
    // type 1, sub 2 (table over var 1), angle -90, 2 points
    CHECK_EQ(read(md, kMovctl, " 1 1 2 -90.0 2\n 0.0 1.0\n 1.0 2.0\nNEXT"), 0);
    LLOBJ_DATA* o = find_object(md.m(), 1);
    CHECK_EQ(at<std::int32_t>(*o, 0x21c8), 1);
    CHECK_EQ(at<double>(*o, 0x21b0), 0.0);  // cos(-90 deg) = 6.1e-17 snapped to 0
    CHECK_EQ(at<double>(*o, 0x21b8), -1.0);
    DYNARY& t = at<DYNARY>(*o, 0x2248);
    CHECK_EQ(t.ndim, 1);
    CHECK_EQ(t.var[0], 1);
    CHECK_EQ(t.n[0], 2);
    CHECK_EQ(t.axis[0][1], 1.0);
    CHECK_EQ(t.values[1], 2.0);
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    CHECK_EQ(write_movctl(out.get(), md.m(), KeyRef{1}), 0);
    CHECK_EQ(out.text(), std::string("MOVCTL       1       1       2   -9.0000000000E+001       2\n"
                                     "    0.0000000000E+000    1.0000000000E+000\n"
                                     "    1.0000000000E+000    2.0000000000E+000\n"));
    // SPDLMT with a table turns a speed movement into type 8
    CHECK_EQ(read(md, kSpdlmt, " 1 1 0.5 3.0\n 0.0 4.0\n"), 0);
    CHECK_EQ(at<std::int32_t>(*o, 0x21c8), 8);
    CHECK_EQ(at<std::uint8_t>(*o, 0x2a18), 1);
}

// 2D direction cosine through the binary's CRT cos (140bb3490): the oracle OUT.DB of
// CREEP_Example1_SI (MOVCTL ... -9.0000003E+001) and Resistance_heat1_SI (-9.0000010000E+001)
// hold these LLBND words; <cmath> cos gives ...CECB / ...B392 (1 ulp off).
TEST_CASE(movctl_2d_direction_crt_cos)
{
    Model md(2);
    CHECK_EQ(read(md, kMovctl, " 1 1 0 -9.0000003E+001  0.0000000E+000\n"), 0);
    LLOBJ_DATA* o = find_object(md.m(), 1);
    CHECK_EQ(std::bit_cast<std::uint64_t>(at<double>(*o, 0x21b0)), 0xBE6C1C496B72CECAULL);
    CHECK_EQ(read(md, kMovctl, " 1 1 0   -9.0000010000E+001    0.0000000000E+000\n"), 0);
    CHECK_EQ(std::bit_cast<std::uint64_t>(at<double>(*o, 0x21b0)), 0xBE876CE7D85CB393ULL);
}

TEST_CASE(movctl_3d_forms)
{
    Model md(3);
    // force constant, sub 0
    CHECK_EQ(read(md, kMovctl, " 1 0 0 0.0 0.0 -1.0 250.0\n"), 0);
    // speed, 1000-form
    CHECK_EQ(read(md, kMovctl, " 2 1 1002 0.0 0.0 -1.0 3.5\n"), 0);
    // speed, negative sub
    CHECK_EQ(read(md, kMovctl, " 3 1 -4 0.0 1.0 0.0 9.0\n"), 0);
    // mechanical press, no profile
    CHECK_EQ(read(md, kMovctl, " 4 4 0 0 -1\n 270.0 1.4 1500.0\n"), 0);
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    for (int i = 1; i <= 4; ++i) CHECK_EQ(write_movctl(out.get(), md.m(), KeyRef{i}), 0);
    CHECK_EQ(out.text(),
             std::string("MOVCTL       1       0       0    0.0000000000E+000    0.0000000000E+000"
                         "   -1.0000000000E+000    2.5000000000E+002\n"
                         "MOVCTL       2       1    1002    0.0000000000E+000    0.0000000000E+000"
                         "   -1.0000000000E+000    3.5000000000E+000\n"
                         "MOVCTL       3       1      -4    0.0000000000E+000    1.0000000000E+000"
                         "    0.0000000000E+000\n"
                         "MOVCTL       4       4    0.0000000000E+000    0.0000000000E+000"
                         "   -1.0000000000E+000\n"
                         "    2.7000000000E+002    1.4000000000E+000    1.5000000000E+003       0\n"));
    // unknown table shape -> 3
    LLOBJ_DATA* o = find_object(md.m(), 1);
    at<DYNARY>(*o, 0x21d0).ndim = 2;
    OutFile out2("kw_g2obj_out2.tmp");
    CHECK_EQ(write_movctl(out2.get(), md.m(), KeyRef{1}), 3);
}

TEST_CASE(stroke_angmov)
{
    Model md(2);
    CHECK_EQ(read(md, kStroke, " 1 0.5 -2.0\n"), 0);
    CHECK_EQ(read(md, kAngmov, " 1 1 0 6.0\n"), 0);  // method 1, constant
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    write_stroke(out.get(), md.m(), KeyRef{1});
    write_angmov(out.get(), md.m(), KeyRef{1});
    CHECK_EQ(out.text(), std::string("STROKE       1    5.0000000000E-001   -2.0000000000E+000\n"
                                     "ANGMOV       1       1       0    6.0000000000E+000\n"));
}

TEST_CASE(control_scalars)
{
    Model md(3);
    CHECK_EQ(read(md, kTrgvol, " 1 4 0.0\n"), 0);  // volume 0.0 -> mode -1
    LLOBJ_DATA* o = find_object(md.m(), 1);
    CHECK_EQ(at<std::int32_t>(*o, 0x2098), -1);
    CHECK_EQ(read(md, kOtprng, " 1 -2 1.5 2.5\n"), 0);  // negative mode -> 3 and |mode|
    CHECK_EQ(at<std::int32_t>(*o, 0x1eec), 3);
    CHECK_EQ(at<std::int32_t>(*o, 0x1ef0), 2);
    CHECK_EQ(at<std::int32_t>(*o, 0x1f08), 0);  // fewer than 4 tokens
    CHECK_EQ(read(md, kRefpos, " 1 2 7.9\n"), 0);
    CHECK_EQ(at<std::int32_t>(*o, 0x1ee8), 7);
    CHECK_EQ(read(md, kVolcrg, " 1 25 1.0\n"), 3);  // class beyond 21: Data Error
    CHECK_EQ(read(md, kVolcrg, " 1 11 2\n 0.0 1.0\n 1.0 3.0\n"), 0);
    CHECK_EQ(read(md, kCsfreq, " 1 0 5.0E4\n"), 0);
    keytest::legacy_printf();
    OutFile out("kw_g2obj_out.tmp");
    write_otprng(out.get(), md.m(), KeyRef{1});
    write_volcrg(out.get(), md.m(), KeyRef{1});
    write_csfreq(out.get(), md.m(), KeyRef{1});
    CHECK_EQ(out.text(), std::string("OTPRNG       1      -2    1.5000000000E+000    2.5000000000E+000       0\n"
                                     "VOLCRG       1      11       2\n"
                                     "    0.0000000000E+000    1.0000000000E+000\n"
                                     "    1.0000000000E+000    3.0000000000E+000\n"
                                     "CSFREQ       1       0    5.0000000000E+004\n"));
}

int main() { return testkit::run_all(); }
