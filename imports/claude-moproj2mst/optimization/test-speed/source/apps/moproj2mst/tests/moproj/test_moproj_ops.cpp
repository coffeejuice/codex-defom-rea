#include "../support/test_paths.hpp"
// moproj-ops: <Operation_List> of the .moproj writer (0x14009cfd0 with the properties 0x14009fb40,
// the blobs 0x1400a1890 and the ActionKeywords helpers) against oracle B, byte for byte. Inputs are
// the corpus projects for the R cases (an unchanged re-save) and the <case>_before copies for the
// others; each case opens the project, loads and saves the active operation and writes the file.
// The view-derived parts come from the input file (InputViews) as in test_moproj_write.cpp.
// ctest-labels: m2m-moproj m2m-moproj-ops
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <regex>
#include <string>

#include "m2m/engine.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"

namespace fs = std::filesystem;
using namespace m2m;

namespace {

std::string readAll(const fs::path& f)
{
    std::ifstream in(f, std::ios::binary);
    return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
}

fs::path oracleB()
{
    return m2m_test::configuredPath("M2M_ORACLE_B");
}

const xml::Node* child(const xml::Node& n, const char* tag)
{
    for (const auto& c : n.children)
        if (c->kind == xml::Node::Element && c->tag == qt::L(tag)) return c.get();
    return nullptr;
}

double num(const xml::Node* n) { return n ? qt::toDouble(n->text()) : 0.0; }

xml::NodePtr deepCopy(const xml::Node& n)
{
    auto c = std::make_shared<xml::Node>();
    c->kind = n.kind;
    c->tag = n.tag;
    c->data = n.data;
    c->attrs = n.attrs;
    for (const auto& k : n.children) c->children.push_back(deepCopy(*k));
    return c;
}

// The view-derived parts as the input file has them (a stand-in for the GUI's views).
class InputViews : public moproj::DiagramViews {
public:
    explicit InputViews(const xml::Node& sub)
    {
        sub_ = deepCopy(sub);
        for (const auto& c : sub_->children)
            if (c->kind == xml::Node::Element && c->tag == qt::L("DiagramItem")) items_.push_back(c.get());
    }
    void viewCentre(const SubProject&, double& x, double& y) override
    {
        const xml::Node* vt = child(*sub_, "ViewTransform");
        x = vt ? num(child(*vt, "CenterX")) : 0;
        y = vt ? num(child(*vt, "CenterY")) : 0;
    }
    xml::NodePtr dispView(const SubProject&) override
    {
        const xml::Node* d = child(*sub_, "SfDispView");
        return d ? deepCopy(*d) : nullptr;
    }
    bool expanded(const SubProject&, int i) override { return item(i)->attribute(qt::L("Expanded")) == qt::L("yes"); }
    moproj::RectF rect(const SubProject&, int i) override
    {
        moproj::RectF r;
        if (const xml::Node* e = child(*item(i), "RectF")) {
            r.x = num(child(*e, "x"));
            r.y = num(child(*e, "y"));
            r.h = num(child(*e, "height"));
            r.w = num(child(*e, "width"));
        }
        return r;
    }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int i) override
    {
        std::vector<moproj::ExpansionItem> r;
        const xml::Node* x = child(*item(i), "Expansion_Items");
        if (!x) return r;
        for (const xml::Node* e : x->childElements()) {
            moproj::ExpansionItem it;
            const std::string t = e->attribute(qt::L("Type")).utf8();
            it.type = t == "Text" ? 0 : t == "Line" ? 1 : t == "Space" ? 2 : 3;
            it.label = e->attribute(qt::L("Label"));
            const std::string st = e->attribute(qt::L("LineStyle")).utf8();
            it.lineStyle = st == "NoLine" ? 0 : st == "Dash" ? 2 : st == "Dot" ? 3 : st == "DashDot" ? 4 : st == "DashDotDot" ? 5 : 1;
            std::sscanf(e->attribute(qt::L("LineColor")).utf8().c_str(), "%d, %d, %d, %d", &it.r, &it.g, &it.b, &it.a);
            r.push_back(it);
        }
        return r;
    }

private:
    xml::NodePtr sub_;
    std::vector<const xml::Node*> items_;
    const xml::Node* item(int i)
    {
        if (i < 0 || i >= static_cast<int>(items_.size())) throw Error(M2M_E_INPUT, "input has fewer DiagramItems");
        return items_[static_cast<std::size_t>(i)];
    }
};

std::string masked(std::string s)
{
    static const std::regex parts[] = {
        std::regex(R"(<ViewTransform>[\s\S]*?</ViewTransform>)"),
        std::regex(R"(<SfDispView>[\s\S]*?</SfDispView>)"),
        std::regex(R"(<RectF>[\s\S]*?</RectF>)"),
        std::regex(R"(<Expansion_Items[\s\S]*?</Expansion_Items>|<Expansion_Items[^>]*/>)"),
        std::regex(R"(Expanded="[a-z]*")"),
    };
    for (const auto& r : parts) s = std::regex_replace(s, r, "<view/>");
    return s;
}

void showDiff(const std::string& a, const std::string& b)
{
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    std::size_t s = a.rfind('\n', i);
    s = s == std::string::npos ? 0 : s + 1;
    auto line = [&](const std::string& t) { return t.substr(s, std::min<std::size_t>(t.find('\n', s) - s, 200)); };
    std::printf("   first difference at byte %zu\n   port: %s\n   gui:  %s\n", i, line(a).c_str(), line(b).c_str());
}

