// defmodel/step/db_step.hpp: the model root (packet p1-03).
//
// Methods of DB_WRITE_VAR, DB_STEP_DATA, ACTION_DB_STEP, the simulation control block
// Cls_1401e7b10 and the DB info object Cls_1401dbfe0 that are not ctors or dtors. They are free
// functions here so that the layout headers (defmodel/layout/, edited by the skeleton thread only)
// stay untouched; each takes the object first, as the binary passes `this`.
// The DB-reading methods (FUN_1401dc3f0 and what only it calls) are in defdbio/step/.
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/cls_1401dbfe0.hpp>
#include <defmodel/layout/cls_1401e7b10.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/db_write_var.hpp>
#include <defmodel/layout/material.hpp>

namespace def::model {

// ---- raw field access ------------------------------------------------------------------------
// For offsets the decompile uses inside sub-objects whose members are not named yet, or of
// classes owned by other packets: field<int32_t>(p, 0x864) is *(int*)(p + 0x864).
template <class T>
inline T& field(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<std::byte*>(p) + off);
}
template <class T>
inline const T& field(const void* p, std::size_t off)
{
    return *reinterpret_cast<const T*>(static_cast<const std::byte*>(p) + off);
}

// ---- DB array index tables (DAT_1406e81f8 ..) ------------------------------------------------

// @data 1406e81f8  dimension the two index tables were last filled for (0 = never)
inline std::int32_t g_index_tables_dim = 0;
// @data 1406e81fc  53 indices into the MAR address table DB_STEP_DATA::u1b90 (filled per dimension)
inline std::int32_t g_mar_index[53] = {};
// @data 1406e82d0  129 indices into the A address table DB_STEP_DATA::u1dc0 (filled per dimension)
inline std::int32_t g_a_index[129] = {};

void init_mar_index_2d();                       // 1401c6e60
void init_mar_index_3d();                       // 1401c7080
void init_a_index_2d();                         // 1401c72a0
void init_a_index_3d();                         // 1401c77c0
void select_index_tables_2d();                  // 1401c7ce0
void select_index_tables_3d();                  // 1401c7d10
void select_index_tables(const std::int32_t* dim); // 1401c7d40

// Resets the globals above (tests).
void reset_globals_p1_03();

// ---- DB_WRITE_VAR ----------------------------------------------------------------------------
void db_write_var_init(DB_WRITE_VAR& w);          // 1401baf30
void db_write_var_free_buffers(DB_WRITE_VAR& w);  // 1401bb320
void db_write_var_set_u008(DB_WRITE_VAR& w, const std::uint8_t* v);  // 1401c5720
void db_write_var_set_u06c(DB_WRITE_VAR& w, const std::int32_t* v);  // 1401c5740
void db_write_var_set_u070(DB_WRITE_VAR& w, const std::int32_t* v);  // 1401c5760
void db_write_var_set_u0c8(DB_WRITE_VAR& w, const std::int32_t* v);  // 1401c5780
void db_write_var_set_u0cc(DB_WRITE_VAR& w, const std::int32_t* v);  // 1401c57a0

// ---- DB_STEP_DATA ----------------------------------------------------------------------------
void step_init(DB_STEP_DATA& s, std::int32_t dim);           // 1401c8c00
void step_clear_u1b48(DB_STEP_DATA& s);                      // 1401ca960
void step_clear_address_tables(DB_STEP_DATA& s);             // 1401cb170
void step_clear_gendb_tables(DB_STEP_DATA& s);               // 1401cb1f0
void step_free_contents(DB_STEP_DATA& s);                    // 1401cb440
void step_free_strings_33b0(DB_STEP_DATA& s);                // 1401cb660
void step_free_strings_33a0(DB_STEP_DATA& s);                // 1401cb910
void step_setup_array_sizes(DB_STEP_DATA& s, std::int32_t dim); // 1401d09c0
bool step_has_array(DB_STEP_DATA& s, std::int32_t idx, std::int32_t* size); // 1401d64f0
std::int32_t step_sum_1401d6b30(DB_STEP_DATA& s);            // 1401d6b30
std::int32_t step_sum_1401d6c60(DB_STEP_DATA& s);            // 1401d6c60
void* step_object_at(DB_STEP_DATA& s, std::int32_t n);       // 1401d6ed0 (LLOBJ_DATA*)
Material* step_material_by_id(DB_STEP_DATA& s, std::int32_t id); // 1401d7060 
std::int32_t step_fun_1401d70c0(DB_STEP_DATA& s, const std::int32_t* kind); // 1401d70c0
std::int32_t step_fun_1401d71b0(DB_STEP_DATA& s, std::int32_t* out);        // 1401d71b0
std::int32_t step_fun_1401d7250(DB_STEP_DATA& s, void* obj, std::int32_t* out); // 1401d7250
std::int32_t step_fun_1401d72a0(DB_STEP_DATA& s, const std::int32_t* id, std::int32_t* out); // 1401d72a0
void step_clear_u2f89(DB_STEP_DATA& s);                      // 1401d7ef0
void step_clear_u2f88(DB_STEP_DATA& s);                      // 1401d7f10
void step_clear_partner(DB_STEP_DATA& s);                    // 1401d7fd0
void step_clear_u33f8(DB_STEP_DATA& s);                      // 1401d8200
void materials_set_owner(MaterialList& l, DB_STEP_DATA* owner);   // 1401d8300
void inter_matl_set_owner(PhasePairList& l, DB_STEP_DATA* owner);  // 1401d8320
void objects_set_owner(Cls_14017a6c0& l, DB_STEP_DATA* owner);     // 1401d8340
void inter_obj_set_owner(Cls_1401fa020& l, DB_STEP_DATA* owner);   // 1401d8360

