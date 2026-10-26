// defcmd/dispatch.hpp: the group 6 (command) keyword parser FUN_140378e00 and the model side of
// the KEY loop (packet p5-01). Interface note: docs/re/status/p5-01-dispatcher.md.
//
// FUN_140378e00(model, f, id) is one switch over the registry id of a command keyword. Each case
// reads its own arguments from `f` (positioned right after the keyword name) and returns what the
// dispatcher returns for that keyword: 0 (continue), 2 ("Reading Error") or another status the
// KEY loop passes up (WRTOBJ returns 1 when its file does not open). Cases that "break" in the
// binary end with `status == 0 ? 0 : 2` (14037e1fd), cases that return early return their value.
//
// The case bodies of other packets are called as `int cmd_<name>(model::DB_STEP_DATA*, FILE*)`
// through the hook table below. Every hook defaults to the owner's function when its header is
// present (__has_include in dispatch.cpp), otherwise to a body that stops with DEF_NOT_PORTED.
// Tests may replace hooks and call reset_dispatch_hooks() afterwards.
#pragma once

#include <cstdint>
#include <cstdio>

#include <defkey/key_reader.hpp>

namespace def::model {
class DB_STEP_DATA;
class Cls_1401dbfe0;
class LLOBJ_DATA;
} // namespace def::model

namespace def::cmd {

// Registry ids of the group 6 keywords (keyword_registry_table.hpp).
enum CommandId : int {
    kPROBID = 0, kRUNTYP = 1, kDBREAD = 2, kKFREAD = 3, kGENDB = 4, kINICTC = 5, kGENCTC = 6,
    kOBJPOS = 7, kTLOC = 8, kWRTOBJ = 9, kKFWRIT = 10, kEXECMD = 11, kSVSCAL = 12, kCPYOBJ = 13,
    kDOESCL = 14, kGENINT = 15, kIMPLUB = 16, kIMPEQP = 17, kTCYCLE = 18, kGENAXS = 19,
    kGENSUB = 20, kARMCTL = 21, kDSTROKE = 154, kACTOUT = 286, kCNVT3D = 287, kREMESH = 291,
    kBRDEXT = 292, kDEFAMG = 293, kDEFINT = 294, kGENBCC = 295, kDEFFRZ = 296, kMORP = 297,
    kGFREAD = 298,
};

// @src 140378e00  the group 6 parser. `m` is the model FUN_1401d81b0 returned for the KEY loop.
int dispatch(model::DB_STEP_DATA* m, std::FILE* f, int id);

// One case body: reads the case's arguments from f and returns the dispatcher's result.
using CaseFn = int (*)(model::DB_STEP_DATA* m, std::FILE* f);

struct DispatchHooks {
    // ---- case bodies of other packets (default: owner header, else DEF_NOT_PORTED) ----------
    CaseFn inictc, genctc;                     // p67-6 <defcmd/contact_cmds.hpp>
    CaseFn objpos;                             // p67-1 <defcmd/cmd_objpos.hpp>
    CaseFn wrtobj, svscal, cpyobj, doescl;     // p67-4 <defcmd/cmd_p67_4.hpp>
    CaseFn tcycle, genaxs, gensub, genbcc;     // p67-5 <defcmd/p67_5.hpp>
    CaseFn execmd, genint, implub, impeqp, armctl, morp, gfread; // p8-01 <defcmd/cmd_p8_01.hpp>
    CaseFn brdext;                             // p8-05 <defcmd/cmd_p8_05.hpp>
    CaseFn defamg;                             // p8-07 <defcmd/cmd_p8_07.hpp>
    CaseFn remesh, defint, deffrz;             // p8-08 <defcmd/cmd_p8_08.hpp>
    CaseFn cnvt3d;                             // p9-01 <defcmd/cmd_p9_01.hpp>

