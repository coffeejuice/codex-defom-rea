// P03: sub-project activation and parse (0x1400a7b90, 0x14008f530, 0x140091f00, 0x1400a78e0) on
// every corpus project, through the real XML seam. Until P01 (QDomDocument) and P04
// (QString::toInt/contains) merge, their stubs throw NotPorted and the cases report SKIP.
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <limits>

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

void checkGridChain(const SubProject& sub, const std::vector<std::size_t>& indices,
                    const std::vector<int>& ids)
{
    const auto& items = sub.flow.items;
    REQUIRE(indices.size() == items.size());
    for (std::size_t k = 0; k < indices.size(); ++k) {
        REQUIRE(indices[k] < items.size());
        const FlowItem& item = *items[indices[k]];
        CHECK(item.prev == (k == 0 ? nullptr : items[indices[k - 1]].get()));
        CHECK(item.next == (k + 1 == indices.size() ? nullptr : items[indices[k + 1]].get()));
    }
    CHECK(mstOrder(sub) == ids);
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
    struct Case { const char* cell; double x; double y; int row; int col; };
    const Case cases[] = {
        {"", 0, 0, 0, 0},
        {"<CellPos Row=\"-1\" Col=\"0\"/>", 410, 0, 0, 2},
        {"<CellPos Row=\"0\" Col=\"-1\"/>", 820, 0, 0, 4},
        {"<CellPos Row=\"-1\" Col=\"-1\"/>", 1230, 0, 0, 6},
        {"<CellPos Row=\"-1\" Col=\"5\"/>", 205, 0, 0, 1},
        {"<CellPos Row=\"5\" Col=\"-1\"/>", 1025, 0, 0, 5},
        {"<CellPos Row=\"0\" Col=\"4\"/>", 0, 0, 0, 4},
    };
    for (const Case& c : cases) {
        Project p;
        p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project><DiagramItem Operation_Id=\"1\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                             std::to_string(c.x) + "</x><y>" + std::to_string(c.y) + "</y><width>0</width><height>0</height></RectF>";
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

TEST_CASE(singleton_forming2d_cell_selection_relayouts_rectf)
{
    struct Sample { const char* x; const char* y; int col; };
    const Sample samples[] = {
        {"-61", "0", 0}, {"-60", "0", 0}, {"0", "0", 0},
        {"0", "-25.000000000000004", 0}, {"0", "-25", 0}, {"0", "-24.999999999999996", 0},
        {"102.49999999999999", "0", 0}, {"102.5", "0", 1}, {"102.50000000000001", "0", 1},
        {"307.49999999999994", "0", 1}, {"307.5", "0", 2}, {"307.50000000000006", "0", 2},
        {"512.49999999999994", "0", 2}, {"512.5", "0", 3}, {"512.5000000000001", "0", 3},
        {"717.4999999999999", "0", 3}, {"717.5", "0", 4}, {"717.5000000000001", "0", 4},
        {"922.4999999999999", "0", 4}, {"922.5", "0", 5}, {"922.5000000000001", "0", 5},
        {"1127.4999999999998", "0", 5}, {"1127.5", "0", 6}, {"1127.5000000000002", "0", 6},
        {"1332.4999999999998", "0", 6}};
    for (const Sample& sample : samples) for (const char* cell : {"", "<CellPos Row=\"-1\" Col=\"-1\"/>"}) {
        Project p; p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>") + sample.x + "</x><y>" + sample.y + "</y><width>0</width><height>0</height></RectF>" + cell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 1u);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, sample.col);
        CHECK_EQ(items[0]->rect[0], 205.0 * sample.col); CHECK_EQ(items[0]->rect[1], 0.0);
        CHECK_EQ(items[0]->rect[2], 0.0); CHECK_EQ(items[0]->rect[3], 0.0);
        CHECK(items[0]->prev == nullptr); CHECK(items[0]->next == nullptr);
        CHECK_EQ(items[0]->opId, 73); CHECK(mstOrder(*p.subs[0]) == std::vector<int>({73}));
    }
    // The upper y boundary is excluded; its immediate predecessor remains selected in row 0.
    Project p; p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source = "<Sub_Project><DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>0</x><y>67.49999999999999</y><width>0</width><height>0</height></RectF>"
        "</DiagramItem></Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; parseSubProject(p, *doc.root, ctx);
    REQUIRE(p.subs[0]->flow.items.size() == 1u);
    CHECK_EQ(p.subs[0]->flow.items[0]->rect[0], 0.0); CHECK_EQ(p.subs[0]->flow.items[0]->rect[1], 0.0);
    CHECK(mstOrder(*p.subs[0]) == std::vector<int>({73}));
}

TEST_CASE(singleton_partial_cellpos_relayouts_signed_rectf_dimensions)
{
    struct Case { const char* cell; const char* width; const char* height; const char* x; const char* y; int col; };
    const Case cases[] = {
        {"<CellPos Row=\"-1\" Col=\"0\"/>", "3.25", "-4.5", "205", "10", 1},
        {"<CellPos Row=\"0\" Col=\"0\"/><CellPos Row=\"-1\" Col=\"5\"/>", "3.25", "4.5", "102.49999999999999", "67.49999999999999", 0},
        {"<CellPos Row=\"-1\" Col=\"0\"/>", "3.25", "4.5", "102.5", "-25.000000000000004", 1},
        {"<CellPos Row=\"-1\" Col=\"5\"/>", "3.25", "4.5", "102.50000000000001", "-25", 1},
        {"<CellPos Row=\"-1\" Col=\"5\"/>", "-3.25", "4.5", "410", "20", 2},
        {"<CellPos Row=\"0\" Col=\"-1\"/>", "4.5", "-3.25", "615", "30", 3},
        {"<CellPos Row=\"0\" Col=\"-1\"/>", "-4.5", "0", "717.5", "30", 4},
        {"<CellPos Row=\"5\" Col=\"-1\"/>", "-4.5", "3.25", "820", "40", 4},
        {"<CellPos Row=\"-1\" Col=\"5\"/>", "-4.5", "-3.25", "922.5", "5", 5},
        {"<CellPos Row=\"0\" Col=\"-1\"/>", "3.25", "4.5", "-61", "-26", 0},
        {"<CellPos Row=\"-1\" Col=\"0\"/>", "4.9406564584124654e-324", "-1.7976931348623157e+308", "102.5", "0", 1},
        {"<CellPos Row=\"0\" Col=\"-1\"/>", "1.7976931348623157e+308", "-4.9406564584124654e-324", "1127.5", "0", 6},
    };
    for (const Case& sample : cases) {
        Project p; p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>") +
            sample.x + "</x><y>" + sample.y + "</y><width>" + sample.width + "</width><height>" + sample.height +
            "</height></RectF>" + sample.cell + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 1u);
        const FlowItem& item = *items[0];
        CHECK_EQ(item.row, 0); CHECK_EQ(item.col, sample.col);
        CHECK_EQ(item.rect[0], 205.0 * sample.col); CHECK_EQ(item.rect[1], 0.0);
        CHECK_BITS(item.rect[2], std::strtod(sample.width, nullptr));
        CHECK_BITS(item.rect[3], std::strtod(sample.height, nullptr));
        CHECK(item.prev == nullptr); CHECK(item.next == nullptr);
        CHECK_EQ(item.opId, 73); CHECK(mstOrder(*p.subs[0]) == std::vector<int>({73}));
    }
    // CellPos missing and fully sentinel remain accepted with nonzero signed dimensions.
    for (const char* cell : {"", "<CellPos Row=\"-1\" Col=\"-1\"/>"}) {
        Project p; p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>307.5</x><y>0</y><width>-0.0</width><height>1.25</height></RectF>" + cell + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 1u);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 2);
        CHECK_EQ(items[0]->rect[0], 410.0); CHECK_EQ(items[0]->rect[1], 0.0);
        CHECK_BITS(items[0]->rect[2], -0.0); CHECK_BITS(items[0]->rect[3], 1.25);
        CHECK(items[0]->prev == nullptr); CHECK(items[0]->next == nullptr);
        CHECK_EQ(items[0]->opId, 73); CHECK(mstOrder(*p.subs[0]) == std::vector<int>({73}));
    }
}

TEST_CASE(singleton_explicit_valid_cellpos_relayouts_rectf)
{
    struct Case { int col; const char* x; const char* y; const char* width; const char* height; };
    const Case cases[] = {
        {0,"1025","50","3.25","-4.5"}, {1,"0","20","-3.25","4.5"},
        {2,"1127.5","0","4.9406564584124654e-324","-1.7976931348623157e+308"},
        {3,"0","67.49999999999999","1.7976931348623157e+308","-4.9406564584124654e-324"},
        {4,"102.5","10","-0.0","0"}, {5,"0","0","-8","9"}, {6,"0","1","7","-6"}};
    for (const Case& sample : cases) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>") +
            sample.x + "</x><y>" + sample.y + "</y><width>" + sample.width + "</width><height>" + sample.height +
            "</height></RectF><CellPos Row=\"0\" Col=\"" + std::to_string(sample.col) + "\"/></DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 1u);
        const FlowItem& item = *items[0];
        CHECK_EQ(item.row, 0); CHECK_EQ(item.col, sample.col);
        CHECK_EQ(item.rect[0], 205.0 * sample.col); CHECK_EQ(item.rect[1], 0.0);
        CHECK_BITS(item.rect[2], std::strtod(sample.width, nullptr));
        CHECK_BITS(item.rect[3], std::strtod(sample.height, nullptr));
        CHECK(item.prev == nullptr); CHECK(item.next == nullptr);
        CHECK_EQ(item.opId, 73); CHECK(mstOrder(*p.subs[0]) == std::vector<int>({73}));
    }
    // The final duplicate CellPos controls the explicit selection.
    Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source = "<Sub_Project><DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>1025</x><y>0</y><width>-2</width><height>3</height></RectF>"
        "<CellPos Row=\"0\" Col=\"1\"/><CellPos Row=\"0\" Col=\"5\"/></DiagramItem></Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; parseSubProject(p, *doc.root, ctx);
    REQUIRE(p.subs[0]->flow.items.size() == 1u);
    CHECK_EQ(p.subs[0]->flow.items[0]->row, 0); CHECK_EQ(p.subs[0]->flow.items[0]->col, 5);
    CHECK_EQ(p.subs[0]->flow.items[0]->rect[0], 1025.0);
}

TEST_CASE(cellpos_minus_one_column_rounding_and_clamps)
{
    struct Case { double x; int col; };
    const Case cases[] = {
        {205.0 + 102.5 - 0.001, 1}, {205.0 + 102.5, 2}, {205.0 + 102.5 + 0.001, 2},
        {3.0 * 205.0 + 102.5 - 0.001, 3}, {3.0 * 205.0 + 102.5, 4}, {3.0 * 205.0 + 102.5 + 0.001, 4},
        {-60.001, 0},
    };
    for (const Case& c : cases) {
        Project p;
        p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project><DiagramItem Operation_Id=\"1\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                             std::to_string(c.x) + "</x><y>0</y><width>0</width><height>0</height></RectF>"
                             "<CellPos Row=\"-1\" Col=\"-1\"/></DiagramItem></Sub_Project>";
        xml::Document doc;
        String error;
        int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col));
        REQUIRE(doc.root != nullptr);
        Ctx ctx;
        parseSubProject(p, *doc.root, ctx);
        REQUIRE(p.subs[0]->flow.items.size() == 1u);
        CHECK_EQ(p.subs[0]->flow.items[0]->row, 0);
        CHECK_EQ(p.subs[0]->flow.items[0]->col, c.col);
    }
}

TEST_CASE(cellpos_minus_one_y_clamp_and_empty_chart_row_boundary)
{
    for (double y : {-25.001, 0.0, 67.499}) {
        Project p;
        p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project><DiagramItem Operation_Id=\"1\"><ClassName>SfOprForming2D</ClassName><RectF><x>0</x><y>" +
                             std::to_string(y) + "</y><width>0</width><height>0</height></RectF>"
                             "<CellPos Row=\"-1\" Col=\"-1\"/></DiagramItem></Sub_Project>";
        xml::Document doc;
        String error;
        int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col));
        REQUIRE(doc.root != nullptr);
        Ctx ctx;
        parseSubProject(p, *doc.root, ctx);
        REQUIRE(p.subs[0]->flow.items.size() == 1u);
        CHECK_EQ(p.subs[0]->flow.items[0]->row, 0);
        CHECK_EQ(p.subs[0]->flow.items[0]->col, 0);
    }
    // Native row lookup selects row 1 at 67.5; this converter subset rejects placement outside row 0.
    Project p;
    p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source = "<Sub_Project><DiagramItem Operation_Id=\"1\"><ClassName>SfOprForming2D</ClassName>"
                               "<RectF><x>0</x><y>67.5</y><width>0</width><height>0</height></RectF>"
                               "<CellPos Row=\"-1\" Col=\"-1\"/></DiagramItem></Sub_Project>";
    xml::Document doc;
    String error;
    int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col));
    REQUIRE(doc.root != nullptr);
    Ctx ctx;
    bool row_guard = false;
    try { parseSubProject(p, *doc.root, ctx); }
    catch (const Error& e) {
        row_guard = e.code == M2M_E_UNSUPPORTED &&
                    std::string(e.what()).find("outside row 0 / the 7 columns") != std::string::npos;
    }
    CHECK(row_guard);
}

