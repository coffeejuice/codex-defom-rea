#pragma once

#include "state_variable.hpp"

#include <cstdint>
#include <memory>
#include <span>
#include <vector>

namespace deform::recovered {

// 186010/32ebd0: descriptor ID78 is distinct from the exact signed custom ID
// at +0x38. This models owned storage; native external/borrowed buffers are
// outside the established profile.
class CustomVariable {
public:
    explicit CustomVariable(std::int32_t custom_id) : custom_id_(custom_id) {}

    // 185d20 sets kind, conditionally sets count, then sets width and resizes.
    // Kinds other than0/1/2 retain the current and previous count fields.
    void configure(int kind, int width, int declared_rows, int nodes, int elements);
    // 301620 fills the physical allocation, not the declared reader row count.
    void fill(double value);

    [[nodiscard]] std::int32_t custom_id() const noexcept { return custom_id_; }
    [[nodiscard]] int kind() const noexcept { return kind_; }
    [[nodiscard]] StateVariable::Shape shape() const noexcept { return shape_; }
    [[nodiscard]] StateVariable::Shape previous_shape() const noexcept { return previous_; }
    [[nodiscard]] std::span<double> values() noexcept { return values_; }
    [[nodiscard]] std::span<const double> values() const noexcept { return values_; }

private:
    const std::int32_t custom_id_;
    int kind_ = 0;
    StateVariable::Shape shape_;
    StateVariable::Shape previous_;
    std::vector<double> values_;
};

class CustomFields {
public:
    // 185e00/185ed0/186010: exact ID lookup; missing IDs append one record.
    // No placeholder records and no positive-ID requirement are imposed.
    [[nodiscard]] CustomVariable& find_or_create(std::int32_t id);
    [[nodiscard]] std::span<const std::unique_ptr<CustomVariable>> records() const noexcept {
        return records_;
    }

private:
    std::vector<std::unique_ptr<CustomVariable>> records_;
};

// DYNARY_DATA is itself the linked payload, not a separately allocated wrapper.
// The selected import path owns both typed buffers; switching type keeps the
// inactive buffer and resizes only the newly selected one.
class DynamicArray {
public:
    // 25b220: the caller normalizes its mode argument to1 for every import.
    // Selectors outside1/2/3 preserve the prior byte/discriminator pair.
    void configure(int selector, int count, int width);
    void fill_real(double value);

    [[nodiscard]] int mode() const noexcept { return mode_; }
    [[nodiscard]] int selector() const noexcept { return selector_; }
    [[nodiscard]] bool integers_selected() const noexcept { return integers_selected_; }
    [[nodiscard]] int discriminator() const noexcept { return discriminator_; }
    [[nodiscard]] int count() const noexcept { return count_; }
    [[nodiscard]] int width() const noexcept { return width_; }
    [[nodiscard]] std::span<double> real_values() noexcept { return real_values_; }
    [[nodiscard]] std::span<const double> real_values() const noexcept { return real_values_; }
    [[nodiscard]] std::span<std::int32_t> integer_values() noexcept { return integer_values_; }
    [[nodiscard]] std::span<const std::int32_t> integer_values() const noexcept { return integer_values_; }

private:
    int mode_ = 0;
    int selector_ = 0;
    bool integers_selected_ = false;
    int discriminator_ = 0;
    int count_ = 0;
    int width_ = 0;
    std::vector<double> real_values_;
    std::vector<std::int32_t> integer_values_;
};

class DynamicArrays {
public:
    // 25a990/25a9e0: a positive position creates all preceding default nodes.
    // A nonpositive position returns the first existing node, or null.
    [[nodiscard]] DynamicArray* select(int position);
    [[nodiscard]] std::span<const std::unique_ptr<DynamicArray>> records() const noexcept {
        return records_;
    }

private:
    std::vector<std::unique_ptr<DynamicArray>> records_;
};

} // namespace deform::recovered
