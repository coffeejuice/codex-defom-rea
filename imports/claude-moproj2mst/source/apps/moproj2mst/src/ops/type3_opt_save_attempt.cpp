#include "type3_opt_save_attempt.hpp"
#include <utility>

namespace m2m::mst::detail {
namespace {
void insertAll(Type3OptAttemptSink& sink, const Type3OptFragments& fragments)
{
    for (const auto& fragment : fragments) (void)sink.insert(fragment);
}
}
namespace {
bool attempt(const Type3OptSavePrepared& p, Type3OptAttemptSink& sink,
             const Type3OptimizerScalarState* state, Type3OptGroupReceipt* receipt)
{
    if (!sink.open(p.path)) {
        sink.assignOpenFailureDiagnostic(p.path);
        return false;
    }
    insertAll(sink, p.header);
    insertAll(sink, p.scalarPrefix);
    if (state && receipt) {
        Type3OptGroupReceipt completed;
        for (std::size_t group = 0; group < completed.temporarySnapshots.size(); ++group) {
            auto& values = completed.temporarySnapshots[group];
            for (const auto& row : state->numConRows)
                if (row.value00 == static_cast<std::int32_t>(group)) values.push_back(row.value04);
            completed.ignoredCounts[group] = values.size();
        }
        *receipt = std::move(completed);
    }
    insertAll(sink, p.scalarSuffix);
    for (const auto& row : p.rows) insertAll(sink, row);
    insertAll(sink, p.managerCountAndNumbers);
    if (p.sectionRows.empty()) insertAll(sink, p.emptySection);
    else for (const auto& row : p.sectionRows) insertAll(sink, row);
    insertAll(sink, p.finalLines);
    (void)sink.close();
    return false;
}
} // namespace
bool attemptType3OptSave(const Type3OptSavePrepared& p, Type3OptAttemptSink& sink)
{
    return attempt(p, sink, nullptr, nullptr);
}
bool attemptType3OptSaveWithGroups(const Type3OptSavePrepared& p,
                                   const Type3OptimizerScalarState& state,
                                   Type3OptAttemptSink& sink, Type3OptGroupReceipt& receipt)
{
    return attempt(p, sink, &state, &receipt);
}
void attemptType3SaveCaller(bool optimizerPresent, bool managerPresent,
                            const Type3OptSavePrepared& p, Type3OptAttemptSink& sink,
                            Type3OptCallerEffects& effects)
{
    if (optimizerPresent && managerPresent) {
        effects.appendStudy();
        effects.saveInputTable();
        (void)attemptType3OptSave(p, sink);
    }
    effects.commonContinuation();
}
} // namespace m2m::mst::detail
