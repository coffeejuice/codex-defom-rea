// CNVT3D per-object entry and its message / exchange-file helpers (packet p9-03a).
// Spec: CNVT3D_SWEEP.md section 2, CNVT3D.md sections 5 and 7.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/cnvt3d_sweep.hpp>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>

#include <defcmd/meshgen/meshgen_base.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "internal.hpp"

namespace def::cmd::cnvt3d_sweep {

using namespace detail;

// @src 140159030  floorf (140485048)
float fun_140159030(float x) { return std::floor(x); }

// @src 14013c340  message with the "[i/n] " prefix (n > 1): CONV_STATUS.DAT ("a"), then
//                 <stem minus ".DB">.LOG ("a") and stdout; nothing more when an open fails
void fun_14013c340(const char* msg, const char* stem)
{
    std::FILE* st = std::fopen("CONV_STATUS.DAT", "a");  // 1401b1880 / 1401b18f0
    if (st == nullptr) return;
    const std::int32_t i = *E().dat_1406e2d84();
    const std::int32_t n = *E().dat_1406e2d88();
    char prefix[16];
    if (1 < n) {
        std::sprintf(prefix, "[%d/%d] ", i, n);
        std::fwrite(prefix, 1, std::strlen(prefix), st);  // 1401b1970
    }
    std::fwrite(msg, 1, std::strlen(msg), st);
    std::fwrite("\n", 1, 1, st);
    std::fflush(st);  // 1401b1940
    std::string s(stem);  // 1400066a0
    if (E().fun_1401b0cc0(s.c_str(), ".DB")) {
        const std::int32_t len = static_cast<std::int32_t>(s.size());  // 140002800
        s = E().fun_1401b1520(s, len - 3);
    }
    char name[1024];
    std::sprintf(name, "%s%s", s.c_str(), ".LOG");
    std::FILE* lg = std::fopen(name, "a");
    if (lg == nullptr) {
        std::fclose(st);  // 1401b18b0
        return;
    }
    if (1 < n) std::fwrite(prefix, 1, std::strlen(prefix), lg);
    std::fwrite(msg, 1, std::strlen(msg), lg);
    std::fwrite("\n", 1, 1, lg);
    std::fflush(lg);
    if (1 < n) std::fprintf(stdout, "%s", prefix);
    std::fprintf(stdout, "%s\n", msg);
    std::fclose(lg);
    std::fclose(st);
}

// @src 14013d7c0  remove DEF_INT_3DTET.DAT (0x14047e29c)
void fun_14013d7c0() { std::remove("DEF_INT_3DTET.DAT"); }

// @src 14013d7e0  DEF_INT_3DTET.DAT ("w"): vals[0..n-1] as "%1d\n"
void fun_14013d7e0(std::int32_t n, const std::int32_t* vals)
{
    std::FILE* f = std::fopen("DEF_INT_3DTET.DAT", "w");
    if (f == nullptr) return;
    char line[16];
    for (std::int32_t k = 0; k < n; k = k + 1) {
        std::sprintf(line, "%1d\n", vals[k]);
        std::fwrite(line, 1, std::strlen(line), f);
    }
    std::fflush(f);
    std::fclose(f);
}

// @src 140147b70  generate geometry (kind 5), a tet mesh (2) or a brick mesh (4) of object id
void fun_140147b70(MeshGenBase* cvt, std::int32_t id, std::int32_t nsec, std::int32_t m,
                   double start, double ang, double c, std::int32_t kind)
{
    DB_STEP_DATA* m2 = cvt->u3f48;
    if (m2 == nullptr) return;
    std::int32_t kd = E().fun_1401ee4c0(&m2->sim_ctrl);
    cvt->u008 = E().fun_1401d6ea0(m2, id);
    const std::int32_t g = at<std::int32_t>(cvt->u008, 0x2be8);  // GEO23 mode
    if (kd != g && 0 < g && g < 3) kd = g;
    E().fun_140148840(cvt, cvt->u008, m2, static_cast<const char*>(ptr(m2, 0x340)));
    if (kind == 4) {
        if (at<std::int32_t>(cvt->u008, 0x2d80) != at<std::int32_t>(cvt->u008, 0x884)) {
            // 140147cd8: IDIV (nsec - 1 == 0 faults in the binary), CVTDQ2PS, floorf, CVTTSS2SI
            const std::int32_t q = m / (nsec - 1);
            const float fq = fun_140159030(static_cast<float>(q));
            E().fun_140148960(cvt, static_cast<std::int32_t>(fq));
        }
        if (kd == 1) {
            E().fun_140148f70(cvt, nsec, start, ang, c, cvt->u008);
        } else if (kd == 2) {
            E().fun_14014c0a0(cvt, nsec, start, ang, cvt->u008);
        }
    } else if (kind == 2) {
        MeshGenBase mg;  // 14013e790 .. 14013e840
        fun_140153710(cvt, kd, start, ang, cvt->u008, &mg, c);
        fun_140153910(cvt, kd, start, ang, mg.u008);
        fun_14013c340("Generating surface mesh...", cvt->u4360);
        E().fun_14013ee00(&mg, 0);
        if (*E().dat_1406e2d74() != 1) return;
        fun_14013c340("Generating solid mesh...", cvt->u4360);
        E().fun_140140110(&mg, 1);
        fun_14013c340("Assigning state variables...", cvt->u4360);
        E().fun_140148a20(cvt, mg.u008);
    }
    if (kind == 5) {
        fun_14013c340("Creating geometry...", cvt->u4360);
        MeshGenBase mg;
        fun_140153710(cvt, kd, start, ang, cvt->u008, &mg, c);
    }
}

} // namespace def::cmd::cnvt3d_sweep
