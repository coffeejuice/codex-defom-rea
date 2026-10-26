// P16: SfOprReport / SfOprShapeOptim write nothing and consume no simulation number (s1-05 1).
#include <testkit/check.hpp>

#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"

using namespace m2m;

TEST_CASE(report_and_shapeoptim_are_empty)
{
    for (int which = 0; which < 2; ++which) {
        auto cls = which == 0 ? mst::makeReport() : mst::makeShapeOptim();
        CHECK(cls->key() == qt::L(which == 0 ? "SfOprReport" : "SfOprShapeOptim"));
        Ctx ctx;
        mst::State st;
        mst::Stream ts(1252);
        mst::W w{ts, st, ctx};
        Project prj;
        Operation op;
        op.project = &prj;
        int sim = 4;
        cls->current = &op;
        cls->writeMst(w, prj, qt::L("D.DB"), &sim, &op, 0, 0);
        CHECK_EQ(sim, 4);
        CHECK(ts.bytes().empty());
        std::vector<mst::SimEntry> out;
        int counter = 2;
        cls->simList(1, &counter, &op, out);
        CHECK(out.empty());
        CHECK_EQ(counter, 2);
    }
}

int main()
{
    return testkit::run_all();
}