TEST_CASE(two_cellless_zero_rect_items_use_native_adjacent_cell)
{
    const char* cases[][2] = {{"", ""},
                              {"<CellPos Row=\"-1\" Col=\"-1\"/>", "<CellPos Row=\"-1\" Col=\"-1\"/>"},
                              {"", "<CellPos Row=\"-1\" Col=\"-1\"/>"}};
    for (const auto& cells : cases) {
        Project p;
        p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int id = 1; id <= 2; ++id) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) + "\"><ClassName>SfOprForming2D</ClassName>"
                      "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>";
            source += cells[id - 1];
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc;
        String error;
        int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col));
        REQUIRE(doc.root != nullptr);
        Ctx ctx;
        parseSubProject(p, *doc.root, ctx);
        REQUIRE(p.subs[0]->flow.items.size() == 2u);
        const FlowItem& a = *p.subs[0]->flow.items[0];
        const FlowItem& b = *p.subs[0]->flow.items[1];
        CHECK_EQ(a.row, 0); CHECK_EQ(a.col, 0);
        CHECK_EQ(b.row, 0); CHECK_EQ(b.col, 1);
        CHECK_EQ(a.rect[0], 0.0); CHECK_EQ(a.rect[1], 0.0); CHECK_EQ(a.rect[2], 0.0); CHECK_EQ(a.rect[3], 0.0);
        CHECK_EQ(b.rect[0], 205.0); CHECK_EQ(b.rect[1], 0.0); CHECK_EQ(b.rect[2], 0.0); CHECK_EQ(b.rect[3], 0.0);
        checkGridChain(*p.subs[0], {0, 1}, {1, 2});
        CHECK_EQ(a.opId, 1); CHECK_EQ(b.opId, 2);
    }
}

TEST_CASE(two_cellless_occupied_cell_other_patterns_stay_guarded)
{
    struct Case { const char* first; const char* second; };
    const Case cases[] = {{"<CellPos Row=\"0\" Col=\"0\"/>", ""},
                          {"<CellPos Row=\"-1\" Col=\"0\"/>", ""},
                          {"", "<CellPos Row=\"-1\" Col=\"0\"/>"}};
    for (const Case& c : cases) {
        Project p;
        p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int id = 1; id <= 2; ++id) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) + "\"><ClassName>SfOprForming2D</ClassName>"
                      "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>";
            source += id == 1 ? c.first : c.second;
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col));
        REQUIRE(doc.root != nullptr);
        Ctx ctx;
        bool occupied_guard = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) {
            occupied_guard = e.code == M2M_E_UNSUPPORTED &&
                             std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos;
        }
        CHECK(occupied_guard);
    }
}

TEST_CASE(two_cellless_adjacent_placement_preserves_parsed_link_and_order)
{
    Project p;
    makeRegistry(p);
    p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source =
        "<Sub_Project><DiagramItem Operation_Id=\"20\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>"
        "<Connectors><Connections><ItemIndex>1</ItemIndex></Connections></Connectors></DiagramItem>"
        "<DiagramItem Operation_Id=\"3\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>"
        "<Connectors><Connections><ItemIndex>-1</ItemIndex></Connections></Connectors></DiagramItem></Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col));
    REQUIRE(doc.root != nullptr);
    Ctx ctx;
    parseSubProject(p, *doc.root, ctx);
    const auto& items = p.subs[0]->flow.items;
    REQUIRE(items.size() == 2u);
    CHECK_EQ(items[0]->opId, 20); CHECK_EQ(items[1]->opId, 3);
    CHECK(mstOrder(*p.subs[0]) == std::vector<int>({20, 3}));
    CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, 1);
    CHECK_EQ(items[1]->rect[0], 205.0); CHECK_EQ(items[1]->rect[1], 0.0);
    CHECK_EQ(items[1]->rect[2], 0.0); CHECK_EQ(items[1]->rect[3], 0.0);
    CHECK(items[0]->next == items[1].get()); CHECK(items[0]->prev == nullptr);
    CHECK(items[1]->prev == items[0].get()); CHECK(items[1]->next == nullptr);
}

TEST_CASE(two_cellless_reverse_self_and_cycle_links_remain_guarded)
{
    struct Case { int aNext; int bNext; };
    const Case cases[] = {{-1, 0}, {0, -1}, {-1, 1}, {1, 0}};
    for (const Case& c : cases) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < 2; ++i) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(20 - i * 17) + "\"><ClassName>SfOprForming2D</ClassName>"
                      "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>"
                      "<Connectors><Connections><ItemIndex>" + std::to_string(i == 0 ? c.aNext : c.bNext) +
                      "</ItemIndex></Connections></Connectors></DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool refused = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) {
            refused = e.code == M2M_E_UNSUPPORTED &&
                      std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos;
        }
        CHECK(refused);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == 2u);
        CHECK(items[0]->next == (c.aNext < 0 ? nullptr : items[c.aNext].get()));
        CHECK(items[1]->next == (c.bNext < 0 ? nullptr : items[c.bNext].get()));
        CHECK(items[0]->prev == (c.aNext == 0 ? items[0].get() : c.bNext == 0 ? items[1].get() : nullptr));
        CHECK(items[1]->prev == (c.bNext == 1 ? items[1].get() : c.aNext == 1 ? items[0].get() : nullptr));
    }
}

TEST_CASE(two_cellless_negative_left_collision_stays_guarded)
{
    for (int count : {2}) {
        Project p;
        p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < count; ++i) {
            const int x = count == 2 && i == 1 ? -1 : 0;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(i + 1) + "\"><ClassName>SfOprForming2D</ClassName>"
                      "<RectF><x>" + std::to_string(x) + "</x><y>0</y><width>0</width><height>0</height></RectF>"
                      "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col));
        REQUIRE(doc.root != nullptr);
        Ctx ctx;
        bool occupied_guard = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) {
            occupied_guard = e.code == M2M_E_UNSUPPORTED &&
                             std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos;
        }
        CHECK(occupied_guard);
    }
}

TEST_CASE(two_item_left_one_nearby_profiles_do_not_enter_cp495_route)
{
    struct Case { const char* name; int ax; int ay; int bx; int by; int bw; int bh; const char* aCell; const char* bCell; int aNext; int bNext; const char* bClass; };
    const Case cases[] = {
        {"reversed origin", 1, 0, 0, 0, 0, 0, "", "", -1, -1, "SfOprForming2D"},
        {"nonzero width", 0, 0, 1, 0, 1, 0, "", "", -1, -1, "SfOprForming2D"},
        {"nonzero height", 0, 0, 1, 0, 0, 1, "", "", -1, -1, "SfOprForming2D"},
        {"forward linked", 0, 0, 1, 0, 0, 0, "", "", 1, -1, "SfOprForming2D"},
        {"reverse linked", 0, 0, 1, 0, 0, 0, "", "", -1, 0, "SfOprForming2D"},
        {"self linked", 0, 0, 1, 0, 0, 0, "", "", 0, -1, "SfOprForming2D"},
        {"valid A cell", 0, 0, 1, 0, 0, 0, "<CellPos Row=\"0\" Col=\"0\"/>", "", -1, -1, "SfOprForming2D"},
        {"other class", 0, 0, 1, 0, 0, 0, "", "", -1, -1, "SfOprForming"},
    };
    for (const Case& c : cases) {
        // CP501 accepts finite signed dimensions; selected-result coverage is below.
        if (std::string(c.name) == "nonzero width" || std::string(c.name) == "nonzero height") continue;
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < 2; ++i) {
            const int id = i + 1;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) + "\"><ClassName>" +
                      std::string(i == 1 ? c.bClass : "SfOprForming2D") + "</ClassName><RectF><x>" +
                      std::to_string(i == 0 ? c.ax : c.bx) + "</x><y>" + std::to_string(i == 0 ? c.ay : c.by) +
                      "</y><width>" + std::to_string(i == 0 ? 0 : c.bw) + "</width><height>" +
                      std::to_string(i == 0 ? 0 : c.bh) + "</height></RectF>";
            source += i == 0 ? c.aCell : c.bCell;
            const int next = i == 0 ? c.aNext : c.bNext;
            if (next >= 0) source += "<Connectors><Connections><ItemIndex>" + std::to_string(next) +
                                    "</ItemIndex></Connections></Connectors>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool refused = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) {
            refused = e.code == M2M_E_UNSUPPORTED &&
                      std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos;
        }
        CHECK(refused);
    }
}

TEST_CASE(two_item_left_one_collision_does_not_admit_prepopulated_chart)
{
    Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
    auto existing = std::make_unique<FlowItem>();
    existing->className = L("SfOprForming2D");
    existing->row = 0; existing->col = 0;
    p.subs[0]->flow.items.push_back(std::move(existing));
    const std::string source =
        "<Sub_Project Active=\"1\"><DiagramItem/><DiagramItem Operation_Id=\"2\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>1</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem></Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; bool refused = false;
    try { parseSubProject(p, *doc.root, ctx); }
    catch (const Error& e) {
        refused = e.code == M2M_E_UNSUPPORTED &&
                  std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos;
    }
    CHECK(refused);
    REQUIRE(p.subs[0]->flow.items.size() == 2u);
    CHECK_EQ(p.subs[0]->flow.items[1]->rect[0], 1.0);
}

TEST_CASE(two_cellless_left_one_collision_uses_native_adjacent_cell_and_grid_graph)
{
    const char* cases[][2] = {{"", ""},
                              {"<CellPos Row=\"-1\" Col=\"-1\"/>", "<CellPos Row=\"-1\" Col=\"-1\"/>"},
                              {"", "<CellPos Row=\"-1\" Col=\"-1\"/>"}};
    for (const auto& cells : cases) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int id = 1; id <= 2; ++id) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) + "\"><ClassName>SfOprForming2D</ClassName>"
                      "<RectF><x>" + std::string(id == 1 ? "0" : "1") + "</x><y>0</y><width>0</width><height>0</height></RectF>";
            source += cells[id - 1]; source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, 1);
        CHECK_EQ(items[0]->rect[0], 0.0); CHECK_EQ(items[0]->rect[1], 0.0);
        CHECK_EQ(items[0]->rect[2], 0.0); CHECK_EQ(items[0]->rect[3], 0.0);
        CHECK_EQ(items[1]->rect[0], 205.0); CHECK_EQ(items[1]->rect[1], 0.0);
        CHECK_EQ(items[1]->rect[2], 0.0); CHECK_EQ(items[1]->rect[3], 0.0);
        CHECK(items[0]->prev == nullptr); CHECK(items[0]->next == items[1].get());
        CHECK(items[1]->prev == items[0].get()); CHECK(items[1]->next == nullptr);
        CHECK(mstOrder(*p.subs[0]) == std::vector<int>({1, 2}));
        CHECK_EQ(items[0]->opId, 1); CHECK_EQ(items[1]->opId, 2);
    }
}

TEST_CASE(two_cellless_small_positive_left_collision_interval_is_selected)
{
    for (const char* x : {"1e-12", "1", "102.49999999999999"}) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source =
            "<Sub_Project><DiagramItem Operation_Id=\"1\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"2\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>" + std::string(x) + "</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        REQUIRE(p.subs[0]->flow.items.size() == 2u);
        CHECK_EQ(p.subs[0]->flow.items[1]->col, 1);
        CHECK_EQ(p.subs[0]->flow.items[1]->rect[0], 205.0);
        CHECK_EQ(p.subs[0]->flow.items[1]->rect[1], 0.0);
        CHECK_EQ(p.subs[0]->flow.items[1]->rect[2], 0.0);
        CHECK_EQ(p.subs[0]->flow.items[1]->rect[3], 0.0);
        CHECK_EQ(p.subs[0]->flow.items[0]->row, 0);
        CHECK_EQ(p.subs[0]->flow.items[0]->col, 0);
        CHECK_EQ(p.subs[0]->flow.items[0]->rect[0], 0.0);
        CHECK_EQ(p.subs[0]->flow.items[0]->rect[1], 0.0);
        CHECK_EQ(p.subs[0]->flow.items[0]->rect[2], 0.0);
        CHECK_EQ(p.subs[0]->flow.items[0]->rect[3], 0.0);
        checkGridChain(*p.subs[0], {0, 1}, {1, 2});
        CHECK_EQ(p.subs[0]->flow.items[0]->opId, 1);
        CHECK_EQ(p.subs[0]->flow.items[1]->opId, 2);
        CHECK(mstOrder(*p.subs[0]) == std::vector<int>({1, 2}));
    }
}

TEST_CASE(two_cellless_free_column_pair_relayouts_half_up_interval)
{
    const char* values[] = {"102.5", "103", "204.99999999999997", "205",
                            "205.00000000000003", "307.49999999999994"};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"5\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"-2147483648\"/>", "<CellPos Row=\"2147483647\" Col=\"-1\"/>",
        "<CellPos Row=\"0\" Col=\"0\"/><CellPos Row=\"-1\" Col=\"5\"/>"};
    for (const char* x : values) for (const char* aCell : cells) for (const char* bCell : cells) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source =
            "<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + std::string(aCell) + "</DiagramItem>"
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
            std::string(x) + "</x><y>0</y><width>0</width><height>0</height></RectF>" + bCell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, 1);
        for (int j = 0; j < 4; ++j) CHECK_EQ(items[0]->rect[j], 0.0);
        CHECK_EQ(items[1]->rect[0], 205.0); CHECK_EQ(items[1]->rect[1], 0.0);
        CHECK_EQ(items[1]->rect[2], 0.0); CHECK_EQ(items[1]->rect[3], 0.0);
        CHECK(items[0]->prev == nullptr); CHECK(items[0]->next == items[1].get());
        CHECK(items[1]->prev == items[0].get()); CHECK(items[1]->next == nullptr);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK(mstOrder(*p.subs[0]) == std::vector<int>({41, 73}));
    }
    // At the exact upper endpoint native half-up selects free column 2; this is
    // an exclusion control, not an occupied-cell refusal.
    Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source =
        "<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
        "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>307.5</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem></Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; parseSubProject(p, *doc.root, ctx);
    const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
    CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, 2);
    CHECK(items[0]->next == nullptr); CHECK(items[1]->prev == nullptr);
    CHECK(mstOrder(*p.subs[0]) == std::vector<int>({41}));
}

