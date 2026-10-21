#pragma once
#include "object_model.hpp"
#include "keyword_reader.hpp"

namespace deform::recovered {
struct GrndatReadResult { int status; bool refresh; };
// 384a50 case108 under the established root864=0 profile. Status2 keeps
// prior stores/created nodes; refresh is deliberately independent of success.
[[nodiscard]] GrndatReadResult read_grndat(MaterialRecord& material, KeywordReader& reader);
}
