#include "type3_region_value.hpp"

#include <string>
#include <utility>

namespace m2m::mst::detail {

qt::String deriveType3RegionValue(const qt::String& rowText)
{
    std::u16string value = rowText.u16();
    bool changed = false;

    const std::size_t hash = value.find(u'#');
    if (hash != std::u16string::npos) {
        value.erase(0, hash + 1);
        changed = true;
    }

    const std::size_t forwardSlash = value.rfind(u'/');
    if (forwardSlash != std::u16string::npos) {
        value.erase(0, forwardSlash + 1);
        changed = true;
    }

    const std::size_t backslash = value.rfind(u'\\');
    if (backslash != std::u16string::npos) {
        value.erase(0, backslash + 1);
        changed = true;
    }

    const std::size_t dot = value.rfind(u'.');
    if (dot != std::u16string::npos) {
        value.resize(dot);
        changed = true;
    }

    // Preserve QString's null/empty distinction on the no-op path. Any actual
    // remove/truncate operation yields a non-null QString, including empty.
    return changed ? qt::String(std::move(value)) : rowText;
}

}  // namespace m2m::mst::detail
