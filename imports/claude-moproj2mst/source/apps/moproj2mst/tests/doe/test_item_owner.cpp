// ctest-labels: m2m-doe
// Canonical Item ownership only; no parser, conversion or delivery admission.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
#include "m2m/mst/doe_model.hpp"
#include "../../src/doe/empty_variable_prune.hpp"
#include "../../src/doe/keyword_constructor_record.hpp"

using namespace m2m;
namespace {
std::unique_ptr<doe::KeywordConstructorRecord> record()
{
    return std::make_unique<doe::KeywordConstructorRecord>(276, qt::L("parameter"), 7, 1.25,
        std::vector<int>{9, 2, 9}, std::vector<int>{3, 1, 3});
}
}

static_assert(!std::is_copy_constructible_v<doe::Item>);
static_assert(!std::is_copy_assignable_v<doe::Item>);
static_assert(std::is_nothrow_move_constructible_v<doe::Item>);
static_assert(std::is_nothrow_move_assignable_v<doe::Item>);
static_assert(!std::is_copy_constructible_v<doe::KeywordConstructorRecord>);

TEST_CASE(item_legacy_snapshot_copies_text_and_preserves_null_empty_distinction)
{
    doe::Var variable;
    variable.items.emplace_back();
    variable.items.emplace_back(qt::String(u""), qt::L("SPEED <x>\n"));
    CHECK(variable.items[0].kind() == doe::ItemKind::LegacyValue);
    REQUIRE(variable.items[0].legacyText());
    CHECK(variable.items[0].legacyText()->before.isNull());
    CHECK(variable.items[0].keyword() == nullptr);
    const auto snapshot = doe::snapshotItems(variable);
    REQUIRE(snapshot.size() == 2);
    CHECK(std::holds_alternative<doe::LegacyItemText>(snapshot[0]));
    CHECK(doe::itemSnapshotText(snapshot[0]).before.isNull());
    CHECK(!doe::itemSnapshotText(snapshot[1]).before.isNull());
    CHECK(doe::itemSnapshotText(snapshot[1]).before.isEmpty());
    variable.items[1].legacyText()->before = qt::L("changed");
    variable.items[1].legacyText()->after = qt::L("changed too");
    CHECK(doe::itemSnapshotText(snapshot[1]).before.isEmpty());
    CHECK(doe::itemSnapshotText(snapshot[1]).after == qt::L("SPEED <x>\n"));
}

TEST_CASE(item_keyword_constructor_prefix_has_distinct_metadata_and_null_data)
{
    auto owned = record();
    const auto* identity = owned.get();
    auto item = doe::Item::ownKeyword(std::move(owned));
    CHECK(!owned);
    CHECK(item.kind() == doe::ItemKind::NativeKeyword);
    CHECK(item.legacyText() == nullptr);
    REQUIRE(item.keyword() == identity);
    const auto& value = *item.keyword();
    CHECK_EQ(value.keywordId, 276);
    CHECK(value.parameterName == qt::L("parameter"));
    CHECK_EQ(value.unitType, 7);
    CHECK(value.nominalValue == 1.25 && value.lowerValue == 0.0 && value.upperValue == 0.0);
    CHECK(value.parameter == -1 && value.saveSelector == -1);
    CHECK(value.text08.isNull() && value.nominalText.isNull());
    CHECK(value.upperText.isNull() && value.lowerText.isNull());
    CHECK(!value.function && !value.enumeration && !value.lowerDefined && !value.upperDefined);
    CHECK(value.nominalData == nullptr && value.upperData == nullptr && value.lowerData == nullptr);
    CHECK(!value.lowerDataDefined && !value.upperDataDefined);
    CHECK(value.entities == std::vector<int>({3, 1, 3}));
    CHECK(value.operations == std::vector<int>({9, 2, 9}));
}

