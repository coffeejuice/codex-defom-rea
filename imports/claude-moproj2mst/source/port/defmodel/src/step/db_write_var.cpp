// DB_WRITE_VAR: base of DB_STEP_DATA (packet p1-03). Layout: defmodel/layout/db_write_var.hpp.
#include <defmodel/step/db_step.hpp>

#include <cstdlib>
#include <cstring>

namespace def::model {

// @src 1401bae10  ctor: vptr, small fields, then 1401baf30
DB_WRITE_VAR::DB_WRITE_VAR()
{
    u008 = 1;
    u009 = 1;
    u030 = 0;
    u034 = 0;
    u038 = 0;
    u03c = 0;
    u040 = 0;
    u044 = 0;
    u048 = 0;
    u050 = nullptr;
    u058 = nullptr;
    u060 = nullptr;
    dimension = 3;
    u020 = 0;
    u024 = 0;
    u028 = 0;
    u02c = 0;
    db_write_var_init(*this);
}

// @src 1401baf10  dtor: only restores the vptr
DB_WRITE_VAR::~DB_WRITE_VAR() = default;

// @src 1401c5700  slot 0: dimension = *dim (getter 140158e90 reads 0x1c back)
void DB_WRITE_VAR::set_dimension(const std::int32_t* dim) { dimension = *dim; }

// @src 1401baf30  init: version 1100, date "06/30/14", all counters 0, then free the buffers
void db_write_var_init(DB_WRITE_VAR& w)
{
    w.version = 0x44c;
    std::strcpy(w.date, "06/30/14");
    std::uint8_t one = 1;
    db_write_var_set_u008(w, &one);
    w.u030 = 0;
    w.u034 = 0;
    w.u038 = 0;
    w.u03c = 0;
    w.u068 = 0;
    std::int32_t zero = 0;
    db_write_var_set_u06c(w, &zero);
    db_write_var_set_u070(w, &zero);
    for (std::int32_t& v : w.u074) { // 0x74..0xc4, 21 stores of 0
        v = 0;
    }
    db_write_var_set_u0c8(w, &zero);
    w.u0d8 = 0;
    w.u0dc = 0;
    w.u0e0 = 0x10;
    db_write_var_set_u0cc(w, &zero);
    w.u0d0 = 0; // 14019a310 (p1-05 setter of 0xd0, inlined)
    w.u0d4 = 0; // 14019a330 (setter of 0xd4)
    w.u0e4 = 0;
    w.u0e8 = 0;
    w.u0f0 = 0;
    w.u0f8 = 0;
    w.u100 = 0;
    w.u104 = 0;
    w.u108 = 0;
    w.u10c = 0;
    w.u110 = 0; // 14019a2d0
    w.u114 = 0; // 140199d00
    w.u118 = 0; // 140199d40
    w.u11c = 0; // 14019a0e0
    w.u120 = 0;
    w.u124 = 0; // 14019a120
    w.u128 = 0; // 14019a160
    w.u12c = 0; // 14019a1a0
    w.u130 = 0; // 14019a1e0
    w.u134 = 0;
    w.u138 = 0;
    w.u13c = 0;
    db_write_var_free_buffers(w);
}

// @src 1401bb320  free the three GENDB buffers
void db_write_var_free_buffers(DB_WRITE_VAR& w)
{
    if (w.u050 != nullptr) {
        std::free(w.u050);
        w.u050 = nullptr;
    }
    if (w.u058 != nullptr) {
        std::free(w.u058);
        w.u058 = nullptr;
    }
    if (w.u060 != nullptr) {
        std::free(w.u060);
        w.u060 = nullptr;
    }
}

// @src 1401c5720  setter 0x08
void db_write_var_set_u008(DB_WRITE_VAR& w, const std::uint8_t* v) { w.u008 = *v; }
// @src 1401c5740  setter 0x6c
void db_write_var_set_u06c(DB_WRITE_VAR& w, const std::int32_t* v) { w.u06c = *v; }
// @src 1401c5760  setter 0x70
void db_write_var_set_u070(DB_WRITE_VAR& w, const std::int32_t* v) { w.u070 = *v; }
// @src 1401c5780  setter 0xc8
void db_write_var_set_u0c8(DB_WRITE_VAR& w, const std::int32_t* v) { w.u0c8 = *v; }
// @src 1401c57a0  setter 0xcc
void db_write_var_set_u0cc(DB_WRITE_VAR& w, const std::int32_t* v) { w.u0cc = *v; }

} // namespace def::model
