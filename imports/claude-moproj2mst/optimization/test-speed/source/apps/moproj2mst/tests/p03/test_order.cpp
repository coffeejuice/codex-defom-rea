// P03: flow-chart order (0x1400b6630), chain numbers (0x140085800), predecessors (0x140086720),
// cycle members (0x1400b7470), and the operation order of every corpus MST (s1-01 8.3).
//
// The corpus check builds each flow chart the way 0x14008f530/0x140091f00 do, with a small XML
// reader in this file, so it runs before P01 (XML) and P04 (QString::toInt) merge.
// test_subproject.cpp runs the same check through the real parse once they are in.
#include <testkit/check.hpp>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "m2m/model.hpp"
#include "order_corpus.hpp"

using namespace m2m;
using m2m::qt::L;

namespace {

FlowItem* add(FlowChart& f, int id, int row, int col)
{
    auto it = std::make_unique<FlowItem>();
    it->opId = id;
    it->row = row;
    it->col = col;
    FlowItem* p = it.get();
    f.items.push_back(std::move(it));
    return p;
}

FlowItem* addGroup(FlowChart& f, int id, int r0, int c0, int r1, int c1)
{
    FlowItem* g = add(f, id, 0, 0);
    g->group = true;
    g->beginRow = r0;
    g->beginCol = c0;
    g->endRow = r1;
    g->endCol = c1;
    return g;
}

void link(FlowItem* a, FlowItem* b)
{
    a->next = b;
    b->prev = a;
}

std::string str(const std::vector<int>& v)
{
    std::string s;
    for (int x : v) s += (s.empty() ? "" : ",") + std::to_string(x);
    return s;
}

}  // namespace

TEST_CASE(order_plain_chain_from_any_cell)
{
    SubProject sub;
    // creation order differs from both cell order and chain order
    FlowItem* c = add(sub.flow, 30, 0, 5);
    FlowItem* a = add(sub.flow, 10, 0, 9);   // chain head, but not the first cell
    FlowItem* b = add(sub.flow, 20, 0, 1);
    link(a, b);
    link(b, c);
    CHECK_EQ(str(mstOrder(sub)), std::string("10,20,30"));
    CHECK_EQ(flowChainNumber(sub.flow, 10), 1);
    CHECK_EQ(flowChainNumber(sub.flow, 30), 3);
    CHECK_EQ(flowChainNumber(sub.flow, 99), 0);
    CHECK_EQ(flowPredecessor(sub.flow, 10), -1);
    CHECK_EQ(flowPredecessor(sub.flow, 30), 20);
    CHECK_EQ(flowPredecessor(sub.flow, 99), -1);
}

TEST_CASE(order_unlinked_items_are_dropped)
{
    SubProject sub;
    FlowItem* a = add(sub.flow, 1, 0, 0);
    FlowItem* b = add(sub.flow, 2, 0, 1);
    add(sub.flow, 3, 5, 5);                  // not connected: not written
    link(a, b);
    CHECK_EQ(str(mstOrder(sub)), std::string("1,2"));
}

TEST_CASE(order_group_collapses_to_group_id)
{
    SubProject sub;
    FlowItem* a = add(sub.flow, 1, 0, 0);
    FlowItem* m1 = add(sub.flow, 2, 1, 0);
    FlowItem* m2 = add(sub.flow, 3, 1, 1);
    FlowItem* d = add(sub.flow, 4, 2, 0);
    addGroup(sub.flow, 7, 1, 0, 1, 3);
    link(a, m1);
    link(m1, m2);
    link(m2, d);
    CHECK_EQ(str(mstOrder(sub)), std::string("1,7,4"));
    CHECK_EQ(str(groupMembers(sub, 7)), std::string("2,3"));
    CHECK_EQ(str(groupMembers(sub, 99)), std::string(""));
    CHECK_EQ(flowChainNumber(sub.flow, 7), 2);
    CHECK_EQ(flowChainNumber(sub.flow, 2), 2);
    CHECK_EQ(flowChainNumber(sub.flow, 3), 2);
    CHECK_EQ(flowChainNumber(sub.flow, 4), 3);
}

