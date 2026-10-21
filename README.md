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

## Keyword importer and original model: recovery milestone

The keyword importer and application model are being recovered from the original
executable before implementing a C++ replacement. No keyword-import API or new
application model is implemented yet.

The recovered root-state constructor uses the `_DB_STEP_DATA` vtable label.
It owns simulation controls, material collections, object lists and interaction
records. The console constructs two linked root states; keyword dispatch can
select an alternate state. Objects use the `LLOBJ_DATA` vtable label and own
separate node and element buffers. These names and byte offsets describe the
reference executable, not a proposed portable layout.

Two bounded original-process runs exercised fourteen imports using twelve small
synthetic 3D KEY fixtures. Passive REA probes recorded state before and after
handlers and finalization. The observations establish, for those cases:

| Case | Original behavior |
|---|---|
| Subsequent name-only import | Updates the existing object and preserves its type |
| Repeated name keyword | Later name takes effect |
| Unknown or lowercase keyword | Warns, skips its line, and continues |
| `OBJTYP 1 BAD 0` | Returns success but makes object1 inactive; later keywords still execute |
| Missing positive object ID4 | Creates inactive placeholders2/3 and a default-type object4 |
| `OBJTYP` code6 | Preserves the previous internal type |
| Nonsequential coordinate/element row labels | Rows still populate sequential array slots |
| Truncated coordinate block | Returns status2 after changing the count/buffer and prefix rows; retains old connectivity |
| Finalization after that error | Still runs; prior model state is not restored |

The allocation paths free old mesh buffers and allocate zero-initialized
replacements. They also process dependent arrays, so replacing coordinates or
connectivity is not an isolated vector assignment. The keyword table confirms
`DBREAD`, `KFREAD` and `GENDB` command entries as well as ordinary data keywords.

The specification and evidence are:

- `analysis/model-lifecycle-specification.json`: root state, object lifecycle,
  ownership paths, import sequence, implementation gate and remaining questions.
- `analysis/keyword-object-contract.json`: detailed partial object-keyword contract.
- `analysis/keyword-observations.json`: verified, normalized observations.
- `analysis/keyword-runtime-1.json`, `keyword-runtime-2.json`: original REA captures.
- `analysis/keyword-table-selected.json`: selected exact table entries.
- `analysis/model-0x*.json`: focused static evidence with target identity and IDs.

Run `python tools/verify_keyword_observations.py` to validate the retained capture
identities and behavioral assertions. It does not launch the original executable.
It requires the private run manifests and generated probes under ignored
`analysis/oracle`. `tools/capture_keywords.py` supplies bounded console transcripts
for new observations; `tools/observe_keywords.js` is the passive probe template.

This is a partial specification. Complete dependent-array ownership, alternate
model routing, material/reference invariants and derived geometry calculations
remain unresolved. The fixtures do not establish full KEY compatibility or 2D
behavior. The existing C++ DB loader and object-summary API remain unchanged.

## Original startup recovery

REA static analysis and four bounded original-process runs now establish the
normal interactive startup path and startup-file selection. Each process was
created suspended, instrumented with passive hooks, then resumed once. The
captures distinguish constructor state, completed console-context state, and the
state after startup configuration.

Both root-state constructors receive dimension 3. Connecting the roots changes
the secondary root to dimension 2. The console also binds each root to its own
database helper. With no startup file, the first menu has empty material and
object collections, problem name `DEFORM`, and initialized simulation controls.

| Files present in the private working directory | Observed result |
|---|---|
| None | First menu: primary 3D, secondary 2D, no objects |
| `DEF_PRE3.INI` and `DEF_PRE.INI` | Only PRE3 imported; first menu: 3D/2D, fixture object 1 present |
| `DEF_PRE.INI` | PRE imported; first menu: 2D/2D, fixture object 1 present |
| All three, including `DEF_MULTI.INI` | Only MULTI imported; process exits successfully without entering the main menu |

The initialization files use the existing keyword reader. They can therefore
populate application state before a user explicitly imports a file. A faithful
replacement needs both a constructor baseline and the subsequent configuration
phase. The two roots cannot be assumed always to have different dimensions.

Evidence and repeatable checks:

- `analysis/startup-existing-evidence.json`: static call order, field offsets,
  startup-file precedence, runtime corroboration and unresolved questions.
- `analysis/startup-observations.json`: verified phase snapshots with normalized
  pointer relationships, selected control values and all 79 component counts.
- `analysis/startup-runtime-default-v3.json`, `startup-runtime-pre3-priority.json`,
  `startup-runtime-pre2.json`, `startup-runtime-multi-priority.json`: authoritative
  passive captures for the four cases.
- `analysis/startup-0x*.json`: focused static evidence with target identity and
  REA evidence IDs.
- `tools/capture_startup.py` and `tools/observe_startup.js`: bounded suspended
  launch harness and passive probe template.

Run `python tools/verify_startup_observations.py` to validate retained executable,
process, probe and file identities, event order, startup defaults, ownership and
configuration behavior. It uses private manifests under `analysis/oracle` and
does not launch the original. Earlier `startup-runtime-default.json` and
`startup-runtime-default-v2.json` captures are superseded because of probe
context/string-decoding defects and are excluded from verification.

This recovery covers the tested startup paths and selected model state. It does
not establish every CRT initializer, every control's domain meaning, complete
database-helper internals, malformed initialization-file behavior, or general
dimension switching with existing meshes. No C++ application-model or importer
implementation was added in this milestone.

## Autonomous application recovery

`analysis/recovery-plan.json` records the full reconstruction objective,
completion criteria, milestone states and continuation point.
`analysis/recovery-feature-ledger.json` contains the initial 22 feature records
and six groups of string-only candidates, with separate static, runtime and
implementation status. This is an expanding inventory, not a completeness claim.
Several geometry import options invoke the external `DEF_CVT3` converter before
reading `SCRATCH.KEY`; those dependency boundaries remain in scope.

The first internal model component is now implemented:
`src/recovered/state_variable.*` reconstructs the observed state-variable
allocation contract. It is built as `deform_recovered_state`, separately from
the existing loader API. It tracks current and previous count, component width
and multiplicity, selection rules, clear-before-replacement behavior and
conditional material-axis defaults. The 28 node and 25 element descriptor
registrations are documented in `analysis/mesh-array-contract.json`.

Two bounded original-process captures cover seven sequential imports, including
same-size replacement, growth, shrinkage and an incomplete coordinate block.
The C++ component exactly reproduces all 1,060 descriptor snapshots across
20 observed object states. This comparison checks shape history, lengths and
values; it does not establish a complete model or importer.

```powershell
python tools/verify_mesh_array_observations.py
python tools/compare_state_variables.py --report analysis/state-variable-comparison.json
ctest --test-dir build -C Release --output-on-failure
```

The first command validates original-process identities and private capture
manifests. The comparison uses retained captures and the C++ replay driver; it
does not execute the original or require the private multi-gigabyte DB files.
When Python is found by CMake, it is also registered as
`state_variable_reference_replay`.

The C++ component deliberately uses portable vector ownership. Original shared
backing and copy/detach behavior are not yet reconstructed, and pointer identity
is not compared. Negative extents and signed-int extent overflow are rejected
outside the established compatibility domain. Nondefault multiplicities,
explicit post-only allocation, nine-component material-axis initialization and
2D mesh cases still need runtime validation.

