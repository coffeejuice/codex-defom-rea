// kw-g2geo family "mesh": element type and material-group model functions
// (defmodel/geo_kw/mesh_model.hpp). Decompiles: deform-rea-work/out/g2geo/fam_mesh.c.
#include <defmodel/geo_kw/mesh_model.hpp>

#include <cstddef>
#include <cstdlib>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::model::geo_kw {

namespace {

template <class T, class C>
T& at(C& o, std::size_t off)
{
    return *reinterpret_cast<T*>(reinterpret_cast<std::byte*>(&o) + off);
}
template <class T, class C>
const T& at(const C& o, std::size_t off)
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const std::byte*>(&o) + off);
}

}  // namespace

// @src 140165500  element type and the node/face counts it implies
void set_element_type(LLOBJ_DATA& obj, std::int32_t type)
{
    at<std::int32_t>(obj, 0x890) = type;
    if (at<std::int32_t>(obj, 0x890) == 2) {
        at<std::int32_t>(obj, 0x894) = 4;
        at<std::int32_t>(obj, 0x898) = 4;
        at<std::int32_t>(obj, 0x89c) = 3;
    } else if (at<std::int32_t>(obj, 0x890) == 4) {
        at<std::int32_t>(obj, 0x894) = 8;
        at<std::int32_t>(obj, 0x898) = 6;
        at<std::int32_t>(obj, 0x89c) = 4;
    }
}

// @src 1401fb270  number of material groups and the object's group
void material_groups(LLOBJ_DATA& obj, std::int32_t* count, std::int32_t* group)
{
    if (at<std::int32_t>(obj, 0x3980) < 1) {
        material_groups_counted(obj, count, group);
        return;
    }
    *count = 1;
    *group = at<std::int32_t>(obj, 0x3980);
    if (at<std::int32_t>(obj, 0x87c) != 0 && at<std::int32_t*>(obj, 0x9f8) != nullptr) {
        auto* seen = static_cast<std::int32_t*>(std::calloc(10, 4));
        std::int32_t cap = 10;
        std::int32_t k = 0;
        for (std::int32_t e = 0; e < at<std::int32_t>(obj, 0x884); ++e) {
            const std::int32_t g = at<std::int32_t*>(obj, 0x9f8)[e];
            bool found = false;
            for (std::int32_t j = 0; j < k; ++j) {
                if (g == seen[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                seen[k] = g;
                k = k + 1;
                if (cap <= k) {
                    cap = cap + 10;
                    seen = static_cast<std::int32_t*>(std::realloc(seen, static_cast<std::size_t>(cap) << 2));
                }
            }
        }
        *count = k;
        if (seen != nullptr) std::free(seen);
    }
}

// @src 1401fb440  count the groups and pick the most frequent one
void material_groups_counted(LLOBJ_DATA& obj, std::int32_t* count, std::int32_t* group)
{
    *count = 0;
    *group = 0;
    if (at<std::int32_t>(obj, 0x87c) == 0 || at<std::int32_t*>(obj, 0x9f8) == nullptr) return;
    auto* vals = static_cast<std::int32_t*>(std::calloc(10, 4));
    auto* hits = static_cast<std::int32_t*>(std::calloc(10, 4));
    std::int32_t cap = 10;
    bool has_old = false;
    std::int32_t k = 0;
    for (std::int32_t e = 0; e < at<std::int32_t>(obj, 0x884); ++e) {
        const std::int32_t g = at<std::int32_t*>(obj, 0x9f8)[e];
        bool found = false;
        if (g == at<std::int32_t>(obj, 0x3980)) has_old = true;
        std::int32_t j = 0;
        for (; j < k; ++j) {
            if (g == vals[j]) {
                hits[j] = hits[j] + 1;
                found = true;
                break;
            }
        }
        if (!found) {
            vals[k] = g;
            hits[j] = 1;  // j == k here
            k = k + 1;
            if (cap <= k) {
                cap = cap + 10;
                vals = static_cast<std::int32_t*>(std::realloc(vals, static_cast<std::size_t>(cap) << 2));
                hits = static_cast<std::int32_t*>(std::realloc(hits, static_cast<std::size_t>(cap) << 2));
            }
        }
    }
    std::int32_t best = 0;
    for (std::int32_t i = 1; i < k; ++i) {
        if (hits[best] < hits[i]) best = i;
    }
    *count = k;
    *group = vals[best];
    if (vals != nullptr) std::free(vals);
    if (hits != nullptr) std::free(hits);
    if (!has_old) at<std::int32_t>(obj, 0x3980) = *group;
}

// @src 140202960  recompute obj+0x3980 from the element groups
void update_material_group(LLOBJ_DATA& obj)
{
    std::int32_t count = 0;
    std::int32_t group = 0;
    const std::int32_t old = at<std::int32_t>(obj, 0x3980);
    at<std::int32_t>(obj, 0x3980) = 0;
    material_groups(obj, &count, &group);
    if (group == 0) {
        at<std::int32_t>(obj, 0x3980) = old;
    } else {
        at<std::int32_t>(obj, 0x3980) = group;
    }
}

// @src 14039aa20  model byte +0x1b5c
std::uint8_t step_u1b5c(const DB_STEP_DATA& m) { return at<std::uint8_t>(m, 0x1b5c); }

}  // namespace def::model::geo_kw
