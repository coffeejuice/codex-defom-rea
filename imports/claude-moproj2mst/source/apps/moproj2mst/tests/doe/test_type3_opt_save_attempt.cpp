#include <testkit/check.hpp>
#include "../../src/ops/type3_opt_save_attempt.hpp"
#include "../../src/ops/type3_optimizer_defopt_view.hpp"
#include <m2m/mst/doe_model.hpp>
#include <algorithm>
#include <cstring>
#include <cstdint>
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
    Type3OptimizerScalarState* mutateState = nullptr;
    bool changeCountOnSecondTypObjEndl = false;
    int typObjEndls = 0;
    bool inTypObj = false;
    bool changeCountOnNumConLiteral = false;
    bool mutateRowOnFirstInsert = false, mutateSecondRowAfterFirstEndl = false;
    bool inNumConRow = false;
    int rowEndls = 0;
    bool open(const std::string& path) override { events.push_back("open:" + path); return opened; }
    bool typed(const std::string& event) { events.push_back(event); if (event == throwOn) throw InsertStopped{}; return insertStatus; }
    bool insertLiteral(const char* s) override {
        const bool result = typed(std::string("literal:") + s);
        if (std::string(s) == "TYP_OBJ       ") inTypObj = true;
        if (mutateState && std::string(s) == "TYP_OBJ       ") {
            mutateState->value30 = 300; mutateState->value10 = 100; mutateState->value34 = 340;
        }
        if (mutateState && std::string(s) == "NUM_CON\t\t" && changeCountOnNumConLiteral)
            mutateState->numConRows.push_back({0, 7, 0, 0, 0.0, {}});
        if (mutateState && std::string(s) == "  ") {
            inNumConRow = true;
            if (mutateRowOnFirstInsert) {
                mutateState->numConRows[0] = {0, 111, 222, 333, 444.5, {}};
                mutateRowOnFirstInsert = false;
            }
        }
        return result;
    }
    bool insertInt(std::int32_t n) override { return typed("int:" + std::to_string(n)); }
    bool insertDouble(double n) override { std::uint64_t bits = 0; std::memcpy(&bits, &n, sizeof(bits)); return typed("double:" + std::to_string(bits)); }
    bool insertString(const m2m::qt::String& s) override { return typed("qstring:" + s.utf8()); }
    bool insertEndl() override {
        const bool result = typed("endl");
        if (inNumConRow && mutateState) {
            ++rowEndls;
            inNumConRow = false;
            if (rowEndls == 1 && mutateSecondRowAfterFirstEndl)
                mutateState->numConRows[1] = {0, 211, 222, 233, 244.5, {}};
        }
        if (mutateState && inTypObj && changeCountOnSecondTypObjEndl && typObjEndls < 2) {
            ++typObjEndls;
            if (typObjEndls == 2) {
                mutateState->numConRows.push_back({0, 7, 0, 0, 0.0, {}});
                inTypObj = false;
            }
        }
        return result;
    }
    bool insert(const std::string& s) override { events.push_back("insert:" + s); if (s == "suffix" && receipt) groupReadyAtSuffix = receipt->ignoredCounts[0] == 2; if (mutateState && s == "prefix") mutateState->value34 = 341; if (s == throwOn) throw InsertStopped{}; return insertStatus; }
    bool close() override { events.push_back("close"); return closeStatus; }
    void assignOpenFailureDiagnostic(const std::string& p) override { events.push_back("diagnostic:" + p); }
    void appendStudy() override { events.push_back("study"); }
    void saveInputTable() override { events.push_back("input"); }
    void commonContinuation() override { events.push_back("common"); }
};
struct Manager : m2m::doe::Model {
    std::vector<std::string>* events = nullptr;
    int scans = 0;
    std::vector<m2m::doe::Var*> geometryVars(int, int) const override { return {}; }
    std::vector<m2m::doe::Var*> unlinkedVars() override {
        ++scans;
        if (events) events->push_back("manager-scan");
        return Model::unlinkedVars();
    }
};
std::size_t position(const std::vector<std::string>& events, const std::string& sought)
{
    const auto it = std::find(events.begin(), events.end(), sought);
    return static_cast<std::size_t>(it - events.begin());
}
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

