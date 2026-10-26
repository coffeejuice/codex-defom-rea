// P08 (P02 split): NSTART / UserDefinedKey / RestartKey parse, WriteBlob into the overlay, and the
// Properties tree (ParseProperties 0x140094f70, ApplyProperty 0x1400952c0, setValue 0x140370600).
#include <cstdio>
#include <string>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"

using namespace m2m;
using qt::L;

static int g_fail = 0;

static void check(const char* what, bool ok)
{
    if (ok) return;
    ++g_fail;
    std::printf("FAIL %s\n", what);
}

static xml::Document parse(const char* text)
{
    xml::Document d;
    String err;
    int line = 0, col = 0;
    if (!xml::setContent(d, text, &err, &line, &col)) {
        std::printf("XML error %s at %d:%d\n", err.utf8().c_str(), line, col);
        ++g_fail;
    }
    return d;
}

static void test_keys()
{
    Project p;
    Operation op;
    check("defaults", !nstartGiven(op) && startStepNo(op) == 0 && startStep(op) == 0 &&
                          !restartGiven(op) && op.restartKey == L("RestartStep.KEY"));

    auto d = parse("<NSTART><StartStepNo>-3</StartStepNo></NSTART>");
    parseNstart(p, *d.root, op);
    check("nstart", nstartGiven(op) && startStepNo(op) == -3 && startStep(op) == 2);
    d = parse("<NSTART><StartStepNo>7</StartStepNo><StartStepNo> 12 </StartStepNo></NSTART>");
    parseNstart(p, *d.root, op);
    check("nstart last wins", startStepNo(op) == 12 && startStep(op) == 12);
    d = parse("<NSTART><Other>5</Other></NSTART>");
    parseNstart(p, *d.root, op);
    check("nstart absent", !nstartGiven(op) && startStepNo(op) == 12 && startStep(op) == 0);
    resetNstart(op);
    check("reset", !nstartGiven(op) && startStepNo(op) == 0);

    d = parse("<UserDefinedKey><KeyFileName>C:/k/User.KEY</KeyFileName></UserDefinedKey>");
    parseUserDefinedKey(p, *d.root, op);
    check("user key", op.userKeyGiven && op.userKey == L("C:/k/User.KEY"));
    d = parse("<UserDefinedKey/>");
    parseUserDefinedKey(p, *d.root, op);
    check("user key absent", !op.userKeyGiven && op.userKey == L("C:/k/User.KEY"));

    d = parse("<RestartKey><KeyFileName>Step12.KEY</KeyFileName></RestartKey>");
    parseRestartKey(p, *d.root, op);
    check("restart key", restartGiven(op) && op.restartKey == L("Step12.KEY"));
    d = parse("<RestartKey></RestartKey>");
    parseRestartKey(p, *d.root, op);
    check("restart key absent", !restartGiven(op) && op.restartKey == L("Step12.KEY"));
}

static void test_blob()
{
    Project p;
    Ctx ctx;
    // qCompress("*KEY\nOTPRNG 1\nOPSTOP 2\n") in hex (4-byte big-endian size, then zlib); the first
    // Data is garbage and is replaced by the second (the last Data wins).
    auto d = parse("<ObjPropertiesKey><Data>00</Data><Data>"
                   "00000017789CD3F2768DE4F20F0908F2735730E4F20F080EF10F5030E2020045950594"
                   "</Data></ObjPropertiesKey>");
    String path = L("C:/x/Operations/Task00004/ObjProperties00001.KEY");
    storeBlob(p, *d.root, path, ctx);
    auto it = p.overlay.find(path);
    check("blob stored", it != p.overlay.end() && it->second == "*KEY\nOTPRNG 1\nOPSTOP 2\n");
    check("blob warning from the first Data", !ctx.warnings.empty());
}

