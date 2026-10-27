// Internal CP292 static-contract reconstruction. Public render binding remains HOLD.
#pragma once

#include <cstdint>
#include <optional>
#include <vector>

namespace m2m::engine {

enum class TransformOutcome {
    completed,
    admissionRejected,
    indeterminateCandidate,
    outsideQualifiedArithmetic,
    outsideQualifiedSize,
    outsideQualifiedAllocation
};
enum class TransformAdmission {
    none, nonpositiveCount, negligibleWidth, nullInput, nullOutput
};

struct CurveRenderTransformResult {
    TransformOutcome outcome = TransformOutcome::admissionRejected;
    TransformAdmission admission = TransformAdmission::none;
    std::optional<int> nativeStatus;
    std::int32_t retainedCount = 0;
    bool closed = false;
    bool outputWritten = false;
    // Set only when correction encounters the ignored native mode-1 failure.
    std::optional<std::int32_t> correctionSegment;
    // Retained-point core output at a stopped correction. This is diagnostic
    // partial state, never a completed rendering or expanded caller output.
    std::vector<double> diagnosticTransformedXY;
};

// @src 0x1417d0850, 0x1417d0b90 and geometry helpers, qualified by
// analysis/resume292-render-transform-static.md. Count is signed, xy/out have
// at least 2*count doubles when nonnull. Ordered native admission precedes
// finite-domain classification; NaN width passes the negligible-width test.
// On completion expand grouped output and report native 0. Native admission
// rejection reports -1 and leaves out untouched. Unqualified paths have no
// invented native status or caller output: this analysis seam stops before an
// indeterminate read, allocation fault or exceptional floating result. That
// stopping policy is not a recovered guard/rollback in the native caller.
// Build without reassociation, fast-math or floating-point contraction.
CurveRenderTransformResult transformCurveRender2D(
    std::int32_t count, const double* xy, double width, double* out, bool relative);

} // namespace m2m::engine
