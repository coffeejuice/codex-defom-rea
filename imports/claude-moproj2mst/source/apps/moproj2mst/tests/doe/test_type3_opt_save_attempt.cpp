#include <testkit/check.hpp>
#include "../../src/ops/type3_opt_save_attempt.hpp"
#include <string>
#include <vector>

using namespace m2m::mst::detail;
namespace {
struct InsertStopped {};
struct Trace : Type3OptAttemptSink, Type3OptCallerEffects {
    std::vector<std::string> events;
    bool opened = true, insertStatus = true, closeStatus = true;
    std::string throwOn;
    bool open(const std::string& path) override { events.push_back("open:" + path); return opened; }
    bool insert(const std::string& s) override { events.push_back("insert:" + s); if (s == throwOn) throw InsertStopped{}; return insertStatus; }
    bool close() override { events.push_back("close"); return closeStatus; }
    void assignOpenFailureDiagnostic(const std::string& p) override { events.push_back("diagnostic:" + p); }
    void appendStudy() override { events.push_back("study"); }
    void saveInputTable() override { events.push_back("input"); }
    void commonContinuation() override { events.push_back("common"); }
};
Type3OptSavePrepared prepared()
{
    Type3OptSavePrepared p;
    p.path = "x.OPT";
    p.header = {"h1", "h2", "h3"}; p.scalarPrefix = {"prefix"};
    p.collections[0] = {true, {"first0"}};
    p.collections[1] = {false, {"must skip"}};
    p.collections[2] = {true, {"first2"}};
    p.scalarSuffix = {"suffix"}; p.rows = {{"row0"}, {"row1"}};
    p.managerCountAndNumbers = {"count", "number1", "number2"};
    p.emptySection = {"empty"}; p.sectionRows = {{"section0"}, {"section1"}};
    p.finalLines = {"final", "closing"};
    return p;
}
}
TEST_CASE(type3_opt_save_failed_open_returns_false_and_caller_continues)
{
    auto p = prepared(); Trace t; t.opened = false;
    CHECK(!attemptType3OptSave(p, t));
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "diagnostic:x.OPT"}));
    t.events.clear();
    attemptType3SaveCaller(true, true, p, t, t);
    CHECK(t.events == std::vector<std::string>({"study", "input", "open:x.OPT", "diagnostic:x.OPT", "common"}));
}
TEST_CASE(type3_opt_save_full_order_ignores_nonthrowing_sink_status)
{
    auto p = prepared(); Trace t; t.insertStatus = false; t.closeStatus = false;
    CHECK(!attemptType3OptSave(p, t));
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "insert:h1", "insert:h2", "insert:h3",
        "insert:prefix", "insert:first0", "insert:first2", "insert:suffix", "insert:row0", "insert:row1",
        "insert:count", "insert:number1", "insert:number2", "insert:section0", "insert:section1",
        "insert:final", "insert:closing", "close"}));
    t.events.clear(); attemptType3SaveCaller(true, true, p, t, t);
    CHECK_EQ(t.events.front(), std::string("study"));
    CHECK_EQ(t.events[t.events.size()-2], std::string("close"));
    CHECK_EQ(t.events.back(), std::string("common"));
}
TEST_CASE(type3_opt_save_sparse_section_and_borrowed_pointer_gates)
{
    auto p = prepared(); p.collections = {}; p.rows.clear(); p.sectionRows.clear();
    p.managerCountAndNumbers = {"zero"}; Trace t;
    CHECK(!attemptType3OptSave(p, t));
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "insert:h1", "insert:h2", "insert:h3",
        "insert:prefix", "insert:suffix", "insert:zero", "insert:empty", "insert:final", "insert:closing", "close"}));
    for (auto gate : {0, 1, 2}) {
        t.events.clear(); attemptType3SaveCaller(gate == 1, gate == 2, p, t, t);
        CHECK(t.events == std::vector<std::string>({"common"}));
    }
}

TEST_CASE(type3_opt_save_throwing_insert_stops_caller_at_observed_prefix)
{
    auto p = prepared(); Trace t; t.throwOn = "h2";
    bool caught = false;
    try {
        attemptType3SaveCaller(true, true, p, t, t);
    } catch (const InsertStopped&) {
        caught = true;
    }
    CHECK(caught);
    CHECK(t.events == std::vector<std::string>({"study", "input", "open:x.OPT",
                                                "insert:h1", "insert:h2"}));
}
int main() { return testkit::run_all(); }
