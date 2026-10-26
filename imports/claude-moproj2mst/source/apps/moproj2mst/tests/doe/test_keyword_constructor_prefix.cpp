// ctest-labels: m2m-doe
// Explicit decoded constructor prefix only; no public Keyword/Save admission.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
#include "m2m/engine.hpp"
#include "../../src/doe/keyword_constructor_prefix.hpp"
#include "../../src/doe/keyword_constructor_record.hpp"

using namespace m2m;
namespace {
struct Rig {
    doe::Study study;
    doe::HeaderVariableRecord& add(const char* name)
    {
        auto owner = std::make_unique<doe::HeaderVariableRecord>(3);
        owner->name = qt::L(name);
        auto* alias = owner.get();
        study.headerVariableOwners.push_back(std::move(owner));
        study.vars.push_back(alias);
        return *alias;
    }
};
doe::KeywordConstructorPrefixInput input()
{
    return {276, qt::L("parameter"), 7, -7, 1.25, {5, 8, 8, -2, 9}, {3, 1, 3}};
}
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
}

static_assert(!doe::KeywordConstructorPrefixResult::completeForReader);
static_assert(!doe::KeywordConstructorPrefixResult::completeForPublicSave);

TEST_CASE(keyword_constructor_prefix_owns_record_and_unions_operations_in_encounter_order)
{
    Rig rig;
    auto& variable = rig.add("Actual header");
    variable.ops = {5, 5, -1};
    const auto decoded = input();
    const auto result = doe::appendFreshKeywordConstructorPrefix(rig.study, variable, decoded);
    REQUIRE(variable.items.size() == 1);
    REQUIRE(result.record);
    CHECK(result.record == variable.items[0].keyword());
    CHECK(variable.items[0].kind() == doe::ItemKind::NativeKeyword);
    CHECK_EQ(variable.dimension, -7); // Explicit decoded store, not parser admission.
    CHECK(variable.ops == std::vector<int>({5, 5, -1, 8, -2, 9}));
    CHECK(result.record->operations == decoded.operations); // Do not dedupe this record's list.
    CHECK(result.record->entities == std::vector<int>({3, 1, 3}));
    CHECK_EQ(result.record->keywordId, 276);
    CHECK(result.record->parameterName == qt::L("parameter"));
    CHECK_EQ(result.record->unitType, 7);
    CHECK(result.record->nominalValue == 1.25);
    CHECK(result.record->parameter == -1 && result.record->saveSelector == -1);
    CHECK(result.record->nominalText.isNull() && result.record->upperText.isNull() &&
          result.record->lowerText.isNull());
    CHECK(result.record->nominalData == nullptr && result.record->upperData == nullptr &&
          result.record->lowerData == nullptr);
    CHECK(!result.record->function && !result.record->enumeration &&
          !result.record->lowerDefined && !result.record->upperDefined &&
          !result.record->lowerDataDefined && !result.record->upperDataDefined);
    CHECK(decoded.operations == std::vector<int>({5, 8, 8, -2, 9}));
}

TEST_CASE(keyword_constructor_prefix_targets_explicit_owned_receiver_not_last_or_alias_position)
{
    Rig rig;
    auto& first = rig.add("First");
    auto& second = rig.add("Second");
    first.dimension = 12;
    second.dimension = 13;
    second.ops = {81};
    std::swap(rig.study.vars[0], rig.study.vars[1]); // Still coherent owned aliases.
    const auto result = doe::appendFreshKeywordConstructorPrefix(rig.study, first, input());
    CHECK_EQ(first.dimension, -7);
    CHECK(first.items[0].keyword() == result.record);
    CHECK_EQ(second.dimension, 13);
    CHECK(second.items.empty() && second.ops == std::vector<int>({81}));
    CHECK(rig.study.vars[0] == &second && rig.study.vars[1] == &first);
}

TEST_CASE(keyword_constructor_prefix_empty_lists_and_special_id_do_not_invent_numeric_validation)
{
    Rig rig;
    auto& variable = rig.add("Explicit values");
    variable.ops = {17, 17};
    const doe::KeywordConstructorPrefixInput decoded{0x131, qt::String(), -1, 0, -0.5, {}, {}};
    const auto result = doe::appendFreshKeywordConstructorPrefix(rig.study, variable, decoded);
    CHECK_EQ(variable.dimension, 0);
    CHECK(variable.ops == std::vector<int>({17, 17}));
    CHECK(result.record->keywordId == 0x131 && result.record->parameterName.isNull());
    CHECK(result.record->operations.empty() && result.record->entities.empty());
    CHECK(result.record->unitType == -1 && result.record->nominalValue == -0.5);
}

