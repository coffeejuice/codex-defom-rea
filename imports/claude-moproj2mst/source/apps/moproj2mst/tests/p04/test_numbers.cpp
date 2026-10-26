// P04: Qt 4.8.4 number text (QTextStream << int / << double, QString::number / arg / toInt /
// toDouble). Expected strings follow the code read from QtCore4_64.dll 4.8.4 (qdtoa 0x67055fd0,
// doubleToString 0x6704b550, decimalForm 0x670547a0, exponentForm 0x670548f0).
#include <testkit/check.hpp>

#include <cfloat>
#include <climits>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>

#include "m2m/qt4.hpp"
#include "m2m/qt4num.hpp"

using namespace m2m::qt;

static std::string fd(double v) { return formatDouble(v); }

static double from_bits(std::uint64_t u)
{
    double d;
    std::memcpy(&d, &u, 8);
    return d;
}

TEST_CASE(double_basic)
{
    CHECK_EQ(fd(0.0), std::string("0"));
    CHECK_EQ(fd(-0.0), std::string("0"));           // isZero: no sign for -0.0
    CHECK_EQ(fd(1.0), std::string("1"));
    CHECK_EQ(fd(-1.0), std::string("-1"));
    CHECK_EQ(fd(0.1), std::string("0.1"));
    CHECK_EQ(fd(2.46), std::string("2.46"));
    CHECK_EQ(fd(315.8), std::string("315.8"));
    CHECK_EQ(fd(55.55), std::string("55.55"));
    CHECK_EQ(fd(static_cast<double>(55.55f)), std::string("55.55"));
    CHECK_EQ(fd(static_cast<double>(0.1f)), std::string("0.1"));
    CHECK_EQ(fd(1.0 / 3.0), std::string("0.333333"));
    CHECK_EQ(fd(2.0 / 3.0), std::string("0.666667"));
    CHECK_EQ(fd(-2.5), std::string("-2.5"));
    CHECK_EQ(fd(100000.0), std::string("100000"));
    CHECK_EQ(fd(123456.0), std::string("123456"));
    CHECK_EQ(fd(999999.0), std::string("999999"));
    CHECK_EQ(fd(9999995.0), std::string("1e+07"));
}

TEST_CASE(double_exponent_switch)
{
    CHECK_EQ(fd(0.001), std::string("0.001"));
    CHECK_EQ(fd(0.0001), std::string("0.0001"));
    CHECK_EQ(fd(0.00012345), std::string("0.00012345"));
    CHECK_EQ(fd(1e-05), std::string("1e-05"));
    CHECK_EQ(fd(-1.5e-05), std::string("-1.5e-05"));
    CHECK_EQ(fd(1e+06), std::string("1e+06"));
    CHECK_EQ(fd(1234567.0), std::string("1.23457e+06"));
    CHECK_EQ(fd(1e+09), std::string("1e+09"));
    CHECK_EQ(fd(1.23456789e15), std::string("1.23457e+15"));
    CHECK_EQ(fd(1e+100), std::string("1e+100"));
    CHECK_EQ(fd(1e-100), std::string("1e-100"));
    CHECK_EQ(fd(DBL_MAX), std::string("1.79769e+308"));
    CHECK_EQ(fd(DBL_MIN), std::string("2.22507e-308"));
    CHECK_EQ(fd(from_bits(1)), std::string("4.94066e-324"));   // smallest denormal
    CHECK_EQ(fd(from_bits(0x000fffffffffffffull)), std::string("2.22507e-308"));
}

TEST_CASE(double_rounding)
{
    // exact ties round half to even (no ROUND_BIASED)
    CHECK_EQ(fd(1.234375), std::string("1.23438"));    // ...37|5, 7 odd -> up
    CHECK_EQ(fd(123456.5), std::string("123456"));     // ...6|5, 6 even -> down
    CHECK_EQ(fd(123457.5), std::string("123458"));
    CHECK_EQ(fd(0.5), std::string("0.5"));
    CHECK_EQ(fd(0.000999999999), std::string("0.001"));
    // Small-integer path (quick path fails on an exact tie): rounding down keeps trailing zeros,
    // and SmartNotation then prints them in exponent form.
    CHECK_EQ(fd(1000005.0), std::string("1.00000e+06"));
    CHECK_EQ(fd(1200005.0), std::string("1.20000e+06"));
    CHECK_EQ(fd(1000015.0), std::string("1.00002e+06"));   // odd -> rounds up, no zeros kept
    CHECK_EQ(fd(1234565.0), std::string("1.23456e+06"));
    CHECK_EQ(fd(1000001.0), std::string("1e+06"));         // quick path: zeros stripped
    CHECK_EQ(fd(150000500000000.0), std::string("1.50000e+14"));
    CHECK_EQ(fd(150000000000005.0), std::string("1.5e+14"));
}

