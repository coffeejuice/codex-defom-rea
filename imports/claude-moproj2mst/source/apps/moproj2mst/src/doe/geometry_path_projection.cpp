#include "geometry_path_projection.hpp"

#include <string>

#include "m2m/common.hpp"
#include "m2m/qt4path.hpp"
#include "m2m/xml.hpp"

namespace m2m::doe {
namespace {
[[noreturn]] void refuse(const char* reason)
{
    throw Error(M2M_E_UNSUPPORTED, std::string("Geometry path value projection: ") + reason);
}

bool absoluteFilenameProfile(const std::u16string& text)
{
    if (text.empty() || text.back() == u'/' || text.find(u'\0') != std::u16string::npos) return false;
    const auto letter = [](char16_t ch) { return (ch >= u'A' && ch <= u'Z') || (ch >= u'a' && ch <= u'z'); };
    if (text.size() > 3 && letter(text[0]) && text[1] == u':' && text[2] == u'/') return true;
    if (text.size() < 5 || text[0] != u'/' || text[1] != u'/') return false;
    const auto hostEnd = text.find(u'/', 2);
    if (hostEnd == std::u16string::npos || hostEnd == 2) return false;
    const auto host = text.substr(2, hostEnd - 2);
    if (host == u"?" || host == u".") return false; // Device namespaces are outside this UNC profile.
    const auto shareEnd = text.find(u'/', hostEnd + 1);
    return shareEnd != std::u16string::npos && shareEnd > hostEnd + 1 && shareEnd + 1 < text.size();
}
} // namespace

GeometryPathValues projectGeometryPathValues(const InputTableSource& source, const xml::Node& geometry)
{
    using qt::L;
    if (geometry.kind != xml::Node::Element || geometry.tag != L("Geometry"))
        refuse("expected an exact Geometry element");
    // The native entry caches a signed16 last-separator index. Bound this
    // value projection rather than inventing overflow/default-source behavior.
    if (source.fileName.u16().size() >= 32768) refuse("filename exceeds the admitted UTF-16 length");
    const auto normalized = qt::fromNativeSeparators(source.fileName);
    if (!absoluteFilenameProfile(normalized.u16())) refuse("requires a named absolute drive/UNC filename");

    // Normal QFileInfo(QString) clears private+B4.bit0; path() then reads the
    // entry directly. These filename profiles skip resolver search rewrites.
    // This value calculation does not execute/admit that native construction.
    const auto parent = qt::path(normalized);
    const auto join = [&](const char* name) { return parent + L("/") + geometry.attribute(L(name)); };
    return {source.fileName, parent, join("UpperGeoFile"), join("LowerGeoFile"), join("NominalGeoFile")};
}
} // namespace m2m::doe
