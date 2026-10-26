// defdbio/src/objcheck/ext.hpp: calls from packet p4-03 (object checks) into functions other
// packets own.
//
// Each wrapper names the binary function it stands for and the packet or ledger phase that ports
// it. Until that packet lands here, a wrapper runs a default body in ext.cpp:
//   * trivial member reads and list walks are written out there ("stub for <addr>", not @src;
//     the owner's @src port replaces them at integration, the call sites here do not change);
//   * anything larger ends in DEF_NOT_PORTED.
// Where an owner's C++ is already on the base branch (step_object_at, step_material_by_id,
// material_list_nth, dynary_value_count, OBJ_GEO3D::node_count, ...), the wrapper forwards to it.
//
// Tier-1 tests install g_hook to observe calls and to supply results, so the p4-03 logic can be
// checked against the decompile without the other packets. The hook sees the call first; when it
// returns false the default body runs.
#pragma once

#include <cstdint>

namespace def::model {
class DB_STEP_DATA;
class LLOBJ_DATA;
class Material;
} // namespace def::model

namespace def::dbio::objcheck::ext {

// Hook: (address, args, nargs, result) -> true when handled. Doubles travel as their bit pattern.
using Hook = bool (*)(std::uint64_t addr, const std::uint64_t* args, int nargs, std::uint64_t* ret);
inline Hook g_hook = nullptr;

// ---- p4-02b: error records. All four binary functions are a bare `ret`. --------------------------
void fun_14001cd60(const std::int32_t* code, const char* key, const void* a, const void* b);
void fun_14001cd70(const std::int32_t* code, const char* key, const void* a, const void* b);
void fun_14001cd80(const std::int32_t* code, const char* key, const void* a, const void* b);
void fun_14001cd90(const std::int32_t* code, const char* key, const void* a, const void* b);

// ---- simulation control getters (Cls_1401e7b10 at DB_STEP_DATA+0x750) ----------------------------
std::int32_t fun_1401f3c10(const void* sim);  // +0x104 (P2)
std::int32_t fun_1401f3c30(const void* sim);  // +0x108 (P2)
std::int32_t fun_1401f3c50(const void* sim);  // +0x10c (p4-02a)
std::int32_t fun_1401f3c70(const void* sim);  // +0x110 (p4-02a)
std::int32_t fun_1401f3cb0(const void* sim);  // +0x118 (p4-02a)
std::int32_t fun_1401ee580(const void* sim);  // +0x240 (P2)

// ---- LLOBJ_DATA (P2/P3 keyword packets, p4-02a) ----------------------------------------------------
bool fun_140160d20(void* obj);                 // 2D geometry count (1401983a0) > 0 (P2)
bool fun_140160d90(const void* obj);           // byte +0x3990 (P2)
bool fun_140160f10(const void* obj);           // type (+0xc) != 7 (P3)
std::int32_t fun_140165170(void* obj);         // edge count for the ECxxxx arrays (P2)
bool fun_140167ae0(void* obj);                 // only-deformation mode (p4-02a)
void* fun_140198620(void* obj, const std::int32_t* i);   // geometry object u3c00[*i] (P2)
void fun_1401fb270(void* obj, std::int32_t* a, std::int32_t* material);  // object material (P2)

// object lists (Cls_14017a6c0) and DB_STEP_DATA lookups
def::model::LLOBJ_DATA* fun_14017aaf0(void* list, std::int32_t id);       // find by number (P2)
def::model::LLOBJ_DATA* fun_1401d6ea0(void* step, std::int32_t id);       // step+0x1380 find (P2)
def::model::LLOBJ_DATA* fun_1401d6ed0(void* step, std::int32_t n);        // n-th object (p1-03)
def::model::Material* fun_1401d7060(void* step, std::int32_t id);         // material by number (p1-03)
def::model::Material* fun_1401d7090(void* step, std::int32_t n);          // n-th material (P2)
std::int32_t fun_14019a280(const void* list);  // count (+0x10) (P3)
def::model::LLOBJ_DATA* fun_14017aa80(void* list, std::int32_t n);        // n-th object (p1-04)

// node / element data blocks of the object (Cls_14017ef20 at +0x8a0, Cls_140182880 at +0x9e0)
void* fun_14017f020(void* blk);                                        // owner arrays (P2)
void fun_14017ffe0(void* blk, std::int32_t n, void* widths, std::int32_t mode);  // allocate (P2)
void* fun_140182980(void* blk);                                        // owner arrays (P2)
void fun_1401836c0(void* blk, std::int32_t n, void* widths, std::int32_t w, std::int32_t id);  // (P2)
void fun_1401d48d0(void* owner, const std::int32_t* id);               // (P2)
double* fun_140184520(void* blk);   // element block: grain array 0x35 at +0x98 (P3)

// boundary / curve / function sub-objects
std::int32_t fun_140186da0(void* blk);         // Cls_1401860a0: entries of type 6 (P2)
std::int32_t fun_14019a530(void* blk);         // Cls_1401860a0: list size (P2)
void* fun_140259b60(void* list, std::int32_t n);          // n-th node, link +0xe0 (P2)
std::int32_t fun_140189580(void* blk);         // Cls_140189440: window count (p4-02a)
std::int32_t fun_140195b60(void* blk);         // Cls_1401956d0: heat exchange count (p4-02a)
std::int32_t fun_140195b90(void* blk);         // Cls_1401956d0: diffusion count (p4-02a)
std::int32_t fun_140136a10(void* list);        // function list: largest number (p4-02a)
void* fun_1401368c0(void* list, std::int32_t id);        // function list: find by number (P3)
void* fun_1402580f0(void* list, std::int32_t id);        // Cls_140257340List: find by id (P2)

// DYNARY (defmodel/table)
bool fun_14012f610(const void* t);             // every n[i] >= 1 (dynary_counts_positive)
std::int32_t fun_14012ff20(const void* t);     // value count (dynary_value_count)

// materials
std::int32_t fun_140199c50(const void* list);  // MaterialList count (+0x10) (P1)
def::model::Material* fun_14029c260(void* list, std::int32_t n);          // n-th material (p1-08)
bool fun_1401b0620(const void* mat);           // +4 != 0 (P2)
bool fun_14028d0a0(const void* mat);           // mixture material (+0x1cc8 == 1) (p1-08)
std::int32_t fun_14029bb50(void* list, const char* flag);  // transformation model check (P2)

// geometry objects (_OBJ_GEO3D, OBJ_GEO2D)
std::int32_t fun_1400034d0(void* geo);         // node count (p1-07)
std::int32_t fun_140003500(void* geo);         // element count (p1-07)
std::int32_t* fun_140005510(void* geo);        // elems.mutable_at(0) (P2)
double* fun_140006410(void* geo);              // coords.mutable_at(0) (P2)
std::int32_t fun_1402fcfe0(void* geo, const char* flag);   // bearing surfaces (P2)

// inter-object list (Cls_1401fa020 at DB_STEP_DATA+0x13c8)
std::int32_t fun_1400015d0(const void* list);  // count (+0x10) (P2)
void* fun_1401fac90(void* list, std::int32_t n);          // n-th, link +0x390 (P2)

// small vector helpers (Fortran-free C++, P2 / p4-02a)
double fun_140239a30(const double* v);         // |v| (1402399b0 then CRT sqrt)
void fun_140239b40(double* out, const double* v);         // v / |v| (0 when |v| <= 1e-12) (p4-02a)
double fun_14023b720(double* v);               // normalise 3-vector in place, returns |v| (P2)
double fun_14023b3a0(const double* a, const double* b);   // 3-vector dot (P2)
double fun_1403091c0(double* v);               // normalise 2-vector in place, returns |v| (P2)
double fun_1403090e0(const double* a, const double* b);   // 2-vector dot (P2)

} // namespace def::dbio::objcheck::ext
