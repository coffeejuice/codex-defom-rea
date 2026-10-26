// defmodel/program_info.hpp: program name, version and product strings that main stores at
// start-up (FUN_140224d80 and its setters, packet p5-01) and the KEY writer banner reads.
// These are plain setters/getters; the readers of the globals are not followed here.
#pragma once

#include <cstdint>

namespace def::model {

// @data 1406fdb20  version string (100 bytes; "1.0" by the static initialiser 140224cd0)
inline char g_version[100] = "1.0";
// @data 1406fdb84  dimension the version reader picks the install folder by (initialiser: 2)
inline std::int32_t g_info_dim = 2;
// @data 1406fe010  program name (0x31 + 1 bytes used; initialiser "NEW")
inline char g_program_name[0x40] = "NEW";
// @data 1406fe050  product family ("DEFORM" initially, "DEFORM-23" from main)
inline char g_product[100] = "DEFORM";
// @data 1406fe0b4  a number main passes (initialiser 100)
inline std::int32_t g_program_number = 100;

// @src 140224cd0  static initialiser of the globals above (and others no batch code reads)
void reset_program_info();

// @src 140224d80  sets name (140224e70), version (140224f20), number (140225310), product
// (140225330)
void set_program_info(const char* name, const char* version, std::int32_t number,
                      const char* product);
void set_program_name(const char* s);      // 140224e70  strncpy(.., 0x31)
void set_version(const char* s);           // 140224f20  strncpy 99, then the service pack of
                                           //            <DEFORM_DIR or DEFORM3_DIR>/PACK/pack.dat
void set_program_number(std::int32_t v);   // 140225310
void set_product(const char* s);           // 140225330  strncpy(.., 99)
std::int32_t info_dim();                   // 1402253d0
const char* version();                     // 140225450
// @src 140224dd0  "DEFORM-2D" for 2, else "DEFORM-3D"
const char* product_name_for_dim(std::int32_t dim);

} // namespace def::model
