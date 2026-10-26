// P17: unit table 0x143d76368 and the evaluator 0x1402ede00 (spec: docs/re/moproj2mst/p17/spec.md 1).
// Cases that reach a P04 seam (QString::split / toDouble) are skipped while P04 is a stub.
#include <cstdio>

#include <testkit/check.hpp>

#include "m2m/p17.hpp"

using m2m::qt::String;
namespace u = m2m::units;

static String S(const char* s) { return String::latin1(s); }

// Runs `body`; a NotPorted from another packet's stub skips the rest of the case.
template <class F>
static void needs(const char* what, F body)
{
    try {
        body();
    } catch (const m2m::NotPorted& e) {
        std::printf("  skipped (%s): %s\n", what, e.what());
    }
}

TEST_CASE(unit_names)
{
    CHECK_EQ(std::string(u::unitName(0, 1)), std::string("sec"));
    CHECK_EQ(std::string(u::unitName(1, 1)), std::string("mm"));
    CHECK_EQ(std::string(u::unitName(1, 2)), std::string("in"));
    CHECK_EQ(std::string(u::unitName(1, 0)), std::string("in"));        // only 1 is SI
    CHECK_EQ(std::string(u::unitName(9, 1)), std::string("C"));
    CHECK_EQ(std::string(u::unitName(9, 2)), std::string("F"));
    CHECK_EQ(std::string(u::unitName(0x13, 1)), std::string("J/sec/mm^2/C^4)"));   // sic
    CHECK_EQ(std::string(u::unitName(0x20, 1)), std::string("N/sec/mm/C"));
    CHECK_EQ(std::string(u::unitName(0x20, 2)), std::string("Btu/sec/in^2/F"));
    CHECK_EQ(std::string(u::unitName(0x2b, 1)), std::string(""));
}

TEST_CASE(unit_index_is_case_insensitive)
{
    CHECK_EQ(u::unitIndex(1, S("MM")), 1);
    CHECK_EQ(u::unitIndex(1, S("yd")), 6);
    CHECK_EQ(u::unitIndex(0xd, S("rad")), 1);         // table text "Rad"
    CHECK_EQ(u::unitIndex(9, String(u"K")), 2);  // KELVIN SIGN folds to 'k'
    CHECK_EQ(u::unitIndex(1, S("furlong")), -1);
    CHECK_EQ(u::unitIndex(0x2b, S("")), -1);           // not a category
    CHECK_EQ(u::unitIndex(1, S("")), -1);
    CHECK_EQ(u::unitIndex(0xb, S("")), 0);             // "No Units": unit text ""
    CHECK_EQ(u::unitIndex(0x1f, String()), 0);         // null == "" in QString::compare
}

TEST_CASE(convert_factors)
{
    double out = -1;
    CHECK(u::convert(0, S("min"), 2.0, S("sec"), &out));
    CHECK_BITS(out, 120.0);
    CHECK(u::convert(0, S("sec"), 3.0, S("msec"), &out));
    CHECK_BITS(out, 3000.0);
    CHECK(u::convert(9, S("C"), 100.0, S("F"), &out));   // v * 1.8 + 32
    CHECK_BITS(out, 100.0 * 0x1.ccccccccccccdp+0 + 32.0);
    CHECK(u::convert(9, S("F"), 212.0, S("C"), &out));
    CHECK_BITS(out, 212.0 * 0x1.1c71c71c71c72p-1 + -0x1.1c71c71c71c72p+4);
    CHECK(u::convert(9, S("C"), 0.0, S("K"), &out));
    CHECK_BITS(out, 0x1.1126666666666p+8);            // 273.15
    out = -1;
    CHECK(!u::convert(1, S("mm"), 1.0, S("furlong"), &out));
    CHECK_BITS(out, -1.0);                             // untouched on failure
    CHECK(!u::convert(0x2b, S(""), 5.0, S(""), &out));
    CHECK_BITS(out, 5.0);                              // plain number: copied, result false
}

TEST_CASE(to_system)
{
    double out = 0;
    CHECK(u::toSystem(1, 2, S("mm"), 25.4, &out));     // mm -> in
    CHECK_BITS(out, 25.4 * 0x1.42850a142850ap-5);    // table entry mm -> in
    CHECK(u::toSystem(0x2b, 1, S(""), 7.5, &out));     // empty system unit, kind 0x2b: true
    CHECK_BITS(out, 7.5);
    CHECK(!u::toSystem(0x13, 1, S("J/sec/mm^2/C^4"), 1.0, &out));   // SI name has a stray ')'
}

TEST_CASE(evaluator)
{
    needs("P04 QString::split/toDouble", [] {
        double out = -1;
        String unit;
        CHECK(u::evalUnit(0, 1, S("2 min"), &out, &unit));
        CHECK_BITS(out, 120.0);
        CHECK(unit == S("sec"));
        // several spaces: empty parts removed
        CHECK(u::evalUnit(9, 2, S("  100   C "), &out, &unit));
        CHECK_BITS(out, 100.0 * 0x1.ccccccccccccdp+0 + 32.0);
        CHECK(unit == S("F"));
        // no unit text: the unit is "" and the conversion fails, the number stays
        unit = String();
        CHECK(!u::evalUnit(0, 1, S("3.5"), &out, &unit));
        CHECK_BITS(out, 3.5);
        CHECK(unit == S("sec"));
        // unit match before conversion is case-sensitive
        unit = String();
        CHECK(!u::evalUnit(0, 1, S("2 MIN"), &out, &unit));
        CHECK_BITS(out, 0.0);
        CHECK(unit.isNull());                          // untouched when the parse fails
        // kind 0x2b: the whole text is the number
        CHECK(u::evalUnit(0x2b, 1, S("1e-3"), &out, &unit));
        CHECK_BITS(out, 1e-3);
        bool ok = false;
        CHECK_BITS(m2m::evalUnitString(S("1 m"), 1, 1, &ok), 1000.0);
        CHECK(ok);
    });
}

TEST_CASE(startup_unit_is_the_bss_zero)
{
    CHECK_EQ(m2m::guiStartupUnit(), 0);
}

int main() { return testkit::run_all(); }
