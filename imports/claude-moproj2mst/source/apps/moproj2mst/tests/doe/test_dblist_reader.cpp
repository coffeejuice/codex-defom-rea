// ctest-labels: m2m-mdt doe
#include <testkit/check.hpp>

#include <string>

#include "../../src/doe/dblist_reader.hpp"

using namespace m2m;

namespace {
xml::NodePtr parse(const std::string& bytes)
{
    xml::Document document;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, bytes, &error, &line, &column));
    return document.root;
}
} // namespace

TEST_CASE(doe_dblist_reader_returns_b127_direct_child_shape_in_order)
{
    // Matches the retained B127 training shape. This is a parser fixture, not
    // a claim that the original helper accepts or resolves this DBNAME.
    const auto root = parse(
        "<SessionRoot><DBLIST><DBNAME>../../../COG_P1_B1.DB</DBNAME></DBLIST>"
        "<ReportList/><Session><Image/></Session></SessionRoot>");
    const auto* dbList = root->childElements(qt::L("DBLIST")).front();
    const auto before = dbList->text();

    const auto children = doe::readDbListDirectChildren(*dbList);

    REQUIRE(children.size() == 1);
    CHECK(children[0].tag == qt::L("DBNAME"));
    CHECK(children[0].text == qt::L("../../../COG_P1_B1.DB"));
    CHECK(dbList->text() == before);
}

TEST_CASE(doe_dblist_reader_preserves_direct_element_order_and_text)
{
    const auto root = parse(
        "<DBLIST><DBNAME>first</DBNAME><Other><Value>nested</Value></Other>"
        "<DBNAME>last</DBNAME></DBLIST>");
    const auto children = doe::readDbListDirectChildren(*root);

    REQUIRE(children.size() == 3);
    CHECK(children[0].tag == qt::L("DBNAME"));
    CHECK(children[0].text == qt::L("first"));
    CHECK(children[1].tag == qt::L("Other"));
    CHECK(children[1].text == qt::L("nested"));
    CHECK(children[2].tag == qt::L("DBNAME"));
    CHECK(children[2].text == qt::L("last"));
}

TEST_CASE(doe_dblist_reader_empty_and_unusual_shapes_are_parser_mechanics_only)
{
    const auto empty = parse("<DBLIST/>");
    CHECK(doe::readDbListDirectChildren(*empty).empty());

    // Repeated names and empty text are preserved rather than interpreted.
    // No native-format validity is asserted by this mechanics probe.
    const auto unusual = parse("<DBLIST><DBNAME/><DBNAME>two</DBNAME></DBLIST>");
    const auto children = doe::readDbListDirectChildren(*unusual);
    REQUIRE(children.size() == 2);
    CHECK(children[0].tag == qt::L("DBNAME"));
    CHECK(children[0].text.isEmpty());
    CHECK(children[1].tag == qt::L("DBNAME"));
    CHECK(children[1].text == qt::L("two"));
}

int main() { return testkit::run_all(); }