TEST_CASE(item_keyword_snapshot_keeps_record_identity_through_envelope_moves)
{
    doe::Var variable;
    variable.items.reserve(1);
    auto first = doe::Item::ownKeyword(record());
    const auto* identity = first.keyword();
    variable.items.push_back(std::move(first));
    const auto snapshot = doe::snapshotItems(variable);
    REQUIRE(snapshot.size() == 1);
    CHECK(std::get<const doe::KeywordConstructorRecord*>(snapshot[0]) == identity);
    // Force envelope relocation without moving or copying the owned record.
    const auto originalCapacity = variable.items.capacity();
    for (std::size_t i = 0; i <= originalCapacity; ++i) variable.items.emplace_back();
    CHECK(variable.items.capacity() > originalCapacity);
    CHECK(variable.items.front().keyword() == identity);
    doe::Item moved;
    moved = std::move(variable.items.front());
    CHECK(moved.keyword() == identity);
    CHECK(std::get<const doe::KeywordConstructorRecord*>(snapshot[0]) == identity);
    CHECK(variable.items.front().kind() == doe::ItemKind::NativeKeyword);
    CHECK(variable.items.front().keyword() == nullptr);
    // The snapshot never dereferences its pointer after the moved owner dies.
}

TEST_CASE(item_keyword_snapshot_reads_text_at_consumption_without_a_mirrored_cache)
{
    doe::Var variable;
    variable.items.emplace_back(qt::L("legacy upper"), qt::L("legacy lower"));
    variable.items.push_back(doe::Item::ownKeyword(record()));
    variable.items[1].keyword()->upperText = qt::L("upper at snapshot");
    variable.items[1].keyword()->lowerText = qt::L("lower at snapshot");
    const auto snapshot = doe::snapshotItems(variable);
    variable.items[0].legacyText()->before = qt::L("new legacy");
    variable.items[1].keyword()->upperText = qt::L("upper at consumption\n");
    variable.items[1].keyword()->lowerText = qt::L("lower at consumption\n");
    CHECK(doe::itemSnapshotText(snapshot[0]).before == qt::L("legacy upper"));
    const auto text = doe::itemSnapshotText(snapshot[1]);
    CHECK(text.before == qt::L("upper at consumption\n"));
    CHECK(text.after == qt::L("lower at consumption\n"));
}

TEST_CASE(item_native_null_refuses_without_becoming_an_empty_legacy_value)
{
    doe::Var variable;
    variable.items.emplace_back();
    variable.items.push_back(doe::Item::ownKeyword(nullptr));
    const auto snapshot = doe::snapshotItems(variable);
    REQUIRE(snapshot.size() == 2);
    CHECK(doe::itemSnapshotText(snapshot[0]).before.isNull());
    CHECK(variable.items[1].kind() == doe::ItemKind::NativeKeyword);
    CHECK(variable.items[1].legacyText() == nullptr);
    CHECK(std::holds_alternative<const doe::KeywordConstructorRecord*>(snapshot[1]));
    bool refused = false;
    try { (void)doe::itemSnapshotText(snapshot[1]); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        CHECK(std::string(error.what()).find("native-null") != std::string::npos);
        refused = true;
    }
    CHECK(refused);
}

TEST_CASE(item_owner_stays_on_concrete_study_header_and_preserves_prune_count)
{
    doe::Study study;
    auto header = std::make_unique<doe::HeaderVariableRecord>(3);
    header->name = qt::L("Owned");
    auto* variable = header.get();
    variable->items.push_back(doe::Item::ownKeyword(record()));
    const auto snapshot = doe::snapshotItems(*variable);
    study.vars.push_back(variable);
    study.headerVariableOwners.push_back(std::move(header));
    study.samples.invalid.fill(false);
    doe::pruneEmptyHeaderVariables(study);
    REQUIRE(study.headerVariableOwners.size() == 1 && study.vars.size() == 1);
    CHECK(study.vars[0] == variable);
    CHECK(std::get<const doe::KeywordConstructorRecord*>(snapshot[0]) == variable->items[0].keyword());
    CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    variable->items.clear(); // Concrete record dies; the borrowed snapshot does not keep it alive.
    doe::pruneEmptyHeaderVariables(study);
    CHECK(study.vars.empty() && study.headerVariableOwners.empty());
    // No dereference of snapshot after record or Header destruction.
}

int main() { return testkit::run_all(); }
