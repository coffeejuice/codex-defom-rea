#pragma once

#include "m2m/doe_study.hpp"

namespace m2m::doe {
struct InputTableNotifierContext;

// 0x14008f530 Type2/SfDoeStudy child: name, InputTable, Samples0/1/2
// (the original writes rather than reads these), then current DEF_DOE.PAR.
// Helper false results are ignored, preserving effects/state in order. Unsupported
// branches still refuse at encounter, without rolling back earlier native effects.
// This is lazy study parsing only, not selected DoeOutput/session/model activation.
void readLazyStudy(Study& study, const xml::Node& element, const String& studiesDir, StudyIo& io);
void readLazyStudy(Study& study, const xml::Node& element, const String& studiesDir, StudyIo& io,
                   const InputTableNotifierContext* notifierContext);

// 0x1400d7820 -> 9156e0/9157c0. Failed open leaves manager mode/tables unchanged;
// successful empty input commits an empty valid current table. ASCII QTextStream
// input is supported here; non-ASCII/NUL/BOM refuses after open pending codecs.
bool loadCurrentSamples(Study& study, const String& controlDir, StudyIo& io);

}  // namespace m2m::doe