// The <Operation_List> element of a saved file (empty when absent).
std::string operationList(const std::string& s)
{
    const std::size_t b = s.find("<Operation_List");
    if (b == std::string::npos) return {};
    const std::size_t e = s.find("</Operation_List>", b);
    return e == std::string::npos ? s.substr(b) : s.substr(b, e + 17 - b);
}

enum class R { Pass, Fail, Skip };

R runCase(const fs::path& dirA, const char* moproj, const char* caseB)
{
    const fs::path ob = oracleB() / caseB / moproj;
    if (!fs::exists(ob) || !fs::exists(dirA / moproj)) {
        std::printf("SKIP %s: no input or oracle B\n", caseB);
        return R::Skip;
    }
    const qt::String file{(dirA / moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) return R::Fail;
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    const xml::Node* inSub = nullptr;
    int k = 0;
    for (const auto& c : p.dom->root->children)
        if (c->kind == xml::Node::Element && c->tag == qt::L("Sub_Project") && k++ == p.activeSub) inSub = c.get();
    if (!inSub) return R::Fail;
    InputViews views(*inSub);
    engine::FileEffects open, save, write;
    try {
        Operation* op = engine::openActiveOperation(p, ctx, open);
        if (!op) return R::Fail;
        engine::saveCurrentOperation(p, *op, ctx, save);
        moproj::saveProjectFile(p, false, views, {&open, &save}, write);
    } catch (const Error& e) {
        if (e.code != M2M_E_UNSUPPORTED) throw;
        std::printf("SKIP %s: %s\n", caseB, e.what());
        return R::Skip;
    }
    if (write.writes.size() != 1) return R::Fail;
    const std::string got = write.writes[0].bytes, want = readAll(ob);
    if (const char* dump = m2m_test::scratchSetting("M2M_MOPROJ_DUMP"))
        std::ofstream(m2m_test::scratchOutput(fs::path(dump) / (std::string(caseB) + ".moproj")), std::ios::binary) << got;
    const std::string og = operationList(got), ow = operationList(want);
    const bool whole = masked(got) == masked(want);
    if (og == ow && !og.empty()) {
        std::printf("PASS %s: Operation_List identical (%zu bytes); whole file %s\n", caseB, og.size(),
                    whole ? "identical outside the view parts" : "differs elsewhere");
        return R::Pass;
    }
    std::printf("FAIL %s: Operation_List differs\n", caseB);
    showDiff(og, ow);
    return R::Fail;
}

}  // namespace

TEST_CASE(operation_list_oracle_b)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    const fs::path c = corpus, b = oracleB();
    struct C { fs::path a; const char* moproj; const char* b; };
    const C cases[] = {
        {c / "11_THEXP", "THEXP.moproj", "R11"},
        {c / "19_XY_H01", "XY_H01.moproj", "R19"},
        // 46, 47, 49, 50: the fresh-process saves (R46F .. R50F; inputs <case>_before). The older
        // reused-process saves 80/81/83/84 lack the Boolean GENCTC line (session state).
        {b / "R46F_before", "MO2_Boolean_Oprations.moproj", "R46F"},
        {b / "R47F_before", "MO2_Extrusion_and_Boolean_Cycles.moproj", "R47F"},
        {b / "R49F_before", "MO2_Boolean_Oprations.moproj", "R49F"},
        {b / "R50F_before", "MO2_Extrusion_and_Boolean_Cycles.moproj", "R50F"},
        {c / "52_BEND_2D", "BEND_2D.moproj", "82_R52_BEND_2D"},
        {b / "93_B_MB3ACT_before", "Gear_blank.moproj", "93_B_MB3ACT"},
        {b / "93D_B_MB3ACT_DWELL_before", "Gear_blank.moproj", "93D_B_MB3ACT_DWELL"},
        {b / "94_B_MB2ACT_before", "MO2_Operation_Cycles2.moproj", "94_B_MB2ACT"},
        {b / "95_B_FE3ACT_before", "Gear_blank.moproj", "95_B_FE3ACT"},
        {b / "96_B_FNOEQ_before", "Gear_blank.moproj", "96_B_FNOEQ"},
        {b / "97_B_CNVACT_before", "MO2_Operation_Cycles2.moproj", "97_B_CNVACT"},
        {b / "98_B_DS3F_before", "Gear_blank.moproj", "98_B_DS3F"},
        {b / "99_B_BM2F_before", "MO2_Operation_Cycles2.moproj", "99_B_BM2F"},
        {b / "100_B_BM3F_before", "Gear_blank.moproj", "100_B_BM3F"},
        {b / "101_B_DIS3F_before", "Gear_blank.moproj", "101_B_DIS3F"},
        {b / "102_B_DIS2F_before", "MO2_Operation_Cycles2.moproj", "102_B_DIS2F"},
    };
    int pass = 0, skip = 0, fail = 0;
    for (const C& k : cases) {
        const R r = runCase(k.a, k.moproj, k.b);
        CHECK(r != R::Fail);
        (r == R::Pass ? pass : r == R::Skip ? skip : fail)++;
    }
    std::printf("%d pass, %d fail, %d skipped\n", pass, fail, skip);
}

int main()
{
    return testkit::run_all();
}
