# moproj2mst

`moproj2mst.dll` converts a DEFORM multiple-operation project (`.moproj`) into the `.MST` master
file that DEF_GUI_MO_64.exe (v11.0.2) writes on **File > Save**, byte for byte, without changing
any project file. It is a plain C ABI DLL for the DMO app: x64, static CRT, no Qt or other
runtime dependency (it imports only `kernel32.dll`).

Status: stage 1 of `docs/plan-gui-mo-stage1-moproj2mst.md`. Until every operation writer is
merged, conversions of the affected types return `M2M_E_UNSUPPORTED`.

What runs of the GUI's File > Save: the active operation is opened and saved first (step 2: what
it changes in the engine model reaches the MST; its KEY files are written only with
`M2M_FLAG_WRITE_SAVE`), the `.moproj` itself (step 3, only with `M2M_FLAG_WRITE_SAVE`; it
overwrites the project file, as the GUI's Save does), then `Cogging.MDT` (step 4, only with
`--write-mdt` or `--write-save`) and the MST (step 5). A project whose active
operation's save is not ported yet returns `M2M_E_UNSUPPORTED`.

## Files to ship

| File | Use |
|---|---|
| `moproj2mst.dll` | the converter (x64 only) |
| `moproj2mst.lib` | import library for C/C++ callers |
| `include/moproj2mst.h` | the C ABI |
| `moproj2mst.exe` | optional command line: `moproj2mst <project.moproj> [out.MST] [--strict] [--codepage N] [--write-mdt] [--write-save] [--deform-dir D]`, exit code = result code |

The DLL carries a version resource: file version `<project version>.<ABI version>`, e.g. `0.1.0.1`.

## C ABI (`include/moproj2mst.h`, `M2M_API_VERSION 1`)

```c
int m2m_api_version(void);                       /* 1 */
int m2m_set_codepage(unsigned codepage);         /* default 1252 */
int m2m_set_gui_unit(unsigned unit);             /* default 0 */
int m2m_convert_w(const wchar_t *moproj_path, const wchar_t *out_mst_path,
                  unsigned flags, wchar_t *msg, size_t msg_chars);
int m2m_convert_utf8(const char *moproj_path, const char *out_mst_path,
                     unsigned flags, char *msg, size_t msg_bytes);
```

All functions use the C calling convention (`__cdecl`; on x64 this is the only convention).

**`m2m_convert_w` / `m2m_convert_utf8`**

* `moproj_path`: the `.moproj` file. Absolute or relative to the current directory.
* `out_mst_path`: where to write the MST. `NULL` writes `<project folder>/<project base name>.MST`,
  the file DEF_GUI_MO itself saves. Any other path is allowed (another folder, another name).
* `flags`: `0`, or a combination of the flags below.
* `msg`, `msg_chars` / `msg_bytes`: a buffer for text. On an error it receives the error message;
  on `M2M_OK` it receives the warnings, one per line separated by `\n`, or an empty string. It is
  always NUL-terminated when `msg != NULL` and the size is above 0, and truncated if it does not
  fit (4096 characters is plenty). `msg` may be `NULL`.
* Returns one of the result codes.

The converter only reads the project. It writes the MST to a temporary file in the output folder
and renames it over the target, so a failed conversion leaves an existing MST as it was. It never
creates, changes or deletes any other file, including the side files DEF_GUI_MO writes during a
save (`ObjPropertiesNNNNN.KEY`, `Boolean.BWZ`, `HT_Proc<k>.MOKEY`, ...). When the MST refers to such
a file and it is missing, the converter warns (an error with `M2M_FLAG_STRICT`).

The one exception is opt-in: with `M2M_FLAG_WRITE_MDT` the converter also rewrites
`Operations/TaskN/Cogging.MDT` of every cogging operation, byte for byte as File > Save does
(each through a temporary file and a rename, before the MST). It refuses, with
`M2M_E_UNSUPPORTED` and nothing written, where the GUI's output cannot be reproduced: more than one
reheat, or a project whose active diagram item is a cogging operation (Save then first takes that
operation's billet, die and manipulator geometry from the project objects). Line 3 of the MDT
carries the installation's version text; `m2m_set_deform_dir_w` points at a DEFORM folder whose
`PACK/pack.dat` gives it (default: the v11.0.2 text, `DEFORM-2D/3D 11.0 (Service Pack 2)`).
Details: `docs/re/moproj2mst/mdt/spec.md`.

The second opt-in, `M2M_FLAG_WRITE_SAVE` (`--write-save`, off by default), writes every file File >
Save writes: the files the open of the active operation writes or deletes, then Save step 2's (the
active operation's task KEY files, `SimCtrl.KEY`, `InterObject.KEY`, `ObjPropertiesNNNNN.KEY`,
equipment and material KEYs, ...), then the `.moproj` (step 3: the project file is overwritten,
byte for byte as a freshly started, maximised DEF_GUI_MO_64 saves it), then `Cogging.MDT` (the flag
includes `M2M_FLAG_WRITE_MDT`), then the MST. The `.moproj`'s view parts follow a fresh GUI on a
3840x2160 display at 175% (window and 3D widget sizes, `docs/re/moproj2mst/moproj/`); one value
differs on purpose: a cycle or group item's `Expanded` is always `yes` (the GUI writes an
uninitialised byte there). Properties vt+0xb8 files of other classes (`Forming.FWZ`, ...) are
not written yet either.

