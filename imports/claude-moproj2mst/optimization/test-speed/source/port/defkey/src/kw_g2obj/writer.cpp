// kw-g2obj: the per-object KEY writer FUN_14029e250 (emission order and conditions:
// docs/re/keywords/group2-writer-map.md; decompile deform-rea-work/out/g2/w_main.c, listing
// out/g2obj/w_main.lst) and the custom state-variable entry writer FUN_1402b7310.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

#include <defkey/kw_g2obj.hpp>
#include <defrt/stdlib/legacy_printf.hpp>
#include <defmodel/keyword_registry.hpp>
#include <defrt/rdata.hpp>

#include "dispatch.hpp"
#include "internal.hpp"

namespace def::key::g2obj {

namespace {

// Registry ids of the keywords the driver writes inline (FUN_1402f7720 arguments).
constexpr int kPenvol = 0x95, kUsrsub = 0xfd, kRotsym = 0x102, kRefpos = 0xd7, kTrgvol = 0x103,
              kElpsol = 0xea, kFrcstp = 199, kFrcnel = 200, kFrcmth = 0xc9, kCrptim = 0xd4;

// Registry ids of the handler2 calls, in the driver's order.
constexpr int kAvgstr = 0x92, kLmtstr = 0x93, kZstr = 0x94, kReftmp = 0x96, kTmplmt = 0xca,
              kOtprng = 0xf0, kDrz = 0xa0, kBccfnc = 0xa1, kUrz = 0xa5, kFrz = 0xa6, kPrz = 0xa7,
              kNdtmp = 0xaa, kNdheat = 0xab, kNdflux = 0xac, kTimerc = 0xdf, kNrecvl = 0xe1,
              kUsrnod = 0xc4, kStrain = 0xaf, kStncmp = 0xb0, kDensty = 0xb1, kDamage = 0xb2,
              kStress = 0xb3, kYlds = 0xe9, kUsrelm = 0xc5, kVolfc = 0xcd, kVolfs = 0xd8,
              kDvolf = 0xce, kGrain = 0xd0, kGrain2 = 0xd1, kSizesh = 0xd2, kTxtodf = 0xd3,
              kErecvl = 0xe0, kTicf = 0xcf, kDatom = 0xd6, kHdnest = 0xda, kHdnobj = 0xd9,
              kBcccrb = 0xdb, kCrbflx = 0xdd, kVotage = 0xd5, kBccrht = 0xdc, kRhtflx = 0xde,
              kCsfreq = 0xeb, kVolcrg = 0xec, kZefi = 0xed, kZmfi = 0xee, kCurrnt = 0xef,
              kNwear = 0xff;

// rdata 14051bed8  0.0
constexpr double kZero = def::rt::rdata::d_14051bed8;

// The body of FUN_14029e250 after a successful OBJTYP line for a full object (internal type not
// 5 or 7). Returns the first non-zero status of a checked writer; the caller resets +0x3f24.
int write_full_object(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA& obj, std::uint32_t n,
                      KeyRef& ref)
{
    const std::uint32_t flags = writer_flags(m);
    int st = 0;

    write_objnam(m, f, &obj, n);  // status ignored
    if ((st = ext::write_keyword(kAvgstr, f, m, ref)) != 0) return st;
    if ((st = ext::write_keyword(kLmtstr, f, m, ref)) != 0) return st;
    if (!is_2d(m)) {
        rt::crt::fprintf(f, "%-6s %7i %20.10E\n", model::keyword_name(kPenvol), n,
                     at<double>(obj, 0x1eb0));
    } else {
        if ((st = ext::write_keyword(kZstr, f, m, ref)) != 0) return st;
        if ((st = ext::write_keyword(kPenvol, f, m, ref)) != 0) return st;
    }
    if (at<std::int32_t>(obj, 0x2150) != 0) {
        rt::crt::fprintf(f, "%-6s %7i %7i\n", model::keyword_name(kUsrsub), n,
                     at<std::int32_t>(obj, 0x2150));
    }
    if ((st = ext::write_keyword(kReftmp, f, m, ref)) != 0) return st;
    if ((st = ext::write_keyword(kTmplmt, f, m, ref)) != 0) return st;
    if (is_3d(m)) {
        rt::crt::fprintf(f, "%-6s %7i %20.10E\n", model::keyword_name(kRotsym), n,
                     at<double>(obj, 0x2058));
        rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", at<double>(obj, 0x2078),
                     at<double>(obj, 0x2080), at<double>(obj, 0x2088));
        rt::crt::fprintf(f, " %20.10E %20.10E %20.10E\n", at<double>(obj, 0x2060),
                     at<double>(obj, 0x2068), at<double>(obj, 0x2070));
    }
    if (at<std::int32_t>(obj, 0x1ec8) != 0) {
        if (at<std::int32_t>(obj, 0x1ec8) == 1) {
            rt::crt::fprintf(f, "%-6s %7i %7i %20.10E %20.10E %20.10E\n", model::keyword_name(kRefpos),
                         n, 1, at<double>(obj, 0x1ed0), at<double>(obj, 0x1ed8),
                         at<double>(obj, 0x1ee0));
        } else {
            if (at<std::int32_t>(obj, 0x1ec8) != 2) return 3;
            rt::crt::fprintf(f, "%-6s %7i %7i %7i\n", model::keyword_name(kRefpos), n, 2,
                         at<std::int32_t>(obj, 0x1ee8));
        }
    }
    // TRGVOL: a negative mode is written as 0 and clears a non-zero volume (the writer changes
    // the model)
    std::int32_t mode = at<std::int32_t>(obj, 0x2098);
    if (mode < 0) {
        mode = 0;
        if (at<double>(obj, 0x2090) != kZero) at<double>(obj, 0x2090) = 0.0;
    }
    rt::crt::fprintf(f, "%-6s %7i %7i %20.10E\n", model::keyword_name(kTrgvol), n, mode,
                 at<double>(obj, 0x2090));
    rt::crt::fprintf(f, "%-6s %7i %7i\n", model::keyword_name(kElpsol), n, at<std::int32_t>(obj, 0x1f18));
    if ((st = ext::write_keyword(kOtprng, f, m, ref)) != 0) return st;

