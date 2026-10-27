#pragma once

// Private CopyMirror seam for the actual per-View plane owner (native +0x348).
// Positive builder admission remains unsupported; this is not public admission.
#include <array>
#include <cstdint>
#include <optional>
#include <vector>

#include "m2m/mst/loader_l5.hpp"

namespace m2m::engine::detail {

struct MirrorPlaneRecordSnapshot {
    std::uint8_t mode = 1;                  // +0xf8
    bool active = false;                    // +0xf9
    std::array<double, 4> c0{};              // +0xc0..+0xd8, ctor zero
    // These native fields are not initialized by 1412479e0. Absence means
    // this projection has not observed their post-load stores.
    std::optional<std::array<double, 3>> e0;  // +0xe0/+0xe8/+0xf0
    std::optional<int> a;                    // +0x104
    std::array<int, 2> links{-1, -1};         // +0xfc/+0x100
};

struct MirrorPlaneOwnerSnapshot {
    const void* viewIdentity = nullptr;      // observation only, never dereferenced
    int allocatedSlotCount = 0;              // owner+0x8468, high-water slot extent
    std::array<int, 2> links{-1, -1};         // owner+0x846c/+0x8470
    std::array<MirrorPlaneRecordSnapshot, 128> records;
};

// 140e23490: false means viewOf returned null, so the caller stops its entry loop.
// Each entry starts at fixed record index zero, independently of prior entries.
bool importMirrorEntry(GuiModel& model, int objectNo, const std::vector<mst::MirrorPlane>& planes);
// 140c82d50 then 141249420: positive numbers lazily grow the actual View list.
int mirrorPlaneCount(GuiModel& model, int objectNo);
// Existing-owner observation only: never grows Views or fabricates Settings.
std::optional<MirrorPlaneOwnerSnapshot> mirrorPlaneOwnerSnapshot(const GuiModel& model, int objectNo);

} // namespace m2m::engine::detail
