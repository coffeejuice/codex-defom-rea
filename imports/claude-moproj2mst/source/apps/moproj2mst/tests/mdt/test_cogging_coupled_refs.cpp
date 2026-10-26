#include <testkit/check.hpp>

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <defmodel/layout/llobj_data.hpp>

#include "../../src/engine/cogging_coupled_refs.hpp"

namespace {

std::int32_t referenceId(const def::model::LLOBJ_DATA& object)
{
    std::int32_t value = 0;
    const auto* bytes = reinterpret_cast<const std::byte*>(&object);
    std::memcpy(&value, bytes + 0x22c0, sizeof value);
    return value;
}

double referenceValue(const def::model::LLOBJ_DATA& object)
{
    double value = 0.0;
    const auto* bytes = reinterpret_cast<const std::byte*>(&object);
    std::memcpy(&value, bytes + 0x22c8, sizeof value);
    return value;
}

void setSentinel(def::model::LLOBJ_DATA& object)
{
    constexpr std::int32_t id = 0x12345678;
    constexpr double value = 37.25;
    auto* bytes = reinterpret_cast<std::byte*>(&object);
    std::memcpy(bytes + 0x22c0, &id, sizeof id);
    std::memcpy(bytes + 0x22c8, &value, sizeof value);
}

}  // namespace

TEST_CASE(cogging_coupled_reference_effects_are_explicit_and_object_specific)
{
    using m2m::engine::detail::applyCoupledObject5Reference;
    using m2m::engine::detail::applyCoupledObject7Reference;

    def::model::LLOBJ_DATA object5(nullptr);
    def::model::LLOBJ_DATA object7(nullptr);

    setSentinel(object5);
    applyCoupledObject5Reference(&object5, false);
    CHECK_EQ(referenceId(object5), 0x12345678);
    CHECK_EQ(referenceValue(object5), 37.25);
    applyCoupledObject5Reference(&object5, true);
    CHECK_EQ(referenceId(object5), 4);
    CHECK_EQ(referenceValue(object5), 1.0);

    setSentinel(object7);
    applyCoupledObject7Reference(&object7, false);
    CHECK_EQ(referenceId(object7), 0x12345678);
    CHECK_EQ(referenceValue(object7), 37.25);
    applyCoupledObject7Reference(&object7, true);
    CHECK_EQ(referenceId(object7), 6);
    CHECK_EQ(referenceValue(object7), 1.0);

    applyCoupledObject5Reference(nullptr, true);
    applyCoupledObject7Reference(nullptr, true);
}

int main() { return testkit::run_all(); }
