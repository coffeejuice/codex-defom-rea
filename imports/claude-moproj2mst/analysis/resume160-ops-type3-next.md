# CP160 ROPS: FRCROT table-reader successor and Type3 owner correction

The smallest new source-closed cut is a **private FRCROT table-read mutation projection**, beyond CP144's prepared success/failure flag. The original table reader now establishes actual ordered axis/value writes and EOF prefixes. It also rules out a later table-read failure for mode 0 and for the reset-only unknown-mode branch. This does not admit FRCROT through the actual Keyword owner, production conversion, or Save. Type3 owner integration remains unclosed; earlier receipts overstate which native bodies are missing.

Only analysis files were written. No application source/test edits, builds, GUI/app-control session, target process, live REA retry or mutable database occurred. `admin/check_continuation.py` returned `CONTINUE`, with 15 items remaining. Binary: immutable original `evidence/original-binaries/DEF_GUI_MO_64.exe`, original DEFORM v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable or deployment is discussed.

## New native FRCROT effects

The decoded FRCROT arm at `140b67701..140b67843` first obtains the mode/value result, returns group-parser status 2 on its early failure, and otherwise resets pair+240 through `14089b9e0(table,0x16)` before the existing mode writes. All ranges in this report are end-exclusive.

`14089b9e0` saves table+54 and table+60, calls `140899010(table,0x16)`, then restores those two metadata fields. The initializer clears the constant at +8, writes dimension +4=0 and value-column count +40=1, clears the three axis codes/counts, then dispatches template 0x16 through byte `14089a0bb=0x5c` and jump target `140899f38+0x5c*4 ->140899f06`. That is the common/default tail; it adds no template-specific stores. The tail calls buffer initialization `14089b840`. Buffer release is reached during initialization; this is not a transaction preserving old payload.

Consequently the state reaching `140af73f0(file,table,0)` is:

| Mode | Dimension | Table constant | Axis code[0] | Axis count[0] | Value columns |
| --- | ---: | --- | ---: | ---: | ---: |
| 0 | 0 | supplied parsed double | 0 | 0 | 1 |
| 1 | 1 | reset 0.0 | 1 | prepared result of native `cvttsd2si` | 1 |
| 2 | 1 | reset 0.0 | 4 | prepared result of native `cvttsd2si` | 1 |
| other | 0 | reset 0.0 | 0 | 0 | 1 |

At `140af742b..140af744e`, table dimension 0 with value-column count <=1 returns **0 before allocation, numeric scanning, or the trailing line read**. Thus mode 0 and unknown modes consume no additional table input and cannot return table-reader status 1 on this ordinary path. CP144's mode-0/table-failure test is still an explicitly prepared-state test; its arbitrary boolean is not evidence that this combination occurs in the native parser. Do not turn that test into a real-input claim or silently generalize it.

For modes 1/2, the reader calls `14089b840`, which releases old buffers, clamps negative axis counts to zero through `14089b970`, and ensures value-column count >=1. For positive count n, the allocator obtains axis[0] and value storage and explicitly clears each through byte-fill helper `141471850`. Native allocation-null behavior is not repaired here: subsequent numeric-scan destinations are used without a new null check. A bounded candidate projection must require successful allocation rather than manufacture a safe error return.

The dimension-1 loop `140af7531..140af7628` visits row i in ascending order. It scans axis[0][i] first (`140af757f`), then each value column (`140af7608`, one column in this FRCROT profile). All five formats selected by the full reader are distinct NUL-terminated `%Flf` literals at `1427e89dc/e4/ec/f4/fc`. Calls use the same varargs scan wrapper `141472ac8`. **Only returned -1 causes table status 1.** Return 0 advances the loop without an assigned value; it is not promoted to an error. An assigned slot is changed immediately, and earlier slots remain changed if a later scan returns -1. Axis failure skips its value scan; value failure retains that row's already assigned axis. The zero-filled remaining slots are retained on the admitted successful-allocation path.