TEST_CASE(two_cellless_free_column_gap_pair_relayouts_columns_two_through_six)
{
    struct Sample { const char* x; int col; };
    const Sample samples[] = {
        {"307.5", 2}, {"307.50000000000006", 2}, {"410", 2},
        {"512.49999999999994", 2}, {"512.5", 3}, {"512.5000000000001", 3}, {"615", 3},
        {"717.4999999999999", 3}, {"717.5", 4}, {"717.5000000000001", 4}, {"820", 4},
        {"922.4999999999999", 4}, {"922.5", 5}, {"922.5000000000001", 5}, {"1025", 5},
        {"1127.4999999999998", 5}, {"1127.5", 6}, {"1127.5000000000002", 6}, {"1230", 6},
        {"1332.4999999999998", 6}};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"5\" Col=\"-1\"/>",
        "<CellPos Row=\"0\" Col=\"0\"/><CellPos Row=\"-1\" Col=\"5\"/>"};
    for (const Sample& sample : samples) for (const char* aCell : cells) for (const char* bCell : cells) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source =
            "<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + std::string(aCell) + "</DiagramItem>"
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
            sample.x + "</x><y>0</y><width>0</width><height>0</height></RectF>" + bCell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, sample.col);
        for (int j = 0; j < 4; ++j) CHECK_EQ(items[0]->rect[j], 0.0);
        CHECK_EQ(items[1]->rect[0], 205.0 * sample.col);
        CHECK_EQ(items[1]->rect[1], 0.0); CHECK_EQ(items[1]->rect[2], 0.0); CHECK_EQ(items[1]->rect[3], 0.0);
        CHECK(items[0]->prev == nullptr); CHECK(items[0]->next == nullptr);
        CHECK(items[1]->prev == nullptr); CHECK(items[1]->next == nullptr);
        CHECK(mstOrder(*p.subs[0]) == std::vector<int>({41}));
    }
}


TEST_CASE(three_cellless_zero_rect_items_use_native_recursive_shift)
{
    const char* cell[] = {"", "<CellPos Row=\"-1\" Col=\"-1\"/>", ""};
    Project p;
    makeRegistry(p);
    p.subs.push_back(makeSubProject(p, 0, 0));
    std::string source = "<Sub_Project>";
    for (int i = 0; i < 3; ++i) {
        source += "<DiagramItem Operation_Id=\"" + std::to_string(30 - i * 7) + "\"><ClassName>SfOprForming2D</ClassName>"
                  "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>";
        source += cell[i];
        source += "</DiagramItem>";
    }
    source += "</Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; parseSubProject(p, *doc.root, ctx);
    const auto& items = p.subs[0]->flow.items;
    REQUIRE(items.size() == 3u);
    CHECK_EQ(items[0]->opId, 30); CHECK_EQ(items[1]->opId, 23); CHECK_EQ(items[2]->opId, 16);
    CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
    CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, 2);
    CHECK_EQ(items[2]->row, 0); CHECK_EQ(items[2]->col, 1);
    const double lefts[] = {0.0, 410.0, 205.0};
    for (std::size_t i = 0; i < items.size(); ++i) {
        CHECK_EQ(items[i]->rect[0], lefts[i]);
        CHECK_EQ(items[i]->rect[1], 0.0); CHECK_EQ(items[i]->rect[2], 0.0); CHECK_EQ(items[i]->rect[3], 0.0);
    }
    checkGridChain(*p.subs[0], {0, 2, 1}, {30, 16, 23});
}

TEST_CASE(four_through_seven_cellless_items_follow_native_seven_column_recurrence)
{
    for (int count : {4, 5, 6, 7}) {
        const std::vector<std::vector<std::size_t>> chainIndices = {
            {0, 3, 2, 1}, {0, 4, 3, 2, 1}, {0, 5, 4, 3, 2, 1}, {0, 6, 5, 4, 3, 2, 1}};
        const std::vector<std::vector<int>> chainIds = {
            {100, 103, 102, 101}, {100, 104, 103, 102, 101},
            {100, 105, 104, 103, 102, 101}, {100, 106, 105, 104, 103, 102, 101}};
        const int patterns = count == 4 ? 3 : 1;
        for (int pattern = 0; pattern < patterns; ++pattern) {
            Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
            std::string source = "<Sub_Project>";
            for (int i = 0; i < count; ++i) {
                source += "<DiagramItem Operation_Id=\"" + std::to_string(100 + i) + "\"><ClassName>SfOprForming2D</ClassName>"
                          "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>";
                const bool sentinel = pattern == 0 ? false : pattern == 1 ? true : (i == 1 || i == count - 1);
                if (sentinel) source += "<CellPos Row=\"-1\" Col=\"-1\"/>";
                source += "</DiagramItem>";
            }
            source += "</Sub_Project>";
            xml::Document doc; String error; int line = 0, col = 0;
            REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
            Ctx ctx; parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items;
            REQUIRE(items.size() == static_cast<std::size_t>(count));
            for (int j = 0; j < count; ++j) {
                CHECK_EQ(items[j]->opId, 100 + j);
                CHECK_EQ(items[j]->row, 0);
                CHECK_EQ(items[j]->col, j == 0 ? 0 : count - j);
                CHECK_EQ(items[j]->rect[0], j == 0 ? 0.0 : 205.0 * (count - j));
                CHECK_EQ(items[j]->rect[1], 0.0); CHECK_EQ(items[j]->rect[2], 0.0); CHECK_EQ(items[j]->rect[3], 0.0);
            }
            checkGridChain(*p.subs[0], chainIndices[count - 4], chainIds[count - 4]);
        }
    }
}

TEST_CASE(eight_negative_id_cellless_items_wrap_to_native_second_row)
{
    Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
    std::string source = "<Sub_Project>";
    for (int i = 0; i < 8; ++i) {
        source += "<DiagramItem Operation_Id=\"" + std::to_string(-10 - i) + "\"><ClassName>SfOprForming2D</ClassName>"
                  "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>";
        if (i % 2) source += "<CellPos Row=\"-1\" Col=\"-1\"/>";
        source += "</DiagramItem>";
    }
    source += "</Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; parseSubProject(p, *doc.root, ctx);
    const auto& items = p.subs[0]->flow.items;
    REQUIRE(items.size() == 8u);
    const int rows[] = {0,1,0,0,0,0,0,0};
    const int cols[] = {0,0,6,5,4,3,2,1};
    const double xs[] = {0,0,1230,1025,820,615,410,205};
    for (std::size_t i=0;i<items.size();++i) {
        CHECK_EQ(items[i]->opId, -10-static_cast<int>(i)); CHECK_EQ(items[i]->row, rows[i]); CHECK_EQ(items[i]->col, cols[i]);
        CHECK_EQ(items[i]->rect[0], xs[i]); CHECK_EQ(items[i]->rect[1], i==1 ? 215.0 : 0.0);
        CHECK_EQ(items[i]->rect[2], 0.0); CHECK_EQ(items[i]->rect[3], 0.0);
    }
    checkGridChain(*p.subs[0], {0, 7, 6, 5, 4, 3, 2, 1}, {-10, -17, -16, -15, -14, -13, -12, -11});
}

TEST_CASE(eight_positive_or_mixed_and_nine_negative_items_remain_guarded)
{
    for (int count : {8, 9}) {
        for (int mode : {0, 1, 2, 3}) {
            if (count == 8 && mode == 3) continue;
            Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
            std::string source = "<Sub_Project>";
            for (int i=0;i<count;++i) {
                int id = mode == 0 ? i + 1 : -10 - i;
                if (mode == 1 && i == count - 1) id = 10;
                if (mode == 2 && i == count - 1) id = 0;
                source += "<DiagramItem Operation_Id=\"" + std::to_string(id) + "\"><ClassName>SfOprForming2D</ClassName>"
                          "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>";
            }
            source += "</Sub_Project>";
            xml::Document doc; String error; int line=0,col=0;
            REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
            Ctx ctx; bool guarded=false;
            try { parseSubProject(p, *doc.root, ctx); }
            catch (const Error& e) {
                guarded = e.code == M2M_E_UNSUPPORTED &&
                          std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos;
            }
            CHECK(guarded);
        }
    }
}

TEST_CASE(eight_cellless_items_keep_seven_column_guard)
{
    Project p; p.subs.push_back(makeSubProject(p, 0, 0));
    std::string source = "<Sub_Project>";
    for (int i = 0; i < 8; ++i)
        source += "<DiagramItem Operation_Id=\"" + std::to_string(i + 1) + "\"><ClassName>SfOprForming2D</ClassName>"
                  "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>";
    source += "</Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; bool refused = false;
    try { parseSubProject(p, *doc.root, ctx); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED &&
        std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos; }
    CHECK(refused);
}

TEST_CASE(two_item_signed_rect_dimensions_survive_native_cell_placement)
{
    struct Sample { const char* x; const char* w; const char* h; int col; bool linked; };
    const Sample samples[] = {
        {"1", "1", "1", 1, true},
        {"102.49999999999999", "-1", "-1", 1, true},
        {"102.5", "-0", "-1", 1, true},
        {"307.49999999999994", "0", "0", 1, true},
        {"307.49999999999994", "4.9406564584124654e-324", "-4.9406564584124654e-324", 1, true},
        {"307.5", "-1.25", "1.7976931348623157e308", 2, false},
        {"512.5", "-1.7976931348623157e308", "2.2250738585072014e-308", 3, false},
        {"717.5", "1", "-1", 4, false},
        {"922.5", "0", "-0", 5, false},
        {"1127.5", "-0", "0", 6, false},
        {"1332.4999999999998", "1.7976931348623157e308", "-0", 6, false},
    };
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"5\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"-2147483648\"/>", "<CellPos Row=\"2147483647\" Col=\"-1\"/>",
        "<CellPos Row=\"0\" Col=\"0\"/><CellPos Row=\"-1\" Col=\"5\"/>"};
    for (const Sample& sample : samples) for (const char* aCell : cells) for (const char* bCell : cells) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + aCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + sample.x +
            "</x><y>0</y><width>" + sample.w + "</width><height>" + sample.h + "</height></RectF>" + bCell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, sample.col);
        CHECK_EQ(items[1]->rect[0], 205.0 * sample.col); CHECK_EQ(items[1]->rect[1], 0.0);
        double expectedW = std::strtod(sample.w, nullptr), expectedH = std::strtod(sample.h, nullptr);
        std::uint64_t expectedWBits, expectedHBits, actualWBits, actualHBits;
        std::memcpy(&expectedWBits, &expectedW, sizeof expectedWBits);
        std::memcpy(&expectedHBits, &expectedH, sizeof expectedHBits);
        std::memcpy(&actualWBits, &items[1]->rect[2], sizeof actualWBits);
        std::memcpy(&actualHBits, &items[1]->rect[3], sizeof actualHBits);
        CHECK_EQ(actualWBits, expectedWBits); CHECK_EQ(actualHBits, expectedHBits);
        for (int j = 0; j < 4; ++j) CHECK_EQ(items[0]->rect[j], 0.0);
        CHECK(items[0]->next == (sample.linked ? items[1].get() : nullptr));
        CHECK(items[1]->prev == (sample.linked ? items[0].get() : nullptr));
        CHECK(items[1]->next == nullptr); CHECK(items[0]->prev == nullptr);
        CHECK(mstOrder(*p.subs[0]) == (sample.linked ? std::vector<int>({41, 73}) : std::vector<int>({41})));
    }
}

TEST_CASE(two_item_signed_dimension_route_exclusion_controls)
{
    struct Control { const char* name; const char* aw; const char* ah; const char* bw; const char* bh;
                     const char* bCell; const char* bx; };
    const Control controls[] = {
        {"nonfinite width", "0", "0", "nan", "0", "", "1"},
        {"nonfinite height", "0", "0", "0", "inf", "", "1"},
        {"nonfinite A width", "nan", "0", "0", "0", "", "1"},
        {"nonfinite A height", "0", "inf", "0", "0", "", "1"},
        {"exact upper x endpoint", "0", "0", "0", "0", "", "1332.5"},
    };
    for (const Control& c : controls) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>" + c.aw + "</width><height>" + c.ah + "</height></RectF></DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + c.bx +
            "</x><y>0</y><width>" + c.bw + "</width><height>" + c.bh + "</height></RectF>" + c.bCell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool excluded = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) { excluded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(excluded);
    }
}

TEST_CASE(two_item_partial_cell_route_keeps_final_non_sentinel_A_outside_selection)
{
    const char* excludedCells[] = {
        "<CellPos Row=\"-1\" Col=\"5\"/><CellPos Row=\"0\" Col=\"0\"/>",
        "<CellPos Row=\"-2\" Col=\"0\"/>"};
    for (const char* aCell : excludedCells) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + aCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>" +
            "<RectF><x>1</x><y>0</y><width>2</width><height>-3</height></RectF></DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool excluded = false;
        try {
            parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items;
            REQUIRE(items.size() == 2u);
            excluded = !(items[1]->row == 0 && items[1]->col == 1 && items[1]->rect[0] == 205.0 &&
                         items[0]->next == items[1].get() && items[1]->prev == items[0].get());
        } catch (const Error& e) { excluded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(excluded);
    }
}

