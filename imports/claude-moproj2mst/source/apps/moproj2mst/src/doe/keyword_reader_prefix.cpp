#include "keyword_reader_prefix.hpp"

#include <algorithm>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
#include "m2m/xml.hpp"
#include "keyword_constructor_prefix.hpp"
#include "keyword_constructor_record.hpp"
#include "input_table_reader.hpp"
#include "variable_notify.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void entryGuard()
{
    throw Error(M2M_E_UNSUPPORTED,
        "DoeVariables reader 0x14090ee20: Keyword entry 0x14090f5bf remains unsupported outside the retained TLOC constructor value profile");
}

enum class RetainedProfile { OperationOne, EmptyOperations, FrcrotFunction, Rejected };

bool exactFrcrotText(const xml::Node& wrapper, const qt::String& tag, const qt::String& value)
{
    using xml::Node;
    if (wrapper.kind != Node::Element || wrapper.tag != tag || !wrapper.attrs.empty() ||
        wrapper.children.size() != 1 || !wrapper.children[0]) return false;
    const auto& text = *wrapper.children[0];
    if (text.kind != Node::Element || text.tag != qt::L("string") || !text.attrs.empty() ||
        text.children.size() != 1 || !text.children[0]) return false;
    const auto& raw = *text.children[0];
    return raw.kind == Node::Text && raw.data == value;
}

bool exactFrcrotFunction(const xml::Node& keyword)
{
    using qt::L;
    using xml::Node;
    if (keyword.kind != Node::Element || keyword.tag != L("Keyword") || keyword.attrs.size() != 6 ||
        keyword.attribute(L("name")) != L("FRCROT") || keyword.attribute(L("unitType")) != L("0") ||
        keyword.attribute(L("parmType")) != L("function") || keyword.attribute(L("parameter")) != L("-1") ||
        keyword.attribute(L("nominalValue")) != L("0") || keyword.attribute(L("funcEntryNo")) != L("0") ||
        keyword.hasAttribute(L("parameterName")) || keyword.children.size() != 5) return false;
    for (const auto& child : keyword.children)
        if (!child || child->kind != Node::Element) return false;
    if (!exactFrcrotText(*keyword.children[0], L("NominalData"), L("FRCROT 1 1 0 5")) ||
        !exactFrcrotText(*keyword.children[1], L("UpperData"), L("FRCROT 1 1 0 6")) ||
        !exactFrcrotText(*keyword.children[2], L("LowerData"), L("FRCROT 1 1 0 4"))) return false;
    const auto& operations = *keyword.children[3];
    const auto& entities = *keyword.children[4];
    if (operations.tag != L("KwOperations") || !operations.attrs.empty() || !operations.children.empty() ||
        entities.tag != L("KwEntities") || !entities.attrs.empty() || entities.children.size() != 1 ||
        !entities.children[0]) return false;
    const auto& entity = *entities.children[0];
    return entity.kind == Node::Element && entity.tag == L("KwEntity") && entity.attrs.size() == 1 &&
        entity.attribute(L("id")) == L("1") && entity.children.empty();
}

RetainedProfile retainedProfile(const xml::Node& keyword)
{
    using qt::L;
    if (exactFrcrotFunction(keyword)) return RetainedProfile::FrcrotFunction;
    // Exact spelling avoids widening Qt numeric/locale/codec reconstruction.
    // These are admission bounds, not native validation or QString ref parity.
    if (keyword.kind != xml::Node::Element || keyword.tag != L("Keyword") || keyword.attrs.size() != 5 ||
        keyword.attribute(L("name")) != L("TLOC") || keyword.attribute(L("unitType")) != L("0") ||
        keyword.attribute(L("parmType")) != L("double") || keyword.attribute(L("parameter")) != L("-1") ||
        keyword.attribute(L("nominalValue")) != L("0.1") || keyword.hasAttribute(L("parameterName")))
        return RetainedProfile::Rejected;
    const auto children = keyword.childElements();
    if (children.size() != 2 || children[0]->tag != L("NominalData") ||
        children[1]->tag != L("KwOperations") || !children[0]->attrs.empty() || !children[1]->attrs.empty())
        return RetainedProfile::Rejected;
    // Native inspects firstChild once. Never search past raw/comment content.
    const auto& nominal = *children[0];
    if (nominal.children.size() != 1 || !nominal.children.front()) return RetainedProfile::Rejected;
    const auto& text = *nominal.children.front();
    if (text.kind != xml::Node::Element || text.tag != L("string") || !text.attrs.empty() ||
        !text.childElements().empty() || text.text() != L("TLOC    \t0.1"))
        return RetainedProfile::Rejected;
    if (children[1]->children.empty()) return RetainedProfile::EmptyOperations;
    const auto operations = children[1]->childElements();
    return operations.size() == 1 && operations[0]->tag == L("KwOperation") &&
        operations[0]->attrs.size() == 1 && operations[0]->attribute(L("id")) == L("1") &&
        operations[0]->children.empty() ? RetainedProfile::OperationOne : RetainedProfile::Rejected;
}
}

