// GENDB inter-object record writer FUN_1401f7730 of packet p4-06 (defdbio/gendb/p4_06_interobj.hpp).
//
// The node is read at the byte offsets of the decompile (param_1 is an int*: int index k is byte
// 4k). Fields: 0x0 friction type, set 1 (flag byte 0x4, ref 0x8, code 0x10, function 0x18 with the
// constant value at 0x20, dims 0x34..0x3c, pairs 0x108..0x130), set 2 (byte 0x5, 0xc, 0x14,
// function 0x90 / value 0x98, dims 0xac..0xb4, pairs 0x138..0x160), 0x170 contact flag, 0x174 3D
// option, 0x178 / 0x180 SEPRES, 0x188 window list, 0x1b8 IHTCOF function (+ 0x230 / 0x234 ref),
// 0x238 SEPDEN, 0x240 FRCROT function, 0x2b8 INTRST function, 0x330..0x338 CNTMTH, 0x340..0x378
// WMODEL, 0x37c..0x388 RGDCNT, 0x398 model.
#include <defdbio/gendb/p4_06_interobj.hpp>

#include <cstring>

#include <defdbio/gendb/ext_p4_06.hpp>
#include <defmodel/step/db_step.hpp>
#include <defrt/stdlib/refcount.hpp>

namespace def::dbio::gendb {

using model::Cls_1401f46e0;
using model::DB_STEP_DATA;
using model::DYNARY;
using model::field;

namespace {

// part of @src 1401f7730: FNCLOC a function slot for the DYNARY at io+off, its number into *fid,
// then the function's words into LLFNC from the slot's start
void write_function(Cls_1401f46e0* io, std::size_t off, std::int32_t* iifnc, double* llfnc, double* fid)
{
    DYNARY* f = &field<DYNARY>(io, off);
    std::int32_t start = 0;
    std::int32_t len = ext::f140130290(f);
    std::int32_t attr = 0;
    ext::f1401bb3d0(io->owner, &len, iifnc, &attr, fid, &start);
    ext::f140131360(f, llfnc + (start - 1));
}

// part of @src 1401f7730: one friction coefficient set (set 0: FRCFAC, set 1: the second set of
// friction type 4); value / function / code at rec[w], rec[w + 1], rec[w + 2], extras at rec[wx..]
void write_friction_set(Cls_1401f46e0* io, int set, double* rec, std::size_t w, std::size_t wx,
                        std::int32_t* iifnc, double* llfnc)
{
    const std::size_t neg = set == 0 ? 0x4 : 0x5;
    const std::size_t ref = set == 0 ? 0x8 : 0xc;
    const std::size_t code = set == 0 ? 0x10 : 0x14;
    const std::size_t fn = set == 0 ? 0x18 : 0x90;      // DYNARY; its constant value is at fn + 8
    const std::size_t dims = set == 0 ? 0x34 : 0xac;
    const std::size_t pairs = set == 0 ? 0x108 : 0x138;
    if (field<std::uint8_t>(io, neg) == 0) {
        rec[wx] = 0.0;
        rec[wx + 1] = 0.0;
        rec[wx + 2] = 0.0;
        rec[w + 2] = static_cast<double>(field<std::int32_t>(io, code));
        const std::int32_t c = field<std::int32_t>(io, code);
        if (c == 0) {
            rec[w] = field<double>(io, fn + 8);
        } else if (c == 5) {
            rec[wx] = static_cast<double>(field<std::int32_t>(io, dims));
            rec[wx + 1] = static_cast<double>(field<std::int32_t>(io, dims + 4));
            rec[wx + 2] = static_cast<double>(field<std::int32_t>(io, dims + 8));
        } else if (c == 6) {
            rec[wx] = field<double>(io, pairs);
            rec[wx + 1] = field<double>(io, pairs + 8);
        } else if (c == 7) {
            rec[wx] = field<double>(io, pairs + 0x10);
            rec[wx + 1] = field<double>(io, pairs + 0x18);
        } else if (c == 8) {
            rec[wx] = field<double>(io, pairs + 0x20);
            rec[wx + 1] = field<double>(io, pairs + 0x28);
        }
        if (field<std::int32_t>(io, fn + 4) != 0) {      // DYNARY ndim
            write_function(io, fn, iifnc, llfnc, &rec[w + 1]);
        }
    } else {
        rec[w + 2] = static_cast<double>(-field<std::int32_t>(io, ref));
        rec[w] = field<double>(io, fn + 8);
    }
}

// part of @src 1401f7730: friction type word and both friction sets (2D and 3D)
void write_friction(Cls_1401f46e0* io, double* rec, std::size_t wx1, std::size_t w2, std::size_t wx2,
                    std::int32_t* iifnc, double* llfnc)
{
    const std::int32_t t = field<std::int32_t>(io, 0x0);
    if (t == 1) {
        rec[2] = 1.0;
    } else if (t == 2) {
        rec[2] = 2.0;
    } else if (t == 3) {
        rec[2] = 3.0;   // DAT_14051f1c8
    } else if (t == 4) {
        rec[2] = 4.0;   // DAT_14051f248
    }
    write_friction_set(io, 0, rec, 3, wx1, iifnc, llfnc);
    if (field<std::int32_t>(io, 0x0) == 4) {
        write_friction_set(io, 1, rec, w2, wx2, iifnc, llfnc);
    }
}

// part of @src 1401f7730: SEPRES code word (0 / 1 / 2; other codes leave the word)
void write_sepres_code(const Cls_1401f46e0* io, double* rec, std::size_t w)
{
    const std::int32_t c = io->u178;
    if (c == 0) {
        rec[w] = 0.0;
    } else if (c == 1) {
        rec[w] = 1.0;
    } else if (c == 2) {
        rec[w] = 2.0;
    }
}

// part of @src 1401f7730: IHTCOF r[6..8] (same words in 2D and 3D)
void write_ihtcof(Cls_1401f46e0* io, double* rec, std::int32_t* iifnc, double* llfnc)
{
    if (io->u1b8.ndim == 0) {
        rec[7] = 0.0;
        rec[6] = io->u1b8.value;
    } else if (io->u1b8.ndim == 1) {
        const std::int32_t k = io->u1b8.var[0];
        if (k == 1) {
            rec[8] = 1.0;
        } else if (k == 3) {
            rec[8] = 3.0;
        } else if (k == 4) {
            rec[8] = 2.0;
        }
        write_function(io, 0x1b8, iifnc, llfnc, &rec[7]);
    }
    if (io->u234 != 0) {
        rec[8] = static_cast<double>(-io->u230);
    }
}

// part of @src 1401f7730: INTRST r[0x13], r[0x14] (same words in 2D and 3D)
void write_intrst(Cls_1401f46e0* io, double* rec, std::int32_t* iifnc, double* llfnc)
{
    if (io->u2b8.ndim == 0) {
        rec[0x13] = 0.0;
        rec[0x14] = io->u2b8.value;
    } else if (io->u2b8.ndim == 1) {
        const std::int32_t k = io->u2b8.var[0];
        if (k == 3) {
            rec[0x13] = 1.0;
        } else if (k == 4) {
            rec[0x13] = 2.0;
        }
        write_function(io, 0x2b8, iifnc, llfnc, &rec[0x14]);
    }
}

// part of @src 1401f7730: WMODEL code r[w], constants as a pseudo-function (number in r[w + 1])
void write_wmodel(Cls_1401f46e0* io, double* rec, std::size_t w, std::int32_t* iifnc, double* llfnc)
{
    if (io->u340 == 0) {
        rec[w] = 0.0;
    } else if (io->u340 == 3) {
        rec[w] = static_cast<double>(-io->u378);
    } else {
        std::int32_t len = 0;
        double buf[4] = {0.0, 0.0, 0.0, 0.0};   // rsp+0x80 (2D) / rsp+0xe0 (3D)
        const std::int32_t mode = io->u340;
        if (mode == 1) {
            rec[w] = 1.0;
            len = 4;
            buf[0] = field<double>(io, 0x348);
            buf[1] = field<double>(io, 0x350);
            buf[2] = field<double>(io, 0x358);
            buf[3] = field<double>(io, 0x360);
        } else if (mode == 2) {
            rec[w] = 2.0;
            len = 2;
            buf[0] = field<double>(io, 0x368);
            buf[1] = field<double>(io, 0x370);
        }
        std::int32_t start = 0;
        std::int32_t attr = 0;
        ext::f1401bb3d0(io->owner, &len, iifnc, &attr, &rec[w + 1], &start);
        std::memcpy(llfnc + (start - 1), buf, static_cast<std::size_t>(static_cast<std::int64_t>(len) << 3));
    }
}

// part of @src 1401f7730: the node's windows (list +0x188) into LLWIN from the window cursor;
// first / last record into rec[wf], rec[wf + 1]; 2D also rec[0xf] = 1.0 when any was written
void write_windows(Cls_1401f46e0* io, double* rec, std::size_t wf, bool dim2, std::int32_t objid, std::int32_t* iifnc,
                   double* llfnc, double* llwin, double* llwrz, const std::int32_t* w_llwin)
{
    const std::int32_t n = ext::f1401f98d0(io);
    if (n == 0) {
        rec[wf] = 0.0;
        rec[wf + 1] = 0.0;
        if (dim2) {
            rec[0xf] = 0.0;
        }
        return;
    }
    std::int32_t win = ext::f14019a5a0(io->owner);
    std::int32_t wrz = ext::f14019a5e0(io->owner);
    const std::int32_t first = win;
    std::int32_t written = 0;
    for (std::int32_t k = 0; k < n; k = k + 1) {
        void* v = ext::f140269900(&io->u188, k + 1);
        if (v == nullptr) {
            continue;
        }
        std::int32_t p9 = 0;
        ext::f140260ea0(v, llwin + *w_llwin * (first - 1 + written), llwrz, iifnc, llfnc, &win, &wrz, io->owner, &p9,
                        objid);
        written = written + 1;
    }
    ext::f14019a5c0(io->owner, &win);
    ext::f14019a600(io->owner, &wrz);
    const std::int32_t last = win;
    if (last - first < 1) {
        rec[wf] = 0.0;
        rec[wf + 1] = 0.0;
        if (dim2) {
            rec[0xf] = 0.0;
        }
    } else {
        rec[wf] = static_cast<double>(first);
        rec[wf + 1] = static_cast<double>(last - 1);
        if (dim2) {
            rec[0xf] = 1.0;
        }
    }
}

// part of @src 1401f7730: RGDCNT 3 ints and a real from rec[w]
void write_rgdcnt(const Cls_1401f46e0* io, double* rec, std::size_t w)
{
    rec[w] = static_cast<double>(io->u37c);
    rec[w + 1] = static_cast<double>(io->u380);
    rec[w + 2] = static_cast<double>(io->u384);
    rec[w + 3] = field<double>(io, 0x388);
}

} // namespace

// @src 1401f7730  inter-object record writer
void write_interobj_record(Cls_1401f46e0* io, const std::int32_t* master, const std::int32_t* slave, double* rec,
                           std::int32_t* iifnc, double* llfnc, double* llwin, double* llwrz,
                           const std::int32_t* w_llwin)
{
    if (io->u170 == 0) {
        *rec = 0.0;
        return;
    }
    if (io->u170 == 1) {
        *rec = 1.0;   // DAT_140509c90
    } else {
        *rec = 2.0;   // DAT_140509fb8
    }
    if (rt::dimension_is_2(io->owner)) {   // 1400015e0
        write_friction(io, rec, 0x18, 0x21, 0x24, iifnc, llfnc);
        // 1401f7f36: windows attach to the other object: the slave when the flag is 1
        const std::int32_t other = io->u170 == 1 ? *slave : *master;
        write_windows(io, rec, 0x16, true, other, iifnc, llfnc, llwin, llwrz, w_llwin);
        write_sepres_code(io, rec, 0x10);
        rec[0x11] = field<double>(io, 0x180);
        rec[0x12] = field<double>(io, 0x238);
        // FRCROT: function only for variable kind 1 sets the mode word
        if (io->u240.ndim == 0) {
            rec[0xd] = 0.0;
            rec[0xe] = io->u240.value;
        } else if (io->u240.ndim == 1) {
            if (io->u240.var[0] == 1) {
                rec[0xd] = 1.0;
            }
            write_function(io, 0x240, iifnc, llfnc, &rec[0xe]);
        }
        write_ihtcof(io, rec, iifnc, llfnc);
        write_intrst(io, rec, iifnc, llfnc);
        write_rgdcnt(io, rec, 0x1b);
        write_wmodel(io, rec, 0x1f, iifnc, llfnc);
    }
    if (rt::dimension_is_3(io->owner)) {   // 1400015f0
        if (io->u174 == 0) {
            rec[1] = 1.0;
        } else {
            rec[1] = 0.0;
        }
        write_sepres_code(io, rec, 0xc);
        rec[0xd] = field<double>(io, 0x180);
        write_friction(io, rec, 0xe, 0x2a, 0x2d, iifnc, llfnc);
        write_ihtcof(io, rec, iifnc, llfnc);
        write_intrst(io, rec, iifnc, llfnc);
        if (io->u330 == 0) {
            rec[0x1f] = 0.0;
        } else {
            rec[0x1f] = 1.0;
            rec[0x20] = static_cast<double>(io->u334);
            rec[0x21] = field<double>(io, 0x338);
        }
        write_wmodel(io, rec, 0x22, iifnc, llfnc);
        write_windows(io, rec, 0x24, false, 0, iifnc, llfnc, llwin, llwrz, w_llwin);
        write_rgdcnt(io, rec, 0x26);
    }
}

} // namespace def::dbio::gendb