The next internal component, `src/recovered/mesh_boundary.*`, reconstructs
four-node tetrahedral boundary construction after connectivity import. It
preserves oriented faces, repeated first nodes in the fourth output slot,
face ordering and owning-element IDs. The embedded sort compares node-ID
sums alone; its permutation of equal keys affects duplicate cancellation.
The reconstruction therefore includes the recovered quicksort and short-sort
behavior instead of relying on the C++ library's unspecified tie order.

Across three original-process captures, 12 tetrahedral boundary outputs and
10 sort permutations match exactly. Four tetrahedral evaluation cases were
captured after freezing the candidate, including mixed duplicate orientation,
reordered elements and inverted connectivity. The candidate remained unchanged
during that evaluation. These small cases do not establish arbitrary mesh-size
equivalence or a complete importer.

```powershell
python tools/verify_mesh_boundary_observations.py
python tools/verify_boundary_sort_observations.py
python tools/compare_mesh_boundaries.py --report analysis/mesh-boundary-comparison.json
ctest --test-dir build -C Release --output-on-failure
```

The provenance verifiers need the private capture manifests under
`analysis/oracle`. The C++ comparison needs only the retained captures and
replay driver, and is registered as `mesh_boundary_reference_replay`.
The contract and remaining compatibility gaps are recorded in
`analysis/mesh-boundary-contract.json`; candidate hashes and
evaluation-case definitions are retained separately.

`src/recovered/hexa_boundary.cpp` now implements the eight-slot branch,
including its zero/repeated-node classification of collapsed wedge and
tetrahedral records. Three more original processes cover 16 imports: five
training cases, five separate evaluation cases, and six collapsed/mixed-mesh
discovery cases. All 16 boundary outputs and 16 sort permutations match
exactly, without changing the candidate after its freeze.

Mixed classes expose an original behavior worth preserving: a mesh containing
one hexahedron and one wedge produces an owner ID of 3 for one face. The
recovered calculation divides the compressed candidate index by that element
class's face count. The implementation retains these observed values.
Their downstream interpretation has not yet been recovered.

```powershell
python tools/verify_hexa_observations.py
python tools/compare_hexa_boundaries.py --report analysis/hexa-boundary-comparison.json
```

The eight-slot contract is in `analysis/hexa-boundary-contract.json`; the replay
is registered as `hexa_boundary_reference_replay`. Native invalid-memory faults,
negative node IDs, signed overflow and allocation failures remain outside this
component's verified compatibility domain. Ten-node boundaries, downstream
geometry and complete importer integration remain work in progress.

`src/recovered/planar_boundary.*` implements the recovered 2D builder. It
cancels reverse-directed edge pairs, preserves surviving edge order, and
builds the original ordered node chains with inclusive one-based ranges and
zero/-1 separators. Twelve inputs across two original processes match exactly,
including separate evaluation cases with reversed orientation, duplicates,
disconnected loops and a ring with a hole. The comparison also checks the
object's copied groups and total edge count.

```powershell
python tools/verify_planar_observations.py
python tools/compare_planar_boundaries.py --report analysis/planar-boundary-comparison.json
```

The contract is in `analysis/planar-boundary-contract.json`; replay is registered
as `planar_boundary_reference_replay`. All seven local CTest checks pass.
The original object's 100-group limit is known statically but not covered by
these small fixtures. Nonpositive IDs and allocation failures remain outside
the tested contract.

Two original processes establish that the tested 3D keyword route rejects
ten-node connectivity before boundary construction. A fresh object can still
be created, and a following name record still takes effect despite the import
error. `analysis/ten-node-boundary-research.json` records this bounded result
and the unresolved reachability of the low-level ten-node branch.

`src/recovered/mesh_storage.*` integrates recovered allocation, descriptor
history, direct integer buffers and boundary construction. It separates
replacement from row filling so incomplete imports can retain partial state.
Across three original processes, all 31 object snapshots and 1,643 descriptor
snapshots match. Eleven new snapshots also compare direct buffers and boundary
faces while switching from a cube to a tetrahedron, importing incomplete
coordinates, then loading a rotated cube.

```powershell
python tools/compare_mesh_storage.py --report analysis/mesh-storage-comparison.json
python tools/verify_mesh_storage_observations.py
python tools/verify_ten_node_observations.py
ctest --test-dir build -C Release --output-on-failure
```

The new integration sequence uses previously exercised individual fixtures;
it is not unseen-input coverage. The candidate was frozen before capture.
`analysis/mesh-storage-contract.json` records the exact scope and remaining
gaps. All eight CTest checks pass, including `mesh_storage_reference_replay`.
This remains an internal mesh component: the full object lifecycle, production
keyword scanner, additional dependent arrays and scientific finalization still
need reconstruction.

`src/recovered/keyword_reader.*` adds the recovered bounded row readers.
Three original processes supplied 66 helper calls, including 23 separate
evaluation calls. Status, conversion count, consumed lines and numeric output
bytes match exactly. This uncovered a real CRT difference: the original reads
`0x1p2` as zero followed by an unrecognized suffix, while modern scanf accepts
hexadecimal floating-point 4. The replacement now uses the recovered decimal
lexical rules, including incomplete exponents such as `1e+`.

```powershell
python tools/verify_keyword_row_observations.py
python tools/compare_keyword_readers.py --report analysis/keyword-reader-comparison.json
```

The component accepts already translated text bytes. It is not the complete
keyword importer; dispatch, object selection and finalization remain separate.
Full numeric-range/rounding equivalence, long-line boundaries and nondefault
blank-line handling still require evaluation. See
`analysis/keyword-reader-contract.json` for candidate history and coverage.

Populated-object deactivation is also captured. The original clears arrays but
keeps selected arity, descriptor history and an empty active boundary group.
`MeshStorage::deactivate` preserves that distinction. The integrated replay now
matches 39 mesh snapshots and 2,067 descriptor snapshots; the eight deactivation
snapshots guided implementation and are not held-out cases. All nine CTest
checks pass. Complete object cleanup remains broader than this mesh component.

```powershell
python tools/verify_object_deactivation_observations.py
```

`src/recovered/object_model.*` and `keyword_import.*` now connect the recovered
readers and mesh storage for `OBJNAM`, `OBJTYP`, `RZ` and `ELMCON`. The complete
304-entry keyword catalog distinguishes unknown tokens from recognized handlers
that have not yet been implemented. The latter stop with an explicit unsupported
outcome. Objects retain identity across imports, and failed imports preserve
earlier mutations.

Thirteen original imports match 52 keyword events, 56 parser-complete object
snapshots, 2,968 descriptor snapshots and 111 token/selector reads. Nine imports
were evaluated after archiving the candidate sources and binaries, which remained
unchanged. Those cases include new sequences and selector edge cases; some mesh
fixtures had already been exercised in component tests. All 13 CTest checks pass.

```powershell
python tools/compare_keyword_import.py --report analysis/keyword-import-training-comparison.json
python tools/compare_keyword_import.py --capture analysis/keyword-import-runtime-held-out.json --run-record analysis/oracle/keyword-import-held-out/run.json --report analysis/keyword-import-held-out-comparison.json
python tools/verify_keyword_import_observations.py
python tools/verify_keyword_import_observations.py --held-out
```

