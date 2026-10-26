// IGES reader driver (packet p8-03): Uic2CObjectIgesFile read 140409af0, the pass dispatcher
// 14040c220, segment-table dedupe 14040c740, the undo list helpers and the C file helpers.
// Spec: docs/re/commands/IGES_READER.md §2-§5, §8.
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/iges.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include <sys/stat.h>
#include <sys/types.h>

#include <defdbio/step/db_info_read.hpp>
#include <defmodel/geo_kw/geo_kw.hpp>
#include <defmodel/layout/geo2d_and_mesh2d.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo2d.hpp>
#include <defmodel/layout/sdctl_g_list.hpp>
#include <defmodel/sdctl/sdctl_node.hpp>
#include <defrt/ftnstr/ftnstr.hpp>

#include "ext_p4.hpp"
#include "ext_p8_01.hpp"
#include "ext_p8_09.hpp"
#include "ftn_read.hpp"
#include "iges_ftn.hpp"
#include "iges_text.hpp"

namespace def::cmd::iges {

using def::model::LLOBJ_DATA;
using def::model::OBJ_GEO2D;

void fun_140197d00(LLOBJ_DATA* obj);
void fun_140197d40(LLOBJ_DATA* obj);
bool fun_1401b8420(const char* name);
int fun_1401b8530(const char* name, struct _stat64i32* st);
void fun_14040c740(IgesCtx* ctx);
bool fun_14040cd30(double a, double b, double eps);
double fun_14040ce00(const LLOBJ_DATA* obj);
void* fun_14019abd0(def::model::SdctlGList* list, std::int32_t i);
void* fun_14019abf0(def::model::SdctlGList* list, std::int32_t i);
def::model::SdctlGListNode* fun_140331250(def::model::SdctlGList* list, std::uint32_t i);

namespace {

// the work arrays as the driver allocates them (operator new 1404797d8 + memset 140478d30)
void alloc_work()
{
    g_iges_rw = static_cast<double*>(std::malloc(static_cast<std::size_t>(g_iges_rw_size) * 8));
    std::memset(g_iges_rw, 0, static_cast<std::size_t>(g_iges_rw_size) << 3);
    g_iges_iw = static_cast<std::int32_t*>(std::malloc(static_cast<std::size_t>(g_iges_iw_size) * 4));
    std::memset(g_iges_iw, 0, static_cast<std::size_t>(g_iges_iw_size) << 2);
}
void free_rw()
{
    std::free(g_iges_rw);
    g_iges_rw = nullptr;
}
void free_iw()
{
    std::free(g_iges_iw);
    g_iges_iw = nullptr;
}

// The DEF_IGS.OUT path buffer of 140409af0 ([rsp+0x30]). Mode 400 deletes the file named by this
// buffer without having written it (stack contents of the frame); the port keeps the last
// mode-300 path there (empty before the first one).
std::string g_out_path;

// fallback text route (IGES_READER.md §8): 0, -14 or -15, or 14040b100's code
int text_route(Uic2CObjectIgesFile* iges, LLOBJ_DATA* obj, std::int32_t p6, const char* out)
{
    auto* t = static_cast<IgsText*>(std::malloc(0x88));
    std::memset(t, 0, 0x88);
    int r = fun_140455e60(t, "DEF_IGS.OUT"); // str 1405666e0
    (void)ext::remove_if_exists(out);
    if (r == -1) {
        fun_140454e40(t);
        std::free(t);
        return -0xe;
    }
    if (r == 0) {
        g_iges_text = t;
        r = fun_14040b100(iges, obj, p6);
        fun_140454e40(t);
        std::free(t);
        g_iges_text = nullptr;
        return r;
    }
    fun_140454e40(t);
    std::free(t);
    return -0xf;
}

} // namespace

// @src 140409af0  Uic2CObjectIgesFile read (iges.hpp)
int fun_140409af0(Uic2CObjectIgesFile* iges, const char* name, const char* prefix, LLOBJ_DATA* obj,
                  double* p5, std::int32_t p6, char p7, std::int32_t p8, std::int32_t mode)
{
    IgesCtx* ctx = &g_iges_ctx;
    static char nm[0x1000]; // local_1158
    int r = 0;
    if (obj == nullptr) return -1;
    if (mode == 300) {
        iges->u20 = 0.0;
        if (name == nullptr) return -2;
        if (static_cast<int>(std::strlen(name)) < 1) return -2;
        std::strcpy(nm, name);
        if (!def::dbio::file_exists(nm)) return -3; // 1401b8560
        if (prefix == nullptr) return -0xc;
        (void)ext::f140197bc0(obj);
        g_out_path = std::string(prefix) + "DEF_IGS.OUT";
        (void)fun_1401b8420(g_out_path.c_str());
        if (g_iges_rw != nullptr) free_rw();
        if (g_iges_iw != nullptr) free_iw();
        alloc_work();
        r = fun_14040c220(nm, ctx, 1, 0);
        if (r != 0) {
            free_rw();
            free_iw();
            return r;
        }
        (void)ext::remove_if_exists(g_out_path.c_str());
        if (ext::f1404098f0(obj) != 0) ctx->tol = ext::f140409930(obj);
        ext::f1404098d0(obj, ctx->tol);
        iges->u20 = ctx->tol;
        if (p7 != '\0') {
            if (p8 == 1) {
                *p5 = ctx->tol;
            } else {
                ctx->tol = *p5;
            }
        }
        const std::int32_t nrec = ctx->n_records; // local_1168
        std::int32_t n;
        if (ctx->n126 < 1 && ctx->n112 < 1) {
            n = nrec * def::model::geo2d_dof(2);
        } else {
            n = ctx->n_points * def::model::geo2d_dof(2);
        }
        ctx->table = n < 1 ? nullptr : def::model::geo2d_alloc_zeroed(n);
        if (ctx->table == nullptr) {
            free_rw();
            free_iw();
            return text_route(iges, obj, p6, g_out_path.c_str());
        }
        OBJ_GEO2D* cont = def::model::geo_kw::curve(*obj, 0);
        if (cont == nullptr) {
            ext::f140198270(obj);
            cont = def::model::geo_kw::curve(*obj, 0);
        }
        r = fun_14040c220(nm, ctx, 2, 500);
        if (r == 200 || r == 0) {
            cont->n_points = ctx->n_records;
            cont->curve_type = 2;
            cont->free_points();
            ext::f14040cde0(cont, ctx->table);
            if (ctx->table == nullptr) {
                ext::f140198230(obj, 0);
                ext::f14019a650(obj, 0);
                ext::f14040ce60(iges, 0);
                free_rw();
                free_iw();
                ctx->table = nullptr;
                return -1;
            }
            if (r == 200) {
                ext::f14040ce40(iges);
                ext::f140198230(obj, 1);
                const std::int32_t nb = nrec < 2 ? 1 : nrec;
                ext::f14019a650(obj, nb);
                ext::f14040ce60(iges, nb);
            } else {
                ext::f140198230(obj, 1);
                ext::f14019a650(obj, 1);
                ext::f14040ce60(iges, 1);
            }
            free_rw();
            free_iw();
            ctx->table = nullptr;
            return 0;
        }
        free_rw();
        free_iw();
        std::free(ctx->table);
        ctx->table = nullptr;
        return r;
    }
    if (mode == 400) {
        ext::f14040ce20(iges, 0);
        if (g_iges_rw != nullptr) free_rw();
        if (g_iges_iw != nullptr) free_iw();
        (void)ext::remove_if_exists(g_out_path.c_str());
        alloc_work();
        OBJ_GEO2D* cont = def::model::geo_kw::curve(*obj, 0);
        if (cont == nullptr) {
            def::model::geo_kw::geometry(*obj, 0)->add_edge_curve(); // 140197f40, 14027fce0
            cont = def::model::geo_kw::curve(*obj, 0);
        }
        ctx->n_points = 0;
        ctx->n_records = cont->n_points;
        if (ext::f1404098f0(obj) == 0) {
            iges->u20 = fun_14040ce00(obj);
        } else {
            iges->u20 = ext::f140409930(obj);
        }
        ctx->tol = iges->u20;
        ctx->table = cont->points;
        if (ctx->table == nullptr) {
            r = -1;
        } else {
            r = fun_14040c220(nm, ctx, 2, 600);
        }
        ctx->table = nullptr;
        if (r == 0) {
            free_rw();
            free_iw();
            return text_route(iges, obj, p6, g_out_path.c_str());
        }
        free_rw();
        free_iw();
        return r;
    }
    return -1;
}

// @src 14040c220  pass dispatcher (iges.hpp)
int fun_14040c220(const char* name, IgesCtx* ctx, std::int32_t pass, std::int32_t mode)
{
    static char nm[0x100]; // local_148 CHARACTER*256
    std::int32_t len = 0;
    std::int32_t iwsize = g_iges_iw_size;
    std::int32_t rwsize = g_iges_rw_size;
    std::int32_t st = 0;
    double* table = nullptr;
    std::int32_t fmode = 0;
    std::int32_t flag = 0;
    if (pass == 1) {
        // 1401b9020 (p5-01): no effect
        std::strcpy(ctx->name, name);
        len = static_cast<std::int32_t>(def::rt::to_fortran(nm, ctx->name, 0x100));
        fun_1404570f0(nm, &len, &ctx->n_points, &ctx->n_records, &ctx->n100, &ctx->n110, &ctx->n112, &ctx->n126,
                      &ctx->n124, &ctx->tol, g_iges_rw, g_iges_iw, &rwsize, &iwsize, &st);
        if (st != 0) return -4;
        fun_140457196(nm, &len, &ctx->n_points, &ctx->n_records, &ctx->n100, &ctx->n110, &ctx->n112, &ctx->n126,
                      &ctx->n124, &g_iges_nurbs_samples, &ctx->tol, g_iges_rw, g_iges_iw, &rwsize, &iwsize, &st);
        if (st == 1) return -5;
        if (st == 2) return -6;
        if (st != 0) return -7;
        if (ctx->n_points < 1) return -8;
    }
    if (pass == 2) {
        if (mode == 600) {
            table = ctx->table;
            fmode = 2;
            flag = 0;
        } else if (mode == 500) {
            table = ctx->table;
            fmode = 1;
            flag = 1;
        } else if (mode == 700) {
            table = ctx->table;
            fmode = 3;
            flag = 1;
        } else {
            table = nullptr;
            fmode = 1;
            flag = 0;
        }
        fun_14040c740(ctx);
        fun_140457312(&ctx->n_points, &ctx->n_records, &ctx->tol, g_iges_rw, g_iges_iw, table, &rwsize, &iwsize,
                      &fmode, &flag, &st);
        if (st == 1) {
            st = -9;
        } else if (st == 2) {
            st = -10;
        } else if (st == 3) {
            st = 200;
        } else {
            st = st != 0 ? -0xb : 0;
        }
    }
    return st;
}

// @src 14040c740  drop segment-table rows equal to an earlier row (lines also reversed; tolerance
// ctx->tol on coordinates, 1e-4 on the type)
void fun_14040c740(IgesCtx* ctx)
{
    const std::int32_t n = ctx->n_records;
    auto* keep = static_cast<std::uint8_t*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n))));
    auto* rows = static_cast<double*>(std::malloc(static_cast<std::size_t>(static_cast<std::int64_t>(n) * 0x30)));
    const double eps = 0.0001; // 14051f250
    for (std::int32_t i = 0; i < n; ++i) {
        keep[i] = 1;
        for (std::int32_t c = 0; c < 6; ++c) rows[i * 6 + c] = ctx->table[i * 6 + c];
    }
    for (std::int32_t i = 0; i < n - 1; ++i) {
        const double* a = &rows[i * 6];
        if (keep[i] == 0) continue;
        for (std::int32_t j = i + 1; j < n; ++j) {
            const double* b = &rows[j * 6];
            if (keep[j] == 0) continue;
            const double tol = ctx->tol;
            if (!fun_14040cd30(a[0], 1.0, eps) || !fun_14040cd30(b[0], 1.0, eps)) { // 140509c90
                if (fun_14040cd30(a[0], 2.0, eps) && fun_14040cd30(b[0], 2.0, eps) && // 140509fb8
                    fun_14040cd30(a[1], b[1], tol) && fun_14040cd30(a[2], b[2], tol) &&
                    fun_14040cd30(a[3], b[3], tol) && fun_14040cd30(a[4], b[4], tol) &&
                    fun_14040cd30(a[5], b[5], tol)) {
                    keep[j] = 0;
                }
            } else if (fun_14040cd30(a[1], b[3], tol) && fun_14040cd30(a[2], b[4], tol) &&
                       fun_14040cd30(a[3], b[1], tol) && fun_14040cd30(a[4], b[2], tol)) {
                keep[j] = 0;
            } else if (fun_14040cd30(a[1], b[1], tol) && fun_14040cd30(a[2], b[2], tol) &&
                       fun_14040cd30(a[3], b[3], tol) && fun_14040cd30(a[4], b[4], tol)) {
                keep[j] = 0;
            }
        }
    }
    std::int32_t k = 0;
    for (std::int32_t i = 0; i < n; ++i) {
        if (keep[i] == 1) {
            for (std::int32_t c = 0; c < 6; ++c) ctx->table[k * 6 + c] = rows[i * 6 + c];
            k = k + 1;
        }
    }
    ctx->n_records = k;
    std::free(keep);
    std::free(rows);
}