After all rows, `140af77b8` asks `14089bc90` for the total count. For dimension 1 and one value column this is n, clamped to nonnegative by the earlier axis-count normalization. Positive n reaches the single bounded line-read call `1414733ac(buffer,0x400,file)` at `140af77db`; its return is ignored and the table function returns 0. Count zero performs no scans or trailing line read and also returns 0. No file-position or exact token-consumption claim is inferred from prepared scan results.

The common group-parser tail still runs after table status 1. It performs reverse identity/direction work and calls `1408e5d60`; only afterward does `140b68f67` map a nonzero table status to group-parser result 2. This is the existing CP144 order, now with a newly qualified source of the saved table status and partial payload.

## Smallest implementation contract

Add a new private table suffix helper; keep CP144 `applyFrcrotPairPrefix` and public callsites unchanged. Its inputs are a successfully decoded mode/value, the already qualified integer-cast result, and an ordered sequence of scanner observations `{returnCode, assignedDouble}`. It must not accept a substitute final table-success boolean. Return code -1 stops; 0 leaves the zero slot and continues; 1 stores the supplied double. Do not parse text or claim CRT numeric/locale/stream parity in this increment.

An initial finite candidate profile can admit modes 0/unknown and modes 1/2 with normalized counts 0..7, successful native-like buffer allocation and normal helper returns. The bound is a candidate restriction, not an original limit: both n-element buffers have at most 56 bytes, so their zero initialization uses the fully decoded small-fill path `141471850..1414718bc` and avoids its uncaptured larger-fill branch. Reject count >7, missing scan observations, invalid observation shapes or unqualified allocation failures as **candidate unsupported**, rather than synthesizing native success/failure. Negative prepared counts normalize to zero in the helper. Do not add floating-point conversion behavior; retain the existing prepared-cast boundary.

Output should expose dimension, code, normalized count, constant, axis/value vectors, scan attempts and destination order, zero/no-write/assigned slot effects, whether the trailing line-read boundary was reached, and native table status 0/1. Metadata +54/+60 remains opaque/preserved; no host pointers or native ownership claims. Tests should cover mode0/unknown consuming zero observations; count0 and negative-count normalization; modes1/2 code distinction; X0,Y0,X1,Y1 order; EOF at X1 vs Y1 retaining different prefixes; scan return0 continuing with zero slots; and final ignored line-read result. These are new table-effect tests, not a repetition of CP144 pair creation/reverse-order tests.

## Mirror payload remains a distinct boundary

New body `1408e5d60..1408e5ddd` saves the destination pair's +168/+16c/+170 IDs/direction, invokes `1408e57e0(destination,source)`, then restores them **on normal return**. No exception restoration is shown. Its delegated copy is not a blanket payload memcpy: at `1408e58ac..1408e58fb` it queries `140065560(destination+398)` and copies source+240 through `14089ad60` only if that result is true. Other feature-dependent fields and calls occur earlier/later. Therefore a table projection cannot claim it has produced reverse FRCROT payload or scratch-model output. The next mirror cut requires the actual destination feature/context identity, that predicate's contract and the conditional deep-copy/earlier-failure semantics. Preserving IDs after a successful delegated return is the only newly closed wrapper guarantee.

## Type3 owner: correct the missing-witness statement

CP128 already closes constructor `140917ff0`, derived vtable `1427bceb8`, scalar defaults and the parser body; CP132/133/141/143/144 advance the tokenizer/rows/file profile. CP145/149's claims that constructor/parser bodies are absent must not be repeated. The concrete vtable's virtual0 is `140918fc0`, which calls `140918060` then conditionally releases the allocation for flags&1.

Likewise CP81 and CP142-close-sub-disposer already close containing-SUB normal destructor `1400b58d0..1400b59de`, deleting wrapper `1400ac330` and project-list deletion. That complete destructor **does not read +20/+28**. CP142 decodes constructor unwind and CP143-close-indirect-owner closes the feature-release predecessor and conditional accepted-Close chain; neither establishes a release of the already published manager or optimizer allocation. The revalidated bytes here match those receipts. Searching again for a direct +20/+28 deletion order in that destructor is not useful.