This is a default-3D model subset, accepting already translated text bytes.
Scientific finalization remains explicitly pending, and only captured model
fields are compared. Remaining handlers, dimension changes, full file I/O and
the complete application still require recovery. Model comparisons use numeric
JSON values; they do not establish bitwise floating-storage equality. See
`analysis/keyword-import-contract.json` for evidence, candidate history and limits.

`src/recovered/component_layout.*` now reconstructs the original 79-entry
component-table initialization and refresh calculations. Twenty calls from two
original processes match all 1,580 integer outputs, covering both dimensions,
different output controls and material counts. The candidate was archived before
both captures. All 14 CTest checks pass.

```powershell
python tools/compare_component_layout.py --capture analysis/root-layout-runtime-training.json --capture analysis/root-layout-runtime-held-out.json
python tools/verify_root_layout_observations.py
```

The original dimension setter clears and reinitializes the model when the
dimension changes; a same-dimension header preserves the captured state.
At that checkpoint the importer still rejected dimension changes. Feature-query
results are inputs to the new component, and their material-dependent behavior
has not yet been reconstructed. See `analysis/component-layout-contract.json`
for the exact evidence, coverage and remaining integration work.

The importer now applies the recovered dimension reset and updates a linked
root to the complementary dimension. Six further original imports match both
roots, seven parser-complete object snapshots and 371 descriptor snapshots.
The comparison includes persisted planar boundary nodes and spatial faces.
The candidate remained unchanged after its archive was made before capture.
All 15 CTest checks pass.

```powershell
python tools/compare_dimension_import.py --report analysis/dimension-import-comparison.json
python tools/verify_dimension_import_observations.py
```

Same-dimension headers preserve the mesh; changed dimensions release existing
objects and regenerate the empty-root control table. Scientific finalization,
alternate-root keyword dispatch and populated linked-root evaluation remain
outstanding. See `analysis/dimension-import-contract.json` for this checkpoint.

`MeshStorage::refresh_components` now preserves existing arrays when component
widths change. Allocation counts and arity are tracked separately from object
headers. Descriptor arrays retain their old values and initialize appended
entries; direct buffers retain a known prefix and mark grown tails unspecified.
The refresh replay prints those unknown entries as `null`.

Fourteen original object refreshes match 742 post-refresh descriptor snapshots,
with exact binary64 comparison for known floating values. The capture also
checks empty containers and seven skips of an inactive object. Initial
allocation hooks were added after the first probe missed those extents; the
complete run used the unchanged frozen candidate.
All 16 CTest checks pass.

```powershell
python tools/compare_mesh_refresh.py --report analysis/mesh-refresh-comparison.json
python tools/verify_mesh_refresh_observations.py
```

This storage comparison supplies captured pre-call values as replay inputs.
At that checkpoint `GEOTYP`/`STNOUT` dispatch was not yet implemented.
See `analysis/mesh-refresh-contract.json` for validity rules and coverage.

The keyword importer now connects `GEOTYP` and `STNOUT` to the recovered
configuration state and preserving mesh refresh. It retains the original
partial/error mutations, including zeroed flags after blank or EOF input.
In 3D, `GEOTYP` leaves the following bytes for the outer token scanner.
The initialized model explicitly asserts the empty-material/reset profile;
an arbitrary component table does not establish that profile.

Eight sequential original imports match 26 tokens and 954 descriptor snapshots.
This comparison supplies fixture bytes and the initialized profile, without
seeding arrays or component tables from captured state. Known floating values
are compared bitwise; unspecified direct-buffer tails remain `null`.
The 21 candidate source/binary files were archived before capture and remained
unchanged. All 17 CTest checks pass.

```powershell
python tools/compare_configuration_import.py --report analysis/configuration-import-comparison.json
python tools/verify_configuration_import_observations.py
```

This covers the selected configuration and mesh fields before scientific
finalization. Material-dependent feature queries, the remaining keyword
handlers, malformed 2D `GEOTYP` runtime cases and complete application recovery
remain outstanding. See `analysis/configuration-refresh-contract.json`.

`NSTART` and the selected post-import finalizer are now implemented. A successful
read marks the step for normalization: a nonnegative `n` becomes `-(n+1)`,
then the result is copied to the saved-step field. Failed reads retain their
partial mutations and do not clear an earlier success flag in the same import.
The next import resets that flag before parsing.

Thirteen original imports match 23 tokens and 1,643 descriptor snapshots after
finalization. The cases include blank and EOF input, text with no numerical
conversion, both signed integer limits, and a dimension change after `NSTART`.
Stage observations establish control, material and object processing order
before step normalization. The candidate remained unchanged after its archive
was made before capture. All 18 CTest checks pass.

```powershell
python tools/compare_import_finalization.py --report analysis/import-finalization-comparison.json
python tools/verify_import_finalization_observations.py
```

Finalization completes only for the asserted empty-material profile with active
objects' geometry-processing flag off. Other profiles and unsupported imports
remain explicitly pending. Material reconciliation, enabled geometry processing
and the remaining application workflows still need implementation. See
`analysis/import-finalization-contract.json` for the exact boundary.

`DSMAX` and `DTMAX` now update the recovered displacement/time controls.
Positive values clear the competing primary control and normalize the mode
immediately; nonpositive reads can defer mode normalization until finalization.
The secondary `DTMAX` value survives a later positive `DSMAX`.

The first comparison exposed five numeric-reader mismatches: a lone sign or
decimal point at the end of input reports an error, whereas a following newline
produces a zero-conversion success. The initial candidate and failed report
are preserved. After correction, 16 discovery imports and 13 further imports
match 65 helper calls and 76 immediate handler states. The corrected candidate
remained unchanged during the further capture. A compound planar case also
matches 159 descriptor snapshots and retains unspecified direct-buffer tails.
All 20 CTest checks pass.

```powershell
python tools/compare_time_controls.py --report analysis/time-controls-corrected-training-comparison.json
python tools/compare_time_controls.py --capture analysis/time-controls-runtime-held-out.json --run-record analysis/oracle/time-controls-held-out/run.json --cases analysis/time-controls-held-out-cases.json --fixtures tests/fixtures/time_controls/held_out --freeze analysis/time-controls-corrected-freeze.json --report analysis/time-controls-held-out-comparison.json
python tools/verify_time_controls_observations.py
python tools/verify_time_controls_observations.py --held-out
```

Raw binary64 comparisons include negative zero. The tests cover the existing
finite decimal domain and automatic mode control; table-driven `DTTYPE`,
`DSMAX_NSTEP`, and general numerical-domain equivalence remain outstanding.
See `analysis/time-controls-contract.json` and `analysis/time-table-next-contract.json`.

`DTTYPE` now imports the selected time-control table into the recovered model.
Header failure preserves the old table; body EOF preserves the newly allocated
partial table. Stream reads retain the original matching-failure behavior,
line discard, scalar-mode interaction and dimension reset.

