# DEFORM database loader reconstruction

This C++20 library reads DEFORM databases independently of the original program.
It reconstructs the loading routines associated with `datarm23.f` in
`source/DEF_PRE_64.exe`, using REA/Ghidra analysis and comparisons with the running
original executable. This is the loader milestone, not a complete replacement
for the input preparation application.

Reference executable SHA-256:
`f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794`.
Source attribution comes from embedded source diagnostics and traced calls;
the executable's DWARF line headers contain no usable line programs. Decompiled
pseudocode is evidence, not recovered original Fortran source.

## Build and use

Requires a C++20 compiler and CMake 3.24 or newer. Verified on Windows x64 with
Visual Studio 2022 Build Tools:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release -- /m:1 /nr:false
ctest --test-dir build -C Release --output-on-failure
```

Outputs are `build/Release/deform_loader.lib`, `deform_db.exe`,
`deform_dump.exe`, and `deform_objects.exe`. The library has no REA, Fortran, or original-executable runtime
dependency. Public headers are in `include/deform`.

```cpp
#include "deform/database.hpp"

deform::database db(L"C:\\PROBLEM\\TEST_REA\\COG_P1_B1\\COG_P1_B1.DB");
const auto index = db.find_step(25); // First matching signed step number.
const auto step = db.load_step(index);
const auto auxiliary = db.load_auxiliary(index); // Original separate mode 5.
// step.header: exact host-order float64 bit patterns, including text slots.
// step.integers / step.reals: fully expanded arrays, in original order.
// auxiliary.present == false when the version/pointer disables that data.
```

Use `records()` for all table rows, `stored_steps()` for the original stored-step
filter, and `load_header(index)` for a header-only row. Indices in the C++ API are
zero based; physical record numbers and block destination indices in descriptors
retain their original one-based values. Duplicate step numbers can be selected
by record index.

```powershell
.\build\Release\deform_db.exe -m header -d "database.DB" -o header.txt
.\build\Release\deform_db.exe -m stepList -d "database.DB" -o steps.txt
.\build\Release\deform_db.exe -m lastStepNo -d "database.DB" -o last.txt
.\build\Release\deform_db.exe -m simStepList -d "database.DB" -o operations.txt
.\build\Release\deform_db.exe -m verify -d "database.DB" -o decoded.json
.\build\Release\deform_dump.exe "database.DB" 25 "step25"
```

The first four modes preserve the tested batch output format. `verify` decodes
every stored main array and writes counts and FNV-1a fingerprints; it is an
extension, not an original command. `deform_dump` writes host-order
`.header.bin`, `.integers.bin`, and `.reals.bin` files. It does not export a KEY
file. Auxiliary data is available through the library API.

## First DB import slice: object summaries

`deform_objects` follows the recovered importer's interpretation of the loaded
arrays and reports each object's ID, name, raw type code, application type code,
mesh flag, and mesh node/element counts. It supports the traced full-array 3D
version-11000 layout with 140 integer and 280 real address-table entries.
Other versions/layouts fail explicitly.

```powershell
.\build\Release\deform_objects.exe "C:\PROBLEM\TEST_REA\COG_P1_B1\COG_P1_B1.DB" 25 "objects.json"
```

The library entry point is `deform::summarize_objects(loaded_step)`, declared in
`include/deform/object_summary.hpp`. This adds an API without changing existing
loader calls or the four batch query modes.

REA static analysis and passive runtime probes established this path:

| Address | Recovered role |
|---|---|
| `0x14000b170` | DB import menu: file selection and step-number prompt |
| `0x1401cc2a0` | Resolve the first matching signed step number |
| `0x1401cc420` | Coordinate header, array, material, and object import |
| `0x1401cca80` | Read step header; obtain object count from header slot 0 |
| `0x1401cd1f0` | Allocate/load arrays and recover their address tables |
| `0x1403db9e8 → 0x1403b5a04 → 0x1403b6679` | Full-array read through the reconstructed loader |
| `0x1403dd950` | Copy address tables from the beginning of the integer array |
| `0x1401d2230` | Locate each object's integer/real descriptor using those tables |
| `0x14016caf0` | Populate object identity/type, name, and mesh counts |

These roles are derived descriptions, not recovered original C++ symbol names.
The import coordinator subsequently loads additional object data and, on the
normal successful path, frees its raw database buffers. The application builds
its own object state; retaining a reference to the raw arrays alone would not
reproduce the whole import.

For this layout, let `M` be the decoded integer array and `A` the decoded real
array. The integer address table is `M[0:140]`; the real address table is
`M[140:420]`. Their lengths are established by `M[1]-M[0]` and `M[2]-M[1]`.
For zero-based object slot `i`, descriptor locations are:

```text
integer descriptor = integer_table[3] - 1 + integer_table[73] * i
real descriptor    = real_table[5]    - 1 + real_table[145]    * i
```

The descriptor addresses and strides come from the file, not fixture-specific
constants. Integer descriptor words 4–7 are inclusive node/element ranges.
Counts are `max(0, end - start + 1)`, then forced to zero for non-mesh objects.
The 64-byte object name starts at real descriptor word 4. Original byte-order
and trailing-padding processing are reproduced; leading spaces are preserved.

| Observed import | Objects compared | First object | Mesh nodes | Mesh elements |
|---|---:|---|---:|---:|
| Small database, step 25 | 7 | Workpiece | 492 | 1,945 |
| Large database, step 690 | 7 | Workpiece | 20,013 | 104,836 |

All fourteen objects match the original importer for names, internal type codes,
mesh flags, and mesh counts. Descriptor positions are also checked against the
original's runtime address tables. The small case is independently corroborated
by the original KEY export. In the large case, the other six objects have
geometry but no mesh: their mesh counts are correctly zero. Geometry counts
must not be substituted for mesh counts.

`raw_type`, `internal_type`, and KEY `OBJTYP` are distinct code systems.
For example, this Workpiece has raw/application code 1 but KEY code 2. General
type-name and KEY-code mappings have not been established, so the API exposes
numeric codes without guessing their names.

Scope limits:

- Coordinates, connectivity, materials, and editable application objects are
  not constructed by this summary API.
- Version 11000, 3D, character mode 2 has original-runtime coverage. Character
  mode 0 byte reversal is supported by static evidence and synthetic tests.
- Inactive slots have `active=false`; their name/counts remain unpopulated.
- Unsafe/ambiguous descriptors are rejected. The original may retain constructor
  state, print an error, or continue instead.
- Names remain legacy character bytes. The JSON tool escapes non-ASCII bytes as
  `\u00XX`; it does not claim to have identified the original text encoding.

`python tools/compare_objects.py` reruns only the new tool against the two
retained original captures. Results are in `analysis/object-comparison.json`,
with summaries in `analysis/object-summary-small.json` and
`analysis/object-summary-large.json`. `analysis/object-summary-schema.json`
links the field mapping to REA evidence IDs. Full procedure dossiers are saved
as `analysis/import-0x*.json`; direct runtime observations are
`analysis/import-objects-small.json` and `analysis/import-objects-large.json`.
The earlier `import-runtime1.json` loader-stack experiment produced unreliable
unwinding above its immediate caller; those stack entries are not used as
call-chain evidence. The later probes observe function entry/caller addresses
directly.

## Recovered behavior and coverage

The reader implements 512-byte physical records, big-endian int32/float64 data,
linked root/page step tables, dimension/version dispatch, direct legacy arrays,
modern block directories, and backward-linked run-length decompression. File
offsets use 64-bit arithmetic. Fixed-width strings, six database header words,
thirteen step words, main directories (including opaque words), and auxiliary
metadata/descriptors survive in the API. Unknown fields remain raw; semantic
names are not invented.

Auxiliary data uses the separate step-version-11000 mode-5 path: twenty metadata
words, ten-word entries, and packed integer/float64 arrays. The API preserves
on-disk allocation bookkeeping instead of reproducing the reference's mutable
global allocator. `size_matches()` exposes a decoded-count mismatch, which the
original diagnoses without assigning a new error code.

| Check | Established result |
|---|---|
| COG_P1_B1.DB, 9,216,000 bytes | 32 table rows; all 14 stored steps decoded |
| Inc718_D420_GFM_V_DIES_Angle2_18.DB, 5,015,897,088 bytes | 790 rows; all 200 stored steps decoded, including offsets beyond 4 GiB |
| Four batch queries on each file | Exit code, stdout, stderr, result existence and result bytes match the original |
| Six invalid-input batch queries | Bad-signature and two short-file variants match for header/stepList |
| Original live loader, steps 25 and 690 | Header and both expanded arrays compared by SHA-256 against C++ output |
| Synthetic tests | Packed chains, exact IEEE payload bits, raw fields, header-only rows, linked index roots, >4-GiB offsets, auxiliary records |

Runtime observations use a passive REA/Frida hook at `0x1403b6679`, hashing
buffers after successful returns. The hook does not change arguments or returned
data. Process manifests bind the PID to the executable hash and record database
hashes before/after. The supplied databases were not modified; original runs
used private copies under ignored `analysis/oracle`.

Both supplied databases are unified 3D version 11.0. All 822 table rows have
zero auxiliary pointers. Consequently legacy 2D/3D branches and auxiliary data
have static/synthetic coverage, not validation against representative original
runtime fixtures. Decoding all 214 stored steps does not mean all 214 have been
compared against the original's memory; that stronger comparison covers two
selected steps.

## Compatibility boundary

The public interface is the agreed modern C++ library API. It does not export
the original Fortran ABI or reproduce the entire executable's CLI, interactive
menus, simulation state updates, KEY export, or write modes. The query harness
supports the documented flag spelling and four default query modes; additional
original aliases/options and all malformed-file behavior are not yet established.
`simStepList` behavior outside the supplied operation layouts also needs fixtures.

Recovered numeric loader errors are available as
`database_error::original_code()`. Checked C++ boundary failures use code zero;
packed data failures throw `packed_array_error`. The reader rejects invalid
offsets, lengths and compression chains rather than reproducing out-of-bounds
accesses or process termination. It stops on auxiliary I/O failures; the original
can continue and overwrite a prior status. Unwritten array gaps are zero
initialized in C++; the original allocator's contents are not a portable
contract. No database writer or selective in-place update API is included.

## Evidence and repeatable comparisons

`analysis/*.json` retains structured REA evidence with provider, target hash,
procedure addresses, evidence IDs, and coverage limitations. Main references:

- `0x1403b6679.json`: main header/array loader.
- `loader_dispatch.json`, `loader_mode5.json`, `auxiliary-schema.json`:
  dispatch and auxiliary layout.
- `physical_header.json`, `read_database_header.json`, `step_table.json`:
  physical header and table traversal.
- `unpack_integers.json`, `unpack_reals.json`: compression algorithm.
- `fixture-structure.json`: independent structural inspection of both files.
- `small-verify.json`, `large-verify.json`: full C++ main-array decode results.
- `oracle-comparison.json`: fourteen retained batch comparisons.
- `runtime-step25.json`, `runtime-step690-v2.json`: passive original-memory hashes.
- `runtime-comparison.json`: process-bound memory comparison results.

`python tools/compare_oracle.py` reruns the new query executable against the
retained original captures. `python tools/compare_runtime.py` reruns the new
dump executable and compares it with the retained REA memory observations.
Both require the local private fixtures/capture manifests under
`analysis/oracle`; those multi-gigabyte files are intentionally ignored by Git.
They do not execute the original program again.
