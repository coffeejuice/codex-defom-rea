#pragma once
#include "object_model.hpp"
#include "keyword_reader.hpp"

namespace deform::recovered {
[[nodiscard]] bool hardening_keyword(int keyword) noexcept;
//384a50 cases109/114/117..120 after common material selection/creation.
//UTSDAT uses the existing FRCMOD-owned table; GRNMDL has no stored output.
[[nodiscard]] int read_hardening(int keyword, MaterialRecord& material, KeywordReader& reader);
}