The unchanged candidate matches 25 original imports, 63 handler states and
79 stream-number reads across two processes. Additional cases cover signs and
decimal points at EOF, numbers spanning lines, header commas, signed zero and
a new sequence using an existing planar mesh fixture. That sequence also
matches 159 descriptor snapshots. All 22 CTest checks pass.

```powershell
python tools/compare_time_table.py --report analysis/time-table-comparison.json
python tools/compare_time_table.py --capture analysis/time-table-runtime-held-out.json --run-record analysis/oracle/time-table-held-out/run.json --cases analysis/time-table-held-out-cases.json --fixtures tests/fixtures/time_table/held_out --report analysis/time-table-held-out-comparison.json
python tools/verify_time_table_observations.py
python tools/verify_time_table_observations.py --held-out
```

This establishes selected private rank-zero/rank-one table storage and import,
not interpolation, higher-rank tables or scientific consumers. Existing public
loader interfaces remain unchanged. See `analysis/time-table-contract.json`.

Independent observation audits reject 71 deliberately corrupted cases and
verify the candidate archives and capture provenance. Runtime coverage of the
two preserved opaque metadata fields contains zero values only; their general
preservation follows from static evidence, not these runtime samples.

`PDIE`, `STROKE` and `DSMAX_NSTEP` now form a recovered object-dependent
calculation path. The importer selects an object, loads its three stroke
values (two in 2D), and computes the displacement increment from the absolute
input and stored third value. Inactive objects remain eligible. The original
ignores the mixed reader's error status and defers mode normalization;
the reconstruction retains both behaviors.

The first comparison caught an address-arithmetic mistake: `root+0x1104`
was associated with an unrelated unit-dependent double. The correct reset
routine establishes a separate integer initialized to zero. That model error
was corrected; the initial candidate and failed comparison are retained.
The original public APIs and three-argument internal reset factory remain
unchanged, without an invented unit dependency.

The corrected candidate matches 17 discovery imports and 12 further imports:
95 handlers, 78 line-reader calls, and 2,544 descriptor snapshots. Additional
cases include signed-zero arithmetic, incomplete integer prefixes, retained
stroke values after deactivation and a populated planar sequence. The corrected
candidate was frozen before the further capture. All 24 CTest checks pass.

```powershell
python tools/compare_step_distance.py --report analysis/step-distance-corrected-training-comparison.json
python tools/compare_step_distance.py --capture analysis/step-distance-runtime-held-out.json --run-record analysis/oracle/step-distance-held-out/run.json --cases analysis/step-distance-held-out-cases.json --fixtures tests/fixtures/step_distance/held_out --freeze analysis/step-distance-corrected-freeze.json --report analysis/step-distance-held-out-comparison.json
python tools/verify_step_distance_observations.py
python tools/verify_step_distance_observations.py --held-out
```

See `analysis/step-distance-contract.json` for the error history, evidence and
compatibility limits. Geometry import and its enabled finalization path are
the next recovery step; the complete application remains unfinished.

Independent observation verification covers 39 lookup/calculation pairs and
rejects 82 deliberately corrupted cases. The complete initial capture was
recovered from REA's retained export after exceeding the response transport
limit; that export is archived separately so future sessions can verify it.

## Direct spatial geometry import and plane finalization

`DIEGEO` now imports the recovered direct 3D coordinate/face buffers into
`GeometryStorage` and runs the original plane-grouping sequence during import
finalization. This includes partial row writes, retained lengths for zero or
negative input counts, exact oriented-normal and distance comparisons, first
matching plane selection, classification identifiers, and object cleanup.

The three-field header's optional native output is the conversion count, not a
fourth input field. Exactly two conversions select a separate segmented format
in a 3D model. The alternate segmented/matrix routes were still pending at
the direct-spatial checkpoint; they are now integrated and compared as described
below.

The unchanged 25-file direct-spatial candidate matches 36 original imports:
13 initial, 17 additional, and six corrected lifecycle imports. Comparisons
cover 184 row-reader calls, 1,802 descriptor snapshots, handler state and stream
positions, and exact binary64 coordinate and plane bytes. All 27 CTests pass.
Independent original-evidence checks reject 42 targeted corrupted cases across
those three captures. These counts establish this bounded feature's coverage,
not complete application or universal numerical equivalence.

Two additional fixtures initially placed `OBJTYP` arguments on the next line;
the original rejected those reads before changing object type. That capture and
its inputs remain intact, with the actual error behavior compared. The separate
six-import lifecycle run uses valid same-line arguments and verifies geometry
clearance, retained secondary flags, reactivation and dimension reset. No C++
change was needed after the initial candidate freeze.

Contracts and records:

- `analysis/geometry-import-contract.json` collects verified coverage and limits.
- `analysis/geometry-import-direct-contract.json` records dispatch and storage.
- `analysis/geometry-finalize-static-audit.json` binds numeric and plane evidence.
- `analysis/geometry-import-*-comparison.json` contains the three exact replays.
- `analysis/geometry-import-*-observations.json` contains independent checks.
- `analysis/geometry-import-held-out-fixture-correction.json` preserves the fixture correction.
- `analysis/geometry-format-static-audit.json` records the alternate-format
  static evidence used by the subsequent integration.

Run `python tools/compare_geometry_import.py` for the initial capture and
`python tools/verify_geometry_import_observations.py` for its independent audit.
The auditor accepts `--held-out` and `--lifecycle` for the other captures; CTest
runs all three replacement replays. Historical source and binary snapshots
remain archived. Strict freeze checks apply to their corresponding checkpoint;
later source changes intentionally stop matching older candidate manifests.

Copied source planes, shared native storage, malformed eligible indices,
nonfinite arithmetic, allocation failure, downstream geometry processing and the
remaining application workflows are still open research areas.

## Segmented geometry and transformation matrices

The integrated `DIEGEO` importer now handles the recovered planar and spatial
segmented formats and the transformed format. `GeometrySegments` stores the
first alternate geometry container separately from direct spatial geometry.
`KeywordReader::matrix_row()` preserves the original partial conversions and
line consumption. Existing public database-loader interfaces are unchanged.

The candidate matches 54 original imports: 30 initial cases and 24 independently
prepared additional cases. Comparisons cover 112 line helpers, 207 scalar scans,
2,862 descriptor snapshots, exact segment and matrix binary64 bytes, and
immediate handler and finalized state. The C++ sources and replay binaries were
unchanged between the two captures.

Independent original-evidence audits reject 35 deliberately corrupted cases.
Both capture sets are registered with CTest; all 29 tests pass. Only the CMake
test registrations changed after the 28-file candidate freeze: the other 27
source, driver and library entries remain identical. The historical strict
comparison reports retain the pre-registration hashes.

Observed behavior includes retained trailing segments and repeated angle
conversion in planar imports, spatial clearing and ignored internal errors,
prefix-row consumption, matrix commit timing, and preserved partial state on
failure. Additional cases verify that deactivation restores an identity matrix,
changed dimensions destroy old objects, and direct spatial geometry coexists
with alternate geometry without sharing its payload.

The first comparison attempt exposed an overly strict comparator assumption:
the passive clear hook also runs during object construction and dimension
reset. That diagnostic is retained in
`analysis/geometry-segments-initial-comparator-error.json`. The corrected
comparator checks those lifecycle events separately; no C++ or fixture change
was needed.

