// ctest-labels: m2m-doe
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/doe_session_notifications.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/model.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4path.hpp"
#include "../support/test_paths.hpp"

using namespace m2m;
namespace {
std::string table(const std::string& children)
{ return "<sfDoeInputTable>" + children + "</sfDoeInputTable>"; }
std::string row(const std::string& children)
{
    return "<DoeVariable name=\"Described variable\" numSamples=\"3\" varId=\"7\" dimension=\"3\">" +
        children + "</DoeVariable>";
}
bool read(doe::Study& study, const std::string& children)
{ return doe::readInputTable(table("<DoeVariables>" + row(children) + "</DoeVariables>"), study); }
template<class Work> std::string refused(const Work& work)
{
    try { work(); }
    catch (const Error& error) {
        CHECK_EQ(error.code, M2M_E_UNSUPPORTED);
        return error.what();
    }
    CHECK(false);
    return {};
}
struct ActiveRig {
    engine::DiagramEffectDriver driver;
    engine::ConversionSession session{driver};
    SubProject* sub = nullptr;
    xml::NodePtr oldSub;
    String control;
    std::size_t seedWrites = 0;
    ActiveRig()
    {
        auto& p = session.project();
        const String folder((m2m_test::scratchRoot() / "doe-variable-description").u16string());
        initProject(p, folder, qt::L("described"));
        p.path = folder + qt::kSep + qt::L("described.moproj");
        p.doeIo = doe::makeRecordingStudyIo();
        p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.subs.push_back(makeSubProject(p, 2, 7));
        sub = p.subs.back().get();
        p.activeSub = 1; // Internal owner fixture, not public C ABI selection.
        p.openedOk = true;
        p.dom = std::make_shared<xml::Document>();
        p.dom->root = xml::createElement(qt::L("Project"));
        p.dom->root->children.push_back(xml::createElement(qt::L("Sub_Project")));
        oldSub = xml::createElement(qt::L("Sub_Project"));
        p.dom->root->children.push_back(oldSub);
        control = projectDir(p) + qt::kSep + qt::L("DoeStudies") + qt::kSep +
            qt::L("DescriptionStudy") + qt::kSep + qt::L("Control") + qt::kSep;
        CHECK(p.doeIo->writeTextFile(control + qt::L("DoeInputTable.XML"), table(
            "<DoeVariables>" + row("<Description>actual description</Description><Description/>") +
            "</DoeVariables><Operations/>")));
        CHECK(p.doeIo->writeTextFile(control + qt::L("DEF_DOE.PAR"), "0\r\n1\t4\t\r\n"));
        seedWrites = p.doeIo->textWrites().size();
    }
    void readStudy()
    {
        auto input = xml::createElement(qt::L("Sub_Project"));
        auto study = xml::createElement(qt::L("SfDoeStudy"));
        xml::setAttribute(*study, qt::L("name"), qt::L("DescriptionStudy"));
        input->children.push_back(study);
        Ctx ctx;
        ctx.session = &session;
        parseSubProject(session.project(), *input, ctx);
    }
};
class MinimalViews final : public moproj::DiagramViews {
public:
    void viewCentre(const SubProject&, double& x, double& y) override { x = 1; y = 2; }
    xml::NodePtr dispView(const SubProject&) override { return xml::createElement(qt::L("SfDispView")); }
    bool expanded(const SubProject&, int) override { return true; }
    moproj::RectF rect(const SubProject&, int) override { return {}; }
    std::vector<moproj::ExpansionItem> expansion(const SubProject&, int) override { return {}; }
};
}

TEST_CASE(doe_variable_description_assigns_nonempty_value_on_same_concrete_record)
{
    doe::Study study;
    REQUIRE(read(study, "<Description>plain description</Description>"));
    REQUIRE(study.headerVariableOwners.size() == 1 && study.vars.size() == 1);
    const auto& record = *study.headerVariableOwners.front();
    CHECK(study.vars[0] == &record);
    CHECK(record.description == qt::L("plain description") && !record.description.isNull());
    CHECK_EQ(record.numSamples, 3);
    CHECK_EQ(record.number, 7);
    CHECK_EQ(record.dimension, 3);
    CHECK_EQ(record.index, 0);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(record.ops.empty() && record.items.empty() && record.files.empty() && !record.link);
}

TEST_CASE(doe_variable_description_empty_and_raw_nodes_preserve_default_null)
{
    doe::Study study;
    REQUIRE(read(study, "raw<![CDATA[not a Description]]><!--outside--><?outside value?>"
                       "<Description/><Description><!--inside--><Inner/></Description>"));
    REQUIRE(study.headerVariableOwners.size() == 1);
    CHECK(study.headerVariableOwners.front()->description.isNull());
    CHECK(study.headerVariableOwners.front()->description.isEmpty());
}

