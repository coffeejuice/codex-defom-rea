#include "deform/step.hpp"

#include <algorithm>
#include <limits>
#include <set>

namespace dk {
namespace {

bool is_node_column(std::string_view n) noexcept
{
    return n == "node" || n == "nd";
}

bool is_element_column(std::string_view n) noexcept
{
    return n == "elem" || n == "element" || n == "el";
}

} // namespace

Step::Step(KeyDocument document) : doc_(std::move(document)) {}

const Record *Step::record(std::string_view keyword, std::optional<int> owner) const
{
    return doc_.find(keyword, owner);
}

std::optional<int> Step::number() const
{
    if (const auto *r = record("NSTART")) {
        if (auto v = r->header_int(0)) {
            return static_cast<int>(*v);
        }
    }
    return std::nullopt;
}

std::optional<double> Step::time() const
{
    if (const auto *r = record("TNOW")) {
        return r->header_real(0);
    }
    return std::nullopt;
}

std::optional<int> Step::simulation() const
{
    if (const auto *r = record("CURSIM")) {
        if (auto v = r->header_int(0)) {
            return static_cast<int>(*v);
        }
    }
    return std::nullopt;
}

std::optional<int> Step::mesh_number() const
{
    if (const auto *r = record("MESHNO")) {
        if (auto v = r->header_int(0)) {
            return static_cast<int>(*v);
        }
    }
    return std::nullopt;
}

std::vector<ObjectSummary> Step::objects() const
{
    std::vector<ObjectSummary> out;
    auto get = [&](int n) -> ObjectSummary & {
        for (auto &o : out) {
            if (o.number == n) {
                return o;
            }
        }
        out.push_back(ObjectSummary{});
        out.back().number = n;
        return out.back();
    };
    for (const auto &r : doc_.records()) {
        if (r.name != "OBJTYP" && r.name != "OBJNAM" && r.name != "RZ" && r.name != "ELMCON") {
            continue;
        }
        const auto obj = r.header_int(0);
        if (!obj) {
            continue;
        }
        auto &o = get(static_cast<int>(*obj));
        if (r.name == "OBJTYP") {
            if (auto t = r.header_int(1)) o.type = static_cast<int>(*t);
        } else if (r.name == "OBJNAM") {
            auto lines = r.body_lines();
            if (!lines.empty()) {
                auto s = lines.front();
                while (!s.empty() && s.back() == ' ') s.remove_suffix(1);
                o.name = std::string(s);
            }
        } else if (r.name == "RZ") {
            if (auto n = r.header_int(1)) o.nodes = static_cast<std::size_t>(*n);
        } else if (r.name == "ELMCON") {
            if (auto n = r.header_int(1)) o.elements = static_cast<std::size_t>(*n);
            if (auto k = r.header_int(2)) o.nodes_per_element = static_cast<int>(*k);
        }
    }
    std::sort(out.begin(), out.end(), [](const auto &a, const auto &b) { return a.number < b.number; });
    return out;
}

std::optional<Decoded> Step::decode(std::string_view keyword, std::optional<int> owner, std::string *why) const
{
    const auto *r = record(keyword, owner);
    if (r == nullptr) {
        if (why) *why = "no " + std::string(keyword) + " record";
        return std::nullopt;
    }
    return dk::decode(*r, why);
}

std::vector<double> Step::coordinates(int object) const
{
    auto f = field("RZ", object);
    if (!f) {
        return {};
    }
    return std::move(f->values);
}

std::vector<std::int32_t> Step::connectivity(int object, int *nodes_per_element) const
{
    auto d = decode("ELMCON", object);
    if (!d || d->tables.empty()) {
        return {};
    }
    const auto &t = d->tables.front();
    const auto *ids = t.column("elem");
    const auto *nodes = t.column("nodes");
    if (ids == nullptr || nodes == nullptr) {
        return {};
    }
    const int k = nodes->width;
    if (nodes_per_element) {
        *nodes_per_element = k;
    }
    std::int32_t max_id = 0;
    for (auto id : ids->ints) {
        max_id = std::max(max_id, id);
    }
    std::vector<std::int32_t> out(static_cast<std::size_t>(max_id) * static_cast<std::size_t>(k), 0);
    for (std::size_t row = 0; row < t.rows; ++row) {
        const auto id = ids->ints[row];
        if (id < 1) {
            continue;
        }
        std::copy_n(nodes->ints.begin() + static_cast<std::ptrdiff_t>(row * k), k,
                    out.begin() + static_cast<std::ptrdiff_t>((id - 1)) * k);
    }
    return out;
}

std::optional<Field> Step::field(std::string_view keyword, int object, std::string *why) const
{
    auto d = decode(keyword, object, why);
    if (!d) {
        return std::nullopt;
    }
    // First table with a leading node/element id column.
    const Table *tab = nullptr;
    Entity entity = Entity::Other;
    for (const auto &t : d->tables) {
        if (t.columns.empty() || t.columns[0].type != 'I' || t.columns[0].width != 1) {
            continue;
        }
        tab = &t;
        if (is_node_column(t.columns[0].name)) entity = Entity::Node;
        else if (is_element_column(t.columns[0].name)) entity = Entity::Element;
        break;
    }
    Field f;
    f.keyword = std::string(keyword);
    f.object = object;
    f.entity = entity;
    if (auto dv = d->header_real("default")) {
        f.default_value = *dv;
    }
    // Entity count from the mesh of the same object.
    std::size_t count = 0;
    if (entity == Entity::Node) {
        if (const auto *rz = record("RZ", object)) {
            if (auto n = rz->header_int(1)) count = static_cast<std::size_t>(*n);
        }
    } else if (entity == Entity::Element) {
        if (const auto *ec = record("ELMCON", object)) {
            if (auto n = ec->header_int(1)) count = static_cast<std::size_t>(*n);
        }
    }
    if (tab == nullptr) {
        // No id table: nothing written (e.g. "STRESS obj 0 default").
        f.count = count;
        f.components = 0;
        if (why && d->tables.empty()) *why = "record has no id table";
        return f;
    }
    const auto &ids = tab->columns[0].ints;
    for (auto id : ids) {
        count = std::max(count, static_cast<std::size_t>(std::max<std::int32_t>(id, 0)));
    }
    int comps = 0;
    for (std::size_t c = 1; c < tab->columns.size(); ++c) {
        comps += tab->columns[c].width;
        f.component_names.push_back(tab->columns[c].name);
    }
    f.count = count;
    f.components = comps;
    f.declared = tab->rows;
    f.values.assign(count * static_cast<std::size_t>(comps), f.default_value);
    for (std::size_t row = 0; row < tab->rows; ++row) {
        const auto id = ids[row];
        if (id < 1) {
            continue;
        }
        double *dst = f.values.data() + static_cast<std::size_t>(id - 1) * static_cast<std::size_t>(comps);
        for (std::size_t c = 1; c < tab->columns.size(); ++c) {
            const auto &col = tab->columns[c];
            const auto w = static_cast<std::size_t>(col.width);
            if (col.type == 'R') {
                std::copy_n(col.reals.begin() + static_cast<std::ptrdiff_t>(row * w), w, dst);
            } else {
                for (std::size_t k = 0; k < w; ++k) dst[k] = static_cast<double>(col.ints[row * w + k]);
            }
            dst += w;
        }
    }
    return f;
}

} // namespace dk
