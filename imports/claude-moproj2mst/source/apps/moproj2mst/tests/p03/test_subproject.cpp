// P03: sub-project activation and parse (0x1400a7b90, 0x14008f530, 0x140091f00, 0x1400a78e0) on
// every corpus project, through the real XML seam. Until P01 (QDomDocument) and P04
// (QString::toInt/contains) merge, their stubs throw NotPorted and the cases report SKIP.
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>

#include "m2m/model.hpp"
#include "m2m/xml.hpp"
#include "order_corpus.hpp"

using namespace m2m;
using m2m::qt::L;

namespace {

bool skipped = false;

void skip(const NotPorted& e)
{
    if (!skipped) std::printf("SKIP: %s\n", e.what());
    skipped = true;
}

}  // namespace

TEST_CASE(subproject_ctor)
{
    Project p;
    auto s = makeSubProject(p, 2, 7);
    CHECK(s->project == &p);
    CHECK_EQ(s->type, 2);
    CHECK_EQ(s->id, 7);
    CHECK(!s->loaded);
    CHECK(s->doe != nullptr); // original Type2 constructor owns a DOE manager even before lazy load
    CHECK(s->ownedDoeStudy != nullptr);
    CHECK(s->dbName.isNull());
}

TEST_CASE(active_subproject_bounds)
{
    Project p;
    CHECK(activeSubProject(p) == nullptr);
    p.subs.push_back(makeSubProject(p, 0, 0));
    CHECK(activeSubProject(p) == p.subs[0].get());
    p.activeSub = 1;
    CHECK(activeSubProject(p) == nullptr);
    p.activeSub = -1;
    CHECK(activeSubProject(p) == nullptr);
}

TEST_CASE(operation_by_id_first_row)
{
    Project p;
    for (int id : {3, 5, 3}) {
        auto op = std::make_unique<Operation>();
        op->id = id;
        p.operations.push_back(std::move(op));
    }
    CHECK(operationById(p, 3) == p.operations[0].get());
    CHECK(operationById(p, 5) == p.operations[1].get());
    CHECK(operationById(p, 4) == nullptr);
}

TEST_CASE(cellpos_minus_one_coordinate_is_recomputed_through_parser)
{
    struct Case { const char* cell; int row; int col; };
    const Case cases[] = {
        {"", 0, 0},
        {"<CellPos Row=\"-1\" Col=\"0\"/>", 0, 0},
        {"<CellPos Row=\"0\" Col=\"-1\"/>", 0, 0},
        {"<CellPos Row=\"-1\" Col=\"-1\"/>", 0, 0},
        {"<CellPos Row=\"-1\" Col=\"5\"/>", 0, 0},
        {"<CellPos Row=\"5\" Col=\"-1\"/>", 0, 0},
        {"<CellPos Row=\"0\" Col=\"4\"/>", 0, 4},
    };
    for (const Case& c : cases) {
        Project p;
        p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project><DiagramItem Operation_Id=\"1\"><ClassName>SfOprForming2D</ClassName>"
                             "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>";
        source += c.cell;
        source += "</DiagramItem></Sub_Project>";
        xml::Document doc;
        String error;
        int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col));
        REQUIRE(doc.root != nullptr);
        Ctx ctx;
        parseSubProject(p, *doc.root, ctx);
        REQUIRE(p.subs[0]->flow.items.size() == 1u);
        CHECK_EQ(p.subs[0]->flow.items[0]->row, c.row);
        CHECK_EQ(p.subs[0]->flow.items[0]->col, c.col);
    }
}

TEST_CASE(activate_parses_every_corpus_flow_chart)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    int checked = 0;
    try {
        for (const p03test::Case& c : p03test::corpusCases(corpus)) {
            Project p;
            p.subs.push_back(makeSubProject(p, 0, 0));
            p.dom = std::make_shared<xml::Document>();
            String err;
            int line = 0, col = 0;
            REQUIRE(xml::setContent(*p.dom, p03test::readFile(c.moproj), &err, &line, &col));
            Ctx ctx;
            activateSubProject(p, 0, ctx);
            CHECK(p.subs[0]->loaded);
            CHECK(!p.loadingSub);
            CHECK_EQ(p.activeSub, 0);

            // same order as the test-side build of test_order.cpp, and as the saved MST
            p03test::Project px = p03test::load(c.moproj);
            SubProject ref;
            p03test::buildFlow(px, ref);
            const std::vector<int> got = mstOrder(*p.subs[0]);
            if (got != mstOrder(ref)) testkit::fail(__FILE__, __LINE__, c.name + ": parse differs from the test build");
            std::vector<std::string> names;
            for (int id : got) p03test::pushRun(names, px.names.count(id) ? px.names[id] : "?" + std::to_string(id));
            if (names != p03test::oprnamRuns(c.mst))
                testkit::fail(__FILE__, __LINE__, c.name + ": order differs: " + p03test::join(names));
            // activation again does not parse twice
            const size_t n = p.subs[0]->flow.items.size();
            activateSubProject(p, 0, ctx);
            CHECK_EQ(p.subs[0]->flow.items.size(), n);
            ++checked;
        }
    } catch (const NotPorted& e) {
        skip(e);
        return;
    }
    std::printf("corpus: %d projects parsed\n", checked);
    CHECK(checked >= 40);
}

int main()
{
    return testkit::run_all();
}