TEST_CASE(two_item_nonzero_top_relayouts_with_populated_row_zero)
{
    struct XSample { const char* x; int col; bool linked; };
    const XSample xs[] = {{"1", 1, true}, {"102.49999999999999", 1, true},
        {"102.5", 1, true}, {"307.49999999999994", 1, true},
        {"307.5", 2, false}, {"512.49999999999994", 2, false},
        {"512.5", 3, false}, {"717.4999999999999", 3, false},
        {"717.5", 4, false}, {"922.4999999999999", 4, false},
        {"922.5", 5, false}, {"1127.4999999999998", 5, false},
        {"1127.5", 6, false}, {"1332.4999999999998", 6, false}};
    const char* ys[] = {"-1.7976931348623157e308", "-4.9406564584124654e-324",
                        "-25.000000000000004", "-25", "-24.999999999999996",
                        "67.49999999999999", "67.5", "67.50000000000001",
                        "100", "159.99999999999997"};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"5\" Col=\"-1\"/>"};
    for (const XSample& sample : xs) for (const char* y : ys)
        for (const char* aCell : cells) for (const char* bCell : cells) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + aCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + sample.x +
            "</x><y>" + y + "</y><width>2.5</width><height>-3.25</height></RectF>" + bCell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, sample.col);
        for (int i = 0; i < 4; ++i) CHECK_EQ(items[0]->rect[i], 0.0);
        CHECK_EQ(items[1]->rect[0], 205.0 * sample.col); CHECK_BITS(items[1]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[2], 2.5); CHECK_BITS(items[1]->rect[3], -3.25);
        CHECK(items[0]->prev == nullptr); CHECK(items[1]->next == nullptr);
        CHECK(items[0]->next == (sample.linked ? items[1].get() : nullptr));
        CHECK(items[1]->prev == (sample.linked ? items[0].get() : nullptr));
        CHECK(mstOrder(*p.subs[0]) == (sample.linked ? std::vector<int>({41, 73}) : std::vector<int>({41})));
    }
    // The positive-ID template proves only y<189; its exact edge remains guarded.
    Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source = "<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
        "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>1</x><y>189</y><width>2.5</width><height>-3.25</height></RectF></DiagramItem></Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; bool guarded = false;
    try { parseSubProject(p, *doc.root, ctx); }
    catch (const Error& e) { guarded = e.code == M2M_E_UNSUPPORTED; }
    CHECK(guarded);
    for (const char* invalidY : {"nan", "inf", "-inf"}) {
        Project invalid; makeRegistry(invalid); invalid.subs.push_back(makeSubProject(invalid, 0, 0));
        const std::string invalidSource = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>1</x><y>" +
            invalidY + "</y><width>2.5</width><height>-3.25</height></RectF></DiagramItem></Sub_Project>";
        xml::Document invalidDoc; String invalidError; int invalidLine = 0, invalidCol = 0;
        REQUIRE(xml::setContent(invalidDoc, invalidSource, &invalidError, &invalidLine, &invalidCol));
        REQUIRE(invalidDoc.root != nullptr);
        Ctx invalidCtx; bool invalidGuarded = false;
        try { parseSubProject(invalid, *invalidDoc.root, invalidCtx); }
        catch (const Error& e) { invalidGuarded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(invalidGuarded);
    }
}

TEST_CASE(two_item_distinct_explicit_cells_relayout_in_grid_order)
{
    struct BSample { const char* x; const char* y; const char* w; const char* h; };
    const BSample samples[] = {
        {"-61", "-26", "-0", "1.25"},
        {"102.5", "67.49999999999999", "4.9406564584124654e-324", "-1.7976931348623157e308"},
        {"1230", "0", "-3.5", "2.5"}};
    for (const BSample& sample : samples) for (int aCol = 0; aCol < 7; ++aCol)
        for (int bCol = 0; bCol < 7; ++bCol) {
        if (aCol == bCol) continue;
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" +
            "<CellPos Row=\"0\" Col=\"" + std::to_string(aCol) + "\"/></DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
            sample.x + "</x><y>" + sample.y + "</y><width>" + sample.w + "</width><height>" + sample.h +
            "</height></RectF><CellPos Row=\"0\" Col=\"" + std::to_string(bCol) + "\"/></DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, aCol);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, bCol);
        CHECK_EQ(items[0]->rect[0], 205.0 * aCol); CHECK_BITS(items[0]->rect[1], 0.0);
        CHECK_BITS(items[0]->rect[2], 0.0); CHECK_BITS(items[0]->rect[3], 0.0);
        CHECK_EQ(items[1]->rect[0], 205.0 * bCol); CHECK_BITS(items[1]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[2], std::strtod(sample.w, nullptr));
        CHECK_BITS(items[1]->rect[3], std::strtod(sample.h, nullptr));
        const int lo = aCol < bCol ? 0 : 1, hi = 1 - lo;
        const bool adjacent = aCol + 1 == bCol || bCol + 1 == aCol;
        CHECK(items[lo]->prev == nullptr); CHECK(items[hi]->next == nullptr);
        CHECK(items[lo]->next == (adjacent ? items[hi].get() : nullptr));
        CHECK(items[hi]->prev == (adjacent ? items[lo].get() : nullptr));
        const std::vector<int> expected = adjacent ?
            std::vector<int>({items[lo]->opId, items[hi]->opId}) : std::vector<int>({items[lo]->opId});
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(two_item_explicit_cells_use_final_duplicate_occurrence)
{
    Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source = "<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>"
        "<CellPos Row=\"0\" Col=\"6\"/><CellPos Row=\"0\" Col=\"4\"/></DiagramItem>"
        "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>1</x><y>0</y><width>-2</width><height>3</height></RectF>"
        "<CellPos Row=\"0\" Col=\"0\"/><CellPos Row=\"0\" Col=\"2\"/></DiagramItem></Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; parseSubProject(p, *doc.root, ctx);
    const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
    CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 4);
    CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, 2);
    CHECK_EQ(items[0]->rect[0], 820.0); CHECK_BITS(items[0]->rect[1], 0.0);
    CHECK_EQ(items[1]->rect[0], 410.0); CHECK_BITS(items[1]->rect[1], 0.0);
    CHECK_BITS(items[1]->rect[2], -2.0); CHECK_BITS(items[1]->rect[3], 3.0);
    CHECK(items[0]->prev == nullptr); CHECK(items[0]->next == nullptr);
    CHECK(items[1]->prev == nullptr); CHECK(items[1]->next == nullptr);
    CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
    CHECK(mstOrder(*p.subs[0]) == std::vector<int>({73}));
}

TEST_CASE(two_item_explicit_cell_selection_keeps_other_profiles_guarded)
{
    struct Control { const char* aCell; const char* bCell; const char* bClass;
                     const char* bY; const char* bW; bool linked; double selectedX; };
    const Control controls[] = {
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"0\"/>", "SfOprForming2D", "0", "2", false, 0.0},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"-1\" Col=\"1\"/>", "SfOprForming2D", "0", "2", false, 205.0},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"7\"/>", "SfOprForming2D", "0", "2", false, 1435.0},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "189", "2", false, 205.0},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "0", "nan", false, 205.0},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming", "0", "2", false, 205.0},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "0", "2", true, 205.0}};
    for (const Control& c : controls) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + c.aCell +
            (c.linked ? "<Connectors><Connections><ItemIndex>1</ItemIndex></Connections></Connectors>" : "") +
            "</DiagramItem><DiagramItem Operation_Id=\"73\"><ClassName>" + c.bClass +
            "</ClassName><RectF><x>1</x><y>" + c.bY + "</y><width>" + c.bW +
            "</width><height>3</height></RectF>" + c.bCell + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool outsideSelectedRoute = false;
        try {
            parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items;
            REQUIRE(items.size() == 2u);
            outsideSelectedRoute = items[1]->rect[0] != c.selectedX;
        } catch (const Error& e) { outsideSelectedRoute = e.code == M2M_E_UNSUPPORTED; }
        CHECK(outsideSelectedRoute);
    }
}

TEST_CASE(two_item_mixed_explicit_sentinel_cells_use_distinct_empty_targets)
{
    struct Case { std::string aCell; std::string bCell; const char* x; const char* y;
                  const char* w; const char* h; int aCol; int bCol; };
    std::vector<Case> cases;
    const char* sentinels[] = {"", "<CellPos Row=\"-1\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"5\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"-2147483648\"/>",
        "<CellPos Row=\"2147483647\" Col=\"-1\"/>",
        "<CellPos Row=\"0\" Col=\"0\"/><CellPos Row=\"-1\" Col=\"5\"/>"};
    const char* columnX[] = {"-61", "102.5", "307.5", "512.5", "717.5", "922.5", "1127.5"};
    const auto explicitCell = [](int col) {
        return std::string("<CellPos Row=\"0\" Col=\"") + std::to_string(col) + "\"/>";
    };
    for (int bCol = 0; bCol < 7; ++bCol) for (int aCol = 0; aCol < 7; ++aCol) {
        if (aCol == bCol) continue;
        for (const char* sentinel : sentinels)
            cases.push_back({explicitCell(aCol), sentinel, columnX[bCol], bCol == 0 ? "-26" : "67.49999999999999",
                             bCol % 2 ? "-0" : "4.9406564584124654e-324",
                             bCol % 2 ? "-1.7976931348623157e308" : "3.25", aCol, bCol});
    }
    for (int bCol = 1; bCol < 7; ++bCol) for (const char* sentinel : sentinels)
        cases.push_back({sentinel, explicitCell(bCol), "-61", "-26", "-3.5", "1.7976931348623157e308", 0, bCol});
    struct Edge { const char* x; const char* y; int col; };
    const Edge edges[] = {
        {"-1.7976931348623157e308", "-1.7976931348623157e308", 0},
        {"-60.00000000000001", "-25.000000000000004", 0},
        {"-60", "-25", 0}, {"-59.99999999999999", "-24.999999999999996", 0},
        {"102.49999999999999", "0", 0}, {"102.50000000000001", "0", 1},
        {"307.49999999999994", "0", 1}, {"307.50000000000006", "0", 2},
        {"512.49999999999994", "0", 2}, {"512.5000000000001", "0", 3},
        {"717.4999999999999", "0", 3}, {"717.5000000000001", "0", 4},
        {"922.4999999999999", "0", 4}, {"922.5000000000001", "0", 5},
        {"1127.4999999999998", "0", 5}, {"1127.5000000000002", "0", 6},
        {"1332.4999999999998", "0", 6}};
    for (const Edge& edge : edges)
        cases.push_back({explicitCell((edge.col + 1) % 7), sentinels[2], edge.x, edge.y, "-0", "3.25",
                         (edge.col + 1) % 7, edge.col});
    for (const Case& c : cases) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + c.aCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + c.x +
            "</x><y>" + c.y + "</y><width>" + c.w + "</width><height>" + c.h +
            "</height></RectF>" + c.bCell + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, c.aCol);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, c.bCol);
        CHECK_EQ(items[0]->rect[0], 205.0 * c.aCol); CHECK_BITS(items[0]->rect[1], 0.0);
        CHECK_BITS(items[0]->rect[2], 0.0); CHECK_BITS(items[0]->rect[3], 0.0);
        CHECK_EQ(items[1]->rect[0], 205.0 * c.bCol); CHECK_BITS(items[1]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[2], std::strtod(c.w, nullptr));
        CHECK_BITS(items[1]->rect[3], std::strtod(c.h, nullptr));
        const int lo = c.aCol < c.bCol ? 0 : 1, hi = 1 - lo;
        const bool adjacent = c.aCol + 1 == c.bCol || c.bCol + 1 == c.aCol;
        CHECK(items[lo]->prev == nullptr); CHECK(items[hi]->next == nullptr);
        CHECK(items[lo]->next == (adjacent ? items[hi].get() : nullptr));
        CHECK(items[hi]->prev == (adjacent ? items[lo].get() : nullptr));
        const std::vector<int> expected = adjacent ?
            std::vector<int>({items[lo]->opId, items[hi]->opId}) : std::vector<int>({items[lo]->opId});
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(two_item_mixed_cell_collisions_remain_outside_selected_route)
{
    const char* aCells[] = {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"-1\" Col=\"5\"/>"};
    const char* bCells[] = {"<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"0\" Col=\"0\"/>"};
    for (int i = 0; i < 2; ++i) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + aCells[i] + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>" +
            "<RectF><x>1</x><y>0</y><width>2</width><height>-3</height></RectF>" + bCells[i] +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool outsideSelectedRoute = false;
        try {
            parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
            outsideSelectedRoute = items[1]->rect[0] == 1.0 &&
                items[0]->next == nullptr && items[1]->prev == nullptr;
        } catch (const Error& e) { outsideSelectedRoute = e.code == M2M_E_UNSUPPORTED; }
        CHECK(outsideSelectedRoute);
    }
}

TEST_CASE(two_item_mixed_cell_route_keeps_invalid_profiles_guarded)
{
    struct Control { const char* aCell; const char* bClass; const char* y;
                     const char* w; bool linked; };
    const Control controls[] = {
        {"<CellPos Row=\"0\" Col=\"7\"/>", "SfOprForming2D", "0", "2", false},
        {"<CellPos Row=\"1\" Col=\"1\"/>", "SfOprForming2D", "0", "2", false},
        {"<CellPos Row=\"0\" Col=\"1\"/><CellPos Row=\"1\" Col=\"1\"/>", "SfOprForming2D", "0", "2", false},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "189", "2", false},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "nan", "2", false},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "0", "nan", false},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming", "0", "2", false},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "0", "2", true}};
    for (const Control& c : controls) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + c.aCell +
            (c.linked ? "<Connectors><Connections><ItemIndex>1</ItemIndex></Connections></Connectors>" : "") +
            "</DiagramItem><DiagramItem Operation_Id=\"73\"><ClassName>" + c.bClass +
            "</ClassName><RectF><x>1</x><y>" + c.y + "</y><width>" + c.w +
            "</width><height>3</height></RectF><CellPos Row=\"-1\" Col=\"5\"/>" +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool outsideSelectedRoute = false;
        try {
            parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
            outsideSelectedRoute = items[1]->rect[0] != 0.0;
        } catch (const Error& e) { outsideSelectedRoute = e.code == M2M_E_UNSUPPORTED; }
        CHECK(outsideSelectedRoute);
    }
}

