# CP135 Type3 private parser boundary and next cut

The retained evidence now supports a bounded **in-memory scalar line-sequence driver**, but not a file-level `.OPT` parser. The recovered QFile/QTextStream call order is useful for future integration, yet the stream’s byte-to-line behavior and row path remain insufficiently qualified. Keep the public Type3/ShapeOptim path guarded.

## What the source closes

The original parser entry at `0x140908630` checks the optimizer pointer before constructing QFile. It opens using mode flags assembled from `ReadOnly` and `Text`; on open failure it constructs the retained failure message and returns false without optimizer mutation. After successful open it writes the purge flag true, constructs one QTextStream, reads the first line separately, trims it, and converts it to an otherwise unused integer. It then loops on `!atEnd()`, reads a line, splits with `QRegExp("\\s+")` and `SkipEmptyParts`, processes keys in file order, and on ENDDATA goes to common close/cleanup at `0x140909400`. The observed return local remains false on the parsed paths. Scalar mutations and row appends are immediate and have no transaction/rollback.

The split delimiter/options are now bound by CP132 and the BMP tokenizer by CP133. The scalar dispatcher covers supported top-level scalar and ignored keys; CP130 added the ENDDATA terminal result. CP131/132 provide prepared row append projections and the pure row-tag classifier, but not a row parser or native row owner.

## Smallest safe integrated prefix

Add a private in-memory sequence helper, separate from public loading, that accepts already supplied lines as `std::span<const qt::String>` and applies the current tokenizer/dispatcher in order. It should:

- skip token-empty lines and preserve the existing ignored-key behavior;
- apply supported scalar writes in encounter order;
- stop before later lines on `Terminate`;
- stop with a distinct candidate guard outcome on surrogate/tokenizer refusal or `RowBodyNotImplemented`;
- retain earlier scalar state when a later line reaches a guard, reflecting native nontransactional ordering without claiming full parser fidelity.

Place the sequence implementation in a private `src/ops/type3_optimizer_scalar_sequence.hpp/.cpp` pair (or adjacent private detail files if root prefers one module) and add `tests/doe/test_type3_optimizer_scalar_sequence.cpp`. Focus cases on ordered overwrite, empty/unknown/skipped line continuation, ENDDATA preventing a following mutation, and an earlier scalar remaining applied when a later row key is refused. Keep short/malformed scalar token behavior categorized as a candidate guard; CP129 found no native rejection contract for short lines. Tests should describe an in-memory prefix, not `.OPT` parsing. No CMake change is needed if the existing recursive globs cover the new source/test tree, but verify that at integration time.

## Why a file-level cut is not yet justified

The assembly establishes QFile construction/open flags, QTextStream construction, `atEnd` and `readLine` call sites/order, and cleanup control flow. It does not establish enough to reproduce file-level input safely:

1. This parser uses QFile `ReadOnly | Text`. Existing PPV evidence uses `ReadOnly` without Text; it cannot be copied wholesale to qualify Type3’s QFile text-mode read translation.
2. The default QTextStream’s codec/autodetection and byte-decoding behavior for this specific path are not qualified. ASCII with no BOM is a plausible finite profile, but it must be stated as candidate input restriction and still needs line-ending/Text-mode mechanics proven.
3. The separate first-line read/trim/toInt result is unused semantically, but its exact effect on empty input, final line without newline, and subsequent `atEnd()` state matters for which lines reach dispatch.
4. Read errors, stream status, early termination, and close/cleanup failure effects are not fully recovered. The original parser returns false on observed paths even after mutations, so a boolean helper/file result risks conflating native status with candidate acceptance.
5. NUM_CON/REGIONS row derivation, malformed/short-row behavior, QString formatting, native shared-data allocations, ownership, and exception cleanup remain open. Earlier scalar writes and completed appends persist if a later row reaches EOF or ENDDATA.

A future private file cut should wait for a source-qualified exact line-reader/decode contract and a row path decision. It must preserve open-before-mutation, purge update after successful open, scalar/row encounter order, ENDDATA cleanup, prior effects on later failure, and the native false return independently from candidate status. Until then, the in-memory sequence seam is the useful small cut.

No source edits, builds, tests, GUI activity, original process execution, or mutable REA actions were performed. Original claims above are static PE/call-flow observations bound to the retained `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC` and the cited receipts; no runtime differential is claimed.
