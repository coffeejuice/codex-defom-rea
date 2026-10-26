#include "m2m/doe_output_data.hpp"

#include "m2m/common.hpp"
#include "m2m/doe_session.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine_scene.hpp"

namespace m2m::doe {
void finishCurrentOutputClassData(Project& project, Operation& operation, const OutputClassRefs& refs)
{
    auto* sub = activeSubProject(project);
    if (!sub || sub->type != 2 || !sub->ownedDoeStudy || sub->doe != sub->ownedDoeStudy.get())
        throw Error(M2M_E_UNSUPPORTED, "DOE output data needs the owned Type2 study");
    auto& study = *sub->ownedDoeStudy;
    auto document = study.outputDocument;
    if (!document || !document->currentPart || *document->currentPart >= document->parts.size())
        throw Error(M2M_E_UNSUPPORTED, "DOE output data needs finalized current-part selection");
    auto& part = document->parts[*document->currentPart];
    SessionRecord* selected = nullptr;
    for (auto* record : part.sessions) { //6483e0: first matching type in selected part
        if (!record)
            throw Error(M2M_E_UNSUPPORTED, "DOE output original null Session pointer dereference remains unsupported");
        if (record->taskType == qt::L("SfTaskDoe")) { selected = record; break; }
    }
    if (!selected) {
        if (!project.doeIo)
            throw Error(M2M_E_UNSUPPORTED, "DOE output missing SfTaskDoe needs actual I/O and notification owner");
        selected = &constructDefaultDoeSession(project, *document, *project.doeIo);
    }
    if (!selected->task) return; //e2ec40 explicitly checks6584f0 before the body
    const auto& lists = selected->task->lists;
    if (!lists.objects.empty()) {
        study.objNo = lists.objects.front(); //91e0d0
        refs.data.objectNo = lists.objects.front();
        refs.data.meshPath = study.commonMeshPath;
        refs.data.roiType = study.roiType;
    }
    if (!lists.operations.empty()) study.oprNo = lists.operations.front().no; //3ed890/91e090
    auto* model = engine::opModel(project, operation); //05f770
    if (!model)
        throw Error(M2M_E_UNSUPPORTED, "DOE output original null model display dereference remains unsupported");
    auto* target = engine::displayRoiModel(model); //c7ae00 ->d0d670, or model itself
    if (!target) return;
    target->reset(); //c7b720, before region and constraint processing
    refs.data.regionNames = std::vector<qt::String>{}; //0aced0
    if (study.regions.empty()) {
        if (study.roiType == 1) {
            study.commonMeshPath = qt::L("Regions/Common_Mesh.Key");
            study.commonMeshOrigName = qt::L("");
        } else {
            study.roiType = 0; //917130, not normalization of the earlier class copy
            study.commonMeshPath = qt::L("");
            study.commonMeshOrigName = qt::L("");
        }
    } else {
        //The first known name is appended before the first unrecovered region
        //geometry/file payload. Do not append later names out of source order.
        refs.data.regionNames->push_back(study.regions.front());
        throw Error(M2M_E_UNSUPPORTED, "DOE output region geometry payload remains unsupported");
    }
    for (int index = 0; index < 4; ++index) refs.flags98[index] = 0; //98..9b only
    refs.listA0.clear();
    refs.listA8.clear();
    if (study.constraints > 0)
        throw Error(M2M_E_UNSUPPORTED, "DOE output constraint payload remains unsupported");
    //The source for-loop also has no iterations for a nonpositive count.
}
} // namespace m2m::doe
