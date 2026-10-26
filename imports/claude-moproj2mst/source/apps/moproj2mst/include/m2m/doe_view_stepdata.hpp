#pragma once

#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <span>

#include "m2m/xml.hpp"

namespace m2m::doe {

struct SliceCoefficients;

// Native 2479e0 initializes these fields, but not the matrix at +40. Missing
// matrix storage is unavailable, never an inferred identity or zero matrix.
struct ViewSymmetryRecord {
    std::optional<std::array<double, 16>> matrix;
    bool keepOrientation = true;
    bool isActive = false;
    std::array<double, 4> equation{};
    std::array<int, 2> neighbors{-1, -1};
};

// A projection owned by the actual engine::View (+340, embedded +348), not by
// Session. Unconsumed native record fields remain opaque.
struct ViewStepDataState {
    bool showSymmetry = false; //1f0a50->1ffa50
    std::array<ViewSymmetryRecord, 128> symmetry;
    int symmetryCount = 0;
    std::array<double, 3> minimum{};
    std::array<double, 3> maximum{1, 1, 1};
    std::array<int, 2> neighbors{-1, -1};
};

using DisplayOptionsImport = std::function<void(const xml::Node&)>;
using SliceImport = std::function<void(const SliceCoefficients&)>;

//1411ef300: pure XML stores into the actual Settings flags/mode passed by the
// engine wrapper. Native reset/preferences and cached mesh/geo copies are caller
// work. objectKind is bound LLOBJ_DATA+0xc, not model dimension.
void readDisplayOptionFields(const xml::Node& node, std::span<std::uint8_t, 46> flags,
                             std::int32_t& mode, std::int32_t objectKind);

//1411eec70: serialize actual existing Settings fields into an already appended
//SfDispOpt. Nonzero general bytes keep their unsigned value; kind5 Mesh/Geo=-1.
void writeDisplayOptionFields(xml::Node& node, std::span<const std::uint8_t, 46> flags,
                              std::int32_t mode, std::int32_t objectKind);

//141200360->14124a890->141248030: append symmetry only when count!=0. Missing
//native matrix values/unsafe record indices guard at first use after earlier
//attached children; no identity matrix or record-count normalization.
void writeObjectSymmetry(const ViewStepDataState& state, xml::Node& object);

//141200e50->14124b080->1412488a0: ordered stores, no reset. SfDispOpt dispatches
// at encounter through the actual Settings owner, or guards if unavailable.
// Slice similarly dispatches through its actual owner or guards if unavailable;
// its initially unavailable coefficient locals persist within this Object call.
// Unsafe native record indices guard after prior effects.
// Uses the existing qualified QString number-conversion seams.
void readObjectStepData(ViewStepDataState& state, const xml::Node& object,
                        const DisplayOptionsImport& importDisplayOptions = {},
                        const SliceImport& importSlice = {});

} // namespace m2m::doe
