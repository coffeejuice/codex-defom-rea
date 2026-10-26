// defmodel/geoprim/def_geoprim.hpp: _DEF_GEOPRIM (RTTI .?AV_DEF_GEOPRIM@@), the geometry
// primitive CNVT3D builds a 3D object from (revolve / extrude of a 2D profile or 2D mesh).
// Owner: packet p9-02 (class layout, ctor/dtor/reset, setup records, profile and int-list
// arrays, the record containers). p9-04 (builder 1402ba500, extrude) and p9-05 (revolve) add
// their code in their own files and use the members below by name.
//
//   vtable   0x140541438 (slot 0 1402f0620 scalar deleting dtor)
//   ctor     1402b7ad0   dtor 1402b8090   reset 1402b81a0   clear arrays 1402b84e0
//   setup    1402b8520 extrude (kind 1), 1402b8980 revolve (kind 2); the second argument goes
//            to +0x23a0 (geometry path), the third to +0x23a4 (mesh path)
//   size     0x2520 (last member 0x2518; the callers keep it on the stack: 1402b7ad0 on
//            RSP+0x110 with the cookie at RSP+0x26c0 in 140153c00, so <= 0x25b0)
//
// Bases in the binary (construction order 1402b7ad0, destruction order 1402b8090):
// ProfileArray at 0x1700, OBJ_MESH2D at 0 (MSVC puts the base with the vfptr first),
// IntArrayList at 0x1720; then the members from 0x1730. Here OBJ_MESH2D is the base and the two
// others are members at the same offsets; their init is trivial, so the order is not visible.
//
// Member names: u<hex offset> until the role is clear; the named ones say what the setup
// functions put there. Adding a member never moves an existing one (ask p9-02).
#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <new>

#include <defmodel/layout/layout_check.hpp>
#include <defmodel/layout/obj_mesh2d.hpp>
#include <defmodel/linked/axis_point_list.hpp>
#include <defmodel/obj_mesh2d/rc_array.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
} // namespace def::model

namespace def::model::geoprim {

// operator new (FUN_1404797d8, malloc that throws) / free: every GEOPRIM heap block is released
// with free() by a scalar deleting dtor, so the classes allocate with malloc.
struct MallocNew {
    static void* operator new(std::size_t n)
    {
        void* p = std::malloc(n);
        if (p == nullptr) throw std::bad_alloc();
        return p;
    }
    static void operator delete(void* p) { std::free(p); }
};

// 2D point list, 1-based: point i (1..n) is pts[2*i], pts[2*i+1]; slot 0 is unused.
// 0x18 bytes; no ctor/dtor of its own in the class (init/clear are called explicitly).
struct ProfileArray {
    std::int32_t n;            // 0x00
    std::byte pad_04[4];
    double* pts;               // 0x08 malloc'd, 2*(n+1) doubles
    char u10;                  // 0x10 cleared by init/clear
    std::byte pad_11[7];

    // @src 1402e3f70  init: n = 0, pts = 0, u10 = 0
    // @src 1402e3f20  ctor (init; returns this)
    void init();
    // @src 1402e4030  clear: n = 0, free(pts), pts = 0, u10 = 0
    // @src 1402e3f50  dtor (clear)
    void clear();
    // @src 1402e3fa0  clear, n = *count, and when n + 1 > 0 a buffer of 2*(n+1) doubles
    //                 (uninitialised, operator new 14047780c)
    void resize(const std::int32_t* count);
    // @src 1402e4080  resize(count), then point i+1 = xy[2*i], xy[2*i+1] for i < *count
    void assign(const std::int32_t* count, const double* xy);
};
static_assert(sizeof(ProfileArray) == 0x18);

// int32 array, 0x10 bytes.
struct IntArray {
    std::int32_t n;            // 0x00
    std::byte pad_04[4];
    std::int32_t* data;        // 0x08 malloc'd