// @src 14040cd30  b - a <= eps && a - b <= eps
bool fun_14040cd30(double a, double b, double eps) { return b - a <= eps && a - b <= eps; }

// @src 14040ce00  obj+0x3a48 (the tolerance of the last read)
double fun_14040ce00(const LLOBJ_DATA* obj)
{
    double v;
    std::memcpy(&v, reinterpret_cast<const unsigned char*>(obj) + 0x3a48, 8);
    return v;
}

// @src 1401b8420  fopen(name, "at") + fclose: true when it could be opened (creates the file)
bool fun_1401b8420(const char* name)
{
    std::FILE* f = _fsopen(name, "at", _SH_DENYNO); // 14047b934, str 1405d123c
    if (f != nullptr) std::fclose(f);
    return f != nullptr;
}

// @src 1401b8530  _stat (14047ec78)
int fun_1401b8530(const char* name, struct _stat64i32* st) { return _stat64i32(name, st); }

// @src 1401b9100  copy src to src+suffix in 80-byte records ending "\r" "\n" (text mode: \r\r\n).
// *st: 0 ok, 1-16 errors (9: bytes read != file size, e.g. any file with CR LF); *count: records
void fun_1401b9100(const char* src, const char* suffix, std::int32_t* count, std::int32_t* st)
{
    *st = 0;
    *count = 0;
    if (src == nullptr) {
        *st = 1;
        return;
    }
    if (static_cast<int>(std::strlen(src)) < 1) {
        *st = 2;
        return;
    }
    if (suffix == nullptr) {
        *st = 3;
        return;
    }
    if (static_cast<int>(std::strlen(suffix)) < 1) {
        *st = 4;
        return;
    }
    if (!def::dbio::file_exists(src)) {
        *st = 5;
        return;
    }
    char dst[0x400];
    std::strcpy(dst, src);
    std::strcat(dst, suffix);
    if (def::dbio::file_exists(dst)) (void)def::dbio::file_exists(dst);
    struct _stat64i32 sb {};
    (void)fun_1401b8530(src, &sb);
    const std::int32_t size = static_cast<std::int32_t>(sb.st_size);
    if (size < 1) {
        *st = 6;
        return;
    }
    char* buf = static_cast<char*>(std::malloc(static_cast<std::size_t>(size + 1)));
    if (buf == nullptr) {
        *st = 7;
        return;
    }
    std::FILE* in = _fsopen(src, "rt", _SH_DENYNO); // str 1405d147c
    if (in == nullptr) {
        *st = 8;
        std::free(buf);
        return;
    }
    const std::int32_t got = static_cast<std::int32_t>(std::fread(buf, 1, static_cast<std::size_t>(size), in));
    std::fclose(in);
    if (got != size) {
        *st = 9;
        std::free(buf);
        return;
    }
    std::FILE* out = _fsopen(dst, "wt", _SH_DENYNO); // str 1405d1480
    if (out == nullptr) {
        *st = 10;
        std::free(buf);
        return;
    }
    const char* p = buf;
    std::int32_t done = 0;
    std::int32_t pieces = 0;
    char piece[0x50];
    do {
        if (size - done < 0x51) break;
        std::memcpy(piece, p, 0x50);
        p += 0x50;
        done += 0x50;
        if (std::fwrite(piece, 1, 0x50, out) != 0x50) {
            *st = 0xb;
            std::free(buf);
            std::fclose(out);
            return;
        }
        pieces = pieces + 1;
        if (std::fwrite("\r", 1, 1, out) != 1) { // str 1405d1484
            *st = 0xc;
            std::free(buf);
            std::fclose(out);
            return;
        }
        if (std::fwrite("\n", 1, 1, out) != 1) { // str 1405d1488
            *st = 0xd;
            std::free(buf);
            std::fclose(out);
            return;
        }
    } while (done < size);
    const std::int32_t rest = size - done;
    if (0 < rest) {
        std::memcpy(piece, p, static_cast<std::size_t>(rest));
        if (static_cast<std::int32_t>(std::fwrite(piece, 1, static_cast<std::size_t>(rest), out)) != rest) {
            *st = 0xe;
            std::free(buf);
            std::fclose(out);
            return;
        }
        pieces = pieces + 1;
        if (std::fwrite("\r", 1, 1, out) != 1) { // str 1405d148c
            *st = 0xf;
            std::free(buf);
            std::fclose(out);
            return;
        }
        if (std::fwrite("\n", 1, 1, out) != 1) { // str 1405d1490
            *st = 0x10;
            std::free(buf);
            std::fclose(out);
            return;
        }
    }
    *count = pieces;
    std::free(buf);
    std::fclose(out);
}