TEST_CASE(double_special)
{
    const double inf = std::numeric_limits<double>::infinity();
    CHECK_EQ(fd(inf), std::string("inf"));
    CHECK_EQ(fd(-inf), std::string("-inf"));
    CHECK_EQ(fd(std::numeric_limits<double>::quiet_NaN()), std::string("nan"));
    CHECK_EQ(fd(-std::numeric_limits<double>::quiet_NaN()), std::string("nan"));
    // qt_is_inf tests the two top bytes only: a NaN whose payload is in the low bytes is "inf",
    // and since 0.0 <= NaN is false it gets a minus sign.
    CHECK_EQ(fd(from_bits(0x7ff0000000000001ull)), std::string("-inf"));
}

TEST_CASE(double_forms)
{
    CHECK_EQ(doubleToString(3.14159, 2, DoubleForm::Decimal), std::string("3.14"));
    CHECK_EQ(doubleToString(0.5, 0, DoubleForm::Decimal), std::string("0"));     // tie, even
    CHECK_EQ(doubleToString(1.5, 0, DoubleForm::Decimal), std::string("2"));
    CHECK_EQ(doubleToString(1234.5, 3, DoubleForm::Decimal), std::string("1234.500"));
    CHECK_EQ(doubleToString(0.0001, 2, DoubleForm::Decimal), std::string("0.00"));
    CHECK_EQ(doubleToString(1234.5, 2, DoubleForm::Exponent), std::string("1.23e+03"));
    CHECK_EQ(doubleToString(-1234.5, 6, DoubleForm::Exponent), std::string("-1.234500e+03"));
    CHECK_EQ(doubleToString(1.5, 6, DoubleForm::SignificantDigits, -1, kAlternate), std::string("1.50000"));
    CHECK_EQ(doubleToString(5.0, 6, DoubleForm::SignificantDigits, 8, kZeroPadded), std::string("00000005"));
    CHECK_EQ(doubleToString(-5.0, 6, DoubleForm::SignificantDigits, 8, kZeroPadded), std::string("-0000005"));
    CHECK_EQ(doubleToString(1e-5, 6, DoubleForm::SignificantDigits, -1, kCapitalEorX), std::string("1E-05"));
    CHECK_EQ(doubleToString(1234567.0, 10, DoubleForm::Decimal, -1, kThousandsGroup), std::string("1,234,567.0000000000"));
    CHECK_EQ(number(0.1).utf8(), std::string("0.1"));
    CHECK_EQ(number(2.0, 'f', 3).utf8(), std::string("2.000"));
    CHECK_EQ(number(1e-7, 'G', 6).utf8(), std::string("1E-07"));
}

TEST_CASE(qdtoa_digits)
{
    Digits d = qdtoa(1.5, 2, 6);
    CHECK_EQ(d.digits, std::string("15"));
    CHECK_EQ(d.decpt, 1);
    d = qdtoa(-0.0, 2, 6);
    CHECK_EQ(d.digits, std::string("0"));
    CHECK_EQ(d.decpt, 1);
    CHECK(d.sign);
    d = qdtoa(105.0, 2, 2);   // tie: the small-integer path keeps the zero on a round-down
    CHECK_EQ(d.digits, std::string("10"));
    CHECK_EQ(d.decpt, 3);
    d = qdtoa(0.0004, 3, 2);  // mode 3 with no digits: Qt returns "0", decpt 1
    CHECK_EQ(d.digits, std::string("0"));
    CHECK_EQ(d.decpt, 1);
}

