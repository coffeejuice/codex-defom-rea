#include "deformdb/model.hpp"

#include <cstring>

namespace deformdb {

namespace {

constexpr int kMarSlots = 70;
constexpr int kASlots = 140;

using K = FieldKind;
const KeywordInfo kKeywords[] = {
    // Per-node reals (A).
    {"RZ", "LLCRZ", K::NodeReal, "nodal coordinates", -1},
    {"DRZ", "LLDRZ", K::NodeReal, "nodal displacement", -1},
    {"URZ", "LLURZ", K::NodeReal, "nodal velocity", -1},
    {"FRZ", "LLPFN", K::NodeReal, "nodal force", -1},
    {"FRZB", "LLCFN", K::NodeReal, "nodal force (second set)", -1},
    {"PRZ", "LLPPR", K::NodeReal, "nodal pressure/traction (3D)", -1},
    {"PRZB", "LLCPR", K::NodeReal, "nodal pressure (second set)", -1},
    {"DRMESH", "LLDRM", K::NodeReal, "mesh displacement (3D)", -1},
    {"NDTMP", "LLTMP", K::NodeReal, "nodal temperature", 1},
    {"NDHEAT", "LLPHT", K::NodeReal, "nodal heat"},
    {"HITN", "LLHIT", K::NodeReal, "nodal heat (second set)"},
    {"NDFLUX", "LLFLX", K::NodeReal, "nodal heat flux", 1, 0},
    {"RHTFLX", "LLFLX", K::NodeReal, "radiation heat flux (LLFLX component 2 when width is 3)", 0, 1},
    {"CRBFLX", "LLFLX", K::NodeReal, "carbon flux (LLFLX component 3 when width is 3)", 0, 2},
    {"USRNOD", "LLND1", K::NodeReal, "user nodal variables"},
    {"VOTAGE", "LLVLT", K::NodeReal, "nodal voltage"},
    {"DATOM", "LLATM", K::NodeReal, "nodal atom content"},
    {"NWEAR", "LLWER", K::NodeReal, "nodal wear"},
    {"DTMP", "LLDTM", K::NodeReal, "temperature increment (2D)"},
    {"TIMERC", "LLTRC", K::NodeReal, "nodal timer"},
    {"NRECVL", "LLNRV", K::NodeReal, "nodal recovery values"},
    // Per-node ints (MAR).
    {"BCCDEF", "IIBCD", K::NodeInt, "deformation BC code per dof", -1},
    {"BCCDFN", "IIBFD", K::NodeInt, "deformation BC function per dof", -1},
    {"BCCTMP", "IIBCT", K::NodeInt, "thermal BC code", 1, 0},
    {"BCCRHT", "IIBCT", K::NodeInt, "radiation BC code (IIBCT component 2 when width is 3)", 0, 1},
    {"BCCCRB", "IIBCT", K::NodeInt, "carbon BC code (IIBCT component 3 when width is 3)", 0, 2},
    {"BCCTFN", "IIBFT", K::NodeInt, "thermal BC function", 1},
    // Per-element reals (A).
    {"STRESS", "LLSTS", K::ElemReal, "stress components (points x 2*dim)"},
    {"EPS", "LLEPS", K::ElemReal, "strain-rate components"},
    {"STNCMP", "LLTSR", K::ElemReal, "strain components"},
    {"EFSTS", "LLFTS", K::ElemReal, "effective stress"},
    {"EFEPS", "LLFEP", K::ElemReal, "effective strain rate"},
    {"STRAIN", "LLTEP", K::ElemReal, "effective plastic strain"},
    {"DENSTY", "LLRDT", K::ElemReal, "relative density"},
    {"HTGN", "LLHGN", K::NodeReal, "heat generation (region sized NUMNP)"},
    {"DAMAGE", "LLDMG", K::ElemReal, "damage"},
    {"MATAXI", "LLMTA", K::ElemReal, "material axes (3D, 9 values)"},
    {"USRELM", "LLET1", K::ElemReal, "user element variables"},
    {"VOLFC", "LLVLF", K::ElemReal, "phase volume fraction, one column per material"},
    {"DVOLF", "LLDVF", K::ElemReal, "phase volume fraction increment, per relation"},
    {"VOLFS", "LLVLN", K::ElemReal, "phase volume fraction (second set)"},
    {"HDNOBJ", "LLHDN", K::ElemReal, "hardness"},
    {"TICF", "LLICB", K::ElemReal, "transformation incubation, per relation"},
    {"GRAIN", "LLGSZ", K::ElemReal, "grain size (element; nodal when ELMNOD grain option is 1)"},
    {"SIZESH", "LLSIZ", K::ElemReal, "element size/shape (v >= 11000)"},
    {"YLDS", "LLYLS", K::ElemReal, "yield stress components"},
    {"ZEFI", "LLZEI", K::ElemReal, "ZEFI (2 values)"},
    {"CURRNT", "LLCUR", K::ElemReal, "current density"},
    {"ZMFI", "LLZMI", K::ElemReal, "ZMFI"},
    {"TXTODF", "LLODF", K::ElemReal, "texture ODF"},
    {"ERECVL", "LLERV", K::ElemReal, "element recovery values"},
    // Per-element ints (MAR).
    {"MTLGRP", "IIMTR", K::ElemInt, "material group", 1},
};

std::optional<int> index_in(std::span<const char* const> names, std::string_view region) {
    for (std::size_t k = 0; k < names.size(); ++k)
        if (region == names[k]) return static_cast<int>(k + 1);
    return std::nullopt;
}

}  // namespace

std::span<const KeywordInfo> keyword_table() { return kKeywords; }

const KeywordInfo* find_keyword(std::string_view keyword) {
    for (const auto& k : kKeywords)
        if (keyword == k.keyword) return &k;
    return nullptr;
}

StepModel::StepModel(const StepData& step) : s_(&step) {
    const auto& e = step.entry;
    if (e.version < 11000 || step.mar.empty() || step.a.empty()) return;
    auto iad1 = step.mar_region("IIAD1");
    auto iad2 = step.mar_region("IIAD2");
    if (iad1.size() < 2 * kMarSlots || iad2.size() < 2 * kASlots) return;
    mar_tab_ = iad1.first(2 * kMarSlots);
    a_tab_ = iad2.first(2 * kASlots);
    supported_ = true;

    const bool d3 = e.dim == 3;
    auto sim = step.mar_region("IISIM");
    auto at = [&](std::size_t k) -> std::int32_t { return k < sim.size() ? sim[k] : 0; };
    meshno_ = at(d3 ? 1 : 3);
    nstart_ = at(d3 ? 2 : 4);
    numnp_ = at(d3 ? 19 : 26);
    numel_ = at(d3 ? 20 : 27);
    for (std::size_t k = d3 ? 57 : 59, n = 0; n < 10; ++k, ++n) elmnod_.push_back(at(k));
    auto llsim = step.a_region("LLSIM");
    if (!llsim.empty()) time_ = llsim[0];

    int nobj = step.header.empty() ? 0 : static_cast<int>(step.header[0]);
    if (nobj <= 0) nobj = at(d3 ? 23 : 30);

    const auto io_idx = mar_index("IIOBJ");
    const auto lo_idx = a_index("LLOBJ");
    if (!io_idx) return;
    const std::int32_t io_start = mar_start(*io_idx), io_w = mar_width(*io_idx);
    for (int k = 0; k < nobj; ++k) {
        const std::size_t base = static_cast<std::size_t>(io_start - 1 + k * io_w);
        if (io_w < 8 || base + static_cast<std::size_t>(io_w) > step.mar.size()) break;
        const std::int32_t* r = step.mar.data() + base;
        ObjectInfo o;
        o.number = k + 1;
        o.status = r[0];
        o.deform_code = r[1];
        o.porous = r[2] == 1;
        if (o.status == 1) {
            o.first_node = r[4];
            o.node_count = r[5] >= r[4] && r[4] > 0 ? r[5] - r[4] + 1 : 0;
            o.first_elem = r[6];
            o.elem_count = r[7] >= r[6] && r[6] > 0 ? r[7] - r[6] + 1 : 0;
        }
        if (d3) {
            o.nodes_per_elem = (io_w > 12 && r[12] == 8) ? 8 : 4;
            o.conn_offset = io_w > 13 ? r[13] : 0;
        } else {
            o.nodes_per_elem = 4;
        }
        if (lo_idx) {
            const std::int32_t w = a_width(*lo_idx);
            const std::size_t b = static_cast<std::size_t>(a_start(*lo_idx) - 1 + k * w);
            if (w >= 12 && b + 12 <= step.a.size()) {
                // Text in real words: the bytes of the decoded (host-order) double are the characters.
                char buf[64];
                std::memcpy(buf, step.a.data() + b + 4, 64);
                std::string name(buf, 64);
                while (!name.empty() && (name.back() == ' ' || name.back() == '\0')) name.pop_back();
                o.name = std::move(name);
            }
        }
        objects_.push_back(std::move(o));
    }
}

std::optional<int> StepModel::mar_index(std::string_view region) const {
    return index_in(mar_region_names(s_->entry.dim, s_->entry.version), region);
}
std::optional<int> StepModel::a_index(std::string_view region) const {
    return index_in(a_region_names(s_->entry.dim, s_->entry.version), region);
}

std::int32_t StepModel::mar_start(int i) const { return supported_ && i >= 1 && i <= kMarSlots ? mar_tab_[i - 1] : 0; }
std::int32_t StepModel::mar_width(int i) const { return supported_ && i >= 1 && i <= kMarSlots ? mar_tab_[kMarSlots + i - 1] : 0; }
std::int32_t StepModel::a_start(int i) const { return supported_ && i >= 1 && i <= kASlots ? a_tab_[i - 1] : 0; }
std::int32_t StepModel::a_width(int i) const { return supported_ && i >= 1 && i <= kASlots ? a_tab_[kASlots + i - 1] : 0; }

namespace {

// `limit` = decoded region length from the step's region table (-1 = unknown).
template <class T>
FieldView<T> rows_of(const std::vector<T>& arr, std::int32_t start, std::int32_t width, std::int64_t first_row,
                     std::int64_t rows, std::int64_t limit) {
    FieldView<T> v;
    if (start < 1 || width <= 0 || first_row < 1 || rows <= 0) return v;
    const std::int64_t rel = (first_row - 1) * width;
    if (limit >= 0 && rel + rows * width > limit) return v;
    const std::int64_t off = start - 1 + rel;
    if (off < 0 || static_cast<std::size_t>(off + rows * width) > arr.size()) return v;
    v.data = arr.data() + off;
    v.rows = static_cast<std::size_t>(rows);
    v.width = width;
    v.row_stride = static_cast<std::size_t>(width);
    return v;
}

std::int64_t region_limit(const std::vector<Region>& regions, int index) {
    if (index >= 1 && static_cast<std::size_t>(index) <= regions.size()) return regions[static_cast<std::size_t>(index - 1)].length;
    return -1;
}

}  // namespace

FieldView<double> StepModel::node_real(const ObjectInfo& o, std::string_view region, int width_if_zero) const {
    auto i = a_index(region);
    if (!i) return {};
    const int w = a_width(*i) > 0 ? a_width(*i) : width_if_zero;
    return rows_of(s_->a, a_start(*i), w, o.first_node, o.node_count, region_limit(s_->a_regions, *i));
}

FieldView<double> StepModel::elem_real(const ObjectInfo& o, std::string_view region, int width_if_zero) const {
    auto i = a_index(region);
    if (!i) return {};
    const int w = a_width(*i) > 0 ? a_width(*i) : width_if_zero;
    return rows_of(s_->a, a_start(*i), w, o.first_elem, o.elem_count, region_limit(s_->a_regions, *i));
}

FieldView<std::int32_t> StepModel::node_int(const ObjectInfo& o, std::string_view region, int width_if_zero) const {
    auto i = mar_index(region);
    if (!i) return {};
    const int w = mar_width(*i) > 0 ? mar_width(*i) : width_if_zero;
    return rows_of(s_->mar, mar_start(*i), w, o.first_node, o.node_count, region_limit(s_->mar_regions, *i));
}

FieldView<std::int32_t> StepModel::elem_int(const ObjectInfo& o, std::string_view region, int width_if_zero) const {
    auto i = mar_index(region);
    if (!i) return {};
    const int w = mar_width(*i) > 0 ? mar_width(*i) : width_if_zero;
    return rows_of(s_->mar, mar_start(*i), w, o.first_elem, o.elem_count, region_limit(s_->mar_regions, *i));
}

FieldView<double> StepModel::node_real(const ObjectInfo& o, std::string_view region) const { return node_real(o, region, 0); }
FieldView<double> StepModel::elem_real(const ObjectInfo& o, std::string_view region) const { return elem_real(o, region, 0); }
FieldView<std::int32_t> StepModel::node_int(const ObjectInfo& o, std::string_view region) const { return node_int(o, region, 0); }
FieldView<std::int32_t> StepModel::elem_int(const ObjectInfo& o, std::string_view region) const { return elem_int(o, region, 0); }

FieldView<double> StepModel::elem_real_nodal(const ObjectInfo& o, std::string_view region) const {
    auto i = a_index(region);
    if (!i) return {};
    return rows_of(s_->a, a_start(*i), a_width(*i), static_cast<std::int64_t>(numel_) + o.first_node, o.node_count,
                   region_limit(s_->a_regions, *i));
}

namespace {

// One component of a per-node region stored component-major (width 3): component c
// of global node g sits at start + c*NUMNP + (g-1).
template <class T>
FieldView<T> component_major(const std::vector<T>& arr, std::int32_t start, std::int32_t numnp, int comp,
                             const ObjectInfo& o) {
    FieldView<T> v;
    const std::int64_t off = static_cast<std::int64_t>(start) - 1 + static_cast<std::int64_t>(comp) * numnp + (o.first_node - 1);
    if (start < 1 || o.node_count <= 0 || off < 0 || static_cast<std::size_t>(off + o.node_count) > arr.size()) return v;
    v.data = arr.data() + off;
    v.rows = static_cast<std::size_t>(o.node_count);
    v.width = 1;
    v.row_stride = 1;
    return v;
}

}  // namespace

FieldView<double> StepModel::real_keyword(const ObjectInfo& o, std::string_view keyword) const {
    const auto* k = find_keyword(keyword);
    if (!k) return {};
    const int dw = k->default_width < 0 ? dim() : k->default_width;
    if (k->component >= 0) {
        auto i = a_index(k->region);
        if (!i) return {};
        if (a_width(*i) == 3) return component_major(s_->a, a_start(*i), numnp_, k->component, o);
        return k->component == 0 ? node_real(o, k->region, dw) : FieldView<double>{};
    }
    if (k->kind == FieldKind::NodeReal) return node_real(o, k->region, dw);
    if (k->kind == FieldKind::ElemReal) return elem_real(o, k->region, dw);
    return {};
}

FieldView<std::int32_t> StepModel::int_keyword(const ObjectInfo& o, std::string_view keyword) const {
    const auto* k = find_keyword(keyword);
    if (!k) return {};
    const int dw = k->default_width < 0 ? dim() : k->default_width;
    if (k->component >= 0) {
        auto i = mar_index(k->region);
        if (!i) return {};
        if (mar_width(*i) == 3) return component_major(s_->mar, mar_start(*i), numnp_, k->component, o);
        return k->component == 0 ? node_int(o, k->region, dw) : FieldView<std::int32_t>{};
    }
    if (k->kind == FieldKind::NodeInt) return node_int(o, k->region, dw);
    if (k->kind == FieldKind::ElemInt) return elem_int(o, k->region, dw);
    return {};
}

std::vector<std::int32_t> StepModel::connectivity(const ObjectInfo& o) const {
    std::vector<std::int32_t> out;
    auto i = mar_index("IINOD");
    if (!i || o.elem_count <= 0) return out;
    const std::int64_t n = static_cast<std::int64_t>(o.elem_count) * o.nodes_per_elem;
    const std::int64_t off = (dim() == 3) ? mar_start(*i) - 1 + (o.conn_offset - 1)
                                          : mar_start(*i) - 1 + static_cast<std::int64_t>(o.first_elem - 1) * 4;
    if (off < 0 || static_cast<std::size_t>(off + n) > s_->mar.size()) return out;
    out.assign(s_->mar.begin() + off, s_->mar.begin() + off + n);
    for (auto& v : out) v -= o.first_node - 1;
    return out;
}

}  // namespace deformdb