    // @src 1402e9040  ctor (init)
    IntArray() { init(); }
    // @src 1402e9070  dtor (clear)
    ~IntArray() { clear(); }
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;

    // @src 1402e9090  init: n = 0, data = 0
    void init();
    // @src 1402e90c0  clear: n = 0, free(data), data = 0
    void clear();
    // @src 1402e9110  clear, n = count, and when count > 0 a buffer of count ints
    void resize(std::int32_t count);
    // @src 1402e9180  resize(*count), memcpy count ints from src
    void assign(const std::int32_t* count, const std::int32_t* src);
};
static_assert(sizeof(IntArray) == 0x10);

// Array of IntArray (new[] with count cookie), 0x10 bytes. Lists are numbered 1..n.
struct IntArrayList {
    std::int32_t n;            // 0x00
    std::byte pad_04[4];
    IntArray* items;           // 0x08 new IntArray[n]

    // @src 1402e9290  init: n = 0, items = 0
    // @src 1402e9240  ctor (init; returns this)
    void init();
    // @src 1402e92c0  clear: clear every item, delete[] items (1402f0940), items = 0, n = 0
    // @src 1402e9270  dtor (clear)
    // @src 1402f0940  vector deleting dtor of the items (delete[])
    void clear();
    // @src 1402e9370  clear, n = *count, and when > 0 new IntArray[n] (all empty)
    // @src 1402e94d0  the same (wrapper)
    void resize(const std::int32_t* count);
    // @src 1402e9500  when 0 < *k <= n: items[*k - 1].assign(count, src)
    void assign(const std::int32_t* k, const std::int32_t* count, const std::int32_t* src);
};
static_assert(sizeof(IntArrayList) == 0x10);

// Revolve record (operator new 0x888 in 1402b8980; also the element type of the extrude
// record's section list).
struct RevolveRec : MallocNew {
    ProfileArray profile;      // 0x000 (ctor 1402e3f20)
    std::byte pad_018[8];
    char u020[0x400];          // 0x020 init: [0] = 0
    char u420[0x400];          // 0x420 init: [0] = 0 (a name, read by p9-05)
    std::int32_t u820;         // 0x820 init 0
    std::byte pad_824[4];
    double axis_pt[3];         // 0x828 init (0,0,0); setup: the 7th argument, copied raw
    double axis_dir[3];        // 0x840 init (0,0,1); setup: the 6th argument, copied raw
    double start;              // 0x858 init 0
    double angle;              // 0x860 init 0
    std::int32_t sections;     // 0x868 init 0x24
    std::int32_t axis_kind;    // 0x86c init 0 (1402ce190 sets 1 or 2)
    std::int32_t u870;         // 0x870 init 0
    std::byte pad_874[0x14];   // 0x874 not written by p9-02 code

    // @src 1402e3820  ctor: profile ctor, then init (Ghidra: CMFCScanlinerBitmap ctor)
    RevolveRec();
    // @src 1402e3860  dtor: profile dtor (1402e3f50)
    ~RevolveRec();
    RevolveRec(const RevolveRec&) = delete;
    RevolveRec& operator=(const RevolveRec&) = delete;
    // @src 1402e3880  init (the profile is not touched)
    void init();
};
static_assert(sizeof(RevolveRec) == 0x888);

// Extrude record (operator new 0x850 in 1402b8520).
struct ExtrudeRec : MallocNew {
    char u000[0x400];          // 0x000 init: [0] = 0
    char u400[0x400];          // 0x400 init: [0] = 0
    std::int32_t u800;         // 0x800 init 0
    std::byte pad_804[4];
    double dir[3];             // 0x808 init (0,0,1); setup: *dir when given
    double length;             // 0x820 init 0
    std::int32_t count;        // 0x828 init 10; setup: *count
    std::int32_t u82c;         // 0x82c init 0
    std::int32_t u830;         // 0x830 setup: = prim+0x23cc
    std::byte pad_834[4];
    CowVector<RevolveRec*> sections; // 0x838 (ctor 1402f0ab0); init: 100 null entries
    char u840;                 // 0x840 init 0
    std::byte pad_841[7];
    std::uint64_t u848;        // 0x848 init 0

