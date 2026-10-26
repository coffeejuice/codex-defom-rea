#pragma once

#include <cstddef>
#include <optional>

#include "m2m/doe_variable_record.hpp"

namespace m2m::doe {
// Already-projected values, not proof of native QFileInfo or numeric parsing.
struct GeometryRowPrefixInput {
    String nominalGeoFile;
    String upperGeoFile;
    String lowerGeoFile;
    int geoOperation;
    int geoObject;
    int morphMethod;
    std::optional<String> upperDefined;
    std::optional<String> lowerDefined;
};

enum class GeometryRowPrefixBoundary { CorrespondencePayload };
struct GeometryRowPrefixResult {
    bool appended;
    std::size_t matchedRowIndex;
    std::size_t tailRowIndex;
    GeometryRowPrefixBoundary nextBoundary = GeometryRowPrefixBoundary::CorrespondencePayload;
};

// Isolated receiver prefix on the concrete record retained by Study. Uses the
// existing Var::files owner; unknown legacy row keys refuse before mutation.
// Native lookup excludes nominal, then reader setters always use the tail.
// No public parser binding, correspondence payload, file I/O or notification.
GeometryRowPrefixResult applyGeometryRowPrefix(HeaderVariableRecord&, const GeometryRowPrefixInput&);

enum class GeometryCorrespondenceSide { Upper, Lower };
// @src 0x140818000 / 0x140818080: one pair append at a valid signed row index.
// Already-converted point values only; no XML/QLocale or QFileInfo admission.
// The native reader passes row0, independently of the prior tail flag writes.
// Return is a host diagnostic: false for the native invalid-index no-op, true
// after normal append. Native methods are void; no allocator parity is claimed.
bool appendGeometryCorrespondencePair(HeaderVariableRecord&, int rowIndex,
    GeometryCorrespondenceSide, const GeometryPoint& first, const GeometryPoint& second);
} // namespace m2m::doe
