// _OBJ_GEO3D (p1-07): the 3D surface mesh. Layout: defmodel/layout/obj_geo3d.hpp.
// Members of other packets' types are built, cleared and released through
// defmodel/geo/pending.hpp. The CowVector members use p1-05's template (defmodel/sdctl/cow.hpp);
// the original reaches them through its own copies of those helpers (p1-01 / p1-04 / p1-05).
#include <defmodel/layout/obj_geo3d.hpp>

#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defmodel/geo/geo_alloc.hpp>
#include <defmodel/geo/pending.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defrt/rdata.hpp>

namespace def::model {

using geo::mul_size_i32;
using geo::op_new;
using geo::wrap_add;
using geo::wrap_mul;
using geo::wrap_shl;

namespace {

// rdata 140509b50  1e30 (initial box minimum)
constexpr double kBig = def::rt::rdata::d_140509b50;
// @data 14051bef0  -1e30 (initial box maximum)
const double kMinusBig = std::bit_cast<double>(0xc6293e5939a08ceaULL);
// @data 14051f230  pi
const double kPi = std::bit_cast<double>(0x400921fb54442d18ULL);
// rdata 14051f228  180
constexpr double k180 = def::rt::rdata::d_14051f228;

inline std::size_t idx(std::int32_t i) { return static_cast<std::size_t>(static_cast<std::int64_t>(i)); }

// FUN_140484ab8: CRT abs (abs(INT_MIN) == INT_MIN)
inline std::int32_t crt_abs(std::int32_t v)
{
    return v < 0 ? static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(v)) : v;
}

} // namespace

// @src 1402f9020  ctor: OBJ_GEO_BASE ctor, vptr, coords (14019a6b0), elems (140159ad0),
// flags 0, point list (140257bf0), u40 (140159860), u50 (1402fedd0), u58 (1402fef20),
// ids (140159ad0), u78 0, tree (14042a7e0), then the slot-2 body 1402f92b0. The CowVector /
// CowList members are constructed before the body here, so their heap blocks come first.
OBJ_GEO3D::OBJ_GEO3D()
{
    elem_flags = nullptr;
    pending::fun_140257bf0(u28);
    pending::fun_1402fedd0(&u50);
    pending::fun_1402fef20(&u58);
    u78 = nullptr;
    pending::fun_14042a7e0(u80);
    OBJ_GEO3D::reset(); // 1402f92b0, direct call
}

// @src 1402f91f0  dtor: clear, then the members in reverse order (tree 14042a870, ids 140159b40,
// u58 1402fef90, u50 1402fee40, u40 140159930, point list 140257c60, elems 140159b40, coords
// 14019a720), OBJ_GEO_BASE dtor
// @src 1402fe990  slot 0: scalar deleting dtor
OBJ_GEO3D::~OBJ_GEO3D()
{
    clear();
    pending::fun_14042a870(u80);
    pending::fun_1402fef90(&u58);
    pending::fun_1402fee40(&u50);
    pending::fun_140257c60(u28);
}

// @src 1402fe970  slot 1  // str "_OBJ_GEO3D"
const char* OBJ_GEO3D::class_name() { return "_OBJ_GEO3D"; }

// @src 1402fe980  slot 11
std::int32_t OBJ_GEO3D::is_extension() { return 100; }

// @src 1402fb690  slot 10: empty
void OBJ_GEO3D::notify() {}

// @src 1400034d0  coords size / 3
std::int32_t OBJ_GEO3D::node_count() const
{
    return static_cast<std::int32_t>(static_cast<std::uint64_t>(coords.size()) / 3 & 0xffffffffu);
}

// @src 140003500  elems size / 4
std::int32_t OBJ_GEO3D::elem_count() const
{
    return static_cast<std::int32_t>(static_cast<std::uint64_t>(elems.size()) >> 2 & 0xffffffffu);
}

// @src 1402f92b0  slot 2
void OBJ_GEO3D::reset()
{
    coords.clear(); // 1402568b0
    elems.clear();  // 14019a7d0
    elem_flags = nullptr;
    u78 = nullptr;
    u70 = false;
    std::int32_t zero = 0;
    set_u48(&zero);
    std::int32_t zero2 = 0;
    set_u60(&zero2);
    elem_ids.clear(); // 14019a7d0
}

// @src 1402f9340  -> 1402fcde0
void OBJ_GEO3D::clear() { clear_all(); }

// @src 1402fcde0  clear every member
void OBJ_GEO3D::clear_all()
{
    clear_nodes();
    clear_elems();
    free_elem_flags();
    pending::fun_1402581b0(u28);
    clear_elem_ids();
    u40.clear(); // 14015a620
    pending::fun_1402ff680(&u50);
    pending::fun_1402ff790(&u58);
}

