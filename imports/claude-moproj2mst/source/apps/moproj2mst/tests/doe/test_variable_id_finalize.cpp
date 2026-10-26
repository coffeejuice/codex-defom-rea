// ctest-labels: m2m-doe
// Source-order variable-number finalization; no variable parsing/notification.
#include <testkit/check.hpp>

#include <cstdint>
#include <initializer_list>
#include <limits>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"

using namespace m2m;
namespace {
std::string table(const std::string& children)
{ return "<sfDoeInputTable>" + children + "</sfDoeInputTable>"; }
const std::string operations = table("<Operations><Operation id=\"8\"/></Operations>");

template<class Work> std::string refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    CHECK(false);
    return {};
}

struct Variables {
    doe::Study study;
    std::vector<std::unique_ptr<doe::Var>> owners;
    explicit Variables(std::initializer_list<int> numbers)
    {
        for (int number : numbers) {
            auto variable = std::make_unique<doe::Var>();
            variable->number = number;
            variable->index = 37;
            variable->ops = {3, 6};
            study.vars.push_back(variable.get());
            owners.push_back(std::move(variable));
        }
    }
    std::vector<int> numbers() const
    {
        std::vector<int> out;
        for (const auto& variable : owners) out.push_back(variable->number);
        return out;
    }
};
}

TEST_CASE(doe_variable_id_normalization_recomputes_after_each_assignment)
{
    Variables vars{0, 0, 5};
    const auto aliases = vars.study.vars;
    CHECK(doe::readInputTable(operations, vars.study));
    CHECK(vars.numbers() == std::vector<int>({6, 7, 5}));
    CHECK(vars.study.vars == aliases);
    for (const auto& variable : vars.owners) {
        CHECK_EQ(variable->index, 37); // No reindexing by Operations/outer pass.
        CHECK(variable->ops == std::vector<int>({3, 6}));
    }
    // Re-reading preserves already-positive numbers; duplicate IDs are not fixed.
    vars.owners[1]->number = 6;
    CHECK(doe::readInputTable(operations, vars.study));
    CHECK(vars.numbers() == std::vector<int>({6, 6, 5}));
}

TEST_CASE(doe_variable_id_negative_only_scan_can_assign_zero_once)
{
    Variables vars{-9, -2};
    CHECK(doe::readInputTable(operations, vars.study));
    CHECK(vars.numbers() == std::vector<int>({0, 1}));
    // Each source pass visits each index once; a later read can change that0.
    CHECK(doe::readInputTable(operations, vars.study));
    CHECK(vars.numbers() == std::vector<int>({2, 1}));
}

TEST_CASE(doe_variable_id_wrap_retains_signed_comparison_and_list_order)
{
    static_assert(sizeof(int) == sizeof(std::int32_t));
    const auto maximum = std::numeric_limits<std::int32_t>::max();
    const auto minimum = std::numeric_limits<std::int32_t>::min();
    Variables first{maximum, 0};
    CHECK(doe::readInputTable(operations, first.study));
    CHECK(first.numbers() == std::vector<int>({maximum, 1}));
    Variables reversed{0, maximum};
    CHECK(doe::readInputTable(operations, reversed.study));
    CHECK(reversed.numbers() == std::vector<int>({minimum, maximum}));
    Variables later{maximum, -2, 0};
    CHECK(doe::readInputTable(operations, later.study));
    CHECK(later.numbers() == std::vector<int>({maximum, 1, 2}));
}

TEST_CASE(doe_variable_id_alias_identity_and_outer_null_skip)
{
    Variables aliased{0};
    aliased.study.vars.push_back(aliased.owners[0].get());
    CHECK(doe::readInputTable(operations, aliased.study));
    CHECK_EQ(aliased.owners[0]->number, 1);
    CHECK(aliased.study.vars[0] == aliased.study.vars[1]);
    Variables positive{4};
    positive.study.vars.insert(positive.study.vars.begin(), nullptr);
    positive.study.vars.push_back(nullptr);
    CHECK(doe::readInputTable(operations, positive.study));
    CHECK_EQ(positive.owners[0]->number, 4);
    CHECK(positive.study.vars.front() == nullptr && positive.study.vars.back() == nullptr);
}

TEST_CASE(doe_variable_id_inner_null_refuses_after_child_effects_before_current_store)
{
    Variables vars{0};
    vars.study.vars.push_back(nullptr);
    vars.study.roiType = 2;
    CHECK(refused([&] { (void)doe::readInputTable(table(
        "<Operations/><DoeRoiType type=\"9\"/>"), vars.study); }).find("null variable dereference") != std::string::npos);
    CHECK_EQ(vars.study.roiType, 9); // Child loop completed before inner scan.
    CHECK_EQ(vars.owners[0]->number, 0); // Source assignment was never reached.
}

