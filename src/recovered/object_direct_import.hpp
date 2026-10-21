#pragma once

#include "object_model.hpp"

namespace deform::recovered {
//38b8a0 cases2..6, including the dimension-gated ZSTR/PENVOL branches.
[[nodiscard]] int read_object_direct(int keyword, ObjectDirectImports& fields,
    KeywordReader& reader);
}
