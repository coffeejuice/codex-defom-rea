#include "variable_prefix.hpp"

#include <algorithm>
#include <string>
#include <utility>

#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/model_manager.hpp"

namespace m2m::doe {
namespace {
using qt::L;

[[noreturn]] void unsupported(const char* reason)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("DOE variable prefix 0x14090ee20: ") + reason);
}

String lowerAscii(const String& input)
{
    auto text = input.u16();
    for (auto& ch : text) {
        if (ch >= 128) unsupported("non-ASCII parameter type remains unrecovered");
        if (ch >= u'A' && ch <= u'Z') ch = static_cast<char16_t>(ch + (u'a' - u'A'));
    }
    return String(std::move(text));
}

String firstStringChild(const xml::Node& wrapper)
{
    // 14090fe64..14090ff8b: inspect firstChild once, not all child elements.
    if (wrapper.children.empty() || !wrapper.children.front()) return {};
    const auto& first = *wrapper.children.front();
    return first.kind == xml::Node::Element && first.tag == L("string") ? first.text() : String();
}

void requireVariableHeaders(const xml::Node& node, const String& name, int samples)
{
    // Existing reconstruction admission only, not native header validation.
    // Numeric varId/dimension, LinkTo and keyword guards remain after append.
    if (name.isEmpty() || samples < 1 || !node.hasAttribute(L("varId")) ||
        !node.hasAttribute(L("dimension")))
        unsupported("missing/invalid B121 variable headers remain unrecovered");
}

void requireRetainableFirstRow(const xml::Node& doeVariables)
{
    for (const auto* node : doeVariables.childElements()) {
        if (node->tag != L("DoeVariable")) continue;
        const String name = node->attribute(L("name"));
        const int samples = qt::toInt(node->attribute(L("numSamples")));
        requireVariableHeaders(*node, name, samples);
        return; // Do not choose a later valid row or inspect later child state.
    }
    unsupported("empty variable-list finalization is outside this prefix");
}
} // namespace

ScalarVariablePrefixRecord::ScalarVariablePrefixRecord(int samples) : numSamples(samples)
{
    // 140816900. Do not use the legacy projection's zero index/type defaults.
    index = -1;
    number = 0;
    geoType = 100;
    link = nullptr;
}

B121ScalarVariablePrefix::B121ScalarVariablePrefix(std::shared_ptr<Study> study)
    : study_(std::move(study)), state_(std::make_shared<B121ScalarPrefixState>())
{
    if (!study_ || !study_->sub || !study_->sub->project)
        unsupported("requires the actual owned Type2 Study");
    sub_ = study_->sub;
    project_ = sub_->project;
    io_ = project_->doeIo;
    requireBinding();
}

B121ScalarVariablePrefix::~B121ScalarVariablePrefix()
{
    // Adapter lifetime only: avoid leaving a dangling legacy Var alias after
    // its explicit prefix owner is disposed. Not native reset/Close behavior.
    if (!retained_ && state_->variable_) {
        auto& vars = study_->vars;
        vars.erase(std::remove(vars.begin(), vars.end(), state_->variable_.get()), vars.end());
    }
}

void B121ScalarVariablePrefix::retainOnStudy()
{
    if (study_->retainedScalarPrefix_)
        unsupported("retained scalar prefix retry is unsupported");
    study_->retainedScalarPrefix_ = state_;
    retained_ = true;
}

void B121ScalarVariablePrefix::requireBinding() const
{
    if (!project_ || activeSubProject(*project_) != sub_ || sub_->type != 2 ||
        study_->sub != sub_ || sub_->ownedDoeStudy.get() != study_.get() ||
        sub_->doe != study_.get() || !io_ || project_->doeIo != io_)
        unsupported("actual active Study ownership or I/O binding is unavailable");
}

void B121ScalarVariablePrefix::requireFresh(const xml::Node& doeVariables) const
{
    requireBinding();
    if (state_->started_ || !study_->vars.empty() ||
        !study_->scalarPrefixVariableOwners.empty() || !study_->geometryPrefixVariableOwners.empty() ||
        (study_->retainedScalarPrefix_ &&
            (!retained_ || study_->retainedScalarPrefix_ != state_)))
        unsupported("only a fresh empty manager is recovered; prefix retry is unsupported");
    if (study_->samples.mode != 0)
        unsupported("mode1/2 design acquisition remains unrecovered");
    for (const auto& table : study_->samples.tables)
        if (!table.empty()) unsupported("preexisting design rows remain unrecovered");
    if (doeVariables.kind != xml::Node::Element || doeVariables.tag != L("DoeVariables"))
        unsupported("expected the actual DoeVariables element");
}