The precise missing Type3 occurrence is now a typed allocation-lifetime edge: an actual optimizer produced by `140917ff0`, published in a particular SUB+28, passed through a qualified registration/retention or disposal route to its virtual0 receiver, including callback-sensitive Project feature release and the constructor's already-published raw owners. A scalar projection or ordinary `unique_ptr` destructor would choose behavior that has not been recovered. Getter-to-ShapeOptim/GEO consumers and actual Save effects remain separate requirements. Keep actual Type3 Open/load/Save guards.

For FRCROT actual integration, the current named Keyword reader still supplies no complete FRCROT owner/text/ordered-operation path through notifier return and post-callback re-fetch; `keywordValues` retains its nondefault-handler3 gate, and public input-table Save retains the nonempty-variable gate. The table helper above closes none of those owner or Save-output occurrences. Current source checks were bounded to `subproject.cpp`, `report.cpp`, `doe_study.cpp`, `activation.cpp` and the existing pair helper.

## Byte and capture receipts

`analysis/resume160-ops-type3-evidence.json` records exact file offsets, raw-byte hashes, formats, vtable slots and SHA-256 hashes of all retained raw disassemblies. Reproduction script: `analysis/resume160-ops-type3-evidence.py`. Captures use read-only Microsoft dumpbin 14.44.35228.0; `.pdata` ranges are evidence bounds, not an assertion that exception/cold paths are all in one region. Initializer/group-parser captures contain embedded tables after RET; those are not code. An initial 16-byte fill probe and the larger fill capture's partial final instruction are explicitly unqualified. The interpretation above uses only the small-fill path ending in its complete RET.

| Half-open native range | Raw-byte SHA-256 |
| --- | --- |
| `140b67701..140b67843`, FRCROT arm | `3c834552adea70f918068e04d53dc3b5db7aae181d881c5af9edba26b959658e` |
| `140b68e6f..140b68f82`, common tail | `9778a7ebfb0bbb59e8ca4aea05c9ea99a623356cdca7daedd4a2f270b3b7b6c3` |
| `140af73f0..140af77fa`, table reader | `a9a7c9faf99726022edd96b06e427fed146093d41162bff2b557a30f91cc25a9` |
| `14089b9e0..14089ba35`, reset wrapper | `f82b148e0298ebaeb6a5bd5d4dca5b68146e3a1451d606d50a309f1235d3e4a3` |
| `140899010..140899f35`, initializer code | `cd158663d7609e2b86c701b4a187aabb2e8496dbda6ef77e102a063d56a7dc96` |
| `140899f38..14089a0ac`, jump targets | `aa31e13113de6ec79ce4d2a624a9a4e1c2ae6785713d6c7fcb0112350df3fa1f` |
| `14089a0bb..14089a0bc`, template22 selector | `a9253dc8529dd214e5f22397888e78d3390daa47593e26f68c18f97fd7a3876b` |
| `14089b840..14089b95f`, allocate/clear | `10cb06f0ffd758fb0609b57bdc07447dd7ab5a6e9e5530b72fa24bb3e76c4fd8` |
| `14089b970..14089b9d3`, count clamp | `3a525d0f6fafdee1a719bc9f4d7a719da9cb80a86e0b84ec4663d007e1ce0eb1` |
| `141471850..1414718bc`, small fill | `9755e5ddedce673bbc4a7a2f4de71eb43848fc1404144d699fb229b4d59dcced` |
| `1408e5d60..1408e5ddd`, mirror wrapper | `0f235bfe104a57306c5117e07fb7dc0249cb864d38334e1411bdd0e7faa0a7a0` |
| `1408e57e0..1408e5bde`, delegated copy | `eb384b63ea37a580ce45a4a25525c7b4c868396132cf7b6e346d69c1fcc87e07` |
| `1427bceb8..1427bd020`, optimizer first45 slots | `892092bd77b5abfd5de107fcb554df603875c6f1ac4a312910d9ebb530fe8da2` |

No native positive-path comparison or production admission was performed. These are immutable static contracts and a source-ready private successor, with the actual lifecycle/input/output dependencies still explicit.