    // ---- callees of the cases ported here that other packets own -----------------------------
    // 1401b5f00 KEY file loader (p8-07): (model, name, &reset, &flags) -> 0 or the loop status
    int (*read_keyword_file)(model::DB_STEP_DATA* m, const char* name, const char* reset,
                             const std::int32_t* flags);
    // 1401cc2a0 DBREAD core (Phase 2): read step `step` of the DB in `db` into m; 0 = ok
    // (R9B = 1 from DBREAD, 0 from KFWRIT; the fifth, byte on the stack, is 0 in both)
    int (*db_read_step)(model::DB_STEP_DATA* m, model::Cls_1401dbfe0* db, std::int32_t step,
                        std::uint8_t a, std::uint8_t b);
    // 1401bb550 GENDB write (p4-01): (model, name, a, b, c) as the dispatcher passes R8, R9, [rsp+20]
    // (= write_db(m, path, check, dbopt, signmode) of defdbio/gendb/gendb.hpp)
    std::int32_t (*gendb_write)(model::DB_STEP_DATA* m, const char* name, std::int32_t a,
                                std::int32_t b, std::int32_t c);
    // 1402059a0 3D object surface patch rebuild (Phase 2, unowned), run by 1401d63c0
    void (*object_surface_update)(model::LLOBJ_DATA* obj, bool copy_axis_points);
    // 1402a1df0 per-object writer of the "short" KEY write (1401c5d20, flags bit 31); default
    // def::key::g2geo::write_object_short (defkey/kw_g2geo.hpp, packet p5-shortkey)
    int (*write_object_short)(model::DB_STEP_DATA* m, std::FILE* f, model::LLOBJ_DATA* obj,
                              std::int32_t opt);
};

DispatchHooks& dispatch_hooks();
void reset_dispatch_hooks();

// ---- parts of the cases ported here, public for the menus and tests --------------------------

// @src 1401c5810  KFWRIT / menu 8: writes the KEY file `name`. flags bit 31 selects the short
// writer 1401c5d20. With +0x1b5a set the step m->u744 is first read from the DB into a temporary
// model (DBREAD core) and that model is written. 0 ok, 1 no name / open error, 2 DB read failed.
std::int32_t write_key_file(model::DB_STEP_DATA* m, const char* name, const std::uint32_t* flags);
// @src 1401c5af0  the KEY writer: banner, group 1, materials, phase pairs, objects, inter-objects
int write_key_stream(model::DB_STEP_DATA* m, std::FILE* f);
// @src 1401c5d20  short writer: NSTART (2D only) and every object through 1402a1df0
int write_key_stream_short(model::DB_STEP_DATA* m, std::FILE* f);

// 1401d7f40 (owner P4 gate, defdbio/gendb/pre_steps.hpp; interim copy here): after a KEY read with reset and before GENDB: sim-control DTTYPE mode
// (1401ee510), material links (1401d6250), object surfaces (1401d63c0), restart step sign
void finish_key_read(model::DB_STEP_DATA* m);
// 1401d6250 (interim, see above): for each material with mode (+0x1cc8) 1, the materials it lists lose their own
// list and get mode 2
void reset_linked_materials(model::DB_STEP_DATA* m);
// 1401d63c0 (interim, see above): 1402059a0(obj, flag) for every object whose type (+0xc) is not 7
void update_object_surfaces(model::DB_STEP_DATA* m, bool flag);

// @src 1401e3b60  the step of the DB directory to read for DBREAD n: the last valid entry (+0x24
// != 0) whose step number is not above n in the order of 1401e3d40; 0 when none
std::int32_t db_find_step(const model::Cls_1401dbfe0* db, std::int32_t step);
// @src 1401e3d40  step order: 0 equal; negative step numbers -n sort as n-0.5 (DAT_140522fc8 *
// n - DAT_14051f240); -1 when a sorts before b, else 1
std::int32_t db_step_compare(std::int32_t a, std::int32_t b);

// The model side of the KEY loop 1401b6520 (what 1401b5f00 gives it): groups 1-5 and 6 on
// 1401d81b0(model), group 7 on the model; the companion model of 1401d8120 is the "sub-model"
// whose dimension the header comment also sets (vtable slot 0 = set_dimension).
class ModelKeyHooks : public key::KeyReaderHooks {
public:
    explicit ModelKeyHooks(model::DB_STEP_DATA* m) : m_(m) {}
    bool has_sub_model() override;
    void set_model_dimension(int dim) override;
    void set_sub_model_dimension(int dim) override;
    int parse_keyword(int group, std::FILE* f, int id) override;

private:
    model::DB_STEP_DATA* m_;
};

} // namespace def::cmd
