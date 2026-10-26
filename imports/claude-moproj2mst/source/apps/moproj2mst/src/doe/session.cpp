#include "m2m/doe_session.hpp"

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_stepdata.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/engine_scene.hpp"
#include "m2m/p17.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::doe {
namespace {
using qt::L;

void freshDom(SessionDocument& session)
{
    session.dom = {};
    session.dom.doctypeName = L("SessionFile");              // 0x1424df610
    session.partRoot = xml::createElement(L("PartRoot"));   // 0x1424df668
    session.dom.root = session.partRoot;
}
}  // namespace

void bindSessionImportOwner(SessionImportContext& context, Project& project)
{
    context.actualSession = nullptr;
    if (context.io != project.doeIo.get())
        throw Error(M2M_E_UNSUPPORTED,
                    "DOE SessionImportContext I/O is not the Project's bound StudyIo");
    if (!project.doeIo) return;

    engine::ModelManager* const manager = project.doeIo->modelManager();
    if (!manager) return; // No actual manager was bound by this conversion profile.

    engine::ConversionSession* const session = manager->ownerSession();
    if (!session || session->liveProject() != &project ||
        session->modelManagerOwner().get() != manager)
        throw Error(M2M_E_UNSUPPORTED,
                    "DOE StudyIo manager is not owned by this live ConversionSession/Project");
    context.actualSession = session; // borrowed; ConversionSession remains caller-owned.
}

SessionDocument::SessionDocument() { freshDom(*this); }

void resetSessionDocument(SessionDocument& session)
{
    session.filename = String();
    if (session.dom.root && session.dom.root->tag != L("PartRoot"))
        throw Error(M2M_E_UNSUPPORTED, "DOE session reset with a non-PartRoot document root");
    // 647030 removes only direct document children matching PartRoot; it does not
    // replace the QDomDocument or its document type here.
    session.partRoot = xml::createElement(L("PartRoot"));
    session.dom.root = session.partRoot;
}

void readSessionDocument(SessionDocument& session, const String& path, StudyIo& io)
{
    session.filename = path;
    auto file = io.openReadFile(path);
    if (!file) return;
    const auto size = file->size();
    if (!size)
        throw Error(M2M_E_UNSUPPORTED, "DOE session open-file size is not available");
    if (*size == 0) return;
    freshDom(session);                                     // 0x14064b950 before setContent
    const std::string bytes = file->readAll();              // after destructive replacement
    if (bytes.find('\r') != std::string::npos)
        throw Error(M2M_E_UNSUPPORTED, "DOE session non-Text CR decoding is not recovered");
    xml::Document parsed;
    String error;
    int line = 0, column = 0;
    if (!xml::setContent(parsed, bytes, &error, &line, &column))
        throw Error(M2M_E_UNSUPPORTED, "DOE session malformed XML partial DOM is not recovered");
    session.dom = std::move(parsed);
    session.partRoot = session.dom.root;                    // Type2 flag+29=false
}

void prepareOutputSession(Study& study, const String& path, StudyIo& io)
{
    if (!study.outputDocument) study.outputDocument = std::make_shared<SessionDocument>();
    resetSessionDocument(*study.outputDocument);
    readSessionDocument(*study.outputDocument, path, io);
}

void prepareCurrentOutputSession(Project& project, Operation&)
{
    SubProject* sub = activeSubProject(project);
    if (!sub || sub->type != 2 || !sub->ownedDoeStudy ||
        sub->doe != sub->ownedDoeStudy.get() || !project.doeIo)
        throw Error(M2M_E_UNSUPPORTED, "DOE session needs owned Type2 study and configured I/O");
    Study& study = *sub->ownedDoeStudy;
    const String path = mst::doeControlDir(*sub, &study.name) + qt::kSep +
                        projectBaseName(project) + L(".DS");
    prepareOutputSession(study, path, *project.doeIo);
    SessionImportContext importContext{ [&project] {
        engine::displayModels(engine::projectDisplay(project)).named.clear(); //cfa790
    }, [&project](const std::string& text) { project.doeIo->noteQtWarning(text); },
    [&project](const xml::Node& node) {
        if (!project.display) return;
        auto* model = engine::displayMain(*project.display);
        if (model) readStepData(*model, node,
            project.originalSettings.doeStateVarConstructor.get(), project.doeIo.get()); //14140d100
    }, project.doeIo.get() };
    bindSessionImportOwner(importContext, project);
    readSessionRecords(*study.outputDocument, importContext);
}

}  // namespace m2m::doe
