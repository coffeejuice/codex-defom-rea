#include "../support/test_paths.hpp"
// .moproj writer 0x1400983c0 against oracle B: open an oracle-A corpus project, load the active
// operation (MDT loader), save it (packet S) and write the project file. The result must equal the
// .moproj DEF_GUI_MO_64 saved for an unchanged copy (gui_mo/oracle_b/<case>).
// RectF, Expanded and Expansion_Items come from the port (should the layout refuse, RectF is taken
// from the input and masked). ViewTransform comes from the ported view transform (moproj::viewCentre)
// for the saves of the fresh, maximised GUI and from the input (masked) for the older saves made at
// another window size. SfDispView comes from the port (its EyeVector, which depends on the window,
// is masked for the saves made at another window size); every other byte must match.
// ctest-labels: m2m-moproj
#include <testkit/check.hpp>

#include <algorithm>
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
    InputViews(Project& p, const xml::Node& sub, bool freshView) : p_(p), fresh_(freshView)
    {
        sub_ = deepCopy(sub);
        for (const auto& c : sub_->children)
            if (c->kind == xml::Node::Element && c->tag == qt::L("DiagramItem")) items_.push_back(c.get());
    }
    void viewCentre(const SubProject& sub, double& x, double& y) override
    {
        if (fresh_) return moproj::viewCentre(sub, *this, x, y);
        const xml::Node* vt = child(*sub_, "ViewTransform");
        x = vt ? num(child(*vt, "CenterX")) : 0;
        y = vt ? num(child(*vt, "CenterY")) : 0;
    }
    xml::NodePtr dispView(const SubProject&) override { return moproj::dispViewElement(p_); }   // the port's
    bool expanded(const SubProject& sub, int i) override { return moproj::savedExpanded(sub, i); }
    moproj::RectF rect(const SubProject& sub, int i) override
    {
        try {
            return moproj::sceneRect(sub, i, *this);
        } catch (const Error& e) {
            if (e.code != M2M_E_UNSUPPORTED) throw;
            rectFromInput = true;
        }
        moproj::RectF r;
        if (const xml::Node* e = child(*item(i), "RectF")) {
            r.x = num(child(*e, "x"));
            r.y = num(child(*e, "y"));
            r.h = num(child(*e, "height"));
            r.w = num(child(*e, "width"));
        }
        return r;
    }
    std::vector<moproj::ExpansionItem> expansion(const SubProject& sub, int i) override
    {
        return sub.flow.items[static_cast<std::size_t>(i)]->expansion;   // the port's (0x140073900)
    }

    bool rectFromInput = false;

private:
    Project& p_;
    bool fresh_;
    xml::NodePtr sub_;
    std::vector<const xml::Node*> items_;
    const xml::Node* item(int i)
    {
        if (i < 0 || i >= static_cast<int>(items_.size())) throw Error(M2M_E_INPUT, "input has fewer DiagramItems");
        return items_[static_cast<std::size_t>(i)];
    }
};

