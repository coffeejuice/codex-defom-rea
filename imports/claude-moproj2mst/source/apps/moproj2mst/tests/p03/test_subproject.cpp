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
        {"nonzero top", 0, 0, 1, 1, 0, 0, "", "", -1, -1, "SfOprForming2D"},
        {"nonzero width", 0, 0, 1, 0, 1, 0, "", "", -1, -1, "SfOprForming2D"},
        {"nonzero height", 0, 0, 1, 0, 0, 1, "", "", -1, -1, "SfOprForming2D"},
        {"forward linked", 0, 0, 1, 0, 0, 0, "", "", 1, -1, "SfOprForming2D"},
        {"reverse linked", 0, 0, 1, 0, 0, 0, "", "", -1, 0, "SfOprForming2D"},
        {"self linked", 0, 0, 1, 0, 0, 0, "", "", 0, -1, "SfOprForming2D"},
        {"partial A cell", 0, 0, 1, 0, 0, 0, "<CellPos Row=\"-1\" Col=\"0\"/>", "", -1, -1, "SfOprForming2D"},
        {"valid A cell", 0, 0, 1, 0, 0, 0, "<CellPos Row=\"0\" Col=\"0\"/>", "", -1, -1, "SfOprForming2D"},
        {"other class", 0, 0, 1, 0, 0, 0, "", "", -1, -1, "SfOprForming"},
    };
    for (const Case& c : cases) {
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
