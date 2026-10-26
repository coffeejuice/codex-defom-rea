// defcmd/src/dxf/dxf_parse.cpp: DXF group-code/value parsing and the entity handlers (p8-04).
// Spec: docs/re/commands/DXF_READER.md sections 4-6, 14.
#define _CRT_SECURE_NO_WARNINGS
#include "dxf_internal.hpp"

#include <cstdlib>
#include <cstring>
#include <share.h>

#include <defcmd/globals_p8_04.hpp>

#include "ext.hpp"

namespace def::cmd::dxf {

namespace {

// strings of the binary
constexpr const char* k_zero = "0";        // str 14056932c / 140569330
constexpr const char* k_two = "2";         // str 140569334
constexpr const char* k_binary = "AutoCAD Binary DXF";
constexpr const char* k_entities = "ENTITIES";

// 14048754c atof (VS2008 _atof_l(s, NULL))
double crt_atof(const char* s) { return std::atof(s); }
// 14047b6c0 atol (strtol(s, NULL, 10))
std::int32_t crt_atol(const char* s) { return static_cast<std::int32_t>(std::strtol(s, nullptr, 10)); }

// VS2008 toupper in the "C" locale: ASCII letters only
int c_toupper(int c) { return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c; }

// fgets into a 0x200 buffer
char* get_line(char* buf, std::FILE* f) { return std::fgets(buf, 0x200, f); }

} // namespace

// 0x14062be50: LINE (7 entries) and POLYLINE (first 4)
const CodeEntry k_codes_line[7] = {{"210", 4}, {"220", 5}, {"230", 6}, {"10", 0}, {"20", 1}, {"11", 2}, {"21", 3}};
// 0x14062bf00: LWPOLYLINE
const CodeEntry k_codes_lw[4] = {{"10", 0}, {"20", 1}, {"42", 2}, {"70", 4}};
// 0x14062bf40: VERTEX
const CodeEntry k_codes_vertex[3] = {{"10", 0}, {"20", 1}, {"42", 2}};
// 0x14062bf70: ARC
const CodeEntry k_codes_arc[8] = {{"210", 5}, {"220", 6}, {"230", 7}, {"10", 0}, {"20", 1}, {"40", 2}, {"50", 3}, {"51", 4}};
// 0x14062bff0: CIRCLE
const CodeEntry k_codes_circle[6] = {{"210", 3}, {"220", 4}, {"230", 5}, {"10", 0}, {"20", 1}, {"40", 2}};
// 0x14062c050: SPLINE
const CodeEntry k_codes_spline[12] = {{"210", 0}, {"220", 1}, {"230", 2}, {"70", 3}, {"71", 4}, {"72", 5},
                                      {"73", 6}, {"40", 7},  {"10", 8},  {"20", 9}, {"30", 10}, {"41", 11}};

// 0x14062bdb0
const EntityEntry k_entity_table[10] = {
    {"SECTION", fun_1404245b0}, {"ENDSEC", fun_1404246c0}, {"LINE", fun_140424700},
    {"POLYLINE", fun_140424da0}, {"LWPOLYLINE", fun_1404251e0}, {"VERTEX", fun_1404259c0},
    {"SEQEND", fun_140425cf0}, {"ARC", fun_140425d50}, {"CIRCLE", fun_1404262e0},
    {"SPLINE", fun_140426810},
};

// @src 140428650  both strings upper-cased into zeroed 512-byte buffers, then strcmp
std::int32_t fun_140428650(const char* a, const char* b)
{
    if (a == nullptr) return 1;
    if (b == nullptr) return -1;
    const int la = static_cast<int>(std::strlen(a));
    const int lb = static_cast<int>(std::strlen(b));
    char ua[0x200];
    char ub[0x200];
    std::memset(ua, 0, sizeof ua);
    std::memset(ub, 0, sizeof ub);
    int k = 0;
    for (int i = 0; i < la; ++i) ua[k++] = static_cast<char>(c_toupper(static_cast<int>(a[i])));
    k = 0;
    for (int i = 0; i < lb; ++i) ub[k++] = static_cast<char>(c_toupper(static_cast<int>(b[i])));
    return std::strcmp(ua, ub);
}

// @src 140427df0  skip leading spaces (when skip_lead), cut trailing '\n', '\r', ' '
char* fun_140427df0(char* s, std::int32_t skip_lead)
{
    char* p = s;
    if (skip_lead != 0) {
        while (*p == ' ') ++p;
    }
    if (std::strlen(p) != 0) {
        char* e = p + (std::strlen(p) - 1);
        while (p <= e && (*e == '\n' || *e == '\r' || *e == ' ')) --e;
        e[1] = '\0';
    }
    return p;
}

// @src 140424c80  code lookup: "0" or an entity name -> term; else the first n table entries
std::int32_t fun_140424c80(const char* code, const CodeEntry* table, std::int32_t n,
                           std::int32_t* idx, std::int32_t* term)
{
    *term = 0;
    if (fun_140428650(code, "0") == 0) {  // str 140569330
        *term = 1;
        return 0;
    }
    for (int i = 0; i < 10; ++i) {
        if (fun_140428650(code, k_entity_table[i].name) == 0) {
            *term = 1;
            return 0;
        }
    }
    for (int i = 0; i < n; ++i) {
        if (fun_140428650(code, table[i].code) == 0) {
            *idx = table[i].idx;
            return 1;
        }
    }
    return 0;
}

// @src 140428b10  name non-empty and _fsopen(name, "rt", 0x40) succeeds (closed again)
std::int32_t fun_140428b10(const char* name)
{
    if (name == nullptr) return 0;
    if (std::strlen(name) == 0) return 0;
    std::FILE* f = _fsopen(name, "rt", _SH_DENYNO);  // 14047b934, str 1405694d8
    if (f == nullptr) return 0;
    std::fclose(f);
    return 1;
}

// @src 140422f10  work struct: calloc(1, 0x60), +0 = arg, +0xc = 1
Work* fun_140422f10(std::uint64_t arg)
{
    Work* w = static_cast<Work*>(std::calloc(1, sizeof(Work)));
    if (w == nullptr) return nullptr;
    w->u00 = arg;
    w->u0c = 1;
    return w;
}

// @src 140424400  free the lines
static std::int32_t fun_140424400(Work* w)
{
    if (w->lines != nullptr) {
        std::free(w->lines);
        w->lines = nullptr;
    }
    w->n_lines = 0;
    return 1;
}

// @src 140424450  free the polylines and their vertices
static std::int32_t fun_140424450(Work* w)
{
    if (w->polys == nullptr) return 0;
    for (int i = 0; i < w->n_polys; ++i) {
        if (w->polys[i].v != nullptr) std::free(w->polys[i].v);
    }
    std::free(w->polys);
    w->polys = nullptr;
    w->n_polys = 0;
    w->cur_poly = nullptr;
    return 1;
}

// @src 140424510  free the arcs
static std::int32_t fun_140424510(Work* w)
{
    if (w->arcs != nullptr) {
        std::free(w->arcs);
        w->arcs = nullptr;
    }
    w->n_arcs = 0;
    return 1;
}

// @src 140424560  free the +0x48 array
static std::int32_t fun_140424560(Work* w)
{
    if (w->u48 != nullptr) {
        std::free(w->u48);
        w->u48 = nullptr;
        w->u40 = 0;
    }
    return 1;
}

// @src 140424390  free the work struct and null the pointer
void fun_140424390(Work** pw)
{
    if (*pw != nullptr) {
        fun_140424400(*pw);
        fun_140424450(*pw);
        fun_140424510(*pw);
        fun_140424560(*pw);
        std::free(*pw);
        *pw = nullptr;
    }
}

// @src 140427dd0  in ENTITIES flag
static std::int32_t fun_140427dd0(const Work* w) { return w->in_entities; }

// @src 140427ec0  v = (0, 0, 1)
static std::int32_t fun_140427ec0(double* v)
{
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 1.0;  // DAT_140509c90
    return 1;
}

// @src 140424b90  append n zeroed lines; returns the first new one
static LineRec* fun_140424b90(Work* w, std::int32_t n)
{
    if (w->lines == nullptr) {
        w->lines = static_cast<LineRec*>(std::calloc(static_cast<std::size_t>(n), sizeof(LineRec)));
        if (w->lines == nullptr) return nullptr;
    } else {
        w->lines = static_cast<LineRec*>(std::realloc(w->lines, static_cast<std::size_t>(w->n_lines + n) * sizeof(LineRec)));
        if (w->lines == nullptr) return nullptr;
        std::memset(w->lines + w->n_lines, 0, static_cast<std::size_t>(n) * sizeof(LineRec));
    }
    const int i = w->n_lines;
    w->n_lines += n;
    return w->lines + i;
}

// @src 1404250f0  append n zeroed polylines
static PolyRec* fun_1404250f0(Work* w, std::int32_t n)
{
    if (w->polys == nullptr) {
        w->polys = static_cast<PolyRec*>(std::calloc(static_cast<std::size_t>(n), sizeof(PolyRec)));
        if (w->polys == nullptr) return nullptr;
    } else {
        w->polys = static_cast<PolyRec*>(std::realloc(w->polys, static_cast<std::size_t>(w->n_polys + n) * sizeof(PolyRec)));
        if (w->polys == nullptr) return nullptr;
        std::memset(w->polys + w->n_polys, 0, static_cast<std::size_t>(n) * sizeof(PolyRec));
    }
    const int i = w->n_polys;
    w->n_polys += n;
    return w->polys + i;
}

// @src 1404258d0  append n zeroed vertices to a polyline
static Vertex* fun_1404258d0(PolyRec* p, std::int32_t n)
{
    if (p->v == nullptr) {
        p->v = static_cast<Vertex*>(std::calloc(static_cast<std::size_t>(n), sizeof(Vertex)));
        if (p->v == nullptr) return nullptr;
    } else {
        p->v = static_cast<Vertex*>(std::realloc(p->v, static_cast<std::size_t>(p->n_vert + n) * sizeof(Vertex)));
        if (p->v == nullptr) return nullptr;
        std::memset(p->v + p->n_vert, 0, static_cast<std::size_t>(n) * sizeof(Vertex));
    }
    const int i = p->n_vert;
    p->n_vert += n;
    return p->v + i;
}

// @src 1404261f0  append n zeroed arcs
static ArcRec* fun_1404261f0(Work* w, std::int32_t n)
{
    if (w->arcs == nullptr) {
        w->arcs = static_cast<ArcRec*>(std::calloc(static_cast<std::size_t>(n), sizeof(ArcRec)));
        if (w->arcs == nullptr) return nullptr;
    } else {
        w->arcs = static_cast<ArcRec*>(std::realloc(w->arcs, static_cast<std::size_t>(w->n_arcs + n) * sizeof(ArcRec)));
        if (w->arcs == nullptr) return nullptr;
        std::memset(w->arcs + w->n_arcs, 0, static_cast<std::size_t>(n) * sizeof(ArcRec));
    }
    const int i = w->n_arcs;
    w->n_arcs += n;
    return w->arcs + i;
}

// @src 140428e70  temporary polyline: calloc(1, 0x30)
static PolyRec* fun_140428e70() { return static_cast<PolyRec*>(std::calloc(1, sizeof(PolyRec))); }

// @src 140428ef0  clear a polyline (free the vertices)
static void fun_140428ef0(PolyRec* p)
{
    if (p == nullptr) return;
    p->n_vert = 0;
    if (p->v != nullptr) std::free(p->v);
    p->v = nullptr;
    p->flag = 0;
    p->ext[0] = 0.0;
    p->ext[1] = 0.0;
    p->ext[2] = 0.0;
}

// @src 140428eb0  clear and free a temporary polyline
static void fun_140428eb0(PolyRec* p)
{
    if (p != nullptr) {
        fun_140428ef0(p);
        std::free(p);
    }
}

// @src 140428b80  dst = copy of src (flag, extrusion, vertices appended one by one)
static std::int32_t fun_140428b80(const PolyRec* src, PolyRec* dst)
{
    if (src == nullptr) return -1;
    if (dst == nullptr) return -2;
    dst->n_vert = 0;
    dst->v = nullptr;
    dst->flag = src->flag;
    dst->ext[0] = src->ext[0];
    dst->ext[1] = src->ext[1];
    dst->ext[2] = src->ext[2];
    const Vertex* s = src->v;
    if (s == nullptr) return 0;
    for (int i = 0; i < src->n_vert; ++i) {
        Vertex* d = fun_1404258d0(dst, 1);
        if (d != nullptr) {
            d->x = s->x;
            d->y = s->y;
            d->bulge = s->bulge;
            ++s;
        }
    }
    return 0;
}

// @src 140428cf0  a 2-vertex polyline whose second vertex is not right of or above the first
// (signed, 1e-8 DAT_1405691a8) is cleared and reported as dropped (*drop = 1, returns 2)
static std::int32_t fun_140428cf0(PolyRec* p, PolyRec** replacement, std::int32_t* drop)
{
    *replacement = nullptr;
    *drop = 0;
    if (p == nullptr) return 0;
    if (p->n_vert != 2) return 0;
    const Vertex* v = p->v;
    if (v == nullptr) return 0;
    if (1e-8 < v[1].x - v[0].x || 1e-8 < v[1].y - v[0].y) return 0;
    fun_140428ef0(p);
    *drop = 1;
    return 2;
}

// @src 140422f70  pair loop: code/value pairs; "0" pairs dispatch through the entity table.
// Returns 0 (end of file, or a handler returned 0) or -10000 (binary DXF).
std::int32_t fun_140422f70(std::FILE* f, Work* w)
{
    char line[0x200];
    char code_buf[0x200];
    char value_buf[0x200];
    char* code = nullptr;
    char* value = nullptr;
    bool first = true;
    w->in_entities = 0;
    for (;;) {
        bool is_entity = false;
        do {
            if (get_line(line, f) == nullptr) return 0;
            is_entity = false;
            std::strcpy(code_buf, line);  // 140484f90
            code = fun_140427df0(code_buf, 1);
            if (first) {
                if (fun_140428650(code, k_binary) == 0) return -10000;
                first = false;
            }
            if (get_line(line, f) == nullptr) return 0;
            std::strcpy(value_buf, line);
            value = fun_140427df0(value_buf, 1);
            if (fun_140428650(code, k_zero) == 0) is_entity = true;
        } while (!is_entity);
        for (int i = 0; i < 10; ++i) {
            if (fun_140428650(value, k_entity_table[i].name) == 0 && k_entity_table[i].handler(f, w) == 0) {
                return 0;
            }
        }
    }
}

// @src 1404245b0  SECTION: one more pair; "2"/"ENTITIES" sets the flag
std::int32_t fun_1404245b0(std::FILE* f, Work* w)
{
    char a[0x200];
    char b[0x200];
    if (get_line(a, f) != nullptr && get_line(b, f) != nullptr) {
        char* c = fun_140427df0(a, 1);
        char* v = fun_140427df0(b, 1);
        if (fun_140428650(c, k_two) == 0 && fun_140428650(v, k_entities) == 0) w->in_entities = 1;
    }
    return 1;
}

// @src 1404246c0  ENDSEC
std::int32_t fun_1404246c0(std::FILE*, Work* w)
{
    w->in_entities = 0;
    return 1;
}

// @src 140424700  LINE
std::int32_t fun_140424700(std::FILE* f, Work* w)
{
    if (fun_140427dd0(w) == 0) return 1;
    std::int32_t term = 0;
    bool s10 = false, s20 = false, s11 = false, s21 = false, s210 = false, s220 = false, s230 = false;
    double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    double ext[3];
    fun_140427ec0(ext);
    std::int32_t idx = 0;
    char line[0x200];
    long save = std::ftell(f);
    do {
        if (get_line(line, f) == nullptr) break;
        char* code = fun_140427df0(line, 1);
        const std::int32_t found = fun_140424c80(code, k_codes_line, 7, &idx, &term);
        if (get_line(line, f) == nullptr) return 0;
        if (found != 0) {
            const double v = crt_atof(line);
            switch (idx) {
            case 0:
                if (!s10) { s10 = true; x1 = v; } else { term = 1; }
                break;
            case 1:
                if (s10) { if (!s20) { s20 = true; y1 = v; } else { term = 1; } }
                break;
            case 2:
                if (s10 && s20) { if (!s11) { s11 = true; x2 = v; } else { term = 1; } }
                break;
            case 3:
                if (s10 && s20 && s11) { if (!s21) { s21 = true; y2 = v; } else { term = 1; } }
                break;
            case 4:
                if (!s210) { s210 = true; ext[0] = v; } else { term = 1; }
                break;
            case 5:
                if (!s220) { s220 = true; ext[1] = v; } else { term = 1; }
                break;
            case 6:
                if (!s230) { s230 = true; ext[2] = v; } else { term = 1; }
                break;
            default: break;
            }
        }
        if (term != 0) {
            std::fseek(f, save, SEEK_SET);
            break;
        }
        save = std::ftell(f);
    } while (!s10 || !s20 || !s11 || !s21 || !s210 || !s220 || !s230);
    if (!s10 || !s20 || !s11 || !s21) return 0;
    LineRec* r = fun_140424b90(w, 1);
    if (r == nullptr) return 0;
    r->x1 = x1;
    r->y1 = y1;
    r->x2 = x2;
    r->y2 = y2;
    r->ext[0] = ext[0];
    r->ext[1] = ext[1];
    r->ext[2] = ext[2];
    return 1;
}

// @src 140424da0  POLYLINE. The code table is LINE's limited to 4 entries (210, 220, 230, 10), so
// only code 210 reaches a slot: idx 4, "closed = (value == 1.0)"; the ext slots 1..3 are never hit.
std::int32_t fun_140424da0(std::FILE* f, Work* w)
{
    std::int32_t term = 0;
    if (fun_140427dd0(w) == 0) return 1;
    PolyRec* p = fun_1404250f0(w, 1);
    if (p == nullptr) return 0;
    w->cur_poly = p;
    w->in_poly = 1;
    fun_140427ec0(p->ext);
    bool s_flag = false, s1 = false, s2 = false, s3 = false;
    std::int32_t closed = 0;
    double ext[3];
    fun_140427ec0(ext);
    std::int32_t idx = 0;
    char line[0x200];
    long save = std::ftell(f);
    do {
        if (get_line(line, f) == nullptr) break;
        char* code = fun_140427df0(line, 1);
        const std::int32_t found = fun_140424c80(code, k_codes_line, 4, &idx, &term);
        if (get_line(line, f) == nullptr) return 0;
        if (found != 0) {
            const double v = crt_atof(line);
            if (idx == 1) {
                if (!s1) { s1 = true; ext[0] = v; } else { term = 1; }
            } else if (idx == 2) {
                if (!s2) { s2 = true; ext[1] = v; } else { term = 1; }
            } else if (idx == 3) {
                if (!s3) { s3 = true; ext[2] = v; } else { term = 1; }
            } else if (idx == 4) {
                if (!s_flag) { closed = (v == 1.0) ? 1 : 0; s_flag = true; } else { term = 1; }  // DAT_140509c90
            }
        }
        if (term != 0) {
            std::fseek(f, save, SEEK_SET);
            break;
        }
        save = std::ftell(f);
    } while (!s_flag || !s1 || !s2 || !s3);
    if (s_flag) {
        p->flag = closed;
        p->ext[0] = ext[0];
        p->ext[1] = ext[1];
        p->ext[2] = ext[2];
    }
    return 1;
}

// @src 1404251e0  LWPOLYLINE (built in a temporary polyline, then copied unless dropped)
std::int32_t fun_1404251e0(std::FILE* f, Work* w)
{
    std::int32_t term = 0;
    std::int32_t flags = 0;
    double v = 0.0;
    double bulge = 0.0;
    bool first_x = true, first_y = true;
    double x0 = 0.0, y0 = 0.0;  // first x / first y (uninitialised in the original when unset)
    double cx = 0.0, cy = 0.0;
    std::int32_t idx = 0;
    if (fun_140427dd0(w) == 0) return 1;
    bool have_x = false, have_y = false;
    // 1404251e0 keeps a constant "use a temporary" flag = 1, so only that branch exists
    PolyRec* tmp = fun_140428e70();
    if (tmp == nullptr) return 0;
    fun_140427ec0(tmp->ext);
    char line[0x200];
    long save = std::ftell(f);
    while (get_line(line, f) != nullptr) {
        char* code = fun_140427df0(line, 1);
        std::int32_t found = fun_140424c80(code, k_codes_lw, 4, &idx, &term);
        if (get_line(line, f) == nullptr) return 0;
        if (found != 0) {
            v = crt_atof(line);
            if (idx == 0) {
                have_x = true;
                cx = v;
                if (first_x) {
                    first_x = false;
                    x0 = v;
                }
            } else if (idx == 1) {
                have_y = true;
                cy = v;
                if (first_y) {
                    first_y = false;
                    y0 = v;
                }
            } else if (idx == 4) {
                flags = crt_atol(line);
                tmp->flag = flags;
            }
            if (have_x && have_y) {
                have_x = false;
                have_y = false;
                bulge = 0.0;
                save = std::ftell(f);
                (void)get_line(line, f);  // return value not tested
                code = fun_140427df0(line, 1);
                found = fun_140424c80(code, k_codes_lw, 4, &idx, &term);
                if (get_line(line, f) == nullptr) return 0;
                if (found != 0 || term != 0) {
                    v = crt_atof(line);
                    if (idx == 2) bulge = v;
                    std::fseek(f, save, SEEK_SET);
                }
                Vertex* vx = fun_1404258d0(tmp, 1);
                if (vx == nullptr) return 0;
                vx->x = cx;
                vx->y = cy;
                vx->bulge = bulge;
            }
        }
        if (term != 0) {
            std::fseek(f, save, SEEK_SET);
            break;
        }
        save = std::ftell(f);
    }
    if (flags != 0) {
        Vertex* vx = fun_1404258d0(tmp, 1);
        if (vx == nullptr) return 0;
        vx->x = x0;
        vx->y = y0;
        vx->bulge = 0.0;
    }
    PolyRec* repl = nullptr;
    std::int32_t drop = 0;
    (void)fun_140428cf0(tmp, &repl, &drop);
    if (drop == 0) {
        PolyRec* p = fun_1404250f0(w, 1);
        if (p == nullptr) return 0;
        w->cur_poly = p;
        w->in_poly = 1;
        fun_140427ec0(p->ext);
        fun_140428b80(repl == nullptr ? tmp : repl, p);
        fun_140428eb0(tmp);
        fun_140428eb0(repl);
    } else {
        fun_140428eb0(tmp);
    }
    return 1;
}

// @src 1404259c0  VERTEX: appended to the current polyline while in a polyline
std::int32_t fun_1404259c0(std::FILE* f, Work* w)
{
    std::int32_t term = 0;
    if (fun_140427dd0(w) == 0) return 1;
    if (w->in_poly == 0) return 1;
    bool s42 = false, s20 = false, s10 = false;
    double x = 0.0, y = 0.0, bulge = 0.0;
    std::int32_t idx = 0;
    char line[0x200];
    long save = std::ftell(f);
    while (get_line(line, f) != nullptr) {
        char* code = fun_140427df0(line, 1);
        const std::int32_t found = fun_140424c80(code, k_codes_vertex, 3, &idx, &term);
        if (get_line(line, f) == nullptr) return 0;
        if (found != 0) {
            const double v = crt_atof(line);
            if (idx == 0) {
                if (!s10) { s10 = true; x = v; } else { term = 1; }
            } else if (idx == 1) {
                if (!s20) { s20 = true; y = v; } else { term = 1; }
            } else if (idx == 2) {
                if (!s42 && w->in_poly != 0) {
                    s42 = true;
                    bulge = v;
                } else {
                    term = 1;
                }
            }
        }
        if (term != 0) {
            std::fseek(f, save, SEEK_SET);
            break;
        }
        save = std::ftell(f);
        if (s10 && s20 && (w->in_poly == 0 || s42)) break;
    }
    if (!s10 || !s20) return 0;
    if (w->cur_poly != nullptr) {
        Vertex* vx = fun_1404258d0(w->cur_poly, 1);
        if (vx == nullptr) return 0;
        vx->x = x;
        vx->y = y;
        vx->bulge = bulge;
    }
    return 1;
}

// @src 140425cf0  SEQEND
std::int32_t fun_140425cf0(std::FILE*, Work* w)
{
    if (fun_140427dd0(w) != 0 && w->in_poly != 0) w->in_poly = 0;
    return 1;
}

// @src 140425d50  ARC (the extrusion is read but not stored: the record keeps (0, 0, 0))
std::int32_t fun_140425d50(std::FILE* f, Work* w)
{
    std::int32_t term = 0;
    if (fun_140427dd0(w) == 0) return 1;
    bool s51 = false, s50 = false, s40 = false, s20 = false, s10 = false, s230 = false, s220 = false, s210 = false;
    double cx = 0, cy = 0, r = 0, a50 = 0, a51 = 0;
    double ext[3];
    fun_140427ec0(ext);
    std::int32_t idx = 0;
    char line[0x200];
    long save = std::ftell(f);
    do {
        if (get_line(line, f) == nullptr) break;
        char* code = fun_140427df0(line, 1);
        const std::int32_t found = fun_140424c80(code, k_codes_arc, 8, &idx, &term);
        if (get_line(line, f) == nullptr) return 0;
        if (found != 0) {
            const double v = crt_atof(line);
            switch (idx) {
            case 0: if (!s10) { s10 = true; cx = v; } else { term = 1; } break;
            case 1: if (!s20) { s20 = true; cy = v; } else { term = 1; } break;
            case 2: if (s10) { if (!s40) { s40 = true; r = v; } else { term = 1; } } break;
            case 3: if (!s50) { s50 = true; a50 = v; } else { term = 1; } break;
            case 4: if (!s51) { s51 = true; a51 = v; } else { term = 1; } break;
            case 5: if (!s210) { s210 = true; ext[0] = v; } else { term = 1; } break;
            case 6: if (!s220) { s220 = true; ext[1] = v; } else { term = 1; } break;
            case 7: if (!s230) { s230 = true; ext[2] = v; } else { term = 1; } break;
            default: break;
            }
        }
        if (term != 0) {
            std::fseek(f, save, SEEK_SET);
            break;
        }
        save = std::ftell(f);
    } while (!s10 || !s20 || !s40 || !s50 || !s51);
    if (!s10 || !s20 || !s40 || !s50 || !s51) return 0;
    ArcRec* a = fun_1404261f0(w, 1);
    if (a == nullptr) return 0;
    a->cx = cx;
    a->cy = cy;
    a->r = r;
    a->a50 = a50;
    a->a51 = a51;
    return 1;
}

// @src 1404262e0  CIRCLE: three 120-degree arcs with the extrusion
std::int32_t fun_1404262e0(std::FILE* f, Work* w)
{
    std::int32_t term = 0;
    if (fun_140427dd0(w) == 0) return 1;
    bool s40 = false, s20 = false, s10 = false, s230 = false, s220 = false, s210 = false;
    double cx = 0, cy = 0, r = 0;
    double ext[3];
    fun_140427ec0(ext);
    std::int32_t idx = 0;
    char line[0x200];
    long save = std::ftell(f);
    do {
        if (get_line(line, f) == nullptr) break;
        char* code = fun_140427df0(line, 1);
        const std::int32_t found = fun_140424c80(code, k_codes_circle, 6, &idx, &term);
        if (get_line(line, f) == nullptr) return 0;
        if (found != 0) {
            const double v = crt_atof(line);
            switch (idx) {
            case 0: if (!s10) { s10 = true; cx = v; } else { term = 1; } break;
            case 1: if (!s20) { s20 = true; cy = v; } else { term = 1; } break;
            case 2: if (!s40) { s40 = true; r = v; } else { term = 1; } break;
            case 3: if (!s210) { s210 = true; ext[0] = v; } else { term = 1; } break;
            case 4: if (!s220) { s220 = true; ext[1] = v; } else { term = 1; } break;
            case 5: if (!s230) { s230 = true; ext[2] = v; } else { term = 1; } break;
            default: break;
            }
        }
        if (term != 0) {
            std::fseek(f, save, SEEK_SET);
            break;
        }
        save = std::ftell(f);
    } while (!s10 || !s20 || !s40 || !s210 || !s220 || !s230);
    if (!s10 || !s20 || !s40) return 0;
    // DAT_140541428 120.0, DAT_140569508 240.0, DAT_14051f210 360.0
    const double a[4] = {0.0, 120.0, 240.0, 360.0};
    for (int k = 0; k < 3; ++k) {
        ArcRec* rec = fun_1404261f0(w, 1);
        if (rec == nullptr) return 0;
        rec->cx = cx;
        rec->cy = cy;
        rec->r = r;
        rec->a50 = a[k];
        rec->a51 = a[k + 1];
        rec->ext[0] = ext[0];
        rec->ext[1] = ext[1];
        rec->ext[2] = ext[2];
    }
    return 1;
}

// @src 140409ae0  SPLINE sample count (DAT_140628550, p8-03's g_iges_nurbs_samples)
std::int32_t fun_140409ae0() { return spline_samples(); }

// @src 140426810  SPLINE: control points and knots, sampled at fun_140409ae0() points by the
// Fortran routine 140463374 (p8-03) with unit weights; the samples become an open polyline.
std::int32_t fun_140426810(std::FILE* f, Work* w)
{
    std::int32_t term = 0;
    if (fun_140427dd0(w) == 0) return 1;
    bool s10 = false, s20 = false;
    std::int32_t ci = 0;  // control-point index (advanced by code 30)
    PolyRec* p = fun_1404250f0(w, 1);
    if (p == nullptr) return 0;
    w->cur_poly = p;
    w->in_poly = 1;
    fun_140427ec0(p->ext);
    double ext_unused[3];
    fun_140427ec0(ext_unused);
    std::int32_t idx = 0;
    std::int32_t degree = 0;
    std::int32_t n_knots = 0;
    std::int32_t n_ctrl = 0;
    std::int32_t ki = 0;
    double* ctrl = nullptr;
    double* knots = nullptr;
    char line[0x200];
    long save = std::ftell(f);
    do {
        if (get_line(line, f) == nullptr) break;
        char* code = fun_140427df0(line, 1);
        const std::int32_t found = fun_140424c80(code, k_codes_spline, 12, &idx, &term);
        if (get_line(line, f) == nullptr) return 0;
        if (found != 0) {
            const double v = crt_atof(line);
            switch (idx - 4) {
            case 0: degree = static_cast<std::int32_t>(v); break;
            case 1:
                n_knots = static_cast<std::int32_t>(v);
                knots = static_cast<double*>(std::malloc(static_cast<std::size_t>(n_knots) * 8));
                ki = 0;
                break;
            case 2:
                n_ctrl = static_cast<std::int32_t>(v);
                ctrl = static_cast<double*>(std::malloc(static_cast<std::size_t>(n_ctrl * 3) * 8));
                break;
            case 3: knots[ki++] = v; break;  // NULL write without a preceding 72 (as the original)
            case 4: s10 = true; ctrl[ci * 3] = v; break;
            case 5: s20 = true; ctrl[ci * 3 + 1] = v; break;
            case 6:
                if (s10 && s20) {
                    ctrl[ci * 3 + 2] = v;
                    ++ci;
                }
                break;
            default: break;
            }
        }
        if (term != 0) {
            std::fseek(f, save, SEEK_SET);
            break;
        }
        save = std::ftell(f);
    } while (!s10 || !s20 || w->in_poly != 0);
    double t0 = knots[0];
    double tn = knots[n_knots - 1];
    std::int32_t one = 1;
    std::int32_t nout = fun_140409ae0();
    double* out = static_cast<double*>(std::malloc(static_cast<std::size_t>(nout * 3) * 8));
    double* weights = static_cast<double*>(std::malloc(static_cast<std::size_t>(n_ctrl) * 8));
    for (int i = 0; i < n_ctrl; ++i) weights[i] = 1.0;  // DAT_140509c90
    ext::f140463374(ctrl, knots, weights, &n_ctrl, &degree, out, &nout, &one, &t0, &tn);
    if (w->cur_poly != nullptr) {
        for (int i = 0; i < nout; ++i) {
            Vertex* vx = fun_1404258d0(w->cur_poly, 1);
            if (vx == nullptr) return 0;
            vx->x = out[i * 3];
            vx->y = out[i * 3 + 1];
        }
    }
    w->in_poly = 0;
    std::free(out);  // 1404797d8 (operator new = malloc) buffers, freed with free()
    std::free(ctrl);
    std::free(knots);
    std::free(weights);
    return 1;
}

} // namespace def::cmd::dxf