TEST_CASE(doe_variable_description_repeated_nonempty_values_win_without_trimming_or_crlf_conversion)
{
    doe::Study study;
    REQUIRE(read(study, "<Description>first</Description><Description/>"
                       "<Description>second\nline</Description><Description><!--empty--></Description>"));
    CHECK(study.headerVariableOwners.front()->description == String(u"second\nline"));
    doe::Study spaces;
    // Explicit CDATA preserves a space through the existing XML parser; its
    // ordinary whitespace-only text-node filtering is a separate primitive.
    REQUIRE(read(spaces, "<Description>first</Description><Description><![CDATA[ ]]></Description><Description/>"));
    CHECK(spaces.headerVariableOwners.front()->description == qt::L(" "));
}

TEST_CASE(doe_variable_description_text_uses_existing_descendant_entity_cdata_seam)
{
    doe::Study study;
    REQUIRE(read(study, "<Description>A&amp;<![CDATA[B]]><Inner>C<Keyword>D</Keyword></Inner>"
                       "<!--ignored--><?ignored value?>&#xE9;</Description>"));
    REQUIRE(study.headerVariableOwners.size() == 1);
    CHECK(study.headerVariableOwners.front()->description == String(u"A&BCD\u00e9"));
    CHECK(study.vars.front()->items.empty() && study.vars.front()->ops.empty());
}

TEST_CASE(doe_variable_description_unqualified_direct_elements_keep_preappend_guard)
{
    const std::vector<std::string> guarded = {
        "<description>case variant</description>",
        "<Unknown><Description>nested</Description></Unknown>"
    };
    for (const auto& children : guarded) {
        doe::Study study;
        study.samples.invalid.fill(false);
        CHECK(refused([&] { (void)read(study, children); }).find("DoeVariables reader 0x14090ee20") != std::string::npos);
        CHECK(study.vars.empty() && study.headerVariableOwners.empty());
        CHECK_EQ(study.selectedVariableIndex, -1);
        CHECK(!study.samples.invalid[0] && !study.samples.invalid[1] && !study.samples.invalid[2]);
    }
}

TEST_CASE(doe_variable_description_direct_geometry_retains_reached_prefix)
{
    doe::Study study;
    study.samples.invalid.fill(false);
    const auto error = refused([&] {
        (void)read(study, "<Description>earlier</Description><Geometry/>");
    });
    CHECK(error.find("Geometry entry 0x140911532") != std::string::npos);
    REQUIRE(study.vars.size() == 1 && study.headerVariableOwners.size() == 1);
    const auto* const record = study.headerVariableOwners.front().get();
    CHECK(study.vars.front() == record);
    CHECK(record->description == qt::L("earlier"));
    CHECK_EQ(record->number, 7);
    CHECK_EQ(record->numSamples, 3);
    CHECK_EQ(record->dimension, 3);
    CHECK_EQ(record->index, -1);
    CHECK_EQ(study.selectedVariableIndex, 0);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
}

TEST_CASE(doe_variable_description_direct_keyword_keeps_only_reached_prefix)
{
    doe::Study beforeKeyword;
    beforeKeyword.samples.invalid.fill(false);
    const auto beforeError = refused([&] {
        (void)read(beforeKeyword,
            "<Description>earlier</Description><Keyword/><Description>unreached</Description>");
    });
    CHECK(beforeError.find("Keyword entry 0x14090f5bf") != std::string::npos);
    REQUIRE(beforeKeyword.vars.size() == 1 && beforeKeyword.headerVariableOwners.size() == 1);
    const auto* const before = beforeKeyword.headerVariableOwners.front().get();
    CHECK(beforeKeyword.vars.front() == before);
    CHECK(before->description == qt::L("earlier"));
    CHECK_EQ(before->number, 7);
    CHECK_EQ(before->index, -1);
    CHECK_EQ(beforeKeyword.selectedVariableIndex, 0);
    CHECK(beforeKeyword.samples.invalid[0] && beforeKeyword.samples.invalid[1] && beforeKeyword.samples.invalid[2]);

    doe::Study firstKeyword;
    firstKeyword.samples.invalid.fill(false);
    CHECK(refused([&] {
        (void)read(firstKeyword, "<Keyword/><Description>unreached</Description>");
    }).find("Keyword entry 0x14090f5bf") != std::string::npos);
    REQUIRE(firstKeyword.vars.size() == 1 && firstKeyword.headerVariableOwners.size() == 1);
    CHECK(firstKeyword.vars.front() == firstKeyword.headerVariableOwners.front().get());
    CHECK(firstKeyword.headerVariableOwners.front()->description.isNull());
    CHECK_EQ(firstKeyword.headerVariableOwners.front()->index, -1);
    CHECK_EQ(firstKeyword.selectedVariableIndex, 0);
    CHECK(firstKeyword.samples.invalid[0] && firstKeyword.samples.invalid[1] && firstKeyword.samples.invalid[2]);
}

