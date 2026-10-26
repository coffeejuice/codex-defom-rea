#pragma once
// Static option-label tables of DEF_PRE_64.exe (packet p1-02): the 38 initialisers at
// 0x140501bd0-0x1405036a0, as constexpr data. Two identical sets: g_<name> (0x1405be8c0..) and
// g_<name>_b (0x1405c0ce0..), one per translation unit that included the original header.

#include "defmodel/options/option_table_types.hpp"
#include "defmodel/options/option_tables_data.hpp"

#include <string>
#include <vector>

namespace def::model::options {

// The table whose initialiser or first element is at `addr`, or nullptr.
constexpr const OptionTable* find_option_table(std::uint64_t addr)
{
    for (const OptionTable& t : kOptionTables)
        if (t.init == addr || t.data == addr) return &t;
    return nullptr;
}

// The labels as std::string values, as the original's std::string array holds them.
std::vector<std::string> option_strings(const OptionTable& table);

}  // namespace def::model::options