std::string masked(std::string s, bool rect, bool view)
{
    static const std::regex viewT(R"(<ViewTransform>[\s\S]*?</ViewTransform>)");
    static const std::regex parts[] = {
        // group items: the original's flag is uninitialised memory, the port writes "yes"
        // (Aleksandr 2026-10-22); either value is accepted there
        std::regex(R"(Expanded="[a-z]*"( Operation_Id="[^"]*">\s*<ClassName>SfOpr(Cycle|Group)<))"),
    };
    static const std::regex rectF(R"(<RectF>[\s\S]*?</RectF>)");
    for (const auto& r : parts) s = std::regex_replace(s, r, "<view/>$1");
    // EyeVector depends on the GL widget's aspect at the fit (82: -0.613922 where the fresh probe /
    // x1 saves of BEND_2D have the port's -0.533441; R19 one ulp in y)
    static const std::regex eyeV(R"(<EyeVector>[\s\S]*?</EyeVector>)");
    if (rect) s = std::regex_replace(s, rectF, "<view/>");
    if (view) s = std::regex_replace(s, viewT, "<view/>");
    if (view) s = std::regex_replace(s, eyeV, "<view/>");
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

bool runCase(const char* caseA, const char* moproj, const char* caseB, bool fresh)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    const fs::path ob = oracleB() / caseB / moproj;
    if (!corpus || !fs::exists(ob)) {
        std::printf("SKIP %s: no corpus or oracle B\n", caseB);
        return true;
    }
    // caseA: a corpus case, or "B:<dir>" for a pre-open copy kept in the oracle B folder
    const std::string a = caseA;
    const fs::path dirA = a.rfind("B:", 0) == 0 ? oracleB() / a.substr(2) : fs::path(corpus) / caseA;
    const qt::String file{(dirA / moproj).u16string()};
    const qt::String db = app::changeSuffix(file, qt::L(".DB"));
    Project p;
    ProjectScope scope(p);
    Ctx ctx;
    initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
    makeRegistry(p);
    if (!openProject(p, file, ctx) || !p.openedOk) {
        std::printf("FAIL %s: open failed (%s)\n", caseB, file.utf8().c_str());
        return false;
    }
    activateSubProject(p, 0, ctx);
    engine::setEngineVersion(u"");
    const xml::Node* inSub = nullptr;
    int k = 0;
    for (const auto& c : p.dom->root->children)
        if (c->kind == xml::Node::Element && c->tag == qt::L("Sub_Project") && k++ == p.activeSub) inSub = c.get();
    if (!inSub) {
        std::printf("FAIL %s: no Sub_Project element\n", caseB);
        return false;
    }
    InputViews views(p, *inSub, fresh);
    engine::FileEffects open, save, write;
    try {
        Operation* op = engine::openActiveOperation(p, ctx, open);
        if (!op) {
            std::printf("FAIL %s: no active operation\n", caseB);
            return false;
        }
        engine::saveCurrentOperation(p, *op, ctx, save);
        moproj::saveProjectFile(p, false, views, {&open, &save}, write);
    } catch (const Error& e) {
        if (e.code != M2M_E_UNSUPPORTED) throw;
        std::printf("SKIP %s: %s\n", caseB, e.what());
        return true;
    }
    if (write.writes.size() != 1) return false;
    const std::string got = write.writes[0].bytes, want = readAll(ob);
    if (got == want) {
        std::printf("PASS %s: identical\n", caseB);
        return true;
    }
    if (views.rectFromInput) std::puts("   RectF taken from the input (the layout refused)");
    const std::string mg = masked(got, views.rectFromInput, !fresh), mw = masked(want, views.rectFromInput, !fresh);
    if (mg == mw) {
        std::printf("PASS %s: identical outside the view-derived parts\n", caseB);
        showDiff(got, want);
        return true;
    }
    std::printf("FAIL %s (%zu vs %zu bytes)\n", caseB, got.size(), want.size());
    showDiff(mg, mw);
    if (const char* dump = m2m_test::scratchSetting("M2M_MOPROJ_DUMP"))
        std::ofstream(m2m_test::scratchOutput(fs::path(dump) / (std::string(caseB) + ".moproj")), std::ios::binary) << got;
    return false;
}

}  // namespace

TEST_CASE(moproj_write_oracle_b)
{
    // session: the oracle was saved in a GUI process that had saved other projects before
    // (run.py --same-process); reported, not counted. No case uses it now.
    // known: the property trees of these classes differ from the GUI's (TypeName "text" for the GUI's
    // "QString", an invalid value for "0"); Operation_List / properties are L3's (coordinator).
    // fresh: saved by a GUI that was maximised before the project opened (the view profile of
    // moproj::viewCentre); R11, R19, 78, 79 and 82 were saved at another window size
    struct C { const char* a; const char* moproj; const char* b; bool session = false; const char* known = nullptr; bool fresh = true; };
    const C cases[] = {
        {"11_THEXP", "THEXP.moproj", "R11", false, nullptr, false},
        {"19_XY_H01", "XY_H01.moproj", "R19", false, nullptr, false},
        {"11_THEXP", "THEXP.moproj", "78_R11_THEXP", false, nullptr, false},
        {"19_XY_H01", "XY_H01.moproj", "79_R19_XY_H01", false, nullptr, false},
        {"52_BEND_2D", "BEND_2D.moproj", "82_R52_BEND_2D", false, nullptr, false},
        {"B:93_B_MB3ACT_before", "Gear_blank.moproj", "93_B_MB3ACT", false, "MultiBlow3D property tree (L3)"},
        {"B:93D_B_MB3ACT_DWELL_before", "Gear_blank.moproj", "93D_B_MB3ACT_DWELL", false, "MultiBlow3D Process editor (L4)"},
        {"B:94_B_MB2ACT_before", "MO2_Operation_Cycles2.moproj", "94_B_MB2ACT", false, "MultiBlow2D property tree (L3)"},
        {"B:95_B_FE3ACT_before", "Gear_blank.moproj", "95_B_FE3ACT", false, "FormingExpress property tree (L3)"},
        {"B:96_B_FNOEQ_before", "Gear_blank.moproj", "96_B_FNOEQ", false, "Forming property values (L3)"},
        {"B:97_B_CNVACT_before", "MO2_Operation_Cycles2.moproj", "97_B_CNVACT"},
        // L4 widget-area sweep probes (oracle_b/_tools/mkl4w.py)
        {"B:105_B_COG7_before", "Cogging_Multiple_Ops_Lab2.moproj", "105_B_COG7"},
        {"B:115_B_COG7P1_before", "Cogging_Multiple_Ops_Lab2.moproj", "115_B_COG7P1"},
        {"B:106_B_F2ACT_before", "MO2_Operation_Cycles2.moproj", "106_B_F2ACT", false, "Forming2D GeometryType editor (L4)"},
        {"B:108_B_SOPTA_before", "Gear_blank.moproj", "108_B_SOPTA"},
        {"B:109_B_FE7_before", "Gear_blank.moproj", "109_B_FE7", false, "FormingExpress 7 digits (L4)"},
        // Boolean projects saved by a fresh GUI process; they replace 80, 81, 83 and 84, saved after
        // other projects in one process, whose Boolean operations lack GENCTC (coordinator
        // 2026-10-22)
        {"B:R46F_before", "MO2_Boolean_Oprations.moproj", "R46F"},
        {"B:R47F_before", "MO2_Extrusion_and_Boolean_Cycles.moproj", "R47F"},
        {"B:R49F_before", "MO2_Boolean_Oprations.moproj", "R49F"},
        {"B:R50F_before", "MO2_Extrusion_and_Boolean_Cycles.moproj", "R50F"},
    };
    for (const C& c : cases) {
        const bool ok = runCase(c.a, c.moproj, c.b, c.fresh);
        if (c.session) {
            if (!ok) std::puts("   (not counted: oracle saved in a non-fresh GUI process)");
        } else if (c.known) {
            if (!ok) std::printf("   (not counted, known: %s)%c", c.known, '\n');
        } else {
            CHECK(ok);
        }
    }
}

