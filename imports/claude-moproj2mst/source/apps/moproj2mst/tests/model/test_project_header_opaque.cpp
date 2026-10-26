// ctest-labels: m2m-model feature-context ownership
// Project's public shell must support pointer ownership without private definitions.
#include <testkit/check.hpp>
#include "m2m/model.hpp"
#include <memory>
#include <type_traits>
#include <utility>

namespace {
template<class T, class = void> struct Complete : std::false_type {};
template<class T> struct Complete<T, std::void_t<decltype(sizeof(T))>> : std::true_type {};
using Project = m2m::Project;
using Anchor = Project::FeatureContextAnchor;
using Report = Project::FeatureContextAttemptReport;

static_assert(!Complete<Anchor>::value);
static_assert(!Complete<Report>::value);
static_assert(std::is_same_v<std::underlying_type_t<Project::FeatureContextStage>, int>);
static_assert(std::is_same_v<std::underlying_type_t<Project::FeatureContextStop>, int>);
static_assert(std::is_same_v<decltype(std::declval<const Project&>().featureContextAnchor()),
                             std::shared_ptr<const Anchor>>);
static_assert(noexcept(std::declval<const Project&>().featureContextAnchor()));
static_assert(std::is_destructible_v<Project>);
static_assert(!std::is_copy_constructible_v<Project> && !std::is_move_constructible_v<Project>);
}

TEST_CASE(project_header_opaque_anchor_ownership_survives_project_scope)
{
    std::shared_ptr<const Anchor> retained;
    std::weak_ptr<const Anchor> observer;
    {
        Project project;
        retained = project.featureContextAnchor();
        REQUIRE(retained);
        const auto same = project.featureContextAnchor();
        CHECK(same == retained);
        CHECK(!same.owner_before(retained) && !retained.owner_before(same));
        observer = retained;
    }
    // Escaped metadata is independently owned; no private observer or native
    // lifetime/completion claim is needed to release its opaque shared pointer.
    CHECK(!observer.expired());
    retained.reset();
    CHECK(observer.expired());
}

int main() { return testkit::run_all(); }