[[noreturn]] void readBoundedKeywordConstructorPrefix(
    Study& study, const qt::String& originalName, int readerLocalDimension, const xml::Node& keyword,
    const InputTableSource* source)
{
    const auto profile = retainedProfile(keyword);
    if (study.samples.mode != 0 || readerLocalDimension != 3 || profile == RetainedProfile::Rejected) entryGuard();
    // 91095a -> derived vt+118 -> base91a5b0 selects the original name. The
    // reader's last-record Header/Description target can be a different object.
    HeaderVariableRecord* receiver = nullptr;
    for (const auto* alias : study.vars) {
        if (!alias) entryGuard();
        const auto owner = std::find_if(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
            [alias](const auto& candidate) { return candidate && candidate.get() == alias; });
        if (owner == study.headerVariableOwners.end()) entryGuard();
        if (!receiver && (*owner)->name == originalName) receiver = owner->get();
    }
    if (!receiver) entryGuard();
    // TLOC ID8 and FRCROT ID276 are byte-qualified in the original registry. This is a bounded
    // value projection, not an invocation-bound toLocal8Bit/data-owner receipt.
    const bool frcrot = profile == RetainedProfile::FrcrotFunction;
    const KeywordConstructorPrefixInput decoded{frcrot ? 276 : 8, qt::String(), 0, readerLocalDimension,
        frcrot ? 0.0 : 0.1,
        profile == RetainedProfile::OperationOne ? std::vector<int>{1} : std::vector<int>{},
        frcrot ? std::vector<int>{1} : std::vector<int>{}};
    const auto appended = appendFreshKeywordConstructorPrefix(study, *receiver, decoded);
    if (profile == RetainedProfile::EmptyOperations || frcrot) {
        // Native's zero-count operation loop skips SetOprHasDoe entirely. Re-fetch
        // by the original row name before either text-setting prefix.
        HeaderVariableRecord* keywordOwner = nullptr;
        for (const auto* alias : study.vars) {
            if (!alias) entryGuard();
            const auto owner = std::find_if(study.headerVariableOwners.begin(), study.headerVariableOwners.end(),
                [alias](const auto& candidate) { return candidate && candidate.get() == alias; });
            if (owner == study.headerVariableOwners.end()) entryGuard();
            if ((*owner)->name == originalName) { keywordOwner = owner->get(); break; }
        }
        if (!keywordOwner || keywordOwner != receiver || keywordOwner->items.empty() ||
            keywordOwner->items.back().keyword() != appended.record) entryGuard();
        auto* record = keywordOwner->items.back().keyword();
        if (!record) entryGuard();
        record->parameter = -1; // +80 selector
        record->function = frcrot;
        record->enumeration = false;
        record->nominalText = qt::String(frcrot ? qt::L("FRCROT 1 1 0 5") : qt::L("TLOC    \t0.1"));
        if (frcrot) {
            // Three single-line ASCII setter values are unchanged by CR removal/LF expansion.
            // Entity 1 supplies the reader-local [1,1] selector at 0x14091101a.
            // Keep it invocation-local: the constructor record retains entity [1].
            const std::vector<int> selector{record->entities.front(), record->entities.front()};
            record->upperText = qt::String(qt::L("FRCROT 1 1 0 6"));
            record->lowerText = qt::String(qt::L("FRCROT 1 1 0 4"));
            if (selector != std::vector<int>({1, 1})) entryGuard();
            // Value-only handoff complete. Stop before converter call 0x14091111d;
            // no file, scanner, parser, table, or output effects are projected.
            throw Error(M2M_E_UNSUPPORTED,
                "DoeVariables reader 0x14090ee20: FRCROT selector [1,1] and text handoff committed; M2M_E_UNSUPPORTED before converter call 0x14091111d");
        }
        throw Error(M2M_E_UNSUPPORTED,
            "DoeVariables reader 0x14090ee20: scalar Keyword nominal-text prefix committed; stopped at 0x140910abf before upper-text probe");
    }
    if (source && source->notifier) {
        // Derived0d25a0 sends int operation first, then bool true. Validate the
        // reached caller identities only now, retaining its constructor prefix.
        const auto outcome = enterSetOprHasDoeNotifierPrefix(*source->notifier, study, 1, true);
        if (outcome == SetOprHasDoeEntryOutcome::SuppressedCalleeReturned)
            throw Error(M2M_E_UNSUPPORTED,
                "DoeVariables reader 0x14090ee20: suppressed SetOprHasDoe returned; post-notifier named keyword lookup remains unsupported");
        throw Error(M2M_E_UNSUPPORTED,
            "DoeVariables reader 0x14090ee20: notifier entry reached before unresolved SetOprHasDoe key construction");
    }
    // Legacy/source-less callers do not gain an implicit Session context.
    throw Error(M2M_E_UNSUPPORTED,
        "DoeVariables reader 0x14090ee20: TLOC constructor prefix committed before unresolved SetOprHasDoe notifier invocation");
}
} // namespace m2m::doe
