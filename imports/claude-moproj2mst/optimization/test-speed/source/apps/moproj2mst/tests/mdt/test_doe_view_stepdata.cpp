// ctest-labels: m2m-mdt mdt-l6 m2m-mdt-l7
#include <testkit/check.hpp>

#include <utility>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/doe_slice.hpp"
#include "m2m/doe_view_stepdata.hpp"

using namespace m2m;
namespace {
xml::NodePtr parse(const std::string& text)
{
    xml::Document document;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(document, text, &error, &line, &column));
    return document.root;
}
bool refuses(const auto& action, const char* message)
{
    try { action(); }
    catch (const Error& e) {
        return e.code == M2M_E_UNSUPPORTED && std::string(e.what()).find(message) != std::string::npos;
    }
    return false;
}
}

TEST_CASE(doe_view_stepdata_constructor_preserves_unavailable_matrix)
{
    doe::ViewStepDataState state;
    CHECK(!state.showSymmetry && state.symmetryCount == 0);
    CHECK((state.minimum == std::array<double, 3>({0, 0, 0})));
    CHECK((state.maximum == std::array<double, 3>({1, 1, 1})));
    CHECK((state.neighbors == std::array<int, 2>({-1, -1})));
    for (const auto& record : state.symmetry) {
        CHECK(!record.matrix && record.keepOrientation && !record.isActive);
        CHECK((record.equation == std::array<double, 4>({0, 0, 0, 0})));
        CHECK((record.neighbors == std::array<int, 2>({-1, -1})));
    }
}

TEST_CASE(doe_view_stepdata_groups_convert_missing_attributes_and_preserve_absent_groups)
{
    doe::ViewStepDataState state;
    doe::readObjectStepData(state, *parse("<Object><SfSymmetryInfo ShowSymm='-2'>"
        "<NumOfSymmData>-7</NumOfSymmData><Min X='1.25' Y='-2' Z='3e1'/>"
        "<Max X='4'/><NeighborSymData Index0='-8' Index1='bad'/>"
        "</SfSymmetryInfo></Object>"));
    CHECK(state.showSymmetry && state.symmetryCount == -7);
    CHECK((state.minimum == std::array<double, 3>({1.25, -2, 30})));
    CHECK((state.maximum == std::array<double, 3>({4, 0, 0})));
    CHECK((state.neighbors == std::array<int, 2>({-8, 0})));
    doe::readObjectStepData(state, *parse("<Object><SfSymmetryInfo/><Unknown/></Object>"));
    CHECK(!state.showSymmetry && state.symmetryCount == -7);
    CHECK(state.minimum[0] == 1.25 && state.maximum[0] == 4 && state.neighbors[0] == -8);
}

TEST_CASE(doe_view_stepdata_record_prefix_removal_invalid_suffix_and_last_index)
{
    doe::ViewStepDataState state;
    doe::readObjectStepData(state, *parse("<Object><SfSymmetryInfo>"
        "<SfSymmetryDataSfSymmetryData127><IsActive>-1</IsActive></SfSymmetryDataSfSymmetryData127>"
        "<SfSymmetryDataBad><KeepOrientation>0</KeepOrientation></SfSymmetryDataBad>"
        "<sfsymmetrydata1><IsActive>1</IsActive></sfsymmetrydata1>"
        "</SfSymmetryInfo></Object>"));
    CHECK(state.symmetry[127].isActive);
    CHECK(!state.symmetry[0].keepOrientation);
    CHECK(!state.symmetry[1].isActive && !state.symmetry[1].matrix);
}

TEST_CASE(doe_view_stepdata_record_matrix_and_repeated_groups_follow_encounter_order)
{
    doe::ViewStepDataState state;
    doe::readObjectStepData(state, *parse("<Object><SfSymmetryInfo><SfSymmetryData3>"
        "<Matrix P0='1.5' P5='-2' P15='9'/><OglEqn P0='3' P3='-4'/>"
        "<NeighborSymData Index0='7' Index1='8'/><IsActive>1</IsActive>"
        "</SfSymmetryData3><SfSymmetryData3><IsActive>bad</IsActive>"
        "<OglEqn P2='5'/></SfSymmetryData3></SfSymmetryInfo></Object>"));
    const auto& record = state.symmetry[3];
    REQUIRE(record.matrix.has_value());
    CHECK((*record.matrix)[0] == 1.5 && (*record.matrix)[5] == -2 && (*record.matrix)[15] == 9);
    CHECK((*record.matrix)[1] == 0 && (*record.matrix)[14] == 0);
    CHECK((record.equation == std::array<double, 4>({0, 0, 5, 0})));
    CHECK((record.neighbors == std::array<int, 2>({7, 8})));
    CHECK(!record.isActive && record.keepOrientation && !state.symmetry[2].matrix);
}

