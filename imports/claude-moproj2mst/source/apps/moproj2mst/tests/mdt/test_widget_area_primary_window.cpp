// ctest-labels: m2m-mdt
// Source-qualified identity relationships, not native Qt execution observations.
#include <testkit/check.hpp>
#include <initializer_list>
#include "m2m/engine/widget_area_primary_window.hpp"
#include "m2m/engine/widget_area_lazy.hpp"
#include "m2m/model.hpp"

using namespace m2m;
using namespace m2m::engine;
namespace {
constexpr const char* q = "synthetic primary-address lineage; not completed native construction";
template<class F> void refused(F&& action)
{
    bool caught = false;
    try { action(); } catch (const Error& e) { caught = e.code == M2M_E_UNSUPPORTED; }
    CHECK(caught);
}
struct Fixture {
    std::shared_ptr<const void> allocation = std::make_shared<const int>(1);
    std::shared_ptr<WidgetAreaWindowOwner> owner = WidgetAreaWindowOwner::create(allocation, q);
    std::shared_ptr<WidgetAreaPrimaryWindow> projection = WidgetAreaPrimaryWindow::bind(owner, q);
};
}

TEST_CASE(primary_window_source_roles_share_one_primary_object_without_qt_effects)
{
    Fixture f;
    const auto main = f.projection->alias(WidgetAreaWindowRole::SfMoMainWindow);
    for (const auto role : {WidgetAreaWindowRole::SfMainWindowBase, WidgetAreaWindowRole::QMainWindow,
                           WidgetAreaWindowRole::QWidget, WidgetAreaWindowRole::QObject}) {
        const auto alias = f.projection->alias(role);
        CHECK(alias.role() == role && alias.admissionAvailable());
        CHECK(alias.primaryIdentity() == f.allocation.get());
        CHECK(alias.byteOffset() == 0u && alias.sameSubobject(main));
    }
    CHECK(!f.owner->nativeAlive() && !f.owner->area3c8().known && !f.owner->stack480().known);
    refused([&] { f.projection->requireQtBaseEffects(); });
    refused([&] { f.projection->requireWidgetComponent(); });
}

TEST_CASE(primary_window_paint_device_is_secondary_relationship_not_an_adjusted_adapter_pointer)
{
    Fixture f;
    const auto widget = f.projection->alias(WidgetAreaWindowRole::QWidget);
    const auto paint = f.projection->alias(WidgetAreaWindowRole::QPaintDevice);
    CHECK(paint.primaryIdentity() == widget.primaryIdentity());
    CHECK(paint.sharesPrimaryOwner(widget) && !paint.sameSubobject(widget));
    CHECK(paint.byteOffset() == 0x10u);
    CHECK(!f.owner->nativeAlive() && !f.owner->area3c8().known);
}

TEST_CASE(primary_window_projection_never_initializes_shared_area_cell)
{
    Fixture f;
    const auto cell = WidgetAreaLazyParent::bindWindowOwner(f.owner, q);
    WidgetAreaLazyGetter getter;
    WidgetAreaLazyObservation input; input.parentOwner = f.allocation; input.qualification = q;
    CHECK(getter.resume(cell, input).stage == WidgetAreaLazyStage::FieldRead);
    CHECK(!getter.report().allocationRequested && !cell->fieldKnown());
    (void)f.projection->alias(WidgetAreaWindowRole::QObject);
    CHECK(!cell->fieldKnown() && f.owner->area3c8Field().revision == 0u);
    CHECK(!f.owner->nativeAlive());
    refused([&] { cell->requireImplRegistration(); });
}

TEST_CASE(primary_window_distinct_owner_records_and_colliding_addresses_do_not_alias)
{
    Fixture f;
    const auto first = f.projection->alias(WidgetAreaWindowRole::QObject);
    Fixture other;
    CHECK(!first.sharesPrimaryOwner(other.projection->alias(WidgetAreaWindowRole::QObject)));
    const std::shared_ptr<const void> collision(f.allocation.get(), [](const void*) {});
    const auto otherOwner = WidgetAreaWindowOwner::create(collision, q);
    const auto otherProjection = WidgetAreaPrimaryWindow::bind(otherOwner, q);
    const auto colliding = otherProjection->alias(WidgetAreaWindowRole::QObject);
    CHECK(first.primaryIdentity() == colliding.primaryIdentity());
    CHECK(!first.sharesPrimaryOwner(colliding) && !first.sameSubobject(colliding));
    const auto separateRecord = WidgetAreaWindowOwner::create(f.allocation, q);
    const auto separate = WidgetAreaPrimaryWindow::bind(separateRecord, q);
    CHECK(!first.sharesPrimaryOwner(separate->alias(WidgetAreaWindowRole::QObject)));
}

TEST_CASE(primary_window_rejects_forged_record_empty_qualification_and_unknown_role)
{
    Fixture f;
    const std::shared_ptr<WidgetAreaWindowOwner> forged(f.owner.get(), [](WidgetAreaWindowOwner*) {});
    refused([&] { WidgetAreaPrimaryWindow::bind(forged, q); });
    refused([&] { WidgetAreaPrimaryWindow::bind({}, q); });
    refused([&] { WidgetAreaPrimaryWindow::bind(f.owner, ""); });
    refused([&] { f.projection->alias(static_cast<WidgetAreaWindowRole>(100)); });
    CHECK(!f.owner->nativeAlive() && !f.owner->area3c8().known);
}

TEST_CASE(primary_window_revoked_or_dead_owner_invalidates_escaped_alias_without_field_clear)
{
    Fixture f;
    f.owner->observeInitialArea3c8({}, q);
    const auto alias = f.projection->alias(WidgetAreaWindowRole::QObject);
    f.owner->releaseAdmissionScope();
    CHECK(!alias.admissionAvailable() && !f.projection->admissionAvailable());
    refused([&] { alias.primaryIdentity(); });
    refused([&] { f.projection->alias(WidgetAreaWindowRole::QWidget); });
    CHECK(f.owner->area3c8().known && !f.owner->area3c8().nonNull);
    Fixture dead;
    const auto deadAlias = dead.projection->alias(WidgetAreaWindowRole::QObject);
    dead.owner->observeNativeAlive(false, q);
    CHECK(!deadAlias.admissionAvailable());
    refused([&] { deadAlias.byteOffset(); });
    CHECK(!dead.owner->area3c8().known);
}

TEST_CASE(primary_window_expired_allocation_is_unavailable_and_not_new_native_liveness)
{
    Fixture f;
    const auto alias = f.projection->alias(WidgetAreaWindowRole::QObject);
    f.allocation.reset();
    CHECK(!alias.admissionAvailable() && !f.projection->admissionAvailable());
    refused([&] { alias.byteOffset(); });
    refused([&] { WidgetAreaPrimaryWindow::bind(f.owner, q); });
    CHECK(!f.owner->nativeAlive() && !f.owner->area3c8().known);
}

TEST_CASE(primary_window_alias_lifetime_follows_canonical_owner_not_projection_wrapper)
{
    Fixture f;
    const auto alias = f.projection->alias(WidgetAreaWindowRole::QObject);
    f.projection.reset();
    CHECK(alias.admissionAvailable() && alias.primaryIdentity() == f.allocation.get());
    f.owner.reset();
    CHECK(!alias.admissionAvailable());
    refused([&] { alias.primaryIdentity(); });
}

int main() { return testkit::run_all(); }