// A class whose slot adds a sibling (as setNumHeats does) and records what it saw.
struct GrowingProps : Props {
    std::string log;
    void applyValue(PropNode& node, const std::vector<PropNode*>& anc, const qt::Variant& v) override
    {
        log += node.name.utf8() + "@" + std::to_string(anc.size()) + "=" +
               (v.kind == qt::Variant::Double ? "d" : v.kind == qt::Variant::Int ? "i" : "s") +
               (node.value.kind == qt::Variant::Invalid ? "(old none)" : "(old set)") + ";";
        if (node.name == L("NumHeats") && !anc.empty()) {
            for (int k = 0; k < v.i; ++k) {
                PropNode h;
                h.name = L("Heat");
                h.children.push_back(PropNode{L("Temperature"), L("double"), {}, -1, {}});
                anc.back()->children.push_back(h);
            }
            // Grow the top level too (as MultiBlow's NumHeats grows the root's children), so the
            // Settings node and every pointer to it move.
            for (int k = 0; k < 10; ++k) tree.push_back(PropNode{L("Extra"), L("double"), {}, -1, {}});
        }
    }
};

static PropNode leaf(const char* n, const char* t) { return PropNode{L(n), L(t), {}, -1, {}}; }

static void test_props()
{
    GrowingProps props;
    PropNode group{L("Settings"), L("group"), {}, -1, {}};
    group.children = {leaf("NumHeats", "int"), leaf("Ratio", "double"), leaf("Mode", "enum")};
    props.tree = {group, leaf("HeatTrasnferExpress Time", "double"), leaf("Top", "double")};
    Operation op;
    op.props = std::shared_ptr<Props>(&props, [](Props*) {});
    Project p;
    Ctx ctx;
    auto d = parse(
        "<Properties>"
        " <Property><Name>Settings</Name><TypeName>group</TypeName>"
        "  <Property><Name>NumHeats</Name><TypeName>int</TypeName><Value>2</Value></Property>"
        "  <Property><Name>Ratio</Name><TypeName>double</TypeName><Value>0.25</Value></Property>"
        "  <Property><Name>Mode</Name><TypeName>enum</TypeName><Value>1</Value></Property>"
        "  <Property><Name>Heat</Name><TypeName>group</TypeName>"
        "   <Property><Name>Temperature</Name><TypeName>double</TypeName><Value>1200</Value></Property>"
        "  </Property>"
        " </Property>"
        " <Property><Name>Missing</Name><TypeName>double</TypeName><Value>9</Value></Property>"
        " <Property><Name>AirConvection</Name><TypeName>double</TypeName><Value>30</Value></Property>"
        " <Property><Value>5</Value><Name>Top</Name><TypeName>double</TypeName><Value>7.5</Value></Property>"
        "</Properties>");
    parseProperties(p, *d.root, op, ctx);
    const auto& s = props.tree[0];
    check("tree size", props.tree.size() == 13 && s.children.size() == 5);
    check("int", s.children[0].value.kind == qt::Variant::Int && s.children[0].value.i == 2);
    check("double", s.children[1].value.kind == qt::Variant::Double && s.children[1].value.d == 0.25);
    check("enum as text", s.children[2].value.kind == qt::Variant::Text && s.children[2].value.s == L("1"));
    // The candidates of a nested <Property> are Settings' children when that element is reached
    // (0x140370f60 per element), so "Heat" finds the first heat the NumHeats slot added.
    check("added heat is a candidate", s.children[3].children[0].value.d == 1200.0 &&
                                           s.children[4].children[0].value.kind == qt::Variant::Invalid);
    check("legacy name", props.tree[1].value.kind == qt::Variant::Double && props.tree[1].value.d == 30.0);
    // A Value before Name has no node (skipped); the later Value is applied.
    check("value after name", props.tree[2].value.d == 7.5);
    check("slot order and ancestors", props.log ==
          "NumHeats@1=i(old none);Ratio@1=d(old none);Mode@1=s(old none);Temperature@2=d(old none);"
          "HeatTrasnferExpress Time@0=d(old none);Top@0=d(old none);");
}

int main()
{
    test_keys();
    test_blob();
    test_props();
    std::printf(g_fail ? "%d failures\n" : "all P08 props/keys tests passed\n", g_fail);
    return g_fail ? 1 : 0;
}