Run `python tools/compare_geometry_segments.py` for the initial capture and
`python tools/verify_geometry_segments_observations.py` for the independent
original-evidence audit. The consolidated contract and both capture reports are
listed in `analysis/geometry-segments-contract.json`.

This is a bounded compatibility result. Shared native ownership, additional
geometry containers, allocation failure, unsafe mixed-width retained segments,
general numeric range behavior, and downstream uses of the transformation
matrix remain unresolved. Complete application recovery is still in progress.

## Material name import and lifecycle

The `MTNAME` importer matches 34 original imports across initial and independent
held-out captures. These compare 133 reader calls, 29 name reads, 16 creations,
exact signed material IDs, insertion order, component refresh, and state after
errors and dimension changes. All 27 frozen source and runtime files remained
unchanged through both captures. Independent observation checks reject 39
deliberate corruptions, and all 31 registered tests pass at this checkpoint.

Material IDs accept zero and negative values without object-style placeholders.
Names are limited to 64 stored bytes. A newly created material and its refresh
survive a later name-read failure; an existing name survives that same failure.
The test cases include hexadecimal and octal selectors, signed integer limits,
whitespace, truncation, and missing first or second name lines.

`analysis/material-import-contract.json` records the completed default-material
checkpoint and its limits. Run `python tools/compare_material_import.py` for the
initial comparison; CTest runs both captures. The independent verifier is
`tools/verify_material_import_observations.py`.

This checkpoint covers default materials without objects. Subsequent checkpoints
below cover populated objects and selected material references. Material property
tables remain incomplete. The static reference contract in
`analysis/material-reference-next-contract.json` records retained buffers after
logical-count changes and insertion-order-dependent finalization; these behaviors
are represented explicitly by the later `MSTMTR` implementation.

## Materials with populated objects

Default materials and the recovered object model now support both import orders.
Material insertion updates the component table and refreshes active mesh arrays
before reading the name. Existing object identity, names, geometry, boundaries,
and preserved array values survive; inactive objects are skipped. Existing
materials also permit later object creation with the original placeholder rules.

The unchanged C++ candidate matches 31 original imports across 14 initial and
17 independently prepared cases, including full model snapshots after each
handler. Checks cover 14 temporary-copy lifetimes, 250 token/material reader
calls, 57 geometry rows, 30 segment scalar scans, and 2,915 final descriptor
snapshots. Independent observation audits reject 64 deliberate corruptions.
All 33 registered tests pass. The public database-loader interfaces remain
unchanged.

The first observation exposed missing probe allocation tracking for copied
segments; its seven errors and complete capture remain preserved. A repaired
probe produced complete observations without changing the C++ candidate.
Three initial spatial fixtures omitted element arity and therefore tested
failure behavior. Their original bytes and errors remain recorded. The
independent cases add four successful tetrahedral connectivity imports and
verify material refresh around those meshes, alongside planar elements,
partial reads, inactive objects, dimension changes, and simulation controls.

Run `python tools/compare_material_object_import.py` for the validated initial
capture and `python tools/verify_material_object_observations.py` for its
independent audit. CTest runs both comparison sets. The consolidated record is
`analysis/material-object-contract.json`; test registration and immutable
candidate identities are recorded separately.

This default-material checkpoint relies on owned objects and flags that leave
material-column remapping disabled. The following checkpoint extends reference
handling. Scientific property tables, shared native storage, and allocation
failures still require recovery.

## Material references

The selected `MSTMTR` importer now preserves the original distinction between a
logical reference count and its retained allocation. Successful nonpositive
counts can retain the old buffer; count-read failure can zero the count without
changing the mode or buffer. Positive reads allocate zeroed storage and retain
partial writes on EOF. Finalization processes materials in insertion order and
applies changes immediately, including self-references and cycles.

The frozen C++ implementation matches 24 original-app imports, 39 complete
post-handler states, 43 reference-reader calls, and 1,219 final mesh descriptor
snapshots. The previous 33 regression tests and the newly registered
`material_reference_replay` test pass. An independent original-only audit also
checks allocation lifetimes and finalizer ordering and rejects 22 deliberate
evidence corruptions. The consolidated checkpoint is
`analysis/material-reference-contract.json`. Run
`python tools/compare_material_reference_import.py` to repeat the captured
comparison without launching the original application, or
`python tools/verify_material_reference_observations.py` for the independent audit.

Those initial cases verify reference-driven array widths and refresh behavior
with zero-valued material columns. The following checkpoint adds nonzero column
permutation and seven independently prepared reference cases. Full material
properties and complete application reconstruction remain unfinished.

## Element material assignments and material columns

`MTLGRP`, `VOLFC`, and `VOLFS` now connect element assignments to the recovered
material model. Integer assignment imports mutate live storage, so defaults and
completed rows survive later read failures. Floating column imports use temporary
storage and commit only on success. Both preserve the original row-format cursor
rules, including whether a final line read consumes a following keyword.

The object stores its most frequent element assignment, with first occurrence
breaking ties. A zero result retains the previous stored value; a subsequent
connectivity replacement uses that value only when it is positive. Material
column selection follows the original ordered reference-marking algorithm,
including the special handling of an initial material ID zero.

The frozen implementation matches 48 original imports: 25 initial producer
cases, seven independent reference cases, and 16 independent column cases.
Comparisons cover 108 complete handler states, 46 column-reader calls, and
2,120 final descriptor snapshots. Eight remap checks have nonzero source values;
three-row planar meshes verify row indexing as well as sorted material positions.
The 34 previous regression tests and both new replay tests pass.

The held-out cases include signed zero. Raw binary64 bytes remain authoritative;
the checker permits sign loss only in the probe's redundant JSON numeric display.
No C++ candidate or probe changes were required after either capture.
Independent original-only audits reject 70 applied corruptions across the two
captures. Evidence, frozen-file identities, and coverage limits are consolidated
in `analysis/material-column-contract.json`.

Run `python tools/compare_material_column_import.py` for the initial comparison;
CTest also runs the held-out capture. The independent original-only verifier is
`tools/verify_material_column_observations.py`. Additional mesh fields, material
properties, full scientific operations, exports, and console integration remain
required work. The next static inventory groups 25 related mesh-field keywords
for recovery through shared readers rather than separate implementations.

## Fixed mesh-field imports

The importer now handles the 25 fixed mesh-field keywords listed in
`analysis/mesh-field-contract.json`. Six use integer columns; the remaining
node and element fields use real-reader modes 0, 1, or 6. They write directly
to the current object's arrays, preserving defaults and completed rows after
a later read failure. Mode 6 distinguishes exact-width vector defaults from
broadcast defaults. Explicit and implicit row widths retain their different
end-of-line behavior.

In 3D, `BCCDEF` converts equal negative triples after the reader returns,
including on failure. Its first component subtracts 200 with native 32-bit
wrapping; the other two become zero. The C++ implementation uses unsigned
arithmetic to preserve this behavior without signed-overflow undefined behavior.

The frozen candidate matches 64 original imports: 32 training cases and
32 independently prepared cases. Both sets exercise all 25 keywords in 2D and
3D. Comparison covers 186 complete handler states, 165 field-reader calls,
281 scalar scans and 3,339 final descriptor snapshots. Independent original-only
audits reject 64 deliberate evidence corruptions. The 36 existing regression
tests and both new replay tests pass; the checkpoint records their build order.