[[noreturn]] void B121ScalarVariablePrefix::read(const xml::Node& doeVariables)
{
    requireFresh(doeVariables);
    state_->started_ = true;
    for (const auto* node : doeVariables.childElements()) {
        if (node->tag != L("DoeVariable")) continue;
        const String name = node->attribute(L("name"));       // 14090ef1b
        const int samples = qt::toInt(node->attribute(L("numSamples"))); // 14090f01f
        requireVariableHeaders(*node, name, samples);
        state_->variable_ = std::make_unique<ScalarVariablePrefixRecord>(samples);
        auto& variable = *state_->variable_;
        study_->vars.push_back(&variable);                  // 140919ec7: append before name
        variable.name = name;
        state_->selectedIndex_ = 0;                         // 140919f8b
        study_->samples.invalid.fill(true);                 // 140919fb8
        variable.number = qt::toInt(node->attribute(L("varId"))); // 14090f225
        variable.dimension = qt::toInt(node->attribute(L("dimension"))); // 14090f352
        if (variable.number < 1 || variable.dimension != 3)
            unsupported("nonpositive variable ID or non-3D B121 branch remains unrecovered");
        if (!node->attribute(L("LinkTo")).isEmpty())
            unsupported("deferred LinkTo state remains unrecovered");
        for (const auto* child : node->childElements()) {
            if (child->tag == L("Description")) {
                const String text = child->text();
                if (!text.isEmpty()) variable.description = text; // 14090f543
            }
            if (child->tag == L("Keyword")) readKeyword(*child);
            if (child->tag == L("Geometry"))
                unsupported("Geometry child remains unrecovered");
        }
        unsupported("variable without the scalar Keyword notification branch remains unrecovered");
    }
    unsupported("empty variable-list finalization is outside this prefix");
}

[[noreturn]] void B121ScalarVariablePrefix::readKeyword(const xml::Node& keyword)
{
    const String name = keyword.attribute(L("name"));        // 14090f637
    const int unit = qt::toInt(keyword.attribute(L("unitType"))); // 14090f6e5
    const String type = lowerAscii(keyword.attribute(L("parmType"))); // 14090f7a8
    const int parameter = qt::toInt(keyword.attribute(L("parameter"))); // 14090f8bf
    const String parameterName = keyword.attribute(L("parameterName")); // 14090f991
    const double nominal = qt::toDouble(keyword.attribute(L("nominalValue"))); // 14090fa48
    if (name != L("TLOC") || unit != 0 || type != L("double") || !parameterName.isEmpty())
        unsupported("only the TLOC/unit0/scalar-double keyword prefix is recovered");
    String nominalText;
    std::vector<int> operations;
    for (const auto* child : keyword.childElements()) {
        if (child->tag == L("NominalData")) nominalText = firstStringChild(*child);
        if (child->tag == L("KwOperations")) {
            for (const auto* operation : child->childElements())
                if (operation->tag == L("KwOperation"))
                    operations.push_back(qt::toInt(operation->attribute(L("id")))); // 140910762
        }
        if (child->tag == L("EnumData") || child->tag == L("UpperData") ||
            child->tag == L("LowerData") ||
            (child->tag == L("KwEntities") && !child->childElements().empty()))
            unsupported("enum/bounds/entity child state remains outside the B121 prefix");
    }
    if (operations.empty() || std::any_of(operations.begin(), operations.end(), [](int id) { return id < 1; }))
        unsupported("B121 prefix requires positive operation IDs for its reached notification");
    ScalarKeywordPrefixRecord record;                       // 1409206e0, keyword ID8 verified PE table
    record.unitType = unit;
    record.parameterName = parameterName;
    record.nominalValue = nominal;
    record.operations = operations;
    auto& variable = *state_->variable_;
    variable.dimension = 3;                                // 140816b8f before keyword append
    variable.keywords.push_back(std::move(record));          // 140816d24
    for (const int id : operations)                         // 140816d29..140816db5
        if (std::find(variable.ops.begin(), variable.ops.end(), id) == variable.ops.end())
            variable.ops.push_back(id);
    state_->pending_.emplace();
    state_->pending_->operation = operations.front();
    state_->pending_->parsedParameter = parameter;
    state_->pending_->collectedNominalText = nominalText;
    // SfMoDoeManager 1400d25a0 ->1402cf3c0. The keyword/list mutations are
    // committed, but no delivery, later selector/type/text setter or reindex ran.
    unsupported("2signalSetOprHasDoe() delivery is unresolved after committed keyword/operation prefix");
}

[[noreturn]] void readOwnedB121ScalarPrefix(engine::ConversionSession& session,
    Project& project, SubProject& sub, const xml::Node& doeVariables)
{
    // Check Session storage and SUB membership before following their backlinks.
    if (session.liveProject() != &project ||
        std::none_of(project.subs.begin(), project.subs.end(),
            [&sub](const auto& owned) { return owned.get() == &sub; }))
        unsupported("retained prefix needs the live Session Project and its owned SUB");
    const auto study = sub.ownedDoeStudy;
    const auto io = project.doeIo;
    const auto manager = session.modelManagerOwner();
    const auto notifier = session.sessionNotificationStateOwner();
    if (activeSubProject(project) != &sub || sub.type != 2 || sub.project != &project ||
        !study || sub.doe != study.get() || study->sub != &sub || !io || !manager ||
        manager->ownerSession() != &session || io->modelManager() != manager.get() ||
        !notifier || io->observedSessionNotificationState() != notifier.get())
        unsupported("retained prefix needs matching active Study/I/O/ModelManager/notifier owners");
    if (study->retainedScalarPrefix())
        unsupported("retained scalar prefix retry is unsupported");
    B121ScalarVariablePrefix driver(study);
    driver.requireFresh(doeVariables); // Admission refusal creates no retained state.
    requireRetainableFirstRow(doeVariables); // No empty/malformed pre-append marker.
    driver.retainOnStudy();            // Before allocation/append and every later guard.
    driver.read(doeVariables);         // Never reports full read or notification delivery.
}

} // namespace m2m::doe
