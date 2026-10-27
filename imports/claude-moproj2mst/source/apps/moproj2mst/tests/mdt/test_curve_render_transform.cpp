// CP292 recovered-static-contract checks; these are not original runtime parity.
// ctest-labels: m2m-mdt mdt-viewer
#include <testkit/check.hpp>
#if __has_include("../../src/engine/curve_render_transform.hpp")
#include "../../src/engine/curve_render_transform.hpp"
#else
#include "curve_render_transform.hpp" // Isolated staging compile only.
#endif

#include <array>
#include <cmath>
#include <cstdint>
#include <limits>

using namespace m2m::engine;
namespace {
void completed(const CurveRenderTransformResult& result) {
    REQUIRE(result.outcome == TransformOutcome::completed);
    REQUIRE(result.nativeStatus.has_value());
    CHECK_EQ(*result.nativeStatus,0);
    CHECK(result.outputWritten);
    CHECK(result.diagnosticTransformedXY.empty());
}
}

TEST_CASE(render_transform_admission_order_and_untouched_output) {
    const double p[] = {0,0,2,0};
    double out[] = {101,102,103,104};
    auto result = transformCurveRender2D(0,nullptr,0,nullptr,false);
    CHECK(result.admission == TransformAdmission::nonpositiveCount);
    REQUIRE(result.nativeStatus.has_value()); CHECK_EQ(*result.nativeStatus,-1);
    result = transformCurveRender2D(2,nullptr,1e-12,nullptr,false);
    CHECK(result.admission == TransformAdmission::negligibleWidth);
    result = transformCurveRender2D(2,nullptr,1,nullptr,false);
    CHECK(result.admission == TransformAdmission::nullInput);
    result = transformCurveRender2D(2,p,1,nullptr,false);
    CHECK(result.admission == TransformAdmission::nullOutput);
    result = transformCurveRender2D(2,p,-1e-12,out,false);
    CHECK(result.outcome == TransformOutcome::admissionRejected);
    CHECK(!result.outputWritten);
    CHECK_BITS(out[0],101.0); CHECK_BITS(out[3],104.0);
}

TEST_CASE(render_transform_nan_width_passes_ordered_admission) {
    const double p[] = {0,0,2,0};
    double out[] = {101,102,103,104};
    const double nan = std::numeric_limits<double>::quiet_NaN();
    auto result = transformCurveRender2D(2,nullptr,nan,nullptr,false);
    CHECK(result.admission == TransformAdmission::nullInput);
    result = transformCurveRender2D(2,p,nan,out,false);
    CHECK(result.admission == TransformAdmission::none);
    CHECK(result.outcome == TransformOutcome::outsideQualifiedArithmetic);
    CHECK(!result.nativeStatus.has_value()); CHECK(!result.outputWritten);
    CHECK_BITS(out[1],102.0);
}

TEST_CASE(render_transform_positive_two_point_absolute_and_relative) {
    const double p[] = {0,0,2,0};
    double out[4] = {};
    auto result = transformCurveRender2D(2,p,0.25,out,false);
    completed(result); CHECK_EQ(result.retainedCount,2); CHECK(!result.closed);
    CHECK_BITS(out[0],0.0); CHECK_BITS(out[1],0.25);
    CHECK_BITS(out[2],2.0); CHECK_BITS(out[3],0.25);
    result = transformCurveRender2D(2,p,0.25,out,true);
    completed(result);
    CHECK_BITS(out[0],0.0); CHECK_BITS(out[1],0.5);
    CHECK_BITS(out[2],2.0); CHECK_BITS(out[3],0.5);
}

TEST_CASE(render_transform_positive_vertical_two_point) {
    const double p[] = {3,4,3,6};
    double out[4] = {};
    completed(transformCurveRender2D(2,p,0.5,out,false));
    CHECK_BITS(out[0],2.5); CHECK_BITS(out[1],4.0);
    CHECK_BITS(out[2],2.5); CHECK_BITS(out[3],6.0);
}

TEST_CASE(render_transform_group_equality_and_forced_final_representative) {
    // D=10000 and threshold=1: equality coalesces with retained point zero.
    const double equal[] = {0,0,1,0,10000,0};
    double out[6] = {};
    auto result = transformCurveRender2D(3,equal,0.25,out,false);
    completed(result); CHECK_EQ(result.retainedCount,2);
    CHECK_BITS(out[0],0.0); CHECK_BITS(out[2],0.0);
    CHECK_BITS(out[1],0.25); CHECK_BITS(out[3],0.25);
    CHECK_BITS(out[4],10000.0);
    // Final input is coalesced but unconditionally replaces its group exemplar.
    const double final[] = {0,0,10000,0,10000.5,0};
    result = transformCurveRender2D(3,final,0.25,out,false);
    completed(result); CHECK_EQ(result.retainedCount,2);
    CHECK_BITS(out[2],10000.5); CHECK_BITS(out[4],10000.5);
    CHECK_BITS(out[3],0.25); CHECK_BITS(out[5],0.25);
}

