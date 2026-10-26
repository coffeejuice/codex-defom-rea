// _OBJ_GEO2D (p1-07): the 2D outline. Layout: defmodel/layout/obj_geo2d.hpp.
// The original is an unoptimised build: every expression below follows the decompile's order.
#include <defmodel/layout/obj_geo2d.hpp>

#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include <defmodel/geo/geo_alloc.hpp>
#include <defmodel/geo/globals_p1_07.hpp>
#include <defmodel/geo/pending.hpp>
#include <defrt/rdata.hpp>

namespace def::model {

using geo::ftoi_trunc;
using geo::mul_size_i32;
using geo::op_new;
using geo::wrap_add;
using geo::wrap_mul;
using geo::wrap_shl;

namespace {

// rdata 14051f1b8  1e-6, duplicate-point tolerance
constexpr double kDupTol = def::rt::rdata::d_14051f1b8;
// rdata 140522fd8  180/pi
constexpr double kRadToDeg = def::rt::rdata::d_140522fd8;
// rdata 14051bed8  0.0 (area threshold of is_ccw)
constexpr double kZero = def::rt::rdata::d_14051bed8;
// rdata 140509fb8  2.0
constexpr double kTwo = def::rt::rdata::d_140509fb8;
// rdata 140509c90  1.0
constexpr double kOne = def::rt::rdata::d_140509c90;

// row r, column c of the point array (index arithmetic in 32 bits, as the binary)
inline double* at(double* p, std::int32_t r, std::int32_t dof, std::int32_t c)
{
    return p + static_cast<std::int64_t>(wrap_add(wrap_mul(r, dof), c));
}

} // namespace

// @src 140276090  values per row
std::int32_t geo2d_dof(std::int32_t curve_type)
{
    std::int32_t r;
    if (curve_type == 0) {
        r = 0;
    } else if (curve_type == 1) {
        r = 3;
    } else if (curve_type == 2) {
        r = 6;
    } else {
        r = 0;
    }
    return r;
}

// @src 140275fa0  new zeroed n doubles
double* geo2d_alloc_zeroed(std::int32_t n)
{
    double* p;
    if (n < 1) {
        p = nullptr;
    } else {
        p = static_cast<double*>(op_new(mul_size_i32(n, 8)));
        if (p == nullptr) {
            p = nullptr;
        } else {
            std::memset(p, 0, static_cast<std::size_t>(static_cast<std::int64_t>(wrap_shl(n, 3))));
        }
    }
    return p;
}

// @src 140276060  free when non-zero
void geo2d_free(void* p)
{
    if (p != nullptr) {
        std::free(p);
    }
}

// @src 14027f3e0  -> 14041e090 (p1-11)
void geo2d_copy_ext(OBJ_GEO2D_DIGITIZED* dst, OBJ_GEO2D_DIGITIZED* src) { pending::fun_14041e090(dst, src); }

// @src 14027f430  ext + 0x10 = owner
void geo2d_set_ext_owner(OBJ_GEO2D_DIGITIZED* ext, OBJ_GEO2D* owner)
{
    *reinterpret_cast<OBJ_GEO2D**>(reinterpret_cast<std::byte*>(ext) + 0x10) = owner;
}

// @src 14027f460  p = (p, 1) * m
void geo_transform_point(double* p, const double* m)
{
    double v[8];
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
    v[3] = 0.0;
    for (int i = 0; i < 3; i = i + 1) {
        v[i + 4] = p[i];
    }
    v[7] = kOne;
    for (int i = 0; i < 4; i = i + 1) {
        for (int k = 0; k < 4; k = k + 1) {
            v[i] = v[i] + v[k + 4] * m[k * 4 + i];
        }
    }
    p[0] = v[0];
    p[1] = v[1];
    p[2] = v[2];
}

// @src 140275cd0  ctor
OBJ_GEO2D::OBJ_GEO2D()
{
    curve_type = 0;
    n_points = 0;
    points = nullptr;
    ext = nullptr;
    flag28 = 0;
    OBJ_GEO2D::reset(); // 140275e50, direct call
}

// @src 140275e00  dtor: clear_all, then OBJ_GEO_BASE dtor
// @src 1401b0650  slot 0: vector deleting dtor (array form: delete[] with element size 0x30)
OBJ_GEO2D::~OBJ_GEO2D() { clear_all(); }

// @src 14027f380  slot 1  // str 140537b30
const char* OBJ_GEO2D::class_name() { return "_OBJ_GEO2D"; }

// @src 140275e50  slot 2
void OBJ_GEO2D::reset()
{
    curve_type = 0;
    n_points = 0;
    points = nullptr;
    ext = nullptr;
}

// @src 14027f360  slot 3
void OBJ_GEO2D::clear_data() { allocate_points(); }

// @src 140275e90  clear_all
void OBJ_GEO2D::clear_all()
{
    curve_type = 0;
    n_points = 0;
    free_points();
    if (ext != nullptr) {
        OBJ_GEO_BASE* e = ext_base();
        if (e != nullptr) {
            delete e; // (**vtable)(ext, 1)
        }
        ext = nullptr;
    }
}

// @src 140275f30  allocate the point array
double* OBJ_GEO2D::allocate_points()
{
    free_points();
    std::int32_t d = dof();
    points = geo2d_alloc_zeroed(wrap_mul(n_points, d));
    return points;
}

// @src 140276030  free the point array
void OBJ_GEO2D::free_points()
{
    geo2d_free(points);
    points = nullptr;
}

// @src 1402760e0  dof of curve_type
std::int32_t OBJ_GEO2D::dof() const { return geo2d_dof(curve_type); }

// @src 140276140  assign
void OBJ_GEO2D::assign(OBJ_GEO2D* other)
{
    clear_all();
    curve_type = other->curve_type;
    n_points = other->n_points;
    std::int32_t n = dof();
    n = wrap_mul(n_points, n);
    if (0 < n && allocate_points() != nullptr) {
        std::memcpy(points, other->points,
                    static_cast<std::size_t>(static_cast<std::int64_t>(wrap_mul(n, 8))));
        make_ext();
        if (other->ext != nullptr) {
            geo2d_copy_ext(ext, other->ext);
        }
    }
}

// @src 140276230  slot 4 (Ghidra: Concurrency::multi_link_registry::add, a code-folding match)
void OBJ_GEO2D::copy_from(OBJ_GEO_BASE* other)
{
    if (other != nullptr) {
        assign(static_cast<OBJ_GEO2D*>(other));
    }
}

// @src 1402763d0  convert to curve type t
std::int64_t OBJ_GEO2D::set_curve_type(std::int32_t t)
{
    std::int64_t result;
    if (curve_type == t || curve_type == 0 || t == 0) {
        result = 0;
    } else {
        std::int32_t n_out = 0;
        std::int32_t n_in = n_points;
        double* buf = nullptr;
        if (n_in < 1) {
            curve_type = t;
            result = 0;
        } else {
            if (curve_type == 2 && t == 1) {
                std::int32_t m = wrap_mul(n_in, 0x3c);
                buf = static_cast<double*>(op_new(mul_size_i32(m, 8)));
                std::memset(buf, 0, static_cast<std::size_t>(static_cast<std::int64_t>(m) << 3));
                pending::fun_14032ac75(&n_out, buf, &n_in, points);
            } else {
                bool dropped = fun_1402765e0();
                if (dropped) {
                    n_in = n_points;
                }
                std::int32_t m = wrap_mul(n_in, 0x3c);
                buf = static_cast<double*>(op_new(mul_size_i32(m, 8)));
                std::memset(buf, 0, static_cast<std::size_t>(static_cast<std::int64_t>(m) << 3));
                pending::fun_140345f5b(points, &n_in);
                pending::fun_14032b34b(&n_in, points, &n_out, buf);
            }
            curve_type = t;
            n_points = n_out;
            clear_data(); // virtual, slot 3
            std::int32_t d = dof();
            std::memcpy(points, buf,
                        static_cast<std::size_t>(static_cast<std::int64_t>(wrap_mul(d, n_points)) << 3));
            std::free(buf);
            result = 1;
        }
    }
    return result;
}

// @src 1402765e0  drop duplicate consecutive points (curve type 1)
bool OBJ_GEO2D::fun_1402765e0()
{
    std::int32_t dropped = 0;
    bool result;
    if (n_points < 1 || points == nullptr) {
        result = false;
    } else if (curve_type == 1) {
        std::int32_t d = dof();
        auto* mark = static_cast<char*>(op_new(static_cast<std::size_t>(static_cast<std::int64_t>(n_points))));
        std::memset(mark, 0, static_cast<std::size_t>(static_cast<std::int64_t>(n_points)));
        for (std::int32_t i = 0; i < n_points + -1; i = i + 1) {
            double y0 = *at(points, i, d, 1);
            double y1 = *at(points, i + 1, d, 1);
            double dx = std::fabs(*at(points, i, d, 0) - *at(points, i + 1, d, 0));
            if (dx < kDupTol) {
                double dy = std::fabs(y0 - y1);
                if (dy < kDupTol) {
                    mark[i] = 1;
                    dropped = dropped + 1;
                }
            }
        }
        if (0 < dropped) {
            std::int32_t n = wrap_mul(n_points, d);
            auto* copy = static_cast<double*>(op_new(mul_size_i32(n, 8)));
            std::memcpy(copy, points, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 3));
            std::int32_t k = 0;
            for (std::int32_t i = 0; i < n_points; i = i + 1) {
                if (mark[i] == '\0') {
                    std::memcpy(at(points, k, d, 0), at(copy, i, d, 0),
                                static_cast<std::size_t>(static_cast<std::int64_t>(d) << 3));
                    k = k + 1;
                }
            }
            n_points = k;
            std::free(copy);
        }
        std::free(mark);
        if (0 < dropped) {
            set_flag28(true);
        }
        result = 0 < dropped;
    } else {
        result = false;
    }
    return result;
}

