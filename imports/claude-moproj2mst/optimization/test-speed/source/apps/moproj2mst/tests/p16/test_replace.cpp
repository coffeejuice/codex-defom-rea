// P16: the Diestress KEY name, QString::replace(".DB", ".KEY", Qt::CaseInsensitive) (s1-05 2).
#include <testkit/check.hpp>

#include "../../src/ops/p16_internal.hpp"

using m2m::qt::String;

static String rep(const char16_t* s)
{
    return m2m::p16::replaceAllCi(String(s), u".DB", u".KEY");
}

TEST_CASE(replace_all_case_insensitive)
{
    CHECK(rep(u"C:/w/P/PROJ.DB") == String(u"C:/w/P/PROJ.KEY"));
    CHECK(rep(u"C:/w/P/proj.db") == String(u"C:/w/P/proj.KEY"));
    CHECK(rep(u"C:/a.dB/b.Db") == String(u"C:/a.KEY/b.KEY"));      // every occurrence
    CHECK(rep(u"C:/w/x.DBX") == String(u"C:/w/x.KEYX"));            // not anchored at the end
    CHECK(rep(u"C:/w/x.D") == String(u"C:/w/x.D"));
    CHECK(rep(nullptr).isNull());
    CHECK(rep(u"..DBB") == String(u"..KEYB"));
}

int main()
{
    return testkit::run_all();
}
