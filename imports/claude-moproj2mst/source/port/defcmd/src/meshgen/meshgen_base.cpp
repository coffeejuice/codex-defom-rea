// _MeshGenBase constructor, destructor and bind (packet p8-07; ported with the p8-05..p8-08 seams
// so that all four packets can run them from the start).
#include <defcmd/meshgen/meshgen_base.hpp>
#include <defcmd/meshgen/p8_seams.hpp>

#include <cstring>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>

namespace def::cmd {

// @src 14013e790  ctor: vptr, LLOBJ_DATA ctor at +0x18 with owner 0 (14015ec80), then zeroes
// +0x3f48, +0x8, +0x3f50, the bytes +0x4760/+0x4761, +0x4768, +0x4770 and the int +0x3f40.
// +0x10 and the name buffers are left as they are in the original (stack garbage there; zero here).
MeshGenBase::MeshGenBase() {
    std::memset(pad_3f50, 0, 8);
}

// @src 14013e840  dtor: vptr, LLOBJ_DATA dtor of +0x18 (14015f790, the member's destructor here)
MeshGenBase::~MeshGenBase() = default;

// 140140110  slot +0x10: body ported by p8-06 (mesh3d_volume_step)
int MeshGenBase::volume_step(char flag) { return mesh3d_volume_step(*this, flag); }

// @src 140158d50
bool MeshGenBase::vslot20(std::int32_t, std::int32_t, std::int32_t) { return true; }
// @src 140158d70
void MeshGenBase::set_busy(char) {}
// @src 140158d80
bool MeshGenBase::confirm_reverse() { return true; }
// @src 140158d90
bool MeshGenBase::vslot38() { return false; }
// @src 140158da0
void MeshGenBase::report_error(std::int32_t) {}
// @src 140158db0
void MeshGenBase::progress() {}
// @src 140158dc0
void MeshGenBase::vslot50() {}
// @src 140158dd0
std::int32_t MeshGenBase::vslot58() { return 0x14; }

// @src 140148840
void meshgen_bind(MeshGenBase& mg, model::LLOBJ_DATA* obj, model::DB_STEP_DATA* model,
                  const char* prob) {
    mg.u008 = obj;
    mg.u3f48 = model;
    std::strcpy(mg.u3f60, prob);  // 140484f90
    model::llobj_ctor::set_owner(&mg.u018, model);  // 14015ef60
    const bool f9 = true, f8 = false, f7 = true, f6 = true, f5 = true, f4 = true;
    model::llobj_copy_from(&mg.u018, mg.u008, model->u15f8, &f4, &f5, &f6, &f7, &f8, &f9);
    mg.u010 = &mg.u018;
    // object number: *(int*)(u010 + 8) = *(int*)(u008 + 8)
    std::memcpy(reinterpret_cast<std::byte*>(mg.u010) + 8, reinterpret_cast<const std::byte*>(mg.u008) + 8, 4);
}

} // namespace def::cmd