// @src 140276e30  append
void OBJ_GEO2D::append(OBJ_GEO2D* other)
{
    OBJ_GEO2D* t = new OBJ_GEO2D(); // operator new(0x30), ctor 140275cd0
    t->reset();                     // virtual, slot 2
    if (curve_type == other->curve_type) {
        t->assign(other);
    } else {
        t->assign(other);
        t->set_curve_type(curve_type);
    }
    std::int32_t n = dof();
    n = wrap_mul(n_points, n);
    auto* keep = static_cast<double*>(op_new(mul_size_i32(n, 8)));
    if (keep == nullptr) {
        if (t != nullptr) {
            delete t;
        }
    } else {
        std::memcpy(keep, points, static_cast<std::size_t>(static_cast<std::int64_t>(wrap_mul(n, 8))));
        n_points = wrap_add(n_points, t->n_points);
        allocate_points();
        if (points == nullptr) {
            if (t != nullptr) {
                delete t;
            }
            std::free(keep);
        } else {
            std::memcpy(points, keep, static_cast<std::size_t>(static_cast<std::int64_t>(wrap_mul(n, 8))));
            std::free(keep);
            double* dst = points;
            std::int32_t d = dof();
            std::memcpy(dst + n, t->points,
                        static_cast<std::size_t>(static_cast<std::int64_t>(wrap_mul(wrap_mul(t->n_points, d), 8))));
            t->clear_all();
            if (t != nullptr) {
                delete t;
            }
            set_flag28(true);
        }
    }
}

