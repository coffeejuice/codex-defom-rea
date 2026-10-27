#include "type3_opt_save_attempt.hpp"
#include "type3_opt_count_section.hpp"
#include "type3_opt_region_section.hpp"
#include "type3_optimizer_defopt_view.hpp"
#include "m2m/mst/doe_model.hpp"
#include <limits>
#include <stdexcept>
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
             const Type3OptimizerScalarState* state, Type3OptGroupReceipt* receipt,
             doe::Model* manager)
{
    if (!sink.open(p.path)) {
        sink.assignOpenFailureDiagnostic(p.path);
        return false;
    }
    (void)sink.insertLiteral("DEF_OPT       \t1.0");
    (void)sink.insertEndl();
    (void)sink.insertLiteral("TYP_OPT       \t1");
    (void)sink.insertEndl();
    (void)sink.insertLiteral("GRD_TYP  \t\t10");
    (void)sink.insertEndl();
    // Operands are already prepared; this does not execute native getters.
    (void)sink.insertLiteral("KRG_MAX       \t");
    (void)sink.insertInt(p.krgMax);
    (void)sink.insertEndl();
    (void)sink.insertLiteral("OPR_NO        \t");
    (void)sink.insertInt(p.oprNo);
    (void)sink.insertEndl();
    (void)sink.insertEndl();
    // The native writer saves TYP_OBJ's +0x34 before group collection. Keep
    // the value local: later sink calls may mutate the prepared projection.
    const bool typedProfile = manager && state && receipt;
    if (!typedProfile) insertAll(sink, p.scalarPrefix);
    const std::int32_t savedValue34 = typedProfile ? state->value34 : 0;
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
    if (typedProfile) {
        const std::int32_t savedValue10 = state->value10;
        const std::int32_t savedValue30 = state->value30;
        (void)sink.insertLiteral("TYP_OBJ       ");
        (void)sink.insertInt(savedValue30);
        (void)sink.insertLiteral("        ");
        (void)sink.insertInt(savedValue10);
        (void)sink.insertLiteral("      ");
        (void)sink.insertInt(savedValue34);
        (void)sink.insertEndl();
        (void)sink.insertEndl();
        const Type3OptimizerDefOptView countView(*state);
        const std::int32_t savedNumConCount = countView.numConCount();
        (void)sink.insertLiteral("NUM_CON\t\t");
        (void)sink.insertInt(savedNumConCount);
        (void)sink.insertEndl();
        const Type3OptimizerDefOptView rowView(*state);
        for (std::int32_t i = 0; i < savedNumConCount; ++i) {
            const std::int32_t field4 = rowView.numConField4At(i);
            const double field3 = rowView.numConField3At(i);
            const std::int32_t field2 = rowView.numConField2At(i);
            const std::int32_t field1 = rowView.numConField1At(i);
            (void)sink.insertLiteral("  "); (void)sink.insertInt(i + 1);
            (void)sink.insertLiteral(" "); (void)sink.insertInt(field1);
            (void)sink.insertLiteral("    "); (void)sink.insertInt(field2);
            (void)sink.insertLiteral("      "); (void)sink.insertDouble(field3);
            (void)sink.insertLiteral("    "); (void)sink.insertInt(field4);
            (void)sink.insertEndl();
        }
        (void)sink.insertEndl();
    } else insertAll(sink, p.scalarSuffix);
    if (!(manager && state && receipt))
        for (const auto& row : p.rows) insertAll(sink, row);
    if (manager && state && receipt) {
        // The original manager query runs here, after open and earlier rows.
        // Its reindexing side effect applies even to linked variables.
        const auto unlinked = manager->unlinkedVars();
        if (unlinked.size() > static_cast<std::size_t>((std::numeric_limits<std::int32_t>::max)()))
            throw std::length_error("Type3 OPT manager list exceeds prepared signed-count profile");
        planType3OptCountSection(static_cast<std::int32_t>(unlinked.size()),
            [&](const Type3OptCountEvent& event) {
                switch (event.kind) {
                case Type3OptCountEventKind::literal: (void)sink.insertLiteral(event.literalText); break;
                case Type3OptCountEventKind::integer: (void)sink.insertInt(event.integer); break;
                case Type3OptCountEventKind::endl: (void)sink.insertEndl(); break;
                }
            });
        const Type3OptimizerDefOptView view(*state);
        emitType3OptRegionSection(view, [&](const Type3OptRegionSectionEvent& event) {
            switch (event.kind) {
            case Type3OptRegionSectionEvent::Kind::Literal: (void)sink.insertLiteral(event.literal); break;
            case Type3OptRegionSectionEvent::Kind::Integer: (void)sink.insertInt(event.integer); break;
            case Type3OptRegionSectionEvent::Kind::String: (void)sink.insertString(event.string); break;
            case Type3OptRegionSectionEvent::Kind::Endl: (void)sink.insertEndl(); break;
            }
        });
        (void)sink.insertEndl();
        const double savedValue40 = view.value40();
        (void)sink.insertLiteral("EPS_OBJ       \t");
        (void)sink.insertDouble(savedValue40);
        (void)sink.insertEndl();
        (void)sink.insertEndl();
        const bool savedValue48 = view.value48();
        (void)sink.insertLiteral(savedValue48 ? "DB_PURGE\t\t0" : "DB_PURGE\t\t1");
        (void)sink.insertEndl();
        (void)sink.insertEndl();
        (void)sink.insertLiteral("ENDDATA");
        (void)sink.insertEndl();
        (void)sink.close();
        return false;
    }
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
    return attempt(p, sink, nullptr, nullptr, nullptr);
}
bool attemptType3OptSaveWithGroups(const Type3OptSavePrepared& p,
                                   const Type3OptimizerScalarState& state,
                                   Type3OptAttemptSink& sink, Type3OptGroupReceipt& receipt)
{
    return attempt(p, sink, &state, &receipt, nullptr);
}
bool attemptType3OptSaveWithTypedSections(const Type3OptSavePrepared& p,
                                          const Type3OptimizerScalarState& state,
                                          doe::Model& manager, Type3OptAttemptSink& sink,
                                          Type3OptGroupReceipt& receipt)
{
    return attempt(p, sink, &state, &receipt, &manager);
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
void attemptType3SaveCallerWithTypedSections(bool optimizerPresent, doe::Model* manager,
                                             const Type3OptSavePrepared& p,
                                             const Type3OptimizerScalarState& state,
                                             Type3OptAttemptSink& sink,
                                             Type3OptCallerEffects& effects,
                                             Type3OptGroupReceipt& receipt)
{
    if (optimizerPresent && manager) {
        effects.appendStudy();
        effects.saveInputTable();
        (void)attemptType3OptSaveWithTypedSections(p, state, *manager, sink, receipt);
    }
    effects.commonContinuation();
}
} // namespace m2m::mst::detail
