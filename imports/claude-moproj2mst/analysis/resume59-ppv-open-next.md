# Resume 59: next PPV Open integration boundary

## Current evidence and correction to older notes

The prior `analysis/resume57-ppv-open-binding.md` is stale in two material
places. Current `model.hpp` has `StateVars::ppvCount()` / `setPpvCount()` as
aliases of `nrecvlCount` at `+0xf0`; `tests/mdt/test_ppv_count_state.cpp`
checks the shared storage. The `analysis/resume50-ppv-pattern.md` receipt also
settles the original row separator as `\\s+` with `SkipEmptyParts`. Do not
continue to cite either as an open prerequisite.

Current source is split across three isolated pieces:

- `readPpvFileLines(project, path)` returns source-qualified ASCII/LF,
  UTF-16LE-BOM, and LF/CRLF decoded lines. It returns `OpenFailed` separately
  and guards unknown encodings or malformed UTF-16. Its exact bounds and Qt
  static qualification are in `analysis/resume58-ppv-file-reader-utf16.md`.
- `simctrl_ppv::readPpvLines(opened, lines, local, shared, flags)` implements
  count, clear, row mutation, and partial failure. Separator and exact-seven
  behavior are source-qualified by `resume50-ppv-pattern.md` and
  `resume48-ppv-reader-receipt.txt`.
- `simcontrol300::readPpvForOperation` binds list effects to the current
  `Operation::recoveryB`, `recoveryListA(operation)`, and `recoveryFlags`
  owners. The bounds and modeled field conversion are documented in
  `analysis/resume56-ppv-owned-binding.md` and
  `analysis/resume54-ppv-model-extension.md`.

None of these pieces is called from public `loader.cpp::loadSimCtrl` yet.

## Native ordering that integration must preserve

Original caller `0x1400594d0` first obtains the engine step. It constructs and
checks `SimCtrl.KEY`; a missing KEY takes the `inheritSimCtrl` return before
PPV is considered. When KEY exists, it imports KEY only if `oprFlag != 0`,
then constructs/checks `SimCtrl.PPV`. If PPV exists, it invokes the reader and
does not branch on that reader's boolean result. After the call it counts the
local list plus shared list when a predecessor exists (local only otherwise),
and writes the count to every present operation slot's `+0xf0` state field.
It returns the KEY import result.

Consequences for the adapter boundary:

1. Keep the current KEY-missing early return and KEY import order.
2. On a supported, successfully read PPV file, call the model binding with
   `opened=true`; even a false parser result must be followed by count
   propagation because native ignores that result. The parser clears local
   points after successful open, leaves shared/flags uncleared, and retains
   any mutations before a malformed row.
3. If the byte reader reports `OpenFailed`, pass the open-failure semantics
   through without clearing any list, but still perform the caller's following
   count update. Do not treat file-open failure as an early return from the
   caller.
4. A byte read that succeeds but returns
   `UnsupportedEncodingOrLineEnding` is a different state: native QTextStream
   did open, but the converter lacks a qualified decode for those bytes. Do not
   map it to `opened=false`, since that would preserve the local list where a
   native successful open may clear it. Keep the public refusal for this
   profile until decoding is qualified.

## Smallest public integration seam

The smallest safe integration is a private helper called only from the current
PPV-exists branch in `loadSimCtrl`. It should:

- obtain `FileLines` once using `readPpvFileLines`;
- reject only the unsupported-decode profile before any model mutation;
- call `readPpvForOperation` with actual open success and the existing
  predecessor predicate;
- ignore the parser boolean for caller sequencing;
- calculate local-plus-shared point count and assign it through each present
  slot's `sv.setPpvCount(count)`;
- return to `loadSimCtrl`, which still returns the KEY import status.

Keep the existing `qt::fileExists(ppv)` branch location. The distinction
between existence and open matters: the file may exist but fail to open.
Exercise that state with a direct adapter test rather than assuming existence
implies successful open. The `readPpvFileLines` test currently uses project
overlay fixtures and does not model Windows sharing or actual QFile error
mapping.

## Remaining guards

### Unknown byte decoding

Only BOM-free ASCII and UTF-16LE BOM input are supported by the current file
adapter. The original QtCore evidence qualifies UTF-16LE BOM detection and
CRLF stripping, but not malformed UTF-16 behavior, BOM-free non-ASCII
`codecForLocale` output, UTF-8/UTF-16BE/UTF-32 BOMs, or their raw decoder error
effects. Preserve the unsupported profile guard for those inputs. Do not reuse
`readTextLines`, which strips CR and decodes CP_ACP under a different
`QFile(ReadOnly|Text)` contract.

### Paired Save support for local PPV points

Current `save_current.cpp::saveSimCtrl` still throws
`M2M_E_UNSUPPORTED` when `op.recoveryB` is nonempty. Original Save calls its
PPV writer under that same local-list-nonempty gate. Thus enabling Open for a
file that creates local points would make a later public Save refuse before
the native PPV write. The next integration must either complete the paired
writer first or keep the public Open branch guarded when local PPV effects
would be created. Do not widen the guard to shared-root-only state without
evidence: `tests/mdt/test_ppv_root_only_save_guard.cpp` documents native's
local-list gate and the resulting KEY/no-PPV-write behavior for that case.

The original reader's row separator and per-slot count alias are now settled;
they are not current blockers. Point lookup equality and the shared/local
recovery-list mapping are represented by newer evidence/model receipts, but
their assumptions should be checked against those receipts before public
integration. No source, test, CMake, build, GUI, or mutable REA work was done
for this audit.
