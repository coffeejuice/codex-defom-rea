#include "m2m/engine/current_operation_binding.hpp"

#include "m2m/model.hpp"

namespace m2m::engine {
CurrentOperationBinding& CurrentOperationBinding::operator=(Operation* operation)
{
    guard_.reset(operation ? &operation->operationInfoIdentity() : nullptr);
    return *this;
}

CurrentOperationBinding& CurrentOperationBinding::operator=(std::nullptr_t) noexcept
{
    guard_.reset();
    return *this;
}
} // namespace m2m::engine