// @src 140197d00  clear the undo list obj+0x3a60 (vtable slot 1); returns 0
void fun_140197d00(LLOBJ_DATA* obj) { obj->u3a60.clear(); }

// @src 140197d40  undo: geometry 0 gets as many curves as the undo list holds, each a copy of the
// list's curve (14019a620 -> OBJ_GEO2D::assign 140276140, as p8-01 ports it); obj+0xbc8 = 0
void fun_140197d40(LLOBJ_DATA* obj)
{
    std::int32_t n = obj->u3a60.count(); // slot 0
    if (0 < n) {
        ext::f14027fe20(def::model::geo_kw::geometry(*obj, 0), &n);
        for (std::int32_t i = 0; i < n; ++i) {
            auto* item = static_cast<OBJ_GEO2D*>(fun_14019abd0(&obj->u3a60, i));
            if (item == nullptr) continue;
            OBJ_GEO2D* c = def::model::geo_kw::curve(*obj, i);
            c->assign(item);
        }
        ext::f14019a650(obj, 0);
    }
}

// @src 14019abd0  -> 14019abf0
void* fun_14019abd0(def::model::SdctlGList* list, std::int32_t i) { return fun_14019abf0(list, i); }

// @src 14019abf0  item of node i (0 when absent)
void* fun_14019abf0(def::model::SdctlGList* list, std::int32_t i)
{
    def::model::SdctlGListNode* node = fun_140331250(list, static_cast<std::uint32_t>(i));
    return node == nullptr ? nullptr : node->item;
}

