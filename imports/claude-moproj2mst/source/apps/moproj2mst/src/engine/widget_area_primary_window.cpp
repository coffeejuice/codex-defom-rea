#include "m2m/engine/widget_area_primary_window.hpp"
#include "m2m/model.hpp"

namespace m2m::engine {
namespace {
[[noreturn]] void refuse(const char* issue) { throw Error(M2M_E_UNSUPPORTED, issue); }
template<class T> bool sameLease(const std::shared_ptr<T>& a, const std::shared_ptr<T>& b)
{ return a && b && a.get() == b.get() && !a.owner_before(b) && !b.owner_before(a); }
bool available(const std::shared_ptr<WidgetAreaWindowOwner>& owner,
               const std::shared_ptr<const void>& allocation) noexcept
{
    return owner && owner->matchesCanonicalLease(owner) && owner->observationScopeAvailable() &&
        owner->nativeAlive() != false && sameLease(allocation, owner->allocationLease());
}
std::size_t offset(WidgetAreaWindowRole role)
{
    switch (role) {
    case WidgetAreaWindowRole::SfMoMainWindow:
    case WidgetAreaWindowRole::SfMainWindowBase:
    case WidgetAreaWindowRole::QMainWindow:
    case WidgetAreaWindowRole::QWidget:
    case WidgetAreaWindowRole::QObject: return 0;
    case WidgetAreaWindowRole::QPaintDevice: return 0x10;
    }
    refuse("Primary window: unknown native subobject role");
}
} // namespace

WidgetAreaWindowAlias::WidgetAreaWindowAlias(const std::shared_ptr<WidgetAreaWindowOwner>& owner,
    const std::shared_ptr<const void>& allocation, WidgetAreaWindowRole role)
    : owner_(owner), allocation_(allocation), role_(role) {}
bool WidgetAreaWindowAlias::admissionAvailable() const noexcept
{ return available(owner_.lock(), allocation_.lock()); }
void WidgetAreaWindowAlias::requireScope() const
{
    if (!admissionAvailable()) refuse("Primary window alias: actual canonical owner/lifetime unavailable");
}
const void* WidgetAreaWindowAlias::primaryIdentity() const
{ requireScope(); return allocation_.lock().get(); }
std::size_t WidgetAreaWindowAlias::byteOffset() const
{ requireScope(); return offset(role_); }
bool WidgetAreaWindowAlias::sharesPrimaryOwner(const WidgetAreaWindowAlias& other) const
{
    requireScope(); other.requireScope();
    return sameLease(owner_.lock(), other.owner_.lock()) &&
        sameLease(allocation_.lock(), other.allocation_.lock());
}
bool WidgetAreaWindowAlias::sameSubobject(const WidgetAreaWindowAlias& other) const
{ return sharesPrimaryOwner(other) && byteOffset() == other.byteOffset(); }

WidgetAreaPrimaryWindow::WidgetAreaPrimaryWindow(const std::shared_ptr<WidgetAreaWindowOwner>& owner,
    const std::shared_ptr<const void>& allocation) : owner_(owner), allocation_(allocation) {}
std::shared_ptr<WidgetAreaPrimaryWindow> WidgetAreaPrimaryWindow::bind(
    const std::shared_ptr<WidgetAreaWindowOwner>& owner, const std::string& qualification)
{
    const auto allocation = owner ? owner->allocationLease() : std::shared_ptr<const void>{};
    if (qualification.empty() || !available(owner, allocation))
        refuse("Primary window: actual canonical window allocation and qualification required");
    return std::shared_ptr<WidgetAreaPrimaryWindow>(new WidgetAreaPrimaryWindow(owner, allocation));
}
bool WidgetAreaPrimaryWindow::admissionAvailable() const noexcept
{ return available(owner_, allocation_.lock()); }
WidgetAreaWindowAlias WidgetAreaPrimaryWindow::alias(WidgetAreaWindowRole role) const
{
    if (!admissionAvailable()) refuse("Primary window: owner admission scope unavailable");
    (void)offset(role); // Reject invalid roles before creating an alias.
    return WidgetAreaWindowAlias(owner_, allocation_.lock(), role);
}
void WidgetAreaPrimaryWindow::requireQtBaseEffects() const
{
    refuse("Primary window: actual QMainWindow/QWidget/QObject private construction and Qt effects remain guarded");
}
void WidgetAreaPrimaryWindow::requireWidgetComponent() const
{
    refuse("Primary window: actual WidgetState component construction/binding remains guarded");
}

} // namespace m2m::engine