TEST_CASE(doe_view_stepdata_unsafe_index_retains_prior_flag_count_and_record_effects)
{
    for (const char* index : {"-1", "128"}) {
        doe::ViewStepDataState state;
        CHECK(refuses([&] { doe::readObjectStepData(state, *parse(
            std::string("<Object><SfSymmetryInfo ShowSymm='1'><NumOfSymmData>3</NumOfSymmData>") +
            "<SfSymmetryData0><IsActive>1</IsActive></SfSymmetryData0><SfSymmetryData" + index +
            "/><NumOfSymmData>9</NumOfSymmData></SfSymmetryInfo></Object>")); }, "unchecked storage"));
        CHECK(state.showSymmetry && state.symmetryCount == 3 && state.symmetry[0].isActive);
    }
}

TEST_CASE(doe_view_stepdata_other_object_branches_guard_in_encounter_order)
{
    for (const char* tag : {"SfDispOpt", "Slice"}) {
        doe::ViewStepDataState state;
        CHECK(refuses([&] { doe::readObjectStepData(state, *parse(
            std::string("<Object>raw<Unknown/><SfSymmetryInfo ShowSymm='1'><NumOfSymmData>4") +
            "</NumOfSymmData></SfSymmetryInfo><" + tag + "/><SfSymmetryInfo ShowSymm='0'/></Object>")); }, tag));
        CHECK(state.showSymmetry && state.symmetryCount == 4);
    }
}

TEST_CASE(doe_display_fields_mode_uses_signed_short_failure_not_wrapping)
{
    std::array<std::uint8_t, 46> flags{};
    std::int32_t mode = 77;
    constexpr std::pair<const char*, int> values[] = {
        {"32767", 32767}, {"-32768", -32768}, {"32768", 0},
        {"-32769", 0}, {"65535", 0}, {"invalid", 0}, {" -2 ", -2}
    };
    for (const auto& [text, expected] : values) {
        doe::readDisplayOptionFields(*parse(std::string("<SfDispOpt><DispModeType>") +
            text + "</DispModeType></SfDispOpt>"), flags, mode, 5);
        CHECK(mode == expected);
    }
}

TEST_CASE(doe_display_fields_map_only_eleven_flags_and_clear_missing_general_values)
{
    std::array<std::uint8_t, 46> flags;
    flags.fill(1);
    std::int32_t mode = 19;
    doe::readDisplayOptionFields(*parse("<SfDispOpt><DispFlagList SelectionOn='-2' ObjectOn='0'"
        " MeshOn='1' GeoOn='0' TranspOn='1' SliceOn='1' GeoNodalShadingOn='1'"
        " MeshNodalShadingOn='1' CrossSectionGeoOn='1' CrossSectionMeshOn='1'"
        " TransformCrossSectionOn='1'/></SfDispOpt>"), flags, mode, 0);
    CHECK(mode == 19 && flags[0] == 1 && flags[1] == 0 && flags[2] == 1 && flags[3] == 0);
    for (int index : {4, 5, 8, 9, 43, 44, 45}) CHECK(flags[index] == 1);
    doe::readDisplayOptionFields(*parse("<SfDispOpt><DispFlagList/></SfDispOpt>"), flags, mode, 0);
    for (int index = 0; index < 46; ++index) {
        const bool mapped = index <= 5 || index == 8 || index == 9 || index >= 43;
        CHECK(flags[index] == (mapped ? 0 : 1));
    }
}

TEST_CASE(doe_display_fields_missing_mesh_geometry_preserve_only_object_kind_five)
{
    for (int kind : {0, 2, 3, 5}) {
        std::array<std::uint8_t, 46> flags{};
        flags[2] = 1;
        flags[3] = 1;
        std::int32_t mode = 0;
        doe::readDisplayOptionFields(*parse("<SfDispOpt><DispFlagList/></SfDispOpt>"), flags, mode, kind);
        CHECK(flags[2] == (kind == 5 ? 1 : 0) && flags[3] == (kind == 5 ? 1 : 0));
    }
}