    if ((flags & 0x40000) == 0) write_movement_block(m, f, &obj, static_cast<std::int32_t>(n));
    ext::write_geometry(m, f, obj, n);  // writer-map steps 19-22
    ext::write_nodes(m, f, obj, n);
    if ((st = ext::write_keyword(kDrz, f, m, ref)) != 0) return st;
    if (is_3d(m)) ext::write_state_var(m, f, obj, 0x19, n, 0);  // DRMESH

    // BCCFNC once per entry of the list obj+0x2e68 (FUN_14013c1b0: count at +0x10), with the
    // reference (obj+8, entry 1..n)
    for (std::int32_t i = 0; i < at<std::int32_t>(obj, 0x2e68 + 0x10); ++i) {
        ref.clear();  // FUN_14015a620
        ref.push_back(at<std::int32_t>(obj, 8));
        ref.push_back(i + 1);
        if ((st = ext::write_keyword(kBccfnc, f, m, ref)) != 0) return st;
    }
    ref.clear();
    ref.push_back(at<std::int32_t>(obj, 8));

    ext::write_state_var(m, f, obj, 0x12, n, 0);  // BCCDEF
    ext::write_state_var(m, f, obj, 0x13, n, 0);  // BCCDFN
    if ((st = ext::write_keyword(kUrz, f, m, ref)) != 0) return st;
    if ((st = ext::write_keyword(kFrz, f, m, ref)) != 0) return st;
    if (is_3d(m) && (st = ext::write_keyword(kPrz, f, m, ref)) != 0) return st;
    ext::write_state_var(m, f, obj, 0x14, n, 0);  // BCCTMP
    ext::write_state_var(m, f, obj, 0x15, n, 0);  // BCCTFN
    if ((st = ext::write_keyword(kNdtmp, f, m, ref)) != 0) return st;
    if ((st = ext::write_keyword(kNdheat, f, m, ref)) != 0) return st;
    if (is_3d(m) && (st = ext::write_keyword(kNdflux, f, m, ref)) != 0) return st;
    if (at<std::int32_t>(m, 0x1680) != 0 && at<void*>(obj, 0x958) != nullptr) {
        if ((st = ext::write_keyword(kTimerc, f, m, ref)) != 0) return st;
    }
    if (at<std::int32_t>(m, 0x1684) != 0 && at<void*>(obj, 0x960) != nullptr) {
        if ((st = ext::write_keyword(kNrecvl, f, m, ref)) != 0) return st;
    }
    if ((st = ext::write_keyword(kUsrnod, f, m, ref)) != 0) return st;
    ext::write_elements(m, f, obj, n);
    ext::write_state_var(m, f, obj, 0x3d, n, 0);  // MTLGRP
    for (int id : {kStrain, kStncmp, kDensty, kDamage, kStress, kYlds, kUsrelm}) {
        if ((st = ext::write_keyword(id, f, m, ref)) != 0) return st;
    }
    if ((flags & 0x10000) == 0) ext::write_mesh_block(m, f, obj, n);
    rt::crt::fprintf(f, "%-6s %7i %7i\n", model::keyword_name(kFrcstp), n, at<std::int32_t>(obj, 0x1f34));
    rt::crt::fprintf(f, "%-6s %7i %7i\n", model::keyword_name(kFrcnel), n, at<std::int32_t>(obj, 0x1f38));
    rt::crt::fprintf(f, "%-6s %7i %7i\n", model::keyword_name(kFrcmth), n, at<std::int32_t>(obj, 0x1f3c));
    rt::crt::fprintf(f, "%-6s %7i %7i\n", model::keyword_name(kCrptim), n, at<std::int32_t>(obj, 0x1f40));
    for (int id : {kVolfc, kVolfs, kDvolf}) {
        if ((st = ext::write_keyword(id, f, m, ref)) != 0) return st;
    }
    if (at<std::int32_t>(m, 0x898) == 0) {
        if ((st = ext::write_keyword(kGrain, f, m, ref)) != 0) return st;
    } else if (at<std::int32_t>(m, 0x898) == 1) {
        if ((st = ext::write_keyword(kGrain2, f, m, ref)) != 0) return st;
    }
    for (int id : {kSizesh, kTxtodf, kErecvl, kTicf, kDatom, kHdnest, kHdnobj, kBcccrb}) {
        if ((st = ext::write_keyword(id, f, m, ref)) != 0) return st;
    }
    if (is_3d(m) && (st = ext::write_keyword(kCrbflx, f, m, ref)) != 0) return st;
    for (int id : {kVotage, kBccrht, kRhtflx, kCsfreq, kVolcrg, kZefi, kZmfi, kCurrnt}) {
        if ((st = ext::write_keyword(id, f, m, ref)) != 0) return st;
    }
    ext::write_window_lists(m, f, obj, n);  // writer-map steps 48-52
    ext::write_local_bcs(m, f, obj, n);
    ext::write_edge_face_bcs(m, f, obj, n);
    ref.clear();
    ref.push_back(at<std::int32_t>(obj, 8));
    ext::write_state_var(m, f, obj, 0x2e, n, 0);  // MATAXI (its registry handler is not used)
    if ((st = ext::write_keyword(kNwear, f, m, ref)) != 0) return st;
    if (!is_3d(m)) {
        ext::write_wpaxis(m, f, obj, n);
    } else {
        ext::write_symsuf(m, f, obj, n);
        ext::write_wpaxis(m, f, obj, n);
    }
    ext::write_opstop(m, f, obj, n);
    ext::write_dynary(m, f, obj, n);
    ext::write_rsedef(f, m, ref);  // 1402a1c33: the status is not checked
    const int ncustom = ext::custom_count(obj);
    for (int i = 0; i < ncustom; ++i) {
        void* e = ext::custom_at(obj, i);
        if (e == nullptr) continue;
        ref.clear();
        std::int32_t id = ext::custom_id(e);
        ref.push_back(at<std::int32_t>(obj, 8));
        ref.push_back(id);
        if ((st = write_custom(f, m, ref)) != 0) return st;
    }
    return 0;
}

} // namespace

