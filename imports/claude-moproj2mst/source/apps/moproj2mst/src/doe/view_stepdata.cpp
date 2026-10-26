#include "m2m/doe_view_stepdata.hpp"

#include <utility>

#include <defrt/stdlib/legacy_printf.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_slice.hpp"

namespace m2m::doe {
namespace {
using qt::L;

void appendText(xml::Node& parent, const char* tag, const qt::String& text)
{
    auto node = xml::createElement(L(tag));
    node->children.push_back(xml::createTextNode(text));
    parent.children.push_back(std::move(node));
}

//Shipped QtXml6100c210: same established CRT seam as DomElement::setDouble.
//The QDom double overload uses %.16g, not QString::number's default precision.
void attributeDouble(xml::Node& node, const qt::String& name, double value)
{
    char buffer[256];
    const int count = def::rt::crt::snprintf(buffer, sizeof buffer, "%.16g", value);
    xml::setAttribute(node, name, count > 0
        ? qt::String::latin1(std::string(buffer, static_cast<std::size_t>(count)))
        : qt::number(value));
}

void writeRecord(const ViewSymmetryRecord& state, xml::Node& node)
{
    auto matrix = xml::createElement(L("Matrix"));
    if (!state.matrix)
        throw Error(M2M_E_UNSUPPORTED, "DOE Object symmetry writer needs native matrix at first P0 read");
    for (int index = 0; index < 16; ++index)
        attributeDouble(*matrix, qt::arg(L("P%1"), index), (*state.matrix)[index]);
    node.children.push_back(std::move(matrix));
    appendText(node, "KeepOrientation", qt::number(state.keepOrientation ? 1 : 0));
    appendText(node, "IsActive", qt::number(state.isActive ? 1 : 0));
    auto equation = xml::createElement(L("OglEqn"));
    for (int index = 0; index < 4; ++index)
        attributeDouble(*equation, qt::arg(L("P%1"), index), state.equation[index]);
    node.children.push_back(std::move(equation));
    auto neighbors = xml::createElement(L("NeighborSymData"));
    for (int index = 0; index < 2; ++index)
        xml::setAttribute(*neighbors, qt::arg(L("Index%1"), index), qt::number(state.neighbors[index]));
    node.children.push_back(std::move(neighbors));
}

int toShort(const qt::String& text)
{
    bool ok = false;
    const int value = qt::toInt(text, &ok);
    return ok && value >= -32768 && value <= 32767 ? value : 0;
}

void readRecord(ViewSymmetryRecord& state, const xml::Node& node)
{
    for (const auto* child : node.childElements()) {
        if (child->tag == L("Matrix")) {
            if (!state.matrix) state.matrix.emplace();
            for (int index = 0; index < 16; ++index)
                (*state.matrix)[index] = qt::toDouble(child->attribute(qt::arg(L("P%1"), index)));
        } else if (child->tag == L("KeepOrientation")) {
            state.keepOrientation = qt::toInt(child->text()) != 0;
        } else if (child->tag == L("IsActive")) {
            state.isActive = qt::toInt(child->text()) != 0;
        } else if (child->tag == L("OglEqn")) {
            for (int index = 0; index < 4; ++index)
                state.equation[index] = qt::toDouble(child->attribute(qt::arg(L("P%1"), index)));
        } else if (child->tag == L("NeighborSymData")) {
            for (int index = 0; index < 2; ++index)
                state.neighbors[index] = qt::toInt(child->attribute(qt::arg(L("Index%1"), index)));
        }
    }
}

void readSymmetry(ViewStepDataState& state, const xml::Node& node)
{
    for (const auto* child : node.childElements()) {
        if (child->tag == L("NumOfSymmData")) {
            state.symmetryCount = qt::toInt(child->text());
        } else if (child->tag == L("Min") || child->tag == L("Max")) {
            auto& bounds = child->tag == L("Min") ? state.minimum : state.maximum;
            const char* const axis[] = {"X", "Y", "Z"};
            for (int index = 0; index < 3; ++index)
                bounds[index] = qt::toDouble(child->attribute(L(axis[index])));
        } else if (child->tag == L("NeighborSymData")) {
            for (int index = 0; index < 2; ++index)
                state.neighbors[index] = qt::toInt(child->attribute(qt::arg(L("Index%1"), index)));
        } else {
            //QString::remove removes every occurrence, not just the prefix.
            auto suffix = child->tag.u16();
            const std::u16string prefix = u"SfSymmetryData";
            if (!suffix.starts_with(prefix)) continue;
            for (auto at = suffix.find(prefix); at != std::u16string::npos; at = suffix.find(prefix))
                suffix.erase(at, prefix.size());
            const int index = qt::toInt(qt::String(std::move(suffix)));
            if (index < 0 || index >= static_cast<int>(state.symmetry.size()))
                throw Error(M2M_E_UNSUPPORTED, "DOE Object symmetry index reaches original unchecked storage");
            readRecord(state.symmetry[index], *child);
        }
    }
}
} // namespace

void writeDisplayOptionFields(xml::Node& node, std::span<const std::uint8_t, 46> flags,
                              std::int32_t mode, std::int32_t objectKind)
{
    appendText(node, "DispModeType", qt::number(mode));
    auto list = xml::createElement(L("DispFlagList"));
    const auto attribute = [&](const char* name, int value) {
        xml::setAttribute(*list, L(name), qt::number(value));
    };
    if (flags[0]) attribute("SelectionOn", flags[0]);
    if (flags[1]) attribute("ObjectOn", flags[1]);
    if (objectKind == 5) attribute("MeshOn", -1);
    else if (flags[2]) attribute("MeshOn", 1);
    if (objectKind == 5) attribute("GeoOn", -1);
    else if (flags[3]) attribute("GeoOn", 1);
    if (flags[4]) attribute("TranspOn", flags[4]);
    if (flags[5]) attribute("SliceOn", flags[5]);
    if (flags[43]) attribute("CrossSectionGeoOn", flags[43]);
    if (flags[44]) attribute("CrossSectionMeshOn", flags[44]);
    if (flags[45]) attribute("TransformCrossSectionOn", flags[45]);
    if (flags[8]) attribute("GeoNodalShadingOn", flags[8]);
    if (flags[9]) attribute("MeshNodalShadingOn", flags[9]);
    node.children.push_back(std::move(list));
}

void writeObjectSymmetry(const ViewStepDataState& state, xml::Node& object)
{
    if (state.symmetryCount == 0) return;
    auto node = xml::createElement(L("SfSymmetryInfo"));
    xml::setAttribute(*node, L("ShowSymm"), qt::number(state.showSymmetry ? 1 : 0));
    object.children.push_back(node); //before14124a890, retaining its partial prefix
    appendText(*node, "NumOfSymmData", qt::number(state.symmetryCount));
    const char* const axes[] = {"X", "Y", "Z"};
    auto minimum = xml::createElement(L("Min"));
    for (int index = 0; index < 3; ++index)
        attributeDouble(*minimum, L(axes[index]), state.minimum[index]);
    node->children.push_back(std::move(minimum));
    auto maximum = xml::createElement(L("Max"));
    for (int index = 0; index < 3; ++index)
        attributeDouble(*maximum, L(axes[index]), state.maximum[index]);
    node->children.push_back(std::move(maximum));
    auto neighbors = xml::createElement(L("NeighborSymData"));
    for (int index = 0; index < 2; ++index)
        xml::setAttribute(*neighbors, qt::arg(L("Index%1"), index), qt::number(state.neighbors[index]));
    node->children.push_back(std::move(neighbors));
    for (int index = 0; index < state.symmetryCount; ++index) {
        auto record = xml::createElement(qt::arg(L("SfSymmetryData%1"), index));
        node->children.push_back(record); //before native indexed record access
        if (static_cast<std::size_t>(index) >= state.symmetry.size())
            throw Error(M2M_E_UNSUPPORTED, "DOE Object symmetry writer reaches native invalid record index");
        writeRecord(state.symmetry[index], *record);
    }
}

void readDisplayOptionFields(const xml::Node& node, std::span<std::uint8_t, 46> flags,
                             std::int32_t& mode, std::int32_t objectKind)
{
    struct Field { int index; const char* name; };
    //Native loop visits0..45; the other35 slots retain their prior bytes.
    constexpr Field fields[] = {
        {0, "SelectionOn"}, {1, "ObjectOn"}, {2, "MeshOn"}, {3, "GeoOn"},
        {4, "TranspOn"}, {5, "SliceOn"}, {8, "GeoNodalShadingOn"},
        {9, "MeshNodalShadingOn"}, {43, "CrossSectionGeoOn"},
        {44, "CrossSectionMeshOn"}, {45, "TransformCrossSectionOn"}
    };
    for (const auto* child : node.childElements()) {
        if (child->tag == L("DispModeType")) {
            mode = toShort(child->text());
        } else if (child->tag == L("DispFlagList")) {
            for (const auto& field : fields) {
                const auto name = L(field.name);
                const bool supplied = child->hasAttribute(name);
                if (field.index == 2 || field.index == 3) {
                    if (!supplied) {
                        if (objectKind != 5) flags[field.index] = 0;
                    } else {
                        const int value = toShort(child->attribute(name));
                        if (value != -1) flags[field.index] = value != 0;
                    }
                } else {
                    flags[field.index] = 0;
                    if (supplied) flags[field.index] = toShort(child->attribute(name)) != 0;
                }
            }
        }
    }
}

void readObjectStepData(ViewStepDataState& state, const xml::Node& object,
                        const DisplayOptionsImport& importDisplayOptions,
                        const SliceImport& importSlice)
{
    SliceCoefficients coefficients; //native stack locals persist between Slice elements
    for (const auto* child : object.childElements()) {
        if (child->tag == L("SfDispOpt")) {
            if (!importDisplayOptions)
                throw Error(M2M_E_UNSUPPORTED, "DOE Object SfDispOpt settings import remains unsupported");
            importDisplayOptions(*child);
            continue;
        }
        if (child->tag == L("Slice")) {
            if (!importSlice)
                throw Error(M2M_E_UNSUPPORTED, "DOE Object Slice import remains unsupported");
            const char* const names[] = {"a", "b", "c", "d"};
            for (const auto* value : child->childElements())
                for (int index = 0; index < 4; ++index)
                    if (value->tag == L(names[index])) coefficients.values[index] = qt::toDouble(value->text());
            importSlice(coefficients);
            continue;
        }
        if (child->tag == L("SfSymmetryInfo")) {
            state.showSymmetry = qt::toInt(child->attribute(L("ShowSymm"))) != 0;
            readSymmetry(state, *child);
        }
    }
}

} // namespace m2m::doe
