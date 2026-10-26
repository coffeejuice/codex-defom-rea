// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <memory>
#include <optional>

#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "../../src/doe/application_state_projection.hpp"

using namespace m2m;

namespace {
bool refuses(const auto& action)
{
    try { action(); }
    catch (const Error& error) { return error.code == M2M_E_UNSUPPORTED; }
    return false;
}
}

TEST_CASE(application_state_projection_outlives_sequential_sessions_without_inventing_dblist_gate)
{
    auto application = std::make_shared<doe::ApplicationStateProjection>();
    CHECK(!application->processType().isKnown());
    CHECK(!application->dbListAdmission().isKnown());

    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session{driver};
        const auto manager = session.modelManagerOwner();
        REQUIRE(manager != nullptr);
        CHECK(manager->ownerSession() == &session);

        // Native mode-0 refresh with no current Project selects process type 5.
        application->observeMode0ProcessTypeRefresh(false, std::nullopt);
        CHECK(application->processType().isKnown());
        CHECK(application->processType().value == 5u);
        CHECK(!application->dbListAdmission().isKnown());
    }

    {
        engine::DiagramEffectDriver driver;
        engine::ConversionSession session{driver};
        const auto manager = session.modelManagerOwner();
        REQUIRE(manager != nullptr);
        CHECK(manager->ownerSession() == &session);

        // The app-scope state persists across Session lifetimes and is refreshed
        // only from the explicit native mode-0 Project/open-success observation.
        CHECK(application->processType().isKnown());
        CHECK(application->processType().value == 5u);
        application->observeMode0ProcessTypeRefresh(true, false);
        CHECK(application->processType().value == 1u);

        // No known caller/reset exists for DBLIST global +0x1c. A new Session,
        // Project, or process-type refresh cannot turn that fact into zero.
        CHECK(!application->dbListAdmission().isKnown());
        CHECK(refuses([&] { application->observeMode0ProcessTypeRefresh(true, std::nullopt); }));
        CHECK(application->processType().value == 1u);
        CHECK(!application->dbListAdmission().isKnown());
    }
}

TEST_CASE(application_state_projection_process_type_refresh_covers_qualified_open_outcomes)
{
    doe::ApplicationStateProjection application;
    application.observeMode0ProcessTypeRefresh(true, true);
    CHECK(application.processType().isKnown() && application.processType().value == 5u);
    application.observeMode0ProcessTypeRefresh(true, false);
    CHECK(application.processType().value == 1u);
    application.observeMode0ProcessTypeRefresh(false, std::nullopt);
    CHECK(application.processType().value == 5u);
    CHECK(!application.dbListAdmission().isKnown());
}

int main() { return testkit::run_all(); }
