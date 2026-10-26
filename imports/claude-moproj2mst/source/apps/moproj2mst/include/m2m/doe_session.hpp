#pragma once

#include <cstddef>
#include <optional>

#include "m2m/doe_io.hpp"
#include "m2m/doe_session_records.hpp"
#include "m2m/xml.hpp"

namespace m2m { struct Project; struct Operation; }

namespace m2m::doe {
struct Study;

// Adapter annotation of6477a0's not-yet-appended store-owned record and detached
// DOM handles across an unsupported boundary. This arena is not a second record
// owner or a claim about native scoped cleanup.
struct AdapterDefaultSessionAllocation {
    std::size_t part = 0;
    SessionRecord* record = nullptr; //same actual I/O-bound allocation store
    bool freshNullImage = true;
    bool factoryInvoked = false;
    xml::NodePtr detachedSession;
    xml::NodePtr detachedStepData;
};

// Type2 state of SP+30 (0x1400b5ad0/643100). Part/Session metadata and bounded
// DOE Task list state are represented; image/model payloads remain guarded.
struct SessionDocument {
    SessionDocument();
    qt::String filename;
    xml::Document dom;
    // The original retains a QDomElement reference separately from the document.
    // Nonempty reads create a default PartRoot before parsing; failed parsing
    // need not restore the old reference/tree.
    xml::NodePtr partRoot;
    std::vector<SessionPart> parts;
    //Original+38 QPointer represented as a nonowning ordinal after import.
    std::optional<std::size_t> currentPart;
    std::vector<std::unique_ptr<AdapterDefaultSessionAllocation>> adapterDefaultAllocations;
};

// 0x140644fd0 -> 64bc80/64bd90/647030: clear filename, remove the direct PartRoot
// and append a fresh one. Existing non-PartRoot roots refuse before unsupported
// multiple-document-root behavior; this is not a generic document reset.
void resetSessionDocument(SessionDocument& session);

// 0x140645dc0: remember filename before open. Failed open/empty file keep prior
// DOM. Size is queried on the opened handle, before any read. Nonzero-size input
// replaces DOM before reading/parsing (even when the read yields zero bytes).
// Malformed input refuses after
// discarding old DOM; exact Qt partial-DOM failure content remains unsupported.
// CR-bearing input also refuses after replacement: this XML seam models Text
// reads, whereas the original .DS QFile is opened without Text.
void readSessionDocument(SessionDocument& session, const qt::String& path, StudyIo& io);

// Real DoeOutput load bridge: lazily own the Type2 document, then reset/read it.
// The caller must retain its guard before unresolved part/task/model processing.
void prepareOutputSession(Study& study, const qt::String& path, StudyIo& io);
// Loader callback also enters recovered part import, then guards at the first
// unsupported record/selection effect. Keeps standalone writers independent of DOE.
void prepareCurrentOutputSession(Project& project, Operation& operation);

//6482c0 missing SfTaskDoe→6477a0(doc,-1,false). Preserve constructor/notifier
// effects and detached64fc30 metadata before the first unsupported payload.
// No record or DOM is appended before the serializer finishes.
SessionRecord& constructDefaultDoeSession(Project& project, SessionDocument& document, StudyIo& io);

}  // namespace m2m::doe