// @src 1402b7310  custom state-variable entry: FUN_1402a4a80(model, f, obj, 0x4e, N, ref[1])
int write_custom(std::FILE* f, DB_STEP_DATA& m, const KeyRef& ref)
{
    std::int32_t n = ref[0];
    LLOBJ_DATA* obj = find_object(m, n);
    if (0 < at<std::int32_t>(*obj, 0x3f24)) n = at<std::int32_t>(*obj, 0x3f24);
    ext::write_state_var(m, f, *obj, 0x4e, static_cast<std::uint32_t>(n), ref[1]);
    return 0;
}

// @src 14029e250  per-object KEY writer
int write_object(DB_STEP_DATA& m, std::FILE* f, LLOBJ_DATA* obj, std::uint32_t out_no,
                 bool with_header)
{
    if (obj == nullptr) return 0;
    at<std::uint32_t>(*obj, 0x3f24) = out_no;
    std::uint32_t n = out_no;
    if (out_no == 0) n = at<std::uint32_t>(*obj, 8);
    if (with_header) ext::write_file_header(m, f);
    if ((writer_flags(m) & 0x1000) != 0) ext::write_stnout(m, f);
    if ((writer_flags(m) & 0x2000) != 0) ext::write_elmnod(m, f);
    char banner[0x400];
    rt::crt::sprintf(banner, "Data for Object #     %i", n);
    rt::crt::fprintf(f, "*\n*  %s\n*\n", banner);
    KeyRef ref;  // 140159860, then 1402b7530(ref, obj+8)
    ref.push_back(at<std::int32_t>(*obj, 8));

    int result = 0;
    if (write_objtyp(m, f, *obj, n) != 0) {
        result = 3;
    } else if (at<std::int32_t>(*obj, 0xc) == 7) {
        result = 0;
    } else if (at<std::int32_t>(*obj, 0xc) == 5) {
        write_movement_block(m, f, obj, static_cast<std::int32_t>(n));
        if (at<std::uint8_t>(*obj, 0x2e05) == 0) ext::write_mesh_block(m, f, *obj, n);
        if ((writer_flags(m) & 0x1000000) != 0) ext::write_winprs_block(m, f, *obj, n);
        result = 0;
    } else {
        result = write_full_object(m, f, *obj, n, ref);
    }
    at<std::int32_t>(*obj, 0x3f24) = 0;
    return result;
}

} // namespace def::key::g2obj
