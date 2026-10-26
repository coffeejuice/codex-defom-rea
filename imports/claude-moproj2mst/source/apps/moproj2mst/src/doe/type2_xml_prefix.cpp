#include "type2_xml_prefix.hpp"

#include <algorithm>

#include "m2m/common.hpp"
#include "m2m/doe_constraint_record.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_xml_output_device.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::doe {
namespace {
void appendConstraintElements(const Study& study, xml::Node& root)
{
    if (study.constraints < 0 ||
        static_cast<std::size_t>(study.constraints) != study.constraintRecords.size() ||
        std::any_of(study.constraintRecords.begin(), study.constraintRecords.end(),
            [](const auto& record) { return !record || record->value != 0.0; }))
        throw Error(M2M_E_UNSUPPORTED, "Type2 InputTable constraint owner remains unsupported after XML open");
    if (study.constraintRecords.empty()) return;
    auto constraints = xml::createElement(qt::L("Constraints"));
    root.children.push_back(constraints);
    for (const auto& record : study.constraintRecords) {
        auto row = xml::createElement(qt::L("Constraint"));
        constraints->children.push_back(row);
        // 90dce3..90de4a read +4,+18,+8,+c, then insert in this order.
        // QtXml's %.16g / fallback g6 both spell the admitted +0.0 as "0".
        // The source-unwritten record+10 is not consumed by this serializer.
        xml::setAttribute(*row, qt::L("Id"), qt::number(record->id));
        xml::setAttribute(*row, qt::L("Value"), qt::L("0"));
        xml::setAttribute(*row, qt::L("ObjectNo"), qt::number(record->objectNo));
        xml::setAttribute(*row, qt::L("RegionNo"), qt::number(record->regionNo));
    }
}

// Empty-variable DOM values and bounded owned Constraint records. Kept separate
// from the legacy FileEffects planner so construction/serialization follows open.
xml::Document emptyInputTableDocument(const Study& study)
{
    using qt::L;
    xml::Document doc;
    doc.doctypeName = L("DoeInputTableFile");
    doc.root = xml::createElement(L("sfDoeInputTable"));
    xml::setAttribute(*doc.root, L("Version"), L("1"));
    const auto child = [&](const char* tag) {
        auto node = xml::createElement(L(tag));
        doc.root->children.push_back(node);
        return node;
    };
    xml::setAttribute(*child("DoeCommonMeshPath"), L("path"), study.commonMeshPath);
    xml::setAttribute(*child("DoeCommonMeshOrigName"), L("originalName"), study.commonMeshOrigName);
    xml::setAttribute(*child("DoeDoPositioning"), L("bool"), L(study.doPositioning ? "true" : "false"));
    xml::setAttribute(*child("DoeRoiType"), L("type"), qt::number(study.roiType));
    if (!study.regions.empty()) {
        auto regions = child("RegionsOfInterest");
        for (const auto& name : study.regions) {
            auto region = xml::createElement(L("Region"));
            xml::setAttribute(*region, L("name"), name);
            regions->children.push_back(region);
        }
    }
    child("DoeVariables");
    child("Operations");
    appendConstraintElements(study, *doc.root);
    return doc;
}
} // namespace

Type2InputTableXmlResult writeType2InputTableXmlPrefix(engine::ConversionSession& session,
    Project& project, SubProject& sub, const Type2SaveNodePrefixResult& nodePrefix, unsigned codepage)
{
    if (session.liveProject() != &project)
        throw Error(M2M_E_UNSUPPORTED, "Type2 XML prefix needs its live Session Project");
    const auto study = nodePrefix.studyOwner.lock();
    const auto io = project.doeIo;
    const auto manager = session.modelManagerOwner();
    const auto validate = [&] {
        if (session.liveProject() != &project ||
            std::none_of(project.subs.begin(), project.subs.end(),
                [&sub](const auto& owned) { return owned.get() == &sub; }))
            throw Error(M2M_E_UNSUPPORTED, "Type2 XML prefix lost its actual Project/SUB owner");
        if (sub.type != 2 || sub.project != &project || !study || sub.ownedDoeStudy != study ||
            sub.ownedDoeStudy.owner_before(study) || study.owner_before(sub.ownedDoeStudy) ||
            sub.doe != study.get() || study->sub != &sub || !io || project.doeIo != io ||
            project.doeIo.owner_before(io) || io.owner_before(project.doeIo) ||
            !manager || manager != session.modelManagerOwner() ||
            io->modelManager() != manager.get() || manager->ownerSession() != &session)
            throw Error(M2M_E_UNSUPPORTED, "Type2 XML prefix lost its cached Study/I/O/manager identities");
    };
    validate();
    if (!nodePrefix.appendedStudyNode || nodePrefix.nextBoundary != Type2SaveNextBoundary::XmlOutputDevice ||
        !study->vars.empty())
        throw Error(M2M_E_UNSUPPORTED, "Type2 XML prefix requires completed node prefix and empty variables before reconciliation");
    Type2InputTableXmlResult result;
    const qt::String sep(std::u16string(1, qt::kSep));
    result.path = projectDir(project) + sep + qt::L("DoeStudies") + sep + study->name + sep +
        qt::L("Control") + sep + qt::L("DoeInputTable.XML");
    auto file = io->openInputTableXmlFile(result.path); // Immediate native/simulated truncation.
    if (!file) throw Error(M2M_E_UNSUPPORTED, "Type2 XML output device allocation is unavailable");
    result.opened = file->opened();
    if (!result.opened) {
        io->setLastError(qt::arg(qt::L("Failed to open file, %1"), result.path));
        validate();
        (void)io->observeInputTableXmlWriterReturn(*file, result.path, false, false);
        return result; // 0d3200 still reaches DoeTableRefresh after this false.
    }
    validate();
    if (!study->vars.empty())
        throw Error(M2M_E_UNSUPPORTED, "Type2 InputTable variable serialization remains unsupported after XML open");
    const auto doc = emptyInputTableDocument(*study);
    {
        mst::Stream stream(codepage, file.get());
        stream << qt::String(xml::toString(doc, 1));
        file->close(); // Underlying QFile pre-flush then aboutToClose text flush.
        result.streamFailed = stream.writeFailed();
        result.fileErrorAfterClose = file->error();
        result.writerSucceeded = *result.fileErrorAfterClose == 0;
    }
    if (!result.writerSucceeded)
        io->setLastError(qt::arg(qt::L("Error writing file, %1"), result.path));
    validate();
    (void)io->observeInputTableXmlWriterReturn(*file, result.path, result.writerSucceeded, result.streamFailed);
    return result; // No refresh receiver or later sample writer is invoked.
}

} // namespace m2m::doe
