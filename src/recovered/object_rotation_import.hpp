#pragma once

#include "object_model.hpp"

namespace deform::recovered {
//38b8a0 cases72/73: ROTSYM258 and TRGVOL259, owned by186200.
[[nodiscard]] int read_object_rotation(int keyword, ObjectDirectImports& fields,
    KeywordReader& reader);
}
