#pragma once

#include <cstddef>
#include <vector>

#include "m2m/mst/doe_model.hpp"

namespace m2m::xml { struct Node; }
namespace m2m::doe {
struct Study;
inline constexpr std::size_t kNativeVariableRecordAllocationBytes = 0x48;

// Constructor/header value projection, not a native-layout or Qt refcount owner.
// Study deletes this concrete type; Var has no virtual destructor.
struct HeaderVariableRecord final : Var {
    explicit HeaderVariableRecord(int samples);
    String description;     // +08, default QString
    int numSamples;         // +10
    int f14 = 0;            // +14
    int dimension = 3;      // +24
};

// One native reader invocation queues borrowed source pointers and target names.
// Study retains all concrete record ownership; these requests never own records.
struct DeferredVariableLink {
    HeaderVariableRecord* source;
    String targetName;
};
using DeferredVariableLinks = std::vector<DeferredVariableLink>;

// Mode0 header/Description rows on this concrete owner only. Existing names
// skip creation; the native reader's following setters target the last record.
// Absent varId skips its setter; normal outer InputTable finalization is later.
// Absent dimension skips its setter; the native parser's local default is3.
// Absent name is null; parsed explicit-empty name is nonnull empty. Lookup
// compares content, so either can match an existing null/empty record name.
// Missing or parsed explicit-empty numSamples yields0 only for nonempty name, positive varId
// and dimension3. A duplicate ignores that value; existing last-record setters
// still run. Exact textual0 is duplicate-only under that explicit-header bound;
// its converted argument is unused, and no fresh0 numeric value is assumed.
// Other nonpositive/malformed samples and missing-header combinations remain
// guarded. Native QLocale initialization/provider state is not replayed.
// Nonempty LinkTo queues this row's actual last record before Description.
// Direct Keyword admits two bounded scalar TLOC profiles. Each resolves the
// original row name to a concrete Header owner and appends a canonical
// constructor record. Operation1 reaches the bounded notifier entry when a
// source context exists; parsed-empty operations skip notifier, commit nominal
// text and stop at the upper-text probe. Other profiles stop at child entry.
// Neither path reaches outer reindex or queued-link resolution; native
// QString/codec lifetime remains unmodeled.
// Direct Geometry likewise throws before actual-input-filename QFileInfo;
// no geometry metadata/file record or path ownership is constructed.
// False leaves this row uncommitted and retains its matched-row guard.
// Not general native input validation or Keyword/Geometry payload ownership.
bool appendBoundedHeaderVariable(Study&, const xml::Node&, DeferredVariableLinks&);
// After normal whole-container reindex, resolve on the same coherent Study
// owner in queue order. A missing target preserves its source's existing link.
void resolveDeferredVariableLinks(Study&, const DeferredVariableLinks&);
} // namespace m2m::doe