TEST_CASE(type3_opt_live_sections_scan_at_late_position_and_emit_typed_rows)
{
    auto p = prepared();
    Type3OptimizerScalarState state;
    appendType3RegionPreparedRow(state, m2m::qt::L("first"));
    appendType3RegionPreparedRow(state, m2m::qt::L("second"));
    m2m::doe::Var first, linked, second;
    first.index = 91; linked.index = 92; second.index = 93;
    linked.link = &first;
    Manager manager; manager.vars = {&first, &linked, &second};
    Trace t; manager.events = &t.events; t.insertStatus = false; t.closeStatus = false;
    Type3OptGroupReceipt receipt;
    attemptType3SaveCallerWithTypedSections(true, &manager, p, state, t, t, receipt);
    CHECK(manager.scans == 1);
    CHECK(first.index == 0 && linked.index == 1 && second.index == 2);
    CHECK(manager.vars == std::vector<m2m::doe::Var*>({&first, &linked, &second}));
    CHECK(position(t.events, "endl") < position(t.events, "manager-scan"));
    const std::vector<std::string> section = {
        "manager-scan", "literal:DOE_PAR    \t\t", "int:2", "endl",
        "int:1", "literal:   0.0  1.0  0.5  0.01   0", "endl",
        "int:2", "literal:   0.0  1.0  0.5  0.01   0", "endl", "endl",
        "literal:REGIONS    \t\t", "int:2", "endl"
    };
    const auto sectionStart = std::find(t.events.begin(), t.events.end(), "manager-scan");
    REQUIRE(sectionStart != t.events.end());
    REQUIRE(static_cast<std::size_t>(t.events.end() - sectionStart) >= section.size());
    CHECK(std::equal(section.begin(), section.end(), sectionStart));
    CHECK(position(t.events, "literal:REGIONS    \t\t") < position(t.events, "qstring:Opt/first.GEO"));
    CHECK(position(t.events, "qstring:Opt/first.GEO") < position(t.events, "qstring:Opt/second.GEO"));
    CHECK(position(t.events, "qstring:Opt/second.GEO") < position(t.events, "insert:final"));
    CHECK(position(t.events, "insert:final") < position(t.events, "close"));
    CHECK(t.events.back() == "common");
    CHECK(position(t.events, "insert:count") == t.events.size());
    CHECK(position(t.events, "insert:section0") == t.events.size());
}

TEST_CASE(type3_opt_live_sections_empty_direct_return_and_missing_caller_gates)
{
    auto p = prepared(); p.rows.clear();
    Type3OptimizerScalarState state;
    Manager manager; Trace t; manager.events = &t.events;
    Type3OptGroupReceipt receipt;
    CHECK(!attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt));
    CHECK(manager.scans == 1);
    const std::vector<std::string> section = {
        "manager-scan", "literal:DOE_PAR    \t\t", "int:0", "endl", "endl",
        "literal:REGIONS\t\t0", "endl", "insert:final", "insert:closing", "close"
    };
    const auto sectionStart = std::find(t.events.begin(), t.events.end(), "manager-scan");
    REQUIRE(sectionStart != t.events.end());
    REQUIRE(static_cast<std::size_t>(t.events.end() - sectionStart) == section.size());
    CHECK(std::equal(section.begin(), section.end(), sectionStart));
    t.events.clear();
    attemptType3SaveCallerWithTypedSections(true, nullptr, p, state, t, t, receipt);
    CHECK(t.events == std::vector<std::string>({"common"}));
    t.events.clear();
    attemptType3SaveCallerWithTypedSections(false, &manager, p, state, t, t, receipt);
    CHECK(t.events == std::vector<std::string>({"common"}));
    CHECK(manager.scans == 1);
}

TEST_CASE(type3_opt_live_sections_failed_open_and_earlier_throw_do_not_scan)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.numConRows = {{0, 9, 8, 7, 6.0, {}}};
    m2m::doe::Var variable; variable.index = 77;
    Manager manager; manager.vars = {&variable};
    Type3OptGroupReceipt receipt; Trace t; manager.events = &t.events; t.opened = false;
    attemptType3SaveCallerWithTypedSections(true, &manager, p, state, t, t, receipt);
    CHECK(manager.scans == 0 && variable.index == 77);
    CHECK(t.events == std::vector<std::string>({"study", "input", "open:x.OPT", "diagnostic:x.OPT", "common"}));
    t.events.clear(); t.opened = true; t.throwOn = "literal:  ";
    bool caught = false;
    try { attemptType3SaveCallerWithTypedSections(true, &manager, p, state, t, t, receipt); }
    catch (const InsertStopped&) { caught = true; }
    CHECK(caught && manager.scans == 0 && variable.index == 77);
    CHECK(position(t.events, "manager-scan") == t.events.size());
    CHECK(position(t.events, "common") == t.events.size());
}