// @src 1402f9600  resize the coordinates to n nodes
double* OBJ_GEO3D::resize_nodes(const std::int32_t* n)
{
    double* r;
    if (*n < 1) {
        r = nullptr;
    } else {
        double zero = 0.0;
        coords.resize(idx(wrap_mul(*n, 3)), zero); // 14019a740
        r = &coords.mutable_at(0);                 // 140005e70
    }
    return r;
}

// @src 1402f9670  coords.clear (1402568b0)
void OBJ_GEO3D::clear_nodes() { coords.clear(); }

// @src 1402f96a0  resize the elements to n
std::int32_t* OBJ_GEO3D::resize_elems(const std::int32_t* n)
{
    std::int32_t* r;
    if (*n < 1) {
        r = nullptr;
    } else {
        std::int32_t zero = 0;
        elems.resize(idx(wrap_shl(*n, 2)), zero); // 140159b60
        r = &elems.mutable_at(0);                 // 140005100
    }
    return r;
}

// @src 1402f9710  elems.clear (14019a7d0)
void OBJ_GEO3D::clear_elems() { elems.clear(); }

// @src 1402f9970  per-element flags
std::int32_t* OBJ_GEO3D::alloc_elem_flags()
{
    std::int32_t n = elem_count();
    std::int32_t* r;
    if (n < 1) {
        r = nullptr;
    } else {
        elem_flags = static_cast<std::int32_t*>(op_new(mul_size_i32(n, 4)));
        if (elem_flags != nullptr) {
            std::memset(elem_flags, 0, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 2));
        }
        r = elem_flags;
    }
    return r;
}

// @src 1402f9a00  free the flags
void OBJ_GEO3D::free_elem_flags()
{
    if (elem_flags != nullptr) {
        std::free(elem_flags);
    }
    elem_flags = nullptr;
}

// @src 1402f9a50  one id per element
std::int32_t* OBJ_GEO3D::resize_elem_ids()
{
    std::int32_t n = elem_count();
    std::int32_t* r;
    if (n < 1) {
        r = nullptr;
    } else {
        std::int32_t zero = 0;
        elem_ids.resize(idx(n), zero);
        r = &elem_ids.mutable_at(0);
    }
    return r;
}

// @src 1402f9ac0  ids.clear (14019a7d0)
void OBJ_GEO3D::clear_elem_ids() { elem_ids.clear(); }

// @src 1402f9b60  assign
void OBJ_GEO3D::assign(OBJ_GEO3D* other)
{
    clear();
    std::int32_t nodes = other->node_count();
    if (0 < nodes) {
        resize_nodes(&nodes);
        const double* src = other->coords.begin(); // 14026a3c0, no detach
        for (std::int32_t i = 0; i < nodes; i = i + 1) {
            for (std::int32_t j = 0; j < 3; j = j + 1) {
                std::int32_t k = wrap_add(wrap_mul(i, 3), j);
                coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j))) = src[k];
            }
        }
    }
    std::int32_t elements = other->elem_count();
    if (0 < elements) {
        resize_elems(&elements);
        const std::int32_t* src = other->elems.begin(); // 1402fe9d0
        for (std::int32_t i = 0; i < elements; i = i + 1) {
            for (std::int32_t j = 0; j < 4; j = j + 1) {
                std::int32_t k = wrap_add(j, wrap_mul(i, 4));
                elems.mutable_at(idx(wrap_add(j, wrap_mul(i, 4)))) = src[k];
            }
        }
        alloc_elem_flags();
        if (elem_flags != nullptr && other->elem_flags != nullptr) {
            std::memcpy(elem_flags, other->elem_flags,
                        static_cast<std::size_t>(static_cast<std::int64_t>(elements) << 2));
        }
        resize_elem_ids();
        const std::int32_t* ids = other->elem_ids.begin(); // 1402fe9f0
        for (std::int32_t i = 0; i < elements; i = i + 1) {
            if (ids != nullptr) {
                elem_ids.mutable_at(idx(i)) = ids[i];
            }
        }
    }
    pending::fun_140257c80(u28, other->u28);
}

// @src 1402f9dc0  slot 4
void OBJ_GEO3D::copy_from(OBJ_GEO_BASE* other) { assign(static_cast<OBJ_GEO3D*>(other)); }