// @src 140277830  new ext
void OBJ_GEO2D::make_ext()
{
    delete_ext();
    void* mem = op_new(0x50);
    OBJ_GEO2D_DIGITIZED* e;
    if (mem == nullptr) {
        e = nullptr;
    } else {
        e = pending::fun_14041dcc0(mem);
    }
    ext = e;
    geo2d_set_ext_owner(ext, this);
}

// @src 1402778f0  delete ext
void OBJ_GEO2D::delete_ext()
{
    if (ext != nullptr) {
        OBJ_GEO_BASE* e = ext_base();
        if (e != nullptr) {
            delete e;
        }
    }
    ext = nullptr;
}

// @src 140277970  slot 10
void OBJ_GEO2D::notify()
{
    if (ext != nullptr) {
        ext_base()->notify(); // ext vtable + 0x50
    }
}

// @src 140277a20  slot 6
void OBJ_GEO2D::scale(const double* f)
{
    std::int32_t d = dof();
    for (std::int32_t i = 0; i < n_points; i = i + 1) {
        for (std::int32_t j = 0; j < d; j = j + 1) {
            if (curve_type == 1 || (0 < j && j < d + -1)) {
                *at(points, i, d, j) = *at(points, i, d, j) * *f;
            }
        }
    }
}

// @src 140277b00  slot 7: curve type 1 adds d[0], d[1] to x, y; type 2 adds them to x1, y1,
// x2, y2 (column j takes d[(j + 1) % 2])
void OBJ_GEO2D::translate(const double* off)
{
    std::int32_t d = dof();
    for (std::int32_t i = 0; i < n_points; i = i + 1) {
        if (curve_type == 1) {
            for (std::int32_t j = 0; j < 2; j = j + 1) {
                *at(points, i, d, j) = *at(points, i, d, j) + off[j];
            }
        } else {
            for (std::int32_t j = 1; j < 5; j = j + 1) {
                std::int32_t c = (j + 1) % 2;
                *at(points, i, d, j) = *at(points, i, d, j) + off[c];
            }
        }
    }
}

