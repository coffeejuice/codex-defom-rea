// Calls of packet p9-03b (CNVT3D sweep half b) into functions whose owner is not merged on this
// branch. Each goes through a function pointer named after the callee's address (the p1-07
// pending.hpp scheme): the default forwards to the owner when its header is present
// (__has_include), otherwise it aborts in DEF_NOT_PORTED. Tier-1 tests install fakes and call
// reset_ext(). When the owner lands, the default picks it up through the __has_include branch or
// the merge points the pointer at it (one line each).
//
// Owners: p9-03a (the other half: 14013d7e0, 14013d7c0, 1402ac8a0, 140212460), p8-07
// (140155340 axis snapping, 140144a70 2D remesh), p8-08 (1401563c0 NEW.MSH, 1401f3660 /
// 14029b630 model copies, 140180a40, 1401b1520), p9-01 (14013c6b0 status log), KEY loader
// 1401b5f00 (assigned to p8-07 for the p8/p9 packets).
#pragma once

#include <cstdint>
#include <string>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd {
class MeshGenBase;
}

namespace def::cmd::cnvt3d_sweep::ext {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;

struct Table {
    // ---- p9-03a ----
    // 14013d7e0: DEF_INT_3DTET.DAT ("w"), n lines "%1d\n" of vals
    void (*fun_14013d7e0)(std::int32_t n, const std::int32_t* vals);
    // 14013d7c0: remove DEF_INT_3DTET.DAT
    void (*fun_14013d7c0)();
    // 1402ac8a0: STEP.DAT / DEF_INT_NO_DB.DAT for DEF_INT (DEFINT.md)
    void (*fun_1402ac8a0)(DB_STEP_DATA* m, const char* db, LLOBJ_DATA* obj);
    // 140212460: owner model ? 1401d72e0(owner, i) : 0
    std::int32_t (*fun_140212460)(LLOBJ_DATA* obj, std::int32_t i);
    // ---- p8-07 ----
    // 140155340: axis snapping of the 2D source (CNVT3D_SWEEP.md 7)
    void (*fun_140155340)(MeshGenBase* cvt, LLOBJ_DATA* obj, double c);
    // 140144a70: 2D AMG remesh of cvt+8 (DEFAMG.md)
    void (*fun_140144a70)(MeshGenBase* cvt, const char* flag);
    // 1401b5f00: KEY loader (model, file, &flag, &opt)
    std::int32_t (*fun_1401b5f00)(DB_STEP_DATA* m, const char* file, const char* flag,
                                  const std::int32_t* opt);
    // ---- p8-08 ----
    // 1401563c0: NEW.MSH writer of cvt->u010 ("wt"); false when it cannot be written
    bool (*fun_1401563c0)(MeshGenBase* cvt, const char* name);
    // 1401f3660: copy simulation control (dst = model+0x750, src = model+0x750)
    void (*fun_1401f3660)(void* dst, const void* src);
    // 14029b630: copy the material list (model+0x1328)
    void (*fun_14029b630)(void* dst, const void* src);
    // 140180a40: node set accessor of state variable 3 (+0x20)
    double* (*fun_140180a40)(void* set);
    // 1401b1520: s truncated to n characters when 0 < n < length
    std::string (*fun_1401b1520)(std::string s, int n);
    // ---- p9-01 ----
    // 14013c6b0: append a message to CONV_STATUS.DAT, <stem>.LOG and stdout
    void (*fun_14013c6b0)(const char* msg, const char* stem);
};

extern Table g;
// restore every pointer to its default
void reset_ext();

} // namespace def::cmd::cnvt3d_sweep::ext
