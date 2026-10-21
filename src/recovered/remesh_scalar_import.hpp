#pragma once

#include "object_model.hpp"

namespace deform::recovered {
[[nodiscard]] bool remesh_scalar_keyword(int keyword) noexcept;
//38b8a0's ten unconditional414990 direct reads, including TMPLMT202.
[[nodiscard]] int read_remesh_scalar(int keyword, ObjectRecord& object, KeywordReader& reader);
}
