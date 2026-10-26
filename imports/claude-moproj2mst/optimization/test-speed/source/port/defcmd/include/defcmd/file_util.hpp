// defcmd/file_util.hpp: small file helpers of the command packets (owner p8-07).
#pragma once

#include <cstdint>

namespace def::cmd {

// @src 1400b9130  write `text` (no newline added) into a new file `name` ("wt"); false when name
// is null or empty or the file cannot be opened. Used for DEFORM_UIRUNREMESH.DAT ("2D"/"3D").
bool write_text_file(const char* name, const char* text);

} // namespace def::cmd
