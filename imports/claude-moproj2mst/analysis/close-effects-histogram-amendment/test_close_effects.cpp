// Isolated typed receiver tests; these do not claim actual Close integration.
// ctest-labels: m2m-mdt; isolated-close-effects
#include <testkit/check.hpp>

#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "m2m/common.hpp"
#include "m2m/doe_statevar.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/close_effects.hpp"

using namespace m2m;

namespace {
struct Fixture {
    std::shared_ptr<engine::DbHandle> owner = std::make_shared<engine::DbHandle>();
    engine::GuiModel model{owner.get(), 3};
    Fixture() {
        model.doeStepData = std::make_shared<doe::StepDataState>();
        model.doeStepData->stateVar = std::make_shared<doe::StateVarState>();
    }
    doe::StateVarState& state() { return *model.doeStepData->stateVar; }
};

template<class F> void expectUnsupported(F&& body, const char* detail)
{
    bool refused = false;
    try { body(); }
    catch (const Error& error) {
        refused = error.code == M2M_E_UNSUPPORTED &&
                  std::string(error.what()).find(detail) != std::string::npos;
    }
    CHECK(refused);
}

engine::PtrkGraphRegistrations known(
    std::vector<std::weak_ptr<engine::PtrkGraphManagerState>> receivers = {})
{
    return {"isolated adapter fixture; not an original receiver-population observation",
            std::move(receivers)};
}
}

TEST_CASE(close_effects_isolated_missing_population_is_not_observed_empty)
{
    Fixture f;
    engine::CloseEffects effects;
    f.state().plotApplyInProgress = true;
    f.state().plotVar = 0;
    f.state().plotCompNo = 0;
    f.state().type = 17; // later render effects have not happened
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "population");
    CHECK(f.state().plotApplyInProgress);
    CHECK_EQ(f.state().type, 17);
    effects.setPtrkGraphRegistrations(engine::PtrkGraphRegistrations{});
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "population");
    effects.setPtrkGraphRegistrations(known());
    effects.changePtrkGraph(f.model, f.state());
    CHECK_EQ(f.state().type, 17);
    effects.setPtrkGraphRegistrations(std::nullopt);
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "population");
}

TEST_CASE(close_effects_isolated_off_receiver_uses_its_live_owned_flag)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    f.state().plotVar = 101; // off branch returns before reading this field
    f.state().plotCompNo = 2;
    effects.changePtrkGraph(f.model, f.state());
    CHECK(!receiver->graphOn);
    CHECK_EQ(f.state().plotVar, 101);
    CHECK_EQ(f.state().plotCompNo, 2);
    receiver->graphOn = true; // explicit fixture state; no automatic fresh inference
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "31fab0");
    receiver->graphOn = false;
    effects.changePtrkGraph(f.model, f.state());
}

TEST_CASE(close_effects_isolated_variable_zero_does_not_skip_enabled_graph_prefix)
{
    Fixture f;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    receiver->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({receiver}));
    f.state().plotVar = 0;
    f.state().plotCompNo = 0;
    f.state().plotApplyInProgress = true;
    f.state().minimum = 23;
    f.state().maximum = 47;
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "31fab0");
    CHECK(receiver->graphOn);
    CHECK(f.state().plotApplyInProgress);
    CHECK_EQ(f.state().minimum, 23.0);
    CHECK_EQ(f.state().maximum, 47.0);
}

TEST_CASE(close_effects_isolated_expired_receiver_skips_local_delivery_only)
{
    Fixture f;
    engine::CloseEffects effects;
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    effects.setPtrkGraphRegistrations(known({receiver}));
    receiver.reset();
    effects.changePtrkGraph(f.model, f.state());
    REQUIRE(effects.ptrkGraphRegistrations().has_value());
    CHECK_EQ(effects.ptrkGraphRegistrations()->receivers.size(), std::size_t{1});
    CHECK(effects.ptrkGraphRegistrations()->receivers.front().expired());
    effects.changePtrkGraph(f.model, f.state()); // persistent registration unchanged
}

TEST_CASE(close_effects_isolated_multiple_receivers_preserve_reached_guard)
{
    Fixture f;
    auto off = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    auto on = std::make_shared<engine::PtrkGraphManagerState>(f.owner);
    on->graphOn = true;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known({off, on, off}));
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "31fab0");
    CHECK(!off->graphOn);
    CHECK(on->graphOn);
    CHECK_EQ(effects.ptrkGraphRegistrations()->receivers.size(), std::size_t{3});
}

