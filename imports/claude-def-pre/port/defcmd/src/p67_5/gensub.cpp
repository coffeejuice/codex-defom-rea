// case 0x14 GENSUB (packet p67-5). Spec: docs/re/commands/GENSUB.md. Listings:
// deform-rea-work/out/p675/gensub/disp.lst (dispatcher case), conv.lst (FUN_14037e5c0 and the
// file / line / connectivity helpers).
//
// The converter FUN_14037e5c0 keeps all its scalars in one stack frame, and several keyword
// blocks scan into the same slots (the RZ count C at +0x6fc is also the second count of NDTMP,
// STRAIN, DAMAGE and USRNOD; +0x6f8 is the row value of every "%d %f" reader and the float of the
// STNCMP header; ...). The port keeps that frame as bytes addressed by the listing's RSP offsets,
// so every stale value the original carries from one block to the next is carried the same way.
// Slots the original reads before writing them are zero here (the original reads whatever the
// stack held; only rows with a partial fscanf result can see such a value).
#define _CRT_SECURE_NO_WARNINGS
#include <defcmd/p67_5.hpp>
#include <defcmd/p67_5_gensub.hpp>
#include <defcmd/contact_cmds.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <share.h>
#include <string>

#include <defdbio/gendb/gendb.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/line_io.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj/cls_140182880.hpp>
#include <defmodel/llobj_ctor/llobj_copy.hpp>
#include <defrt/stdlib/crt_emulation.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::cmd {

