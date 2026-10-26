#include "defmodel/options/option_tables.hpp"

namespace def::model::options {

std::vector<std::string> option_strings(const OptionTable& table)
{
    std::vector<std::string> out;
    out.reserve(table.labels.size());
    for (const OptionLabel& l : table.labels) out.emplace_back(l.text);
    return out;
}

}  // namespace def::model::options
