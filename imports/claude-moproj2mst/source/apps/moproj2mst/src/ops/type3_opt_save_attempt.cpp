#include "type3_opt_save_attempt.hpp"

namespace m2m::mst::detail {
namespace {
void insertAll(Type3OptAttemptSink& sink, const Type3OptFragments& fragments)
{
    for (const auto& fragment : fragments) (void)sink.insert(fragment);
}
}
bool attemptType3OptSave(const Type3OptSavePrepared& p, Type3OptAttemptSink& sink)
{
    if (!sink.open(p.path)) {
        sink.assignOpenFailureDiagnostic(p.path);
        return false;
    }
    insertAll(sink, p.header);
    insertAll(sink, p.scalarPrefix);
    for (const auto& collection : p.collections)
        if (collection.nonempty) insertAll(sink, collection.firstMember);
    insertAll(sink, p.scalarSuffix);
    for (const auto& row : p.rows) insertAll(sink, row);
    insertAll(sink, p.managerCountAndNumbers);
    if (p.sectionRows.empty()) insertAll(sink, p.emptySection);
    else for (const auto& row : p.sectionRows) insertAll(sink, row);
    insertAll(sink, p.finalLines);
    (void)sink.close();
    return false;
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
