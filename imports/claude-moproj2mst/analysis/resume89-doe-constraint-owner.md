# CP89 ownership evidence and CP90 matched-Constraint cut

The matched Constraint route has a concrete non-GUI record owner: a list field at native manager+0x18 owns separately allocated0x28 value records. Constructor, populated fields, append/copy, final-owner clear and exception rollback are recovered. The smallest next implementation is an **actual Study-owned bounded record import**, rather than another no-row dispatcher. Its source-unwritten eight-byte field+0x10 must remain explicitly unknown. Existing Save/output constraint consumers stay guarded before serialization/use.

This is read-only evidence work. No source, build/test, GUI/native process or mutable database operation occurred. It complements `resume89-doe-post-constraint-next.md`, which correctly found no additional no-row continuation work. Root confirmed CP89 freeze and authorized the CP90 source cut after this report.

## Original caller and record fields

Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained completed `0x140912d10` reader in `wave2-doe-lazy-input-empty-readers-decompile.json` is bound to this PE. Immutable literals are: Id at `0x1427bb5fc`, Value at `0x1427bb7d4`, ObjectNo at `0x1427bb7e0`, RegionNo at `0x1427bb7f0`. The auxiliary text literal `0x1427ba22f` is the empty C string.

For each exact direct Constraint row the source reads those attributes, converts Id/ObjectNo/RegionNo to int and Value to double, creates a non-null empty text QString, selects category and calls manager vt+8. Category is1 for Id1081 (`0x439`),0 for Id1010/1012/1080 (`0x3f2/0x3f4/0x438`), otherwise2. It ignores other attributes/nested children. No QWidget, QFileInfo or geometry-object lookup occurs here.

Both manager vtables bind vt+8 to `0x14091e110`. Its record is:

| Offset | Initialization/assignment before append |
| --- | --- |
| +0x00, signed32 | category argument |
| +0x04, signed32 | Id argument |
| +0x08, signed32 | constructor default1, overwritten by parsed ObjectNo |
| +0x0c, signed32 | constructor default1, overwritten by parsed RegionNo |
| +0x10, eight bytes | **not initialized by this constructor or append setter** |
| +0x18, double bits | parsed Value argument |
| +0x20, QString | default-constructed by ctor, then assigned explicit empty text argument |

Ctor `0x14091fab0` constructs only QString+0x20 and stores the two default1 values. It does not initialize category, ID, either double or padding beyond that. `0x14091e110` then performs the known stores above, calls append helper `0x14091fb60` on manager+0x18, returns count-1, destroys its local record and destroys the by-value text argument. The reader ignores the returned index and advances to the next row.

The native copy constructor `0x140919aa0` copies all four int fields, both eight-byte fields+0x10/+0x18 and copy-constructs QString+0x20. Thus +0x10 is not merely padding: source **copies unwritten bits using movsd**, without arithmetic or a default0. It must not become zero, NaN, parsed Value or a guessed computed-result field.

## Append owner and normal release

`0x14091fb60` compares the list data's QBasicAtomicInt against1 (IAT `0x141e042d8`, operator!=). It has detach/grow and unshared append paths. The unshared path calls imported QListData::append (`0x141e04200`) to obtain a node slot. Node construction `0x14091ffc0` requests0x28 raw bytes, calls record copy ctor on non-null allocation and stores that pointer in the node. No QObject/virtual GUI object is built.

Normal append preserves source row order and duplicates as separate allocations. It does not deduplicate by Id, ObjectNo, RegionNo or value. A raw allocation that returns null follows an explicit null-node store path; no actual heap-outcome observation is available, so a normal host allocation must not be called proof of that failure branch.

Clear vt+0x20 -> `0x14091e2a0` operates on the same manager+0x18 field through `0x140919000`. It replaces the field with an empty list using assignment `0x1409190e0`: ref incoming data, deref old data, conditionally free old final-owner data, publish incoming data, then preserve its detach state. A shared list release is not unconditional record death.

When old data reaches its final owner, `0x140919290` computes begin/end slots, invokes `0x140919400` and qFrees list data. The record cleanup walks **end to begin**, skips null node pointers, calls `0x140919600(flag1)` -> record QString destructor `0x140919640`, then raw release helper `0x1414dfc68`. Neither record destructor reads +0x10 or calls a geometry/UI destructor. The record has no Study/Session back edge.

These are the original ownership rules. CP90 can represent actual Study-owned model values; it must not label its host vector/ref/destruction events as original QList/QString counter or allocator parity. No current native list-ref observation exists, so literal COW/native release replay remains guarded/unrepresented, and host cleanup is adapter cleanup only.

## Exception and partial-effect contract

MSVC FuncInfo for append `0x14091fb60` is `0x1427bed28`. It has three catch-all handlers. The selected dynamic node branches use handlers `0x1419ee9e0` and `0x1419eea20`: each decrements the newly grown list end then rethrows. Node-construction FuncInfo `0x1427beda0` has raw-allocation cleanup `0x141a95da0` (and the analogous placement path); this releases the newly allocated raw block if record copy fails.

Outer append FuncInfo `0x1427beaf0` unwinds local record with `0x1419ee490` and by-value text with `0x1419ee470`. Prior records remain in the manager. There is no whole-input transaction/rollback. A later unsupported row or sibling must retain earlier clear/appends; a failed current append must not publish a completed current record or increment the count projection. Do not infer success from requested allocation bytes or invent a native failure schedule.

Normal host record/string allocation failures can propagate while preserving prior committed model records. Their exact original Qt allocation/fault timing remains outside the value model. Native raw-null-node and COW-ref branches must not be fabricated as deterministic profile defaults or treated as successful ordinary records.

