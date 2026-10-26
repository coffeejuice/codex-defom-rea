// Isolated PPV payload-to-Text-device writer; not public Save wiring.
#pragma once

#include "m2m/engine/simctrl_ppv_reader.hpp"
#include "m2m/mst/stream.hpp"
#include "m2m/qt4.hpp"

#include <vector>

namespace m2m::engine::simctrl_ppv {

enum class TextWriterStatus {
    LocalListEmpty,
    CodecUnqualified,
    CountNotRepresentable,
    FlagsTooShort,
    OpenFailed,
    Returned,
};

struct TextWriterResult {
    TextWriterStatus status = TextWriterStatus::LocalListEmpty;
    bool openAttempted = false;
    bool opened = false;
    bool writerReturned = false;
    bool streamFailed = false;
};

// For deterministic fault tests and device injection. The caller has already
// opened the QFile-like device; local must be nonempty and codec must be known
// ASCII-compatible. Successful-open writer return ignores later stream errors.
TextWriterResult writePpvTextWriterAfterOpen(
    mst::OutputDevice& file, unsigned localeCodepage,
    const std::vector<Point>& local, const std::vector<Point>* shared,
    const std::vector<bool>* flags);

// Models native local-list gate before QFile construction/open. `asciiCompatible`
// qualifies the caller's supported output-codec profile; it is not an inferred
// original locale code page.
TextWriterResult writePpvTextWriterFile(
    const qt::String& path, unsigned localeCodepage, bool asciiCompatible,
    const std::vector<Point>& local, const std::vector<Point>* shared,
    const std::vector<bool>* flags);

} // namespace m2m::engine::simctrl_ppv
