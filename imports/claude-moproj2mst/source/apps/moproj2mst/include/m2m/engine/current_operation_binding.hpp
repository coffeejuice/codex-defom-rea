// Pointer-compatible OpClass current seam backed by nonowning OPI guard metadata.
#pragma once

#include <cstddef>

#include "m2m/engine/operation_info_guard.hpp"

namespace m2m::engine {
class CurrentOperationBinding final {
public:
    CurrentOperationBinding() = default;
    CurrentOperationBinding(const CurrentOperationBinding&) = default;
    CurrentOperationBinding(CurrentOperationBinding&&) noexcept = default;
    CurrentOperationBinding& operator=(const CurrentOperationBinding&) = default;
    CurrentOperationBinding& operator=(CurrentOperationBinding&&) noexcept = default;
    CurrentOperationBinding& operator=(Operation*);
    CurrentOperationBinding& operator=(std::nullptr_t) noexcept;

    Operation* get() const noexcept { return guard_.get(); }
    operator Operation*() const noexcept { return get(); }
    explicit operator bool() const noexcept { return get() != nullptr; }
    Operation* operator->() const noexcept { return get(); }
    Operation& operator*() const noexcept { return *get(); } // same unchecked pointer precondition

    // Save these metadata bindings across callbacks, never a raw pointer that
    // could be destroyed before restoration. Restore is deliberately explicit:
    // this class does not change existing no-restore-on-throw adapter sequences.
    OperationInfoGuard snapshot() const noexcept { return guard_; }
    void restore(const OperationInfoGuard& saved) noexcept { guard_ = saved; }
private:
    OperationInfoGuard guard_;
};
} // namespace m2m::engine
