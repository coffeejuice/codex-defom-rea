# CP108 — DBLIST `+0x1c` admission writer after CRT binding

## Finding

The newly bound CRT initializer closes *who constructs the application-state singleton*, but it does not close the DBLIST admission field's runtime writer. The constructor at `0x1402c7f80` runs through initializer-table slot `0x141e0c950` (index 31) before native `main`. CP106's retained constructor prefix establishes initialization of process type `+0x0c`, current-main `+0`, byte `+0x18`, busy `+9`, and bytes `+0xA/+0xB`; it does not establish a `+0x1c` store. DBLIST's `+0x1c` remains unknown after startup and across later Opens.

The distinction matters: `+0x0c` is separately written by its setter (`0x1402cb780`) and is used as an application process-type/warning-suppression value. `+0x1c` is read by the DBLIST admission predicate and set to one by a receiver-relative helper. Initial image zero-fill and the AppState constructor's known field resets do not imply a continuing zero admission value.

## Evidence chain

* Original `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
* The CP106 static CRT trace binds the initializer table slot `0x141e0c950` to `0x141ca0bb0`, whose calls reach `0x1402c7ee0` / constructor `0x1402c7f80` on singleton `0x143d766b8`. The bounded startup receipt is `analysis/resume106-close-producer-minimal-evidence.md`, SHA-256 `f69a3499b8d456c8be2dc6e48f80120e7be82e0c9d7708c1907a7acf0e43fed2`.
* The earlier direct helper capture `analysis/resume65-dblist-global-owner.asm`, SHA-256 `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e`, shows `0x1402cbec0` testing receiver `+0x1c == 0`, sibling `0x1402cbf00` testing it nonzero, and `0x1402cbf30` storing DWORD 1 to receiver `+0x1c`. These helpers do not identify their caller or transition semantics.
* The active DoeOutput gate at `0x140651eba` passes `0x143d766b8` to `0x1402cbec0` when its DBLIST argument gate is enabled. This links the admission read to the singleton address, not the setter's call-time receiver.
* Retained full-`.text` direct-call/jump and exact-pointer searches found no direct caller of `0x1402cbf30`. A nearby-looking initializer store to `+0x1c` belongs to a different global base (`0x143d763a0`, versus `0x143d766b8`), so it is not evidence for DBLIST admission. The search limitation is explicit: it cannot rule out indirect dispatch, constructed function pointers, or stores not covered by the bounded scans. See `analysis/resume81-dblist-setter-pe.md` and `analysis/resume79-dblist-admission-writer.md`.

## Separate `+0x0c` process type and phase

The `+0x0c` helper pair is independent: `0x1402cb780` stores the caller-supplied DWORD and `0x1402cb7a0` reads it. Retained Open/Close evidence identifies it as application process type; a successful native Open can set it to 5 when the current Project's native-open-success flag is true. The DBLIST warning path uses value 5 only to suppress a missing-file warning; it does not replace the `+0x1c` admission predicate. The CRT AppState constructor's startup zero for `+0x0c` is an earlier phase than QApplication/main-window construction and the later Open refresh. Neither phase supplies a `+0x1c` reset.

The field's native `.data` RVA lies in virtual zero-fill tail, so the loader starts both `+0x0c` and `+0x1c` at zero in a fresh image. That is only the pre-initialization image state. The CRT constructor receipt newly binds the AppState initialization occurrence, but no retained constructor bytes show a `+0x1c` write, and no evidence binds the `+0x1c=1` helper to that constructor, Open, Close, or another startup event.

## Candidate implication and exact missing witness

Keep the public DBLIST refusal and the candidate admission observation unavailable. Do not initialize candidate `+0x1c` to zero from CRT image zero-fill or infer it from process type, successful Open, or a fresh `ConversionSession`.

The exact missing native receiver occurrence is a caller/dataflow witness for `0x1402cbf30` showing that its `rcx` is `0x143d766b8` (or an alias proven to be that same object), together with a qualified enumeration of writes/resets to this receiver over startup and project Open/Close. An original same-instance trace recording this field and owner identity through those transitions would also close the gap. Existing evidence supplies neither. The earlier full-text direct-call scan and REA xref queries are bounded and incomplete; no new mutable REA query or app execution was performed here.

## Work and receipt

Read-only reconciliation of retained immutable PE captures and prior source reports. No source, build, GUI, original application, or mutable REA database was changed or run. This report does not claim the setter is never called; it records that its DBLIST receiver/caller and reset lifecycle remain unresolved.
