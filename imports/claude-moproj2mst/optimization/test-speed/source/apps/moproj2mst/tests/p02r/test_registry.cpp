// P02 rescue: class registry 0x14006dba0 and opBase 0x140054e90.
#include <testkit/check.hpp>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"

using m2m::qt::String;

TEST_CASE(registry_keys)
{
    m2m::Project p;
    m2m::makeRegistry(p);
    const char16_t* keys[] = {
        u"SfOprForming", u"SfOprForming2D", u"SfOprHeatTransfer", u"SfOprHeatTransfer2D",
        u"SfOprMultiBlow3D", u"SfOprMultiBlow2D", u"SfOprFormingExpress", u"SfOprFormingExpress2D",
        u"SfOprDistortion3D", u"SfOprDistortion2D", u"SfOprHeatTransferExpress", u"SfOprHeatTransferExpress2D",
        u"SfOprHTFurnace", u"SfOprHeatTreat", u"SfOprHeatTreat2D", u"SfOprCogging", u"SfOprPre3D", u"SfOprPre2D",
        u"SfOprReport", u"SfOprShapeOptim", u"SfOprDoeOutput", u"SfOprConvert2Dto3D", u"SfOprBoolean2D",
        u"SfOprBoolean3D", u"SfOprCycle", u"SfOprDiestress", u"SfOprDiestressBM2D", u"SfOprDiestressBM3D"};
    CHECK_EQ(p.registry.size(), std::size(keys));
    for (const char16_t* k : keys) {
        auto it = p.registry.find(String(k));
        CHECK(it != p.registry.end());
        if (it != p.registry.end()) CHECK(it->second->key() == String(k));
    }
    CHECK(p.registry.find(String(u"SfOprGroup")) == p.registry.end());
    CHECK(p.registry.find(String(u"SfOprCopyMirror")) == p.registry.end());
    CHECK(p.registry.find(String(u"SfOprMultiBlow")) == p.registry.end());   // legacy: mapped by the factory
}

TEST_CASE(op_base_lookup)
{
    m2m::Project p;
    m2m::makeRegistry(p);
    m2m::Operation op;
    op.classKey = String(u"SfOprCogging");
    CHECK(m2m::opBase(op) == nullptr);                // no project
    op.project = &p;
    CHECK(m2m::opBase(op) == p.registry[String(u"SfOprCogging")].get());
    op.classKey = String(u"sfoprcogging");            // QMap lookup is case-sensitive
    CHECK(m2m::opBase(op) == nullptr);
    const size_t n = p.registry.size();
    op.classKey = String(u"SfOprNothing");
    CHECK(m2m::opBase(op) == nullptr);
    CHECK_EQ(p.registry.size(), n);                   // a miss does not insert
}

int main() { return testkit::run_all(); }
