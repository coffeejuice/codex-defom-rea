#pragma once

#include <string>
#include <optional>
#include <memory>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m {
struct Project;
struct SubProject;
namespace engine { class ConversionSession; }
namespace xml { struct Node; }
}
namespace m2m::doe {
struct Study;
class StudyIo;
struct DeferredVariableLink;

// Borrowed synchronous call context from the actual Type2 parse caller. Never
// retain it on Study/I/O or use it after that caller's Session lifetime ends.
struct InputTableNotifierContext {
    engine::ConversionSession* session = nullptr;
    Project* project = nullptr;
    SubProject* sub = nullptr;
    Study* study = nullptr;
    StudyIo* io = nullptr;
    std::weak_ptr<StudyIo> ioOwner; // Identity only; does not keep a replaced I/O alive.
};

// Internal per-invocation filename VALUE from the successful input read.
// No native QString data/ref, QFile/QFileInfo or geometry capability is implied.
struct InputTableSource {
    qt::String fileName;
    std::optional<InputTableNotifierContext> notifier;
    // Exact StudyIo used by the lazy caller to read these bytes. The notifier
    // entry independently compares it with the caller-retained live context.
    StudyIo* readerIo = nullptr;
};

// Own the exact caller value through parsing/unwinding. Bytes-only public
// callers keep the separate source-less entry; an empty filename is not absence.
// Current Geometry still refuses before consuming this context as QFileInfo.
bool readInputTableFromSource(const std::string& bytes, Study&, InputTableSource source);
// Internal contextual entry; the public Header helper retains its original
// source-less signature and behavior.
bool appendBoundedHeaderVariableFromSource(Study&, const xml::Node&,
    std::vector<DeferredVariableLink>&, const InputTableSource*);
} // namespace m2m::doe