TEST_CASE(type3_opt_live_sections_count_throw_keeps_scan_but_stops_region)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    appendType3RegionPreparedRow(state, m2m::qt::L("later"));
    m2m::doe::Var variable; variable.index = 77;
    Manager manager; manager.vars = {&variable};
    Type3OptGroupReceipt receipt; Trace t; manager.events = &t.events;
    t.throwOn = "literal:DOE_PAR    \t\t";
    bool caught = false;
    try { attemptType3SaveCallerWithTypedSections(true, &manager, p, state, t, t, receipt); }
    catch (const InsertStopped&) { caught = true; }
    CHECK(caught && manager.scans == 1 && variable.index == 0);
    CHECK(position(t.events, "manager-scan") < position(t.events, t.throwOn));
    CHECK(position(t.events, "literal:REGIONS    \t\t") == t.events.size());
    CHECK(position(t.events, "close") == t.events.size());
    CHECK(position(t.events, "common") == t.events.size());
}
TEST_CASE(type3_opt_typed_profile_emits_saved_typ_obj_after_groups)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.value30 = 30; state.value10 = 10; state.value34 = 34;
    state.numConRows = {{0, 42, 0, 0, 0.0, {}}};
    Manager manager; Trace t; manager.events = &t.events; t.mutateState = &state;
    Type3OptGroupReceipt receipt;
    CHECK(!attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt));
    const std::vector<std::string> expected = {
        "literal:TYP_OBJ       ", "int:30", "literal:        ", "int:10",
        "literal:      ", "int:341", "endl", "endl",
        "literal:NUM_CON\t\t", "int:1", "endl"
    };
    const auto start = std::find(t.events.begin(), t.events.end(), expected.front());
    REQUIRE(start != t.events.end());
    REQUIRE(static_cast<std::size_t>(t.events.end() - start) >= expected.size());
    CHECK(std::equal(expected.begin(), expected.end(), start));
    CHECK(position(t.events, "insert:suffix") == t.events.size());
    CHECK(position(t.events, "insert:suffix") == t.events.size());
    CHECK(position(t.events, expected.front()) < position(t.events, "insert:row0"));
    CHECK(position(t.events, "manager-scan") > 0);
    CHECK(t.events[position(t.events, "manager-scan") - 1] == "endl");
}
TEST_CASE(type3_opt_typed_profile_typed_literal_throw_stops_after_receipt)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.value30 = 30; state.value10 = 10; state.value34 = 34;
    state.numConRows = {{0, 42, 0, 0, 0.0, {}}};
    Manager manager; Trace t; manager.events = &t.events; t.throwOn = "literal:TYP_OBJ       ";
    Type3OptGroupReceipt receipt;
    bool caught = false;
    try { (void)attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt); }
    catch (const InsertStopped&) { caught = true; }
    CHECK(caught);
    CHECK(receipt.temporarySnapshots[0] == std::vector<std::int32_t>({42}));
    CHECK(position(t.events, t.throwOn) < t.events.size());
    CHECK(position(t.events, "int:1021") == t.events.size());
    CHECK(position(t.events, "insert:row0") == t.events.size());
    CHECK(position(t.events, "manager-scan") == t.events.size());
    CHECK(position(t.events, "close") == t.events.size());
}
TEST_CASE(type3_opt_numcon_count_is_sampled_after_typ_obj_and_before_literal)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.value30 = 30; state.value10 = 10; state.value34 = 34;
    state.numConRows = {{0, 42, 0, 0, 0.0, {}}};
    Manager manager; Trace t; manager.events = &t.events; t.mutateState = &state;
    t.changeCountOnSecondTypObjEndl = true;
    Type3OptGroupReceipt receipt;
    CHECK(!attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt));
    const std::vector<std::string> expected = {
        "literal:TYP_OBJ       ", "int:30", "literal:        ", "int:10",
        "literal:      ", "int:341", "endl", "endl", "literal:NUM_CON\t\t", "int:2", "endl"
    };
    const auto start = std::find(t.events.begin(), t.events.end(), expected.front());
    REQUIRE(start != t.events.end());
    REQUIRE(static_cast<std::size_t>(t.events.end() - start) >= expected.size());
    CHECK(std::equal(expected.begin(), expected.end(), start));

    t.events.clear(); t.typObjEndls = 0; t.inTypObj = false;
    t.changeCountOnSecondTypObjEndl = false; t.changeCountOnNumConLiteral = true;
    state.numConRows = {{0, 42, 0, 0, 0.0, {}}};
    CHECK(!attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt));
    CHECK(position(t.events, "literal:NUM_CON\t\t") + 1 == position(t.events, "int:1"));
    CHECK(state.numConRows.size() == 2);
}
TEST_CASE(type3_opt_numcon_literal_throw_preserves_group_receipt_prefix)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.numConRows = {{0, 42, 0, 0, 0.0, {}}};
    Manager manager; Trace t; manager.events = &t.events; t.throwOn = "literal:NUM_CON\t\t";
    Type3OptGroupReceipt receipt;
    bool caught = false;
    try { (void)attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt); }
    catch (const InsertStopped&) { caught = true; }
    CHECK(caught);
    CHECK(receipt.temporarySnapshots[0] == std::vector<std::int32_t>({42}));
    CHECK(position(t.events, t.throwOn) > position(t.events, "endl"));
    CHECK(!t.events.empty() && t.events.back() == t.throwOn);
    CHECK(position(t.events, "manager-scan") == t.events.size());
    CHECK(position(t.events, "insert:row0") == t.events.size());
    CHECK(position(t.events, "close") == t.events.size());
}
TEST_CASE(type3_opt_numcon_rows_emit_native_order_double_bits_and_empty_endl)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.numConRows = {{0, 11, 22, 44, 444.5, {}}, {0, 12, 23, 45, -0.0, {}}};
    Manager manager; Trace t; t.insertStatus = false; t.closeStatus = false; manager.events = &t.events; Type3OptGroupReceipt receipt;
    (void)attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt);
    const std::vector<std::string> rows = {
        "literal:  ", "int:1", "literal: ", "int:11", "literal:    ", "int:22",
        "literal:      ", "double:4646527342888353792", "literal:    ", "int:44", "endl",
        "literal:  ", "int:2", "literal: ", "int:12", "literal:    ", "int:23",
        "literal:      ", "double:9223372036854775808", "literal:    ", "int:45", "endl", "endl"
    };
    const auto start = std::find(t.events.begin(), t.events.end(), rows.front());
    REQUIRE(start != t.events.end());
    REQUIRE(static_cast<std::size_t>(t.events.end() - start) >= rows.size());
    CHECK(std::equal(rows.begin(), rows.end(), start));
    CHECK(position(t.events, "insert:row0") == t.events.size());
    CHECK(position(t.events, "close") < t.events.size());

    state.numConRows.clear(); t.events.clear(); manager.events = &t.events;
    (void)attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt);
    const std::vector<std::string> empty = {"literal:NUM_CON\t\t", "int:0", "endl", "endl", "manager-scan"};
    const auto emptyStart = std::find(t.events.begin(), t.events.end(), empty.front());
    REQUIRE(emptyStart != t.events.end());
    REQUIRE(static_cast<std::size_t>(t.events.end() - emptyStart) >= empty.size());
    CHECK(std::equal(empty.begin(), empty.end(), emptyStart));
}
TEST_CASE(type3_opt_numcon_count_and_row_operands_are_sampled_before_mutation)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.numConRows = {{0, 11, 22, 44, 444.5, {}}, {0, 12, 23, 45, -0.0, {}}};
    Manager manager; Trace t; t.mutateState = &state; t.changeCountOnNumConLiteral = true;
    t.mutateRowOnFirstInsert = true; t.mutateSecondRowAfterFirstEndl = true;
    Type3OptGroupReceipt receipt;
    (void)attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt);
    CHECK(position(t.events, "literal:NUM_CON\t\t") + 1 == position(t.events, "int:2"));
    const auto rowOneEnd = position(t.events, "double:4646527342888353792");
    REQUIRE(rowOneEnd < t.events.size());
    const auto rowTwoOrdinal = std::find(t.events.begin() + static_cast<std::ptrdiff_t>(rowOneEnd + 1), t.events.end(), "int:2");
    CHECK(rowTwoOrdinal != t.events.end());
    CHECK(static_cast<std::size_t>(std::count(t.events.begin(), t.events.end(), "literal:  ")) == 2);
    CHECK(position(t.events, "int:111") == t.events.size());
    CHECK(position(t.events, "int:211") < position(t.events, "int:222"));
    CHECK(position(t.events, "int:222") < position(t.events, "double:4642806595539959808"));
    CHECK(position(t.events, "double:4642806595539959808") < position(t.events, "int:233"));
}
TEST_CASE(type3_opt_numcon_row_throw_stops_later_rows_manager_and_close)
{
    auto p = prepared(); Type3OptimizerScalarState state;
    state.numConRows = {{0, 11, 22, 44, 444.5, {}}, {0, 12, 23, 45, 555.5, {}}};
    Manager manager; Trace t; manager.events = &t.events; Type3OptGroupReceipt receipt;
    t.throwOn = "double:4646527342888353792";
    bool caught = false;
    try { (void)attemptType3OptSaveWithTypedSections(p, state, manager, t, receipt); }
    catch (const InsertStopped&) { caught = true; }
    CHECK(caught);
    CHECK(!t.events.empty() && t.events.back() == t.throwOn);
    CHECK(position(t.events, "int:12") == t.events.size());
    CHECK(position(t.events, "manager-scan") == t.events.size());
    CHECK(position(t.events, "close") == t.events.size());
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
