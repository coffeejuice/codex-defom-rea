// P03: operation list of a sub-project's flow chain (0x1400b94f0, called by DoeOutput's dim
// 0x140e329d0). It reads opBase (0x140054e90) and flowNumber (0x140050b20) from P02; until P02
// merges their stubs throw NotPorted and the cases report SKIP.
#include <testkit/check.hpp>

#include <cstdio>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;
using m2m::qt::L;

namespace {

struct Fixture {
    Project p;
    Fixture()
    {
        p.subs.push_back(makeSubProject(p, 0, 0));
        for (const char* k : {"SfOprForming", "SfOprReport", "SfOprDoeOutput", "SfOprShapeOptim"})
            p.registry[L(k)] = std::make_unique<mst::OpClass>(L(k));
    }
    Operation* op(int id, const char* key)
    {
        auto o = std::make_unique<Operation>();
        o->id = id;
        o->project = &p;
        o->classKey = L(key);
        Operation* r = o.get();
        p.operations.push_back(std::move(o));
        return r;
    }
    FlowItem* item(int id, int col)
    {
        auto it = std::make_unique<FlowItem>();
        it->opId = id;
        it->col = col;
        FlowItem* r = it.get();
        p.subs[0]->flow.items.push_back(std::move(it));
        return r;
    }
};

void link(FlowItem* a, FlowItem* b)
{
    a->next = b;
    b->prev = a;
}

}  // namespace

TEST_CASE(flow_operations_chain_order_without_report_types)
{
    Fixture f;
    Operation* a = f.op(1, "SfOprForming");
    f.op(2, "SfOprReport");
    Operation* c = f.op(3, "SfOprForming");
    f.op(4, "SfOprDoeOutput");
    FlowItem* i1 = f.item(1, 0);
    FlowItem* i2 = f.item(2, 1);
    FlowItem* i3 = f.item(3, 2);
    FlowItem* i4 = f.item(4, 3);
    link(i1, i2);
    link(i2, i3);
    link(i3, i4);
    f.item(9, 9);                                 // no operation with id 9: skipped
    try {
        const std::vector<Operation*> v = flowOperations(*f.p.subs[0]);
        // numbers 1 and 3: op 3 (number 3) appends at index 1 since the list is shorter
        REQUIRE(v.size() == 2u);
        CHECK(v[0] == a);
        CHECK(v[1] == c);
    } catch (const NotPorted& e) {
        std::printf("SKIP: %s\n", e.what());
    }
}

TEST_CASE(flow_operations_without_project_is_empty)
{
    SubProject s;
    CHECK(flowOperations(s).empty());
}

int main()
{
    return testkit::run_all();
}
