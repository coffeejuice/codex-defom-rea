// defcmd vec3.hpp: out-of-line vector helpers.
#include <defcmd/vec3.hpp>
#include <defdbio/gendb/obj_counts.hpp>
#include <defdbio/gendb/pre_steps.hpp>
#include <defdbio/objcheck/objcheck.hpp>
#include <defmodel/geo_kw/curve_model.hpp>

#include <cmath>

#include <defrt/ftn/helpers.hpp>

namespace def::cmd::v3 {

double normalize(double* v) { return def::rt::ftn::vec3_normalize(v); }

// @src 14023bd90
int project(const double* p, const double* a, const double* b, double* t, double* d2, double* q)
{
    const double dx = b[0] - a[0];
    const double dy = b[1] - a[1];
    const double dz = b[2] - a[2];
    *t = (p[0] - a[0]) * dx + (p[1] - a[1]) * dy + (p[2] - a[2]) * dz;
    *t = *t / (dx * dx + dy * dy + dz * dz);
    q[0] = a[0] + dx * *t;
    q[1] = a[1] + dy * *t;
    q[2] = a[2] + dz * *t;
    *d2 = dist2(p, q);
    return 0;
}

// 14023b850: ported in defdbio/gendb/pre_steps.hpp (vec3_dist2)
double dist2(const double* a, const double* b) { return dbio::gendb::vec3_dist2(a, b); }

// 140239800: ported in defdbio/objcheck/objcheck.hpp (vec3_copy)
V3 make(const double* a)
{
    double r[3];
    dbio::objcheck::vec3_copy(r, a);
    return V3{r[0], r[1], r[2]};
}

// 140239b40: ported in defdbio/gendb/obj_counts.hpp (vec3_normalized); comisd + jbe: a NaN
// length also gives 0
V3 unit(const V3& a)
{
    const double v[3] = {a.x, a.y, a.z};
    double r[3];
    dbio::gendb::vec3_normalized(r, v);
    return V3{r[0], r[1], r[2]};
}

// 140239ea0: ported in defdbio/objcheck/objcheck.hpp (vec3_scale)
V3 mul(const V3& a, double s)
{
    const double v[3] = {a.x, a.y, a.z};
    double r[3];
    dbio::objcheck::vec3_scale(r, v, s);
    return V3{r[0], r[1], r[2]};
}

// @src 140308810
V2 unit(const V2& a)
{
    V2 r = a;
    const double n = len(a);
    if (!(std::fabs(n) > 1e-12)) {  // jbe: NaN also gives 0
        r.x = 0.0;
        r.y = 0.0;
    } else {
        r.x = r.x / n;
        r.y = r.y / n;
    }
    return r;
}

// 1403091c0: ported in defmodel/geo_kw/curve_model.hpp (geo_kw::normalize2)
double normalize2(double* v)
{
    return model::geo_kw::normalize2(v);
}

} // namespace def::cmd::v3
