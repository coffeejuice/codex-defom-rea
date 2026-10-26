#pragma once

#include <cstddef>
#include <memory>
#include <string>

#include "m2m/engine/widget_area_owner_state.hpp"

namespace m2m::engine {

enum class WidgetAreaWindowRole {
    SfMoMainWindow, SfMainWindowBase, QMainWindow, QWidget, QObject, QPaintDevice
};

class WidgetAreaPrimaryWindow;

// A source-qualified subobject relationship, not an observed native vtable or
// completed Qt base constructor. No offset is applied to an adapter allocation.
class WidgetAreaWindowAlias final {
public:
    WidgetAreaWindowRole role() const noexcept { return role_; }
    bool admissionAvailable() const noexcept;
    const void* primaryIdentity() const;
    std::size_t byteOffset() const;
    bool sharesPrimaryOwner(const WidgetAreaWindowAlias&) const;
    bool sameSubobject(const WidgetAreaWindowAlias&) const;
private:
    friend class WidgetAreaPrimaryWindow;
    WidgetAreaWindowAlias(const std::shared_ptr<WidgetAreaWindowOwner>&,
        const std::shared_ptr<const void>&, WidgetAreaWindowRole);
    void requireScope() const;
    std::weak_ptr<WidgetAreaWindowOwner> owner_;
    std::weak_ptr<const void> allocation_;
    WidgetAreaWindowRole role_;
};

// Identity-only projection of the selected 10490 -> c98b00 -> QMainWindow ->
// QWidget -> QObject lineage. Binding neither constructs nor registers anything.
class WidgetAreaPrimaryWindow final {
public:
    static std::shared_ptr<WidgetAreaPrimaryWindow> bind(
        const std::shared_ptr<WidgetAreaWindowOwner>&, const std::string& qualification);
    WidgetAreaPrimaryWindow(const WidgetAreaPrimaryWindow&) = delete;
    WidgetAreaPrimaryWindow& operator=(const WidgetAreaPrimaryWindow&) = delete;
    WidgetAreaWindowAlias alias(WidgetAreaWindowRole) const;
    bool admissionAvailable() const noexcept;
    // Actual private blocks, field values, base effects and WidgetState are
    // unavailable. In particular this does not mark +3c8 null or native-alive.
    [[noreturn]] void requireQtBaseEffects() const;
    [[noreturn]] void requireWidgetComponent() const;
private:
    WidgetAreaPrimaryWindow(const std::shared_ptr<WidgetAreaWindowOwner>&,
        const std::shared_ptr<const void>&);
    std::shared_ptr<WidgetAreaWindowOwner> owner_;
    std::weak_ptr<const void> allocation_;
};

} // namespace m2m::engine