// @src 140331250  node i of the list, walking from the cached node, the head or the tail
def::model::SdctlGListNode* fun_140331250(def::model::SdctlGList* list, std::uint32_t i)
{
    using Node = def::model::SdctlGListNode;
    if (i == static_cast<std::uint32_t>(list->cur_index)) return static_cast<Node*>(list->u20);
    if (list->u20 == nullptr && list->head != nullptr) {
        list->u20 = list->head;
        list->cur_index = 0;
    }
    std::uint32_t d = i - static_cast<std::uint32_t>(list->cur_index);
    const std::uint32_t n = static_cast<std::uint32_t>(list->n);
    if (!(i < n)) return nullptr;
    if (static_cast<std::int32_t>(d) < 0) d = 0u - d;
    Node* node;
    bool fwd;
    if (d < i && d < n - i) {
        node = static_cast<Node*>(list->u20);
        fwd = static_cast<std::uint32_t>(list->cur_index) < i;
    } else if (i < n - i) {
        node = static_cast<Node*>(list->head);
        fwd = true;
        d = i;
    } else {
        node = static_cast<Node*>(list->tail);
        d = (n - i) - 1;
        if (static_cast<std::int32_t>(d) < 0) d = 0;
        fwd = false;
    }
    if (fwd) {
        for (; d != 0; --d) node = node->next;
    } else {
        for (; d != 0; --d) node = static_cast<Node*>(node->u08);
    }
    list->cur_index = static_cast<std::int32_t>(i);
    list->u20 = node;
    return node;
}

void reset_globals()
{
    std::free(g_iges_rw);
    g_iges_rw = nullptr;
    std::free(g_iges_iw);
    g_iges_iw = nullptr;
    g_iges_ctx = IgesCtx{};
    g_iges_text = nullptr;
    g_out_path.clear();
    ftnrd::reset();
    reset_ftn_state();
    reset_pass1_state();
}

} // namespace def::cmd::iges
