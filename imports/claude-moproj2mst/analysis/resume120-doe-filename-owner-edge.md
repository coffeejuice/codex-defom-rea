# CP120 native InputTable filename owner edge

The native filename producer now closes a useful ownership fact: under the normal nonempty final-append profile, the complete InputTable filename has fresh QString data with ref1 before it is passed to the outer reader. The reader then makes explicit filename leases, including a QFile private filename that survives the file's close and remains live during Geometry parsing. The actual native path also necessarily contains backslashes. This is stronger than a conditional path-value projection, but it still does not establish the total reference word, process shared-null owner or handler state at Geometry entry. No private Geometry admission is source-closed at the candidate's current post-read value-only entry.

## Native construction and fresh data

Original GUI PE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

New bounded `0x1400d3970..0x1400d3c06` shows the manager's InputTable loader constructing:

```text
supplied studies directory + QDir separator + manager.name
    + QDir separator + "Control" + QDir separator + "DoeInputTable.XML"
```

`0x1400ab770` copy-constructs the manager name from manager+0x98. `0x1409094f0` constructs the `Control` literal; the terminal literal is at `0x142166bd0`. QtCore's QDir::separator export `0x670ab970` explicitly writes `0x005c`. Three native separators are appended regardless of the original directory's spelling, so this producer's filename contains backslashes. A candidate path spelled with forward slashes does not make the original use the no-backslash QFileInfo branch.

The two glue functions are independently disassembled:

- `0x140004970` copy-constructs the left QString, appends the QChar, copy-constructs its result and destroys its local.
- `0x1400048f0` does the same with a right QString. Its imported QString operator+= resolves to QtCore export `0x6701e180`, which tail-jumps directly to `0x67070410`. The last call at `0x1400d3b2f` appends the terminal filename to the still-live preceding path, returning the completed filename in `[rsp+0x20]`. The character operator+= export `0x670208f0` explicitly grows the string and writes/increments its length for each separator append.

On the bounded normal profile where this final right string is nonempty/non-shared-null and sizes do not overflow, the copied left data is shared with the still-live preceding temporary. Qt QString::append at `0x67070410` therefore takes the detach/reallocation path (`ref != 1`), not an in-place unshared append. Reallocation `0x670701a0` allocates a new data block, copies the old content, explicitly stores its ref1 at `0x67070325`, releases the old data lease, then publishes the new pointer. The glue function's result copy and local destruction leave the returned full filename as one owner of that new data P. Subsequent destruction of the earlier path temporaries does not delete P.

This is a source-qualified **conditional fresh-data origin**, not an unconditional observation of a live ref word. Terminal C-string decoding still uses the process's QString codec behavior; its normal nonempty result must not be assumed from a host UTF-16 value. Allocation failure, pathological overflow and other decoder branches remain outside this normal profile. The new fact is the data origin/ref initialization after this proven append branch, not a fabricated post-read count.

## Explicit leases and lifetime through the two readers

The normal source has these mandatory P owners at Geometry entry:

| Owner | Acquisition | Lifetime evidenced |
| --- | --- | --- |
| Complete filename local in D3970 | final append/result at `0x1400d3b2f` | remains until after outer reader returns, destroyed at `0x1400d3be0` |
| Outer `0x14090e080` by-value filename argument | copy at `0x1400d3bb1`, call at `0x1400d3bcf` | callee destroys it at `0x14090edfc` |
| QFile private filename `+0xe8` | QFile ctor at `0x14090e0c6`; QtCore ctor `0x670b5c70` assigns it through `0x670703d0` at `0x670b5cd5` | QFile wrapper/private survives through DOM traversal, destroyed at `0x14090ede1` |
| Variable reader `0x14090ee20` by-value filename argument | copy at `0x14090e467`, call at `0x14090e48d` | callee destroys it at `0x1409129eb` |

The actual QString copy constructor export `0x6702b6d0` copies the same data pointer then atomically increments its reference word. Assignment `0x670703d0` increments the incoming data, decrements/releases the previous member value, then publishes the incoming pointer. These are actual aliases of P, not independent text allocations or `shared_ptr` counts.

The four owners above are a local lease ledger, **not** a proof that P's total live count equals4. QFile engine work, parsing and callbacks have already occurred before Geometry and have not been closed as a complete alias inventory. Unknown additional aliases must not be silently set to zero. Nor can process shared-null's live count be derived by summing just these P owners: default/error/cache strings and engine paths touch the canonical shared-null independently.

The file-handle timing is precise: after successful DOM loading, `0x14090e340` calls QFile::close **before** walking DOM children. The QFile object itself remains until `0x14090ede1`. QtCore close `0x670b5f30` closes its engine through virtual+0x10 but does not delete the private object, clear its filename+0xe8 or free the engine pointer+0xf0 in the captured close body. Thus Geometry sees a closed-file wrapper/filename/engine lifetime, not an open input handle. Retaining a candidate openReadFile handle through parsing would extend the native handle lifetime incorrectly.

