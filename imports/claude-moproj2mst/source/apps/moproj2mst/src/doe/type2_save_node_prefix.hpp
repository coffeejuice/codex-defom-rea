#pragma once

#include <memory>

#include "m2m/xml.hpp"

namespace m2m {
struct Project;
struct SubProject;
namespace engine { class ConversionSession; }
namespace doe {
struct Study;

enum class Type2SaveNextBoundary {
    VariableDimensionReconciliation,
    XmlOutputDevice
};

struct Type2SaveNodePrefixResult {
    Type2SaveNextBoundary nextBoundary;
    xml::NodePtr appendedStudyNode;
    std::weak_ptr<Study> studyOwner; // Diagnostic lease, not native lifetime proof.
    const char* guardMessage() const noexcept;
};

// Source-qualified 099040 Type2 node prefix only. The supplied SUB is the
// caller's cached Save-frame identity, not reselected from activeSubProject.
// Requires actual live Session/Project/SUB/Study/I/O/ModelManager identities.
// Appends to the caller's pending Sub_Project and returns the exact next
// unavailable phase. No file, notification, current-model or public success
// effect occurs. Root must retain the returned guard before any continuation.
Type2SaveNodePrefixResult appendType2SaveNodePrefix(engine::ConversionSession& actualSession,
    Project& actualProject, SubProject& actualSub, xml::Node& pendingSubProject);

} // namespace doe
} // namespace m2m
