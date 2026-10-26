#include "type2_save_node_prefix.hpp"

#include <algorithm>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"
#include "m2m/model.hpp"

namespace m2m::doe {

const char* Type2SaveNodePrefixResult::guardMessage() const noexcept
{
    if (nextBoundary == Type2SaveNextBoundary::VariableDimensionReconciliation)
        return "Type2 study Save: InputTable variable dimension reconciliation remains unsupported";
    return "Type2 study Save: InputTable XML output device remains unsupported";
}

Type2SaveNodePrefixResult appendType2SaveNodePrefix(engine::ConversionSession& session,
    Project& project, SubProject& sub, xml::Node& pending)
{
    if (session.liveProject() != &project ||
        std::none_of(project.subs.begin(), project.subs.end(),
            [&sub](const auto& owned) { return owned.get() == &sub; }))
        throw Error(M2M_E_UNSUPPORTED, "Type2 study Save prefix needs the live Session Project and its owned SUB");
    const auto study = sub.ownedDoeStudy;
    const auto io = project.doeIo;
    const auto manager = session.modelManagerOwner();
    if (sub.type != 2 || sub.project != &project || !study || sub.doe != study.get() ||
        study->sub != &sub || !io || !manager || io->modelManager() != manager.get() ||
        manager->ownerSession() != &session)
        throw Error(M2M_E_UNSUPPORTED, "Type2 study Save prefix needs matching owned Study/I/O/ModelManager identities");
    if (pending.kind != xml::Node::Element || pending.tag != qt::L("Sub_Project") ||
        (project.dom && project.dom->root &&
            std::any_of(project.dom->root->children.begin(), project.dom->root->children.end(),
                [&pending](const auto& installed) { return installed.get() == &pending; })))
        throw Error(M2M_E_UNSUPPORTED, "Type2 study Save prefix needs a pending new Sub_Project node");

    auto node = studyElement(*study); // Native0d29d0, before0d3200 and its file effects.
    pending.children.push_back(node);
    return {study->vars.empty() ? Type2SaveNextBoundary::XmlOutputDevice
                               : Type2SaveNextBoundary::VariableDimensionReconciliation,
            node, study};
}

} // namespace m2m::doe
