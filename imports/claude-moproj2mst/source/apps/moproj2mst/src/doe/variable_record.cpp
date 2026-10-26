#include "m2m/doe_variable_record.hpp"

#include <cstdint>
#include <limits>
#include <memory>
#include <optional>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "geometry_row_prefix.hpp"
#include "keyword_reader_prefix.hpp"
#include "input_table_reader.hpp"

namespace m2m::doe {
namespace {
std::optional<int> canonicalPositive(const String& text)
{
    const auto& value = text.u16();
    if (value.empty() || value.front() < u'1' || value.front() > u'9') return {};
    int number = 0;
    for (const auto ch : value) {
        if (ch < u'0' || ch > u'9') return {};
        const int digit = ch - u'0';
        if (number > (std::numeric_limits<int>::max() - digit) / 10) return {};
        number = number * 10 + digit;
    }
    return number;
}
}

HeaderVariableRecord::HeaderVariableRecord(int samples) : numSamples(samples)
{
    // 0x140816900; the legacy Var projection has different defaults.
    index = -1;
    number = 0;
    geoType = 100;
    link = nullptr;
}

GeometryRowPrefixResult applyGeometryRowPrefix(HeaderVariableRecord& variable, const GeometryRowPrefixInput& input)
{
    // Reconstruction admission only: a legacy row has no qualified native
    // integer key. Inspect the whole owner before performing any prefix write.
    for (const auto& row : variable.files)
        if (!row.geoOperation || !row.geoObject)
            throw Error(M2M_E_UNSUPPORTED, "DOE Geometry row prefix: existing row key unknown");

    std::size_t matched = 0;
    for (; matched < variable.files.size(); ++matched) {
        const auto& row = variable.files[matched];
        // 0x14081762c..76b3: nominal is deliberately absent from this key.
        if (row.before == input.upperGeoFile && row.after == input.lowerGeoFile &&
            *row.geoOperation == input.geoOperation && *row.geoObject == input.geoObject) break;
    }
    const bool appended = matched == variable.files.size();
    if (appended) {
        // 0x140818790 constructs paths/containers and false flags; key ints
        // are written by 0x1408175c0 before publishing to Var+0x28.
        GeoFile row;
        row.path = input.nominalGeoFile;
        row.before = input.upperGeoFile;
        row.after = input.lowerGeoFile;
        row.geoOperation = input.geoOperation;
        row.geoObject = input.geoObject;
        variable.files.push_back(std::move(row));
    }

    const auto tail = variable.files.size() - 1;
    // 0x140911f99..11ffe: even a match at an earlier index is followed by
    // setters on count-1. Preserve the asymmetric absent-attribute defaults.
    variable.files[tail].useBefore = input.upperDefined
        ? *input.upperDefined != qt::L("false") : !input.upperGeoFile.isEmpty();
    variable.files[tail].useAfter = input.lowerDefined
        ? *input.lowerDefined != qt::L("false") : true;
    variable.geoType = input.morphMethod; // 0x140817890, after both flag writes.
    return {appended, matched, tail, GeometryRowPrefixBoundary::CorrespondencePayload};
}

bool appendGeometryCorrespondencePair(HeaderVariableRecord& variable, int rowIndex,
    GeometryCorrespondenceSide side, const GeometryPoint& first, const GeometryPoint& second)
{
    // Native wrappers return before pair construction/row selection on either
    // failed signed bounds check. No key lookup, flag check or list clear runs.
    if (rowIndex < 0 || static_cast<std::size_t>(rowIndex) >= variable.files.size()) return false;
    const GeometryPointPair pair{first, second}; // Snapshot both inputs before append.
    auto& row = variable.files[static_cast<std::size_t>(rowIndex)];
    auto& pairs = side == GeometryCorrespondenceSide::Upper
        ? row.upperCorrespondence : row.lowerCorrespondence;
    pairs.push_back(pair); // One normal append, not replacement; host failures propagate.
    return true;
}

bool appendBoundedHeaderVariable(Study& study, const xml::Node& row, DeferredVariableLinks& deferredLinks)
{
    return appendBoundedHeaderVariableFromSource(study, row, deferredLinks, nullptr);
}

bool appendBoundedHeaderVariableFromSource(Study& study, const xml::Node& row,
    std::vector<DeferredVariableLink>& deferredLinks, const InputTableSource* source)
{
    static_assert(sizeof(int) == sizeof(std::int32_t));
    using qt::L;
    if (row.kind != xml::Node::Element || row.tag != L("DoeVariable")) return false;
    // Unknown other elements retain their pre-append reconstruction guard.
    // Direct Keyword/Geometry instead stop at their reached child entries.
    for (const auto& child : row.children)
        if (child && child->kind == xml::Node::Element &&
            child->tag != L("Description") && child->tag != L("Keyword") &&
            child->tag != L("Geometry")) return false;
    if (study.samples.mode != 0 ||
        !study.scalarPrefixVariableOwners.empty() || !study.geometryPrefixVariableOwners.empty() ||
        study.retainedScalarPrefix()) return false;
    if (study.vars.size() != study.headerVariableOwners.size() ||
        study.vars.size() >= static_cast<std::size_t>(std::numeric_limits<int>::max())) return false;
    for (std::size_t i = 0; i < study.vars.size(); ++i) {
        // Compare identities before following a borrowed alias. Never adopt a
        // foreign/raw Var merely because its values resemble this record type.
        if (!study.headerVariableOwners[i] || study.vars[i] != study.headerVariableOwners[i].get()) return false;
    }
    // Admission narrows conversion/child/owner branches before native allocation;
    // it is not evidence that original QString conversion rejects these strings.
    const auto name = row.attribute(L("name")); // 0x14090ef74: absent returns default-null QString.
    const bool hasSamples = row.hasAttribute(L("numSamples"));
    const auto sampleText = row.attribute(L("numSamples"));
    const bool defaultSamples = !hasSamples || sampleText.isEmpty();
    const bool textualZero = sampleText == L("0");
    const auto samples = defaultSamples ? std::optional<int>(0) : canonicalPositive(sampleText);
    const bool hasId = row.hasAttribute(L("varId"));
    std::optional<int> id;
    if (hasId) id = canonicalPositive(row.attribute(L("varId")));
    const bool hasDimension = row.hasAttribute(L("dimension"));
    if ((!samples && !textualZero) || (hasId && !id) ||
        (hasDimension && row.attribute(L("dimension")) != L("3"))) return false;
    // Missing/default-null or parsed explicit-empty numSamples reaches native
    // toInt(value, nullptr, 10) ->0, preserving the parsed value's length0 path.
    // Bound this increment to otherwise explicit headers; this is reconstruction
    // admission, not native validation or a replay of default QLocale lifetime.
    if ((defaultSamples || textualZero) && (name.isEmpty() || !hasId || !hasDimension)) return false;
    bool existingName = false;
    for (const auto& owned : study.headerVariableOwners) {
        if (owned->name == name) { existingName = true; break; }
    }
    // Native duplicate helper never reads the converted sample argument. Do
    // not synthesize its value or admit a fresh0 owner while actual system
    // locale zero-digit/provider state remains unqualified. samples stays
    // disengaged on this exact textual0 profile and cannot reach construction.
    if (textualZero && !existingName) return false;
    HeaderVariableRecord* variable = nullptr;
    if (existingName) {
        // 0x140919feb: helper returns the matching record without creation,
        // selection or invalidation. 0x14090f0b2..f0ef ignores that pointer
        // and obtains count-1 for every following header/child setter.
        variable = study.headerVariableOwners.back().get();
    } else {
        auto record = std::make_unique<HeaderVariableRecord>(*samples);
        variable = record.get();
        study.headerVariableOwners.push_back(std::move(record)); // Host lifetime, no native append claim.
        try {
            study.vars.push_back(variable); // 0x140919ec7: append pointer before name assignment.
        } catch (...) {
            study.headerVariableOwners.pop_back(); // No observable alias was appended.
            throw;
        }
        variable->name = name; // 0x140919efe
        // Native vt+1e8 scans all records, including this constructor-number0
        // record, even with false assignment flag. Its candidate is discarded.
        study.selectedVariableIndex = static_cast<int>(study.vars.size() - 1); // 0x140919f8b, unique-name lookup.
        study.samples.invalid.fill(true); // 0x140919fb8; mode0 skips design acquisition.
    }
    // 0x14090f150 skips the setter entirely when varId is absent. Preserve
    // constructor0 or the existing last object's number until the outer pass.
    if (id) variable->number = *id; // 0x14090f225, after create helper returns.
    // 0x14090f27d skips the record setter when absent. The reader's local3
    // is not an unconditional store, especially on existing-name rows.
    if (hasDimension) variable->dimension = 3; // 0x14090f352
    const auto targetName = row.attribute(L("LinkTo"));
    if (!targetName.isEmpty()) {
        // 0x14090f3f6/408: source is the reader's last record, even when
        // the row name matched an earlier object. No link store happens yet.
        deferredLinks.push_back({variable, targetName});
    }
    for (const auto& child : row.children) {
        if (!child || child->kind != xml::Node::Element) continue;
        if (child->tag == L("Keyword")) {
            // 91095a selects by original name, independently of this reader's
            // last-record Header target. The native reader-local dimension is3.
            readBoundedKeywordConstructorPrefix(study, name, 3, *child, source);
        }
        if (child->tag == L("Geometry")) {
            // 0x140911532: native next constructs QFileInfo from the actual
            // input filename. No path result or geometry payload is supplied.
            throw Error(M2M_E_UNSUPPORTED,
                "DoeVariables reader 0x14090ee20: Geometry entry 0x140911532 remains unsupported before QFileInfo filename/path ownership");
        }
        // 0x14090f4e9..0x14090f543: direct Description text, assigned only
        // when nonempty. Setter0x14010f200 performs no newline replacement.
        const auto text = child->text();
        if (!text.isEmpty()) variable->description = text;
    }
    // Caller reindexes after its entire variable-node loop, then binds links.
    return true;
}

void resolveDeferredVariableLinks(Study& study, const DeferredVariableLinks& deferredLinks)
{
    // Same invocation's admitted rows preserve coherent concrete owner/list
    // order. No deletion/callback occurs between queueing and this native tail.
    for (const auto& request : deferredLinks) {
        if (!request.source) continue; // 0x14091290f
        for (const auto& target : study.headerVariableOwners) {
            if (target->name == request.targetName) {
                // vt+180 first exact name, vt+1a0 pointer, setter0x140100a60.
                // Raw +40 store only; self/cycles are nonowning, not flattened.
                request.source->link = target.get();
                break;
            }
        }
        // No target: native null branch skips the setter, retaining old link.
    }
}
} // namespace m2m::doe