    // @src 1402e2b10  ctor: sections ctor (1402f0ab0), init
    ExtrudeRec();
    // @src 1402e2b50  dtor: delete every non-null section (1402f0760), then the vector dtor
    ~ExtrudeRec();
    ExtrudeRec(const ExtrudeRec&) = delete;
    ExtrudeRec& operator=(const ExtrudeRec&) = delete;
    // @src 1402e2c00  init: fields above; delete every non-null section (pointers are left in
    //                 place), clear the vector (1402f0c40), push 100 nulls (1402f0ba0)
    void init();
};
static_assert(sizeof(ExtrudeRec) == 0x850);

// The records at +0x2338/+0x2340/+0x2348: allocated outside p9-02 (not on the CNVT3D path
// that p9-02 reads); only their scalar deleting dtors are here. Rest of the layout unknown.
struct Rec2338 : MallocNew {
    // @src 1402e2840  dtor (empty)
    ~Rec2338() = default;
};
struct Rec2340 : MallocNew {
    ProfileArray profile;      // 0x000
    // @src 1402e2960  dtor: profile dtor (1402e3860 -> 1402e3f50)
    ~Rec2340() { profile.clear(); }
};
struct Rec2348 : MallocNew {
    ProfileArray profile;      // 0x000
    // @src 1402e2a50  dtor: profile dtor (1402e3860 -> 1402e3f50)
    ~Rec2348() { profile.clear(); }
};

// Element of the RcArray at +0x24e0 (0xa0 bytes; dtor 1402f0890 releases the int vector at
// +0x50). Filled by p9-04/p9-05 code; the other fields are not known to p9-02.
struct Elem24e0 {
    std::byte u00[0x50];
    CowVector<std::int32_t> v50;   // 0x50 (dtor 140159b40)
    std::byte u58[0x48];
    // @src 1402f0890  dtor: v50 dtor
    ~Elem24e0() = default;
};
static_assert(sizeof(Elem24e0) == 0xa0);
DEF_LAYOUT_OFFSET(Elem24e0, v50, 0x50);

class DEF_GEOPRIM : public OBJ_MESH2D {
public:
    // @src 1402b7ad0  ctor
    DEF_GEOPRIM();
    // @src 1402b8090  dtor
    // @src 1402f0620  slot 0: scalar deleting dtor (dtor, free when flag & 1)
    ~DEF_GEOPRIM() override;
    DEF_GEOPRIM(const DEF_GEOPRIM&) = delete;
    DEF_GEOPRIM& operator=(const DEF_GEOPRIM&) = delete;
    static void* operator new(std::size_t n) { return MallocNew::operator new(n); }
    static void operator delete(void* p) { std::free(p); }

    // @src 1402b81a0  reset: kind/modes 0, delete the records, clear the arrays, delete the
    //                 owned object/model, free 0x24a8/0x24b0, clear the list at 0x2518
    void reset();
    // @src 1402b84e0  clear the profile, the 2D mesh (1403de3d0) and the int lists
    void clear_arrays();

