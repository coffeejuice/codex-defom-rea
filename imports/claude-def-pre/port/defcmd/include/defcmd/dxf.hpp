// defcmd/dxf.hpp: the in-process DXF reader of GFREAD (packet p8-04), public API.
//
// Spec: docs/re/commands/DXF_READER.md. Reached from GFREAD on a 2D object whose file extension is
// DXF/dxf: p8-01's 2D front end (FUN_140244500) builds a Uic2CObjectDxfFile on the stack and calls
// fun_1404093a0(dxf, file, "", obj, 2, 300); on 0 it copies dxf+0x1c into obj+0xbc8.
//
// Class Uic2CObjectDxfFile (RTTI .?AVUic2CObjectDxfFile@@, base Uic2CObject, p8-03's
// defcmd/uic2c_object.hpp):
//   vtable   0x140563800, 2 slots: 0 = scalar deleting dtor 140409890 (p8-04; the compiler
//            generates it from the virtual dtor), 1 = class name 140409880 (p8-04)
//   ctor     1404092b0 (body p8-01): base ctor 140454900(this, name), vptr, +0x18 = +0x1c = 0,
//            +0x20 = 0.0, then 140409330 (p8-01, the same three stores again). Every caller
//            passes name = 0.
//   dtor     140409370 (body p8-01): vptr, base dtor 140454980
//   size     0x28
// The class lives in namespace def::cmd (p8-01's gfread.hpp forward-declares
// def::cmd::Uic2CObjectDxfFile); def::cmd::dxf::Uic2CObjectDxfFile names the same class.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defcmd/dxf_uic2c_base.hpp>

namespace def::model {
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd {

class Uic2CObjectDxfFile : public Uic2CObject {
public:
    // ctor 1404092b0 (p8-01's body): the C++ base ctor runs 140454900, the member stores are
    // done here and once more by 140409330 (p8-01's) when its header is present
    explicit Uic2CObjectDxfFile(const char* name = nullptr);
    Uic2CObjectDxfFile(const Uic2CObjectDxfFile&) = delete;
    Uic2CObjectDxfFile& operator=(const Uic2CObjectDxfFile&) = delete;
    // dtor 140409370 (p8-01's body = the base dtor); slot 0 is 140409890 (p8-04)
    ~Uic2CObjectDxfFile() override;
    // slot 1 (140409880): "Uic2CObjectDxfFile"
    const char* class_name() const override;

    std::int32_t u18;  // 0x18 ctor 0 (no reader on the DXF path)
    std::int32_t u1c;  // 0x1c boundary count (140409970 writes, 1402566c0 reads; p8-01 copies it
                       //      to obj+0xbc8)
    double u20;        // 0x20 tolerance (1404099a0 writes, 140409990 reads; MOVSD, a double)
};
static_assert(sizeof(Uic2CObjectDxfFile) == 0x28);
static_assert(offsetof(Uic2CObjectDxfFile, u18) == 0x18);
static_assert(offsetof(Uic2CObjectDxfFile, u1c) == 0x1c);
static_assert(offsetof(Uic2CObjectDxfFile, u20) == 0x20);

} // namespace def::cmd

namespace def::cmd::dxf {

using def::cmd::Uic2CObjectDxfFile;

// @src 1404093a0  Uic2CObjectDxfFile read: DXF file -> curve 0 of obj's geometry 0 (type-2
// segment curve). mode 300 is the GFREAD path; any other mode delegates to the IGES reader
// 140409af0 (p8-03) with the DXF object's tolerance. Returns 0 on success, -1 (obj NULL, no
// segment array, allocation failure: GFREAD "Failed"), -2 (no file name), -3 (file not
// readable), or the parse / conversion codes of DXF_READER.md section 12. kind is unused on the
// mode-300 path (GFREAD passes 2).
int fun_1404093a0(Uic2CObjectDxfFile* dxf, const char* file, const char* prefix,
                  def::model::LLOBJ_DATA* obj, std::int32_t kind, std::int32_t mode);
inline int read_dxf(Uic2CObjectDxfFile* dxf, const char* file, const char* prefix,
                    def::model::LLOBJ_DATA* obj, std::int32_t kind, std::int32_t mode)
{
    return fun_1404093a0(dxf, file, prefix, obj, kind, mode);
}

// @src 1404099c0  the DXF reader's global tolerance G (DAT_14062bc68): 0.01 * size of the last DXF
// read, initially 1e-9; also the arbitrary-axis threshold of the OCS transform.
double fun_1404099c0();
// @src 140428f70  G = v
void fun_140428f70(double v);
// Process start value of G (tests: the original starts a new process per case).
void reset_globals();

} // namespace def::cmd::dxf