## Handler and shared-null state is reached before Geometry

Native QFile::open export `0x670b4ae0` invokes its actual engine selector through virtual+0xd8. The constructor-installed QFile vtable `0x67229d18` binds that slot to `0x670b4800`. It lazily creates engine+0xf0 from filename+0xe8 through `0x670a7620`.

That engine creator constructs a QFileSystemEntry at `0x670a7642 -> 0x670eed10`, then calls `0x670f0080` at `0x670a765a`. This is the same resolver family later used by QFileInfo. Entry construction normalizes separators and acquires actual QString shared-null for the native-path slot. Resolver `0x670efbc0` reaches `0x670a7dd0`, whose first state gate reads process byte `0x672f3ed0`; nonzero traverses actual handler/lock owners and calls handlers. A successful native InputTable read does not identify which branch/handler ran or which aliases it retained.

Consequently, shared-null and handler state at `0x140911542` are not pristine library-initial values. The preceding InputTable QFile open/parse/close has already used them. A new Geometry-only context created after byte reading cannot assume handler-empty, shared-null ref1 or a QFileInfo-normal-return receipt.

At Geometry, QFileInfo constructs another private object and normalizes the **same P filename**. Because this producer has backslashes, `0x670ad700` uses its copy/detach/replace branch, not CP80's resource-path same-data shortcut. The branch acquires P temporarily, detaches to normalized data, replaces backslashes, copy-publishes the normalized result and releases its temporary. File-entry construction normalizes/copies that result again and installs shared-null before its own handler call. These stages must preserve their actual data identities; one P/ref delta table borrowed from the no-backslash icon resource is wrong for this caller.

## Candidate comparison and first admissible cut

Current `InputTableSource` owns only `qt::String fileName`, a host UTF-16 value plus null state. `readLazyStudy` computes that value, `StudyIo::readFile` opens/reads/closes its local `ifstream`, then passes the source value to `readInputTableFromSource`. This safely preserves path text and per-call lifetime, as CP105 established. It does not retain native P identity, the closed QFile private filename lease, engine state, normalization allocation or process shared-null/handler owner. Candidate close-before-parse is compatible with the native **handle** order; missing closed-object/QString ownership must not be misreported as a required open-handle retention.

There are two distinct answers to “first source-closed profile”:

1. **Producer-side private owner profile:** normal successful, bounded nonempty final-suffix append, with a live retained left temporary, yields a new filename data owner P/ref1 at D3970's completed-concatenation phase. That owner can be tracked through the actual copy/assignment edges above. This is the first newly source-closed owner cut, before the input file engine opens; it is not another lexical parent helper. Any implementation must construct this owner from the actual producer, not adopt a matching string from arbitrary bytes/named-source callers.
2. **Geometry-entry admission:** not yet closed. It additionally needs a same-invocation closed-input QFile/engine context, the complete applicable P aliases/current ref state, canonical process shared-null owner/state and reached handler decision. The isolated QStringPathOwner explicitly refuses unknown counts; neither the four-owner ledger nor a “read succeeded” Boolean can bypass that rule. CP82 resource-null-codec origin is a different producer and cannot certify this filename.

A useful future private read context would therefore begin before the actual filename/read operation, own the producer-origin QString leases, record the file close phase while keeping the closed-file identity alive, and live through variable parsing/unwind. It would remain distinct from the public bytes-only API and should not store a context on Study or silently hold the OS file open. Root must assign StudyIo/private reader interfaces before such work. Do not add a new detached “success” profile/helper merely to relabel the current Geometry guard.

## Exact next evidence edge and limits

The smallest remaining source investigation is the canonical handler flag/list **producer** and its applicable InputTable engine branch before `0x670a765a`, together with the retained closed engine's filename aliases. That must establish or explicitly guard the no-handler/native-engine profile for this same read; it cannot be inferred from an absolute path or successful parse. Process shared-null remains its own canonical tracked owner. Once those state producers are bound, the freshly allocated normalized QFileInfo data and its cleanup can be integrated on the real filename owner; numeric/PointPair parsing remains the later CP119 boundary. No GUI GEO loader is required by this XML read.

New analysis-only captures include `resume120-doe-input-filename-producer-static.json`, `resume120-doe-filename-string-glue-static.json`, `resume120-doe-qfile-qstring-static.json`, `resume120-doe-copy-realloc-v2-static.json`, the QFile engine/first-handler/close captures, native-separator and append-operator exports/captures, and compact export/literal receipts. The initial copy-realloc capture stopped when a leaf had no unwind entry; its partial outputs are retained historically and the v2 receipt supplies explicit leaf bounds. Bytes beyond selected leaf RET/JMP boundaries are excluded. Final `resume120-doe-filename-owner-edge-capture-index-v2.json` binds all analysis evidence, including the earlier index; unwind spans are not alone proof of entire logical procedures. Existing CP104/50/79/80/113 receipts retain their source qualifications.

No source/test/CMake edit, compiler/build, GUI/original execution or mutable REA database access occurred.