TEST_CASE(doe_display_fields_minus_one_sentinel_differs_from_short_overflow)
{
    for (int kind : {0, 5}) {
        std::array<std::uint8_t, 46> flags{};
        flags[2] = 1;
        flags[3] = 1;
        std::int32_t mode = 0;
        doe::readDisplayOptionFields(*parse("<SfDispOpt><DispFlagList MeshOn='-1' GeoOn='65535'"
            " SelectionOn='32768' ObjectOn='-32769' TranspOn='-1'/></SfDispOpt>"), flags, mode, kind);
        CHECK(flags[2] == 1 && flags[3] == 0 && flags[0] == 0 && flags[1] == 0 && flags[4] == 1);
        doe::readDisplayOptionFields(*parse("<SfDispOpt><DispFlagList MeshOn='65535' GeoOn='-1'/>"
            "</SfDispOpt>"), flags, mode, kind);
        CHECK(flags[2] == 0 && flags[3] == 0);
    }
}

TEST_CASE(doe_display_fields_repeated_children_merge_without_implicit_reset)
{
    std::array<std::uint8_t, 46> flags{};
    flags[7] = 1;
    std::int32_t mode = 99;
    doe::readDisplayOptionFields(*parse("<SfDispOpt>text<Unknown/><DispModeType>-3</DispModeType>"
        "<DispFlagList MeshOn='1' GeoOn='1' ObjectOn='1'/><DispModeType>8</DispModeType>"
        "<DispFlagList SelectionOn='1'/></SfDispOpt>"), flags, mode, 5);
    CHECK(mode == 8 && flags[0] == 1 && flags[1] == 0 && flags[2] == 1 && flags[3] == 1);
    CHECK(flags[7] == 1);
}

TEST_CASE(doe_display_fields_unknown_or_absent_children_leave_state_untouched)
{
    std::array<std::uint8_t, 46> flags;
    flags.fill(1);
    const auto prior = flags;
    std::int32_t mode = -9;
    doe::readDisplayOptionFields(*parse("<SfDispOpt><dispflaglist MeshOn='0'/>raw<!--x--></SfDispOpt>"),
        flags, mode, 0);
    CHECK(flags == prior && mode == -9);
}

TEST_CASE(doe_object_display_callback_observes_original_child_order)
{
    doe::ViewStepDataState state;
    std::vector<int> observed;
    doe::readObjectStepData(state, *parse("<Object><SfDispOpt Mark='100'/><SfSymmetryInfo>"
        "<NumOfSymmData>3</NumOfSymmData></SfSymmetryInfo><SfDispOpt Mark='200'/></Object>"),
        [&](const xml::Node& options) {
            CHECK(options.tag == qt::L("SfDispOpt"));
            observed.push_back(qt::toInt(options.attribute(qt::L("Mark"))) + state.symmetryCount);
        });
    CHECK(observed == std::vector<int>({100, 203}));
}

TEST_CASE(doe_object_display_callback_failure_retains_prior_effects_and_stops_later_children)
{
    doe::ViewStepDataState state;
    int calls = 0;
    CHECK(refuses([&] { doe::readObjectStepData(state, *parse("<Object><SfSymmetryInfo ShowSymm='1'>"
        "<NumOfSymmData>4</NumOfSymmData></SfSymmetryInfo><SfDispOpt/>"
        "<SfSymmetryInfo ShowSymm='0'/></Object>"), [&](const xml::Node&) {
            ++calls;
            throw Error(M2M_E_UNSUPPORTED, "injected callback stop");
        }); }, "injected callback stop"));
    CHECK(calls == 1 && state.showSymmetry && state.symmetryCount == 4);
}

TEST_CASE(doe_object_slice_locals_merge_within_call_but_start_unavailable_next_call)
{
    doe::ViewStepDataState state;
    std::vector<doe::SliceCoefficients> observed;
    const auto import = [&](const doe::SliceCoefficients& values) { observed.push_back(values); };
    doe::readObjectStepData(state, *parse("<Object><Slice><a>2</a><c>0</c><d>-4</d></Slice>"
        "<Slice><b>3</b><a>1</a><a>4</a><B>9</B></Slice></Object>"), {}, import);
    REQUIRE(observed.size() == 2);
    CHECK(observed[0].values[0] == 2 && !observed[0].values[1] && observed[0].values[3] == -4);
    CHECK(observed[1].values[0] == 4 && observed[1].values[1] == 3 && observed[1].values[2] == 0);
    CHECK(observed[1].values[3] == -4);
    doe::readObjectStepData(state, *parse("<Object><Slice/></Object>"), {}, import);
    REQUIRE(observed.size() == 3);
    for (const auto& value : observed[2].values) CHECK(!value.has_value());
}

