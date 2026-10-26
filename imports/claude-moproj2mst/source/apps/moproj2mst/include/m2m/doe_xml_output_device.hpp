#pragma once

#include <functional>
#include <memory>

#include "m2m/mst/stream.hpp"

namespace m2m::doe {

// QFile error is independent of QTextStream status and is queried after close.
class StudyXmlFile : public mst::OutputDevice {
public:
    virtual int error() const = 0;
};

struct StudyXmlObserver {
    std::function<void(std::string_view, std::int64_t)> write;
    std::function<void(bool)> flush;
    std::function<void()> closing, closed;
    std::function<void(int)> error;
};

// Binary owner supplied to the adapter; aboutToClose is forwarded from that
// owner's close, after its pre-close flush, never emitted prematurely here.
std::unique_ptr<StudyXmlFile> textModeStudyXmlFile(std::unique_ptr<StudyXmlFile> binary);
std::unique_ptr<StudyXmlFile> observeStudyXmlFile(std::unique_ptr<StudyXmlFile>, StudyXmlObserver);
std::unique_ptr<StudyXmlFile> nativeStudyXmlFile(const qt::String& path);
// Already-observed simulated successful open. Caller truncates its map first;
// every raw byte write becomes visible immediately through this callback.
std::unique_ptr<StudyXmlFile> recordingStudyXmlFile(std::function<void(std::string_view)> append);

} // namespace m2m::doe
