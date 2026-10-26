// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
namespace {
std::string table(const std::string& children)
{ return "<sfDoeInputTable>" + children + "</sfDoeInputTable>"; }
std::string linked(const std::string& name, const std::string& target, const std::string& id)
{
    return "<DoeVariable name=\"" + name + "\" varId=\"" + id +
        "\" dimension=\"3\" numSamples=\"3\" LinkTo=\"" + target + "\"/>";
}
bool readRows(doe::Study& study, const std::string& rows)
{ return doe::readInputTable(table("<DoeVariables>" + rows + "</DoeVariables>"), study); }
}

TEST_CASE(doe_remove_variable_clears_inbound_cycle_links_and_preserves_owner_alignment)
{
    doe::Study study;
    REQUIRE(readRows(study, linked("A", "B", "7") + linked("B", "A", "8") +
        linked("C", "B", "9")));
    REQUIRE(study.vars.size() == 3 && study.headerVariableOwners.size() == 3);
    auto* const a = study.vars[0];
    auto* const b = study.vars[1];
    auto* const c = study.vars[2];
    CHECK(a->link == b && b->link == a && c->link == b);

    study.samples.invalid.fill(false);
    engine::FileEffects effects;
    doe::removeVarFiles(study, 1, String{}, qt::L("B"), effects);

    REQUIRE(study.vars.size() == 2 && study.headerVariableOwners.size() == 2);
    CHECK(study.vars[0] == a && study.vars[1] == c);
    CHECK(study.headerVariableOwners[0].get() == a);
    CHECK(study.headerVariableOwners[1].get() == c);
    CHECK(a->link == nullptr && c->link == nullptr);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);

    // Removal keeps the concrete owner list aligned for another reader call.
    REQUIRE(readRows(study, linked("D", "C", "10")));
    REQUIRE(study.vars.size() == 3 && study.headerVariableOwners.size() == 3);
    CHECK(study.vars[1] == c && study.vars[2]->link == c);
    REQUIRE(readRows(study, linked("E", "D", "11")));
    REQUIRE(study.vars.size() == 4 && study.headerVariableOwners.size() == 4);
    CHECK(study.vars[2]->link == study.vars[1]);
    CHECK(study.vars[3]->link == study.vars[2]);
}

TEST_CASE(doe_remove_self_linked_variable_clears_edge_before_concrete_owner_release)
{
    doe::Study study;
    REQUIRE(readRows(study, linked("Self", "Self", "7")));
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    CHECK(study.vars[0]->link == study.vars[0]);
    study.samples.invalid.fill(false);

    engine::FileEffects effects;
    doe::removeVarFiles(study, 1, String{}, qt::L("Self"), effects);
    CHECK(study.vars.empty() && study.headerVariableOwners.empty());
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
}

TEST_CASE(doe_remove_unsupported_scalar_or_foreign_owner_refuses_before_graph_mutation)
{
    doe::Study scalar;
    REQUIRE(readRows(scalar, linked("A", "Unused", "7")));
    auto* const header = scalar.vars[0];
    auto scalarRecord = std::make_unique<doe::Var>();
    scalarRecord->name = qt::L("Scalar");
    auto* const scalarTarget = scalarRecord.get();
    scalar.scalarPrefixVariableOwners.push_back(std::move(scalarRecord));
    scalar.vars.push_back(scalarTarget);
    header->link = scalarTarget;
    scalar.samples.invalid.fill(false);
    engine::FileEffects scalarEffects;
    bool scalarRefused = false;
    try { doe::removeVarFiles(scalar, 1, String{}, qt::L("Scalar"), scalarEffects); }
    catch (const Error& error) { scalarRefused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(scalarRefused);
    CHECK(scalar.vars.size() == 2 && scalar.vars[0] == header && scalar.vars[1] == scalarTarget);
    CHECK(scalar.headerVariableOwners.size() == 1 && scalar.scalarPrefixVariableOwners.size() == 1);
    CHECK(header->link == scalarTarget);
    CHECK(!scalar.samples.invalid[0] && !scalar.samples.invalid[1] && !scalar.samples.invalid[2]);
    CHECK(scalarEffects.deletes.empty());

    doe::Study foreign;
    REQUIRE(readRows(foreign, linked("A", "Unused", "7")));
    auto* const foreignHeader = foreign.vars[0];
    doe::Var foreignRecord;
    foreignRecord.name = qt::L("Foreign");
    foreign.vars.push_back(&foreignRecord);
    foreignHeader->link = &foreignRecord;
    foreign.samples.invalid.fill(false);
    engine::FileEffects foreignEffects;
    bool foreignRefused = false;
    try { doe::removeVarFiles(foreign, 1, String{}, qt::L("Foreign"), foreignEffects); }
    catch (const Error& error) { foreignRefused = error.code == M2M_E_UNSUPPORTED; }
    CHECK(foreignRefused);
    CHECK(foreign.vars.size() == 2 && foreign.vars[0] == foreignHeader && foreign.vars[1] == &foreignRecord);
    CHECK(foreign.headerVariableOwners.size() == 1);
    CHECK(foreignHeader->link == &foreignRecord);
    CHECK(!foreign.samples.invalid[0] && !foreign.samples.invalid[1] && !foreign.samples.invalid[2]);
    CHECK(foreignEffects.deletes.empty());
}

int main() { return testkit::run_all(); }
