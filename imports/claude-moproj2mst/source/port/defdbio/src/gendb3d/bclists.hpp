// gendb3d/bclists.hpp: LLWINDEF_DATA record writer and the record helpers of the sub-object
// writers write_obj_1ea0 / write_obj_2d10 / write_obj_2e68 (bclists.cpp, packet p4-05).
//
// Argument conventions as in obj3d.hpp. A window record is one LLWIN row (rec, win_w doubles);
// llwrz receives the window's coordinates; *row / *pos are the LLWIN row / LLWRZ position
// cursors (1-based), advanced by the writer.
#pragma once

#include <cstdint>

#include <defdbio/gendb3d/obj3d.hpp>

namespace def::dbio::g3d {

// @src 140260ea0  LLWINDEF_DATA::WriteToDBArrays: one window into rec / llwrz / functions;
//                 code2d is the 2D type-1 code used when +0x36c is 0 (the callers pass 0)
void windef_write_to_db_arrays(void* w, double* rec, double* llwrz, std::int32_t* iifnc,
                               double* llfnc, std::int32_t* row, std::int32_t* pos,
                               DB_STEP_DATA* m, const std::int32_t* obj_no, std::int32_t code2d);
// @src 140262250  LLWINDEF_DATA::storeWindefFric (types 1 and 7)
void windef_store_fric(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                       const std::int32_t* obj_no);
// @src 140262c00  DYNARY at +0x748 -> rec[6] (flag) / rec[7] (value or function number)
void windef_store_748(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                      const std::int32_t* obj_no);
// @src 140262cf0  DYNARYs at +0x380 / +0x3f8 / +0x470 -> rec[6..13]
void windef_store_380(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                      const std::int32_t* obj_no);
// @src 140263040  DYNARYs at +0x5e0 / +0x658 -> rec[6..11]
void windef_store_5e0(void* w, double* rec, std::int32_t* iifnc, double* llfnc, DB_STEP_DATA* m,
                      const std::int32_t* obj_no);

// @src 140258c40  record of the +0x200 list (link +0xe0) into 12 doubles by its type (+4)
void record_e8_to_values(const void* r, double* out);
// @src 140258f10  if (dst) memcpy(dst, r + 0x08, 0x18)
void record_e8_get_08(const void* r, void* dst);
// @src 140258f90  if (dst) memcpy(dst, r + 0x20, 0x18)
void record_e8_get_20(const void* r, void* dst);
// @src 140259020  if (dst) memcpy(dst, r + 0x68, 0x18)
void record_e8_get_68(const void* r, void* dst);
// @src 1402590b0  if (dst) memcpy(dst, r + 0x80, 0x18)
void record_e8_get_80(const void* r, void* dst);
// @src 140259130  if (dst) memcpy(dst, r + 0x98, 0x18)
void record_e8_get_98(const void* r, void* dst);

} // namespace def::dbio::g3d
