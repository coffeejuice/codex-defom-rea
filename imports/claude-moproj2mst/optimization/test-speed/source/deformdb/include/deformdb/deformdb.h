/* C ABI of deformdb.dll: read DEFORM .DB files without DEFORM.
 * All functions return 0 on success, non-zero on error; deformdb_last_error()
 * gives the message for the calling thread. Pointers returned for a step stay
 * valid until deformdb_step_free(). Distinct step handles may be used from
 * different threads at the same time. */
#ifndef DEFORMDB_H
#define DEFORMDB_H

#include <stdint.h>

#if defined(_WIN32)
#  if defined(DEFORMDB_BUILDING)
#    define DEFORMDB_API __declspec(dllexport)
#  else
#    define DEFORMDB_API __declspec(dllimport)
#  endif
#else
#  define DEFORMDB_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct deformdb_db deformdb_db;
typedef struct deformdb_step deformdb_step;

typedef struct deformdb_entry {
    int32_t slot, mesh_no, step, header_size, mar_size, a_size, sim_no, sim_mode, dim, version;
    int32_t rec_dynamic, rec_header, rec_mar, rec_a;
} deformdb_entry;

typedef struct deformdb_region {
    int32_t index, start, length, stored, record;
    char name[8];
} deformdb_region;

DEFORMDB_API const char* deformdb_last_error(void);

DEFORMDB_API int deformdb_open(const char* utf8_path, deformdb_db** out);
DEFORMDB_API void deformdb_close(deformdb_db* db);
DEFORMDB_API int deformdb_refresh(deformdb_db* db);

DEFORMDB_API int deformdb_title(const deformdb_db* db, const char** title);
DEFORMDB_API int deformdb_version(const deformdb_db* db, int32_t* version);
DEFORMDB_API int deformdb_entry_count(const deformdb_db* db, int32_t* count);
DEFORMDB_API int deformdb_get_entry(const deformdb_db* db, int32_t index0, deformdb_entry* out);
DEFORMDB_API int deformdb_find_step(const deformdb_db* db, int32_t step, int32_t* index0);

/* flags: 1 = header, 2 = MAR/A arrays, 4 = dynamic arrays (0 = everything) */
DEFORMDB_API int deformdb_step_read(const deformdb_db* db, int32_t index0, uint32_t flags, deformdb_step** out);
DEFORMDB_API void deformdb_step_free(deformdb_step* step);

/* Reads many directory entries in parallel; out[k] receives entry indices0[k]. */
DEFORMDB_API int deformdb_steps_read(const deformdb_db* db, const int32_t* indices0, int32_t count,
                                     uint32_t flags, uint32_t threads, deformdb_step** out);

DEFORMDB_API int deformdb_step_header(const deformdb_step* s, const double** data, int64_t* count);
DEFORMDB_API int deformdb_step_mar(const deformdb_step* s, const int32_t** data, int64_t* count);
DEFORMDB_API int deformdb_step_a(const deformdb_step* s, const double** data, int64_t* count);

DEFORMDB_API int deformdb_step_region_count(const deformdb_step* s, int is_real, int32_t* count);
DEFORMDB_API int deformdb_step_region(const deformdb_step* s, int is_real, int32_t index0, deformdb_region* out);

DEFORMDB_API int deformdb_step_dynamic_count(const deformdb_step* s, int is_real, int32_t* count);
/* For integer arrays *data is const int32_t*, for real arrays const double*. */
DEFORMDB_API int deformdb_step_dynamic(const deformdb_step* s, int is_real, int32_t index0, int32_t* slot,
                                       const void** data, int64_t* count);

/* ---- Typed view (step version >= 11000) ---- */

typedef struct deformdb_object {
    int32_t number, status, deform_code, porous;
    int32_t first_node, node_count, first_elem, elem_count, nodes_per_elem;
    char name[72];
} deformdb_object;

typedef struct deformdb_field {
    const void* data;      /* const double* or const int32_t* */
    int64_t rows;
    int32_t width;
    int32_t is_int;
    int64_t row_stride;    /* element (value) stride between rows */
    int64_t comp_stride;   /* element stride between components */
} deformdb_field;

/* Globals: NSTART, MESHNO, NUMNP, NUMEL and time; returns non-zero if the step has no typed view. */
DEFORMDB_API int deformdb_step_globals(deformdb_step* s, int32_t* nstart, int32_t* meshno, int32_t* numnp,
                                       int32_t* numel, double* time);
DEFORMDB_API int deformdb_step_object_count(deformdb_step* s, int32_t* count);
DEFORMDB_API int deformdb_step_object(deformdb_step* s, int32_t index0, deformdb_object* out);
/* Keyword data of one object, e.g. "RZ", "STRESS", "NDTMP", "BCCDEF" (see keyword table). */
DEFORMDB_API int deformdb_step_keyword(deformdb_step* s, int32_t index0, const char* keyword, deformdb_field* out);
/* Connectivity as local 1-based node numbers; pass buffer=NULL to get the required count. */
DEFORMDB_API int deformdb_step_connectivity(deformdb_step* s, int32_t index0, int32_t* buffer, int64_t capacity,
                                            int64_t* count);

#ifdef __cplusplus
}
#endif

#endif
