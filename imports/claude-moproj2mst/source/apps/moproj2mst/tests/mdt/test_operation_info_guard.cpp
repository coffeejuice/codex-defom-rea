// ctest-labels: m2m-mdt operation-info-guard
#include <testkit/check.hpp>

#include <functional>
#include <memory>
#include <string>
#include <type_traits>

#include "m2m/common.hpp"
#include "m2m/model.hpp"
#include "m2m/engine/operation_info_guard.hpp"
#include "m2m/props.hpp"

using namespace m2m;
using engine::OperationInfoExpiry;
using engine::OperationInfoGuard;
using engine::OperationInfoIdentity;
namespace {
constexpr const char* fixtureBoundary =
    "isolated declared fixture: OPI cleanup completed; 050531/6712f8bd boundary; no native acceptance";

template<class F> bool refuses(F&& body)
{
    try { body(); }
    catch (const Error& error) {
        return error.code == M2M_E_UNSUPPORTED &&
            std::string(error.what()).find("source qualification") != std::string::npos;
    }
    return false;
}

struct CleanupProbe final : Props {
    explicit CleanupProbe(std::function<void()> callback) : callback_(std::move(callback)) {}
    ~CleanupProbe() override { callback_(); }
private:
    std::function<void()> callback_;
};
}

static_assert(!std::is_copy_constructible_v<OperationInfoIdentity>);
static_assert(!std::is_move_constructible_v<OperationInfoIdentity>);
static_assert(std::is_copy_constructible_v<OperationInfoGuard>);
static_assert(std::is_move_constructible_v<OperationInfoGuard>);

TEST_CASE(operation_info_unbound_and_same_identity_binding_preserve_current)
{
    Operation operation;
    OperationInfoIdentity identity(operation);
    OperationInfoGuard current;
    CHECK(!current);
    CHECK(current.expiry() == OperationInfoExpiry::Unbound);
    CHECK(current.boundaryQualification().empty());
    current.reset(&identity);
    CHECK(current.get() == &operation);
    CHECK(identity.get() == &operation);
    current.reset(&identity);
    CHECK(current.get() == &operation);
    CHECK(current.expiry() == OperationInfoExpiry::Live);
    current.reset();
    CHECK(!current);
    CHECK(current.expiry() == OperationInfoExpiry::Unbound);
    CHECK(identity.get() == &operation);
}

TEST_CASE(operation_info_aliases_expire_together_and_unrelated_identity_remains_live)
{
    Operation first, second;
    OperationInfoIdentity firstIdentity(first), secondIdentity(second);
    auto firstGuard = firstIdentity.guard();
    auto alias = firstGuard;
    auto other = secondIdentity.guard();
    CHECK(firstIdentity.commitQObjectBoundary(fixtureBoundary));
    CHECK(!firstGuard);
    CHECK(!alias);
    CHECK(firstGuard.expiry() == OperationInfoExpiry::QObjectBoundary);
    CHECK(alias.boundaryQualification() == fixtureBoundary);
    CHECK(other.get() == &second);
    CHECK(secondIdentity.expiry() == OperationInfoExpiry::Live);
    // A newly bound guard to an expired identity cannot resurrect its pointer.
    OperationInfoGuard late(firstIdentity);
    CHECK(!late);
    CHECK(late.expiry() == OperationInfoExpiry::QObjectBoundary);
}

TEST_CASE(operation_info_rebinding_one_alias_is_not_invalidated_by_old_identity)
{
    Operation first, second;
    OperationInfoIdentity firstIdentity(first), secondIdentity(second);
    auto current = firstIdentity.guard();
    auto oldAlias = current;
    current.reset(&secondIdentity);
    CHECK(firstIdentity.commitQObjectBoundary(fixtureBoundary));
    CHECK(!oldAlias);
    CHECK(current.get() == &second);
    CHECK(current.expiry() == OperationInfoExpiry::Live);
    CHECK(secondIdentity.commitQObjectBoundary(fixtureBoundary));
    CHECK(!current);
}

TEST_CASE(operation_info_qualified_boundary_is_terminal_and_missing_evidence_does_not_mutate)
{
    Operation operation;
    OperationInfoIdentity identity(operation);
    auto current = identity.guard();
    CHECK(refuses([&] { identity.commitQObjectBoundary(""); }));
    CHECK(refuses([&] { identity.commitQObjectBoundary(" \t\r\n"); }));
    CHECK(current.get() == &operation);
    CHECK(current.boundaryQualification().empty());
    CHECK(identity.commitQObjectBoundary(fixtureBoundary));
    CHECK(!identity.commitQObjectBoundary("different later receipt"));
    CHECK(current.expiry() == OperationInfoExpiry::QObjectBoundary);
    CHECK(current.boundaryQualification() == fixtureBoundary);
}

TEST_CASE(operation_info_fixture_cleanup_sees_current_then_qobject_boundary_expires_before_observer)
{
    auto outerRow = std::make_unique<Operation>();
    OperationInfoIdentity identity(*outerRow);
    auto current = identity.guard();
    bool cleanupSawCurrent = false;
    outerRow->props = std::make_shared<CleanupProbe>([&] {
        cleanupSawCurrent = current.get() == outerRow.get();
    });
    // Controlled fixture cleanup, not a replacement for native 050690 payloads.
    outerRow->props.reset();
    CHECK(cleanupSawCurrent);
    CHECK(current.get() == outerRow.get());
    CHECK(identity.commitQObjectBoundary(fixtureBoundary));
    const bool destroyedObserverSawNull = current.get() == nullptr;
    CHECK(destroyedObserverSawNull);
    CHECK(outerRow != nullptr); // OPI boundary precedes outer row destruction
    outerRow.reset();
    CHECK(current.expiry() == OperationInfoExpiry::QObjectBoundary);
}

TEST_CASE(operation_info_adapter_identity_lifetime_is_distinct_and_does_not_own_operation)
{
    auto operation = std::make_unique<Operation>();
    OperationInfoGuard escaped;
    {
        OperationInfoIdentity identity(*operation);
        escaped = identity.guard();
        CHECK(escaped.get() == operation.get());
    }
    CHECK(escaped.expiry() == OperationInfoExpiry::AdapterIdentityLifetime);
    CHECK(!escaped);
    CHECK(escaped.boundaryQualification().empty());
    bool destroyed = false;
    operation->props = std::make_shared<CleanupProbe>([&] { destroyed = true; });
    operation.reset();
    CHECK(destroyed); // metadata guard has not retained the target allocation
}

TEST_CASE(operation_info_committed_receipt_survives_identity_scope_without_becoming_adapter_expiry)
{
    Operation operation;
    OperationInfoGuard escaped;
    {
        OperationInfoIdentity identity(operation);
        escaped = identity.guard();
        CHECK(identity.commitQObjectBoundary(fixtureBoundary));
    }
    CHECK(!escaped);
    CHECK(escaped.expiry() == OperationInfoExpiry::QObjectBoundary);
    CHECK(escaped.boundaryQualification() == fixtureBoundary);
    escaped.reset();
    CHECK(escaped.expiry() == OperationInfoExpiry::Unbound);
}

int main() { return testkit::run_all(); }