The first capture was excluded because the probe retained a recycled Frida
return-value wrapper. The repaired probe copies that pointer. The failed capture,
diagnostic and original input freeze remain available. Neither valid capture
required a C++ candidate or comparison-logic correction. The comparison tool's
defaults now select the repaired capture.

Run `python tools/compare_mesh_field_import.py` for the repaired training comparison,
or use CTest's `mesh_field_reference_replay` and `mesh_field_held_out_replay`.
The independent audit is `tools/verify_mesh_field_observations.py`.

`DVOLF` and `TICF` remain zero-width in the established configuration. Their
positive-width producer, remaining material properties and full application
workflows still require recovery. This checkpoint preserves the public
database-loader API.

## Temperature and dynamic field imports

The importer now handles `NDTMP`, `USRNOD`, `USRELM`, `MATAXI`, `NWEAR` and
`DRMESH` in the bounded owned-storage profile recorded in
`analysis/dynamic-field-contract.json`. The recovered behavior includes 2D
temperature-pair conversion, global refresh when user-field widths change,
64-byte user-field metadata records, and selected node/element allocation.
Equal user widths preserve existing state; growth, shrinkage and clearing
follow the original ordering across multiple objects.

`MATAXI` changes the root's effective width after allocating its physical
array. These widths can differ, so the implementation preserves the physical
tail instead of resizing it to the parsed width. Partial reader failures
retain defaults and completed rows. The planar temperature-pair conversion
also runs after a partial row failure, matching the original.

The frozen implementation matches 64 original imports: 32 training cases and
32 independently prepared cases. Both cohorts exercise all six keywords in
2D and 3D. Comparisons cover 99 complete handler states, 105 dynamic-reader
calls, 212 scalar scans and 6,572 final descriptor snapshots. Independent
original-only audits reject 76 applied evidence corruptions. All 38 existing
tests passed before freezing the candidate; both new replay tests then passed
on those same binaries. No C++ or probe changes were needed after either capture.

Each native capture exposed a 16-byte row-buffer leak on a temperature-pair
EOF path. The C++ reader releases its temporary storage while preserving
observed values, status and stream position. The contract records this
resource-management difference and the checker corrections made during
training. The independent cases required no checker changes.

Run `python tools/compare_dynamic_field_import.py`, or CTest's
`dynamic_field_reference_replay` and `dynamic_field_held_out_replay`.
The independent audit is `tools/verify_dynamic_field_observations.py`.
At that checkpoint, custom `CUSVAR`/`DYNARY` storage remained unresolved;
the following checkpoint recovers its bounded owned-storage behavior.
Additional width producers, material properties, complete DB application-state
import/export, scientific operations and console integration remain unresolved.
The public database-loader API is unchanged.

## Custom data imports

`CUSVAR` and `DYNARY` now populate separate object stores recovered from the
original executable. `CUSVAR` identifies descriptors by exact signed IDs,
derives sizes from explicit rows or mesh counts, and preserves shape history.
Its root selection changes only after successful row loading. `DYNARY` uses
positions and retains independent real and integer buffers when types change.
Defaults, partial writes, inactive objects and dimension resets follow the
observed ordering. A dimension reset clears the stores but retains the root's
last selected custom ID.

The frozen implementation matches 64 original imports across training and
independently prepared inputs. Comparisons cover 80 complete handler states,
118 custom reader calls, 114 scalar scans and 3,286 standard descriptor
snapshots. Both keywords are exercised in 2D and 3D, including nine failed
handlers. Independent original-only audits reject 71 applied evidence
corruptions. All 40 existing tests passed before freezing the candidate;
both new replay tests passed on the same unchanged runtime files.

The first probe failed to capture some row-reader completions and is excluded.
The repaired probe observes return values at verified caller instructions.
Failed captures and diagnostics remain preserved. Neither valid capture
required a C++ correction. Training required two verifier/checker corrections;
the independent cases passed those unchanged tools.

Run `python tools/compare_custom_field_import.py`, or CTest's
`custom_field_reference_replay` and `custom_field_held_out_replay`.
The independent audit is `tools/verify_custom_field_observations.py`.
`analysis/custom-field-contract.json` binds the evidence and limitations.
The established scope covers finite numeric inputs and bounded owned buffers;
arbitrary shared ownership, allocation failures and extreme numeric conversion
remain unverified. This checkpoint preserves the public database-loader API
and does not establish complete application equivalence.

## Material size and texture imports

`SIZEMD` and `TXTURE` now populate recovered material fields, which determine
the widths of element components 54 and 55. `SIZESH` and `TXTODF` load those
element arrays. The implementation preserves the original ordering: creating
a material refreshes layout before reading its property, while changing an
existing property leaves the layout unchanged until a later refresh.
Texture widths follow immediate material references, including duplicates.

Explicit array headers can override root widths. A rejected selector still
changes that width, but does not allocate or write rows. Successful selection
updates saved object widths and allocation history. Defaults, completed rows
before failure, deactivation and resets follow the observed native behavior.

The corrected implementation matches 57 original imports: 32 training cases
and 25 independently prepared cases. Comparisons cover 120 complete handler
states, 85 aggregate readers, 58 scalar scans and 2,438 descriptor snapshots.
All four keywords are exercised in both dimensions. Independent original-only
audits reject 62 applied evidence corruptions. Texture queries up to width
4,784 are covered with no objects; populated mesh tests use bounded small arrays.

Training found one implementation mistake: an unknown-texture diagnostic went
to standard error instead of the original's standard output. That failure and
the incorrect initial review are preserved. The corrected candidate was frozen
before independent evaluation, which passed without further implementation or
checker changes. All 42 existing tests passed after the correction, followed
by both new replay tests on the same 29 unchanged runtime files.

Use CTest's `material_shape_reference_replay` and
`material_shape_held_out_replay`. The comparison tool is
`tools/compare_material_shape_import.py`; the independent audit is
`tools/verify_material_shape_observations.py`.
`analysis/material-shape-contract.json` records evidence, correction history
and limits. The public database-loader API remains unchanged.

The next relation-count producer has been identified as `PHASTF`. Its static
contract records a significant asymmetry: enabling a relation recounts and
refreshes arrays, while disabling one leaves the cached count unchanged.
Its interaction with populated `DVOLF`/`TICF` arrays still requires recovery
and runtime validation at that checkpoint; the following checkpoint supplies it.
Full application reconstruction remains incomplete.

## Relations and volume-fraction fields

`PHASTF` now selects stable relation records by exact ordered signed key pairs.
It creates missing records before reading the mode, including when that read
fails. Enabling a relation recounts active records and refreshes the component
layout. Disabling it leaves the cached count and layout unchanged. List size,
active flags and cached count therefore remain separate state.

With the material feature enabled, that cached count supplies the widths of
`DVOLF` and `TICF`. The original preserves their flat value sequence during
resize. For example, two elements with width one and values `[11, 22]` become
`[11, 22, 0, 0]` at width two. Shrinking removes a suffix. The implementation
preserves this behavior, including allocation history, gate changes and resets.