    // @src 1402b8520  extrude setup (kind 1). Arguments as in the binary (all by pointer except
    // the two modes and the array bases):
    //   mode_geo -> +0x23a0, mode_mesh -> +0x23a4, length -> rec+0x820 and +0x2360,
    //   count -> rec+0x828 and +0x2380, profile (n_prof, xy_prof) -> +0x1700,
    //   2D mesh (nn, xy, ne, conn) -> 1402e9aa0, int list 1 (n_list, list) -> +0x1720,
    //   b23c8 / b23c9 / i23cc -> +0x23c8 / +0x23c9 / +0x23cc (and rec+0x830),
    //   d2388 -> +0x2388, v2490 / v2498 -> element 0 of the vectors (push when empty),
    //   d2448 / d2450 -> +0x2448 / +0x2450, dir (may be null) -> rec+0x808.
    void setup_extrude(std::int32_t mode_geo, std::int32_t mode_mesh, const double* length,
                       const std::int32_t* count_, const std::int32_t* n_prof, const double* xy_prof,
                       const std::int32_t* nn, const double* xy, const std::int32_t* ne,
                       const std::int32_t* conn, const std::int32_t* n_list, const std::int32_t* list,
                       const char* b23c8, const char* b23c9, const std::int32_t* i23cc,
                       const double* d2388, const double* v2490, const double* v2498,
                       const double* d2448, const double* d2450, const double* dir);

    // @src 1402b8980  revolve setup (kind 2):
    //   mode_geo -> +0x23a0, mode_mesh -> +0x23a4, the 2D mesh is cleared (1403de3d0),
    //   start -> rec+0x858, angle -> rec+0x860, axis_dir / axis_pt (3 doubles each, copied raw,
    //   no normalisation: 140239800 is a copy) -> rec+0x840 / +0x828, sections -> rec+0x868,
    //   then 1402dd0d0, profile -> +0x1700, 2D mesh -> 1402e9aa0, int list 1 -> +0x1720.
    void setup_revolve(std::int32_t mode_geo, std::int32_t mode_mesh, const double* start,
                       const double* angle, const double* axis_dir, const double* axis_pt,
                       const std::int32_t* sections, const std::int32_t* n_prof, const double* xy_prof,
                       const std::int32_t* nn, const double* xy, const std::int32_t* ne,
                       const std::int32_t* conn, const std::int32_t* n_list, const std::int32_t* list);

    // @src 1402dd0d0  when rec: angle (+0x2378) = rec->angle, sections (+0x2380) = rec->sections
    void take_revolve_params(const RevolveRec* rec);

    // @src 1402e9aa0  2D mesh: allocate nn nodes (1403de680) and ne 4-node elements
    //                 (1403de6e0), memcpy 2*nn doubles to the coordinates (140220440) and 4*ne
    //                 ints to the connectivity (140220460)
    void set_mesh2d(const std::int32_t* nn, const double* xy, const std::int32_t* ne,
                    const std::int32_t* conn);