TEST_CASE(doe_object_slice_and_display_callbacks_preserve_child_order)
{
    doe::ViewStepDataState state;
    std::vector<int> observed;
    doe::readObjectStepData(state, *parse("<Object><Slice><a>bad</a></Slice><SfSymmetryInfo>"
        "<NumOfSymmData>3</NumOfSymmData></SfSymmetryInfo><SfDispOpt/><Slice><d>2</d></Slice></Object>"),
        [&](const xml::Node&) { observed.push_back(100 + state.symmetryCount); },
        [&](const doe::SliceCoefficients& values) {
            CHECK(values.values[0] == 0);
            observed.push_back(state.symmetryCount);
        });
    CHECK(observed == std::vector<int>({0, 103, 3}));
}

TEST_CASE(doe_object_slice_owner_refusal_retains_earlier_symmetry_effects)
{
    doe::ViewStepDataState state;
    int calls = 0;
    CHECK(refuses([&] { doe::readObjectStepData(state, *parse("<Object><SfSymmetryInfo ShowSymm='1'>"
        "<NumOfSymmData>2</NumOfSymmData></SfSymmetryInfo><Slice><a>1</a></Slice>"
        "<SfSymmetryInfo ShowSymm='0'/></Object>"), {}, [&](const doe::SliceCoefficients& values) {
            ++calls;
            CHECK(values.values[0] == 1 && !values.values[3]);
            throw Error(M2M_E_UNSUPPORTED, "injected Slice owner boundary");
        }); }, "Slice owner boundary"));
    CHECK(calls == 1 && state.showSymmetry && state.symmetryCount == 2);
}

TEST_CASE(doe_display_options_writer_zero_flags_still_emits_mode_and_empty_list)
{
    std::array<std::uint8_t, 46> flags{};
    flags[7] = flags[10] = flags[42] = 255; //unserialized slots are ignored
    const auto before = flags;
    auto node = parse("<SfDispOpt/>");
    doe::writeDisplayOptionFields(*node, flags, -32768, 0);
    const auto children = node->childElements();
    REQUIRE(children.size() == 2);
    CHECK(children[0]->tag == qt::L("DispModeType") && children[0]->text() == qt::L("-32768"));
    CHECK(children[1]->tag == qt::L("DispFlagList") && children[1]->attrs.empty());
    CHECK(flags == before);
}

TEST_CASE(doe_display_options_writer_preserves_unsigned_bytes_and_attribute_order)
{
    std::array<std::uint8_t, 46> flags{};
    for (int index : {0, 1, 2, 3, 4, 5, 43, 44, 45, 8, 9}) flags[index] = 255;
    auto node = parse("<SfDispOpt/>");
    doe::writeDisplayOptionFields(*node, flags, 6, 0);
    const auto list = node->childElements()[1];
    const char* const names[] = {"SelectionOn", "ObjectOn", "MeshOn", "GeoOn", "TranspOn", "SliceOn",
        "CrossSectionGeoOn", "CrossSectionMeshOn", "TransformCrossSectionOn", "GeoNodalShadingOn",
        "MeshNodalShadingOn"};
    REQUIRE(list->attrs.size() == 11);
    for (int index = 0; index < 11; ++index) {
        CHECK(list->attrs[index].name == qt::L(names[index]));
        CHECK(list->attrs[index].value == qt::L(index == 2 || index == 3 ? "1" : "255"));
    }
}

TEST_CASE(doe_display_options_writer_kind5_forces_only_mesh_geometry_sentinels)
{
    std::array<std::uint8_t, 46> flags{};
    auto node = parse("<SfDispOpt/>");
    doe::writeDisplayOptionFields(*node, flags, 0, 5);
    const auto list = node->childElements()[1];
    REQUIRE(list->attrs.size() == 2);
    CHECK(list->attrs[0].name == qt::L("MeshOn") && list->attrs[0].value == qt::L("-1"));
    CHECK(list->attrs[1].name == qt::L("GeoOn") && list->attrs[1].value == qt::L("-1"));
    CHECK(flags[2] == 0 && flags[3] == 0);
}

TEST_CASE(doe_symmetry_writer_zero_count_skips_show_flag_and_unavailable_matrices)
{
    doe::ViewStepDataState state;
    state.showSymmetry = true;
    auto node = parse("<Object><Earlier/></Object>");
    doe::writeObjectSymmetry(state, *node);
    REQUIRE(node->childElements().size() == 1);
    CHECK(node->childElements()[0]->tag == qt::L("Earlier"));
    CHECK(state.showSymmetry && !state.symmetry[0].matrix);
}