The frozen implementation matches 62 original imports: 32 training cases and
30 independently authored cases. All three keywords are exercised in both
dimensions. Comparisons cover 106 complete handler states, 96 relation-reader
calls, 25 field readers, 140 scalar scans and 2,332 descriptor snapshots.
Independent original-only audits reject all 78 applied evidence corruptions.
All 44 existing tests passed before freezing; the two new relation tests passed
with the same 29 unchanged runtime files. Neither capture required a C++ or
probe correction.

The initial launch failed before creating a process because of a Windows DLL
relocation error. A bounded run outside the sandbox completed normally; the
failed launch record is preserved. Both valid captures exited normally with
complete observation streams. Comparator and verifier default-path adjustments
are recorded separately from implementation changes.

Run `python tools/compare_relation_import.py`, or CTest's
`relation_reference_replay` and `relation_held_out_replay`.
The independent audit is `tools/verify_relation_observations.py`;
`analysis/relation-contract.json` binds the evidence and limitations.
The public database-loader API is unchanged.

The relation constructor also contains twelve scientific tables with distinct
types and defaults. Their keyword imports are covered by the later relation
property checkpoint below. Scientific consumers, complete DB state import/export
and the remaining application workflows still require reconstruction.

## Material property tables

Thirteen material keywords now populate the recovered table model: `YOUNG`,
`POISON`, `EXPAND`, `THRCND`, `HEATCP`, `EMSVTY`, `ELRST`, `PMEAB`, `PMITT`,
`MASDEN`, `COARSE`, `GBENGY` and `NUCSIZ`. Their shared reader preserves scalar
constants, axis selectors, counts, arrays and metadata. Existing tables survive
header EOF; accepted headers reset them according to the original rules.

Partial reads have different effects. A one-axis table writes values directly
as they arrive. A two-axis table writes its axes directly, but commits dependent
values only after reading the entire temporary array. An incomplete dependent
array therefore leaves the new zero-filled destination intact. Caller-specific
behavior is also preserved: `HEATCP` commits its auxiliary integer even after a
failed read, and `PMEAB` changes its second axis selector even after failure.

The frozen C++ implementation matches 64 original imports: 32 training cases
and 32 independently authored cases. All thirteen keywords are exercised in
both dimensions. Comparisons cover 120 complete handler states, including 111
property handlers, 116 aggregate property readers and 193 scalar reads.
Independent checks reject all 80 applied evidence corruptions. The 46 existing
tests passed before freezing, followed by both new tests with all 31 frozen
runtime files unchanged. No candidate corrections were needed after capture.

Observation tooling required two repairs: a shared continuation was incorrectly
treated as exclusive to temporary arrays, and an allocation hook applied the
selected material bounds to unrelated tables during reset. The invalid capture
and superseded input freezes remain recorded. Both final captures completed
without observation errors. An independent verifier's scratch-cleanup
expectation was also corrected from retained source before both final audits.

Run `python tools/compare_property_table_import.py`, or CTest's
`property_table_reference_replay` and `property_table_held_out_replay`.
The independent audit is `tools/verify_property_table_observations.py`;
`analysis/property-table-contract.json` records the evidence and limits.
The public database-loader API remains unchanged.

These imports reach ranks zero through two with one dependent value per point.
The shared implementation also contains statically recovered type defaults and
higher-rank reading paths; those paths are not runtime-verified by this cohort.
Scientific evaluation, interpolation, table export and the remaining relation
and material handlers still require recovery. The next work is recorded in
`analysis/property-table-follow-on-plan.json`; the complete application remains
unfinished.

## Relation property imports

Nine more keywords now populate the recovered relation model: `TTTD`, `TTTN`,
`PHASLH`, `PHASVL`, `TRNSFP`, `PHEQVF`, `COARSN`, `IPSNRG` and `VARSEL`.
They preserve the original ordered signed key selection, twelve table defaults,
explicit scalar fields, names and partial-failure mutations. These handlers do
not perform the active-relation recount or layout refresh used by `PHASTF`.

`TTTD` selects among thirteen payload forms, including coefficient blocks,
three-axis tables and vectors with several values per point. `TRNSFP` changes
an existing table without resetting it first. Switching it to a scalar can leave
earlier arrays allocated; the replacement preserves that observable data state.
The model consequently keeps current dimensions separate from retained storage.

The frozen implementation matches 64 original imports: 32 training cases and
32 independently prepared cases. Comparisons cover 123 complete handler states,
70 table payload reads, 172 relation header/name/aggregate readers and 182 table
scalar reads. Independent original-only audits cover 113 relation handlers and
reject all 92 applied evidence corruptions, with no skipped checks. Both cohorts
exercise all nine keywords in both dimensions. All 48 existing tests passed
before freezing; both new tests then passed with the same 33 runtime files.
No C++ candidate corrections were needed after either valid capture.

The first capture was invalid because four reset completion observations failed.
The repaired probe observes their source-proven shared join before later caller
writes, while retaining the actual call return address. The exact initial Frida
mechanism remains unknown. The failed capture, original probe and repair audit
are retained. An independent verifier also needed a source-backed correction
for two constructor stores; this did not change the candidate. Both final
captures have complete observation streams and normal original-process exits.

Run `python tools/compare_relation_property_import.py`, or CTest's
`relation_property_reference_replay` and `relation_property_held_out_replay`.
The independent audit is `tools/verify_relation_property_observations.py`;
`analysis/relation-property-contract.json` records the evidence and limits.
The public database-loader API remains unchanged. Native allocation failure,
uninitialized malformed-input behavior, pointer-sharing ABI and memory leaks
are outside this bounded comparison. Interpolation, scientific evaluation and
table export remain unrecovered. The next bounded work is described in
`analysis/relation-property-follow-on-plan.json`; the complete application is
still unfinished.

## Nested material imports

`FSTRES` mode 14 and `GRNDAT` selector 4 now populate their nested material
tables. The model retains the recovered constructor defaults: the stress table
starts as type174 with constant1.0; the grain table starts as type172 with two
axes selected but no allocated payload. This was the first selected-branch
checkpoint; the complete FSTRES import family is covered below. Remaining
GRNDAT selectors still require recovery.

The importer preserves their distinct failure ordering. `FSTRES` writes four
live coefficients and then reads its table even if the coefficient read fails;
the table result determines the handler status. `GRNDAT` sets its selector and
refreshes the component layout after the table read, including after failure.
The tested reset configuration has root864=0, which disables the grain feature;
these comparisons do not establish its enabled behavior.

The unchanged frozen candidate matches 64 original imports, split into 32
training and 32 independently prepared cases. Comparisons cover 95 complete
handler states, including 72 selected nested handlers, 105 outer/coefficient
readers, 66 property readers and 187 table scalar reads. Independent audits
verify 42 refreshes and 50 disabled feature queries, rejecting all 96 applied
evidence corruptions with no skipped checks. All 50 existing tests passed
before freezing; both new tests passed with all 33 runtime files unchanged.
Both captures completed normally without probe errors or repairs.

The independent verifier initially excluded a bare `+` in a partial numeric
line. Its local lexer was extended using the recovered numeric rules, and both
cohorts passed with the same corrected verifier. The initial script, receipt
and diagnostic remain preserved; no C++ or probe correction was needed.

