// defcmd/meshgen/meshgen_base.hpp: _MeshGenBase (RTTI .?AV_MeshGenBase@@), the stack
// mesh-generation context that BRDEXT, DEFAMG, DEFINT, DEFFRZ, REMESH and CNVT3D build around
// one object. Owner: packet p8-07 (docs/phase5_8_9_10-packets.md). Other packets (p8-05, p8-06,
// p8-08, p9-01, p9-03) add the members they need at their recovered offsets and never move one;
// the p8-05..p8-08 integration resolves overlaps.
//
//   vtable   0x14051f148 (10 slots, see DEFAMG.md §3): +0x08 14013ee00, +0x10 140140110,
//            +0x18 14013f010, +0x30 returns 1, +0x58 returns 0x14, +0x28/+0x40/+0x48/+0x50 empty
//   ctor     14013e790, dtor 14013e840, bind 140148840(mg, obj, model, model+0x340)
//   size     0x4be0 (19424 bytes on the stack of 1401a9ac0 / 1401a9be0 / 1401aa000)
//   members  from the specs (DEFAMG.md, DEFINT.md, DEFFRZ.md, BRDEXT.md); unknown gaps are pads.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/llobj_data.hpp>

namespace def::model {
class DB_STEP_DATA;
}

namespace def::cmd {

class MeshGenBase {
public:
    // @src 14013e790  ctor
    MeshGenBase();
    // @src 14013e840  dtor; slot +0x00 is the scalar deleting dtor 140158de0 (dtor, free if flag & 1)
    // @src 140158de0
    virtual ~MeshGenBase();

    // Virtual slots after the destructor in the order of vtable 0x14051f148 (12 slots, owner p8-07).
    // No subclass is known; the bodies are in src/meshgen/meshgen_base.cpp.
    // @src 14013ee00  +0x08 3D surface step (DEF_AMG3.INI ... DEF_AMG3_GUI, MESH3D.INP read-back)
    virtual int surface_step(char flag);
    // @src 140140110  +0x10 3D volume step: calls p8-06's mesh3d_volume_step(*this, flag)
    virtual int volume_step(char flag);
    // @src 14013f010  +0x18 read MESH3D.INP into the object (surface mesh); false on failure
    virtual bool read_mesh3d_inp();
    // @src 140158d50  +0x20 returns true (three int arguments, unused)
    virtual bool vslot20(std::int32_t a, std::int32_t b, std::int32_t c);
    // @src 140158d70  +0x28 empty (byte argument: 1 before / 0 after a long step; UI busy hook)
    virtual void set_busy(char on);
    // @src 140158d80  +0x30 returns true (asked before reversing a 2D curve)
    virtual bool confirm_reverse();
    // @src 140158d90  +0x38 returns false
    virtual bool vslot38();
    // @src 140158da0  +0x40 empty (int argument: the failing step code)
    virtual void report_error(std::int32_t code);
    // @src 140158db0  +0x48 empty (progress hook, the code is in g_mesher_progress)
    virtual void progress();
    // @src 140158dc0  +0x50 empty
    virtual void vslot50();
    // @src 140158dd0  +0x58 returns 0x14
    virtual std::int32_t vslot58();

    model::LLOBJ_DATA* u008 = nullptr;      // +0x008 bound object (140148840 arg 2)
    model::LLOBJ_DATA* u010 = nullptr;      // +0x010 working object, = &u018 after the bind
    model::LLOBJ_DATA u018{nullptr};        // +0x018 copy of the object (1401613a0)
    std::int32_t u3f40 = 0;                 // +0x3f40 interpolation mode (1; 2 = forced, DEFFRZ)
    std::byte pad_3f44[4];
    model::DB_STEP_DATA* u3f48 = nullptr;   // +0x3f48 model
    std::byte pad_3f50[0x10];               // +0x3f50 (ctor zeroes 0x3f50)
    char u3f60[0x400];                      // +0x3f60 problem id (model+0x340)
    char u4360[0x400];                      // +0x4360 DB name ("DEFORM" when none)
    bool u4760 = false;                     // +0x4760 object had a mesh (140144620)
    bool u4761 = false;                     // +0x4761 node temperature uniform (140144810)
    std::byte pad_4762[6];
    double u4768 = 0.0;                     // +0x4768 uniform node temperature (140144810)
    double u4770 = 0.0;                     // +0x4770 GEO23 limit passed to 140155340 (0 from the ctor)
    std::int32_t u4778 = 0;                 // +0x4778 number of source objects
    std::byte pad_477c[4];
    model::LLOBJ_DATA** u4780 = nullptr;    // +0x4780 source-object pointer array
    double u4788 = 0.0;                     // +0x4788 DEFFRZ value (140157670), "%f" line of FRZ1/FRZ3.INI (p8-08)
    char u4790[0x400];                      // +0x4790 DB name as given to DBREAD
    std::int32_t u4b90 = 0;                 // +0x4b90 DB step
    std::byte pad_4b94[4];
    // +0x4b98..+0x4bc8: the numbers DEF_FRZ / DEF_FRZ3 leave in DEF_FRZI.TMP / SCRATCH.DAT, read by
    // 140157890 (5: 4b98, 4ba0 4ba8, 4bb0 4bb8) and 140157be0 (7) with fscanf "%lf"; nothing
    // reads them afterwards (p8-08, DEFFRZ.md section 6)
    double u4b98[7] = {};
    bool u4bd0 = false;                     // +0x4bd0 140157be0: FRZ3.INI line 2 (1401576a0 arg 2; p8-08)
    std::byte pad_4bd1[0x4be0 - 0x4bd1];
};

static_assert(sizeof(MeshGenBase) == 0x4be0);
DEF_LAYOUT_OFFSET(MeshGenBase, u018, 0x018);
DEF_LAYOUT_OFFSET(MeshGenBase, u3f40, 0x3f40);
DEF_LAYOUT_OFFSET(MeshGenBase, u3f48, 0x3f48);
DEF_LAYOUT_OFFSET(MeshGenBase, u3f60, 0x3f60);
DEF_LAYOUT_OFFSET(MeshGenBase, u4360, 0x4360);
DEF_LAYOUT_OFFSET(MeshGenBase, u4760, 0x4760);
DEF_LAYOUT_OFFSET(MeshGenBase, u4768, 0x4768);
DEF_LAYOUT_OFFSET(MeshGenBase, u4778, 0x4778);
DEF_LAYOUT_OFFSET(MeshGenBase, u4780, 0x4780);
DEF_LAYOUT_OFFSET(MeshGenBase, u4790, 0x4790);
DEF_LAYOUT_OFFSET(MeshGenBase, u4b90, 0x4b90);
DEF_LAYOUT_OFFSET(MeshGenBase, u4788, 0x4788);
DEF_LAYOUT_OFFSET(MeshGenBase, u4b98, 0x4b98);
DEF_LAYOUT_OFFSET(MeshGenBase, u4bd0, 0x4bd0);

// @src 140148840  bind: u008 = obj, u3f48 = model, u3f60 = prob, copy obj into u018, u010 = &u018
void meshgen_bind(MeshGenBase& mg, model::LLOBJ_DATA* obj, model::DB_STEP_DATA* model,
                  const char* prob);

} // namespace def::cmd