// @src 1402fb120  slot 9
double OBJ_GEO3D::char_length()
{
    std::int32_t nodes = node_count();
    std::int32_t elements = elem_count();
    double r;
    if (nodes < 1 || elements < 1) {
        r = 0.0;
    } else {
        double box[6];
        box[2] = kBig;
        box[1] = kBig;
        box[0] = kBig;
        box[5] = kMinusBig;
        box[4] = kMinusBig;
        box[3] = kMinusBig;
        for (std::int32_t i = 0; i < nodes; i = i + 1) {
            for (std::int32_t j = 0; j < 3; j = j + 1) {
                double lo = box[j];
                double v;
                if (coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j))) <= lo) {
                    v = coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j)));
                } else {
                    v = box[j];
                }
                box[j] = v;
                double hi = box[j + 3];
                if (hi <= coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j)))) {
                    v = coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j)));
                } else {
                    v = box[j + 3];
                }
                box[j + 3] = v;
            }
        }
        double s = 0.0;
        for (std::int32_t j = 0; j < 3; j = j + 1) {
            s = s + (box[j + 3] - box[j]) * (box[j + 3] - box[j]);
        }
        r = std::sqrt(s);
    }
    return r;
}

// @src 1402fb3a0  slot 6
void OBJ_GEO3D::scale(const double* f)
{
    std::int32_t nodes = node_count();
    if (nodes != 0) {
        for (std::int32_t i = 0; i < nodes; i = i + 1) {
            for (std::int32_t j = 0; j < 3; j = j + 1) {
                double v = coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j)));
                double s = *f;
                coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j))) = v * s;
            }
        }
    }
}

// @src 1402fb480  slot 7
void OBJ_GEO3D::translate(const double* d)
{
    std::int32_t nodes = node_count();
    if (nodes != 0) {
        for (std::int32_t i = 0; i < nodes; i = i + 1) {
            for (std::int32_t j = 0; j < 3; j = j + 1) {
                double* p = &coords.mutable_at(idx(wrap_add(wrap_mul(i, 3), j)));
                *p = *p + d[j];
            }
        }
    }
}

// @src 1402fb550  slot 8: the point list rotates itself in degrees; the nodes go through the
// rotation matrix that 1402214c0 builds from the angle in radians
void OBJ_GEO3D::rotate(const double* center, const void* axis, double angle)
{
    double a = angle;
    std::int32_t nodes = node_count();
    if (nodes != 0) {
        const auto* ax = static_cast<const double*>(axis);
        pending::fun_140258400(u28, center, ax, a);
        a = (a * kPi) / k180;
        alignas(8) std::byte m[0x80];
        pending::fun_1402214c0(m, center, ax, &a);
        for (std::int32_t i = 0; i < nodes; i = i + 1) {
            double* p = &coords.mutable_at(idx(wrap_mul(i, 3)));
            geo_transform_point(p, reinterpret_cast<const double*>(m));
        }
        pending::fun_140220560(m);
    }
}

// @src 1402fce60  slot 5
void OBJ_GEO3D::reverse() { reverse_elements(); }

// @src 1402fbf60  reverse each element's node order
void OBJ_GEO3D::reverse_elements()
{
    std::int32_t nodes = node_count();
    std::int32_t elements = elem_count();
    if (0 < elements && 0 < nodes) {
        for (std::int32_t e = 0; e < elements; e = e + 1) {
            std::int32_t last = elems.mutable_at(idx(wrap_add(wrap_mul(e, 4), 3)));
            for (std::int32_t k = 0; k < 2; k = k + 1) {
                std::size_t a = idx(wrap_add(k, wrap_mul(e, 4)));
                std::size_t b = idx(wrap_add(wrap_mul(e, 4), 3) - k);
                std::uint32_t* pa = reinterpret_cast<std::uint32_t*>(&elems.mutable_at(a));
                std::uint32_t* pb = reinterpret_cast<std::uint32_t*>(&elems.mutable_at(b));
                *pa = *pa ^ *pb;
                pb = reinterpret_cast<std::uint32_t*>(&elems.mutable_at(b));
                pa = reinterpret_cast<std::uint32_t*>(&elems.mutable_at(a));
                *pb = *pb ^ *pa;
                pa = reinterpret_cast<std::uint32_t*>(&elems.mutable_at(a));
                pb = reinterpret_cast<std::uint32_t*>(&elems.mutable_at(b));
                *pa = *pa ^ *pb;
            }
            if (last < 0) {
                std::int32_t v = crt_abs(elems.mutable_at(idx(wrap_shl(e, 2))));
                elems.mutable_at(idx(wrap_shl(e, 2))) = v;
                std::int32_t w = crt_abs(elems.mutable_at(idx(wrap_add(wrap_mul(e, 4), 3))));
                elems.mutable_at(idx(wrap_add(wrap_mul(e, 4), 3))) =
                    static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(w));
            }
        }
    }
}

// @src 1402fbb50  0x48
void OBJ_GEO3D::set_u48(const std::int32_t* v) { u48 = *v; }

// @src 1402fbba0  0x60
void OBJ_GEO3D::set_u60(const std::int32_t* v) { u60 = *v; }

} // namespace def::model