TEST_CASE(doe_variable_id_header_only_false_status_still_finalizes_after_dispatch)
{
    Variables vars{0, 3};
    CHECK(!doe::readInputTable(table("<DoeRoiType type=\"4\"/>"), vars.study));
    CHECK_EQ(vars.study.roiType, 4);
    CHECK(vars.numbers() == std::vector<int>({4, 3}));
    Variables emptyRoot{0};
    CHECK(!doe::readInputTable(table(""), emptyRoot.study));
    CHECK_EQ(emptyRoot.owners[0]->number, 1);
    Variables malformed{0};
    CHECK(!doe::readInputTable("<sfDoeInputTable><Operations></sfDoeInputTable>", malformed.study));
    CHECK_EQ(malformed.owners[0]->number, 0); // Parse failure precedes the pass.
}

TEST_CASE(doe_variable_id_existing_payload_guards_do_not_reach_finalization)
{
    Variables variable{0};
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        "<DoeRoiType type=\"7\"/><DoeVariables><DoeVariable/></DoeVariables>"), variable.study); }).empty());
    CHECK_EQ(variable.study.roiType, 7);
    CHECK_EQ(variable.owners[0]->number, 0);
    Variables region{0};
    region.study.regions = {qt::L("prior")};
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        "<Operations/><RegionsOfInterest><Region/></RegionsOfInterest>"
        "<Constraints><Constraint/></Constraints>"), region.study); }).empty());
    REQUIRE(region.study.regions.size() == 1);
    CHECK(region.study.regions.front().isNull());
    CHECK_EQ(region.owners[0]->number, 0);
    Variables constraint{0};
    constraint.study.constraints = 3;
    CHECK(!refused([&] { (void)doe::readInputTable(table(
        "<Operations/><Constraints><Constraint/></Constraints>"), constraint.study); }).empty());
    CHECK_EQ(constraint.study.constraints, 0);
    CHECK_EQ(constraint.owners[0]->number, 0);
}

TEST_CASE(doe_variable_id_actual_active_study_owner_finalizes_before_sample_continuation)
{
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session(driver);
    auto& project = session.project();
    const String folder((m2m_test::scratchRoot() / "doe-variable-ids").u16string());
    initProject(project, folder, qt::L("ids"));
    project.path = folder + qt::kSep + qt::L("ids.moproj");
    project.doeIo = doe::makeRecordingStudyIo();
    project.doeIo->bindModelManager(session.modelManagerOwner());
    project.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
    project.subs.push_back(makeSubProject(project, 2, 7));
    project.activeSub = 1;
    auto* sub = activeSubProject(project);
    REQUIRE(sub != nullptr && sub->ownedDoeStudy != nullptr);
    const auto owner = sub->ownedDoeStudy;
    // Concrete plain Var owners, not derived deletion or proof variable parsing ran.
    for (int number : {-4, 0, 7}) {
        auto variable = std::make_unique<doe::Var>();
        variable->number = number;
        variable->index = 29;
        owner->vars.push_back(variable.get());
        owner->scalarPrefixVariableOwners.push_back(std::move(variable));
    }
    const auto aliases = owner->vars;
    const String control = projectDir(project) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
        qt::L("IdStudy") + qt::kSep + qt::L("Control") + qt::kSep;
    CHECK(project.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), operations));
    CHECK(project.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
    const auto seedWrites = project.doeIo->textWrites().size();
    auto input = xml::createElement(qt::L("Sub_Project"));
    auto study = xml::createElement(qt::L("SfDoeStudy"));
    xml::setAttribute(*study, qt::L("name"), qt::L("IdStudy"));
    input->children.push_back(study);
    Ctx ctx;
    ctx.session = &session;
    parseSubProject(project, *input, ctx);
    CHECK(sub->ownedDoeStudy == owner && sub->doe == owner.get());
    CHECK(owner->vars == aliases);
    CHECK_EQ(owner->vars[0]->number, 8);
    CHECK_EQ(owner->vars[1]->number, 9);
    CHECK_EQ(owner->vars[2]->number, 7);
    for (const auto* variable : owner->vars) CHECK_EQ(variable->index, 29);
    REQUIRE(project.doeIo->textWrites().size() == seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t mode = 0; mode < 3; ++mode)
        CHECK(project.doeIo->textWrites()[seedWrites + mode].path == control + qt::L(names[mode]));
    REQUIRE(project.doeIo->reads().size() == 2);
    CHECK(project.doeIo->reads().back().path == control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    CHECK(project.doeIo->xmlWrites().empty());
}

int main() { return testkit::run_all(); }
