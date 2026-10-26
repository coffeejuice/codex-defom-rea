// PhasePair ctor (packet kw-g57). Layout: defmodel/layout/phase_pair.hpp; dtor and list by p1-10.
// Spec: docs/re/keywords/_common_g457.md section 5, TTTD.md "Defaults".
#include <defmodel/layout/phase_pair.hpp>

#include <cstdio>

#include <defmodel/dynary_init.hpp>

namespace def::model {

namespace {
// constant at 14051f258: 0.01, at 1405501c8: 0.99 (TTTD fractions f1/f2)
constexpr double kTttdF1 = 0.01;
constexpr double kTttdF2 = 0.99;
} // namespace

// @src 140333f60  ctor: the twelve DYNARY members are constructed first (14012d050, in member
// order 0x078 ... 0x6f0), then the fields and the DYNARY types
PhasePair::PhasePair(DB_STEP_DATA* owner_)
{
    owner = owner_;
    next = nullptr;
    from = 0;
    to = 0;
    phastf = 0;
    phastf_on = 0;
    tttd_kind = 0;
    tttd_type = 0;
    std::sprintf(name, " ");
    tttd_nc = 2;
    tttd_x = 0;
    tttd_f1 = 0.0;
    dynary_init(&tttd_t1, 0x2c); // 14012d2a0
    dynary_init(&tttd_t9, 0x37);
    dynary_init(&tttd_vec, 0xb3);
    tttd_f1b = kTttdF1;
    tttd_f2 = kTttdF2;
    tttd_f9[0] = 0.0;
    tttd_f9[1] = 0.0;
    dynary_init(&tttd_t2, 0x30);
    for (double& c : tttd_c3) c = 0.0;
    dynary_init(&tttd_t4, 0x32);
    for (double& c : tttd_c4) c = 0.0;
    for (double& c : tttd_c5) c = 0.0;
    for (double& c : tttd_c6) c = 0.0;
    for (double& c : tttd_c7) c = 0.0;
    dynary_init(&tttd_t8, 0x36);
    tttd_ref = 0;
    pheqvf_t = 0;
    trnsfp_on = 0;
    coarsn_on = 0;
    coarsn_c1 = 0.0;
    coarsn_c2 = 0.0;
    dynary_init(&coarsn, 0x6d);
    dynary_init(&ipsnrg, 0x6e);
    varsel = 0;
}

} // namespace def::model