TEST_CASE(doe_symmetry_writer_negative_count_keeps_headers_and_qdom_double_precision)
{
    doe::ViewStepDataState state;
    state.symmetryCount = -2;
    state.minimum[0] = 1.2345678901234567;
    state.neighbors = {-5, 11};
    auto node = parse("<Object/>");
    doe::writeObjectSymmetry(state, *node);
    const auto symmetry = node->childElements()[0];
    CHECK(symmetry->tag == qt::L("SfSymmetryInfo") && symmetry->attribute(qt::L("ShowSymm")) == qt::L("0"));
    const auto children = symmetry->childElements();
    REQUIRE(children.size() == 4);
    CHECK(children[0]->tag == qt::L("NumOfSymmData") && children[0]->text() == qt::L("-2"));
    CHECK(children[1]->tag == qt::L("Min") && children[1]->attribute(qt::L("X")) == qt::L("1.234567890123457"));
    CHECK(children[2]->tag == qt::L("Max") && children[2]->attribute(qt::L("Z")) == qt::L("1"));
    CHECK(children[3]->tag == qt::L("NeighborSymData"));
    CHECK(children[3]->attribute(qt::L("Index0")) == qt::L("-5"));
    CHECK(children[3]->attribute(qt::L("Index1")) == qt::L("11"));
}

TEST_CASE(doe_symmetry_writer_unknown_matrix_keeps_attached_header_and_record_prefix)
{
    doe::ViewStepDataState state;
    state.showSymmetry = true;
    state.symmetryCount = 1;
    auto node = parse("<Object/>");
    CHECK(refuses([&] { doe::writeObjectSymmetry(state, *node); }, "first P0"));
    const auto symmetry = node->childElements()[0];
    CHECK(symmetry->attribute(qt::L("ShowSymm")) == qt::L("1"));
    const auto children = symmetry->childElements();
    REQUIRE(children.size() == 5);
    CHECK(children[4]->tag == qt::L("SfSymmetryData0") && children[4]->childElements().empty());
    CHECK(!state.symmetry[0].matrix && state.symmetryCount == 1);
}

TEST_CASE(doe_symmetry_writer_known_matrix_record_order_and_values_roundtrip)
{
    doe::ViewStepDataState state;
    state.symmetryCount = 1;
    state.symmetry[0].matrix.emplace();
    for (int index = 0; index < 16; ++index) (*state.symmetry[0].matrix)[index] = index * 0.125;
    state.symmetry[0].keepOrientation = false;
    state.symmetry[0].isActive = true;
    state.symmetry[0].equation = {0.5, -0.25, 2, 4};
    state.symmetry[0].neighbors = {-3, 19};
    auto node = parse("<Object/>");
    doe::writeObjectSymmetry(state, *node);
    const auto record = node->childElements()[0]->childElements()[4];
    const auto fields = record->childElements();
    REQUIRE(fields.size() == 5);
    CHECK(fields[0]->tag == qt::L("Matrix") && fields[0]->attrs.size() == 16);
    CHECK(fields[0]->attrs[0].name == qt::L("P0") && fields[0]->attrs[15].name == qt::L("P15"));
    CHECK(fields[0]->attribute(qt::L("P15")) == qt::L("1.875"));
    CHECK(fields[1]->tag == qt::L("KeepOrientation") && fields[1]->text() == qt::L("0"));
    CHECK(fields[2]->tag == qt::L("IsActive") && fields[2]->text() == qt::L("1"));
    CHECK(fields[3]->tag == qt::L("OglEqn") && fields[3]->attribute(qt::L("P1")) == qt::L("-0.25"));
    CHECK(fields[4]->tag == qt::L("NeighborSymData") && fields[4]->attribute(qt::L("Index1")) == qt::L("19"));
    doe::ViewStepDataState restored;
    doe::readObjectStepData(restored, *node);
    CHECK(restored.symmetryCount == 1 && restored.symmetry[0].matrix == state.symmetry[0].matrix);
    CHECK(!restored.symmetry[0].keepOrientation && restored.symmetry[0].isActive);
    CHECK(restored.symmetry[0].equation == state.symmetry[0].equation);
    CHECK(restored.symmetry[0].neighbors == state.symmetry[0].neighbors);
}

TEST_CASE(doe_symmetry_writer_invalid_record_index_preserves_all_prior_records)
{
    doe::ViewStepDataState state;
    state.symmetryCount = 129;
    for (auto& record : state.symmetry) record.matrix.emplace();
    auto node = parse("<Object/>");
    CHECK(refuses([&] { doe::writeObjectSymmetry(state, *node); }, "invalid record index"));
    const auto children = node->childElements()[0]->childElements();
    REQUIRE(children.size() == 4 + 129);
    CHECK(children[4 + 127]->tag == qt::L("SfSymmetryData127"));
    CHECK(children[4 + 127]->childElements().size() == 5);
    CHECK(children.back()->tag == qt::L("SfSymmetryData128") && children.back()->childElements().empty());
}

int main() { return testkit::run_all(); }