    // ---- layout (OBJ_MESH2D 0x0000..0x16ff) -------------------------------------------------
    ProfileArray profile;           // 0x1700 2D profile (base in the binary)
    double u1718;                   // 0x1718 ctor 0.0004 (DAT_140541420)
    IntArrayList int_lists;         // 0x1720 (base in the binary); setup: one list
    char key_name[0x400];           // 0x1730 ctor "DEF_GEOPRIM.KEY"
    char u1b30[0x400];              // 0x1b30 ctor [0] = 0 (extrude runs 1402bc580 when set)
    char u1f30[0x400];              // 0x1f30 ctor [0] = 0
    std::int32_t kind;              // 0x2330 0 none, 1 extrude, 2 revolve
    std::byte pad_2334[4];
    Rec2338* u2338;                 // 0x2338
    Rec2340* u2340;                 // 0x2340 (reset skips deleting 0x2358 when it equals this)
    Rec2348* u2348;                 // 0x2348
    ExtrudeRec* extrude;            // 0x2350
    RevolveRec* revolve;            // 0x2358
    double u2360;                   // 0x2360 extrude setup: length
    double u2368;                   // 0x2368 extrude setup: 0
    std::int32_t u2370;             // 0x2370
    std::byte pad_2374[4];
    double angle;                   // 0x2378 revolve angle (1402dd0d0)
    std::int32_t sections;          // 0x2380 ctor 0x12; revolve sections / extrude count
    std::byte pad_2384[4];
    double u2388;                   // 0x2388 ctor 1.0; extrude setup: *d2388
    CowVector<double> v2390;        // 0x2390 (14019a6b0)
    CowVector<double> v2398;        // 0x2398 (14019a6b0)
    std::int32_t mode_geo;          // 0x23a0 geometry path (second setup argument)
    std::int32_t mode_mesh;         // 0x23a4 mesh path (third setup argument)
    char owns_obj;                  // 0x23a8 obj deleted by reset when set (14019a290)
    std::byte pad_23a9[7];
    LLOBJ_DATA* obj;                // 0x23b0 3D object
    char owns_model;                // 0x23b8 model deleted by reset when set (140158fc0)
    std::byte pad_23b9[7];
    DB_STEP_DATA* model;            // 0x23c0 3D model
    char u23c8;                     // 0x23c8
    char u23c9;                     // 0x23c9
    std::byte pad_23ca[2];
    std::int32_t u23cc;             // 0x23cc
    char u23d0;                     // 0x23d0
    char u23d1;                     // 0x23d1
    std::byte pad_23d2[6];
    void* ctx;                      // 0x23d8 builder context (1402ba500 5th argument)
    std::int32_t u23e0;             // 0x23e0
    std::int32_t u23e4;             // 0x23e4
    std::int32_t u23e8;             // 0x23e8
    std::int32_t u23ec;             // 0x23ec
    double u23f0;                   // 0x23f0 ctor 0 (8-byte zero; type not known)
    double u23f8;                   // 0x23f8
    double u2400;                   // 0x2400
    double u2408;                   // 0x2408
    double u2410;                   // 0x2410
    double u2418;                   // 0x2418 ctor 1.0
    double u2420;                   // 0x2420 ctor 0
    double u2428;                   // 0x2428 ctor 0
    std::byte pad_2430[0x18];       // 0x2430 not written by the ctor
    double u2448;                   // 0x2448 ctor -1.0; extrude setup: *d2448
    double u2450;                   // 0x2450 ctor -1.0; extrude setup: *d2450
    std::int32_t u2458;             // 0x2458 ctor 0
    std::byte pad_245c[4];
    std::int32_t u2460;             // 0x2460
    std::int32_t u2464;             // 0x2464
    std::int32_t u2468;             // 0x2468
    std::int32_t u246c;             // 0x246c
    std::int32_t u2470;             // 0x2470
    std::byte pad_2474[4];
    std::uint64_t u2478;            // 0x2478 ctor 0
    std::uint64_t u2480;            // 0x2480 ctor 0
    std::uint64_t u2488;            // 0x2488 ctor 0
    CowVector<double> v2490;        // 0x2490 (14019a6b0); extrude setup: element 0
    CowVector<double> v2498;        // 0x2498 (14019a6b0); extrude setup: element 0
    CowVector<std::int32_t> v24a0;  // 0x24a0 (140159ad0)
    void* u24a8;                    // 0x24a8 ctor 0; reset: free
    void* u24b0;                    // 0x24b0 ctor 0; reset: free
    char u24b8;                     // 0x24b8
    char u24b9;                     // 0x24b9
    char u24ba;                     // 0x24ba
    std::byte pad_24bb[1];
    std::int32_t u24bc;             // 0x24bc ctor 3
    double u24c0;                   // 0x24c0 ctor 120.0
    double u24c8;                   // 0x24c8 ctor 120.0
    double u24d0;                   // 0x24d0 ctor 120.0
    char u24d8;                     // 0x24d8
    char u24d9;                     // 0x24d9
    char u24da;                     // 0x24da
    char u24db;                     // 0x24db revolve: set when the axis kind is 2
    std::byte pad_24dc[4];
    mesh2d::RcArray<Elem24e0>* h24e0;                   // 0x24e0 (1402f0c70 / 1402f0ce0)
    mesh2d::RcArray<std::byte>* h24e8;                  // 0x24e8 (1402f0e10 / 1402f0e80)
    mesh2d::RcArray<linked::Cls_140257340>* h24f0;      // 0x24f0 (1402f0fd0 / 1402f1040)
    char u24f8;                     // 0x24f8
    char u24f9;                     // 0x24f9
    char u24fa;                     // 0x24fa
    char u24fb;                     // 0x24fb
    char u24fc;                     // 0x24fc
    char u24fd;                     // 0x24fd
    char u24fe;                     // 0x24fe
    std::byte pad_24ff[1];
    double u2500;                   // 0x2500 ctor 0.001
    double u2508;                   // 0x2508 ctor 0.3
    double u2510;                   // 0x2510 ctor 5.233
    CowList<double> l2518;          // 0x2518 (140159860, 8-byte values)
};

static_assert(sizeof(DEF_GEOPRIM) == 0x2520);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, profile, 0x1700);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u1718, 0x1718);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, int_lists, 0x1720);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, key_name, 0x1730);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u1b30, 0x1b30);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u1f30, 0x1f30);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, kind, 0x2330);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2338, 0x2338);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, extrude, 0x2350);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, revolve, 0x2358);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2360, 0x2360);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2370, 0x2370);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, angle, 0x2378);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, sections, 0x2380);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2388, 0x2388);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, v2390, 0x2390);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, v2398, 0x2398);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, mode_geo, 0x23a0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, mode_mesh, 0x23a4);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, owns_obj, 0x23a8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, obj, 0x23b0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, owns_model, 0x23b8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, model, 0x23c0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u23c8, 0x23c8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u23cc, 0x23cc);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u23d0, 0x23d0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, ctx, 0x23d8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u23e0, 0x23e0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u23f0, 0x23f0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2418, 0x2418);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2448, 0x2448);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2458, 0x2458);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2460, 0x2460);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2470, 0x2470);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2478, 0x2478);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, v2490, 0x2490);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, v24a0, 0x24a0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24a8, 0x24a8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24b8, 0x24b8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24bc, 0x24bc);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24c0, 0x24c0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24d8, 0x24d8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24db, 0x24db);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, h24e0, 0x24e0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, h24f0, 0x24f0);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24f8, 0x24f8);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u24fb, 0x24fb);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2500, 0x2500);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, u2510, 0x2510);
DEF_LAYOUT_OFFSET(DEF_GEOPRIM, l2518, 0x2518);