namespace gensub {

namespace {

namespace crt = def::rt::crt;

// @src 14047b934  fopen wrapper: _fsopen(name, mode, _SH_DENYNO)
std::FILE* fsopen(const char* name, const char* mode) { return ::_fsopen(name, mode, _SH_DENYNO); }

template <class... T>
int sc(const char* s, const char* fmt, T*... targets)
{
    void* const a[] = {static_cast<void*>(targets)...};
    return crt_sscanf(s, fmt, a);
}

template <class... T>
int fs(std::FILE* f, const char* fmt, T*... targets)
{
    void* const a[] = {static_cast<void*>(targets)...};
    return crt_fscanf(f, fmt, a);
}

constexpr double kLimit = 1.0e7;  // 14051f1a8

// An array the converter allocated with 140257150 / 140256dc0 (calloc, NULL for 0 elements).
// The size is kept so that the port can refuse the out-of-bounds accesses of the original.
template <class T>
struct Arr {
    T* p = nullptr;
    std::int64_t n = 0;
    void alloc(std::int32_t count)
    {
        p = static_cast<T*>(sizeof(T) == 8 ? model::llobj_copy::calloc_double(count) : model::llobj_copy::calloc_int(count));
        n = p != nullptr ? count : 0;
    }
    explicit operator bool() const { return p != nullptr; }
    void put(std::int64_t i, T v)
    {
        // the original's stores are all guarded by its count checks; one outside the array is
        // only possible with a NULL array from a failed calloc (negative product of counts)
        if (p == nullptr || i < 0 || i >= n) def::rt::abnormal_exit(def::rt::kStatusAccessViolation);
        p[i] = v;
    }
    T get(std::int64_t i) const
    {
        if (p == nullptr || i < 0 || i >= n) {
            ++oob_reads();  // see p67_5_gensub.hpp: the original prints heap contents here
            return T{};
        }
        return p[i];
    }
    void release()
    {
        std::free(p);
        p = nullptr;
    }
};

// The converter's stack frame (RSP offsets of FUN_14037e5c0 after its prologue).
struct Frame {
    alignas(8) unsigned char b[0x760] = {};
    std::int32_t& i(int off) { return *reinterpret_cast<std::int32_t*>(b + off); }
    float& f(int off) { return *reinterpret_cast<float*>(b + off); }
    void* a(int off) { return b + off; }
    char* s(int off) { return reinterpret_cast<char*>(b + off); }
};

// Slots (RSP offsets) with a fixed role
enum : int {
    kObj = 0x118,      // object number of the last header (every header but ELMCON)
    kStatus = 0x180,   // block status
    kI = 0x184,        // row loop counter
    kNN = 0x704,       // nodes read (RZ rows)
    kNE = 0x1e0,       // elements read (ELMCON rows)
    kC = 0x6fc,        // RZ count; second count of NDTMP / STRAIN / DAMAGE / USRNOD
    kN = 0x25c,        // ELMCON / STRAIN / STNCMP / DAMAGE / GRAIN / USRELM / MATAXI count
    kNpe = 0xe0,       // ELMCON nodes per element
    kCT = 0x248,       // BCCTMP count
    kCD = 0x108,       // BCCDEF count
    kCX = 0x20c,       // ECCTMP count
    kDX = 0x138,       // ECCTMP header value 4 (also the 6th ECCTMP row value)
    kSeenX = 0x198,    // an ECCTMP line was seen
    kHasUE = 0x688,
    kHasUN = 0x684,
    kCU = 0x680,       // USRELM count copy
    kNvE = 0x6f0,
    kNvN = 0x6b0,
    kC6 = 0x250,       // STNCMP count copy
    kCM = 0xa8,        // MATAXI count copy
    kK = 0x70c,        // inner loop counter
    kTok = 0x270,      // header token (512 bytes)
    kLine = 0x470,     // line buffer (512 bytes)
    kLineMax = 0x708,  // 500
};

// "\tERROR : <what> could not be read\n" appended to DEF_CTRL.OUT (mode "a")
void ctrl_error(const char* text)
{
    std::FILE* c = fsopen("DEF_CTRL.OUT", "a");
    crt::fprintf(c, "%s", text);
    std::fclose(c);
}

// The row loops of RZ and ELMCON advance only on a result other than -1, which fscanf returns
// only at the end of the file: there the original loops for ever.
[[noreturn]] void hang(const char* what)
{
    throw InputHang(std::string("GENSUB converter: end of AX_BIL.KEY inside the ") + what +
                    " rows; the original loops for ever here (FUN_14037e5c0)");
}

// @src 1401b8320  line reader: getc until '\n' or n - 1 characters; NULL at EOF with nothing read
char* get_line(char* buf, int n, std::FILE* f)
{
    char* p = buf;
    int c = 0;
    for (;;) {
        --n;
        if (n < 1) break;
        c = std::getc(f);
        if (c == -1) break;
        *p++ = static_cast<char>(c);
        if (static_cast<char>(c) == '\n') break;
    }
    *p = 0;
    if (c == -1 && p == buf) return nullptr;
    return buf;
}

// 140183cb0  connectivity of the element set obj+0x9e0 (sync 1401d5820, then +8); owner defcmd contact/contact_cmds.cpp element_set_conn (p67)
// @src 1401d2bb0  returns 1 on both branches (tests model+0x1b59)
const std::int32_t* connectivity(model::LLOBJ_DATA& obj)
{
    return element_set_conn(reinterpret_cast<unsigned char*>(&obj) + 0x9e0);
}

} // namespace

std::vector<std::FILE*>& leaked_files()
{
    static std::vector<std::FILE*> v;
    return v;
}

void close_leaked_files()
{
    for (std::FILE* f : leaked_files()) std::fclose(f);
    leaked_files().clear();
}

std::int32_t& oob_reads()
{
    static std::int32_t n = 0;
    return n;
}

// @src 1401b83d0  strlen of the line, 0 at EOF
std::size_t read_line(char* buf, int n, std::FILE* f)
{
    if (get_line(buf, n, f) == nullptr) return 0;
    return std::strlen(buf);
}

// 1401b85d0: ported in defdbio/gendb/gendb.hpp (remove_if_exists): delete the file if it can be
// opened ("rt")
int delete_if_exists(const char* name) { return dbio::gendb::remove_if_exists(name); }

// @src 14037e5c0  AX_BIL.KEY -> MESH.DAT converter
int convert_key_to_mesh(model::DB_STEP_DATA&, const char* in_name, const char* out_name, std::int32_t* o_obj,
                        std::int32_t* o_nodes, std::int32_t* o_elems, const std::int32_t* mesh_no)
{
    Frame fr;
    std::FILE* in = fsopen(in_name, "rt");  // str 140554b24
    if (in == nullptr) return 1;
    fr.i(kLineMax) = 500;
    fr.i(0x1a0) = 0x10;  // 14037e796; the other initialised slots are 0 (14037e664..14037e86b)
    Arr<double> rz;            // 0x228
    Arr<std::int32_t> el;      // 0x1d8
    Arr<double> ndtmp;         // 0xb0
    Arr<double> strE;          // 0x6a8
    Arr<double> strN;          // 0x1b0
    Arr<double> stn;           // 0x190
    Arr<std::int32_t> stnIdx;  // 0x6d8
    Arr<double> damE;          // 0xf0
    Arr<double> damN;          // 0x670
    Arr<double> grain;         // 0x220
    Arr<double> usrE;          // 0x6e8
    Arr<double> usrN;          // 0x200
    Arr<std::int32_t> bcctmp;  // 0x6e0
    Arr<std::int32_t> ecctmp;  // 0x690
    Arr<std::int32_t> bccdef;  // 0x218
    Arr<double> mataxi;        // 0x130
    Arr<std::int32_t> matIdx;  // 0x1f8

    char* line = fr.s(kLine);
    char* tok = fr.s(kTok);
    std::int32_t& S = fr.i(kStatus);
    std::int32_t& I = fr.i(kI);
    std::int32_t& NN = fr.i(kNN);
    std::int32_t& NE = fr.i(kNE);
    std::int32_t& C = fr.i(kC);
    std::int32_t& N = fr.i(kN);
    std::int32_t& K = fr.i(kK);
    auto valid = [](std::int32_t n) { return n > 0 && static_cast<double>(n) < kLimit; };
    auto fail_leaving_open = [&](const char* msg) {
        ctrl_error(msg);
        leaked_files().push_back(in);  // `in` is not closed on these returns
        return S;
    };
    // node rows "%d %f\n" into a node array (NDTMP, STRAIN / DAMAGE node part)
    auto node_rows = [&](Arr<double>& a) {
        a.alloc(NN);
        for (I = 0; I < C; ++I) {
            const int r = fs(in, "%d %f\n", fr.a(0x210), fr.a(0x6f8));
            if (r != -1 && fr.i(0x210) > 0 && fr.i(0x210) <= NN) a.put(fr.i(0x210) - 1, static_cast<double>(fr.f(0x6f8)));
        }
    };
    // element rows "%d %f\n" (STRAIN / DAMAGE element part); an invalid count is no error
    auto elem_rows = [&](Arr<double>& a) {
        if (valid(N) && N <= NE && !a) {
            a.alloc(NE);
            for (I = 0; I < N; ++I) {
                const int r = fs(in, "%d %f\n", fr.a(0x1e4), fr.a(0x6f8));
                if (r != -1 && fr.i(0x1e4) > 0 && fr.i(0x1e4) <= NE)
                    a.put(fr.i(0x1e4) - 1, static_cast<double>(fr.f(0x6f8)));
            }
        }
    };

    for (;;) {
        if (read_line(line, fr.i(kLineMax), in) == 0) break;

        // ---- RZ (14037e89a): also entered by DRZ / URZ / FRZ / PRZ lines -------------------------
        if (S == 0 && std::strstr(line, "RZ") != nullptr) {
            sc(line, "%s %d %d", tok, fr.a(kObj), fr.a(kC));
            if (std::strcmp(tok, "RZ") == 0) {
                if (valid(C) && !rz) {
                    rz.alloc(C * 3);
                    std::int64_t cur = 0;  // 0x1b8
                    NN = 0;
                    while (NN < C) {
                        const int r = fs(in, "%*d %f %f %f\n", fr.a(0xf8), fr.a(0xfc), fr.a(0x100));
                        if (r == -1) hang("RZ");
                        rz.put(cur++, static_cast<double>(fr.f(0xf8)));
                        rz.put(cur++, static_cast<double>(fr.f(0xfc)));
                        rz.put(cur++, static_cast<double>(fr.f(0x100)));
                        ++NN;
                    }
                } else {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : RZ co-ordinates could not be read\n");

        // ---- BCCTMP (14037eac7) -------------------------------------------------------------------
        if (S == 0 && std::strstr(line, "BCCTMP") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %f", tok, fr.a(kObj), fr.a(kCT), fr.a(0x13c));
            const std::int32_t cT = fr.i(kCT);
            if (std::strcmp(tok, "BCCTMP") == 0 && cT > 0) {
                if (valid(cT) && cT <= NN && !bcctmp) {
                    bcctmp.alloc(cT << 1);
                    for (I = 0; I < fr.i(kCT); ++I) {
                        const int r = fs(in, "%d %d\n", fr.a(0x210), fr.a(0x700));
                        if (r != -1) {
                            bcctmp.put(I * 2, fr.i(0x210));
                            bcctmp.put(I * 2 + 1, fr.i(0x700));
                        }
                    }
                } else if (cT > 0) {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : BCCTMP could not be read\n");

        // ---- ELMCON (14037ecca): no strcmp; an invalid count sets status = npe ------------------
        if (S == 0 && std::strstr(line, "ELMCON") != nullptr) {
            sc(line, "%s %d %d %d", tok, fr.a(0x24c), fr.a(kN), fr.a(kNpe));
            if (valid(N) && !el) {
                el.alloc(N * fr.i(kNpe));
                std::int64_t cur = 0;  // 0x110
                NE = 0;
                if (fr.i(kNpe) == 8) {
                    while (NE < N) {
                        const int r = fs(in, "%*d %d %d %d %d %d %d %d %d\n", fr.a(0x6b8), fr.a(0x6bc), fr.a(0x6c0),
                                         fr.a(0x6c4), fr.a(0x6c8), fr.a(0x6cc), fr.a(0x6d0), fr.a(0x6d4));
                        if (r == -1) hang("ELMCON");
                        for (int k = 0; k < 8; ++k) el.put(cur++, fr.i(0x6b8 + 4 * k));
                        ++NE;
                    }
                } else {
                    S = fr.i(kNpe);
                }
            } else {
                S = fr.i(kNpe);
            }
        }
        if (S == 4) {
            leaked_files().push_back(in);  // 14037efab: returns 4, `in` stays open
            return 4;
        }
        if (S == 1) return fail_leaving_open("\tERROR : ELMCON could not be read\n");

        // ---- BCCDEF (14037f009) -------------------------------------------------------------------
        if (S == 0 && std::strstr(line, "BCCDEF") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %f", tok, fr.a(kObj), fr.a(kCD), fr.a(0x258));
            const std::int32_t cD = fr.i(kCD);
            if (std::strcmp(tok, "BCCDEF") == 0 && cD > 0) {
                if (valid(cD) && cD <= NN && !bccdef) {
                    bccdef.alloc(cD << 2);
                    for (I = 0; I < fr.i(kCD); ++I) {
                        const int r = fs(in, "%d %d %d %d\n", fr.a(0x210), fr.a(0x238), fr.a(0x23c), fr.a(0x240));
                        if (r != -1) {
                            bccdef.put(I * 4, fr.i(0x210));
                            bccdef.put(I * 4 + 1, fr.i(0x238));
                            bccdef.put(I * 4 + 2, fr.i(0x23c));
                            bccdef.put(I * 4 + 3, fr.i(0x240));
                        }
                    }
                } else if (cD > 0) {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : BCCDEF could not be read\n");

        // ---- NDTMP (14037f279) --------------------------------------------------------------------
        if (S == 0 && std::strstr(line, "NDTMP") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %f", tok, fr.a(kObj), fr.a(kC), fr.a(0x208));
            if (std::strcmp(tok, "NDTMP") == 0 && NN > 0) {
                if (valid(C) && C <= NN && !ndtmp)
                    node_rows(ndtmp);
                else if (C > 0)
                    S = 1;
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : NDTMP could not be read\n");

        // ---- STRAIN (14037f47c) -------------------------------------------------------------------
        if (S == 0 && std::strstr(line, "STRAIN") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %f %d %f", tok, fr.a(kObj), fr.a(kN), fr.a(0x6f4), fr.a(kC), fr.a(0x19c));
            if (std::strcmp(tok, "STRAIN") == 0 && NE > 0) {
                elem_rows(strE);
                if (valid(C) && C <= NN && !strN)
                    node_rows(strN);
                else if (C > 0)
                    S = 1;
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : STRAIN could not be read\n");

        // ---- STNCMP (14037f799): the error test uses the stale C ---------------------------------
        if (S == 0 && std::strstr(line, "STNCMP") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %d %f", tok, fr.a(kObj), fr.a(kN), fr.a(0x1a8), fr.a(0x6f8));
            if (std::strcmp(tok, "STNCMP") == 0 && NE > 0) {
                if (valid(N) && N <= NE && !stn) {
                    stn.alloc(NE * 6);
                    stnIdx.alloc(N);
                    fr.i(kC6) = N;
                    for (I = 0; I < N; ++I) {
                        const int r = fs(in, "%d %f %f %f %f %f %f\n", fr.a(0x1e4), fr.a(0x1c0), fr.a(0x1c4), fr.a(0x1c8),
                                         fr.a(0x1cc), fr.a(0x1d0), fr.a(0x1d4));
                        const std::int32_t e = fr.i(0x1e4);
                        if (r != -1 && e > 0 && e <= NE) {
                            stnIdx.put(I, e);
                            for (K = 0; K < 6; ++K)
                                stn.put(static_cast<std::int64_t>((e - 1) * 6 + K), static_cast<double>(fr.f(0x1c0 + 4 * K)));
                        }
                    }
                } else if (C > 0) {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : STNCMP could not be read\n");

        // ---- DAMAGE (14037fa71) -------------------------------------------------------------------
        if (S == 0 && std::strstr(line, "DAMAGE") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %f %d %f", tok, fr.a(kObj), fr.a(kN), fr.a(0x230), fr.a(kC), fr.a(0x254));
            if (std::strcmp(tok, "DAMAGE") == 0 && NE > 0) {
                elem_rows(damE);
                if (valid(C) && C <= NN && !damN)
                    node_rows(damN);
                else if (C > 0)
                    S = 1;
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : DAMAGE could not be read\n");

        // ---- GRAIN (14037fd8e): the error test uses the stale C ----------------------------------
        if (S == 0 && std::strstr(line, "GRAIN") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %d", tok, fr.a(kObj), fr.a(kN), fr.a(0x1a0));
            if (std::strcmp(tok, "GRAIN") == 0 && NE > 0) {
                if (valid(N) && N <= NE && !grain) {
                    grain.alloc(NE * 16);
                    for (I = 0; I < N; ++I) {
                        void* t[17];
                        t[0] = fr.a(0x1e4);
                        for (int k = 0; k < 16; ++k) t[1 + k] = fr.a(0x140 + 4 * k);
                        const int r = crt_fscanf(in, "%d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n", t);
                        const std::int32_t e = fr.i(0x1e4);
                        if (r != -1 && e > 0 && e <= NE)
                            for (K = 0; K < 16; ++K)
                                grain.put(static_cast<std::int64_t>((e - 1) * 16 + K), static_cast<double>(fr.f(0x140 + 4 * K)));
                    }
                } else if (C > 0) {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : GRAIN could not be read\n");

        // ---- USRELM (1403800a0): values stored by row position, not by element id ---------------
        if (S == 0 && std::strstr(line, "USRELM") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %f %d", tok, fr.a(kObj), fr.a(kN), fr.a(0x67c), fr.a(kNvE));
            fr.i(kCU) = N;
            if (std::strcmp(tok, "USRELM") == 0 && NE > 0) {
                if (valid(N) && N <= NE && !usrE) {
                    usrE.alloc(fr.i(kNvE) * N);
                    for (I = 0; I < N; ++I) {
                        const int r = fs(in, "%d ", fr.a(0x1e4));
                        if (r != -1 && fr.i(0x1e4) > 0 && fr.i(0x1e4) <= NE)
                            for (K = 0; K < fr.i(kNvE); ++K) {
                                fs(in, "%f", fr.a(0x6f8));
                                usrE.put(static_cast<std::int64_t>(fr.i(kNvE) * I + K), static_cast<double>(fr.f(0x6f8)));
                            }
                    }
                    fr.i(kHasUE) = 1;
                } else if (N > 0) {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : USRELM could not be read\n");

        // ---- USRNOD (14038032d) -------------------------------------------------------------------
        if (S == 0 && std::strstr(line, "USRNOD") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %f %d", tok, fr.a(kObj), fr.a(kC), fr.a(0x678), fr.a(kNvN));
            if (std::strcmp(tok, "USRNOD") == 0 && NN > 0) {
                if (valid(C) && C <= NN && !usrN) {
                    usrN.alloc(fr.i(kNvN) * C);
                    for (I = 0; I < C; ++I) {
                        const int r = fs(in, "%d ", fr.a(0x210));
                        if (r != -1 && fr.i(0x210) > 0 && fr.i(0x210) <= NN)
                            for (K = 0; K < fr.i(kNvN); ++K) {
                                fs(in, "%f", fr.a(0x6f8));
                                usrN.put(static_cast<std::int64_t>(fr.i(kNvN) * I + K), static_cast<double>(fr.f(0x6f8)));
                            }
                    }
                    fr.i(kHasUN) = 1;
                } else if (C > 0) {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : USRNOD could not be read\n");

        // ---- ECCTMP (1403805ac): seenX is set even when the token differs ------------------------
        if (S == 0 && std::strstr(line, "ECCTMP") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %d", tok, fr.a(kObj), fr.a(kCX), fr.a(kDX));
            fr.i(kSeenX) = 1;
            const std::int32_t cX = fr.i(kCX);
            if (std::strcmp(tok, "ECCTMP") == 0 && NE > 0) {
                if (valid(cX) && cX <= NE && !ecctmp) {
                    ecctmp.alloc(cX * 5);
                    for (I = 0; I < fr.i(kCX); ++I) {
                        const int r = fs(in, "%d %d %d %d %d %d\n", fr.a(0x1e4), fr.a(0x1a8), fr.a(0xa0), fr.a(0x1a4),
                                         fr.a(0x1e8), fr.a(kDX));
                        if (r != -1 && fr.i(0x1e4) > 0 && fr.i(0x1e4) <= NE) {
                            ecctmp.put(I * 5, fr.i(0x1a8));
                            ecctmp.put(I * 5 + 1, fr.i(0xa0));
                            ecctmp.put(I * 5 + 2, fr.i(0x1a4));
                            ecctmp.put(I * 5 + 3, fr.i(0x1e8));
                            ecctmp.put(I * 5 + 4, fr.i(kDX));
                        }
                    }
                } else if (cX > 0) {
                    S = 1;  // e.g. a face count larger than the element count
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : ECCTMP could not be read\n");

        // ---- MATAXI (140380882): the error test uses the stale C ---------------------------------
        if (S == 0 && std::strstr(line, "MATAXI") != nullptr) {
            fr.i(0xe4) = sc(line, "%s %d %d %d", tok, fr.a(kObj), fr.a(kN), fr.a(0x1a8));
            if (std::strcmp(tok, "MATAXI") == 0 && NE > 0) {
                if (valid(N) && N <= NE && !mataxi) {
                    mataxi.alloc(NE * 9);
                    matIdx.alloc(N);
                    fr.i(kCM) = N;
                    for (I = 0; I < N; ++I) {
                        void* t[10];
                        t[0] = fr.a(0x1e4);
                        for (int k = 0; k < 9; ++k) t[1 + k] = fr.a(0xb8 + 4 * k);
                        const int r = crt_fscanf(in, "%d %f %f %f %f %f %f  %f %f %f\n", t);
                        const std::int32_t e = fr.i(0x1e4);
                        if (r != -1 && e > 0 && e <= NE) {
                            matIdx.put(I, e);
                            for (K = 0; K < 9; ++K)
                                mataxi.put(static_cast<std::int64_t>((e - 1) * 9 + K), static_cast<double>(fr.f(0xb8 + 4 * K)));
                        }
                    }
                } else if (C > 0) {
                    S = 1;
                }
            }
        }
        if (S == 1) return fail_leaving_open("\tERROR : MATAXI could not be read\n");
    }

    // ---- end of file: MESH.DAT (140380b79) ----------------------------------------------------------
    std::fclose(in);
    const std::int32_t obj = fr.i(kObj);
    *o_obj = obj;
    *o_nodes = NN;
    *o_elems = NE;
    std::FILE* out = fsopen(out_name, "wt");  // str 1405562fc; not NULL-tested
    if (out == nullptr) def::rt::abnormal_exit(def::rt::kStatusInvalidCrtParameter);  // crt::fprintf(NULL, ...)

    crt::fprintf(out, "RZ        %3d %6d\n", obj, NN);
    for (I = 0; I < NN; ++I)
        crt::fprintf(out, "%6d %20.10E %20.10E %20.10E\n", I + 1, rz.get(I * 3), rz.get(I * 3 + 1), rz.get(I * 3 + 2));
    crt::fprintf(out, "ELMCON    %6d %6d %6d\n", obj, NE, fr.i(kNpe));
    for (I = 0; I < NE; ++I)
        crt::fprintf(out, "%6d %6d %6d %6d %6d %6d %6d %6d %6d\n", I + 1, el.get(I * 8), el.get(I * 8 + 1), el.get(I * 8 + 2),
                el.get(I * 8 + 3), el.get(I * 8 + 4), el.get(I * 8 + 5), el.get(I * 8 + 6), el.get(I * 8 + 7));
    if (ndtmp) {
        crt::fprintf(out, "NDTMP     %3d %6d\n", obj, NN);
        for (I = 0; I < NN; ++I) crt::fprintf(out, "%6d %20.10E \n", I + 1, ndtmp.get(I));
    } else {
        crt::fprintf(out, "NDTMP     %3d %6d\n", obj, NN);
        for (I = 0; I < NN; ++I) crt::fprintf(out, "%6d %20.10E \n", I + 1, static_cast<double>(fr.f(0x208)));
    }
    if (strE) {
        if (strN) {
            crt::fprintf(out, "STRAIN    %3d %6d %10.5f %6d %10.5f\n", obj, NE, static_cast<double>(fr.f(0x6f4)), NN,
                    static_cast<double>(fr.f(0x19c)));
        } else {
            I = 0;
            fr.f(0x19c) = 0.0f;
            crt::fprintf(out, "STRAIN    %3d %6d %10.5f %6d %10.5f\n", obj, NE, static_cast<double>(fr.f(0x6f4)), I,
                    static_cast<double>(fr.f(0x19c)));
        }
        for (I = 0; I < NE; ++I) crt::fprintf(out, "%6d %20.10E \n", I + 1, strE.get(I));
        if (strN)
            for (I = 0; I < NN; ++I) crt::fprintf(out, "%6d %20.10E \n", I + 1, strN.get(I));
    } else {
        I = 0;
        crt::fprintf(out, "STRAIN    %3d %6d %10.5f %6d %10.5f\n", obj, I, static_cast<double>(fr.f(0x6f4)), I,
                static_cast<double>(fr.f(0x19c)));
    }
    if (stn) {
        crt::fprintf(out, "STNCMP    %3d %6d   6   0.0000\n", obj, fr.i(kC6));
        for (I = 0; I < fr.i(kC6); ++I) {
            const std::int32_t e = stnIdx.get(I);
            const std::int64_t b = static_cast<std::int64_t>((e - 1) * 6);
            crt::fprintf(out, "%6d %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n", e, stn.get(b), stn.get(b + 1),
                    stn.get(b + 2), stn.get(b + 3), stn.get(b + 4), stn.get(b + 5));
        }
    } else {
        crt::fprintf(out, "STNCMP    %3d       0     6      0.0000\n", obj);
    }
    if (damE) {
        if (damN) {
            crt::fprintf(out, "DAMAGE    %3d %6d %10.5f %6d %10.5f\n", obj, NE, static_cast<double>(fr.f(0x230)), NN,
                    static_cast<double>(fr.f(0x254)));
        } else {
            I = 0;
            fr.f(0x254) = 0.0f;
            crt::fprintf(out, "DAMAGE    %3d %6d %10.5f %6d %10.5f\n", obj, NE, static_cast<double>(fr.f(0x230)), I,
                    static_cast<double>(fr.f(0x254)));
        }
        for (I = 0; I < NE; ++I) crt::fprintf(out, "%6d %20.10E \n", I + 1, damE.get(I));
        if (damN)
            for (I = 0; I < NN; ++I) crt::fprintf(out, "%6d %20.10E \n", I + 1, damN.get(I));
    } else {
        I = 0;
        crt::fprintf(out, "DAMAGE    %3d %6d %10.5f %6d %10.5f\n", obj, I, static_cast<double>(fr.f(0x230)), I,
                static_cast<double>(fr.f(0x254)));
    }
    if (grain) {
        crt::fprintf(out, "GRAIN    %3d %6d  16\n", obj, NE);
        for (I = 0; I < NE; ++I) {
            const std::int64_t b = static_cast<std::int64_t>(I) * 16;
            crt::fprintf(out, "%6d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f \n", I + 1, grain.get(b), grain.get(b + 1),
                    grain.get(b + 2), grain.get(b + 3), grain.get(b + 4), grain.get(b + 5), grain.get(b + 6),
                    grain.get(b + 7), grain.get(b + 8), grain.get(b + 9), grain.get(b + 10), grain.get(b + 11),
                    grain.get(b + 12), grain.get(b + 13), grain.get(b + 14), grain.get(b + 15));
        }
    } else {
        crt::fprintf(out, "GRAIN    %3d %6d  0\n", obj, NE);
    }
    if (usrE && fr.i(kHasUE) != 0) {
        crt::fprintf(out, "USRELM   %3d %6d %f %3d\n", obj, fr.i(kCU), static_cast<double>(fr.f(0x67c)), fr.i(kNvE));
        for (K = 0; K < fr.i(kNvE); ++K)
            for (I = 0; I < fr.i(kCU); ++I)
                crt::fprintf(out, "%6d  %20.10E\n", I + 1, usrE.get(static_cast<std::int64_t>(fr.i(kNvE) * I + K)));
    } else {
        crt::fprintf(out, "USRELM   %3d %6d  0.0   0\n", obj, NE);
    }
    if (usrN && fr.i(kHasUN) != 0) {
        crt::fprintf(out, "USRNOD   %3d %6d %f %3d\n", obj, NN, static_cast<double>(fr.f(0x678)), fr.i(kNvN));
        for (K = 0; K < fr.i(kNvN); ++K)
            for (I = 0; I < NN; ++I)
                crt::fprintf(out, "%3d  %20.10E\n", I + 1, usrN.get(static_cast<std::int64_t>(fr.i(kNvN) * I + K)));
    } else {
        crt::fprintf(out, "USRNOD   %3d %6d  0.0   0\n", obj, NN);
    }
    if (fr.i(kCX) == 0 && fr.i(kCT) > 0) {
        crt::fprintf(out, "BCCTMP    %3d %6d\n", obj, fr.i(kCT));
        if (bcctmp)
            for (I = 0; I < fr.i(kCT); ++I) crt::fprintf(out, "%6d %6d \n", bcctmp.get(I * 2), bcctmp.get(I * 2 + 1));
    }
    if (fr.i(kSeenX) != 0) {
        if (fr.i(kCX) > 0) {
            crt::fprintf(out, "ECCTMP   %3d %6d  0\n", obj, fr.i(kCX));
            if (ecctmp)
                for (I = 0; I < fr.i(kCX); ++I)
                    crt::fprintf(out, "%6d %6d %6d %6d %6d %3d \n", I + 1, ecctmp.get(I * 5), ecctmp.get(I * 5 + 1),
                            ecctmp.get(I * 5 + 2), ecctmp.get(I * 5 + 3), ecctmp.get(I * 5 + 4));
        } else {
            crt::fprintf(out, "ECCTMP    %3d  0  %3d\n", obj, fr.i(kDX));
        }
    }
    if (fr.i(kCD) != 0) {
        crt::fprintf(out, "BCCDEF    %3d %6d\n", obj, fr.i(kCD));
        if (bccdef)
            for (I = 0; I < fr.i(kCD); ++I)
                crt::fprintf(out, "%6d %6d %6d %6d\n", bccdef.get(I * 4), bccdef.get(I * 4 + 1), bccdef.get(I * 4 + 2),
                        bccdef.get(I * 4 + 3));
    } else {
        fr.i(0x714) = 0;
        fr.i(0x710) = 1;
        crt::fprintf(out, "BCCDEF    %3d %6d\n", obj, fr.i(0x710));
        for (I = 0; I < fr.i(0x710); ++I)
            crt::fprintf(out, "%3d %6d %6d %6d\n", fr.i(0x710), fr.i(0x714), fr.i(0x714), fr.i(0x714));
    }
    crt::fprintf(out, "MESHNO    %3d\n", *mesh_no);
    if (mataxi) {
        crt::fprintf(out, "MATAXI    %3d %6d   9   \n", obj, fr.i(kCM));
        for (I = 0; I < fr.i(kCM); ++I) {
            const std::int32_t e = matIdx.get(I);
            const std::int64_t b = static_cast<std::int64_t>((e - 1) * 9);
            crt::fprintf(out, "%6d %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E %20.10E\n", e,
                    mataxi.get(b), mataxi.get(b + 1), mataxi.get(b + 2), mataxi.get(b + 3), mataxi.get(b + 4),
                    mataxi.get(b + 5), mataxi.get(b + 6), mataxi.get(b + 7), mataxi.get(b + 8));
        }
    } else {
        crt::fprintf(out, "MATAXI    %3d       0     9      \n", obj);
    }
    std::fclose(out);
    for (auto* a : {&rz, &ndtmp, &strE, &strN, &stn, &damE, &damN, &grain, &usrE, &usrN, &mataxi}) a->release();
    for (auto* a : {&el, &stnIdx, &bcctmp, &ecctmp, &bccdef, &matIdx}) a->release();
    return 0;
}

} // namespace gensub

// part of @src 140378e00: dispatcher case 0x14 GENSUB (listing 14037dcc5..14037dee7)
int cmd_gensub(model::DB_STEP_DATA& m, std::FILE* f)
{
    using namespace gensub;
    std::int32_t one = 1;  // [RSP+0x65e0]
    std::int32_t obj_no = 0;
    double value = 0.0;
    def::key::read_i_d(f, &obj_no, &value);  // 1404154c0; its result is not kept
    // 14037dced tests the dispatcher status [RSP+0x12d4], 0 at this point: never "Reading Error"
    if (obj_no == 0) {
        delete_if_exists("DEF_SUB.INI");
        return 0;
    }
    std::FILE* ini = ::_fsopen("DEF_SUB.INI", "wt", _SH_DENYNO);  // 14047b934, str 1405546e4
    if (ini == nullptr) return 0;
    model::LLOBJ_DATA* obj = def::key::g2obj::find_object(m, obj_no);  // 1401d6ea0 -> 14017aaf0
    if (obj == nullptr) {
        leaked_files().push_back(ini);  // 14037dd6d: returns 2, DEF_SUB.INI stays open and empty
        return 2;
    }
    std::int32_t mesh_no = 1;  // [RSP+0x6608]
    std::FILE* k = ::_fsopen("AX_BIL.KEY", "wt", _SH_DENYNO);
    if (k == nullptr) def::rt::abnormal_exit(def::rt::kStatusInvalidCrtParameter);  // the writer on NULL
    def::key::g2obj::write_object(m, k, obj, 0, false);  // 14029e250
    std::fclose(k);
    std::int32_t o_obj = 0, o_nodes = 0, o_elems = 0;
    convert_key_to_mesh(m, "AX_BIL.KEY", "MESH.DAT", &o_obj, &o_nodes, &o_elems, &mesh_no);
    delete_if_exists("AX_BIL.KEY");  // fails while the converter left the file open
    const std::int32_t* c_first = connectivity(*obj);
    const std::int32_t* c_second = connectivity(*obj);
    if (c_second == nullptr) def::rt::abnormal_exit(def::rt::kStatusAccessViolation);  // 14037de50
    const std::int32_t off = c_first[4] - c_second[0];
    const unsigned char* ob = reinterpret_cast<const unsigned char*>(obj);
    std::int32_t nodes, elems;
    std::memcpy(&nodes, ob + 0x880, 4);
    std::memcpy(&elems, ob + 0x884, 4);
    def::rt::crt::fprintf(ini, "%6d %6d %6d\n", obj_no, nodes, elems);
    def::rt::crt::fprintf(ini, "%6d %6d %20.10E\n", one, off, value);
    std::fclose(ini);
    delete_if_exists("NEWBRICK.KEY");
    return 0;
}

} // namespace def::cmd