Thread safety: the settings (code page, GUI unit, DEFORM folder) are global. Conversions may be started from several threads,
but they run one at a time, because the embedded engine (project database reader, KEY reader) keeps process-wide state, as
the original program did.

**`m2m_set_codepage`**: the ANSI code page used for text from the project (title, names, paths)
in the MST, as DEF_GUI_MO writes it through the Windows locale codec. Default 1252. For DEFORM
language settings use 932 (Japanese), 950 (Chinese, Big5), 20866 (Russian, KOI8-R), 949 (Korean),
936 (Chinese, GBK). Returns `M2M_E_ARG` for a code page Windows does not know. It applies to
conversions that start after the call, in every thread. ASCII-only projects do not depend on it.

**`m2m_set_gui_unit`**: the unit DEF_GUI_MO starts with, which unit-dependent defaults use when the
project does not set a value itself (s1-01 Q2). `0` (default) derives it as the original does,
`1` = SI, `2` = English. Returns `M2M_E_ARG` for other values. Like the code page, it applies to
conversions that start after the call, in every thread.

### Result codes

| Code | Name | Meaning |
|---|---|---|
| 0 | `M2M_OK` | MST written; `msg` holds warnings, if any |
| 1 | `M2M_E_ARG` | bad argument or buffer |
| 2 | `M2M_E_OPEN` | the project file cannot be read |
| 3 | `M2M_E_PARSE` | not a valid `.moproj` (DEF_GUI_MO: "Error on reading project file!") |
| 4 | `M2M_E_INPUT` | a referenced data file is missing or malformed |
| 5 | `M2M_E_NO_OPERATIONS` | the project has no sub-project; DEF_GUI_MO writes no MST then |
| 6 | `M2M_E_WRITE` | the output cannot be written |
| 7 | `M2M_E_UNSUPPORTED` | an operation type whose writer is not verified yet |

### Flags

| Flag | Value | Meaning |
|---|---|---|
| `M2M_FLAG_DOE_TEMP` | `0x1` | write the DOE `.MST.TEMP` form (not supported yet) |
| `M2M_FLAG_STRICT` | `0x2` | turn warnings (missing side files) into errors |
| `M2M_FLAG_WRITE_MDT` | `0x4` | also rewrite each cogging operation's `Cogging.MDT` as Save does |
| `M2M_FLAG_WRITE_SAVE` | `0x8` | also write every file Save writes, the `.moproj` included (off by default; includes `M2M_FLAG_WRITE_MDT`) |

### Output format

The MST is what DEF_GUI_MO writes: ASCII text in the chosen code page, LF line ends, no BOM,
paths relative with `/`, numbers as Qt 4.8 prints them (6 significant digits, `-0` printed as
`0`, `1e-05`, `1e+09`).

## Calling it

Working samples are in `samples/` and run as tests (`ctest -L m2m-release`). Each one prints the
API version and the result of converting the project named on its command line.

### C / C++ (`samples/cpp/convert.cpp`)

```cpp
#include "moproj2mst.h"            // link moproj2mst.lib

wchar_t msg[4096];
int rc = m2m_convert_w(L"C:\\jobs\\COG_P1_B1\\COG_P1_B1.moproj", nullptr, 0, msg, 4096);
if (rc != M2M_OK) { /* msg = error text */ }
```

### C# (`samples/csharp/Moproj2Mst.cs`, P/Invoke)

Build the application as **x64** (not AnyCPU with "prefer 32-bit", not x86). Works on .NET
Framework 4.x and .NET 6+. Put `moproj2mst.dll` next to the application.

```csharp
using Moproj2Mst;

string message;
M2MResult rc = Converter.Convert(@"C:\jobs\COG_P1_B1\COG_P1_B1.moproj", null, M2MFlags.None, out message);
if (rc != M2MResult.Ok) Console.Error.WriteLine(message);
```

The P/Invoke declarations behind it:

```csharp
[DllImport("moproj2mst.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
static extern int m2m_convert_w(string moprojPath, string outMstPath, uint flags,
                                StringBuilder msg, UIntPtr msgChars);
```

### Python (`samples/python/moproj2mst.py`, ctypes)

64-bit Python, standard library only.

```python
from moproj2mst import Converter
m2m = Converter(r"C:\tools\moproj2mst.dll")
rc, message = m2m.convert(r"C:\jobs\COG_P1_B1\COG_P1_B1.moproj")
```

## Building

`apps\moproj2mst\build.cmd [Release|Debug]` configures with MSVC 2022 x64 and Ninja, builds the
DLL, the CLI, the samples and the tests, and runs tier 1 (`ctest -L m2m`). Products are in
`apps\moproj2mst\build\bin`.

Release checks (`ctest -L m2m-release`):

* `m2m.release.exports`: the DLL exports exactly the five functions above, is x64, imports no
  C/C++ runtime DLL, and its file version is set.
* `m2m.release.unchanged`: for each tier-2 corpus case, a copy of the project is converted with the
  default output path and with an output folder outside the project. Every file and folder is
  hashed (SHA-256) and its size, attributes, creation and last-write times recorded before and
  after. Any change fails except the output MST itself and its folder's last-write time after a
  successful default-path conversion. Stray files in the output folder also fail.
* `m2m.release.sample_cpp`, `sample_csharp`, `sample_python`: the samples load the DLL and call it.

Corpus comparisons: `cmake --build apps\moproj2mst\build -t m2m_tier2` (12 cases) or `m2m_tier3`.
