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
    Type3OptGroupReceipt* receipt = nullptr;
    bool groupReadyAtSuffix = false;
    bool open(const std::string& path) override { events.push_back("open:" + path); return opened; }
    bool typed(const std::string& event) { events.push_back(event); if (event == throwOn) throw InsertStopped{}; return insertStatus; }
    bool insertLiteral(const char* s) override { return typed(std::string("literal:") + s); }
    bool insertInt(std::int32_t n) override { return typed("int:" + std::to_string(n)); }
    bool insertEndl() override { return typed("endl"); }
    bool insert(const std::string& s) override { events.push_back("insert:" + s); if (s == "suffix" && receipt) groupReadyAtSuffix = receipt->ignoredCounts[0] == 2; if (s == throwOn) throw InsertStopped{}; return insertStatus; }
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
    p.krgMax = 123; p.oprNo = -456; p.scalarPrefix = {"prefix"};
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
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "literal:DEF_OPT       \t1.0", "endl", "literal:TYP_OPT       \t1", "endl",
        "literal:GRD_TYP  \t\t10", "endl", "literal:KRG_MAX       \t", "int:123", "endl",
        "literal:OPR_NO        \t", "int:-456", "endl", "endl",
        "insert:prefix", "insert:suffix", "insert:row0", "insert:row1",
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
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "literal:DEF_OPT       \t1.0", "endl", "literal:TYP_OPT       \t1", "endl",
        "literal:GRD_TYP  \t\t10", "endl", "literal:KRG_MAX       \t", "int:123", "endl",
        "literal:OPR_NO        \t", "int:-456", "endl", "endl",
        "insert:prefix", "insert:suffix", "insert:zero", "insert:empty", "insert:final", "insert:closing", "close"}));
    for (auto gate : {0, 1, 2}) {
        t.events.clear(); attemptType3SaveCaller(gate == 1, gate == 2, p, t, t);
        CHECK(t.events == std::vector<std::string>({"common"}));
    }
}

TEST_CASE(type3_opt_save_throwing_insert_stops_caller_at_observed_prefix)
{
    auto p = prepared(); Trace t; t.throwOn = "int:123";
    bool caught = false;
    try {
        attemptType3SaveCaller(true, true, p, t, t);
    } catch (const InsertStopped&) {
        caught = true;
    }
    CHECK(caught);
    CHECK(t.events == std::vector<std::string>({"study", "input", "open:x.OPT",
                                                "literal:DEF_OPT       \t1.0", "endl", "literal:TYP_OPT       \t1", "endl",
                                                "literal:GRD_TYP  \t\t10", "endl", "literal:KRG_MAX       \t", "int:123"}));
}
TEST_CASE(type3_opt_group_phase_uses_field_values_in_temporary_snapshots)
{
    auto p = prepared(); Trace t; Type3OptimizerScalarState state;
    state.numConRows = {
        {0, 42, 101, 102, 0.0, {}}, {1, -7, 103, 104, 0.0, {}},
        {0, 42, 105, 106, 0.0, {}}, {9, 88, 107, 108, 0.0, {}}
    };
    Type3OptGroupReceipt receipt; t.receipt = &receipt;
    CHECK(!attemptType3OptSaveWithGroups(p, state, t, receipt));
    CHECK(receipt.temporarySnapshots[0] == std::vector<std::int32_t>({42, 42}));
    CHECK(receipt.temporarySnapshots[1] == std::vector<std::int32_t>({-7}));
    CHECK(receipt.temporarySnapshots[2].empty());
    CHECK_EQ(receipt.ignoredCounts[0], std::size_t(2));
    CHECK_EQ(receipt.ignoredCounts[1], std::size_t(1));
    CHECK_EQ(receipt.ignoredCounts[2], std::size_t(0));
    for (std::size_t i = 0; i < 3; ++i) {
        CHECK(receipt.writerOriginals[i].empty());
        CHECK(!receipt.preliminaryBranchTaken[i]);
    }
    CHECK(t.groupReadyAtSuffix);
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "literal:DEF_OPT       \t1.0", "endl", "literal:TYP_OPT       \t1", "endl",
        "literal:GRD_TYP  \t\t10", "endl", "literal:KRG_MAX       \t", "int:123", "endl",
        "literal:OPR_NO        \t", "int:-456", "endl", "endl",
        "insert:prefix", "insert:suffix", "insert:row0", "insert:row1", "insert:count",
        "insert:number1", "insert:number2", "insert:section0", "insert:section1", "insert:final",
        "insert:closing", "close"}));
}

TEST_CASE(type3_opt_group_phase_does_not_run_on_failed_open_or_throwing_prefix)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.numConRows = {{0, 42, 0, 0, 0.0, {}}};
    Type3OptGroupReceipt receipt; receipt.ignoredCounts[0] = 99;
    receipt.temporarySnapshots[0] = {123};
    Trace t; t.opened = false;
    CHECK(!attemptType3OptSaveWithGroups(p, state, t, receipt));
    CHECK_EQ(receipt.ignoredCounts[0], std::size_t(99));
    CHECK(receipt.temporarySnapshots[0] == std::vector<std::int32_t>({123}));
    t.opened = true; t.throwOn = "int:123"; t.events.clear();
    bool typedCaught = false;
    try { (void)attemptType3OptSaveWithGroups(p, state, t, receipt); }
    catch (const InsertStopped&) { typedCaught = true; }
    CHECK(typedCaught);
    CHECK_EQ(receipt.ignoredCounts[0], std::size_t(99));
    CHECK(receipt.temporarySnapshots[0] == std::vector<std::int32_t>({123}));
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "literal:DEF_OPT       \t1.0", "endl",
        "literal:TYP_OPT       \t1", "endl", "literal:GRD_TYP  \t\t10", "endl",
        "literal:KRG_MAX       \t", "int:123"}));
    t.opened = true; t.throwOn = "prefix"; t.events.clear();
    bool caught = false;
    try { (void)attemptType3OptSaveWithGroups(p, state, t, receipt); }
    catch (const InsertStopped&) { caught = true; }
    CHECK(caught);
    CHECK_EQ(receipt.ignoredCounts[0], std::size_t(99));
    CHECK(receipt.temporarySnapshots[0] == std::vector<std::int32_t>({123}));
    CHECK(t.events == std::vector<std::string>({"open:x.OPT", "literal:DEF_OPT       \t1.0", "endl", "literal:TYP_OPT       \t1", "endl",
        "literal:GRD_TYP  \t\t10", "endl", "literal:KRG_MAX       \t", "int:123", "endl",
        "literal:OPR_NO        \t", "int:-456", "endl", "endl", "insert:prefix"}));
}
int main() { return testkit::run_all(); }
