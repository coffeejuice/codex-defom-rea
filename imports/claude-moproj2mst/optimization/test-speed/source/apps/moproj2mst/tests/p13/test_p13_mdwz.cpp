// ctest-labels: m2m-p13
// ctest-standalone: src/core/basic.cpp src/datafiles/textfile.cpp src/core/longpath.cpp src/datafiles/token.cpp src/ops/distortion_load.cpp src/ops/props_base.cpp src/model/props_rest_slots.cpp src/qt4/number.cpp src/qt4/dtoa.cpp
// Tier-1 test of the Distortion props post-load 0x141365c60 -> 0x1413680e0 (MachDistortion.MDWZ),
// with P06's text file and token reader and local path doubles. The file comes from the overlay.
#include <testkit/check.hpp>

#include <memory>
#include <string>

#include "m2m/data/distortion.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"   // complete OpClass for ~Project
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

using namespace m2m;
using m2m::qt::String;

// ---- doubles -------------------------------------------------------------------------------------
namespace m2m::qt {
String cleanPath(const String& p)
{
    if (p.isNull() || p.isEmpty()) return p;
    std::u16string out;
    for (char16_t c : p.u16()) {
        if (c == u'\\') c = u'/';
        if (c == u'/' && !out.empty() && out.back() == u'/') continue;
        out += c;
    }
    return String(out);
}
String convertSeparators(const String& p)
{
    std::u16string out = p.u16();
    for (auto& c : out)
        if (c == u'/') c = u'\\';
    return p.isNull() ? p : String(out);
}
}  // namespace m2m::qt
namespace m2m {
void Props::postLoad(Project&, Ctx&) {}
String taskDirAbs(const Operation& op)
{
    if (!op.project) return String();
    return qt::convertSeparators(qt::cleanPath(String(u"C:/prj") + qt::kSep + op.taskDir));
}
}  // namespace m2m

namespace {

Operation* g_modelOf = nullptr;   // the operation the 0x14005f770 double was asked for

struct Fx {
    Project prj;
    Ctx ctx;
    Operation op;
    DistortionProps props;

    Fx()
    {
        op.project = &prj;
        op.taskDir = String(u"Operations/Task00004");
        props.owner = &op;
        props.data = std::make_shared<data::DistortionData>();
        // the trailing 0x14005f770(owner): no engine in this build; the double records the call
        g_modelOf = nullptr;
        mst::g_loaderSlots.opModel = [](Project&, Operation& o) -> engine::GuiModel* {
            g_modelOf = &o;
            return nullptr;
        };
    }
    void file(const std::string& bytes)
    {
        prj.overlay[String(u"C:/prj/Operations/Task00004/MachDistortion.MDWZ")] = bytes;
    }
};

}  // namespace

TEST_CASE(mdwz_full)
{
    Fx f;
    f.file("WZD_MACHDIST_WORKPIECE_OBJECT_NO\r\n  * WORKPIECE\r\n2\r\n"
           "WZD_MACHDIST_NUM_FIXTURES\r\n2 4 5\r\n"
           "WZD_MACHDIST_NUM_PASSES\r\n3\r\n6\r\n7\r\n8\r\n"
           "BEGIN_FREE_DIST_BCC_INFO\r\nFIXEDNODE 11\r\nFIXEDCOORD 1.5 2.5 3.5\r\nAUTO 1\r\n"
           "FIXEDZANGLE 0.25D0\r\nEND_FREE_DIST_BCC_INFO\r\n"
           "INIT_VEL_BCC\r\n1\r\nDONE\r\n");
    f.props.data->firstPass = 7;                        // reset by 0x141366710
    f.props.postLoad(f.prj, f.ctx);
    const data::DistortionData& D = *f.props.data;
    CHECK(g_modelOf == &f.op);                          // 0x14005f770(owner) after the read
    CHECK_EQ(D.workpiece, 2);
    CHECK_EQ(D.fixtureCount, 2);
    CHECK_EQ(D.fixtures[0], 4);
    CHECK_EQ(D.fixtures[1], 5);
    CHECK_EQ(D.passCount, 3);
    CHECK_EQ(D.passObjects[0], 6);
    CHECK_EQ(D.passObjects[2], 8);
    CHECK_EQ(D.firstPass, 0);
    CHECK_EQ(D.freeDist.fixedNode, 11);
    CHECK_BITS(D.freeDist.fixedCoord[1], 2.5);
    CHECK_EQ(static_cast<int>(D.freeDist.autoFix), 1);
    CHECK_BITS(D.freeDist.fixedZAngle, 0.25);
    CHECK_EQ(static_cast<int>(D.initVelBcc), 1);
    CHECK(f.ctx.warnings.empty());
}

TEST_CASE(mdwz_end_stops_and_missing_file_resets)
{
    Fx f;
    f.file("WZD_MACHDIST_NUM_PASSES\n1 3\n$END\nWZD_MACHDIST_WORKPIECE_OBJECT_NO\n9\nDONE\n");
    f.props.postLoad(f.prj, f.ctx);
    CHECK_EQ(f.props.data->passCount, 1);
    CHECK_EQ(f.props.data->passObjects[0], 3);
    CHECK_EQ(f.props.data->workpiece, 1);               // after $END: not read

    Fx g;                                               // no file: defaults, no warning
    g.props.data->passCount = 4;
    g.props.data->workpiece = 3;
    g.props.postLoad(g.prj, g.ctx);
    CHECK_EQ(g.props.data->passCount, 0);
    CHECK_EQ(g.props.data->workpiece, 1);
    CHECK(g.ctx.warnings.empty());
}

TEST_CASE(mdwz_without_owner_does_nothing)
{
    Fx f;
    f.props.owner = nullptr;
    f.props.data->passCount = 4;
    f.props.postLoad(f.prj, f.ctx);
    CHECK_EQ(f.props.data->passCount, 4);
}

int main()
{
    return testkit::run_all();
}