TEST_CASE(keyword_constructor_prefix_refuses_foreign_alias_legacy_and_existing_records_before_mutation)
{
    for (int profile = 0; profile < 11; ++profile) {
        Rig rig;
        auto& first = rig.add("First");
        rig.add("Second");
        first.dimension = 44;
        first.ops = {71, 71};
        rig.study.samples.invalid.fill(false);
        doe::HeaderVariableRecord foreignHeader(3);
        foreignHeader.dimension = 44;
        foreignHeader.ops = {71, 71};
        doe::Var foreignVar;
        auto* receiver = &first;
        std::unique_ptr<doe::HeaderVariableRecord> displaced;
        switch (profile) {
        case 0: receiver = &foreignHeader; break;
        case 1: rig.study.vars[1] = nullptr; break;
        case 2: rig.study.vars[1] = &first; break;
        case 3: rig.study.vars[1] = &foreignVar; break;
        case 4: displaced = std::move(rig.study.headerVariableOwners[1]); break;
        case 5: rig.study.vars.pop_back(); break;
        case 6: first.items.emplace_back(qt::L("legacy"), qt::L("value")); break;
        case 7:
            first.items.push_back(doe::Item::ownKeyword(
                std::make_unique<doe::KeywordConstructorRecord>(8, qt::String(), 0, 0.1,
                    std::vector<int>{1}, std::vector<int>{})));
            break;
        case 8: first.items.push_back(doe::Item::ownKeyword(nullptr)); break;
        case 9: rig.study.scalarPrefixVariableOwners.push_back(std::make_unique<doe::Var>()); break;
        case 10: rig.study.geometryPrefixVariableOwners.push_back(std::make_unique<doe::Var>()); break;
        }
        const auto aliases = rig.study.vars;
        const auto itemCount = first.items.size();
        const auto* priorRecord = itemCount ? first.items[0].keyword() : nullptr;
        const auto message = refused([&] {
            (void)doe::appendFreshKeywordConstructorPrefix(rig.study, *receiver, input());
        });
        CHECK(message.find("keyword constructor prefix") != std::string::npos);
        CHECK(first.dimension == 44 && receiver->dimension == 44);
        CHECK(first.ops == std::vector<int>({71, 71}));
        CHECK(receiver->ops == std::vector<int>({71, 71}));
        CHECK(first.items.size() == itemCount && rig.study.vars == aliases);
        if (itemCount) CHECK(first.items[0].keyword() == priorRecord);
        if (profile == 6) CHECK(first.items[0].legacyText()->before == qt::L("legacy"));
        CHECK(!rig.study.samples.invalid[0] && !rig.study.samples.invalid[1] &&
              !rig.study.samples.invalid[2]);
    }
}

TEST_CASE(keyword_constructor_prefix_retry_retains_first_owner_dimension_and_operation_union)
{
    Rig rig;
    auto& variable = rig.add("Once");
    const auto first = doe::appendFreshKeywordConstructorPrefix(rig.study, variable, input());
    auto again = input();
    again.dimension = 99;
    again.operations = {123};
    CHECK(refused([&] {
        (void)doe::appendFreshKeywordConstructorPrefix(rig.study, variable, again);
    }).find("empty canonical") != std::string::npos);
    CHECK_EQ(variable.dimension, -7);
    CHECK(variable.items.size() == 1 && variable.items[0].keyword() == first.record);
    CHECK(variable.ops == std::vector<int>({5, 8, -2, 9}));
}

TEST_CASE(keyword_constructor_prefix_preserves_other_study_state_and_public_save_guard)
{
    Rig rig;
    auto& variable = rig.add("Kept");
    auto& linked = rig.add("Linked");
    variable.description = qt::L("unchanged");
    variable.index = 12;
    variable.number = 42;
    variable.geoType = 100;
    variable.link = &linked;
    variable.files.emplace_back();
    variable.files[0].path = qt::L("C:/PROBLEM/reference.GEO");
    rig.study.selectedVariableIndex = 8;
    rig.study.regions = {qt::L("region")};
    rig.study.samples.mode = 2;
    rig.study.samples.tables[2] = {{4.0, 5.0}};
    rig.study.samples.counts[2] = 6;
    rig.study.samples.invalid.fill(false);
    const auto samples = rig.study.samples;
    const auto aliases = rig.study.vars;
    const auto prefix = doe::appendFreshKeywordConstructorPrefix(rig.study, variable, input());
    CHECK(variable.items[0].keyword() == prefix.record);
    CHECK(variable.description == qt::L("unchanged"));
    CHECK(variable.index == 12 && variable.number == 42 && variable.geoType == 100);
    CHECK(variable.link == &linked && variable.files.size() == 1);
    CHECK(variable.files[0].path == qt::L("C:/PROBLEM/reference.GEO"));
    CHECK(rig.study.selectedVariableIndex == 8 && rig.study.vars == aliases);
    CHECK(rig.study.regions == std::vector<qt::String>({qt::L("region")}));
    CHECK(rig.study.samples.tables == samples.tables && rig.study.samples.counts == samples.counts);
    CHECK(rig.study.samples.mode == samples.mode && rig.study.samples.invalid == samples.invalid);
    engine::FileEffects effects;
    CHECK(refused([&] {
        (void)doe::saveInputTable(rig.study, qt::L("C:/PROBLEM/guarded"), effects);
    }).find("0x1400d2b60") != std::string::npos);
    CHECK(effects.writes.empty() && effects.deletes.empty() && effects.renames.empty() &&
          effects.mkdirs.empty());
    CHECK(variable.items[0].keyword() == prefix.record && variable.dimension == -7);
}

int main() { return testkit::run_all(); }
