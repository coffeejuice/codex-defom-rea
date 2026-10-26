// ctest-labels: m2m-p15
// applyProperty (0x1400952c0) matches a saved <Name> against the node name of 0x14036fcc0, which
// appends index + 1 to an indexed node ("Heat1", "NumBlows1", "BlowEnergy2"). Links m2m_core.
#include <testkit/check.hpp>

#include <string>

#include "m2m/props.hpp"
#include "m2m/xml.hpp"

using namespace m2m;

namespace {

struct RecProps : Props {
    std::string seen;
    void applyValue(PropNode& n, const std::vector<PropNode*>&, const qt::Variant&) override
    {
        seen += n.name.utf8() + std::to_string(n.index) + ",";
    }
};

PropNode leaf(const char* name, int index)
{
    PropNode n;
    n.name = qt::L(name);
    n.typeName = qt::L("int");
    n.index = index;
    return n;
}

}  // namespace

TEST_CASE(indexed_names_match_with_suffix)
{
    RecProps p;
    PropNode heat = leaf("Heat", 1);
    heat.typeName = qt::L("group");
    heat.children.push_back(leaf("NumBlows", 1));
    p.tree.push_back(leaf("Heat", 0));
    p.tree.push_back(heat);
    p.tree.push_back(leaf("NumHeats", -1));

    const std::string xml =
        "<Property><Name>Heat2</Name><TypeName>group</TypeName>"
        "<Property><Name>NumBlows2</Name><TypeName>int</TypeName><Value>3</Value></Property>"
        "</Property>";
    const std::string bare =
        "<Property><Name>Heat</Name><TypeName>group</TypeName>"
        "<Property><Name>NumBlows</Name><TypeName>int</TypeName><Value>4</Value></Property>"
        "</Property>";
    for (const std::string* s : {&xml, &bare}) {
        xml::Document doc;
        String err;
        int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, *s, &err, &line, &col));
        applyProperty(p, *doc.root, {});
    }
    CHECK(p.seen == "NumBlows1,");                 // the bare names match no indexed node
    CHECK(p.tree[1].children[0].value.i == 3);
}

int main() { return testkit::run_all(); }
