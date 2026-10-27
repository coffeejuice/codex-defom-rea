// P03: sub-project activation and parse (0x1400a7b90, 0x14008f530, 0x140091f00, 0x1400a78e0) on
// every corpus project, through the real XML seam. Until P01 (QDomDocument) and P04
// (QString::toInt/contains) merge, their stubs throw NotPorted and the cases report SKIP.
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

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
    struct Control { const char* name; const char* aw; const char* bw; const char* bh;
                     const char* bCell; const char* bx; };
    const Control controls[] = {
        {"nonfinite width", "0", "nan", "0", "", "1"},
        {"nonfinite height", "0", "0", "inf", "", "1"},
        {"nonzero A dimensions", "1", "0", "0", "", "1"},
        {"exact upper x endpoint", "0", "0", "0", "", "1332.5"},
    };
    for (const Control& c : controls) {
        Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
        const std::string source = std::string("<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>") +
            "<RectF><x>0</x><y>0</y><width>" + c.aw + "</width><height>0</height></RectF></DiagramItem>" +
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
    // Exact 160 is outside the shared row-zero cut, even though taller native graphics may admit it.
    Project p; makeRegistry(p); p.subs.push_back(makeSubProject(p, 0, 0));
    const std::string source = "<Sub_Project><DiagramItem Operation_Id=\"41\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>0</x><y>0</y><width>0</width><height>0</height></RectF></DiagramItem>"
        "<DiagramItem Operation_Id=\"73\"><ClassName>SfOprForming2D</ClassName>"
        "<RectF><x>1</x><y>160</y><width>2.5</width><height>-3.25</height></RectF></DiagramItem></Sub_Project>";
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
        {"<CellPos Row=\"0\" Col=\"0\"/>", "<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "67.5", "2", false, 205.0},
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
        {"<CellPos Row=\"0\" Col=\"1\"/>", "SfOprForming2D", "67.5", "2", false},
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
