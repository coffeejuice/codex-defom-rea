#include "state_variable.hpp"

#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {

std::size_t checked_size(StateVariable::Shape shape) {
    // The reference performs signed-int multiplication without these checks.
    // Overflow/negative extents are outside the established compatibility domain.
    if (shape.count < 0 || shape.width < 0 || shape.multiplicity < 0)
        throw std::invalid_argument("negative state-variable extent");
    std::size_t size = static_cast<std::size_t>(shape.count);
    for (const int factor : {shape.width, shape.multiplicity}) {
        if (factor != 0 &&
            size > static_cast<std::size_t>(std::numeric_limits<int>::max()) /
                       static_cast<std::size_t>(factor))
            throw std::length_error("state-variable extent exceeds legacy int range");
        size *= static_cast<std::size_t>(factor);
    }
    return size;
}

} // namespace

StateVariable::StateVariable(int id, Policy policy) : id_(id), policy_(policy) {
    if (id < 0) throw std::invalid_argument("negative state-variable ID");
}

void StateVariable::clear() {
    previous_.count = shape_.count;
    shape_.count = 0;
    std::vector<double>().swap(values_);
}

bool StateVariable::allocate(int count, std::span<const int> components,
                             int selector, std::optional<int> multiplicity) {
    const bool selected = selector == id_ ||
        (selector == -1 && policy_ != Policy::explicit_selection_only);
    if (!selected) return false;
    if (static_cast<std::size_t>(id_) >= components.size())
        throw std::out_of_range("state-variable component index");

    const Shape next{count, components[static_cast<std::size_t>(id_)],
                     multiplicity.value_or(shape_.multiplicity)};
    // The recovered MATAXI allocator never changes the constructor's multiplier.
    if (policy_ == Policy::material_axes && next.multiplicity != 1)
        throw std::invalid_argument("material-axis multiplicity must remain one");
    checked_size(next);
    previous_.count = shape_.count;
    previous_.width = shape_.width;
    if (multiplicity) previous_.multiplicity = shape_.multiplicity;
    shape_ = next;
    const bool changed = resize_storage();
    if (changed && policy_ == Policy::material_axes && shape_.width == 9) {
        for (int row = 0; row < shape_.count; ++row) {
            const auto start = static_cast<std::size_t>(row) * 9;
            values_[start] = values_[start + 4] = values_[start + 8] = 1.0;
        }
    }
    return true;
}

bool StateVariable::resize_storage() {
    const auto size = checked_size(shape_);
    if (values_.size() == size) return false;
    // The reference uses vector-like resize: existing linear elements survive,
    // and added elements are zero. Mesh wrappers call clear() before allocation.
    values_.resize(size, 0.0);
    return true;
}

} // namespace deform::recovered
