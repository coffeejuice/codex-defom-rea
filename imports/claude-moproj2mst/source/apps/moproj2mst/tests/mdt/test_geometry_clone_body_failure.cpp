// CP159: actual CowVector body32 allocation failure ordering; host-only test.
#include <testkit/check.hpp>
#include <defmodel/sdctl/cow.hpp>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

namespace {
using Values = def::model::CowVector<double>;
using Body = def::model::CowVectorBody<double>;
static_assert(sizeof(Values) == 8);
static_assert(sizeof(Body) == 0x20);

struct Probe {
    bool armed = false;
    std::size_t calls = 0;
    std::size_t request = 0;
    std::int32_t refsSeen = -1;
    std::size_t deletes = 0;
    Body* old = nullptr;
};
Probe probe;

// Runs before either handle is destroyed. Repair only the expected unchanged
// shared-body fixture, so an unexpected publication cannot overcount it.
struct RestoreFixtureRefs {
    Values& first;
    Values& second;
    Body* old;
    ~RestoreFixtureRefs()
    {
        probe.armed = false;
        if (first.body() == old && second.body() == old) old->refs = 2;
    }
};
}

void* operator new(std::size_t bytes)
{
    if (probe.armed) {
        ++probe.calls;
        probe.request = bytes;
        probe.refsSeen = probe.old ? probe.old->refs : -1;
        throw std::bad_alloc();
    }
    if (void* p = std::malloc(bytes ? bytes : 1)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept
{
    if (probe.armed) ++probe.deletes;
    std::free(p);
}
void operator delete(void* p, std::size_t) noexcept { ::operator delete(p); }

TEST_CASE(actual_cowvector_detach_body32_allocation_throw_keeps_decremented_old_body)
{
    Values first;
    Values second(first);
    Body* const old = const_cast<Body*>(first.body());
    const auto* const oldFirst = old->first;
    const auto* const oldLast = old->last;
    const auto* const oldEnd = old->end;
    REQUIRE(first.body() == old && second.body() == old);
    REQUIRE(old->refs == 2);
    REQUIRE(oldFirst == nullptr && oldLast == nullptr && oldEnd == nullptr);
    RestoreFixtureRefs restore{first, second, old};

    probe.calls = 0;
    probe.request = 0;
    probe.refsSeen = -1;
    probe.deletes = 0;
    probe.old = old;
    probe.armed = true;
    bool caught = false;
    try {
        first.detach(); // Actual translated detach; hook throws on its body new.
    } catch (const std::bad_alloc&) {
        probe.armed = false;
        caught = true;
    } catch (...) {
        probe.armed = false;
        throw;
    }
    probe.armed = false;

    CHECK(caught);
    CHECK(probe.calls == 1u);
    CHECK(probe.request == 32u);
    CHECK(probe.refsSeen == 1);
    CHECK(probe.deletes == 0u);
    CHECK(first.body() == old && second.body() == old);
    CHECK(old->refs == 1);
    CHECK(old->first == oldFirst && old->last == oldLast && old->end == oldEnd);
}

int main() { return testkit::run_all(); }