TEST_CASE(two_item_a_signed_dimensions_survive_selected_pair_relayout)
{
    struct Dim { const char* w; const char* h; };
    const Dim dimensions[] = {{"-0", "3.25"},
        {"4.9406564584124654e-324", "-4.9406564584124654e-324"},
        {"-1.7976931348623157e308", "1.7976931348623157e308"},
        {"-3.5", "0"}};
    struct Route { const char* aCell; const char* bCell; const char* bx; const char* by;
                   int aCol; int bCol; };
    const Route routes[] = {
        {"", "", "1", "159.99999999999997", 0, 1},
        {"<CellPos Row=\"5\" Col=\"-1\"/>", "<CellPos Row=\"-1\" Col=\"-1\"/>", "205", "10", 0, 1},
        {"", "", "307.5", "0", 0, 2},
        {"<CellPos Row=\"0\" Col=\"4\"/>", "<CellPos Row=\"0\" Col=\"2\"/>", "1", "0", 4, 2},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>", "1", "0", 0, 1},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "<CellPos Row=\"0\" Col=\"0\"/>", "1", "0", 1, 0},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "<CellPos Row=\"-1\" Col=\"5\"/>", "0", "0", 1, 0},
        {"<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"0\" Col=\"3\"/>", "0", "0", 0, 3}};
    for (const Route& r : routes) for (const Dim& d : dimensions) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>-0</x><y>-0</y><width>" + d.w + "</width><height>" + d.h +
            "</height></RectF>" + r.aCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + r.bx +
            "</x><y>" + r.by + "</y><width>2.5</width><height>-3.25</height></RectF>" + r.bCell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, r.aCol);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, r.bCol);
        CHECK_BITS(items[0]->rect[0], 205.0 * r.aCol); CHECK_BITS(items[0]->rect[1], 0.0);
        CHECK_BITS(items[0]->rect[2], std::strtod(d.w, nullptr));
        CHECK_BITS(items[0]->rect[3], std::strtod(d.h, nullptr));
        CHECK_BITS(items[1]->rect[0], 205.0 * r.bCol); CHECK_BITS(items[1]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[2], 2.5); CHECK_BITS(items[1]->rect[3], -3.25);
        const int lo = r.aCol < r.bCol ? 0 : 1, hi = 1 - lo;
        const bool adjacent = r.aCol + 1 == r.bCol || r.bCol + 1 == r.aCol;
        CHECK(items[lo]->prev == nullptr); CHECK(items[hi]->next == nullptr);
        CHECK(items[lo]->next == (adjacent ? items[hi].get() : nullptr));
        CHECK(items[hi]->prev == (adjacent ? items[lo].get() : nullptr));
        const std::vector<int> expected = adjacent ?
            std::vector<int>({items[lo]->opId, items[hi]->opId}) : std::vector<int>({items[lo]->opId});
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(two_item_a_nonfinite_dimensions_stay_outside_explicit_and_mixed_routes)
{
    struct Control { const char* aw; const char* ah; const char* aCell;
                     const char* bCell; const char* bx; int checkedItem; double selectedX; };
    const Control controls[] = {
        {"nan", "0", "<CellPos Row=\"0\" Col=\"1\"/>", "<CellPos Row=\"0\" Col=\"2\"/>", "1", 1, 410.0},
        {"0", "inf", "<CellPos Row=\"0\" Col=\"1\"/>", "<CellPos Row=\"-1\" Col=\"5\"/>", "0", 0, 205.0},
        {"-inf", "0", "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"0\" Col=\"2\"/>", "0", 1, 410.0}};
    for (const Control& c : controls) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>" + c.aw + "</width><height>" + c.ah +
            "</height></RectF>" + c.aCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>" +
            "<RectF><x>" + c.bx + "</x><y>0</y><width>2.5</width><height>-3.25</height></RectF>" +
            c.bCell + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool outsideSelectedRoute = false;
        try {
            parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
            outsideSelectedRoute = items[c.checkedItem]->rect[0] != c.selectedX;
        } catch (const Error& e) { outsideSelectedRoute = e.code == M2M_E_UNSUPPORTED; }
        CHECK(outsideSelectedRoute);
    }
}

TEST_CASE(three_item_positive_c_collision_shifts_b_and_rebuilds_graph)
{
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"-1\"/>",
        "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"5\" Col=\"-1\"/>"};
    const char* xs[] = {"4.9406564584124654e-324", "1", "102.49999999999999"};
    for (const char* x : xs) for (const char* aCell : cells)
        for (const char* bCell : cells) for (const char* cCell : cells) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + aCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"52\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + bCell + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + x +
            "</x><y>0</y><width>0</width><height>0</height></RectF>" + cCell +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 3u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 52); CHECK_EQ(items[2]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, 2);
        CHECK_EQ(items[2]->row, 0); CHECK_EQ(items[2]->col, 1);
        for (int j = 0; j < 3; ++j) {
            CHECK_BITS(items[j]->rect[0], 205.0 * items[j]->col);
            CHECK_BITS(items[j]->rect[1], 0.0);
            CHECK_BITS(items[j]->rect[2], 0.0); CHECK_BITS(items[j]->rect[3], 0.0);
        }
        CHECK(items[0]->prev == nullptr); CHECK(items[0]->next == items[2].get());
        CHECK(items[2]->prev == items[0].get()); CHECK(items[2]->next == items[1].get());
        CHECK(items[1]->prev == items[2].get()); CHECK(items[1]->next == nullptr);
        CHECK(mstOrder(*p.subs[0]) == std::vector<int>({41, 73, 52}));
    }
}

TEST_CASE(three_item_positive_c_thresholds_and_signed_dimensions)
{
    struct Case { const char* x; int bCol; int cCol; };
    const Case cases[] = {
        {"102.5", 2, 1}, {"204.99999999999997", 2, 1},
        {"205", 1, 2}, {"307.49999999999994", 1, 2},
        {"307.5", 1, 2}, {"512.4999999999999", 1, 2},
        {"512.5", 1, 3}, {"717.4999999999999", 1, 3},
        {"717.5", 1, 4}, {"922.5", 1, 5},
        {"1127.5", 1, 6}, {"1332.4999999999998", 1, 6}};
    const char* ys[] = {"-1.7976931348623157e308", "159.99999999999997"};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + cells[form] + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"52\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" + cells[(form + 1) % 3] + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>" + c.x + "</x><y>" + y +
            "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>" +
            cells[(form + 2) % 3] + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 3u);
        const int cols[] = {0, c.bCol, c.cCol};
        for (int i = 0; i < 3; ++i) {
            CHECK_EQ(items[i]->opId, i == 0 ? 41 : (i == 1 ? 52 : 73));
            CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]);
            CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i == 2 ? -0.0 : 0.0);
            CHECK_BITS(items[i]->rect[3], i == 2 ? 4.9406564584124654e-324 : 0.0);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < 3; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        const std::vector<int> expected = c.cCol == 1 ? std::vector<int>({41, 73, 52}) :
            c.cCol == 2 ? std::vector<int>({41, 52, 73}) : std::vector<int>({41, 52});
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(three_item_negative_c_direction_clamp_and_underflow)
{
    const double eps = std::numeric_limits<double>::denorm_min();
    struct Case { double x; int aCol; int bCol; int cCol; };
    const Case cases[] = {
        {-std::numeric_limits<double>::max(), 0, 2, 1},
        {-60.00000000000001, 0, 2, 1},
        {-60.0, 1, 2, 0},
        {-1.0, 1, 2, 0},
        {-103.0 * eps, 1, 2, 0},
        {-102.0 * eps, 0, 2, 1},
        {-eps, 0, 2, 1}};
    const char* ys[] = {"-1.7976931348623157e308", "159.99999999999997"};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        char xText[64]; std::snprintf(xText, sizeof(xText), "%.17g", c.x);
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>-0</x><y>-0</y><width>-0</width><height>0</height></RectF>" + cells[form] + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"52\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>-0</height></RectF>" + cells[(form + 1) % 3] + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + xText +
            "</x><y>" + y +
            "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>" +
            cells[(form + 2) % 3] + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 3u);
        const int cols[] = {c.aCol, c.bCol, c.cCol};
        for (int i = 0; i < 3; ++i) {
            CHECK_EQ(items[i]->opId, i == 0 ? 41 : (i == 1 ? 52 : 73));
            CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]);
            CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i == 1 ? 0.0 : -0.0);
            CHECK_BITS(items[i]->rect[3], i == 2 ? eps : (i == 1 ? -0.0 : 0.0));
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < 3; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        const std::vector<int> expected = c.cCol == 0 ?
            std::vector<int>({73, 41, 52}) : std::vector<int>({41, 73, 52});
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(four_item_positive_d_inserts_and_breaks_graph_at_gap)
{
    struct Case { const char* x; int bCol; int cCol; int dCol; };
    const Case cases[] = {
        {"4.9406564584124654e-324", 3, 2, 1},
        {"102.49999999999999", 3, 2, 1}, {"102.5", 3, 2, 1},
        {"204.99999999999997", 3, 2, 1},
        {"205", 3, 1, 2}, {"307.49999999999994", 3, 1, 2},
        {"307.5", 3, 1, 2}, {"409.99999999999994", 3, 1, 2},
        {"410", 2, 1, 3}, {"512.5", 2, 1, 3},
        {"717.4999999999999", 2, 1, 3}, {"717.5", 2, 1, 4},
        {"922.5", 2, 1, 5}, {"1127.5", 2, 1, 6},
        {"1332.4999999999998", 2, 1, 6}};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    const char* ys[] = {"-1.7976931348623157e308", "159.99999999999997"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        const int ids[] = {41, 52, 63, 74};
        for (int i = 0; i < 4; ++i) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(ids[i]) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (i == 3 ? std::string(c.x) : std::string("0")) + "</x><y>" +
                (i == 3 ? std::string(y) : std::string("0")) +
                "</y><width>" + (i == 3 ? "-0" : "0") +
                "</width><height>" + (i == 3 ? "4.9406564584124654e-324" : "0") +
                "</height></RectF>" + cells[(i + form) % 3] + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 4u);
        const int cols[] = {0, c.bCol, c.cCol, c.dCol};
        for (int i = 0; i < 4; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i == 3 ? -0.0 : 0.0);
            CHECK_BITS(items[i]->rect[3], i == 3 ? 4.9406564584124654e-324 : 0.0);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < 4; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        const std::vector<int> expected = c.dCol == 1 ? std::vector<int>({41, 74, 63, 52}) :
            c.dCol == 2 ? std::vector<int>({41, 63, 74, 52}) :
            c.dCol == 3 ? std::vector<int>({41, 63, 52, 74}) :
                             std::vector<int>({41, 63, 52});
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(five_item_positive_e_inserts_and_breaks_graph_at_gap)
{
    struct Case { const char* x; int bCol; int cCol; int dCol; int eCol; };
    const Case cases[] = {
        {"4.9406564584124654e-324", 4, 3, 2, 1},
        {"102.5", 4, 3, 2, 1}, {"204.99999999999997", 4, 3, 2, 1},
        {"205", 4, 3, 1, 2}, {"307.5", 4, 3, 1, 2},
        {"409.99999999999994", 4, 3, 1, 2},
        {"410", 4, 2, 1, 3}, {"512.5", 4, 2, 1, 3},
        {"614.9999999999999", 4, 2, 1, 3},
        {"615", 3, 2, 1, 4}, {"717.5", 3, 2, 1, 4},
        {"922.4999999999999", 3, 2, 1, 4},
        {"922.5", 3, 2, 1, 5}, {"1127.5", 3, 2, 1, 6},
        {"1332.4999999999998", 3, 2, 1, 6}};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    const char* ys[] = {"-1.7976931348623157e308", "159.99999999999997"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        const int ids[] = {41, 52, 63, 74, 85};
        for (int i = 0; i < 5; ++i) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(ids[i]) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (i == 4 ? std::string(c.x) : std::string("0")) + "</x><y>" +
                (i == 4 ? std::string(y) : std::string("0")) +
                "</y><width>" + (i == 4 ? "-0" : "0") +
                "</width><height>" + (i == 4 ? "4.9406564584124654e-324" : "0") +
                "</height></RectF>" + cells[(i + form) % 3] + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 5u);
        const int cols[] = {0, c.bCol, c.cCol, c.dCol, c.eCol};
        for (int i = 0; i < 5; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i == 4 ? -0.0 : 0.0);
            CHECK_BITS(items[i]->rect[3], i == 4 ? 4.9406564584124654e-324 : 0.0);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < 5; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        const std::vector<int> expected = c.eCol == 1 ? std::vector<int>({41, 85, 74, 63, 52}) :
            c.eCol == 2 ? std::vector<int>({41, 74, 85, 63, 52}) :
            c.eCol == 3 ? std::vector<int>({41, 74, 63, 85, 52}) :
            c.eCol == 4 ? std::vector<int>({41, 74, 63, 52, 85}) :
                            std::vector<int>({41, 74, 63, 52});
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(six_item_positive_f_inserts_and_breaks_graph_at_gap)
{
    struct Case { const char* x; int bCol; int cCol; int dCol; int eCol; int fCol; };
    const Case cases[] = {
        {"4.9406564584124654e-324", 5, 4, 3, 2, 1},
        {"102.5", 5, 4, 3, 2, 1}, {"204.99999999999997", 5, 4, 3, 2, 1},
        {"205", 5, 4, 3, 1, 2}, {"307.5", 5, 4, 3, 1, 2},
        {"409.99999999999994", 5, 4, 3, 1, 2},
        {"410", 5, 4, 2, 1, 3}, {"512.5", 5, 4, 2, 1, 3},
        {"614.9999999999999", 5, 4, 2, 1, 3},
        {"615", 5, 3, 2, 1, 4}, {"717.5", 5, 3, 2, 1, 4},
        {"819.9999999999999", 5, 3, 2, 1, 4},
        {"820", 4, 3, 2, 1, 5}, {"922.5", 4, 3, 2, 1, 5},
        {"1127.4999999999998", 4, 3, 2, 1, 5},
        {"1127.5", 4, 3, 2, 1, 6},
        {"1332.4999999999998", 4, 3, 2, 1, 6}};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    const char* ys[] = {"-1.7976931348623157e308", "159.99999999999997"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        const int ids[] = {41, 52, 63, 74, 85, 96};
        for (int i = 0; i < 6; ++i) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(ids[i]) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (i == 5 ? std::string(c.x) : std::string("0")) + "</x><y>" +
                (i == 5 ? std::string(y) : std::string("0")) +
                "</y><width>" + (i == 5 ? "-0" : "0") +
                "</width><height>" + (i == 5 ? "4.9406564584124654e-324" : "0") +
                "</height></RectF>" + cells[(i + form) % 3] + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 6u);
        const int cols[] = {0, c.bCol, c.cCol, c.dCol, c.eCol, c.fCol};
        for (int i = 0; i < 6; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i == 5 ? -0.0 : 0.0);
            CHECK_BITS(items[i]->rect[3], i == 5 ? 4.9406564584124654e-324 : 0.0);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < 6; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        std::vector<int> expected;
        for (int col = 0; col < 6; ++col) {
            int index = -1;
            for (int i = 0; i < 6; ++i) if (cols[i] == col) index = i;
            if (index < 0) break;
            expected.push_back(ids[index]);
        }
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(seven_item_positive_g_inserts_with_no_gap_at_right_edge)
{
    struct Case { const char* x; int bCol; int cCol; int dCol; int eCol; int fCol; int gCol; };
    const Case cases[] = {
        {"4.9406564584124654e-324", 6, 5, 4, 3, 2, 1},
        {"102.5", 6, 5, 4, 3, 2, 1}, {"204.99999999999997", 6, 5, 4, 3, 2, 1},
        {"205", 6, 5, 4, 3, 1, 2}, {"307.5", 6, 5, 4, 3, 1, 2},
        {"409.99999999999994", 6, 5, 4, 3, 1, 2},
        {"410", 6, 5, 4, 2, 1, 3}, {"512.5", 6, 5, 4, 2, 1, 3},
        {"614.9999999999999", 6, 5, 4, 2, 1, 3},
        {"615", 6, 5, 3, 2, 1, 4}, {"717.5", 6, 5, 3, 2, 1, 4},
        {"819.9999999999999", 6, 5, 3, 2, 1, 4},
        {"820", 6, 4, 3, 2, 1, 5}, {"922.5", 6, 4, 3, 2, 1, 5},
        {"1024.9999999999998", 6, 4, 3, 2, 1, 5},
        {"1025", 5, 4, 3, 2, 1, 6}, {"1127.5", 5, 4, 3, 2, 1, 6},
        {"1332.4999999999998", 5, 4, 3, 2, 1, 6}};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    const char* ys[] = {"-1.7976931348623157e308", "159.99999999999997"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        const int ids[] = {41, 52, 63, 74, 85, 96, 107};
        for (int i = 0; i < 7; ++i) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(ids[i]) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (i == 6 ? std::string(c.x) : std::string("0")) + "</x><y>" +
                (i == 6 ? std::string(y) : std::string("0")) +
                "</y><width>" + (i == 6 ? "-0" : "0") +
                "</width><height>" + (i == 6 ? "4.9406564584124654e-324" : "0") +
                "</height></RectF>" + cells[(i + form) % 3] + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 7u);
        const int cols[] = {0, c.bCol, c.cCol, c.dCol, c.eCol, c.fCol, c.gCol};
        for (int i = 0; i < 7; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i == 6 ? -0.0 : 0.0);
            CHECK_BITS(items[i]->rect[3], i == 6 ? 4.9406564584124654e-324 : 0.0);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < 7; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        std::vector<int> expected;
        for (int col = 0; col < 7; ++col)
            for (int i = 0; i < 7; ++i) if (cols[i] == col) expected.push_back(ids[i]);
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(four_to_seven_negative_last_direction_and_underflow)
{
    const double eps = std::numeric_limits<double>::denorm_min();
    struct Case { double x; bool negativeQuotient; };
    const Case cases[] = {
        {-std::numeric_limits<double>::max(), false},
        {-60.00000000000001, false}, {-60.0, true}, {-1.0, true},
        {-103.0 * eps, true}, {-102.0 * eps, false}, {-eps, false}};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    const char* ys[] = {"-1.7976931348623157e308", "159.99999999999997"};
    for (int count = 4; count <= 7; ++count) for (const Case& c : cases)
        for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        char xText[64]; std::snprintf(xText, sizeof(xText), "%.17g", c.x);
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < count; ++i) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(100 + i) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (i + 1 == count ? std::string(xText) : std::string(i == 0 ? "-0" : "0")) +
                "</x><y>" + (i + 1 == count ? std::string(y) : std::string("0")) +
                "</y><width>" + ((i == 0 || i + 1 == count) ? "-0" : "0") +
                "</width><height>" + (i + 1 == count ? "4.9406564584124654e-324" :
                                       (i == 1 ? "-0" : "0")) +
                "</height></RectF>" + cells[(i + form) % 3] + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(count));
        std::vector<int> cols(count);
        cols[0] = c.negativeQuotient ? 1 : 0;
        for (int i = 1; i + 1 < count; ++i) cols[i] = count - i;
        cols[count - 1] = c.negativeQuotient ? 0 : 1;
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, 100 + i); CHECK_EQ(items[i]->row, 0);
            CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]);
            CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], (i == 0 || i + 1 == count) ? -0.0 : 0.0);
            CHECK_BITS(items[i]->rect[3], i + 1 == count ? eps : (i == 1 ? -0.0 : 0.0));
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        std::vector<int> expected;
        for (int col = 0; col < count; ++col)
            for (int i = 0; i < count; ++i) if (cols[i] == col) expected.push_back(100 + i);
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(negative_id_pair_expanded_graphic_admits_row_zero_to_190)
{
    struct Case { const char* x; int col; };
    const Case cases[] = {{"4.9406564584124654e-324", 1}, {"1", 1},
        {"205", 1}, {"307.49999999999994", 1}, {"307.5", 2},
        {"512.5", 3}, {"717.5", 4}, {"922.5", 5},
        {"1127.5", 6}, {"1332.4999999999998", 6}};
    const char* ys[] = {"160", "189.99999999999997"};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"-10\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" +
            cells[form] + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"-11\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>" + c.x + "</x><y>" + y +
            "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>" +
            cells[(form + 1) % 3] + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, -10); CHECK_EQ(items[1]->opId, -11);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, c.col);
        CHECK_BITS(items[0]->rect[0], 0.0); CHECK_BITS(items[0]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[0], 205.0 * c.col); CHECK_BITS(items[1]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[2], -0.0);
        CHECK_BITS(items[1]->rect[3], 4.9406564584124654e-324);
        CHECK(items[0]->prev == nullptr); CHECK(items[1]->next == nullptr);
        CHECK(items[0]->next == (c.col == 1 ? items[1].get() : nullptr));
        CHECK(items[1]->prev == (c.col == 1 ? items[0].get() : nullptr));
        CHECK(mstOrder(*p.subs[0]) == (c.col == 1 ?
            std::vector<int>({-10, -11}) : std::vector<int>({-10})));
    }
    for (const char* y : {"190", "190.00000000000003"}) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"-10\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"-11\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>1</x><y>" + y +
            "</y><width>0</width><height>0</height></RectF></DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool guarded = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) { guarded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(guarded);
    }
}

