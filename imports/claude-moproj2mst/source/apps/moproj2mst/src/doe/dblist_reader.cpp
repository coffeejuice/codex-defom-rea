#include "doe/dblist_reader.hpp"

namespace m2m::doe {

std::vector<DbListChildValue> readDbListDirectChildren(const xml::Node& dbList)
{
    std::vector<DbListChildValue> values;
    for (const auto* child : dbList.childElements())
        values.push_back({child->tag, child->text()});
    return values;
}

} // namespace m2m::doe
