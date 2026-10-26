#include "variable_dimension_prefix.hpp"

#include <algorithm>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/model.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void unavailable(const char* reason)
{ throw Error(M2M_E_UNSUPPORTED, std::string("DOE variable dimension prefix: ") + reason); }
}

VariableDimensionPrefixResult reconcileHeaderDimensions(Study& study, const Project& operationProject)
{
    // Admission before mutation. Borrowed aliases never establish ownership or
    // authorize deletion through Var, which has no virtual destructor.
    if (study.vars.size() != study.headerVariableOwners.size() ||
        std::any_of(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
            [](const auto& owner) { return !owner; }))
        unavailable("coherent concrete header owners are required");
    std::vector<HeaderVariableRecord*> variables;
    variables.reserve(study.vars.size());
    for (const auto* variable : study.vars) {
        const auto owner = std::find_if(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
            [variable](const auto& candidate) { return candidate.get() == variable; });
        if (!variable || owner == study.headerVariableOwners.end() ||
            std::find(variables.begin(), variables.end(), variable) != variables.end())
            unavailable("coherent distinct header aliases are required");
        variables.push_back(owner->get());
    }

    VariableDimensionPrefixResult result;
    for (std::size_t index = 0; index < variables.size(); ++index) {
        auto& variable = *variables[index];
        result.visitedRows = index + 1;
        // 816ae0(var,0) returns -1 on empty. Do not try a later operation ID.
        const int operationId = variable.ops.empty() ? -1 : variable.ops.front();
        const auto* operation = operationById(operationProject, operationId);
        if (!operation) continue;
        if (operation->project != &operationProject)
            unavailable("matched operation does not belong to the supplied Project");
        const int dimension = opDim(*operation); // 05f0f0: absent class defaults to3.
        if (dimension == variable.dimension) continue; // Before any item inspection.
        variable.dimension = dimension; // 0d8040: plain +24 store at d2d25.
        ++result.dimensionsStored;
        if (!variable.items.empty()) {
            // Keep the store; stop before the native item snapshot/conversion.
            result.stop = VariableDimensionPrefixStop::KeywordItemSnapshotBoundary;
            result.stoppedAt = index;
            return result;
        }
    }
    return result; // Supplied rows only, never native/global reconciliation completion.
}
} // namespace m2m::doe