## CP90 implementable cut

Bind concrete records to the existing actual Study, with stable per-row identity and no owner cycle. Use a vector of unique_ptr to a concrete value record; no deletion through an unrelated base type. Keep `Study::constraints` as the existing compatibility count projection, synchronized from committed owned records after each append and reset at the existing clear encounter. Do not create a detached registry or return a public Save-ready token.

Exact +0x10 representation: `std::optional<std::uint64_t> sourceField10Bits`, initialized and kept **nullopt** by this import path, with explicit source-unwritten/copied provenance. Copies retain unknownness. It is not host storage for arbitrary uninitialized bytes and is never read as a double. A future consumer requiring it must guard until a source write/observation exists. Record host size need not equal0x28; that is a native allocation-size fact, not an ABI claim.

The first public cut should admit only rows with all four exact attributes present, canonical unsigned decimal positive int32 Id/ObjectNo/RegionNo, and exact `Value="0"`. This is a **reconstruction admission bound**, not native validation. Native permits other strings/defaults, but general QString::toDouble/default-locale equivalence is not newly recovered. Exact zero avoids that ambiguity; category selection is source-closed for all admitted IDs. Use source-equivalent non-null empty text. Committed fields are final known values after native constructor/setters; do not claim the constructor alone initialized the unspecified fields.

After clear, each admitted row creates and appends one actual record, updates the count, then continues the reader. Repeated constraints lists clear prior model records at encounter before reading new rows; explicit reverse host removal can preserve the recovered normal destruction order without claiming native COW release. Unknown rows still skip. An unadmitted matched row retains the old unsupported guard and all earlier row/sibling effects. Matched DoeVariable remains guarded as before. Normal successful child completion still reaches the existing outer ID pass and lazy sample effects.

Existing writers/output already guard positive constraints before consuming record payload. Keep those guards. The actual public Type2 Save helper opens/truncates InputTable XML first, then refuses constraint serialization before body bytes; it records no normal writer return or refresh boundary for that interrupted serialization. The legacy writeInputTable planner has an earlier guard, which must not be confused with that immediate public device path. This is real owner-bound record/state progress, not full constraint parsing/serialization, output geometry, refresh receiver or public Save completion. Do not add general native allocator-failure emulation to this slice.

## Exclusive files and focused tests

Proposed DOE-only source set, to announce before edits:

- New `include/m2m/doe_constraint_record.hpp`: concrete committed record fields and explicit unknown+0x10 value representation.
- Existing `include/m2m/doe_study.hpp`: actual Study-owned concrete records; legacy count remains a synchronized projection.
- New `src/doe/constraint_record.cpp` if needed for bounded attribute admission/category/append helpers; no separate private registry.
- Existing `src/doe/doe_study.cpp`: clear/append at original encounters, preserving other row guards and outer finalizers.
- New `tests/doe/test_constraint_record.cpp`: source fields/category, stable duplicate identity, unknown10, clear/replacement and public owner/guard/partial-effect tests.

Existing `<Constraint/>` guard tests lack the four required attributes and remain guarded, so no wholesale expectation rewrite is justified. Root owns build integration through existing globs; no CMake/shared Session/writer/ABI edit is needed.

Focused tests should exercise categories0/1/2, zero value/non-null empty text, nonzero/decimal/missing/overflow attributes retaining the matched-row guard, unknown10 never becoming0, duplicate rows with distinct owners, earlier rows retained before later unadmitted row, earlier/later matched-variable guard timing, clear/repeated list order, reindex/outer-ID timing, actual Session-owned lazy read/sample continuation, and same-owner positive-constraint Save retaining its existing open/truncate then serialization guard, with no body bytes/normal writer return/refresh boundary. Use explicit recording/scratch setup and report source-backed synthetic fields rather than original constraint oracle parity.

## Selected immutable byte identities

Read-only DUMPBIN `/disasm:bytes` viewed complete selected normal bodies/RET boundaries. PE functions can be reproduced by these starts/lengths; no analysis database was opened.

| Entry / bytes | Raw SHA-256 |
| --- | --- |
| `0x14091e110`,193 | `684078887dcdc0cb99a45f8c38088667d362dc8a0cbf047fe5b1b992f24a593e` |
| `0x14091fab0`,59 | `469ff9986e447b72eb5ada9f2ed726cdce2e1e0350330ac1ef4ceda09c245e7f` |
| `0x140919aa0`,150 | `94d9e8ebacc93652cae1c31477ca3c4ca29fad9500a7ca2af9b1f86aa99ac6ab` |
| `0x14091fb60`,209 | `086baa71481a35a151da06a92a5590fd55a10038440194c75ef591051c76155d` |
| `0x14091ffc0`,227 | `84422f83687bdd1d37a83957a1e1acdcb7c50cf2fdb35b8e3b1e7a86ace69ad6` |
| `0x14091e2a0`,28 | `62e35cbe678752848ddb9111703b677307d849aa01b4bd8284cc78e4df127f09` |
| `0x1409190e0`,146 | `b5c14b9413263c4dfe60ed65e20ba8cf0a7ce20f6f8dac90c37494c9703e6c3c` |
| `0x140919400`,170 | `3db7c1c11039c1dc0babdc5bf5750b88921c7645e35975c66be6513fb5467472` |
| `0x140919600`,54 | `04793dba565dc6e1fb55cbd066be3933f20a95c2bac961cdc3efe08c9ea1674e` |

The report closes a bounded normal-value owner/import contract. Literal native refcount/allocator replay, general numeric locale, uninitialized-field consumers and output serialization remain exact separate dependencies.