TEST_CASE(order_group_range_is_row_major_inclusive)
{
    SubProject sub;
    FlowItem* a = add(sub.flow, 1, 1, 5);    // row 1 from col 5: inside
    FlowItem* b = add(sub.flow, 2, 2, 0);    // whole middle row: inside
    FlowItem* c = add(sub.flow, 3, 3, 2);    // last row up to col 2: inside
    FlowItem* d = add(sub.flow, 4, 3, 3);    // past the end: outside
    FlowItem* e = add(sub.flow, 5, 1, 4);    // before the begin: outside
    addGroup(sub.flow, 9, 1, 5, 3, 2);
    link(e, a);
    link(a, b);
    link(b, c);
    link(c, d);
    CHECK_EQ(str(mstOrder(sub)), std::string("5,9,4"));
    CHECK_EQ(str(groupMembers(sub, 9)), std::string("1,2,3"));
}

TEST_CASE(order_later_group_overwrites_and_runs_repeat)
{
    SubProject sub;
    FlowItem* a = add(sub.flow, 1, 0, 0);
    FlowItem* b = add(sub.flow, 2, 0, 1);
    FlowItem* c = add(sub.flow, 3, 0, 2);
    FlowItem* d = add(sub.flow, 4, 0, 3);
    addGroup(sub.flow, 20, 0, 1, 0, 2);      // begins later: overwrites 2 and 3
    addGroup(sub.flow, 10, 0, 0, 0, 3);      // begins first: 1..4
    // chain 1 2 4 3: group 10, then 20 (members 2), 10 again (4), 20 again (3)
    link(a, b);
    link(b, d);
    link(d, c);
    CHECK_EQ(str(mstOrder(sub)), std::string("10,20,10,20"));
}

TEST_CASE(order_empty_and_groups_only)
{
    SubProject sub;
    CHECK_EQ(str(mstOrder(sub)), std::string(""));
    addGroup(sub.flow, 1, 0, 0, 5, 5);
    CHECK_EQ(str(mstOrder(sub)), std::string(""));
    CHECK_EQ(str(groupMembers(sub, 1)), std::string(""));
}

TEST_CASE(order_loop_is_an_error)
{
    SubProject sub;
    FlowItem* a = add(sub.flow, 1, 0, 0);
    FlowItem* b = add(sub.flow, 2, 0, 1);
    link(a, b);
    link(b, a);
    bool threw = false;
    try {
        mstOrder(sub);
    } catch (const Error& e) {
        threw = e.code == M2M_E_PARSE;
    }
    CHECK(threw);
}

TEST_CASE(order_qsort_picks_the_same_head_as_qt)
{
    // Equal cells: qSort is not stable, so the first sorted item (the walk's start) follows Qt
    // 4.8's qSortHelper. With three equal keys the median-of-three swaps nothing and the first
    // created item stays first; the chain head is then found by walking back anyway.
    SubProject sub;
    FlowItem* a = add(sub.flow, 1, 0, 0);
    FlowItem* b = add(sub.flow, 2, 0, 0);
    FlowItem* c = add(sub.flow, 3, 0, 0);
    link(c, a);
    link(a, b);
    CHECK_EQ(str(mstOrder(sub)), std::string("3,1,2"));
}

TEST_CASE(order_number_cache_is_kept)
{
    SubProject sub;
    FlowItem* a = add(sub.flow, 1, 0, 0);
    FlowItem* b = add(sub.flow, 2, 0, 1);
    link(a, b);
    CHECK_EQ(flowChainNumber(sub.flow, 2), 2);
    a->next = nullptr;   // reverse the chain: a rebuild would number 2 first
    b->prev = nullptr;
    link(b, a);
    CHECK_EQ(flowChainNumber(sub.flow, 1), 1);   // the cache (model+0x40) answers
    CHECK_EQ(str(mstOrder(sub)), std::string("2,1"));
    CHECK(sub.flow.numberCacheBuilt);
}

TEST_CASE(order_every_corpus_mst)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    int checked = 0;
    for (const p03test::Case& c : p03test::corpusCases(corpus)) {
        p03test::Project px = p03test::load(c.moproj);
        SubProject sub;
        p03test::buildFlow(px, sub);
        std::vector<std::string> got;
        for (int id : mstOrder(sub)) p03test::pushRun(got, px.names.count(id) ? px.names[id] : "?" + std::to_string(id));
        const std::vector<std::string> want = p03test::oprnamRuns(c.mst);
        if (got != want) {
            testkit::fail(__FILE__, __LINE__, c.name + ": order differs: got " + p03test::join(got) +
                                                  " want " + p03test::join(want));
        }
        ++checked;
    }
    std::printf("corpus: %d MSTs checked\n", checked);
    CHECK(checked >= 40);
}

int main()
{
    return testkit::run_all();
}