TEST_CASE(positive_id_pair_without_operation_uses_conservative_template_height)
{
    struct Case { const char* x; int col; };
    const Case cases[] = {{"1", 1}, {"205", 1}, {"307.49999999999994", 1},
        {"307.5", 2}, {"512.5", 3}, {"717.5", 4}, {"922.5", 5},
        {"1127.5", 6}, {"1332.4999999999998", 6}};
    const char* ys[] = {"160", "188.99999999999997"};
    const char* cells[] = {"", "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
                           "<CellPos Row=\"-2147483648\" Col=\"-1\"/>"};
    for (const Case& c : cases) for (const char* y : ys) for (int form = 0; form < 3; ++form) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF>" +
            cells[form] + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>" + c.x + "</x><y>" + y +
            "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>" +
            cells[(form + 1) % 3] + "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, 41); CHECK_EQ(items[1]->opId, 73);
        CHECK_EQ(items[0]->row, 0); CHECK_EQ(items[0]->col, 0);
        CHECK_EQ(items[1]->row, 0); CHECK_EQ(items[1]->col, c.col);
        CHECK_BITS(items[1]->rect[0], 205.0 * c.col); CHECK_BITS(items[1]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[2], -0.0);
        CHECK_BITS(items[1]->rect[3], 4.9406564584124654e-324);
        CHECK(items[0]->prev == nullptr); CHECK(items[1]->next == nullptr);
        CHECK(items[0]->next == (c.col == 1 ? items[1].get() : nullptr));
        CHECK(items[1]->prev == (c.col == 1 ? items[0].get() : nullptr));
        CHECK(mstOrder(*p.subs[0]) == (c.col == 1 ?
            std::vector<int>({41, 73}) : std::vector<int>({41})));
    }
    // The measured template gives only a conservative minimum, so the exact
    // boundary and an associated Operation remain outside the selected route.
    for (bool withOperation : {false, true}) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        if (withOperation) {
            auto op = std::make_unique<Operation>(); op->id = 41;
            p.operations.push_back(std::move(op));
        }
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>1</x><y>" + (withOperation ? "160" : "189") +
            "</y><width>0</width><height>0</height></RectF></DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool guarded = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) { guarded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(guarded);
    }
}

