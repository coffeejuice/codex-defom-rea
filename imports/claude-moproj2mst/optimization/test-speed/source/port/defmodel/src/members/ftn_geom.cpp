// Two small PathScale Fortran routines of the p1-10 range, called by the geometry code
// (140325691 from 140257750, 140345f5b from 1402763d0). Arguments are by reference as in the
// binary; SAVE storage is in defmodel/globals_p1_10.hpp. FP order follows the assembly.
#include <defmodel/globals_p1_10.hpp>
#include <defmodel/members/ext_deps.hpp>
#include <defmodel/members/ftn_geom.hpp>

namespace def::model {

void reset_globals_p1_10()
{
    for (double& v : g_rot_140701790)
        v = 0.0;
    dat_1407017e0 = dat_1407017e8 = 0.0;
    for (double& v : g_work_1407017f0)
        v = 0.0;
    dat_140b74428 = dat_140b74430 = 0.0;
    dat_140b84720 = dat_140b84728 = dat_140b84730 = 0.0;
    dat_140702710 = dat_140702718 = dat_140702720 = dat_140702728 = 0.0;
}

// @src 140325691  rotate p(3) by angle_deg about the axis `dir` through `origin`
void fun_140325691(double* p, const double* origin, const double* dir, const double* angle_deg)
{
    ext::fun_1404cf5c0(dir, g_rot_140701790);
    // 1403256ce: (angle / 45.0) * (pi/4)   str 140baea58 = 45.0, 140baea60 = 0.7853981633974483
    double a = *angle_deg / 45.0;
    a = a * 0.7853981633974483;
    double s, c;
    ext::fun_1404b3b30(a, &s, &c);
    std::int32_t n = 1;
    std::int32_t mode = 1;
    dat_1407017e0 = s;
    dat_1407017e8 = c;
    ext::fun_1404ded0c(p, g_work_1407017f0, origin, g_rot_140701790, &n, &mode);
    // 1403257a5: x' = w0*c - s*w1, y' = s*w0 + w1*c
    double w0 = g_work_1407017f0[0];
    double w1 = g_work_1407017f0[1];
    double sw1 = dat_1407017e0 * w1;
    double sw0 = dat_1407017e0 * w0;
    double cw0 = w0 * dat_1407017e8;
    double cw1 = w1 * dat_1407017e8;
    double x = cw0 - sw1;
    double y = sw0 + cw1;
    p[2] = g_work_1407017f0[2];
    n = 1;
    p[0] = x;
    mode = 2;
    p[1] = y;
    g_work_1407017f0[1] = y;
    g_work_1407017f0[0] = x;
    ext::fun_1404ded0c(p, g_work_1407017f0, origin, g_rot_140701790, &n, &mode);
    p[2] = g_work_1407017f0[2];
    p[1] = g_work_1407017f0[1];
    p[0] = g_work_1407017f0[0];
}

// @src 140345f5b  largest coordinate range (columns 1 and 2) of n points stored xyz(3, n)
void fun_140345f5b(const double* xyz, const std::int32_t* n)
{
    std::int32_t ld = 3;
    std::int32_t comp = 1;
    std::int32_t init = 1;
    dat_140b84728 = dat_140b74430;
    dat_140b84720 = dat_140b74428;
    ext::fun_140437aa9(xyz, &ld, n, &comp, &dat_140702710, &dat_140702718, &init);
    init = 1;
    comp = 2;
    ld = 3;
    ext::fun_140437aa9(xyz, &ld, n, &comp, &dat_140702720, &dat_140702728, &init);
    double r1 = dat_140702718 - dat_140702710;
    double r2 = dat_140702728 - dat_140702720;
    dat_140b84730 = (r2 > r1) ? r2 : r1; // 14034609e MAXSD r2, r1
}

} // namespace def::model
