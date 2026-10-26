#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m { struct Project; struct Operation; }
namespace m2m::doe {

// Class-private values not previously consumed by the recovered prefix. Unknown
// constructor contents remain unavailable until an actual source assignment.
struct OutputClassState {
    std::optional<int> objectNo;             //OB+b8
    std::optional<int> roiType;              //OB+c0, copied before Study changes
    std::optional<qt::String> meshPath;      //OB+d0
    std::optional<std::vector<qt::String>> regionNames; //OB+d8
};

// References to the actual OpClass storage, not a temporary output snapshot.
struct OutputClassRefs {
    OutputClassState& data;
    std::uint8_t (&flags98)[6];
    std::vector<std::int32_t>& listA0;
    std::vector<std::uint64_t>& listA8;
};

//e2ec40 after644440: existing selected-part SfTaskDoe only. Copies scalar list
// selections, resets the actual ROI model, finishes empty-region/constraint
// processing. Missing Task construction and populated payloads guard at encounter.
void finishCurrentOutputClassData(Project&, Operation&, const OutputClassRefs&);
} // namespace m2m::doe
