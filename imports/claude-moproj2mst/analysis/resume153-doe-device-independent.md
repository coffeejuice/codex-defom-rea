# CP153: independent DOE Regions device audit

## Decision

No smaller faithful Regions Save effect can be integrated from the retained evidence before cleanup. The current CP149 caller correctly ends at the cleanup-enumeration guard after preserving its immediate Regions mkdir attempt and executing the actual current-operation model lookup. Advancing into even the root-preserving clear helper would require choosing results for fresh Qt device resolutions and metadata calls that the source candidate does not possess.

## Existing effects are already distinct and wired

`analysis/resume149-doe-output-wired-next.md` and `analysis/resume149-doe-output-wired-review.md` record the CP149 prefix: the actual active Type2/Study-owned path is attempted immediately, with the physical attempt retained; failed mkdir stops at the unsupported warning-modal-return boundary; successful mkdir reaches `opModel(p, op)` for that same operation; null returns the callback; nonnull stops before QDir construction/enumeration. The review confirms the real Save dispatch tests and expressly does not claim cleanup or full Save.

The integrated mkdir adapter in `source/apps/moproj2mst/src/qt4/native_directories.cpp` is specifically `nativeMkpathAbsolute`. Its Windows attribute query and optional `FindFirstFileW` fallback answer whether mkdir's path components exist/is directories. They do not produce QDir's retained engine pointer, an entry-name snapshot, QFileInfo classification, or removal results. The model lookup is also an already integrated, separate effect; it cannot be reused as a native filesystem-device result.

## Same-occurrence evidence gap

`analysis/resume150-doe-output-device.md` closes static call/field relationships but explicitly has no observed pointer for the actual Output Save QDir. QDir path setup calls resolver `0x670f0080` at `0x670b01a6` and stores its return at private `+0x20`. The initial constructor zero is not an invariant. Each QFileInfo constructor separately calls the resolver at `0x670b808e`, storing its own result at `+0x50`; neither the QDir return nor another QFileInfo return determines it. The iterator can also re-resolve its own file entry if the QDir engine was nonnull.

Even a qualified null for the QDir would only select a native iterator branch; the retained evidence does not establish this invocation's result or the native enumeration's success/error outcome, accepted entries, filtering and ordering. Each subsequent QFileInfo type query has independent cache/engine state. Removal has further independent engine resolution, and QDir `cdUp`/`rmdir` are distinct calls. Thus no exact smaller effect such as “root is a directory,” “snapshot is empty,” or “remove this file” follows from the mkdir attempt or path syntax.

## Smaller-branch check

The first cleanup action is not deletion: the helper obtains `absolutePath`, constructs a fresh QFileInfo and calls `isDir`. That already depends on QDir's selected engine plus the QFileInfo resolver/classification result. Skipping that probe to run `entryList`, using a host directory iterator, synthesizing an empty list on host failure, or deleting only named/supplied entries would alter the retained native order/semantics. Removing only the Regions root would also contradict the qualified root-preserving helper. A path-only or record-only removal request is not an observed native return.

The exact blocker is therefore **missing same-invocation QDir resolver return at `0x670b01a6` bound to the actual DoeOutput Regions path/object, plus unqualified native enumeration and independent QFileInfo/device results**. The CP150 report identifies a possible next evidence edge, not a result already present in the PE: the resolver return at `0x670b01a6`; if an actual nonnull return is reached, its owner callbacks and later occurrence returns must be recovered rather than bypassed. Until this evidence exists, retain the CP149 enumeration guard and its frozen source. No source edits, builds, GUI control, mutable REA, or original runtime actions were performed for this audit.
