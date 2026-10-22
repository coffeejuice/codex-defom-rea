#pragma once

#include "object_model.hpp"

namespace deform::recovered {
[[nodiscard]] int read_object_output_range(ObjectOutputRange& fields, KeywordReader& reader);
}