TEST_CASE(all_negative_distinct_targets_use_expanded_row_zero_height)
{
    for (int count = 2; count <= 7; ++count) for (int rotation = 0; rotation < 7; ++rotation)
        for (int mixed = 0; mixed < 2; ++mixed) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        std::vector<int> cols;
        for (int i = 0; i < count; ++i) {
            const int target = (rotation + 3 * i) % 7;
            cols.push_back(target);
            const bool explicitCell = (i + mixed) % 2 == 0;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(-100 - i) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (explicitCell ? std::string("-1.7976931348623157e308") :
                                std::to_string(205 * target)) +
                "</x><y>" + (i == 0 ? "67.49999999999999" : "189.99999999999997") +
                "</y><width>" + (i % 2 == 0 ? "-0" : "-3.5") +
                "</width><height>" + (i % 2 == 0 ? "4.9406564584124654e-324" : "2.5") +
                "</height></RectF>";
            if (explicitCell)
                source += "<CellPos Row=\"0\" Col=\"" + std::to_string(target) + "\"/>";
            else if (i % 3 == 0)
                source += "<CellPos Row=\"2147483647\" Col=\"-1\"/>";
            else if (i % 3 == 1)
                source += "<CellPos Row=\"-1\" Col=\"-2147483648\"/>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, -100 - i); CHECK_EQ(items[i]->row, 0);
            CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i % 2 == 0 ? -0.0 : -3.5);
            CHECK_BITS(items[i]->rect[3], i % 2 == 0 ? 4.9406564584124654e-324 : 2.5);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        int lowest = 0;
        for (int i = 1; i < count; ++i) if (cols[i] < cols[lowest]) lowest = i;
        std::vector<int> expected{-100 - lowest};
        int current = lowest;
        for (int step = 1; step < count; ++step) {
            int next = -1;
            for (int i = 0; i < count; ++i) if (cols[i] == cols[current] + 1) next = i;
            if (next < 0) break;
            expected.push_back(-100 - next); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(first_negative_id_mixed_signs_keep_expanded_row_zero_height)
{
    for (int count = 2; count <= 7; ++count) for (int rotation = 0; rotation < 7; ++rotation) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        std::vector<int> cols;
        for (int i = 0; i < count; ++i) {
            const int target = (rotation + 3 * i) % 7;
            cols.push_back(target);
            const bool explicitCell = i % 2 == 0;
            source += "<DiagramItem Operation_Id=\"" +
                std::to_string(i == 0 ? -100 : 100 + i) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (explicitCell ? std::string("-1.7976931348623157e308") :
                                std::to_string(205 * target)) +
                "</x><y>" + (i == 0 ? "67.49999999999999" : "189.99999999999997") +
                "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>";
            if (explicitCell)
                source += "<CellPos Row=\"0\" Col=\"" + std::to_string(target) + "\"/>";
            else if (i % 3 == 0)
                source += "<CellPos Row=\"-1\" Col=\"2147483647\"/>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, i == 0 ? -100 : 100 + i);
            CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], -0.0);
            CHECK_BITS(items[i]->rect[3], 4.9406564584124654e-324);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        int lowest = 0;
        for (int i = 1; i < count; ++i) if (cols[i] < cols[lowest]) lowest = i;
        std::vector<int> expected{items[lowest]->opId};
        int current = lowest;
        for (int step = 1; step < count; ++step) {
            int next = -1;
            for (int i = 0; i < count; ++i) if (cols[i] == cols[current] + 1) next = i;
            if (next < 0) break;
            expected.push_back(items[next]->opId); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
    for (const char* y : {"160", "189.99999999999997"}) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"-10\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"11\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>1</x><y>" + y +
            "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>"
            "<CellPos Row=\"-1\" Col=\"2147483647\"/></DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 2u);
        CHECK_EQ(items[0]->opId, -10); CHECK_EQ(items[1]->opId, 11);
        CHECK_EQ(items[0]->col, 0); CHECK_EQ(items[1]->col, 1);
        CHECK_BITS(items[1]->rect[0], 205.0); CHECK_BITS(items[1]->rect[1], 0.0);
        CHECK_BITS(items[1]->rect[2], -0.0);
        CHECK_BITS(items[1]->rect[3], 4.9406564584124654e-324);
        CHECK(items[0]->next == items[1].get()); CHECK(items[1]->prev == items[0].get());
        CHECK(mstOrder(*p.subs[0]) == std::vector<int>({-10, 11}));
    }
}

TEST_CASE(positive_unbound_ids_distinct_targets_use_conservative_row_height)
{
    for (int count = 2; count <= 7; ++count) for (int rotation = 0; rotation < 7; ++rotation)
        for (int mixed = 0; mixed < 2; ++mixed) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        std::vector<int> cols;
        for (int i = 0; i < count; ++i) {
            const int target = (rotation + 3 * i) % 7;
            cols.push_back(target);
            const bool explicitCell = (i + mixed) % 2 == 0;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(i) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (explicitCell ? std::string("-1.7976931348623157e308") :
                                std::to_string(205 * target)) +
                "</x><y>" + (i == 0 ? "67.49999999999999" : "188.99999999999997") +
                "</y><width>" + (i % 2 == 0 ? "-0" : "-3.5") +
                "</width><height>" +
                (i % 2 == 0 ? "4.9406564584124654e-324" : "2.5") +
                "</height></RectF>";
            if (explicitCell)
                source += "<CellPos Row=\"0\" Col=\"" + std::to_string(target) + "\"/>";
            else if (i % 3 == 0)
                source += "<CellPos Row=\"-1\" Col=\"2147483647\"/>";
            else if (i % 3 == 1)
                source += "<CellPos Row=\"2147483647\" Col=\"-1\"/>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, i); CHECK_EQ(items[i]->row, 0);
            CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i % 2 == 0 ? -0.0 : -3.5);
            CHECK_BITS(items[i]->rect[3], i % 2 == 0 ? 4.9406564584124654e-324 : 2.5);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        int lowest = 0;
        for (int i = 1; i < count; ++i) if (cols[i] < cols[lowest]) lowest = i;
        std::vector<int> expected{lowest};
        int current = lowest;
        for (int step = 1; step < count; ++step) {
            int next = -1;
            for (int i = 0; i < count; ++i) if (cols[i] == cols[current] + 1) next = i;
            if (next < 0) break;
            expected.push_back(next); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
}

TEST_CASE(first_positive_unbound_id_allows_later_mixed_sign_distinct_targets)
{
    for (int count = 2; count <= 7; ++count) for (int rotation = 0; rotation < 7; ++rotation)
        for (int mixed = 0; mixed < 2; ++mixed) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        std::vector<int> cols, ids;
        for (int i = 0; i < count; ++i) {
            const int id = i == 0 ? 0 : (i % 2 ? -100 - i : 100 + i);
            const int target = (rotation + 3 * i) % 7;
            ids.push_back(id); cols.push_back(target);
            const bool explicitCell = (i + mixed) % 2 == 0;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (explicitCell ? std::string("-1.7976931348623157e308") :
                                std::to_string(205 * target)) +
                "</x><y>" + (i == 0 ? "67.49999999999999" : "188.99999999999997") +
                "</y><width>" + (i % 2 ? "-3.5" : "-0") +
                "</width><height>" +
                (i % 2 ? "2.5" : "4.9406564584124654e-324") +
                "</height></RectF>";
            if (explicitCell)
                source += "<CellPos Row=\"0\" Col=\"" + std::to_string(target) + "\"/>";
            else if (i % 3 == 0)
                source += "<CellPos Row=\"-1\" Col=\"2147483647\"/>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0);
            CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], i % 2 ? -3.5 : -0.0);
            CHECK_BITS(items[i]->rect[3], i % 2 ? 2.5 : 4.9406564584124654e-324);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        int lowest = 0;
        for (int i = 1; i < count; ++i) if (cols[i] < cols[lowest]) lowest = i;
        std::vector<int> expected{ids[lowest]};
        int current = lowest;
        for (int step = 1; step < count; ++step) {
            int next = -1;
            for (int i = 0; i < count; ++i) if (cols[i] == cols[current] + 1) next = i;
            if (next < 0) break;
            expected.push_back(ids[next]); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
    // An associated later Operation may add object-row graph effects; its
    // above-common-envelope profile stays outside this selected direct route.
    {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        auto op = std::make_unique<Operation>(); op->id = -101;
        p.operations.push_back(std::move(op));
        const std::string source =
            "<Sub_Project><DiagramItem Operation_Id=\"0\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"-101\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>205</x><y>188</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"102\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>410</x><y>188</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool guarded = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) { guarded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(guarded);
    }
}

TEST_CASE(first_associated_zero_slot_operation_preserves_height_and_name_order)
{
    for (int count : {2, 3, 7}) for (int rotation : {0, 2}) for (int nameMode = 0;
        nameMode < 4; ++nameMode) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        auto op = std::make_unique<Operation>(); op->id = 41; op->name = L("Operation Name");
        p.operations.push_back(std::move(op));
        std::string source = "<Sub_Project>";
        std::vector<int> cols, ids;
        for (int i = 0; i < count; ++i) {
            const int id = i == 0 ? 41 : (i % 2 ? -100 - i : 100 + i);
            const int target = (rotation + 3 * i) % 7;
            ids.push_back(id); cols.push_back(target);
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) + "\">";
            if (i == 0 && nameMode == 1) source += "<ItemName>Before</ItemName>";
            source += "<ClassName>SfOprForming2D</ClassName>";
            if (i == 0 && nameMode == 2) source += "<ItemName>Override</ItemName>";
            if (i == 0 && nameMode == 3) source += "<ItemName></ItemName>";
            source += "<RectF><x>" + std::to_string(205 * target) +
                "</x><y>" + (i == 0 ? "0" : "188.99999999999997") +
                "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>";
            if (i == 0)
                source += "<CellPos Row=\"0\" Col=\"" + std::to_string(target) + "\"/>";
            else if (i % 2 == 0)
                source += "<CellPos Row=\"-1\" Col=\"2147483647\"/>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(count));
        CHECK(items[0]->itemName == (nameMode == 2 ? L("Override") :
              nameMode == 3 ? L("") : L("Operation Name")));
        CHECK_EQ(items[0]->expansion.size(), 7u);
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0);
            CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], -0.0);
            CHECK_BITS(items[i]->rect[3], 4.9406564584124654e-324);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        int lowest = 0;
        for (int i = 1; i < count; ++i) if (cols[i] < cols[lowest]) lowest = i;
        std::vector<int> expected{ids[lowest]};
        int current = lowest;
        for (int step = 1; step < count; ++step) {
            int next = -1;
            for (int i = 0; i < count; ++i) if (cols[i] == cols[current] + 1) next = i;
            if (next < 0) break;
            expected.push_back(ids[next]); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expected);
    }
    for (int exclusion = 0; exclusion < 3; ++exclusion) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        auto op = std::make_unique<Operation>();
        op->id = exclusion == 2 ? -101 : 41;
        if (exclusion == 0) op->restartGiven = 1;
        if (exclusion == 1) op->objIds.push_back(-2);
        p.operations.push_back(std::move(op));
        const std::string source =
            "<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"-101\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>205</x><y>188</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "<DiagramItem Operation_Id=\"102\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>410</x><y>188</y><width>0</width><height>0</height></RectF></DiagramItem>"
            "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool guarded = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) { guarded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(guarded);
    }
}

TEST_CASE(first_associated_undefined_rows_keep_empty_accepts_and_direct_targets)
{
    for (int slots : {1, 2, 7}) for (int count : {2, 3, 7}) for (int rotation : {0, 2}) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        auto op = std::make_unique<Operation>(); op->id = 41;
        op->name = L("Undefined Holder"); op->objIds.assign(slots, -1);
        p.operations.push_back(std::move(op));
        std::string source = "<Sub_Project>";
        std::vector<int> cols, ids;
        for (int i = 0; i < count; ++i) {
            const int id = i == 0 ? 41 : (i % 2 ? -100 - i : 100 + i);
            const int target = (rotation + 3 * i) % 7;
            ids.push_back(id); cols.push_back(target);
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                std::to_string(205 * target) + "</x><y>" +
                (i == 0 ? "0" : "188.99999999999997") +
                "</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>";
            if (i == 0)
                source += "<CellPos Row=\"0\" Col=\"" + std::to_string(target) + "\"/>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(count));
        CHECK(items[0]->itemName == L("Undefined Holder"));
        CHECK_EQ(items[0]->expansion.size(), static_cast<std::size_t>(7 + slots));
        for (int s = 0; s < slots; ++s) {
            const auto& row = items[0]->expansion[static_cast<std::size_t>(7 + s)];
            CHECK_EQ(row.type, 0); CHECK(row.label == L("  Undefined"));
            CHECK(row.accepts.empty());
        }
        p.activeSub = 0;
        diagramEqpDataAdded(p, 41, -1, L("Equipment"));
        for (int s = 0; s < slots; ++s)
            CHECK(items[0]->expansion[static_cast<std::size_t>(7 + s)].label == L("  Undefined"));
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0);
            CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], -0.0);
            CHECK_BITS(items[i]->rect[3], 4.9406564584124654e-324);
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (cols[j] + 1 == cols[i]) prev = items[j].get();
                if (cols[i] + 1 == cols[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
    }
}

TEST_CASE(selected_multi_item_graph_guards_read_from_db_row_ports_before_placement)
{
    struct Case { const char* cells[4]; const char* xs[4]; int count; int associated; };
    const Case cases[] = {
        {{"", "", "", ""}, {"0", "205", "0", "0"}, 2, 73},
        {{"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>", "", ""},
         {"0", "123", "0", "0"}, 2, 41},
        {{"<CellPos Row=\"0\" Col=\"0\"/>", "", "", ""},
         {"0", "205", "410", "0"}, 3, 52},
        {{"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>",
          "<CellPos Row=\"0\" Col=\"2\"/>", "<CellPos Row=\"0\" Col=\"3\"/>"},
         {"0", "123", "410", "615"}, 4, 84},
        {{"", "", "", ""}, {"0", "0", "0", "0"}, 2, 41}};
    for (const Case& c : cases) for (int slotId : {-2, 100, 101}) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        auto op = std::make_unique<Operation>(); op->id = c.associated;
        op->objIds.push_back(slotId);
        if (slotId >= 0) {
            auto entry = std::make_unique<ObjEntry>();
            entry->name = slotId == 100 ? L("Read From DB") : L("Defined");
            op->objs.push_back(std::move(entry));
        }
        p.operations.push_back(std::move(op));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < c.count; ++i) {
            const int id = i == 0 ? 41 : i == 1 ? 73 : i == 2 ? 52 : 84;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + c.xs[i] +
                "</x><y>0</y><width>0</width><height>0</height></RectF>" +
                c.cells[i] + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool guarded = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) {
            guarded = e.code == M2M_E_UNSUPPORTED &&
                std::string(e.what()).find("object-row ports") != std::string::npos;
        }
        CHECK(guarded);
        const auto& items = p.subs[0]->flow.items;
        REQUIRE(items.size() == static_cast<std::size_t>(c.count));
        for (int i = 0; i < c.count; ++i) {
            CHECK_EQ(items[i]->rect[0], std::stod(c.xs[i]));
            CHECK(items[i]->prev == nullptr); CHECK(items[i]->next == nullptr);
        }
    }
}

TEST_CASE(above_common_row_height_routes_guard_unqualified_associations)
{
    struct Case { int a; int b; int associated; int count; const char* y; };
    const Case cases[] = {
        {41, 73, 73, 2, "160"}, {41, 73, 73, 2, "188"},
        {-10, -11, -10, 2, "160"}, {-10, -11, -11, 2, "188"},
        {-10, -11, -10, 2, "189.5"}, {-10, -11, -12, 3, "188"}};
    for (const Case& c : cases) for (int variant = 0; variant < 4; ++variant) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        auto op = std::make_unique<Operation>(); op->id = c.associated;
        if (variant == 1) op->restartGiven = 1;
        if (variant == 2) op->objIds.push_back(-1);
        if (variant == 3) op->objIds.push_back(-2);
        p.operations.push_back(std::move(op));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < c.count; ++i) {
            const int id = i == 0 ? c.a : i == 1 ? c.b : -12;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(id) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                std::to_string(205 * i) + "</x><y>" + (i == 0 ? "0" : c.y) +
                "</y><width>0</width><height>0</height></RectF></DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool guarded = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) { guarded = e.code == M2M_E_UNSUPPORTED; }
        CHECK(guarded);
    }
}

TEST_CASE(three_item_distinct_explicit_cells_relayout_and_connect_grid_runs)
{
    const auto cell = [](int col) {
        return std::string("<CellPos Row=\"0\" Col=\"") + std::to_string(col) + "\"/>";
    };
    for (int aCol = 0; aCol < 7; ++aCol) for (int bCol = 0; bCol < 7; ++bCol)
        for (int cCol = 0; cCol < 7; ++cCol) {
        if (aCol == bCol || aCol == cCol || bCol == cCol) continue;
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>2.5</width><height>-3.25</height></RectF>" + cell(aCol) + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"52\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>205</x><y>10</y><width>-0</width><height>4.9406564584124654e-324</height></RectF>" + cell(bCol) + "</DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>410</x><y>-26</y><width>-1.7976931348623157e308</width><height>1.25</height></RectF>" + cell(cCol) +
            "</DiagramItem></Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 3u);
        const int cols[] = {aCol, bCol, cCol};
        const int ids[] = {41, 52, 73};
        const double widths[] = {2.5, -0.0, -1.7976931348623157e308};
        const double heights[] = {-3.25, 4.9406564584124654e-324, 1.25};
        for (int i = 0; i < 3; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]);
            CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], widths[i]); CHECK_BITS(items[i]->rect[3], heights[i]);
            FlowItem* expectedPrev = nullptr; FlowItem* expectedNext = nullptr;
            for (int j = 0; j < 3; ++j) {
                if (cols[j] + 1 == cols[i]) expectedPrev = items[j].get();
                if (cols[i] + 1 == cols[j]) expectedNext = items[j].get();
            }
            CHECK(items[i]->prev == expectedPrev); CHECK(items[i]->next == expectedNext);
        }
        int lowest = 0;
        for (int i = 1; i < 3; ++i) if (cols[i] < cols[lowest]) lowest = i;
        std::vector<int> expectedMst{ids[lowest]};
        int current = lowest;
        for (int step = 0; step < 2; ++step) {
            int next = -1;
            for (int i = 0; i < 3; ++i) if (cols[i] == cols[current] + 1) next = i;
            if (next < 0) break;
            expectedMst.push_back(ids[next]); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expectedMst);
    }
}