// ViewTransform of every oracle B save made in the fresh-GUI profile: open the saved project (its
// sub-project 0, the one File > Save wrote), lay it out with the GUI views and compare CenterX / CenterY as written.
// The profile's saves are the 1650 family of CenterX; the rest were saved at other window sizes
// (the _before copies, B01..B20, 59..92 of the earlier sessions).
// Not open-then-save, so not this profile: the DOE runs (12x_B_DOE*) add a study in the diagram
// before saving. 120_B_DOE and 121_B_DOE_VAR saved the main sub-project while the study was active
// (scale 0.4774, no fit of its items); their _before copies and 122_B_DOE_GEO hold COG_P1_B1 with the
// view scrolled 26 px down from the open state (CenterY 239.026 against 160.359). The DB-step runs'
// inputs (124/125/126_B_DBSTEP*_before) are copies of that same scrolled file, so they are skipped
// too; their saves are checked. A project whose database opens has the diagram view 175 px high
// instead of 123 (viewport 173: scale 169 / (bh + 60), CenterY with py 87): the open sets
// Project::diagramViewHeight (db_load.cpp), so 124, 127..133 get it from their DB and 125 (no DB)
// and 126 (renamed DB) keep 123.
TEST_CASE(moproj_view_centre_oracle_b)
{
    const fs::path ob = oracleB();
    if (ob.empty() || !fs::exists(ob)) {
        std::puts("SKIP view centre: no oracle B");
        return;
    }
    static const std::regex cx(R"(<CenterX>([^<]*)</CenterX>\s*<CenterY>([^<]*)</CenterY>)");
    int n = 0, pass = 0;
    for (const auto& d : fs::directory_iterator(ob)) {
        if (!d.is_directory()) continue;
        const std::string dn = d.path().filename().string();
        if (dn.find("_B_DOE") != std::string::npos) continue;   // see above
        if (dn.find("_B_DBSTEP") != std::string::npos && dn.size() > 7 && dn.compare(dn.size() - 7, 7, "_before") == 0)
            continue;
        for (const auto& f : fs::directory_iterator(d.path())) {
            if (f.path().extension() != ".moproj") continue;
            const std::string text = readAll(f.path());
            std::smatch m;
            if (!std::regex_search(text, m, cx) || std::atof(m[1].str().c_str()) < 1500.0) continue;
            const qt::String file{f.path().u16string()};
            const qt::String db = app::changeSuffix(file, qt::L(".DB"));
            Project p;
            ProjectScope scope(p);
            Ctx ctx;
            initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
            makeRegistry(p);
            if (!openProject(p, file, ctx) || !p.openedOk) continue;
            activateSubProject(p, 0, ctx);
            if (p.subs.empty()) continue;
            // the converter's own views (expansion lists built at open, 0x140073900)
            const auto views = moproj::guiViews(p);
            double x = 0, y = 0;
            views->viewCentre(*p.subs[0], x, y);
            const std::string gx = qt::number(x, 'g', 6).utf8(), gy = qt::number(y, 'g', 6).utf8();
            ++n;
            if (gx == m[1].str() && gy == m[2].str()) {
                ++pass;
            } else {
                std::printf("FAIL view centre %s: port %s %s, gui %s %s\n", d.path().filename().string().c_str(),
                            gx.c_str(), gy.c_str(), m[1].str().c_str(), m[2].str().c_str());
            }
        }
    }
    std::printf("view centre: %d/%d saves match\n", pass, n);
    CHECK(n > 0);
    CHECK_EQ(pass, n);
}

int main()
{
    return testkit::run_all();
}
