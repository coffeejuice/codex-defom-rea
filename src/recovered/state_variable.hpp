#pragma once

#include <cstddef>
#include <optional>
#include <span>
#include <vector>

namespace deform::recovered {

// Internal reconstruction of the observed StateVarBase allocation contract.
// This is not a recovered public ABI or a complete application model.
class StateVariable {
public:
    enum class Policy { ordinary, explicit_selection_only, material_axes };

    struct Shape {
        int count = 0;
        int width = 1;
        int multiplicity = 1;
        bool operator==(const Shape&) const = default;
    };

    explicit StateVariable(int id, Policy policy = Policy::ordinary);

    // Corresponds to 0x140300e30. Component width and multiplicity survive.
    void clear();

    // Corresponds to allocation vtable slot +0x10. An omitted multiplicity
    // leaves it unchanged, as the ordinary node/element implementations do.
    // Return value says whether the descriptor was selected.
    bool allocate(int count, std::span<const int> component_counts, int selector,
                  std::optional<int> multiplicity = {});

    [[nodiscard]] int id() const noexcept { return id_; }
    [[nodiscard]] Shape shape() const noexcept { return shape_; }
    [[nodiscard]] Shape previous_shape() const noexcept { return previous_; }
    [[nodiscard]] std::span<double> values() noexcept { return values_; }
    [[nodiscard]] std::span<const double> values() const noexcept { return values_; }

private:
    // Allocation paths above call 0x140301380 with preserve_rows=false.
    bool resize_storage();
    int id_;
    Policy policy_;
    Shape shape_;
    Shape previous_;
    std::vector<double> values_;
};

} // namespace deform::recovered
