/* moproj2mst: reconstruct the DEF_GUI_MO_64.exe (v11.0.2) Open > Save > Close lifecycle
 * for a DEFORM multiple-operation project (.moproj). Unrecovered branches remain guarded;
 * passing conversion does not by itself establish complete original equivalence.
 *
 * Plan: docs/plan-gui-mo-stage1-moproj2mst.md section 3. Plain C ABI, x64, no runtime dependency.
 *
 *   - Without WRITE_SAVE, the converter reads the project and creates/replaces the output (written to a
 *     temporary file in the same folder, then renamed over the target), and with M2M_FLAG_WRITE_MDT
 *     the cogging operations' Cogging.MDT files.
 *   - No global state except the code page set with m2m_set_codepage(). Conversions may be started
 *     from several threads, but they run one at a time: the embedded engine (project database
 *     reader, KEY reader) keeps process-wide state, as the original program did.
 *   - msg receives the error text, or the warnings separated by '\n' when the result is M2M_OK. It is
 *     always NUL-terminated when msg != NULL and the size is > 0 (truncated if it does not fit).
 */
#ifndef MOPROJ2MST_H
#define MOPROJ2MST_H

#include <stddef.h>
#include <wchar.h>

#if defined(M2M_STATIC)
#  define M2M_API
#elif defined(M2M_BUILD_DLL)
#  define M2M_API __declspec(dllexport)
#else
#  define M2M_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define M2M_API_VERSION 1

enum {
    M2M_OK = 0,
    M2M_E_ARG = 1,           /* bad argument or buffer */
    M2M_E_OPEN = 2,          /* legacy conversion read preflight failed */
    M2M_E_PARSE = 3,         /* rejected input or failed project-read/Save decision */
    M2M_E_INPUT = 4,         /* a referenced data file is missing or malformed */
    M2M_E_NO_OPERATIONS = 5, /* the original writes no MST without operations */
    M2M_E_WRITE = 6,         /* output cannot be written */
    M2M_E_UNSUPPORTED = 7    /* operation type whose writer is not verified yet */
};

#define M2M_FLAG_DOE_TEMP 0x1u /* write the DOE .MST.TEMP form (later)           */
#define M2M_FLAG_STRICT   0x2u /* turn warnings (missing side files) into errors */
/* Also rewrite Operations/TaskN/Cogging.MDT of every cogging operation, as File > Save does
   (written before the MST, each through a temporary file and a rename). Without the flag the
   project folder is never written. Where the original's output is not defined (more than one
   reheat) the conversion fails with M2M_E_UNSUPPORTED and nothing is written. */
#define M2M_FLAG_WRITE_MDT 0x4u
/* Also write every file File > Save writes: the files the open of the active operation writes or
   deletes, those of Save step 2 (its task KEY files, SimCtrl.KEY, InterObject.KEY,
   ObjPropertiesNNNNN.KEY, equipment and material KEYs, ...), the .moproj (step 3; the project file
   is overwritten as the GUI's Save does), operation property files in row order, then the MST.
   Off by default. Native project, supported property files and MST open their final destinations
   directly. Property writers preserve their individual open gates and ignored write failures;
   permitted later writes continue and earlier partial effects remain. Boolean BWZ/INI are
   attempted during MST writer dispatch, only after the main MST destination opens.
   The adapter reports M2M_E_WRITE for a failed project write or main MST open/stream; this is
   separate from the original GUI Save wrapper's status. Earlier Open/Save-current effects,
   remaining property writers and qualified filesystem/encoding edges still require recovery.
   Input is an already selected path: this mode does not perform the legacy E_OPEN preflight.
   An unreadable .moproj follows Open's state/remesh steps before Save rejects openedOk=false
   with M2M_E_PARSE; an earlier unrecovered Open step returns M2M_E_UNSUPPORTED instead. */
#define M2M_FLAG_WRITE_SAVE 0x8u

/* Text is written in the ANSI code page, as the original (s1-02 O8); the default is 1252.
   m2m_set_codepage() selects another (932, 950, 20866, 949, 936 for DEFORM language codes).
   Returns M2M_OK, or M2M_E_ARG for a code page Windows does not know. Applies to conversions that
   start after the call. */
M2M_API int m2m_set_codepage(unsigned codepage);

/* GUI start-up unit that unit-dependent defaults use (s1-01 Q2): 0 = derive it as the original
   does (default), 1 = SI, 2 = English. Returns M2M_E_ARG for other values. Applies to conversions
   that start after the call. */
M2M_API int m2m_set_gui_unit(unsigned unit);

/* DEFORM installation folder whose PACK/pack.dat gives the version text on line 3 of a written
   Cogging.MDT ("* DEFORM-2D/3D 11.0 (Service Pack 2)  " for v11.0.2). NULL or "" = the v11.0.2
   text. A folder without a readable pack.dat gives "11.0" alone, as the original. Applies to
   conversions that start after the call. */
M2M_API int m2m_set_deform_dir_w(const wchar_t *dir);

M2M_API int m2m_api_version(void);

/* Adapter settings contract (not an original environment interface): each conversion reads
   M2M_MO_INPUT_MODE once. Unset or empty = 0; otherwise optional +/- and decimal digits in
   signed 32-bit range; invalid values return M2M_E_ARG. It supplies Setting/MoInputMode
   explicitly without reading the original installation's INI. Nonzero selects the active
   Forming editor300; its unrecovered Save returns M2M_E_UNSUPPORTED. Mode0 retains existing
   behavior. No ABI signatures, flags or API version change. */

/* out_mst_path NULL: <project folder>/<project base name>.MST, as the original. */
M2M_API int m2m_convert_w(const wchar_t *moproj_path, const wchar_t *out_mst_path,
                          unsigned flags, wchar_t *msg, size_t msg_chars);

/* Same with UTF-8 paths and message. */
M2M_API int m2m_convert_utf8(const char *moproj_path, const char *out_mst_path,
                             unsigned flags, char *msg, size_t msg_bytes);

#ifdef __cplusplus
}
#endif

#endif /* MOPROJ2MST_H */