TEST_CASE(close_effects_isolated_rejects_detached_state_and_lost_owner)
{
    Fixture f;
    engine::CloseEffects effects;
    effects.setPtrkGraphRegistrations(known());
    doe::StateVarState detached;
    expectUnsupported([&] { effects.changePtrkGraph(f.model, detached); }, "actual GuiModel");

    // A separately retained manager cannot silently outlive its QObject parent.
    auto otherOwner = std::make_shared<engine::DbHandle>();
    auto receiver = std::make_shared<engine::PtrkGraphManagerState>(otherOwner);
    effects.setPtrkGraphRegistrations(known({receiver}));
    otherOwner.reset();
    expectUnsupported([&] { effects.changePtrkGraph(f.model, f.state()); }, "data-root owner");
}


TEST_CASE(close_effects_isolated_second_hook_defaults_to_guard)
{
    struct FirstOnly : engine::StateVarPlotEffects {
        void changePtrkGraph(engine::GuiModel&, doe::StateVarState&) override {}
    } firstOnly;
    Fixture f;
    expectUnsupported([&] { firstOnly.changeHistoGraph(f.model, f.state()); }, "ChangeHistoGraph");
}

TEST_CASE(close_effects_isolated_histogram_requires_actual_qualified_runtime)
{
    Fixture f;
    engine::CloseEffects effects;
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "coverage");
    effects.setHistogramRegistrations(engine::HistogramRegistrations{});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "coverage");
    auto io = doe::makeRecordingStudyIo();
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, ""});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "coverage");
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated observed-empty fixture"});
    effects.changeHistoGraph(f.model, f.state());
    doe::StateVarState detached;
    expectUnsupported([&] { effects.changeHistoGraph(f.model, detached); }, "actual GuiModel");
}

TEST_CASE(close_effects_isolated_histogram_reads_actual_registry_and_live_flags)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated live-registry fixture"});
    f.state().histogram = std::make_shared<doe::HistogramState>();
    // Added AFTER binding; dispatch must not use a cloned registration population.
    io->stateVarRuntime().histogramRegistrations.push_back(
        {"signalChangeHistoGraph", "slotChangeHistoGraph", f.state().histogram});
    auto& histogram = *f.state().histogram;
    histogram.binWidth = 27;
    effects.changeHistoGraph(f.model, f.state());
    histogram.graphOn = true;
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "+0xb4");
    CHECK(histogram.graphOn);
    CHECK_EQ(histogram.binWidth, 27);
    histogram.graphOn = false;
    effects.changeHistoGraph(f.model, f.state());
}

TEST_CASE(close_effects_isolated_histogram_global_receivers_are_not_just_current_helper)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated global-registry fixture"});
    f.state().histogram = std::make_shared<doe::HistogramState>();
    auto other = std::make_shared<doe::HistogramState>();
    other->graphOn = true;
    auto& records = io->stateVarRuntime().histogramRegistrations;
    records.push_back({"signalHistoGraphOff", "slotHistoGraphOff", other});
    records.push_back({"signalChangeHistoGraph", "slotChangeHistoGraph", f.state().histogram});
    effects.changeHistoGraph(f.model, f.state()); // unrelated key is ignored
    records.push_back({"signalChangeHistoGraph", "slotChangeHistoGraph", other});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "+0xb4");
    CHECK(!f.state().histogram->graphOn);
    CHECK(other->graphOn);
}

TEST_CASE(close_effects_isolated_histogram_expiry_preserves_actual_registry)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated lifetime fixture"});
    auto receiver = std::make_shared<doe::HistogramState>();
    auto& records = io->stateVarRuntime().histogramRegistrations;
    records.push_back({"signalChangeHistoGraph", "unrecoveredLiveSlot", receiver});
    expectUnsupported([&] { effects.changeHistoGraph(f.model, f.state()); }, "unsupported slot");
    receiver.reset();
    effects.changeHistoGraph(f.model, f.state()); // null receiver skips slot invocation
    CHECK_EQ(records.size(), std::size_t{1});
    CHECK(records.front().receiver.expired());
}

TEST_CASE(close_effects_isolated_histogram_lease_retains_same_runtime)
{
    Fixture f;
    auto io = doe::makeRecordingStudyIo();
    std::weak_ptr<doe::StudyIo> observed = io;
    auto* runtime = &io->stateVarRuntime();
    engine::CloseEffects effects;
    effects.setHistogramRegistrations(engine::HistogramRegistrations{io, "isolated session lease fixture"});
    io.reset();
    CHECK(!observed.expired());
    CHECK(&effects.histogramRegistrations()->owner->stateVarRuntime() == runtime);
    effects.changeHistoGraph(f.model, f.state());
    effects.setHistogramRegistrations(std::nullopt);
    CHECK(observed.expired());
}

int main() { return testkit::run_all(); }