TEST_CASE(doe_variable_description_partial_records_follow_variable_and_sibling_guards)
{
    doe::Study study;
    CHECK(refused([&] { (void)doe::readInputTable(table("<DoeVariables>" +
        row("<Description>kept</Description>") +
        "<DoeVariable name=\"Second variable\" numSamples=\"3\" varId=\"8\" dimension=\"3\">"
        "<Description>before Keyword</Description><Keyword/></DoeVariable>" +
        "</DoeVariables>"), study); }).find("Keyword entry 0x14090f5bf") != std::string::npos);
    REQUIRE(study.headerVariableOwners.size() == 2 && study.vars.size() == 2);
    CHECK(study.headerVariableOwners.front()->description == qt::L("kept"));
    CHECK(study.vars[0] == study.headerVariableOwners[0].get());
    CHECK(study.vars[1] == study.headerVariableOwners[1].get());
    CHECK(study.headerVariableOwners[1]->description == qt::L("before Keyword"));
    CHECK_EQ(study.vars[0]->index, -1); // The exact Keyword entry stops before whole-list reindex.
    CHECK_EQ(study.vars[1]->index, -1);
    CHECK(study.samples.invalid[0] && study.samples.invalid[1] && study.samples.invalid[2]);
    doe::Study later;
    CHECK(refused([&] { (void)doe::readInputTable(table("<DoeVariables>" +
        row("<Description>kept before constraint</Description>") +
        "</DoeVariables><Constraints><Constraint/></Constraints>"), later); })
        .find("Constraints reader") != std::string::npos);
    REQUIRE(later.headerVariableOwners.size() == 1);
    CHECK(later.headerVariableOwners.front()->description == qt::L("kept before constraint"));
    CHECK_EQ(later.vars[0]->index, 0);
}

TEST_CASE(doe_variable_description_actual_lazy_owner_continues_samples_and_save_guard)
{
    ActiveRig rig;
    auto& p = rig.session.project();
    const auto owner = rig.sub->ownedDoeStudy;
    rig.readStudy();
    CHECK(rig.sub->ownedDoeStudy == owner && rig.sub->doe == owner.get());
    REQUIRE(owner->headerVariableOwners.size() == 1 && owner->vars.size() == 1);
    const auto* record = owner->headerVariableOwners.front().get();
    CHECK(owner->vars[0] == record && record->description == qt::L("actual description"));
    CHECK_EQ(record->index, 0);
    REQUIRE(p.doeIo->textWrites().size() == rig.seedWrites + 3);
    const char* names[] = {"Samples_LHS.par", "Samples_FF.par", "Samples_USR.par"};
    for (std::size_t i = 0; i < 3; ++i)
        CHECK(p.doeIo->textWrites()[rig.seedWrites + i].path == rig.control + qt::L(names[i]));
    REQUIRE(p.doeIo->reads().size() == 2);
    CHECK(p.doeIo->reads().back().path == rig.control + qt::L("DEF_DOE.PAR"));
    CHECK(owner->samples.tables[0] == doe::SampleTable({{4.0}}));
    const auto writesBefore = p.doeIo->textWrites().size();
    MinimalViews views;
    Ctx ctx;
    ctx.session = &rig.session;
    engine::FileEffects opened;
    engine::SaveEffects saved;
    CHECK(refused([&] { (void)engine::saveProject(p, ctx, views, opened, saved); })
        .find("InputTable variable dimension reconciliation remains unsupported") != std::string::npos);
    CHECK(owner->headerVariableOwners.front().get() == record && owner->vars[0] == record);
    CHECK(record->description == qt::L("actual description"));
    CHECK(p.dom->root->children[1] == rig.oldSub && rig.oldSub->children.empty());
    CHECK(p.doeIo->textWrites().size() == writesBefore && p.doeIo->xmlWrites().empty());
    CHECK(saved.project.writes.empty());
    CHECK(rig.session.sessionNotificationStateOwner()->attempts.empty());
}

int main() { return testkit::run_all(); }
