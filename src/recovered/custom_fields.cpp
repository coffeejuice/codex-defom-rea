#include "custom_fields.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

namespace deform::recovered {
namespace {

std::size_t checked_extent(int count, int width) {
    if (count < 0 || width < 0)
        throw std::invalid_argument("negative custom-array extent is outside the recovered domain");
    if (width != 0 && count > std::numeric_limits<int>::max() / width)
        throw std::length_error("custom-array extent exceeds native signed32 range");
    return static_cast<std::size_t>(count) * static_cast<std::size_t>(width);
}

} // namespace

void CustomVariable::configure(int kind, int width, int declared_rows, int nodes, int elements) {
    kind_ = kind;
    if (kind == 0 || kind == 1 || kind == 2) {
        previous_.count = shape_.count;
        shape_.count = kind == 0 ? declared_rows : kind == 1 ? nodes : elements;
    }
    previous_.width = shape_.width;
    shape_.width = width;
    // 301380(mode0): preserve the linear prefix; zero newly added elements.
    // Multiplicity remains the constructor value1 in this import path.
    values_.resize(checked_extent(shape_.count, shape_.width), 0.0);
}

void CustomVariable::fill(double value) {
    std::fill(values_.begin(), values_.end(), value);
}

CustomVariable& CustomFields::find_or_create(std::int32_t id) {
    for (const auto& record : records_)
        if (record->custom_id() == id) return *record;
    auto& record = records_.emplace_back(std::make_unique<CustomVariable>(id));
    return *record;
}

void DynamicArray::configure(int selector, int count, int width) {
    mode_ = 1;
    selector_ = selector;
    count_ = count;
    width_ = width;
    if (selector == 1 || selector == 2 || selector == 3) {
        integers_selected_ = selector == 2;
        discriminator_ = selector;
    }
    const auto size = checked_extent(count, width);
    if (integers_selected_) integer_values_.resize(size, 0);
    else real_values_.resize(size, 0.0);
}

void DynamicArray::fill_real(double value) {
    if (integers_selected_)
        throw std::logic_error("real default fill requires a selected real dynamic array");
    std::fill(real_values_.begin(), real_values_.end(), value);
}

DynamicArray* DynamicArrays::select(int position) {
    if (position > 0) {
        while (records_.size() < static_cast<std::size_t>(position))
            records_.push_back(std::make_unique<DynamicArray>());
        return records_[static_cast<std::size_t>(position) - 1].get();
    }
    return records_.empty() ? nullptr : records_.front().get();
}

} // namespace deform::recovered
