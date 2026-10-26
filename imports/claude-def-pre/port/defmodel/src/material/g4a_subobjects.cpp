// Ctors and dtors of the Material sub-objects whose keywords belong to packet kw-g4a: FSTRES
// (Cls_140288ae0), CREEP (Cls_140289b80), BURGRS..RECVRY (Cls_14028bcc0), FRCMOD/UTSDAT
// (Cls_14028c040), ANISO (Cls_140289fe0). Decompile: deform-rea-work/out/kw457/ctors.c and
// out/p1/p1_all.c. The DYNARY members are constructed by their own ctor (14012d050) in
// declaration order, which is the order of the original's calls; the dtors are the member dtors
// (14012f410) in reverse order, which C++ does implicitly.
#include <defmodel/dynary_init.hpp>
#include <defmodel/layout/cls_140288ae0.hpp>
#include <defmodel/layout/cls_140289b80.hpp>
#include <defmodel/layout/cls_140289fe0.hpp>
#include <defmodel/layout/cls_14028bcc0.hpp>
#include <defmodel/layout/cls_14028c040.hpp>

namespace def::model {

// @src 140288ae0  FSTRES block ctor: model 1 (KEY type 3 table), coefficients 0, DYNARY types
Cls_140288ae0::Cls_140288ae0()
{
    model = 1;
    c_t1[3] = 0.0;
    c_t1[2] = 0.0;
    c_t1[1] = 0.0;
    c_t1[0] = 0.0;
    dynary_init(&table, 0x4f);
    for (int i = 4; i >= 0; --i) c_t4[i] = 0.0;
    for (int i = 3; i >= 0; --i) c_t5[i] = 0.0;
    dynary_init(&h, 0x25);
    dynary_init(&y, 0x24);
    dynary_init(&s, 0x50);
    dynary_init(&p, 0x51);
    for (int i = 13; i >= 0; --i) c_t9[i] = 0.0;
    for (int i = 5; i >= 0; --i) c_t10[i] = 0.0;
    for (int i = 4; i >= 0; --i) c_t11[i] = 0.0;
    for (int i = 3; i >= 0; --i) c_t14[i] = 0.0;
    dynary_init(&bmd_n, 0xae);
    dynary_init(&f, 0x26);
    cnt = 0;
    dynary_init(&a, 0xa6);
    dynary_init(&b, 0xa7);
    dynary_init(&c, 0xa8);
    user_routine = 0;
}

// @src 140289b80  CREEP block ctor: model 0, DYNARY types 0x43..0x48 and 0xa9..0xab, cnt -1
Cls_140289b80::Cls_140289b80()
{
    model = 0;
    dynary_init(&d1, 0x45);
    dynary_init(&d2, 0x46);
    dynary_init(&d3, 0x47);
    dynary_init(&d4, 0x48);
    dynary_init(&d6, 0x43);
    dynary_init(&d5, 0x44);
    user_routine = 0;
    cnt = -1;
    dynary_init(&a, 0xa9);
    dynary_init(&b, 0xaa);
    dynary_init(&c, 0xab);
}

// @src 14028bcc0  BURGRS/ALPHA/NDISFM/RECVRY block ctor: four DYNARYs of type 0, nothing else
Cls_14028bcc0::Cls_14028bcc0() = default;

// @src 14028c040  FRCMOD/UTSDAT block ctor: zeroes, DYNARY types 0x1b (UTSDAT), 0x1c, 0xb4
Cls_14028c040::Cls_14028c040()
{
    model = 0;
    e = 0.0;
    b_t2 = 0.0;
    b_t4 = 0.0;
    b_t5 = 0.0;
    c_t7 = 0.0;
    b_t7 = 0.0;
    a = 0.0;
    user_routine = 0;
    for (int i = 0; i < 9; ++i) c_t11[i] = 0.0;
    dynary_init(&uts, 0x1b);
    dynary_init(&g, 0x1c);
    dynary_init(&k, 0xb4);
}

// @src 140289fe0  ANISO block ctor: t 0, DYNARY type 0, ref 0
Cls_140289fe0::Cls_140289fe0()
{
    t = 0;
    ref = 0;
}

} // namespace def::model
