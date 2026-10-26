// Packet p67-7, group "pipeline": stage 4 of FUN_1400512aa, FUN_14002cc15 (GENCTC_3d_search.md
// sec. 6.3). Own file so that tests can link it with a test double of the cube attribute store.
#include <defcmd/ctc3d/cube.hpp>
#include <defcmd/ctc3d/decide.hpp>
#include <defcmd/ctc3d/pipeline.hpp>

namespace def::cmd::ctc3d {

namespace {

inline i32& I(std::uint64_t a) { return m<i32>(a); }
inline i32* P(std::uint64_t a) { return mp<i32>(a); }

} // namespace

// @src 14002cc15  stage 4: EI2 = 0 for valid slave polygons whose nodes share a positive BC code
void fun_14002cc15(i32* obj, i32* inst, const i32* nface, const i32* bc, const i32* ndof)
{
    I(0x14064ca28) = 1; // count of the EI2 get / init
    I(0x14064ca2c) = 1;
    if (*nface < 1) {
        return;
    }
    do {
        i32 p = I(0x14064ca2c);
        i32 f2 = 2;
        fun_140228260(obj, inst, &p, &f2, P(0x14064ca30), P(0x14064ca28));
        if (I(0x14064ca30) == 1) {
            i32 p2 = I(0x14064ca2c);
            fun_140227e00(obj, inst, &p2, P(0x14064ca40), P(0x14064ca70));
            fun_14003a978(bc, P(0x14064ca40), P(0x14064ca70), ndof, P(0x14064ca74), P(0x14064ca78));
            if (I(0x14064ca74) > 0) {
                I(0x14064ca30) = 0;
                i32 p3 = I(0x14064ca2c);
                i32 f2b = 2;
                fun_1402280d0(obj, inst, &p3, &f2b, P(0x14064ca30), P(0x14064ca28));
            }
        }
        I(0x14064ca2c) = I(0x14064ca2c) + 1;
    } while (I(0x14064ca2c) <= *nface);
}

} // namespace def::cmd::ctc3d
