#pragma once
#include "object_model.hpp"
#include "keyword_reader.hpp"

namespace deform::recovered {
//384a50 case105 after common material selection/creation. Immediate mutations
//preserve original header/payload failure ordering; numerical evaluation is separate.
[[nodiscard]] int read_frcmod(FrcmodProperties& properties, KeywordReader& reader);
}
