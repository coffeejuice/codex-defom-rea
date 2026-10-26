// case 0x12 TCYCLE (packet p67-5). Spec: docs/re/commands/TCYCLE.md.
#include <defrt/ftn/sse_ops.hpp>
#include <defcmd/p67_5.hpp>

#include <cstdint>

#include <defdbio/step/db_header_cache.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/rdata.hpp>

namespace def::cmd {

namespace {
// rdata 14051bed8  0.0
constexpr double kZero = def::rt::rdata::d_14051bed8;
// rdata 140509c90  1.0
constexpr double kOne = def::rt::rdata::d_140509c90;
} // namespace

// part of @src 140378e00: dispatcher case 0x12 (listing 0x14037d981..0x14037dc51)
int cmd_tcycle(model::DB_STEP_DATA& m, std::FILE* f)
{
    model::Cls_1401dbfe0* db = m.db; // saved at dispatcher entry ([RSP+0x12c8])
    std::int32_t k = 0;              // [RSP+0x6560]
    std::int32_t n = 0;              // [RSP+0x6540]
    double T = 0.0;                  // [RSP+0x6550] zeroed by the case (14037d981)
    double d2 = 0.0;                 // [RSP+0x6548]
    double d3 = 0.0;                 // [RSP+0x6558]
    // FUN_140416100; its status is discarded. The case then tests the dispatcher status
    // [RSP+0x12d4], which is 0 here, so a read failure never returns 2 (14037d9e7).
    (void)key::read_ii_d3(f, &k, &n, &T, &d2, &d3);

    double t_first = 0.0; // [RSP+0x6590]
    double t_last = 0.0;  // [RSP+0x6580]
    double s_first = 0.0; // [RSP+0x6570]
    double s_last = 0.0;  // [RSP+0x65a0]
    std::int32_t sim = dbio::step::entry_sim_no(*db, m.u740); // [RSP+0x6578]
    if (0 < sim) {
        dbio::step::load_header_cache(*db, false);
        if (d2 > kZero) { // 14037da77 COMISD / JBE: NaN does not shift
            sim = sim - 1;
        }
        if (d3 > kZero) {
            sim = sim - 1;
        }
        std::int32_t dim = 0, nobj = 0, pdie = 0, step_first = 0, step_last = 0;
        // result stored in [RSP+0x65b0], never read
        (void)dbio::step::sim_first_last(*db, sim, &dim, &nobj, &pdie, &step_first, &step_last, &t_first,
                                         &t_last, &s_first, &s_last);
        double span = t_last - t_first; // 14037db45 SUBSD
        if (0.0 > span) {               // 14037db5b COMISD 0, span; JBE
            span = 0.0;
        }
        // 14037db7c COMISD span, 0.0; JC skips (span < 0 or NaN); then k > 0
        if (span >= kZero && 0 < k) {
            const double total = rt::sse::add(rt::sse::add(span, d2), d3);                                                 // 14037db98
            const double dt = rt::sse::div(rt::sse::sub(rt::sse::mul(rt::sse::add(kOne, static_cast<double>(n)), T), total),
                                           static_cast<double>(k)); // dest/src as listed (NaN rule) // 14037dbbc
            // 14037dc05 COMISD 0, dt; JC skips the message when 0 < dt or dt is NaN
            if (dt <= 0.0) {
                // str 140555c70, printf 14047af54
                rt::crt::printf("ERROR: Total cycle time(%g) has exceeded defined cycle time(%g)\n", total, T);
            }
            // DTMAX[0], M+0x920 = sim_ctrl+0x1d0, stored also when <= 0
            *reinterpret_cast<double*>(reinterpret_cast<unsigned char*>(&m) + 0x920) = dt;
        }
    }
    return 0;
}

} // namespace def::cmd
