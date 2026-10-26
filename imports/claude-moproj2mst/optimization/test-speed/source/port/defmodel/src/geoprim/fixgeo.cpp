// FIXGEO and the outline file reader/writer (packet p9-01). Spec: defmodel/geoprim/fixgeo.hpp.
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defmodel/geoprim/fixgeo.hpp>

#include <cmath>
#include <cstdio>
#include <share.h>

#include <defmodel/layout/obj_geo2d.hpp>
#include <defrt/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::model::geoprim {

namespace {

constexpr double kRad = 0.017453292519943295;  // DAT_140522fd0
constexpr double kDeg = 57.29577951308232;     // DAT_140522fd8

// 14047b934: fopen = _fsopen(name, mode, _SH_DENYNO)
std::FILE* open(const char* name, const char* mode) { return _fsopen(name, mode, _SH_DENYNO); }

// 1401b85d0 (defdbio's remove_if_exists; not reachable from defmodel): remove the file when it
// can be opened "rt"
void remove_if_exists(const char* name)
{
    std::FILE* f = open(name, "rt");
    if (f == nullptr) return;
    std::fclose(f);
    std::remove(name);
}

// 1401b8560 (defdbio's file_exists): the file opens "r"
bool file_exists(const char* name)
{
    if (name == nullptr || *name == '\0') return false;
    std::FILE* f = open(name, "r");
    if (f == nullptr) return false;
    std::fclose(f);
    return true;
}

} // namespace

// @src 140220390  FIXGEO options ctor
FixgeoOptions::FixgeoOptions()
    : tol_a(0.001), tol_b(0.001), angle_a(20.0), angle_b(300.0), i20(0), i24(0), b28(0), b29(0),
      pad_2a{}
{
}

// @src 140220420  dtor (empty)
FixgeoOptions::~FixgeoOptions() = default;

// @src 14027f450
double* geo_points(const OBJ_GEO2D* geo) { return geo->points; }

// @src 14027cc00  outline file writer
std::int32_t write_geo_file(const OBJ_GEO2D* geo, const char* name)
{
    if (name == nullptr) return -2;
    const std::int32_t type = geo->curve_type;
    const std::int32_t n = geo->n_points;
    const std::int32_t dof = geo->dof();  // 1402760e0
    std::FILE* f = open(name, "wt");      // str 14053695c
    if (f == nullptr) return -3;
    std::fprintf(f, "  %d\n", type + 1);
    std::fprintf(f, "  %d   %d\n", n, 1);
    const double* p = geo->points;
    if (type == 1) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            std::fprintf(f, " %d", i + 1);
            for (std::int32_t k = 0; k < dof; k = k + 1) rt::crt::fprintf(f, " %20.10e", p[i * dof + k]);
            std::fprintf(f, "\n");
        }
    } else {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            std::fprintf(f, " %d    %d", i + 1, static_cast<std::int32_t>(p[i * dof]));  // cvttsd2si
            std::int32_t k = 1;
            for (; k < dof - 1; k = k + 1) rt::crt::fprintf(f, " %20.10e", p[i * dof + k]);
            rt::crt::fprintf(f, " %20.10e", p[i * dof + k] * kDeg);
            std::fprintf(f, "\n");
        }
    }
    std::fclose(f);
    return 0;
}

