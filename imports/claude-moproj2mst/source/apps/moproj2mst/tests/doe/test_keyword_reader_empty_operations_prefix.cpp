// ctest-labels: m2m-doe
// Synthetic exploratory branch case; not an original observation or parser-success claim.
#include <testkit/check.hpp>

#include <memory>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_study.hpp"
#include "m2m/doe_variable_record.hpp"
#include "m2m/xml.hpp"
#include "../../src/doe/keyword_reader_prefix.hpp"
#include "../../src/doe/keyword_constructor_record.hpp"

using namespace m2m;
namespace {
struct Rig {
    doe::Study study;
    doe::HeaderVariableRecord& add(const char* name)
    {
        auto owner = std::make_unique<doe::HeaderVariableRecord>(3);
        owner->name = qt::L(name);
        auto* alias = owner.get();
        study.headerVariableOwners.push_back(std::move(owner));
        study.vars.push_back(alias);
        return *alias;
    }
};
xml::NodePtr element(const char* name)
{
    auto node = std::make_shared<xml::Node>();
    node->tag = qt::L(name);
    return node;
}
xml::Node keyword(bool emptyOperations)
{
    auto node = *element("Keyword");
    node.attrs = {{qt::L("name"), qt::L("TLOC")}, {qt::L("unitType"), qt::L("0")},
        {qt::L("parmType"), qt::L("double")}, {qt::L("parameter"), qt::L("-1")},
        {qt::L("nominalValue"), qt::L("0.1")}};
    auto nominal = element("NominalData");
    auto text = element("string");
    text->children.push_back(xml::createTextNode(qt::L("TLOC    \t0.1")));
    nominal->children.push_back(text);
    auto operations = element("KwOperations");
    if (!emptyOperations) {
        auto operation = element("KwOperation");
        operation->attrs.push_back({qt::L("id"), qt::L("1")});
        operations->children.push_back(operation);
    }
    node.children = {nominal, operations};
    return node;
}
template<class Work> std::string refused(Work&& call)
{
    try { call(); }
    catch (const Error& error) { CHECK_EQ(error.code, M2M_E_UNSUPPORTED); return error.what(); }
    CHECK(false);
    return {};
}
}

TEST_CASE(keyword_reader_empty_operations_commits_named_scalar_text_then_stops)
{
    Rig rig;
    auto& a = rig.add("A");
    auto& b = rig.add("B");
    a.ops = {71, 71};
    // InputTable's outer header setter uses B while the keyword re-fetch selects A.
    b.description = qt::L("outer row effect");
    const auto message = refused([&] {
        doe::readBoundedKeywordConstructorPrefix(rig.study, qt::L("A"), 3, keyword(true), nullptr);
    });
    CHECK(message.find("scalar Keyword nominal-text prefix committed") != std::string::npos);
    REQUIRE(a.items.size() == 1);
    auto* record = a.items[0].keyword();
    REQUIRE(record);
    CHECK(record->nominalText == qt::L("TLOC    \t0.1"));
    CHECK_EQ(record->parameter, -1);
    CHECK(!record->function && !record->enumeration);
    CHECK(record->operations.empty() && a.ops == std::vector<int>({71, 71}));
    CHECK(b.items.empty() && b.description == qt::L("outer row effect"));
    CHECK(record->upperText.isNull() && record->lowerText.isNull());
}

TEST_CASE(keyword_reader_operation_one_keeps_legacy_guard_and_null_nominal_text)
{
    Rig rig;
    auto& a = rig.add("A");
    const auto message = refused([&] {
        doe::readBoundedKeywordConstructorPrefix(rig.study, qt::L("A"), 3, keyword(false), nullptr);
    });
    CHECK(message.find("before unresolved SetOprHasDoe notifier invocation") != std::string::npos);
    REQUIRE(a.items.size() == 1);
    CHECK(a.items[0].keyword()->nominalText.isNull());
    CHECK(a.items[0].keyword()->operations == std::vector<int>({1}));
}

int main() { return testkit::run_all(); }