TEST_CASE(render_transform_open_collinear_interior_has_unit_scale) {
    const double p[] = {0,0,2,0,4,0};
    double out[6] = {};
    const auto result = transformCurveRender2D(3,p,0.5,out,false);
    completed(result); CHECK_EQ(result.retainedCount,3); CHECK(!result.closed);
    CHECK_BITS(out[0],0.0); CHECK_BITS(out[1],0.5);
    CHECK_BITS(out[2],2.0); CHECK_BITS(out[3],0.5);
    CHECK_BITS(out[4],4.0); CHECK_BITS(out[5],0.5);
}

TEST_CASE(render_transform_open_corner_has_recovered_interior_displacement) {
    const double p[] = {0,0,2,0,2,2};
    double out[6] = {};
    const auto result = transformCurveRender2D(3,p,0.25,out,false);
    completed(result); CHECK(!result.closed);
    CHECK_BITS(out[0],0.0); CHECK_BITS(out[1],0.25);
    // Exact derived geometry is (1.75, .25); sqrt/normal/scale order can round.
    CHECK(std::fabs(out[2]-1.75) < 1e-14);
    CHECK(std::fabs(out[3]-0.25) < 1e-14);
    CHECK_BITS(out[4],1.75); CHECK_BITS(out[5],2.0);
}

TEST_CASE(render_transform_closed_square_copies_seam_and_uses_cyclic_normals) {
    const double p[] = {0,0,4,0,4,4,0,4,0,0};
    double out[10] = {};
    const auto result = transformCurveRender2D(5,p,0.25,out,false);
    completed(result); CHECK(result.closed); CHECK_EQ(result.retainedCount,5);
    // All vertices move inward by .25 on each axis. Last is copied from first.
    const double expected[] = {.25,.25,3.75,.25,3.75,3.75,.25,3.75,.25,.25};
    for (int i = 0; i != 10; ++i) CHECK(std::fabs(out[i]-expected[i]) < 1e-14);
    CHECK_BITS(out[0],out[8]); CHECK_BITS(out[1],out[9]);
}

TEST_CASE(render_transform_negative_width_stops_before_indeterminate_candidate) {
    const double p[] = {0,0,2,0};
    double out[] = {101,102,103,104};
    const auto result = transformCurveRender2D(2,p,-0.25,out,false);
    CHECK(result.outcome == TransformOutcome::indeterminateCandidate);
    REQUIRE(result.correctionSegment.has_value()); CHECK_EQ(*result.correctionSegment,0);
    CHECK(!result.nativeStatus.has_value()); CHECK(!result.outputWritten);
    REQUIRE(result.diagnosticTransformedXY.size() == 4);
    CHECK_BITS(result.diagnosticTransformedXY[1],-0.25);
    CHECK_BITS(result.diagnosticTransformedXY[3],-0.25);
    CHECK_BITS(out[0],101.0); CHECK_BITS(out[3],104.0);
}

TEST_CASE(render_transform_classifies_nonfinite_and_count_overflow_separately) {
    const double p[] = {0,0,2,0};
    double out[] = {101,102,103,104};
    auto result = transformCurveRender2D(std::numeric_limits<std::int32_t>::max(),p,1,out,false);
    CHECK(result.outcome == TransformOutcome::outsideQualifiedSize);
    CHECK(!result.nativeStatus.has_value());
    const double huge[] = {-std::numeric_limits<double>::max(),0,
                            std::numeric_limits<double>::max(),0};
    result = transformCurveRender2D(2,huge,1,out,false);
    CHECK(result.outcome == TransformOutcome::outsideQualifiedArithmetic);
    CHECK(!result.outputWritten); CHECK_BITS(out[0],101.0);
    const double invalid[] = {0,0,std::numeric_limits<double>::infinity(),0};
    result = transformCurveRender2D(2,invalid,1,out,false);
    CHECK(result.outcome == TransformOutcome::outsideQualifiedArithmetic);
}

TEST_CASE(render_transform_single_group_keeps_finite_zero_normal_result) {
    const double p[] = {3,4,3,4};
    double out[4] = {};
    const auto result = transformCurveRender2D(2,p,1,out,false);
    completed(result); CHECK_EQ(result.retainedCount,1); CHECK(!result.closed);
    CHECK_BITS(out[0],3.0); CHECK_BITS(out[1],4.0);
    CHECK_BITS(out[2],3.0); CHECK_BITS(out[3],4.0);
}

