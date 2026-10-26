// p8-01: GFREAD handler, loader, extension -> type and the 2D table (docs/re/commands/GFREAD.md,
// DXF_IGES_READERS.md section 2).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/gfread.hpp>

#include <cstdio>
#include <cstring>

#include <defdbio/step/db_info_read.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>

#include "ext_p8_02.hpp"
#include "ext_p8_03.hpp"
#include "ext_p8_04.hpp"
#include "ext_p8_09.hpp"

namespace def::cmd::gfread {

// @src 140243340  identity (base-class ctor of the loader, nothing to do)
// @src 1402437c0  loader ctor
Loader* fun_1402437c0(Loader* self, void* cb)
{
    self->callback = cb;
    fun_140244e30(self);
    return self;
}

// @src 140243360  empty
// @src 140243810  loader dtor -> 140243360
void fun_140243810(Loader*) {}

// @src 140244e30  defaults (DAT_140522f98 = 1.0E-5, DAT_140509c90 = 1.0)
void fun_140244e30(Loader* self)
{
    self->tol = 1.0E-5;  // 0x3ee4f8b588e368f1
    self->scale = 1.0;
}

// @src 140243370  extension
bool fun_140243370(const char* file, char* ext)
{
    int start = -1;  // local_10
    const int len = static_cast<int>(std::strlen(file));
    if (3 < len && file[len - 3] == '.') start = len - 2;
    if (start < 0 && 4 < len && file[len - 4] == '.') start = len - 3;
    if (start < 0 && 5 < len && file[len - 5] == '.') start = len - 4;
    if (start < 0 && 6 < len && file[len - 6] == '.') start = len - 5;
    if (start < 0 && 7 < len && file[len - 7] == '.') start = len - 6;
    if (start < 0) return false;
    int i = start;
    for (; i < len; ++i) ext[i - start] = file[i];
    ext[i - start] = '\0';
    return true;
}

// @src 140243500  type code
std::int32_t fun_140243500(const char* ext)
{
    if (std::strncmp(ext, "KEY", 3) == 0 || std::strncmp(ext, "key", 3) == 0) return 0;
    if (std::strncmp(ext, "DB", 2) == 0 || std::strncmp(ext, "db", 2) == 0) return 1;
    if (std::strncmp(ext, "GEO", 3) == 0 || std::strncmp(ext, "geo", 3) == 0) return 2;
    if (std::strncmp(ext, "STL", 3) == 0 || std::strncmp(ext, "stl", 3) == 0) return 3;
    if (std::strncmp(ext, "PDA", 3) == 0 || std::strncmp(ext, "pda", 3) == 0) return 4;
    if (std::strncmp(ext, "PAT", 3) == 0 || std::strncmp(ext, "pat", 3) == 0) return 4;
    if (std::strncmp(ext, "UNV", 3) == 0 || std::strncmp(ext, "unv", 3) == 0) return 5;
    if (std::strncmp(ext, "NAS", 3) == 0 || std::strncmp(ext, "nas", 3) == 0) return 10;
    if (std::strncmp(ext, "IGS", 3) == 0 || std::strncmp(ext, "igs", 3) == 0) return 6;
    if (std::strncmp(ext, "DXF", 3) == 0 || std::strncmp(ext, "dxf", 3) == 0) return 7;
    return -1;
}

// @src 140243830  load by extension
bool fun_140243830(Loader* self, const char* file, LLOBJ_DATA* obj, std::int32_t* type)
{
    char ex[16];  // local_30
    bool ok = fun_140243370(file, ex);
    if (!ok) return false;
    *type = fun_140243500(ex);
    if (*type == -1) return false;
    // FUN_140158ca0 (p67-5's object_is_3d): obj+0x39c0 == 3 (defmodel 140158ce0)
    if (model::geo_kw::object_dim(*obj) != 3) {
        ok = fun_140244010(self, file, obj, *type);
    } else {
        ok = ext::load_3d(self, file, obj, *type);
    }
    if (ok) fun_140256670(obj, *type);
    return ok;
}

// @src 140244010  2D table
bool fun_140244010(Loader* self, const char* file, LLOBJ_DATA* obj, std::int32_t type)
{
    bool result = true;  // [rsp+0x30]
    if (type == 0 || type == 1) return result;
    if (type == 2) {
        ext::GeoOnStack g1;  // [rsp+0x80]
        char flag1 = 0;      // [rsp+0xc8]
        bool r = ext::geo_read(g1.get(), file, &flag1, 3, nullptr);
        if (r) r = ext::geo_store(g1.get(), obj, 2);
        ext::GeoOnStack g2;  // [rsp+0x40]
        if (obj == nullptr) return false;  // 1402440f5; both dtors run
        // 140244121: a constant 0 is tested, so the flag byte is always 0
        char flag2 = 0;  // [rsp+0xca]
        r = ext::geo_read(g2.get(), file, &flag2, 4, obj);
        (void)r;  // stored at [rsp+0xc0], never read
        return result;
    }
    if (type == 6 || type == 7) result = fun_140244200(self, obj, file, type, 300);
    return result;
}

// @src 140244200  IGES/DXF import by mode
bool fun_140244200(Loader* self, LLOBJ_DATA* obj, const char* file, std::int32_t type, std::int32_t mode)
{
    if (obj == nullptr) return false;
    std::int32_t r;
    if (mode == 300) {
        r = fun_140244500(self, obj, file, type);
    } else if (mode == 400) {
        r = fun_1402442f0(self, obj);
    } else {
        r = fun_140244500(self, obj, file, type);
        if (r == 0) r = fun_1402442f0(self, obj);
    }
    return r != -1;
}

// @src 1402442f0  mode-400 import
std::int32_t fun_1402442f0(Loader*, LLOBJ_DATA* obj)
{
    ext::IgesFileOnStack iges;  // [rsp+0x78]
    ext::DxfFileOnStack dxf;    // [rsp+0x50]
    if (obj == nullptr) return -1;
    const std::int32_t type = fun_140256690(obj);
    std::int32_t r = -1;
    if (type == 6) {
        double x = 0.0;  // [rsp+0xb0]
        r = ext::iges_read(iges.get(), nullptr, nullptr, obj, &x, 2, 0, 1, 400);
    } else if (type == 7) {
        r = ext::dxf_read(dxf.get(), nullptr, nullptr, obj, 2, 400);
    }
    return r;
}

// @src 140244500  mode-300 import
std::int32_t fun_140244500(Loader*, LLOBJ_DATA* obj, const char* file, std::int32_t type)
{
    ext::IgesFileOnStack iges;  // [rsp+0x78]
    ext::DxfFileOnStack dxf;    // [rsp+0x50]
    if (obj == nullptr) return -1;
    std::int32_t r = -1;
    if (type == 6) {
        double x = 0.0;  // [rsp+0xb0]
        r = ext::iges_read(iges.get(), file, "", obj, &x, 2, 0, 1, 300);  // str 140531786
        if (r == 0) fun_14019a650(obj, fun_1402566b0(iges.get()));
    } else if (type == 7) {
        r = ext::dxf_read(dxf.get(), file, "", obj, 2, 300);  // str 140531787
        if (r == 0) fun_14019a650(obj, fun_1402566c0(dxf.get()));
    }
    return r;
}

// @src 1401aa9a0  GFREAD handler
bool fun_1401aa9a0(model::DB_STEP_DATA* m, const std::int32_t* id, const char* file)
{
    LLOBJ_DATA* obj = key::g2obj::find_object(*m, *id);  // 1401d6ea0
    if (obj == nullptr) return false;
    if (file == nullptr) return false;
    if (!dbio::file_exists(file)) {  // 1401b8560
        std::fprintf(stdout, "ERROR  : Reading GFREAD\n");
        std::fprintf(stdout, "         %s does not exist.\n", file);
        return false;
    }
    std::int32_t type = 0;  // [rsp+0x38]
    Loader loader;          // [rsp+0x20]
    fun_1402437c0(&loader, nullptr);
    const bool ok = fun_140243830(&loader, file, obj, &type);
    if (!ok) {
        std::fprintf(stdout, "INFO   : Loading geometry - Failed\n");
    } else {
        std::fprintf(stdout, "INFO   : Loading geometry - Done\n");
    }
    fun_140243810(&loader);
    return ok;
}

} // namespace def::cmd::gfread