TEST_CASE(three_item_distinct_mixed_cells_preserve_grid_and_dimensions)
{
    struct Route { const char* aCell; const char* bCell; const char* cCell;
                   const char* ax; const char* bx; const char* cx; int aCol; int bCol; int cCol; };
    const Route routes[] = {
        {"", "<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"5\" Col=\"-1\"/>",
         "0", "205", "410", 0, 1, 2},
        {"<CellPos Row=\"0\" Col=\"2\"/>", "<CellPos Row=\"-1\" Col=\"5\"/>",
         "<CellPos Row=\"0\" Col=\"6\"/><CellPos Row=\"5\" Col=\"-1\"/>",
         "0", "0", "205", 2, 0, 1},
        {"<CellPos Row=\"-1\" Col=\"5\"/>", "<CellPos Row=\"0\" Col=\"2\"/>",
         "<CellPos Row=\"-1\" Col=\"2147483647\"/>",
         "-1.7976931348623157e308", "205", "512.5", 0, 2, 3}};
    for (const Route& r : routes) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        const char* xs[] = {r.ax, r.bx, r.cx};
        const char* cells[] = {r.aCell, r.bCell, r.cCell};
        const int ids[] = {41, 52, 73};
        const int cols[] = {r.aCol, r.bCol, r.cCol};
        for (int i = 0; i < 3; ++i) {
            source += "<DiagramItem Operation_Id=\"" + std::to_string(ids[i]) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + xs[i] +
                "</x><y>" + (i == 2 ? std::string("-26") : std::string("0")) +
                "</y><width>" + std::to_string(i + 1) + "</width><height>" +
                std::to_string(-i - 1) + "</height></RectF>" + cells[i] + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == 3u);
        for (int i = 0; i < 3; ++i) {
            CHECK_EQ(items[i]->opId, ids[i]); CHECK_EQ(items[i]->row, 0); CHECK_EQ(items[i]->col, cols[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * cols[i]); CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], static_cast<double>(i + 1));
            CHECK_BITS(items[i]->rect[3], static_cast<double>(-i - 1));
            FlowItem* expectedPrev = nullptr; FlowItem* expectedNext = nullptr;
            for (int j = 0; j < 3; ++j) {
                if (cols[j] + 1 == cols[i]) expectedPrev = items[j].get();
                if (cols[i] + 1 == cols[j]) expectedNext = items[j].get();
            }
            CHECK(items[i]->prev == expectedPrev); CHECK(items[i]->next == expectedNext);
        }
        int lowest = 0;
        for (int i = 1; i < 3; ++i) if (cols[i] < cols[lowest]) lowest = i;
        std::vector<int> expectedMst{ids[lowest]};
        int current = lowest;
        for (int step = 0; step < 2; ++step) {
            int next = -1;
            for (int i = 0; i < 3; ++i) if (cols[i] == cols[current] + 1) next = i;
            if (next < 0) break;
            expectedMst.push_back(ids[next]); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expectedMst);
    }
}

TEST_CASE(three_item_distinct_route_keeps_other_profiles_guarded)
{
    struct Control { const char* cCell; const char* cClass; const char* cx;
                     const char* cy; const char* cw; bool linked; bool fourth; double selectedX; };
    const Control controls[] = {
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "1332.5", "0", "3", false, false, 205.0},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "410", "189", "3", false, false, 205.0},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "410", "0", "nan", false, false, 205.0},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming", "410", "0", "3", false, false, 205.0},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "410", "0", "3", true, false, 205.0},
        {"<CellPos Row=\"0\" Col=\"0\"/>", "SfOprForming2D", "410", "0", "3", false, false, 0.0},
        {"<CellPos Row=\"1\" Col=\"1\"/>", "SfOprForming2D", "410", "0", "3", false, false, 205.0},
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "410", "0", "3", false, true, 205.0}};
    for (const Control& c : controls) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project>") +
            "<DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>0</x><y>0</y><width>1</width><height>-1</height></RectF><CellPos Row=\"0\" Col=\"2\"/>" +
            (c.linked ? "<Connectors><Connections><ItemIndex>1</ItemIndex></Connections></Connectors>" : "") +
            "</DiagramItem><DiagramItem Operation_Id=\"52\"><ClassName>SfOprForming2D</ClassName>"
            "<RectF><x>205</x><y>0</y><width>2</width><height>-2</height></RectF><CellPos Row=\"0\" Col=\"0\"/></DiagramItem>" +
            "<DiagramItem Operation_Id=\"73\"><ClassName>" + c.cClass +
            "</ClassName><RectF><x>" + c.cx + "</x><y>" + c.cy + "</y><width>" + c.cw +
            "</width><height>-3</height></RectF>" + c.cCell + "</DiagramItem>" +
            (c.fourth ? "<DiagramItem Operation_Id=\"84\"><ClassName>SfOprForming2D</ClassName>"
                        "<RectF><x>615</x><y>0</y><width>4</width><height>-4</height></RectF>"
                        "<CellPos Row=\"0\" Col=\"2\"/></DiagramItem>" : "") + "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool outsideSelectedRoute = false;
        try {
            parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() >= 3u);
            outsideSelectedRoute = items[2]->rect[0] != c.selectedX;
        } catch (const Error& e) { outsideSelectedRoute = e.code == M2M_E_UNSUPPORTED; }
        CHECK(outsideSelectedRoute);
    }
}

TEST_CASE(four_to_seven_distinct_targets_relayout_and_rebuild_grid)
{
    const char* widths[] = {"-0", "4.9406564584124654e-324", "-3.5",
                            "1.7976931348623157e308", "2.5", "-7", "0"};
    const char* heights[] = {"3.25", "-4.9406564584124654e-324", "-0",
                             "-1.7976931348623157e308", "-2.5", "7", "1"};
    for (int count = 4; count <= 7; ++count) for (int rotation = 0; rotation < 7; ++rotation)
        for (int mixed = 0; mixed < 2; ++mixed) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        std::vector<int> columns;
        for (int i = 0; i < count; ++i) {
            const int target = (rotation + 3 * i) % 7;
            columns.push_back(target);
            const bool explicitCell = (i + mixed) % 2 == 0;
            const char* y = i == 0 ? "67.49999999999999" :
                (i % 2 == 0 ? "159.99999999999997" : "-1.7976931348623157e308");
            source += "<DiagramItem Operation_Id=\"" + std::to_string(100 + i) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" +
                (explicitCell ? std::string("-1.7976931348623157e308") :
                                std::to_string(205 * target)) +
                "</x><y>" + y + "</y><width>" + widths[i] +
                "</width><height>" + heights[i] + "</height></RectF>";
            if (explicitCell)
                source += "<CellPos Row=\"0\" Col=\"" + std::to_string(target) + "\"/>";
            else if (i % 3 == 0)
                source += "<CellPos Row=\"2147483647\" Col=\"-1\"/>";
            else if (i % 3 == 1)
                source += "<CellPos Row=\"-1\" Col=\"-2147483648\"/>";
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; parseSubProject(p, *doc.root, ctx);
        const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == static_cast<std::size_t>(count));
        for (int i = 0; i < count; ++i) {
            CHECK_EQ(items[i]->opId, 100 + i); CHECK_EQ(items[i]->row, 0);
            CHECK_EQ(items[i]->col, columns[i]);
            CHECK_BITS(items[i]->rect[0], 205.0 * columns[i]);
            CHECK_BITS(items[i]->rect[1], 0.0);
            CHECK_BITS(items[i]->rect[2], std::strtod(widths[i], nullptr));
            CHECK_BITS(items[i]->rect[3], std::strtod(heights[i], nullptr));
            FlowItem* prev = nullptr; FlowItem* next = nullptr;
            for (int j = 0; j < count; ++j) {
                if (columns[j] + 1 == columns[i]) prev = items[j].get();
                if (columns[i] + 1 == columns[j]) next = items[j].get();
            }
            CHECK(items[i]->prev == prev); CHECK(items[i]->next == next);
        }
        int lowest = 0;
        for (int i = 1; i < count; ++i) if (columns[i] < columns[lowest]) lowest = i;
        std::vector<int> expectedMst{100 + lowest};
        int current = lowest;
        for (int step = 1; step < count; ++step) {
            int next = -1;
            for (int i = 0; i < count; ++i)
                if (columns[i] == columns[current] + 1) next = i;
            if (next < 0) break;
            expectedMst.push_back(100 + next); current = next;
        }
        CHECK(mstOrder(*p.subs[0]) == expectedMst);
    }
}

TEST_CASE(four_to_seven_distinct_route_keeps_collision_and_admission_guarded)
{
    struct Control { int count; int changed; const char* x; const char* y;
                     const char* width; const char* cell; };
    const Control controls[] = {
        {4, 1, "123", "189", "1", "<CellPos Row=\"0\" Col=\"1\"/>"},
        {4, 0, "10", "67.5", "1", "<CellPos Row=\"0\" Col=\"0\"/>"},
        {5, 2, "1332.5", "0", "1", "<CellPos Row=\"0\" Col=\"2\"/>"},
        {6, 3, "614", "0", "nan", "<CellPos Row=\"0\" Col=\"3\"/>"},
        {7, 6, "1230", "0", "1", "<CellPos Row=\"0\" Col=\"0\"/>"}};
    for (const Control& c : controls) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < c.count; ++i) {
            const std::string x = i == c.changed ? c.x : std::to_string(205 * i);
            const std::string y = i == c.changed ? c.y : "0";
            const std::string width = i == c.changed ? c.width : "1";
            const std::string cell = i == c.changed ? c.cell :
                "<CellPos Row=\"0\" Col=\"" + std::to_string(i) + "\"/>";
            source += "<DiagramItem Operation_Id=\"" + std::to_string(100 + i) +
                "\"><ClassName>SfOprForming2D</ClassName><RectF><x>" + x +
                "</x><y>" + y + "</y><width>" + width +
                "</width><height>1</height></RectF>" + cell + "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool outsideSelectedRoute = false;
        try {
            parseSubProject(p, *doc.root, ctx);
            const auto& items = p.subs[0]->flow.items; REQUIRE(items.size() == static_cast<std::size_t>(c.count));
            outsideSelectedRoute = items[c.changed]->rect[0] != 205.0 * items[c.changed]->col;
        } catch (const Error& e) { outsideSelectedRoute = e.code == M2M_E_UNSUPPORTED; }
        CHECK(outsideSelectedRoute);
    }
}

TEST_CASE(nonempty_initial_chart_does_not_enter_cell_recurrence)
{
    Project p; p.subs.push_back(makeSubProject(p, 0, 0));
    auto existing = std::make_unique<FlowItem>();
    existing->className = L("SfOprForming2D");
    existing->row = 0; existing->col = 0;
    p.subs[0]->flow.items.push_back(std::move(existing));
    std::string source = "<Sub_Project>";
    for (int i = 0; i < 4; ++i)
        source += "<DiagramItem Operation_Id=\"" + std::to_string(i + 1) + "\"><ClassName>SfOprForming2D</ClassName>"
                  "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>";
    source += "</Sub_Project>";
    xml::Document doc; String error; int line = 0, col = 0;
    REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
    Ctx ctx; bool refused = false;
    try { parseSubProject(p, *doc.root, ctx); }
    catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED &&
        std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos; }
    CHECK(refused);
}

TEST_CASE(three_cellless_unsupported_patterns_remain_guarded)
{
    struct Case { const char* cls; const char* cell; int x; bool linked; };
    const Case cases[] = {{"SfOprForming2D", "", 1, false},
                          {"SfOprForming", "", 0, false},
                          {"SfOprGroup", "", 0, false},
                          {"SfOprForming2D", "<CellPos Row=\"-1\" Col=\"0\"/>", 0, false},
                          {"SfOprForming2D", "<CellPos Row=\"0\" Col=\"0\"/>", 0, false},
                          {"SfOprForming2D", "", 0, true}};
    for (const Case& c : cases) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        std::string source = "<Sub_Project>";
        for (int i = 0; i < 3; ++i) {
            const char* cls = (std::string(c.cls) == "SfOprGroup" && i != 1) ? "SfOprForming2D" : c.cls;
            source += "<DiagramItem Operation_Id=\"" + std::to_string(i + 1) + "\"><ClassName>" + cls + "</ClassName>"
                      "<RectF><x>" + std::to_string(i == 1 ? c.x : 0) + "</x><y>0</y><width>0</width><height>0</height></RectF>";
            if (c.linked) source += i == 0 ? "<Connectors><Connections><ItemIndex>1</ItemIndex></Connections></Connectors>"
                                           : "<Connectors><Connections><ItemIndex>-1</ItemIndex></Connections></Connectors>";
            if (i == 1) source += c.cell;
            source += "</DiagramItem>";
        }
        source += "</Sub_Project>";
        xml::Document doc; String error; int line = 0, col = 0;
        REQUIRE(xml::setContent(doc, source, &error, &line, &col)); REQUIRE(doc.root != nullptr);
        Ctx ctx; bool refused = false;
        try { parseSubProject(p, *doc.root, ctx); }
        catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED &&
            std::string(e.what()).find("on an occupied cell (0x14015b610)") != std::string::npos; }
        if (c.linked) {
            REQUIRE(p.subs[0]->flow.items.size() == 3u);
            CHECK(p.subs[0]->flow.items[0]->next == p.subs[0]->flow.items[1].get());
            CHECK(p.subs[0]->flow.items[1]->prev == p.subs[0]->flow.items[0].get());
        }
        if (std::string(c.cls) == "SfOprGroup") {
            REQUIRE(p.subs[0]->flow.items.size() == 3u);
            CHECK(p.subs[0]->flow.items[1]->group);
        }
        CHECK(refused);
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
