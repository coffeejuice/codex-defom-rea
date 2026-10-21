#pragma once

#include "keyword_reader.hpp"
#include "object_model.hpp"

namespace deform::recovered {

// 384a50 case2, after exact material selection and creation. All mode branches
// are represented; unsafe extents and uninitialized native data remain outside
// the checked reader domain. This imports state, not stress-law evaluation.
[[nodiscard]] int read_fstres(FstresProperties& properties, KeywordReader& reader);

} // namespace deform::recovered
