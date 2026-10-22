#pragma once

#include "object_model.hpp"

namespace deform::recovered {
[[nodiscard]] bool object_integer_keyword(int keyword) noexcept;
[[nodiscard]] int read_object_integer(int keyword, ObjectDirectImports& fields,
    KeywordReader& reader);
}
