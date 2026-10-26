#pragma once

// One exported database step: the parsed KEY document plus typed access to
// the per-object mesh and field records.

#include "deform/key_document.hpp"
#include "deform/layout.hpp"

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace dk {

struct ObjectSummary {
    int number = 0;
    std::string name;
    int type = -1;                 // OBJTYP second field
    std::size_t nodes = 0;         // RZ count
    std::size_t elements = 0;      // ELMCON count
    int nodes_per_element = 0;     // ELMCON third field
};

enum class Entity : std::uint8_t { Node, Element, Other };

// A sparse id table expanded to one row per node/element.
struct Field {
    std::string keyword;
    int object = 0;
    Entity entity = Entity::Other;
    std::size_t count = 0;         // rows (nodes, elements or max id)
    int components = 0;            // values per row
    std::size_t declared = 0;      // rows written in the KEY record
    double default_value = 0.0;    // value of rows not written
    std::vector<double> values;    // count * components, row i = id i+1
    std::vector<std::string_view> component_names; // one per column group
};

class Step {
public:
    Step() = default;
    explicit Step(KeyDocument document);

    [[nodiscard]] const KeyDocument &document() const noexcept { return doc_; }
    [[nodiscard]] std::span<const Record> records() const noexcept { return doc_.records(); }

    [[nodiscard]] std::optional<int> number() const;        // NSTART
    [[nodiscard]] std::optional<double> time() const;       // TNOW, first value
    [[nodiscard]] std::optional<int> simulation() const;    // CURSIM, first value
    [[nodiscard]] std::optional<int> mesh_number() const;   // MESHNO

    [[nodiscard]] std::vector<ObjectSummary> objects() const;

    // First record of the keyword owned by `owner` (object/material number).
    [[nodiscard]] const Record *record(std::string_view keyword,
                                       std::optional<int> owner = std::nullopt) const;

    // Layout-decoded record; nullopt if missing or not decodable.
    [[nodiscard]] std::optional<Decoded> decode(std::string_view keyword,
                                                std::optional<int> owner = std::nullopt,
                                                std::string *why = nullptr) const;

    // Node coordinates, 3 per node (RZ).
    [[nodiscard]] std::vector<double> coordinates(int object) const;

    // Element connectivity (ELMCON), `nodes_per_element` 1-based node ids per element.
    [[nodiscard]] std::vector<std::int32_t> connectivity(int object, int *nodes_per_element = nullptr) const;

    // Dense node/element field from a sparse id table such as NDTMP, STRESS,
    // STRAIN, STNCMP, DAMAGE, USRELM, DRZ, URZ.
    [[nodiscard]] std::optional<Field> field(std::string_view keyword, int object,
                                             std::string *why = nullptr) const;

private:
    KeyDocument doc_;
};

} // namespace dk