// @src 140277c50  slot 8: 14032ab90(point, &angle, center) per point / segment end
void OBJ_GEO2D::rotate(const double* center, const void*, double angle)
{
    double a = angle;
    std::int32_t d = dof();
    for (std::int32_t i = 0; i < n_points; i = i + 1) {
        if (curve_type == 1) {
            pending::fun_14032ab90(at(points, i, d, 0), &a, center);
        } else {
            pending::fun_14032ab90(at(points, i, d, 1), &a, center);
            pending::fun_14032ab90(at(points, i, d, 3), &a, center);
        }
    }
}

// @src 1402567c0  slot 5
void OBJ_GEO2D::reverse() { reverse_points(); }

// @src 1402788a0  reverse the rows
void OBJ_GEO2D::reverse_points()
{
    if (0 < n_points && points != nullptr) {
        set_flag28(true);
        std::int32_t d = dof();
        std::int32_t n = wrap_mul(d, n_points);
        auto* buf = static_cast<double*>(op_new(mul_size_i32(n, 8)));
        std::memcpy(buf, points, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 3));
        if (curve_type == 2) {
            for (std::int32_t i = d + -1; i < n; i = i + d) {
                buf[i] = buf[i] * kRadToDeg;
            }
        }
        pending::fun_14041f590(&d, &n_points, buf);
        std::memcpy(points, buf, static_cast<std::size_t>(static_cast<std::int64_t>(n) << 3));
        std::free(buf);
    }
}

// @src 1402789f0  orientation: signed area of the outline >= 0
bool OBJ_GEO2D::is_ccw()
{
    using pending::Vec3;
    std::int32_t d = dof();
    bool result;
    if (n_points < 1 || points == nullptr) {
        result = false;
    } else {
        double* buf;
        std::int32_t k;
        if (curve_type == 1) {
            k = n_points;
            buf = static_cast<double*>(op_new(mul_size_i32(wrap_mul(k, 2), 8)));
            for (std::int32_t i = 0; i < k; i = i + 1) {
                std::memcpy(buf + static_cast<std::int64_t>(wrap_shl(i, 1)),
                            points + static_cast<std::int64_t>(wrap_mul(d, i)), 0x10);
            }
        } else {
            const std::int32_t steps = 4;
            double step = 0.0;
            double angle = 0.0;
            buf = static_cast<double*>(op_new(mul_size_i32(wrap_mul(n_points, 10), 8)));
            k = 0;
            for (std::int32_t i = 0; i < n_points; i = i + 1) {
                if (ftoi_trunc(*(points + static_cast<std::int64_t>(wrap_mul(d, i)))) == 1) {
                    std::memcpy(buf + static_cast<std::int64_t>(wrap_mul(k, 2)), at(points, i, d, 1), 0x20);
                    k = k + 2;
                } else {
                    step = (*at(points, i, d, 5) / static_cast<double>(steps)) * kRadToDeg;
                    Vec3 tmp;
                    Vec3 start = *pending::fun_140308730(&tmp, at(points, i, d, 1));
                    Vec3 tmp2;
                    Vec3 center = *pending::fun_140308730(&tmp2, at(points, i, d, 3));
                    std::memcpy(buf + static_cast<std::int64_t>(wrap_mul(k, 2)), at(points, i, d, 1), 0x10);
                    Vec3 p{};
                    for (std::int32_t j = 1; k = k + 1, j <= steps; j = j + 1) {
                        angle = step * static_cast<double>(j);
                        pending::fun_1403077e0(&center, &start, &p, angle);
                        Vec3 tmp3;
                        p = *pending::fun_140308a20(&tmp3, &center, &p);
                        buf[wrap_mul(k, 2)] = p.v[0];
                        buf[wrap_add(wrap_mul(k, 2), 1)] = p.v[1];
                    }
                }
            }
        }
        double area = pending::fun_140309550(k, buf);
        std::free(buf);
        if (area < kZero) {
            result = false;
        } else {
            result = true;
        }
    }
    return result;
}

