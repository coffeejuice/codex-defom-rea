// Isolated PPV QFile Text-mode line sink; not connected to public Save.
#pragma once

#include "m2m/mst/stream.hpp"
#include "m2m/qt4.hpp"

#include <span>
#include <string>
#include <vector>

namespace m2m::engine::simctrl_ppv {

struct TextDeviceWriteResult {
    bool openAttempted = false;
    bool opened = false;
    bool writerReturned = false;
    bool streamFailed = false;
};

// Lines are the writer's already-formatted ASCII-compatible segments, without
// CR/LF terminators. The QFile Text-mode adapter supplies CRLF; each line gets
// one QTextStream::endl flush. Successful open returns the native writer's
// true result even if a later stream/device operation fails.
TextDeviceWriteResult writePpvTextSegments(mst::OutputDevice& file,
                                           unsigned localeCodepage,
                                           std::span<const std::string> lines);

// Opens with the existing QFile WriteOnly device, then performs the same
// bounded line/endl streaming sequence. This adapter does not infer or promise
// exact file residue after a post-open failure.
TextDeviceWriteResult writePpvTextFile(const qt::String& path,
                                       unsigned localeCodepage,
                                       std::span<const std::string> lines);

} // namespace m2m::engine::simctrl_ppv
