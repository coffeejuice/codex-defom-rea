#include "cogging_coupled_refs.hpp"

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <defmodel/layout/llobj_data.hpp>

namespace m2m::engine::detail {
namespace {

void applyReference(def::model::LLOBJ_DATA* object, bool coupled, std::int32_t referenceId)
{
    if (!object || !coupled) return;

    auto* bytes = reinterpret_cast<std::byte*>(object);
    constexpr std::size_t kReferenceIdOffset = 0x22c0;
    constexpr std::size_t kReferenceValueOffset = 0x22c8;
    constexpr double kReferenceValue = 1.0;
    std::memcpy(bytes + kReferenceIdOffset, &referenceId, sizeof referenceId);
    std::memcpy(bytes + kReferenceValueOffset, &kReferenceValue, sizeof kReferenceValue);
}

}  // namespace

void applyCoupledObject5Reference(def::model::LLOBJ_DATA* object, bool coupled)
{
    applyReference(object, coupled, 4);
}

void applyCoupledObject7Reference(def::model::LLOBJ_DATA* object, bool coupled)
{
    applyReference(object, coupled, 6);
}

}  // namespace m2m::engine::detail