TEST_CASE(ints)
{
    CHECK_EQ(formatInt(0), std::string("0"));
    CHECK_EQ(formatInt(-7), std::string("-7"));
    CHECK_EQ(formatInt(1234567), std::string("1234567"));   // no grouping in the C locale
    CHECK_EQ(formatInt(INT_MIN), std::string("-2147483648"));
    CHECK_EQ(formatInt(INT_MAX), std::string("2147483647"));
    CHECK_EQ(number(12).utf8(), std::string("12"));
    CHECK_EQ(number(-3).utf8(), std::string("-3"));
}

TEST_CASE(args)
{
    const String key = arg(arg(L("%1%2.KEY"), L("ObjProperties"), 0, u' '), 3, 5, 10, u'0');
    CHECK_EQ(key.utf8(), std::string("ObjProperties00003.KEY"));
    CHECK_EQ(arg(L("BL%1-xx"), 12, 0, 10, u' ').utf8(), std::string("BL12-xx"));
    CHECK_EQ(arg(L("%1 (%2)"), L("Cycle")).utf8(), std::string("Cycle (%2)"));
    CHECK_EQ(arg(L("%2 %1 %1"), 7).utf8(), std::string("%2 7 7"));
    CHECK_EQ(arg(L("x%1y"), 5, -4).utf8(), std::string("x5   y"));
    CHECK_EQ(arg(L("x%1y"), 5, 4).utf8(), std::string("x   5y"));
    CHECK_EQ(arg(L("%1"), -5, 5, 10, u'0').utf8(), std::string("-0005"));
    CHECK_EQ(arg(L("%1"), 255, 0, 16).utf8(), std::string("ff"));
    CHECK_EQ(arg(L("%10|%9"), L("a")).utf8(), std::string("%10|a"));
    CHECK_EQ(arg(L("100% %1"), L("a")).utf8(), std::string("100% a"));
    CHECK_EQ(arg(L("no escape"), 1).utf8(), std::string("no escape"));
    CHECK_EQ(arg(L("Angle %1"), 45.0).utf8(), std::string("Angle 45"));
    CHECK_EQ(arg(L("(%1,%2)"), 0.1, 0, 'g', -1).utf8(), std::string("(0.1,%2)"));
}

TEST_CASE(to_int)
{
    bool ok = false;
    CHECK_EQ(toInt(L(" 42 "), &ok), 42);
    CHECK(ok);
    CHECK_EQ(toInt(L("+7"), &ok), 7);
    CHECK(ok);
    CHECK_EQ(toInt(L("-0"), &ok), 0);
    CHECK(ok);
    CHECK_EQ(toInt(L("2147483647"), &ok), INT_MAX);
    CHECK(ok);
    CHECK_EQ(toInt(L("-2147483648"), &ok), INT_MIN);
    CHECK(ok);
    for (const char* bad : {"", "  ", "4 2", "1,000", "2147483648", "0x10", "1.0", "12a", "-"}) {
        ok = true;
        CHECK_EQ(toInt(L(bad), &ok), 0);
        CHECK(!ok);
    }
    CHECK_EQ(toInt(String(), &ok), 0);
    CHECK(!ok);
}

TEST_CASE(to_double)
{
    bool ok = false;
    CHECK_BITS(toDouble(L("1.5"), &ok), 1.5);
    CHECK(ok);
    CHECK_BITS(toDouble(L(" 2e3 "), &ok), 2000.0);
    CHECK(ok);
    CHECK_BITS(toDouble(L("-.5"), &ok), -0.5);
    CHECK(ok);
    CHECK_BITS(toDouble(L("5."), &ok), 5.0);
    CHECK(ok);
    CHECK_BITS(toDouble(L("1E-2"), &ok), 0.01);
    CHECK(ok);
    CHECK_BITS(toDouble(L("0.1"), &ok), 0.1);
    CHECK(ok);
    CHECK(toDouble(L("INF"), &ok) == std::numeric_limits<double>::infinity());
    CHECK(ok);
    for (const char* bad : {"", "abc", "1e", "1.2.3", "1,5", "infinity", "0x1p3", "1e999", "."}) {
        ok = true;
        CHECK_BITS(toDouble(L(bad), &ok), 0.0);
        CHECK(!ok);
    }
}

int main() { return testkit::run_all(); }
