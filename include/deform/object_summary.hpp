#pragma once

#include "deform/database.hpp"

namespace deform {

// A bounded reconstruction of ReadObjInfo, not the complete application object.
struct object_summary {
    std::int32_t id{}; // One-based slot assigned by the original importer.
    bool active{};
    bool has_mesh{};
    std::int32_t raw_type{}; // Descriptor word 1; not a KEY OBJTYP code.
    std::int32_t internal_type{}; // Original application enum, deliberately unnamed.
    std::int32_t node_count{};
    std::int32_t element_count{};
    std::string name; // Original character bytes; encoding has not been established.
    std::size_t integer_descriptor_index{}; // Zero-based decoded-array positions.
    std::size_t real_descriptor_index{};
};

// Supports the traced full-array 3D v11000 layout. Other versions fail explicitly.
// Inactive slots are reported with active=false; name/counts are not interpreted.
[[nodiscard]] std::vector<object_summary> summarize_objects(const loaded_step& step);

} // namespace deform