// @src 14027ce90  outline file reader
std::int32_t read_geo_file(OBJ_GEO2D* geo, const char* name)
{
    if (name == nullptr) return -2;
    std::FILE* f = open(name, "rt");  // str 140536e64
    if (f == nullptr) return -3;
    std::int32_t ftype = 0, n = 0, dummy = 0;
    std::fscanf(f, "%d\n", &ftype);
    std::fscanf(f, "%d%d\n", &n, &dummy);
    if (0 < ftype) {
        if (ftype < 3) geo->curve_type = 1;
        else if (ftype == 3) geo->curve_type = 2;
    }
    geo->n_points = n;
    geo->clear_data();  // vtable +0x18 (slot 3: allocate_points)
    const std::int32_t dof = geo->dof();
    double* p = geo->points;
    if (ftype == 1) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            const std::int32_t b = i * dof;
            std::fscanf(f, "%d%lf%lf\n", &dummy, &p[b], &p[b + 1]);
            p[b + 2] = 0.0;
        }
    } else if (ftype == 2) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            const std::int32_t b = i * dof;
            std::fscanf(f, "%d%lf%lf%lf\n", &dummy, &p[b], &p[b + 1], &p[b + 2]);
        }
    } else if (ftype == 3) {
        for (std::int32_t i = 0; i < n; i = i + 1) {
            const std::int32_t b = i * dof;
            std::fscanf(f, "%d", &dummy);
            for (std::int32_t k = 0; k < dof; k = k + 1) std::fscanf(f, "%lf", &p[b + k]);
            std::fscanf(f, "\n");
            p[b - 1 + dof] = p[b - 1 + dof] * kRad;
        }
    }
    std::fclose(f);
    geo->set_flag28(true);  // 14027f410(geo, 1)
    return 0;
}

// @src 14027efa0  points left of the axis
std::int32_t negative_x_check(OBJ_GEO2D* geo)
{
    std::int32_t flag = 0;
    double mn[2], mx[2], radius;
    geo->bounding_box(mn, mx, &radius);  // 140279040
    const double w = std::fabs(mx[0] - mn[0]);
    if (!(1e-6 < w || std::isnan(w))) return flag;  // COMISD 1e-6, w; JC (also unordered)
    const double tol = 0.0005 * w;                   // DAT_140530d98
    OBJ_GEO2D c;                                     // 140275cd0
    c.assign(geo);                                   // 140276140
    if (c.curve_type == 2) c.set_curve_type(1);      // 1402763d0
    const std::int32_t dof = c.dof();
    for (std::int32_t i = 0; i < c.n_points; i = i + 1) {
        const double x = geo_points(&c)[i * dof];
        if (0.0 > x) {
            if (flag == 0) flag = 1;
            if (std::fabs(geo_points(&c)[i * dof]) > tol) return -1;  // dtor 140275e00 on return
        }
    }
    return flag;
}

// @src 14027ece0  FIXGEO
std::int32_t fixgeo(OBJ_GEO2D* geo, const FixgeoOptions* opt, std::int32_t* neg,
                    std::int32_t del_pntgeo, char check)
{
    remove_if_exists("DEF_GEO.TMP");
    if (write_geo_file(geo, "FIXGEO_BUF.GEO") != 0) return -1;
    std::FILE* f = open("FIXGEO.INI", "wt");  // str 14053779c
    if (f == nullptr) return -2;
    const double tol = opt->i20 == 0 ? opt->tol_a : opt->tol_b;
    const double a = opt->angle_a * 0.5 * kRad;  // DAT_14051f240, then DAT_140522fd0
    const double b = opt->angle_b * kRad;
    rt::crt::fprintf(f, "%s\n%f  %f  %f\n%d  %d\n%d  %d\n", "FIXGEO_BUF.GEO", tol, a, b,
                     static_cast<std::int32_t>(opt->b28), static_cast<std::int32_t>(opt->b29), opt->i20,
                     opt->i24);
    std::fclose(f);
    char cmd[0x1000];
    std::sprintf(cmd, "%s > %s", "FIXGEO.EXE", "DEF_GEO.TMP");
    if (g_fixgeo_launch == nullptr) DEF_NOT_PORTED(0x140224100);
    g_fixgeo_launch(1, cmd);  // 140224100: dirKind 2
    remove_if_exists("FIXGEO.INI");
    if (del_pntgeo != 0) remove_if_exists("PNTGEO.GEO");
    remove_if_exists("FIXGEO_BUF.GEO");
    remove_if_exists("DEF_GEO.TMP");
    if (!file_exists("NEWGEO.GEO")) {
        if (check != 0) *neg = negative_x_check(geo);
        return 0;
    }
    if (read_geo_file(geo, "NEWGEO.GEO") != 0) return -3;
    if (check != 0) *neg = negative_x_check(geo);
    return 1;
}

} // namespace def::model::geoprim
