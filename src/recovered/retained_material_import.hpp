#pragma once

#include "property_table.hpp"

namespace deform::recovered {
[[nodiscard]] bool retained_material_keyword(int keyword) noexcept;
//384a50 cases12..15 mutate existing tables without reset. Import only.
[[nodiscard]] int read_retained_material(int keyword, PropertyTable& table,
    KeywordReader& reader);
}
