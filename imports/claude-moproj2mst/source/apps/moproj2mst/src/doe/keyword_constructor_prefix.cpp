#include "keyword_constructor_prefix.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
#include "keyword_constructor_record.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void unavailable(const char* reason)
{ throw Error(M2M_E_UNSUPPORTED, std::string("DOE keyword constructor prefix: ") + reason); }
}

KeywordConstructorPrefixResult appendFreshKeywordConstructorPrefix(
    Study& study, HeaderVariableRecord& receiver, const KeywordConstructorPrefixInput& input)
{
    // Reconstruction admission before mutation. A borrowed Var alias alone is
    // never ownership, and no derived record may be deleted through Var.
    if (!study.scalarPrefixVariableOwners.empty() || !study.geometryPrefixVariableOwners.empty() ||
        study.retainedScalarPrefix() || study.vars.size() != study.headerVariableOwners.size() ||
        std::any_of(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
            [](const auto& owner) { return !owner; }))
        unavailable("coherent concrete header owners are required");
    std::vector<const Var*> aliases;
    aliases.reserve(study.vars.size());
    bool foundReceiver = false;
    for (const auto* alias : study.vars) {
        const auto owner = std::find_if(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
            [alias](const auto& candidate) { return candidate.get() == alias; });
        if (!alias || owner == study.headerVariableOwners.end() ||
            std::find(aliases.begin(), aliases.end(), alias) != aliases.end())
            unavailable("coherent distinct header aliases are required");
        aliases.push_back(alias);
        if (owner->get() == &receiver) foundReceiver = true;
    }
    if (!foundReceiver) unavailable("receiver is not an owned HeaderVariableRecord of this Study");
    if (!receiver.items.empty()) unavailable("an empty canonical keyword list is required");

    // 816b8f precedes search/allocation. Empty-list admission skips the search,
    // including its special ID0x131 case, without inventing duplicate behavior.
    receiver.dimension = input.dimension;
    auto record = std::make_unique<KeywordConstructorRecord>(input.keywordId, input.parameterName,
        input.unitType, input.nominalValue, input.operations, input.entities); // 9206e0 values
    const auto* borrowed = record.get();
    receiver.items.push_back(Item::ownKeyword(std::move(record))); // 816d24
    // This happens after ownership commits. Do not sort/deduplicate the record's
    // own list or reserve/rollback the receiver's union before native encounter.
    for (const int operation : input.operations)
        if (std::find(receiver.ops.begin(), receiver.ops.end(), operation) == receiver.ops.end())
            receiver.ops.push_back(operation);
    return {borrowed}; // Prefix only; reader notification/text work remains guarded.
}
} // namespace m2m::doe
