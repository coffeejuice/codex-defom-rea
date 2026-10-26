// defdbio/step/db_info_read.hpp: reading the .DB header and step directory into the DB info
// object Cls_1401dbfe0 (packet p1-03; used by the -m modes and the step read).
#pragma once

#include <cstdint>

#include <defmodel/step/db_step.hpp>

namespace def::dbio {

// FUN_1401b8560: fopen(name, "r") succeeds. NULL or "" is false.
bool file_exists(const char* name);

// FUN_1401dc3f0: reads header and (unless header_only) the step directory of `name` into `d`.
// 0 ok, 1 not a DEFORM DB, 2 header error, 3 directory error, 4 file missing.
std::int32_t load_db_info(def::model::Cls_1401dbfe0& d, const char* name, bool header_only);

// FUN_1401e39b0: operation number of the last stored directory entry, 0 if none.
std::int32_t last_stored_sim_no(const def::model::Cls_1401dbfe0& d);

// FUN_1401e3e20: step sampling interval for a directory of n entries.
std::int32_t step_interval(std::int32_t n);

} // namespace def::dbio