// @src 140279040  bounding box
bool OBJ_GEO2D::bounding_box(double* min, double* max, double* radius)
{
    std::int32_t d = dof();
    if (n_points < 1 || points == nullptr) {
        return false;
    }
    if (curve_type == 1) {
        max[0] = points[0];
        min[0] = max[0];
        max[1] = points[1];
        min[1] = max[1];
        *radius = points[2];
        for (std::int32_t i = 1; i < n_points; i = i + 1) {
            double x = *at(points, i, d, 0);
            min[0] = (x < min[0] || x == min[0]) ? *at(points, i, d, 0) : min[0];
            max[0] = (max[0] <= *at(points, i, d, 0)) ? *at(points, i, d, 0) : max[0];
            min[1] = (*at(points, i, d, 1) <= min[1]) ? *at(points, i, d, 1) : min[1];
            max[1] = (max[1] <= *at(points, i, d, 1)) ? *at(points, i, d, 1) : max[1];
            *radius = (*radius <= *at(points, i, d, 2)) ? *at(points, i, d, 2) : *radius;
        }
    } else {
        max[0] = points[1];
        min[0] = max[0];
        max[1] = points[2];
        min[1] = max[1];
        *radius = 0.0;
        for (std::int32_t i = 0; i < n_points; i = i + 1) {
            double type = *at(points, i, d, 0);
            double x1 = *at(points, i, d, 1);
            min[0] = (x1 < min[0] || x1 == min[0]) ? *at(points, i, d, 1) : min[0];
            max[0] = (max[0] <= *at(points, i, d, 1)) ? *at(points, i, d, 1) : max[0];
            min[1] = (*at(points, i, d, 2) <= min[1]) ? *at(points, i, d, 2) : min[1];
            max[1] = (max[1] <= *at(points, i, d, 2)) ? *at(points, i, d, 2) : max[1];
            double x2 = *at(points, i, d, 3);
            min[0] = (x2 < min[0] || x2 == min[0]) ? *at(points, i, d, 3) : min[0];
            max[0] = (max[0] <= *at(points, i, d, 3)) ? *at(points, i, d, 3) : max[0];
            min[1] = (*at(points, i, d, 4) <= min[1]) ? *at(points, i, d, 4) : min[1];
            max[1] = (max[1] <= *at(points, i, d, 4)) ? *at(points, i, d, 4) : max[1];
            if (ftoi_trunc(type) == 2) {
                *radius = (*radius <= *at(points, i, d, 5)) ? *at(points, i, d, 5) : *radius;
            }
        }
    }
    return true;
}

// @src 140278ed0  this box inside other's box. The original leaves the boxes uninitialised
// when an outline is empty; the port starts them at 0.
bool OBJ_GEO2D::contains_bbox(OBJ_GEO2D* other)
{
    double a_min[2] = {0.0, 0.0};
    double a_max[2] = {0.0, 0.0};
    double b_min[2] = {0.0, 0.0};
    double b_max[2] = {0.0, 0.0};
    double ra = 0.0;
    double rb = 0.0;
    bounding_box(a_min, a_max, &ra);
    other->bounding_box(b_min, b_max, &rb);
    bool r;
    if (a_min[0] < b_min[0] || a_min[1] < b_min[1] || b_max[0] < a_max[0] || b_max[1] < a_max[1]) {
        r = false;
    } else {
        r = true;
    }
    return r;
}

// @src 140278f70  slot 9 (box uninitialised for an empty outline in the original; 0 here)
double OBJ_GEO2D::char_length()
{
    double min[2] = {0.0, 0.0};
    double max[2] = {0.0, 0.0};
    double radius = 0.0;
    bounding_box(min, max, &radius);
    double len = std::sqrt((max[0] - min[0]) * (max[0] - min[0]) + (max[1] - min[1]) * (max[1] - min[1]));
    if (len <= radius * kTwo) {
        len = radius * kTwo;
    }
    return len;
}

// @src 14027f390  slot 11
std::int32_t OBJ_GEO2D::is_extension() { return 0; }

// @src 14027f410  0x28
void OBJ_GEO2D::set_flag28(bool v) { flag28 = v ? 1 : 0; }

} // namespace def::model
