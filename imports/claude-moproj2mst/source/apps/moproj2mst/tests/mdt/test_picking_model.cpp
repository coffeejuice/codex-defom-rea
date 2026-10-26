// ctest-labels: m2m-mdt lifecycle-failure
#include <testkit/check.hpp>
#include <string>
#include "m2m/common.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/picking_owner.hpp"

using namespace m2m;

TEST_CASE(picking_prefix_is_retained_on_actual_model_without_publishing_incomplete_owner)
{
    engine::GuiModel model(nullptr,3);
    CHECK(!model.pendingPickingConstruction());
    auto& pending=model.beginPickingConstructionPrefix();
    CHECK(model.pendingPickingConstruction()==&pending);
    CHECK(pending.guardedModel()==&model && pending.boundNativeModel()==&model);
    CHECK(pending.construction().stage==engine::PickingConstructorStage::AwaitingGraphicsRegistration);
    REQUIRE(pending.registrationRequest().has_value());
    CHECK(pending.registrationRequest()->owner==&pending);
    CHECK(pending.selectionBuffer().knownByteCount()==0x1000);
    CHECK(!model.viewer && !model.display && !model.auxiliaryDrawings);
}

TEST_CASE(picking_prefix_repeated_call_keeps_existing_partial_effects)
{
    engine::GuiModel model(nullptr,3);
    auto& pending=model.beginPickingConstructionPrefix();
    const auto effects=pending.construction().effects;
    bool refused=false;
    try { model.beginPickingConstructionPrefix(); }
    catch(const Error& error) {
        refused=error.code==M2M_E_UNSUPPORTED && std::string(error.what()).find("already pending")!=std::string::npos;
    }
    CHECK(refused && model.pendingPickingConstruction()==&pending);
    CHECK(pending.construction().effects==effects);
}

TEST_CASE(picking_prefix_uses_distinct_actual_models_and_buffers)
{
    engine::GuiModel first(nullptr,3),second(nullptr,3);
    auto& a=first.beginPickingConstructionPrefix();
    auto& b=second.beginPickingConstructionPrefix();
    CHECK(&a!=&b && &a.selectionBuffer()!=&b.selectionBuffer());
    CHECK(a.guardedModel()==&first && b.guardedModel()==&second);
    CHECK(!a.identityMatches(second) && !b.identityMatches(first));
}

int main() { return testkit::run_all(); }