// ---- simulation control Cls_1401e7b10 (DB_STEP_DATA + 0x750) ----------------------------------
void sim_ctrl_set_owner(Cls_1401e7b10& c, DB_STEP_DATA* owner);    // 1401e7c10
void sim_ctrl_init(Cls_1401e7b10& c);                              // 1401e7c50
void sim_ctrl_clear(Cls_1401e7b10& c);                             // 1401e7da0
void sim_ctrl_init_names(Cls_1401e7b10& c);                        // 1401e7e90
void sim_ctrl_init_u160(Cls_1401e7b10& c);                         // 1401e8490
void sim_ctrl_init_u1c8(Cls_1401e7b10& c);                         // 1401e8650
void sim_ctrl_init_u170(Cls_1401e7b10& c);                         // 1401e8fb0
void sim_ctrl_init_u2d0(Cls_1401e7b10& c);                         // 1401e96b0
void sim_ctrl_init_u328(Cls_1401e7b10& c);                         // 1401e9850
void sim_ctrl_init_u3a8(Cls_1401e7b10& c);                         // 1401ea6b0
void sim_ctrl_init_u640(Cls_1401e7b10& c);                         // 1401ead60
void sim_ctrl_update_u240(Cls_1401e7b10& c);                       // 1401ee510
void sim_ctrl_set_u240(Cls_1401e7b10& c, const std::int32_t* v);   // 1401ee5a0

// ---- DB info object Cls_1401dbfe0 ------------------------------------------------------------

// One step-directory entry, the array at Cls_1401dbfe0::uad8 (new[] of 0x28-byte elements,
// element ctor 1401d9080 zeroes the 10 ints, dtor 1401d9110 is empty).
struct DbDirRecord {
    // @src 1401d9080  element ctor: 10 ints = 0
    DbDirRecord();
    // @src 1401d9110  element dtor (empty)
    ~DbDirRecord();

    std::int32_t mesh_no;  // +0x00 directory word 1
    std::int32_t step;     // +0x04 word 2
    std::int32_t hsize;    // +0x08 word 3
    std::int32_t mar_size; // +0x0c word 4
    std::int32_t a_size;   // +0x10 word 5
    std::int32_t sim_no;   // +0x14 operation number (word 6, or 1 for 3D steps before 3000)
    std::int32_t sim_mode; // +0x18 word 7, or 0
    std::int32_t dim;      // +0x1c word 8 (version >= 10000), else the DB family
    std::int32_t version;  // +0x20 word 9 (version >= 10000), else the DB version
    std::int32_t stored;   // +0x24 1 when mar_size >= 1
};
static_assert(sizeof(DbDirRecord) == 0x28);

// The array at Cls_1401dbfe0::uae8 (new[] of 0xe0-byte elements; dtor 1401d91d0 frees +0x20).
// Who allocates it is outside this packet; only the dtor is known.
struct DbInfoEntryE0 {
    DbInfoEntryE0() = default;
    // @src 1401d91d0  element dtor: free(+0x20)
    ~DbInfoEntryE0();
    std::byte pad_00[0x20];
    void* u20 = nullptr;      // +0x20 malloc'd
    std::byte pad_28[0xb8];
};
static_assert(sizeof(DbInfoEntryE0) == 0xe0);

void db_info_init(Cls_1401dbfe0& d);   // 1401dc030
void db_info_reset(Cls_1401dbfe0& d);  // 1401dc220

inline DbDirRecord* db_info_records(Cls_1401dbfe0& d) { return static_cast<DbDirRecord*>(d.uad8); }
inline const DbDirRecord* db_info_records(const Cls_1401dbfe0& d)
{
    return static_cast<const DbDirRecord*>(d.uad8);
}

// 1401e6730 / 1401e67c0: the vector deleting dtors of the two arrays, flag 3 = delete[].
void delete_dir_records(DbDirRecord* p);     // 1401e6730
void delete_entries_e0(DbInfoEntryE0* p);    // 1401e67c0

} // namespace def::model
