#pragma once

#include <array>

namespace deform::recovered {
// bb3490/bb3000 finite binary64 path; returns cosine,sine in that order.
// Callers retain their own 1e-12 cleanup and early-return ordering.
[[nodiscard]] std::array<double, 2> legacy_motion_cos_sin(double radians);
}
