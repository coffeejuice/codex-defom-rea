// P04: QString members with rules (split, contains, startsWith, endsWith, toAscii), the locale
// codec and the MST stream (QTextStream write buffer, endl flush).
#include <testkit/check.hpp>

#include <string>
#include <vector>

#include "m2m/mst/stream.hpp"
#include "m2m/qt4.hpp"

using namespace m2m::qt;

static std::vector<std::string> u8(const std::vector<String>& v)
{
    std::vector<std::string> r;
    for (const auto& s : v) r.push_back(s.utf8());
    return r;
}

TEST_CASE(split_rules)
{
    using V = std::vector<std::string>;
    CHECK(u8(split(L("a,,b"), L(","))) == (V{"a", "", "b"}));
    CHECK(u8(split(L("a,,b"), L(","), SplitBehavior::SkipEmptyParts)) == (V{"a", "b"}));
    CHECK(u8(split(L(""), L(","))) == (V{""}));
    CHECK(split(L(""), L(","), SplitBehavior::SkipEmptyParts).empty());
    CHECK(u8(split(L("a b"), L(" "))) == (V{"a", "b"}));
    CHECK(u8(split(L(",a,"), L(","))) == (V{"", "a", ""}));
    CHECK(u8(split(L("ab"), L(""))) == (V{"", "a", "b", ""}));
    // mid(): a part at the end of a non-null string is empty, not null
    auto parts = split(L("a,"), L(","));
    CHECK(!parts.back().isNull());
    CHECK(split(String(), L(","))[0].isNull());
}

TEST_CASE(contains_starts_ends)
{
    CHECK(contains(L("Operations/Task00001"), L("Task")));
    CHECK(!contains(L("abc"), L("ABC")));
    CHECK(contains(L("abc"), L("ABC"), Case::Insensitive));
    CHECK(contains(L("abc"), L("")));
    CHECK(startsWith(L("SfOprHeat"), L("SfOpr")));
    CHECK(!startsWith(L("Sf"), L("SfOpr")));
    CHECK(startsWith(L("x.MST.TEMP"), L("X.mst"), Case::Insensitive));
    CHECK(endsWith(L("x.MST.TEMP"), L(".MST.TEMP")));
    CHECK(endsWith(L("x.mst.temp"), L(".MST.TEMP"), Case::Insensitive));
    CHECK(!endsWith(L("x.mst.temp"), L(".MST.TEMP")));
    // qt_starts_with / qt_ends_with null rules
    CHECK(startsWith(String(), String()));
    CHECK(!startsWith(String(), L("")));
    CHECK(startsWith(L(""), String()));
    CHECK(startsWith(L(""), L("")));
    CHECK(!startsWith(L(""), L("a")));
    CHECK(startsWith(L("a"), L("")));
    CHECK(!endsWith(String(), L("")));
    CHECK(endsWith(L("a"), String()));
}

TEST_CASE(to_ascii_and_codec)
{
    CHECK_EQ(toAscii(String(std::u16string(u"aéĀ"))), std::string("a\xe9?"));
    CHECK_EQ(encodeLocale(String(std::u16string(u"aé€")), 1252), std::string("a\xe9\x80"));
    CHECK_EQ(encodeLocale(String(std::u16string(u"А")), 1251), std::string("\xc0"));
    CHECK_EQ(encodeLocale(String(), 1252), std::string());
    // Unmappable characters: best fit, else the default character '?' (WideCharToMultiByte, 0)
    CHECK_EQ(encodeLocale(String(std::u16string(u"中")), 1252), std::string("?"));
}

TEST_CASE(stream_basic)
{
    m2m::mst::Stream ts(1252);
    ts << "*!TITLE  " << L("x") << m2m::mst::endl;
    ts << "*!UNIT   " << 1 << m2m::mst::endl;
    ts << "\t" << 2.5 << "\t" << -0.0 << "\t" << 1e-5 << m2m::mst::endl;
    ts << m2m::mst::endl;
    CHECK_EQ(ts.bytes(), std::string("*!TITLE  x\n*!UNIT   1\n\t2.5\t0\t1e-05\n\n"));
    CHECK(ts.pending().empty());
}

TEST_CASE(stream_unflushed_is_lost)
{
    // The writers close the QFile before ~QTextStream: text after the last endl never arrives.
    m2m::mst::Stream ts(1252);
    ts << "a" << m2m::mst::endl << "b";
    CHECK_EQ(ts.bytes(), std::string("a\n"));
    CHECK(ts.pending() == std::u16string(u"b"));
}

TEST_CASE(stream_buffer_limit_and_codec)
{
    m2m::mst::Stream ts(1252);
    std::string big(16384, 'x');
    ts << big.c_str();
    CHECK(ts.bytes().empty());          // exactly 16384: not over the limit yet
    ts << "y";
    CHECK_EQ(ts.bytes().size(), static_cast<size_t>(16385));   // flushed when it grew past it
    ts << "\xe9" << String(std::u16string(u"€")) << m2m::mst::endl;
    CHECK_EQ(ts.bytes().substr(16385), std::string("\xe9\x80\n"));
}

int main() { return testkit::run_all(); }