Run `python tools/compare_nested_material_import.py`, or CTest's
`nested_material_reference_replay` and `nested_material_held_out_replay`.
The independent audit is `tools/verify_nested_material_observations.py`;
`analysis/nested-material-contract.json` binds the evidence and limitations.
The database-loader API is unchanged. Scientific evaluation and enabled grain
features remain incomplete. The FSTRES follow-on plan was completed by the
import checkpoint below; remaining GRNDAT branches are still pending.


## Complete FSTRES import family

The internal importer now reconstructs all sixteen positive `FSTRES` modes,
plus zero, negative and unknown positive selectors, within the recovered
finite, initialized input domain. Its model owns ten independent property
tables, 42 coefficient doubles and three integer fields. Constructor defaults,
physical table allocations and retained buffers are represented explicitly;
this recovers imported state, not the stress-law calculations that consume it.

The implementation preserves branch-specific failure order. Mode 9 executes
both seven-value reads and returns the second status; mode 10 consumes seven
values but stores six. Modes 13 and 15 derive subsequent table widths from the
first table's axis coordinates, using their ordered integer sum and product.
Negative counts normalize when allocation runs, while rank-zero widths at or
below one preserve existing storage and consume no payload. Derived integer
conversion and arithmetic are limited to the established finite, non-overflowing
domain. Mode 14 retains its previously verified coefficient/table behavior.

All four captures passed their first strict comparison with the same frozen
35-file candidate: 48 training imports and 48 independently prepared held-out
imports. They cover 117 complete handler snapshots, 106 FSTRES handlers,
228 fixed-line helper calls, 93 direct table payloads, 99 typed resets and
312 property scalar reads. The 36 FSTRES failures are expected native reader
results whose partial state and final status match the replacement.

Independent original-only audits cover 108 selected handlers, including two
GRNDAT handlers, and reject 188 applied evidence corruptions. Four checks were
not applicable because the two B batches contained no temporary commit/free
events; those checks ran in the A batches. The independent verifier's initial
allocation-attribution error was corrected using the recorded wrapper and
child serials. Its initial version and diagnostic are retained. Two launch
attempts expired before fixture input; their successful retries used the same
frozen inputs. No C++ or probe correction was required.

The 52 existing tests passed before freezing, and all four new replay tests
passed afterward, giving 56 registered tests. This is a baseline-plus-new-test
result, not a claim that all 56 ran in one invocation. Run the four new tests
with `ctest --test-dir build -C Release -R fstres --output-on-failure`.
`analysis/fstres-contract.json` binds all comparisons, independent audits,
inputs, diagnostics and test receipts. The public database-loader API is unchanged.

The full application remains incomplete. The remaining `GRNDAT` import
branches now have reviewed static contracts from Chat A and a bounded
implementation with native comparisons described below. See
`analysis/chat-a-grndat-contract.json` and `analysis/worker-integration.md`.
Scientific evaluation, enabled grain features, complete DB state import/export
and console workflows remain separate recovery work.

## Paused checkpoint and local evidence

The user resumed recovery after reconciling the single-chat checkpoint.
The subsequent A/B/C research and independent reviews are integrated
and pushed at c6388aa. GRNDAT, keyword metadata and DB mapping research is
recorded separately from implementation in `analysis/recovery-feature-ledger.json`.
The 123 GRNDAT cases now pass bounded native replay. The 17 keyword metadata actions
pass replacement replay, and bounded native metadata comparisons pass (below). Reviewed evidence
lookup tooling is also integrated; it does not establish whole-task speedup.

This chat is the sole owner of recovery, implementation, tests and integration.
Chat A, Chat B, Chat C and Bottleneck are archived; both schedules are paused.
No regular checks or dispatch to those chats are required. The full-app goal is
unchanged and active following the user's resume instruction. Implementation
follows `analysis/recovery-plan.json`.

This Git checkpoint includes the recovered C++ sources, replay programs,
synthetic keyword fixtures, observer/comparison tools, compact contracts and
audit receipts. Large runtime captures, comparison snapshots and new native
decompiler dumps remain local under `analysis/`; provider snapshots, original
process records and frozen candidate archives remain under `analysis/oracle/`.
No local evidence was deleted. `analysis/local-evidence-manifest.json` records
the hashes and sizes of the newly excluded top-level evidence files; it does
not inventory the separate oracle archive.

A clean checkout can build the C++ targets. Most reference replay tests also
require the local evidence and oracle archives at their recorded relative
paths, plus the reference executable and any external database fixtures used
by the corresponding test. The published repository alone is not a complete
standalone reproduction of all 56 local tests. On this machine, use the
original checkout as the evidence source when preparing isolated worktrees;
do not copy its `.git` directory or mutate its evidence files.

## Root keyword metadata checkpoint

TITLE, CURSIM, OPRNAM and SIMNAM now populate owned root metadata. Changed
dimension headers reset their defaults; same-dimension and later headers retain
values. The existing bounded readers preserve string EOF state and CURSIM partial
assignments. The database-loader API is unchanged.

All 17 reviewed action cases pass replacement replay with 49 handler snapshots
and 980 assertions. Passive REA observations cover 51 original console imports
(including setup/reset imports) and 82 metadata handlers across three processes.
The held-out batch adds 11 imports and 13 handlers within those totals.
`analysis/metadata-import-contract.json` records digests, comparisons and gaps.
Linked nondefault seeds remain static/replacement coverage; absolute native
stream positions are unknown after EOF resets its CRT buffer pointer.
The retained full export stops explicitly at unsupported SMODE.

Run `ctest --test-dir build -C Release -R metadata --output-on-failure`.
The static fixture test uses tracked data; three native replay tests require
local ignored captures and run records named in the contract. No original
application process runs during these replay tests.

## GRNDAT import checkpoint

The disabled-grain-profile GRNDAT importer now handles all selector branches,
including nine-table headers/payloads, embedded and appended records, five owned
lists, six coefficients, negative selector bit arithmetic and partial failures.
The existing selector4 behavior remains covered. The public loader API is unchanged.

All 123 prepared cases pass replacement status/cursor checks. Native comparisons
cover those cases and six new evaluation actions across 23 processes: 636 console
imports including resets/setup, and 720 GRNDAT handlers. Known raw fields and
complete table/list state match. The native action refresh calls and the count-clamp
stdout diagnostic also match the reviewed expectations. All 84 registered tests pass.

`analysis/grndat-import-contract.json` records source digests, per-batch results
and limitations. Enabled grain flags, scientific evaluation/export, allocation
failure, pointer ABI and native scratch leaks remain unverified. Native EOF
buffer resets leave 64 absolute handler cursor positions unobserved; state and
status are still compared. The six new evaluation actions used the frozen
candidate; the previously exercised reserved cases are not called held-out.

Run `ctest --test-dir build -C Release -R grndat --output-on-failure`. Native
replay tests require the ignored runtime captures and private run directories
listed in the contract, including retained observer-source.js. Replay tests
do not run the original application. Future capture preparation uses
`tools/prepare_grndat_batches.py` in a fresh build workspace and the bounded
console harness `tools/run_grndat_batch.py`; it requires a passive REA observer.
The next blocker in the retained keyword export remains SMODE.
