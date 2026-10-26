// GENDB per-object counts (packet p4-02a): the walk from 1401beb30 over the object list, the
// property tables and window definitions of each object. Header: defdbio/gendb/obj_counts.hpp.
#include <defdbio/gendb/obj_counts.hpp>

#include <cstdio>

#include <defmodel/layout/cls_1401364a0.hpp>
#include <defmodel/layout/cls_14017a6c0.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/obj_geo3d.hpp>
#include <defmodel/linked/record_e8_list.hpp>
#include <defmodel/llobj/dynary_table.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defrt/stdlib/refcount.hpp>

#include "raw.hpp"

namespace def::dbio::gendb {

using model::DB_STEP_DATA;
using model::LLOBJ_DATA;
using raw::at;
using raw::ptr;

namespace {

ObjCountExt& ext() { return obj_count_ext(); }

// LLOBJ_DATA+15000 (0x3a98): the owning step.
DB_STEP_DATA* owner_of(LLOBJ_DATA* obj) { return obj->owner; }

// The count at +0x14 of Cls_140269250 (14019a590).
std::int32_t window_list_count(void* list) { return at<std::int32_t>(list, 0x14); }

// Adds `words` and one function when the table at `off` of `self` is one-dimensional
// (its ndim at off + 4 is 1), the pattern of 14018cec0, 1401869c0 and 14025d3b0.
bool add_1d(void* self, std::size_t off, std::int32_t* nfn, std::int32_t* lfnc)
{
    if (at<std::int32_t>(self, off + 4) != 1)
        return false;
    *nfn = *nfn + 1;
    *lfnc = ext().dynary_words_1d(static_cast<const DYNARY*>(ptr(self, off))) + *lfnc;
    return true;
}

} // namespace

// @src 1401beb30  objects of step (list at +0x1380): 14017ad20(step + 0x1380, ...)
void count_object_tables(DB_STEP_DATA* step, std::int32_t* nfn, std::int32_t* lfnc,
                         std::int32_t* nwin, std::int32_t* lwin, std::int32_t* err)
{
    count_list_objects(&step->objects, nfn, lfnc, nwin, lwin, err);
}

// @src 14017ad20  every object whose type (+0xc) is not 7 (an empty placeholder): 140167b50.
// The sixth argument is passed on but 140167b50 never reads it.
void count_list_objects(void* list, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                        std::int32_t* lwin, std::int32_t* err)
{
    (void)err;
    for (LLOBJ_DATA* o = static_cast<model::Cls_14017a6c0*>(list)->head; o != nullptr; o = o->next) {
        if (at<std::int32_t>(o, 0xc) != 7)
            count_object(o, nfn, lfnc, nwin, lwin);
    }
}

// @src 140167b50  one object: 2D geometry clean-up, property tables 0x2160 and 0x1ea0, 3D surface
// sets, then (for non-rigid objects (+0xc != 0), or rigid ones with mesh options 0x880 > 3 and
// 0x884 > 0 when not only deformation is on) the windows of 0x2d10 / 0x2e68 and the 0x3590 table.
void count_object(LLOBJ_DATA* obj, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                  std::int32_t* lwin)
{
    if (def::rt::dimension_is_2(owner_of(obj)))
        ext().obj_drop_bad_geo2d(obj, 1);  // 140304e40(obj, 1)
    const std::int32_t t21c8 = at<std::int32_t>(obj, 0x21c8);
    if (t21c8 != 7 && t21c8 != 8 && t21c8 != 9 && t21c8 != 10 && at<std::uint8_t>(obj, 0x2a18) != 0)
        at<std::uint8_t>(obj, 0x2a18) = 0;
    count_props_2160(ptr(obj, 0x2160), nfn, lfnc, owner_of(obj), obj);
    count_props_1ea0(ptr(obj, 0x1ea0), nfn, lfnc);
    if (def::rt::dimension_is_3(owner_of(obj)) && obj->u3c00[0] != nullptr)
        count_surface_sets(obj->u3c00[0], nfn, lfnc);
    if (at<std::int32_t>(obj, 0xc) != 0
        || (only_deformation_mode_on(obj) == 0 && 3 < at<std::int32_t>(obj, 0x880)
            && 0 < at<std::int32_t>(obj, 0x884))) {
        count_windows_2d10(ptr(obj, 0x2d10), nwin, lwin, owner_of(obj));
        count_props_2e68(ptr(obj, 0x2e68), nfn, lfnc, nwin, lwin, owner_of(obj));
        if (0 < at<std::int32_t>(obj, 0x3590)) {
            *nfn = *nfn + 1;
            *lfnc = step_u078(owner_of(obj)) + *lfnc;
        }
    }
}

// @src 140167ae0  1401f3d90 on the owner's simulation control (+0x750); without an owner the
// error line and 0
std::uint8_t only_deformation_mode_on(LLOBJ_DATA* obj)
{
    std::uint8_t r = 0;
    if (owner_of(obj) == nullptr) {
        // FUN_14047903c() + 0x30: the CRT's stdout (second FILE of _iob)
        def::rt::crt::fprintf(stdout, "Error in LLOBJ_DATA::onlyDeformationModeOn()\n");
    } else {
        r = simctrl_only_deformation(&owner_of(obj)->sim_ctrl) ? 1 : 0;
    }
    return r;
}

// @src 1401f3c50  int at +0x10c
std::int32_t simctrl_u10c(const void* sc) { return at<std::int32_t>(sc, 0x10c); }
// @src 1401f3c70  int at +0x110
std::int32_t simctrl_u110(const void* sc) { return at<std::int32_t>(sc, 0x110); }
// @src 1401f3cb0  int at +0x118
std::int32_t simctrl_u118(const void* sc) { return at<std::int32_t>(sc, 0x118); }

// @src 1401f3d90  mode flags: +0x104 == 0 (1401f3c10), +0x108 == 1 (1401f3c30), +0x10c, +0x110
// and +0x118 == 0
bool simctrl_only_deformation(model::Cls_1401e7b10* sc)
{
    if (at<std::int32_t>(sc, 0x104) == 0 && at<std::int32_t>(sc, 0x108) == 1 && simctrl_u10c(sc) == 0
        && simctrl_u110(sc) == 0 && simctrl_u118(sc) == 0)
        return true;
    return false;
}

// @src 14018cec0  the property tables of Cls_14018b220. In 2D each one counts once per 2D
// geometry of the object (140198310, at least 1). The switch on +0x68 picks one of nine tables
// (case 4 falls through into case 5, as in the binary); the table at +0x8c0 counts when the
// byte at +0x8b8 is set; +0x628 / +0x758 (3D only, counted once) / +0x960 (2D only) pick one of
// two tables each.
void count_props_2160(void* self, std::int32_t* nfn, std::int32_t* lfnc, DB_STEP_DATA* owner,
                      LLOBJ_DATA* obj)
{
    std::int32_t words = 0;
    std::int32_t mult = 1;
    if (def::rt::dimension_is_2(owner)) {
        std::int32_t m;
        if (ext().obj_geo2d_count(obj) < 1)
            m = 1;
        else
            m = ext().obj_geo2d_count(obj);
        mult = m;
    }
    const auto w1 = [&](std::size_t off) {
        return ext().dynary_words_1d(static_cast<const DYNARY*>(ptr(self, off)));
    };
    bool on = false;
    switch (at<std::int32_t>(self, 0x68)) {
    case 0:
        on = at<std::int32_t>(self, 0x74) == 1;
        if (on) words = w1(0x70);
        break;
    case 1:
        on = at<std::int32_t>(self, 0xec) == 1;
        if (on) words = w1(0xe8);
        break;
    case 2:
        on = at<std::int32_t>(self, 0x20c) == 1;
        if (on) words = w1(0x208);
        break;
    case 3:
        on = at<std::int32_t>(self, 0x2a4) == 1;
        if (on) words = w1(0x2a0);
        break;
    case 4:
        if (at<std::uint8_t>(self, 0x330) != 0 && at<std::int32_t>(self, 0x33c) == 1) {
            on = true;
            words = w1(0x338);
        }
        [[fallthrough]];  // 14018d043: no jump to the end
    case 5:
        if (at<std::int32_t>(self, 0x3b4) == 1) {
            on = true;
            words = w1(0x3b0);
        }
        break;
    case 6:
        on = at<std::int32_t>(self, 0x510) == 0;
        if (on) words = props_2160_table6_words(self, owner);
        break;
    case 7:
        on = at<std::int32_t>(self, 0x51c) == 1;
        if (on) words = w1(0x518);
        break;
    case 8:
        if (at<std::int32_t>(self, 0x594) == 1) {
            on = true;
            words = w1(0x590);
        }
        break;
    default:
        break;
    }
    if (on) {
        *nfn = *nfn + mult;
        *lfnc = words * mult + *lfnc;
    }
    if (at<std::uint8_t>(self, 0x8b8) != 0) {
        *nfn = *nfn + mult;
        *lfnc = w1(0x8c0) * mult + *lfnc;
    }
    on = false;
    if (at<std::int32_t>(self, 0x628) == 0) {
        on = at<std::int32_t>(self, 0x664) == 1;
        if (on) words = w1(0x660);
    } else if (at<std::int32_t>(self, 0x628) == 1 && at<std::int32_t>(self, 0x6dc) == 1) {
        on = true;
        words = w1(0x6d8);
    }
    if (on) {
        *nfn = *nfn + mult;
        *lfnc = words * mult + *lfnc;
    }
    if (def::rt::dimension_is_3(owner)) {
        on = false;
        if (at<std::int32_t>(self, 0x758) == 0) {
            on = at<std::int32_t>(self, 0x794) == 1;
            if (on) words = w1(0x790);
        } else if (at<std::int32_t>(self, 0x758) == 1 && at<std::int32_t>(self, 0x80c) == 1) {
            on = true;
            words = w1(0x808);
        }
        if (on) {
            *nfn = *nfn + 1;
            *lfnc = *lfnc + words;
        }
    }
    if (def::rt::dimension_is_2(owner)) {
        on = false;
        if (at<std::int32_t>(self, 0x960) == 0) {
            on = at<std::int32_t>(self, 0x96c) == 1;
            if (on) words = w1(0x968);
        } else if (at<std::int32_t>(self, 0x960) == 1 && at<std::int32_t>(self, 0x9e4) == 1) {
            on = true;
            words = w1(0x9e0);
        }
        if (on) {
            *nfn = *nfn + mult;
            *lfnc = words * mult + *lfnc;
        }
    }
}

// @src 14018cde0  words of the table at +0x448: 2D its padded 1D size (140130230); 3D a copy of
// it, rotated by the matrix at +0x4c8 when it is a velocity table (140135bc0), padded size + 10
std::int32_t props_2160_table6_words(void* self, DB_STEP_DATA* owner)
{
    std::int32_t r;
    if (!def::rt::dimension_is_3(owner)) {
        r = dynary_words_1d_padded(static_cast<const DYNARY*>(ptr(self, 0x448)));
    } else {
        DYNARY copy;  // 14012d050
        ext().dynary_copy(&copy, static_cast<const DYNARY*>(ptr(self, 0x448)));
        double m[9];
        ext().cls_14018b220_matrix(self, m);
        dynary_rotate_velocity(&copy, m);
        r = dynary_words_1d_padded(&copy);
        r = r + 10;
    }  // 14012f410
    return r;
}

// @src 1401869c0  Cls_1401860a0: the curves at +0xc0 and +0x140 when 1D; body forces (records
// of the list at +0x200, 12 words each: in 2D only kinds 5 and 6 (140186d00), and the list is
// emptied with a warning when other kinds are present); the 0x218 list (20 words per entry);
// the table at +0x238 when its +0x40 word is 8 (140130290 words)
void count_props_1ea0(void* self, std::int32_t* nfn, std::int32_t* lfnc)
{
    add_1d(self, 0xc0, nfn, lfnc);
    add_1d(self, 0x140, nfn, lfnc);
    auto* owner = at<DB_STEP_DATA*>(self, 0x2b8);
    auto* bf = static_cast<model::linked::Cls_140258580List*>(ptr(self, 0x200));
    if (!def::rt::dimension_is_3(owner)) {
        const std::int32_t n = bf->count;  // 14019a530 -> 14019a550: +0x10
        const std::int32_t n56 = count_body_force_records(self);
        if (n56 != n && -1 < n && -1 < n56) {
            bf->clear();  // 140259c70
            def::rt::crt::fprintf(stdout,
                                  "Warning: the number of body forces is wrong. Body foreces were deleted.\n");
        }
        if (0 < n56) {
            *nfn = *nfn + n56;
            *lfnc = n56 * 0xc + *lfnc;
        }
    } else {
        const std::int32_t n = bf->count;
        if (0 < n) {
            *nfn = *nfn + n;
            *lfnc = n * 0xc + *lfnc;
        }
    }
    const std::int32_t n2 = at<std::int32_t>(self, 0x218 + 0x18);  // 14019a560 -> 14019a580
    if (0 < n2) {
        *nfn = *nfn + n2;
        *lfnc = n2 * 0x14 + *lfnc;
    }
    if (at<std::int32_t>(self, 0x278) == 8) {
        *nfn = *nfn + 1;
        *lfnc = ext().dynary_words(static_cast<const DYNARY*>(ptr(self, 0x238))) + *lfnc;
    }
}

// @src 140186d00  records of the +0x200 list whose kind (+4) is 5 or 6
std::int32_t count_body_force_records(void* self)
{
    const std::int32_t n = static_cast<model::linked::Cls_140258580List*>(ptr(self, 0x200))->count;
    std::int32_t k = 0;
    if (0 < n) {
        for (std::int32_t i = 1; i <= n; ++i) {
            void* rec = ext().record_e8_nth(ptr(self, 0x200), i);
            if (at<std::int32_t>(rec, 4) == 5 || at<std::int32_t>(rec, 4) == 6)
                k = k + 1;
        }
    }
    return k;
}

// @src 140189580  count of the window list at +0x40 (14019a590)
std::int32_t windows_2d10_count(void* self) { return window_list_count(ptr(self, 0x40)); }

// @src 140189f00  windows of the +0x40 list. 2D: every one, its point count (+0x178) of words,
// turned counter-clockwise (140265bf0). 3D: those with +0x370 > 0 by kind (+0x210): 1, 2 -> 8
// words; 3 -> nodes of the geometry at +0x220 plus ceil(4 * elements / 3); 11..13 -> points
void count_windows_2d10(void* self, std::int32_t* nwin, std::int32_t* lwin, DB_STEP_DATA* owner)
{
    const std::int32_t n = windows_2d10_count(self);
    if (!def::rt::dimension_is_3(owner)) {
        for (std::int32_t i = 0; i < n; ++i) {
            void* w = ext().window_nth(ptr(self, 0x40), i + 1);
            if (w != nullptr) {
                *nwin = *nwin + 1;
                *lwin = *lwin + at<std::int32_t>(w, 0x178);
                const std::uint8_t yes = 1;
                orient_window(w, &yes);
            }
        }
    } else {
        for (std::int32_t i = 0; i < n; ++i) {
            void* w = ext().window_nth(ptr(self, 0x40), i + 1);
            // DAT_14051bed8 = 0.0
            if (w != nullptr && 0.0 < at<double>(w, 0x370)) {
                const std::int32_t kind = at<std::int32_t>(w, 0x210);
                if (kind == 1 || kind == 2) {
                    *nwin = *nwin + 1;
                    *lwin = *lwin + 8;
                } else if (kind == 3) {
                    *nwin = *nwin + 1;
                    auto* g = static_cast<const model::OBJ_GEO3D*>(ptr(w, 0x220));
                    *lwin = g->node_count() + *lwin;           // 1400034d0
                    const std::int32_t ne4 = g->elem_count() << 2;  // 140003500
                    std::int32_t q = ne4 / 3;
                    if (q * 3 < ne4)
                        q = q + 1;
                    *lwin = *lwin + q;
                } else if (kind == 0xb || kind == 0xc || kind == 0xd) {
                    *nwin = *nwin + 1;
                    *lwin = *lwin + at<std::int32_t>(w, 0x178);
                }
            }
        }
    }
}

// @src 140195ae0  count of the DYNARY list at 0 (14013c1b0)
std::int32_t c2e68_tables_count(void* self) { return static_cast<model::Cls_1401364a0*>(self)->size(); }
// @src 140195b00  int at +0x10 of the list at +0x18 (140158e70)
std::int32_t c2e68_list18_count(void* self) { return model::llobj::int_at_10(ptr(self, 0x18)); }
// @src 140195b30  window list +0x48 count (14019a590)
std::int32_t c2e68_win48_count(void* self) { return window_list_count(ptr(self, 0x48)); }
// @src 140195b60  window list +0x78 count
std::int32_t c2e68_win78_count(void* self) { return window_list_count(ptr(self, 0x78)); }
// @src 140195b90  window list +0xa8 count
std::int32_t c2e68_wina8_count(void* self) { return window_list_count(ptr(self, 0xa8)); }
// @src 140195bc0  window list +0xd8 count
std::int32_t c2e68_wind8_count(void* self) { return window_list_count(ptr(self, 0xd8)); }
// @src 140195bf0  window list +0x108 count
std::int32_t c2e68_win108_count(void* self) { return window_list_count(ptr(self, 0x108)); }
// @src 140195c20  window list +0x138 count
std::int32_t c2e68_win138_count(void* self) { return window_list_count(ptr(self, 0x138)); }
// @src 140195c50  window list +0x168 count
std::int32_t c2e68_win168_count(void* self) { return window_list_count(ptr(self, 0x168)); }

// @src 140195da0  Cls_1401956d0: window lists 0x78, 0xa8, 0xd8, 0x108, 0x138, the DYNARY list,
// the +0x18 list (3D only), window lists 0x48 and 0x168, in this order
void count_props_2e68(void* self, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                      std::int32_t* lwin, DB_STEP_DATA* owner)
{
    if (0 < c2e68_win78_count(self))
        count_window_list(ptr(self, 0x78), nfn, lfnc, nwin, lwin, owner);
    if (0 < c2e68_wina8_count(self))
        count_window_list(ptr(self, 0xa8), nfn, lfnc, nwin, lwin, owner);
    if (0 < c2e68_wind8_count(self))
        count_window_list(ptr(self, 0xd8), nfn, lfnc, nwin, lwin, owner);
    if (0 < c2e68_win108_count(self))
        count_window_list(ptr(self, 0x108), nfn, lfnc, nwin, lwin, owner);
    if (0 < c2e68_win138_count(self))
        count_window_list(ptr(self, 0x138), nfn, lfnc, nwin, lwin, owner);
    if (c2e68_tables_count(self) != 0)
        count_dynary_list(self, nfn, lfnc);
    // the owner read here is the one at +0x198 (1401956d0's copy of the object's owner)
    if (def::rt::dimension_is_3(at<DB_STEP_DATA*>(self, 0x198))) {
        if (c2e68_list18_count(self) != 0)
            count_list38(ptr(self, 0x18), nfn, lfnc);
    }
    if (c2e68_win48_count(self) != 0)
        count_window_list(ptr(self, 0x48), nfn, lfnc, nwin, lwin, owner);
    if (0 < c2e68_win168_count(self))
        count_window_list(ptr(self, 0x168), nfn, lfnc, nwin, lwin, owner);
}

// @src 1401365f0  each DYNARY of the list (140130410, link +0x60); then the function count is
// set to its value on entry plus the larger of the list's count and the nodes' ids (+0x54)
void count_dynary_list(void* list, std::int32_t* nfn, std::int32_t* lfnc)
{
    const std::int32_t n0 = *nfn;
    for (void* p = static_cast<model::Cls_1401364a0*>(list)->head; p != nullptr; p = at<void*>(p, 0x60))
        count_dynary(static_cast<DYNARY*>(p), nfn, lfnc);
    *nfn = n0 + dynary_list_max_id(list);
}

// @src 140136a10  max(count, every node's +0x54)
std::int32_t dynary_list_max_id(void* list)
{
    std::int32_t m = static_cast<model::Cls_1401364a0*>(list)->size();
    for (void* p = static_cast<model::Cls_1401364a0*>(list)->head; p != nullptr; p = at<void*>(p, 0x60)) {
        std::int32_t v;
        if (at<std::int32_t>(p, 0x54) < m)
            v = m;
        else
            v = at<std::int32_t>(p, 0x54);
        m = v;
    }
    return m;
}

// @src 140130410  one function and its words by ndim 1 / 2 / 3 (140130200 / 140130160 / 14012fff0)
void count_dynary(const DYNARY* t, std::int32_t* nfn, std::int32_t* lfnc)
{
    if (t->ndim == 1) {
        *nfn = *nfn + 1;
        *lfnc = ext().dynary_words_1d(t) + *lfnc;
    } else if (t->ndim == 2) {
        *nfn = *nfn + 1;
        *lfnc = ext().dynary_words_2d(t) + *lfnc;
    } else if (t->ndim == 3) {
        *nfn = *nfn + 1;
        *lfnc = ext().dynary_words_3d(t) + *lfnc;
    }
}

// @src 140257d10  each node of the list (link +0x38): 140257450
void count_list38(void* list, std::int32_t* nfn, std::int32_t* lfnc)
{
    for (void* p = *static_cast<void**>(list); p != nullptr; p = at<void*>(p, 0x38))
        count_list38_node(p, nfn, lfnc);
}

// @src 140257450  one function of 6 words
void count_list38_node(void* node, std::int32_t* nfn, std::int32_t* lfnc)
{
    (void)node;
    *nfn = *nfn + 1;
    *lfnc = *lfnc + 6;
}

// @src 140269b40  each window of the list (link +0x840): 14025d3b0
void count_window_list(void* list, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                       std::int32_t* lwin, DB_STEP_DATA* owner)
{
    for (void* w = *static_cast<void**>(list); w != nullptr; w = at<void*>(w, 0x840))
        count_window(w, nfn, lfnc, nwin, lwin, owner);
}

// @src 14025d3b0  one window definition by its kind (+0x174). "Active": 2D its point count
// (+0x178) > 0, 3D its shape (+0x210) > 0. An active window of kind 1, 3 or 4 counts with its
// points (2D, turned counter-clockwise) or 8 words (3D); kinds 5, 6 and 7 count without words;
// kind 8 and 10 count with their points. Then the window's tables: 1D -> 140130200 words;
// 2D -> sorted (140134120), 140130160 words; 3D -> 14012fff0 words (at most 100, unsorted, for
// kind 1; sorted, uncapped, for kind 7).
void count_window(void* win, std::int32_t* nfn, std::int32_t* lfnc, std::int32_t* nwin,
                  std::int32_t* lwin, DB_STEP_DATA* owner)
{
    bool active;
    if (!def::rt::dimension_is_3(owner))
        active = 0 < at<std::int32_t>(win, 0x178);
    else
        active = 0 < at<std::int32_t>(win, 0x210);
    const auto tab = [&](std::size_t off) { return static_cast<DYNARY*>(ptr(win, off)); };
    // the window itself, kinds 1, 3, 4
    const auto count_self = [&]() {
        if (!def::rt::dimension_is_3(owner)) {
            *nwin = *nwin + 1;
            *lwin = *lwin + at<std::int32_t>(win, 0x178);
            const std::uint8_t yes = 1;
            orient_window(win, &yes);
        } else {
            *nwin = *nwin + 1;
            *lwin = *lwin + 8;
        }
    };
    // a 1D table, or a 2D one after sorting
    const auto tab_12 = [&](std::size_t off) {
        if (tab(off)->ndim == 1) {
            *nfn = *nfn + 1;
            *lfnc = ext().dynary_words_1d(tab(off)) + *lfnc;
        } else if (tab(off)->ndim == 2) {
            dynary_sort(tab(off));
            const std::int32_t w = ext().dynary_words_2d(tab(off));
            *nfn = *nfn + 1;
            *lfnc = *lfnc + w;
        }
    };
    switch (at<std::int32_t>(win, 0x174) - 1) {
    case 0:  // kind 1
        if (active) {
            count_self();
            if (tab(0x20)->ndim == 1) {
                add_1d(win, 0x20, nfn, lfnc);
            } else if (tab(0x20)->ndim == 3) {
                std::int32_t w = ext().dynary_words_3d(tab(0x20));
                if (99 < w)
                    w = 100;
                *nfn = *nfn + 1;
                *lfnc = *lfnc + w;
            }
            if (at<std::int32_t>(win, 8) == 4) {
                if (tab(0x98)->ndim == 1) {
                    add_1d(win, 0x98, nfn, lfnc);
                } else if (tab(0x98)->ndim == 3) {
                    std::int32_t w = ext().dynary_words_3d(tab(0x98));
                    if (99 < w)
                        w = 100;
                    *nfn = *nfn + 1;
                    *lfnc = *lfnc + w;
                }
            }
        }
        break;
    case 2:  // kind 3
        if (active) {
            count_self();
            add_1d(win, 0x380, nfn, lfnc);
            tab_12(0x3f8);
            add_1d(win, 0x470, nfn, lfnc);
            add_1d(win, 0x4e8, nfn, lfnc);
        }
        break;
    case 3:  // kind 4
        if (active) {
            count_self();
            add_1d(win, 0x5e0, nfn, lfnc);
            tab_12(0x658);
        }
        break;
    case 4:  // kind 5
        *nwin = *nwin + 1;
        add_1d(win, 0x380, nfn, lfnc);
        tab_12(0x3f8);
        add_1d(win, 0x470, nfn, lfnc);
        add_1d(win, 0x4e8, nfn, lfnc);
        add_1d(win, 0x568, nfn, lfnc);
        break;
    case 5:  // kind 6
        *nwin = *nwin + 1;
        add_1d(win, 0x5e0, nfn, lfnc);
        tab_12(0x658);
        add_1d(win, 0x6d0, nfn, lfnc);
        break;
    case 6:  // kind 7
        *nwin = *nwin + 1;
        add_1d(win, 0x748, nfn, lfnc);
        if (tab(0x20)->ndim == 1) {
            add_1d(win, 0x20, nfn, lfnc);
        } else if (tab(0x20)->ndim == 3) {
            dynary_sort(tab(0x20));
            const std::int32_t w = ext().dynary_words_3d(tab(0x20));
            *nfn = *nfn + 1;
            *lfnc = *lfnc + w;
        }
        break;
    case 7:  // kind 8
        if (active) {
            *nwin = *nwin + 1;
            *lwin = *lwin + at<std::int32_t>(win, 0x178);
            const std::uint8_t yes = 1;
            orient_window(win, &yes);
            add_1d(win, 0x748, nfn, lfnc);
            add_1d(win, 0x7c0, nfn, lfnc);
        }
        break;
    case 9:  // kind 10
        if (active) {
            *nwin = *nwin + 1;
            *lwin = *lwin + at<std::int32_t>(win, 0x178);
        }
        break;
    default:
        break;
    }
}

// @src 140199c10  int at +0x78
std::int32_t step_u078(const DB_STEP_DATA* step) { return at<std::int32_t>(step, 0x78); }

// @src 14019a080  size of the surface-set list at +0x50 (14019a820)
std::int32_t surface_list_a_size(void* geo) { return ext().surface_list_a_size(ptr(geo, 0x50)); }
// @src 14019a0a0  size of the surface-set list at +0x58 (14019a840)
std::int32_t surface_list_b_size(void* geo) { return ext().surface_list_b_size(ptr(geo, 0x58)); }

// @src 1402fe780  every set of both lists: 1402f87f0 / 1402f8d80
void count_surface_sets(void* geo, std::int32_t* nfn, std::int32_t* lfnc)
{
    std::int32_t n = surface_list_a_size(geo);
    for (std::int32_t i = 0; i < n; ++i)
        count_surface_a(ext().surface_set_a(ptr(geo, 0x50), i), nfn, lfnc);
    n = surface_list_b_size(geo);
    for (std::int32_t i = 0; i < n; ++i)
        count_surface_b(ext().surface_set_b(ptr(geo, 0x58), i), nfn, lfnc);
}

// @src 1402f8760  5 when the set's kind (1402b7470) is 1, else 0
std::int32_t surface_a_ncomp(void* set)
{
    std::int32_t r = 0;
    if (ext().surface_set_a_kind(set) == 1)
        r = 5;
    return r;
}

// @src 1402f87a0  count * ncomp + 10, 0 when either is < 1
std::int32_t surface_a_words(void* set)
{
    std::int32_t n = ext().surface_set_a_count(set);  // 14019a000
    const std::int32_t c = surface_a_ncomp(set);
    if (n < 1 || c < 1)
        n = 0;
    else
        n = n * c + 10;
    return n;
}

// @src 1402f87f0  one function when it has words
void count_surface_a(void* set, std::int32_t* nfn, std::int32_t* lfnc)
{
    const std::int32_t w = surface_a_words(set);
    if (0 < w) {
        *nfn = *nfn + 1;
        *lfnc = *lfnc + w;
    }
}

// @src 1402f8ce0  1 when the set's kind (1402b74d0) is 1 or 2, else 0
std::int32_t surface_b_ncomp(void* set)
{
    std::int32_t r = 0;
    const std::int32_t k = ext().surface_set_b_kind(set);
    if (0 < k && k < 3)
        r = 1;
    return r;
}

// @src 1402f8d30  count * ncomp + 10, 0 when either is < 1
std::int32_t surface_b_words(void* set)
{
    std::int32_t n = ext().surface_set_b_count(set);  // 14019a060
    const std::int32_t c = surface_b_ncomp(set);
    if (n < 1 || c < 1)
        n = 0;
    else
        n = n * c + 10;
    return n;
}

// @src 1402f8d80  one function when it has words
void count_surface_b(void* set, std::int32_t* nfn, std::int32_t* lfnc)
{
    const std::int32_t w = surface_b_words(set);
    if (0 < w) {
        *nfn = *nfn + 1;
        *lfnc = *lfnc + w;
    }
}

} // namespace def::dbio::gendb
