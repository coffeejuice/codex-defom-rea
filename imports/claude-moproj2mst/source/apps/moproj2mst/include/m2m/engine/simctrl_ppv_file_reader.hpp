// Bounded raw-file adapter for the isolated SimCtrl.PPV line parser.
#pragma once

#include "m2m/model.hpp"

#include <vector>

namespace m2m::engine::simctrl_ppv {

enum class FileReadStatus {
    OpenFailed,
    Read,
    UnsupportedEncodingOrLineEnding,
};

struct FileLines {
    FileReadStatus status = FileReadStatus::OpenFailed;
    std::vector<qt::String> lines;
};

// Native source qualifies QFile(ReadOnly) followed by default QTextStream
// readLine calls. This adapter supports BOM-free ASCII, UTF-8 BOM plus ASCII
// payload, and UTF-16LE with BOM, with LF/CRLF line endings. Other codecs,
// non-ASCII UTF-8, and malformed encodings remain guarded. A CR is removed
// only as part of CRLF; other CR characters are preserved in the line.
FileLines readPpvFileLines(const Project& project, const qt::String& path);

// Physical QFile-like read for a sidecar path. Unlike readPpvFileLines, this
// entry does not consult Project::overlay; it reuses the same bounded decoder.
FileLines readPpvFileLinesFromDisk(const qt::String& path);

} // namespace m2m::engine::simctrl_ppv
