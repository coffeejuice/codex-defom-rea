// defdbio/dbread/dbread.hpp: DBREAD, the read side of a .DB step (packet p-dbread).
// Entry points: fun_1401cc2a0 (by step number, the KEY keyword DBREAD and the -m paths) and
// fun_1401cc420 (_DB_STEP_DATA::ReadRecordFromDB, by directory index). Subtree readers in
// defdbio/dbread/w1..w4.hpp.
#pragma once

#include <cstdarg>
#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class Cls_1401dbfe0;
}

namespace def::dbio::dbread {

// @src 1401cc2a0  read step `stepno` of db (Cls_1401dbfe0) into m; 1 when the step is missing
std::int32_t fun_1401cc2a0(model::DB_STEP_DATA* m, void* db, std::int32_t stepno, char fold, char arg5);
// part of @src 1401cc2a0  the same with the types of p5-01's dispatcher hook
// (ModelHooks::db_read_step in defcmd/dispatch.hpp)
int db_read_step(model::DB_STEP_DATA* m, model::Cls_1401dbfe0* db, std::int32_t stepno, std::uint8_t a,
                 std::uint8_t b);
// @src 1401cc420  read directory entry `index` (-1: the current one) of db (null: m's own DB)
std::int32_t fun_1401cc420(model::DB_STEP_DATA* m, void* db, std::int32_t index);
// @src 1401cd020  per-object STROKE / second triple from the step header
std::int32_t fun_1401cd020(model::DB_STEP_DATA* m, void* db, std::int32_t step);
// @src 1401d5f50  2D: six doubles of the object +0x8b8 into the model
void fun_1401d5f50(model::DB_STEP_DATA* m);
// @src 1401cc120  +0x1b60 = v
void fun_1401cc120(model::DB_STEP_DATA* m, std::int32_t v);
// @src 1401d83b0  byte +0x1b5b
bool fun_1401d83b0(model::DB_STEP_DATA* m);
// @src 1401d8f80  byte +0x1b5a = *v
void fun_1401d8f80(model::DB_STEP_DATA* m, const char* v);

// @src 14039d570  message print: vsprintf, then a newline, to stdout
void fun_14039d570(int unused, const char* fmt, std::va_list ap);
// @src 14039d3a0  message print with three arguments (realloc failure text)
void fun_14039d3a0(const char* fmt, const char* a, const char* b, std::int32_t n);

} // namespace def::dbio::dbread