// Calls p9-02 makes into code owned elsewhere. Each pointer starts at a default: the OBJ_MESH2D
// accessors (Phase 6-7 callees, through p1-04 sub-objects) stop in DEF_NOT_PORTED until their
// owner sets them; the two scalar deleting dtors default to delete, which is the same code
// (dtor, then free). Tests point them at recorders.
struct Ext {
    // FUN_1403de680(mesh, &nn): allocate nn nodes (140182860, 14017ffe0 on mesh+8)
    void (*mesh_alloc_nodes)(OBJ_MESH2D* mesh, const std::int32_t* nn);
    // FUN_1403de6e0(mesh, &ne, &nper): allocate ne elements of nper nodes (1401836c0 on +0x148)
    void (*mesh_alloc_elems)(OBJ_MESH2D* mesh, const std::int32_t* ne, const std::int32_t* nper);
    // FUN_140220440(mesh): node coordinates (1401808d0 on mesh+8)
    double* (*mesh_coords)(OBJ_MESH2D* mesh);
    // FUN_140220460(mesh): element connectivity (140183cb0 on mesh+0x148)
    std::int32_t* (*mesh_conn)(OBJ_MESH2D* mesh);
    // FUN_14019a290(obj, 1): LLOBJ_DATA scalar deleting dtor (default llobj::ext::llobj_delete)
    void (*obj_delete)(LLOBJ_DATA* obj);
    // FUN_140158fc0(model, 1): DB_STEP_DATA scalar deleting dtor, p8-09 (default: delete)
    void (*model_delete)(DB_STEP_DATA* model);
};
Ext& ext();

} // namespace def::model::geoprim
