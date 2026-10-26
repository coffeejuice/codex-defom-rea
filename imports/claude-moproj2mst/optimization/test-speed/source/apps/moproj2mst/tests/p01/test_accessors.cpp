// P01: accessors moved from P02 (0x14005a400, 0x14005fc10, 0x14005fb80, 0x140061310, 0x14005ff30).
// They call P02's objSlots/objId/objEntry and P03's activeSubProject; until those are merged a
// case prints "PENDING <address>" and does not fail.
#include <testkit/check.hpp>

#include <cstdio>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"   // Project owns OpClass instances

using m2m::qt::String;

namespace {

template <class F>
void pending(const char* name, F f)
{
    try {
        f();
    } catch (const m2m::NotPorted& e) {
        std::printf("PENDING %s: %s\n", name, e.what());
    }
}

}  // namespace

TEST_CASE(recovery_list_a)
{
    m2m::Operation lone;
    CHECK(m2m::recoveryListA(lone) == nullptr);       // no project
    CHECK(m2m::Project().recoveryA != nullptr);       // the data root's list exists and is empty
    CHECK(m2m::Project().recoveryA->empty());
    pending("recovery_list_a", [] {
        m2m::Project p;
        auto main = std::make_unique<m2m::SubProject>();
        main->project = &p;
        p.subs.push_back(std::move(main));
        auto ds = std::make_unique<m2m::SubProject>();
        ds->project = &p;
        ds->type = 4;
        p.subs.push_back(std::move(ds));
        m2m::Operation op;
        op.project = &p;
        CHECK(m2m::recoveryListA(op) == p.recoveryA.get());
        p.activeSub = 1;                              // die-stress sub-project: its own data root
        std::vector<m2m::RecoveryEntry>* a = m2m::recoveryListA(op);
        CHECK(a != nullptr);
        CHECK(a != p.recoveryA.get());
        CHECK(a == p.subs[1]->ownRootRecoveryA.get());
        CHECK(m2m::recoveryListA(op) == a);           // created once
    });
}

TEST_CASE(slots_and_names)
{
    pending("slots_and_names", [] {
        m2m::Operation op;
        op.objIds = {5, -1, -2, 7, 5};
        CHECK_EQ(m2m::slotNumber(op, 0), 1);
        CHECK_EQ(m2m::slotNumber(op, 4), 5);
        CHECK_EQ(m2m::slotNumber(op, 5), 0);
        CHECK_EQ(m2m::slotNumber(op, -1), 0);
        CHECK_EQ(m2m::slotOfObject(op, 5), 0);       // first match
        CHECK_EQ(m2m::slotOfObject(op, -2), 2);
        CHECK_EQ(m2m::slotOfObject(op, -1), -1);     // padding is not a valid id
        CHECK_EQ(m2m::slotOfObject(op, 9), -1);
        CHECK(m2m::objEntryById(op, 9) == nullptr);
        m2m::ObjEntry* e = m2m::objEntryById(op, 7);
        REQUIRE(e != nullptr);
        e->name = String(u"Top Die");
        CHECK(m2m::objDisplayName(op, 7) == String(u"Top Die"));
        CHECK(m2m::objDisplayName(op, -2) == String(u"Read From DB"));
        CHECK(m2m::objDisplayName(op, 9).isNull());
    });
}

int main() { return testkit::run_all(); }
