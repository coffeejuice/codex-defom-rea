#include "geometry_storage.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>

// Each recorded scalar SSE operation rounds separately. Do not contract the
// products/subtractions or reassociate the left-to-right sums below.
#if defined(_MSC_VER)
#pragma float_control(precise, on, push)
#pragma fp_contract(off)
#elif defined(__clang__)
#pragma clang fp contract(off)
#elif defined(__GNUC__)
#pragma GCC optimize ("fp-contract=off")
#endif

namespace deform::recovered {
namespace {
using Point = std::array<double, 3>;

double finite(double value) {
    if (!std::isfinite(value))
        throw std::domain_error("GeometryStorage: nonfinite geometry or arithmetic result");
    return value;
}

double squared_length(const Point& point) {
    const double x = finite(point[0] * point[0]);
    const double y = finite(point[1] * point[1]);
    const double xy = finite(x + y);
    const double z = finite(point[2] * point[2]);
    return finite(xy + z);
}

Point normal(const Point& a, const Point& b, const Point& c) {
    Point u{}, v{};
    for (std::size_t axis = 0; axis != 3; ++axis)
        u[axis] = finite(c[axis] - b[axis]);
    for (std::size_t axis = 0; axis != 3; ++axis)
        v[axis] = finite(a[axis] - b[axis]);
    Point n{
        finite(finite(u[1] * v[2]) - finite(u[2] * v[1])),
        finite(finite(u[2] * v[0]) - finite(u[0] * v[2])),
        finite(finite(u[0] * v[1]) - finite(u[1] * v[0]))};
    const double length = finite(std::sqrt(squared_length(n)));
    if (length != 0.0)
        for (double& component : n) component = finite(component / length);
    return n;
}

double normal_distance(const Point& a, const Point& b) {
    Point difference{};
    for (std::size_t axis = 0; axis != 3; ++axis)
        difference[axis] = finite(a[axis] - b[axis]);
    return squared_length(difference);
}

double plane_distance(const Point& point, const GeometryStorage::Plane& plane) {
    const double x = finite(plane.normal[0] * finite(point[0] - plane.origin[0]));
    const double y = finite(plane.normal[1] * finite(point[1] - plane.origin[1]));
    const double xy = finite(x + y);
    const double z = finite(plane.normal[2] * finite(point[2] - plane.origin[2]));
    return std::abs(finite(xy + z));
}

std::int32_t first_unused_id(std::span<const GeometryStorage::Plane> planes) {
    std::int32_t id = 1;
    while (std::any_of(planes.begin(), planes.end(), [id](const auto& p) { return p.id == id; })) {
        if (id == std::numeric_limits<std::int32_t>::max())
            throw std::overflow_error("GeometryStorage: plane identifier exhausted");
        ++id;
    }
    return id;
}
} // namespace

void GeometryStorage::begin_replace(int coordinate_count, int face_count) {
    // Native count*3 and count*4 arithmetic is signed32. Restrict to extents
    // whose source arithmetic is defined; do not silently emulate overflow.
    if (coordinate_count > std::numeric_limits<std::int32_t>::max() / 3 ||
        face_count > std::numeric_limits<std::int32_t>::max() / 4)
        throw std::length_error("GeometryStorage: count exceeds recovered index range");
    if (coordinate_count > 0)
        coordinates_.resize(static_cast<std::size_t>(coordinate_count) * 3, 0.0);
    if (face_count > 0)
        connectivity_.resize(static_cast<std::size_t>(face_count) * 4, 0);
    if (!connectivity_.empty()) {
        classifications_.assign(connectivity_.size() / 4, 0);
        auxiliary68_.resize(connectivity_.size() / 4, 0);
    }
}

void GeometryStorage::write_coordinate(std::size_t row, std::span<const double, 3> values) {
    if (row >= coordinates_.size() / 3)
        throw std::out_of_range("GeometryStorage: coordinate row");
    for (double value : values) finite(value);
    std::copy(values.begin(), values.end(), coordinates_.begin() + row * 3);
}

void GeometryStorage::write_face(std::size_t row, std::span<const std::int32_t, 4> values) {
    if (row >= connectivity_.size() / 4)
        throw std::out_of_range("GeometryStorage: face row");
    std::copy(values.begin(), values.end(), connectivity_.begin() + row * 4);
}

int GeometryStorage::coordinate_count() const noexcept {
    return static_cast<int>(coordinates_.size() / 3);
}
int GeometryStorage::face_count() const noexcept {
    return static_cast<int>(connectivity_.size() / 4);
}

double GeometryStorage::bounding_diagonal() const {
    if (coordinates_.empty() || connectivity_.empty()) return 0.0;
    Point minimum{1e30, 1e30, 1e30}, maximum{-1e30, -1e30, -1e30};
    for (std::size_t row = 0; row != coordinates_.size() / 3; ++row) {
        for (std::size_t axis = 0; axis != 3; ++axis) {
            const double value = finite(coordinates_[row * 3 + axis]);
            // Keep the original <= tie behavior, including signed zero.
            if (value <= minimum[axis]) minimum[axis] = value;
            if (maximum[axis] <= value) maximum[axis] = value;
        }
    }
    double total = 0.0;
    for (std::size_t axis = 0; axis != 3; ++axis) {
        const double extent = finite(maximum[axis] - minimum[axis]);
        total = finite(total + finite(extent * extent));
    }
    return finite(std::sqrt(total));
}

void GeometryStorage::finalize_planes(std::span<const Plane> copied_planes) {
    // Copy before clearing also makes a span into our own vector safe. Native
    // aliased list-copy behavior is not claimed by this owned-storage API.
    std::vector<Plane> seed(copied_planes.begin(), copied_planes.end());
    for (const auto& plane : seed) {
        for (double value : plane.origin) finite(value);
        for (double value : plane.normal) finite(value);
    }
    planes_.clear();
    planes_ = std::move(seed);
    for (double value : coordinates_) finite(value);
    // Validate all eligible indices before dereferencing any coordinate row.
    for (std::size_t face = 0; face != connectivity_.size() / 4; ++face) {
        if (connectivity_[face * 4 + 3] >= 1) continue;
        for (std::size_t slot = 0; slot != 3; ++slot) {
            const auto id = connectivity_[face * 4 + slot];
            if (id < 1 || id > coordinate_count())
                throw std::out_of_range("GeometryStorage: eligible face coordinate index");
        }
    }
    if (classifications_.size() != connectivity_.size() / 4)
        throw std::logic_error("GeometryStorage: classification extent");
    const double scale = bounding_diagonal();
    const double tolerance = finite(0.001 * scale);
    const auto coordinate = [this](std::int32_t id) {
        const auto offset = static_cast<std::size_t>(id - 1) * 3;
        return Point{coordinates_[offset], coordinates_[offset + 1], coordinates_[offset + 2]};
    };
    for (std::size_t face = 0; face != connectivity_.size() / 4; ++face) {
        if (connectivity_[face * 4 + 3] >= 1) continue;
        const Point a = coordinate(connectivity_[face * 4]);
        const Point b = coordinate(connectivity_[face * 4 + 1]);
        const Point c = coordinate(connectivity_[face * 4 + 2]);
        const Point n = normal(a, b, c);
        if (finite(std::sqrt(squared_length(n))) < 1e-6) continue;
        auto plane = std::find_if(planes_.begin(), planes_.end(), [&](const Plane& candidate) {
            return normal_distance(n, candidate.normal) < 0.01 &&
                   plane_distance(a, candidate) < tolerance;
        });
        if (plane == planes_.end()) {
            const auto id = first_unused_id(planes_);
            planes_.push_back(Plane{id, a, n});
            plane = planes_.end() - 1;
        }
        if (plane->id > std::numeric_limits<std::int32_t>::max() - 600)
            throw std::overflow_error("GeometryStorage: classification identifier overflow");
        classifications_[face] = plane->id + 600;
    }
}

void GeometryStorage::reset() noexcept {
    coordinates_.clear();
    connectivity_.clear();
    classifications_.clear();
    auxiliary68_.clear();
}

void GeometryStorage::deactivate() noexcept {
    reset();
    planes_.clear();
}

} // namespace deform::recovered

#if defined(_MSC_VER)
#pragma float_control(pop)
#endif
