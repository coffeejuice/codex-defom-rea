// defmodel/hdrstats/object_stats.hpp: per-object statistics that GENDB writes into the step header
// (packet p4-07): min/max of nodal and element state variables, stress and strain invariants and
// principal values, surface area, the state-variable accessors they use and the header slot table.
// The step-header writer itself (1401c5020 / 1401e2180) is in defdbio/gendb/step_header.hpp.
//
// Free functions; each takes the object first, as the binary passes `this`. Codes are DEFORM's
// state-variable codes as 14020cae0 switches on them: 100 damage, 110 nodal damage, 200 relative
// density, 300 strain, 310 nodal strain, 400 strain rate, 500 stress, 600 temperature,
// 700 velocity, 800 displacement, 900 / 1000 user node / element variables, 1600, 2600, 2800,
// 5000, 5500, 5620. Callees owned by other packets go through ext_p4_07.hpp.
#pragma once

#include <cstdint>

#include <defmodel/hdrstats/ext_p4_07.hpp>

namespace def::model {
class LLOBJ_DATA;
class StateVarBase;
} // namespace def::model

namespace def::model::p407 {

// ---- object getters ----
bool obj_kind_is_2(const LLOBJ_DATA* obj);                 // 140158d00: 140158ce0(obj) == 2
bool obj_flag_3612(const LLOBJ_DATA* obj);                 // 140161100: byte at +0x3612
std::int32_t obj_kind_140220520(const LLOBJ_DATA* obj);    // 140220520: 140158ce0(obj)
// 140166200: offset of the component block of `*code` (3000, 3100, 3200, 3300, 3400, 3500) in the
// element strain array, from the five counts at model +0x86c; 0 when all counts are 0, -1 unknown
std::int32_t strain_block_offset(LLOBJ_DATA* obj, const std::int32_t* code);

// ---- node state-variable set LLOBJ_DATA+0x8a0: allocate variable `id`, mark it, return it ----
double* node_var_01(void* set);  // 140180940 id 0x01 (+0x10)  code 800 displacement
double* node_var_02(void* set);  // 1401809c0 id 0x02 (+0x18)  code 700 velocity
double* node_var_08(void* set);  // 140180c40 id 0x08 (+0x40)  code 600 temperature
double* node_var_0d(void* set);  // 140180f90 id 0x0d (+0x68)  code 2600
double* node_var_0f(void* set);  // 140181090 id 0x0f (+0x78)  code 5000
double* node_var_1b(void* set);  // 140181320 id 0x1b (+0x98)  code 310 nodal strain

// ---- element state-variable set LLOBJ_DATA+0x9e0 ----
double* elem_var_26(void* set);  // 140183d50 id 0x26 (+0x20)  stress tensor
double* elem_var_27(void* set);  // 140183dd0 id 0x27 (+0x28)  strain-rate tensor
double* elem_var_28(void* set);  // 140183e50 id 0x28 (+0x30)  strain tensor
double* elem_var_29(void* set);  // 140183ed0 id 0x29 (+0x38)  code 500 effective stress
double* elem_var_2a(void* set);  // 140183f50 id 0x2a (+0x40)  code 400 effective strain rate
double* elem_var_2b(void* set);  // 140183fd0 id 0x2b (+0x48)  code 300 effective strain
double* elem_var_38(void* set);  // 1401846d0 id 0x38 (+0xb0)  code 2800

// ---- StateVarBase (LLOBJ_DATA+0xb70) ----
void sv_set_id(StateVarBase* sv, std::int32_t id);         // 140300e80: +0x08 = id
// 140301280: value (row, col, k) = v, index row * dim1 * dim2 + col * dim1 + k
void sv_set_value(StateVarBase* sv, std::int32_t row, std::int32_t col, std::int32_t k, double v);

// ---- min / max ----
// 14020cae0: min and max of variable *code over the object's nodes or elements; component *comp
// (1-based) when the variable has more than one value per row; vectors (700, 800) by length.
// *init: start from the first value (else from 0).
void obj_min_max(LLOBJ_DATA* obj, const std::int32_t* code, double* vmin, double* vmax,
                 const std::int32_t* comp, const bool* init);
// 14020d320: the same for code 500 (stress) and *kind: 0 effective, 1 mean, 2 / 4 / 5 principal
void obj_stress_min_max(LLOBJ_DATA* obj, const std::int32_t* code, const std::int32_t* kind,
                        double* vmin, double* vmax, const std::int32_t* comp, const bool* init);
// 140211ef0: per component (at most 6) min and max of the tensor *code (300, 400, 500) into
// out[2k], out[2k + 1]
void obj_component_min_max(LLOBJ_DATA* obj, const std::int32_t* code, double* out);

// ---- derived arrays ----
// 14020f220 "LLOBJ_AllocSVArray": calloc'ed array for *mode (1 node, 2 element, 3 node vector,
// 4 element tensor); *rows = rows
double* alloc_sv_array(LLOBJ_DATA* obj, const std::int32_t* mode, const std::int32_t* code,
                       const std::int32_t* dims, std::int32_t* rows);
// 14020f4d0 "LLOBJ_DATA::AllocSVMagArray": one double per row when (*code, *kind) needs it
double* alloc_sv_mag_array(LLOBJ_DATA* obj, const std::int32_t* code, const std::int32_t* kind);
// 14020f6a0: dst = the scalar of kind *kind of src (per element / averaged over integration points
// when sv has more than one); sv = 0 from the step header
void sv_derive(LLOBJ_DATA* obj, const std::int32_t* code, double* src, double* dst,
               const std::int32_t* kind, std::int32_t* width, std::int32_t* offset, double* aux,
               StateVarBase* sv);
// 140210e90: the per-row kernel of sv_derive
void sv_derive_rows(LLOBJ_DATA* obj, const std::int32_t* code, double* src, double* dst,
                    const std::int32_t* kind, const std::int32_t* width,
                    const std::int32_t* offset, double* aux);
// 140211d30: *kind is a principal value (2..5) of a tensor code; 700, 800, 1500, 11300, 14000:
// always (returned in AL)
bool sv_kind_is_principal(std::int32_t code, std::int32_t kind);

// ---- kernels ----
float fabs_float(float x);                                 // 1402204d0 -> 1402204f0
float fabs_float_b(float x);                               // 1402204f0: (float)fabs((double)x)
// 14023e670: half the length of the sum of the cross products of consecutive points
double polygon_area(std::int32_t n, const Vec3* pts);
// 1401638f0: surface area of the object (3D only): which 0 the geometry's elements, 1 the mesh's
// surface polygons (LLOBJ_DATA+0xbe0)
double obj_surface_area(LLOBJ_DATA* obj, std::int32_t which);
// 14023efb0: cyclic Jacobi for a symmetric 3x3 float matrix (rows of 3 floats; destroyed above the
// diagonal); d eigenvalues, v eigenvectors in columns; returns the number of rotations
std::int32_t jacobi3(float* a, float* d, float* v);
// 14023fc50: sort d descending and permute the columns of v
void eigsrt3(float* d, float* v);
// 14032d1b0: 2D principal values of s = (sxx, syy, szz, sxy): *kind 0/1 effective (von Mises /
// 1: deviatoric form), p1 >= p2, *pmax / *pmin of (p1, p2, szz), angle
void principal_2d(const std::int32_t* kind, const double* s, double* eff, double* pmax,
                  double* p1, double* p2, double* angle);
// 1403f1980: 3D effective value and principal values of s (6 components; *kind 0 halves the
// shear terms, 1 effective stress, else the 2/3 / 4/3 weighted norm)
void principal_3d(const std::int32_t* kind, const double* s, double* eff, double* pmax,
                  double* pmid, double* pmin);
// 1403f1caa: invariants of the 6-component tensor, then the cubic's roots; all zero -> zeros
void principal_3d_roots(double* s, double* pmax, double* pmid, double* pmin, std::int32_t* fail);
// 1403f1ec3: roots of x^3 + a x^2 + b x + c (trigonometric form); *fail = 1 when q <= 0 or
// |r / q^1.5| > 1 + 1e-6
void cubic_roots(const double* a, const double* b, const double* c, double* rmax, double* rmid,
                 double* rmin, std::int32_t* fail);

// ---- header slot table ----
void load_hdr_slots(const std::int32_t* dim);             // 1401c7f80

} // namespace def::model::p407