TEST_CASE(render_transform_smoothing_reads_all_old_normals) {
    const double p[] = {0,0,2,0,4,2,4,6};
    double out[8] = {};
    const auto result = transformCurveRender2D(4,p,0.25,out,false);
    completed(result); CHECK(!result.closed);
    // Independently derived old normals have angles 90,112.5,157.5,180 degrees.
    // The neighboring sums have angles 101.25,135,168.75 degrees. With no
    // directional clipping, the first limiting edge is diagonal and the second
    // is vertical, fixing x2=3.75.
    const double pi = std::acos(-1.0);
    const double s22 = std::sin(pi/8), c22 = std::cos(pi/8);
    const double s11 = std::sin(pi/16), c11 = std::cos(pi/16);
    const double h = std::sqrt(0.5);
    const double c1x = -s22-0.15625*s11-0.078125*h;
    const double c1y = c22+0.15625*c11+0.078125*h;
    const double c2x = -c22-0.078125*h-0.0390625*c11;
    const double c2y = s22+0.078125*h+0.0390625*s11;
    CHECK(std::fabs(out[2]-(2+0.25*std::sqrt(2.0)*c1x/(c1y-c1x))) < 1e-14);
    CHECK(std::fabs(out[3]-(0.25*std::sqrt(2.0)*c1y/(c1y-c1x))) < 1e-14);
    CHECK(std::fabs(out[4]-3.75) < 1e-14);
    CHECK(std::fabs(out[5]-(2+0.25*c2y/(-c2x))) < 1e-14);
}

TEST_CASE(render_transform_segment_correction_uses_shortened_interval) {
    const double p[] = {0,0,4,-4,6,-4,6,0,0,6,6.5,0};
    double out[12] = {};
    const auto result = transformCurveRender2D(6,p,6.0,out,false);
    completed(result); CHECK(!result.closed); CHECK_EQ(result.retainedCount,6);
    // The endpoint normal is (1,1)/sqrt(2). The initial quad intersects its
    // y=x ray at t=3.20161546540393498, strictly beyond both later crossings.
    // Original segment j=3 is x+y=6, so it shortens the ray to exactly (3,3).
    // Segment j=4 joins (0,6) to (6.5,0), crossing at (78/25,78/25).
    // Its 3.12 coordinate must now fail segment containment; processing each
    // original segment against a frozen pre-j=3 interval would overwrite (3,3).
    // The analytic ray/determinant derivation is retained in STAGING_REPORT.md.
    CHECK_BITS(out[0],3.0); CHECK_BITS(out[1],3.0);
    CHECK(out[0] < 78.0/25.0); CHECK(out[1] < 78.0/25.0);
}

TEST_CASE(render_transform_unequal_closed_seam_copies_first_to_last) {
    const double p[] = {0,0,4,0,0,2,0,0};
    double out[8] = {};
    const auto result = transformCurveRender2D(4,p,1.0,out,false);
    completed(result); CHECK(result.closed); CHECK_EQ(result.retainedCount,4);
    // Independent 80-digit Decimal sqrt/vector-determinant expectations.
    // Pre-seam outputs are Q01,Q12,Q20,Q20. Their unequal seam distances
    // are 1.97289985193817486 < 1.98923790079153756, selecting first->last.
    const double expected[] = {
        1.8692900517495214212,0.63094257124396338170,
        1.9077155228133837127,0.61956407951908046075,
        1.8847700834179234250,0.63616755544299569284,
        1.8692900517495214212,0.63094257124396338170};
    CHECK(std::hypot(expected[4],expected[5]) > std::hypot(expected[0],expected[1])+0.016);
    for (int i = 0; i != 8; ++i) CHECK(std::fabs(out[i]-expected[i]) < 1e-14);
    CHECK_BITS(out[0],out[6]); CHECK_BITS(out[1],out[7]);
}

TEST_CASE(render_transform_unequal_closed_seam_copies_last_to_first) {
    const double p[] = {0,0,2,0,0,4,0,0};
    double out[8] = {};
    const auto result = transformCurveRender2D(4,p,1.0,out,false);
    completed(result); CHECK(result.closed); CHECK_EQ(result.retainedCount,4);
    // Transpose the wide triangle and reverse its vertex order to preserve
    // CCW winding. Before the seam, outputs are T(Q20),T(Q20),T(Q12),T(Q01).
    // First distance 1.98923790079153756 > last 1.97289985193817486 strictly,
    // so the last output replaces the first, including both coordinates.
    const double expected[] = {
        0.63094257124396338170,1.8692900517495214212,
        0.63616755544299569284,1.8847700834179234250,
        0.61956407951908046075,1.9077155228133837127,
        0.63094257124396338170,1.8692900517495214212};
    CHECK(std::hypot(expected[2],expected[3]) > std::hypot(expected[0],expected[1])+0.016);
    for (int i = 0; i != 8; ++i) CHECK(std::fabs(out[i]-expected[i]) < 1e-14);
    CHECK_BITS(out[0],out[6]); CHECK_BITS(out[1],out[7]);
}
int main() { return testkit::run_all(); }



