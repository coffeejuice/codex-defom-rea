// P16: objItem (0x140060030): the object manager's first row with the id, object items only.
#include <testkit/check.hpp>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"   // Project owns OpClass objects

using namespace m2m;

TEST_CASE(obj_item_lookup)
{
    Project prj;
    Operation op;
    CHECK(objItem(op, 1) == nullptr);              // no project
    op.project = &prj;
    for (int id : {3, 5, 3}) {
        auto it = std::make_unique<Item>();
        it->kind = ItemKind::Object;
        it->id = id;
        it->objectType = id;
        prj.objects.rows.push_back(std::move(it));
    }
    prj.objects.rows[2]->objectType = 99;          // second row with id 3
    REQUIRE(objItem(op, 3) != nullptr);
    CHECK_EQ(objItem(op, 3)->objectType, 3);       // first match
    CHECK(objItem(op, 5) == prj.objects.rows[1].get());
    CHECK(objItem(op, 4) == nullptr);
    auto m = std::make_unique<Item>();
    m->kind = ItemKind::Material;                  // not an SfMoObjItem: the cast gives null
    m->id = 7;
    prj.objects.rows.push_back(std::move(m));
    CHECK(objItem(op, 7) == nullptr);
}

int main()
{
    return testkit::run_all();
}
