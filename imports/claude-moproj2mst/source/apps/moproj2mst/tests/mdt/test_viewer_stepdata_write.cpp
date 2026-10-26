// ctest-labels: m2m-mdt mdt-viewer doe
#include <testkit/check.hpp>
#include <cstring>
#include <string>
#include <defmodel/layout/llobj_data.hpp>
#include "m2m/common.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_view_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"

using namespace m2m;
namespace {
xml::NodePtr parse(const std::string& text)
{
    xml::Document document; qt::String error; int line=0,column=0;
    REQUIRE(xml::setContent(document,text,&error,&line,&column));
    return document.root;
}
engine::LLOBJ_DATA* bound(engine::GuiModel& model,int number)
{
    auto* object=model.objectWrap(number,true,false); REQUIRE(object);
    engine::viewerSync(model,object,true); return object;
}
bool refuses(const auto& action,const char* fragment)
{
    try { action(); }
    catch(const Error& error) { return error.code==M2M_E_UNSUPPORTED && std::string(error.what()).find(fragment)!=std::string::npos; }
    return false;
}
}

TEST_CASE(viewer_writer_uses_actual_settings_in_view_order_without_mutating_them)
{
    engine::GuiModel model(nullptr,2);
    bound(model,1); bound(model,2);
    engine::readViewerDisplayOptions(model,0,*parse("<SfDispOpt><DispModeType>9</DispModeType>"
        "<DispFlagList MeshOn='1' GeoOn='1' TranspOn='1'/></SfDispOpt>"));
    engine::readViewerDisplayOptions(model,1,*parse("<SfDispOpt><DispModeType>-7</DispModeType></SfDispOpt>"));
    const auto before=engine::viewerDisplayOptionSnapshot(model,0); REQUIRE(before);
    auto node=parse("<StepData/>");
    engine::writeViewerStepDataObjects(model,*node);
    const auto objects=node->childElements(); REQUIRE(objects.size()==2);
    for(int index=0;index<2;++index) {
        CHECK(objects[index]->attribute(qt::L("No"))==qt::number(index));
        const auto options=objects[index]->childElements(qt::L("SfDispOpt")); REQUIRE(options.size()==1);
        const auto mode=options[0]->childElements(qt::L("DispModeType")); REQUIRE(mode.size()==1);
        CHECK(mode[0]->text()==qt::number(index==0?9:-7));
        CHECK(objects[index]->childElements(qt::L("SfSymmetryInfo")).empty());
    }
    const auto after=engine::viewerDisplayOptionSnapshot(model,0); REQUIRE(after);
    CHECK(after->flags==before->flags && after->mode==before->mode);
    CHECK(!model.doeStepData && engine::viewerItemCount(model)==2);
}

TEST_CASE(viewer_writer_uses_actual_object_kind5_for_mesh_and_geometry_sentinels)
{
    engine::GuiModel model(nullptr,2);
    auto* object=bound(model,1);
    const std::int32_t kind=5;
    std::memcpy(reinterpret_cast<char*>(object)+0x0c,&kind,sizeof kind);
    auto node=parse("<StepData/>"); engine::writeViewerStepDataObjects(model,*node);
    const auto options=node->children[0]->childElements(qt::L("SfDispOpt")); REQUIRE(options.size()==1);
    const auto flags=options[0]->childElements(qt::L("DispFlagList")); REQUIRE(flags.size()==1);
    CHECK(flags[0]->attribute(qt::L("MeshOn"))==qt::L("-1"));
    CHECK(flags[0]->attribute(qt::L("GeoOn"))==qt::L("-1"));
}

TEST_CASE(viewer_writer_populated_slice_refuses_before_appending_current_object)
{
    engine::GuiModel model(nullptr,2);
    float minimum[3]{},maximum[3]{};
    CHECK(engine::viewerBox(model,2,minimum,maximum));
    doe::SliceConstructorRuntime runtime;
    doe::SliceCoefficients coefficients; coefficients.values={1.,0.,0.,-1.};
    engine::readViewerSlice(model,1,coefficients,&runtime);
    // Explicit unsupported-domain fixture; no graphics backend is fabricated.
    auto* slice=const_cast<doe::SliceCollectionState*>(engine::viewerSliceState(model,1));
    REQUIRE(slice); slice->graphicCount=1;
    engine::viewerStepDataState(model,1).symmetryCount=1;
    auto node=parse("<StepData/>");
    CHECK(refuses([&]{engine::writeViewerStepDataObjects(model,*node);},"Slice graphic payload"));
    REQUIRE(node->children.size()==1);
    CHECK(node->children[0]->attribute(qt::L("No"))==qt::L("0"));
    CHECK(!engine::viewerStepDataState(model,1).symmetry[0].matrix);
    CHECK(slice->graphicCount==1 && engine::viewerItemCount(model)==2);
}

int main() { return testkit::run_all(); }
