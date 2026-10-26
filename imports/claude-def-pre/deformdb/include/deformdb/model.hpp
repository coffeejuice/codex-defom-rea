// Typed view of one decoded step: objects, nodes, elements and keyword fields.
// Layout rules: docs/db-format-spec.md sections 6-9. Supports step version >= 11000
// (address tables in IIAD1/IIAD2); older layouts expose raw arrays only.
#pragma once

#include <cstdint>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "deformdb/database.hpp"

namespace deformdb {

enum class FieldKind { NodeReal, NodeInt, ElemReal, ElemInt };

// Static description of a keyword stored in a MAR/A region.
struct KeywordInfo {
    const char* keyword;   // DEFORM keyword (RZ, STRESS, ...)
    const char* region;    // address name of the region holding it
    FieldKind kind;
    const char* meaning;
    int default_width = 0;   // width used when the address table stores 0 (-1 = dimension)
    int component = -1;      // >= 0: one component of a region stored component-major when its width is 3
};
std::span<const KeywordInfo> keyword_table();
const KeywordInfo* find_keyword(std::string_view keyword);

// Rows of a per-node or per-element field for one object: row r, component c
// is data[r * row_stride + c * comp_stride] (row-major: row_stride = width, comp_stride = 1).
template <class T>
struct FieldView {
    const T* data = nullptr;
    std::size_t rows = 0;
    int width = 0;
    std::size_t row_stride = 0;
    std::size_t comp_stride = 1;
    explicit operator bool() const noexcept { return data != nullptr && rows > 0 && width > 0; }
    T at(std::size_t row, int comp) const { return data[row * row_stride + static_cast<std::size_t>(comp) * comp_stride]; }
};

struct ObjectInfo {
    int number = 0;              // 1-based object number
    int status = 0;              // IIOBJ[0]: 0 empty, 1 meshed, 2 no mesh
    int deform_code = 0;         // IIOBJ[1]: 0 rigid, 1 plastic, 2 elastic, 3 elasto-plastic, 4 user
    bool porous = false;         // IIOBJ[2] == 1
    std::int32_t first_node = 0; // global, 1-based
    std::int32_t node_count = 0;
    std::int32_t first_elem = 0; // global, 1-based
    std::int32_t elem_count = 0;
    int nodes_per_elem = 0;      // 4 tet / 8 brick (3D), 4 quad (2D)
    std::int32_t conn_offset = 0;// 3D: 1-based offset of the object's connectivity in IINOD
    std::string name;            // OBJNAM, from LLOBJ doubles 4..11
};

class StepModel {
public:
    explicit StepModel(const StepData& step);

    const StepData& step() const noexcept { return *s_; }
    bool supported() const noexcept { return supported_; }
    int dim() const noexcept { return s_->entry.dim; }

    // Globals.
    int object_count() const noexcept { return static_cast<int>(objects_.size()); }
    std::int32_t numnp() const noexcept { return numnp_; }
    std::int32_t numel() const noexcept { return numel_; }
    std::int32_t nstart() const noexcept { return nstart_; }
    std::int32_t meshno() const noexcept { return meshno_; }
    double time() const noexcept { return time_; }
    std::span<const std::int32_t> elmnod() const noexcept { return elmnod_; }

    const std::vector<ObjectInfo>& objects() const noexcept { return objects_; }

    // Region placement from the address tables (index = 1-based region number).
    std::optional<int> mar_index(std::string_view region) const;
    std::optional<int> a_index(std::string_view region) const;
    std::int32_t mar_start(int index) const;
    std::int32_t mar_width(int index) const;
    std::int32_t a_start(int index) const;
    std::int32_t a_width(int index) const;

    // Field rows for one object, by region name.
    FieldView<double> node_real(const ObjectInfo& o, std::string_view region) const;
    FieldView<double> elem_real(const ObjectInfo& o, std::string_view region) const;
    FieldView<std::int32_t> node_int(const ObjectInfo& o, std::string_view region) const;
    FieldView<std::int32_t> elem_int(const ObjectInfo& o, std::string_view region) const;
    // Explicit width for regions whose address-table width is 0 (DEFORM then uses its default).
    FieldView<double> node_real(const ObjectInfo& o, std::string_view region, int width_if_zero) const;
    FieldView<double> elem_real(const ObjectInfo& o, std::string_view region, int width_if_zero) const;
    FieldView<std::int32_t> node_int(const ObjectInfo& o, std::string_view region, int width_if_zero) const;
    FieldView<std::int32_t> elem_int(const ObjectInfo& o, std::string_view region, int width_if_zero) const;
    // Nodal block of an element field written with ELMNOD option 2 (after NUMEL element rows).
    FieldView<double> elem_real_nodal(const ObjectInfo& o, std::string_view region) const;

    // Keyword access: dispatches through keyword_table().
    FieldView<double> real_keyword(const ObjectInfo& o, std::string_view keyword) const;
    FieldView<std::int32_t> int_keyword(const ObjectInfo& o, std::string_view keyword) const;

    // Element connectivity as LOCAL 1-based node numbers, nodes_per_elem per element.
    std::vector<std::int32_t> connectivity(const ObjectInfo& o) const;

private:
    const StepData* s_;
    bool supported_ = false;
    std::span<const std::int32_t> mar_tab_;  // 140: starts[70], widths[70]
    std::span<const std::int32_t> a_tab_;    // 280: starts[140], widths[140]
    std::int32_t numnp_ = 0, numel_ = 0, nstart_ = 0, meshno_ = 0;
    double time_ = 0.0;
    std::vector<std::int32_t> elmnod_;
    std::vector<ObjectInfo> objects_;
};

}  // namespace deformdb
