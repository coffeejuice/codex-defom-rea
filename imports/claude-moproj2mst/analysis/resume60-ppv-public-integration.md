# Resume 60: PPV public Open/Save integration design

## Current pieces and boundary

There are now isolated helpers for each half of the PPV path:

- `readPpvFileLines` decodes a bounded file profile;
- `readPpvOpenSequence` composes that decoder with the PPV parser/model binding
  and the original caller's slot-count update;
- `writePpvForOperation` binds current operation/root/flag owners to a
  successful-open payload, but does not open or write a file;
- `writePpvPayload` creates the ordered text payload but has no device I/O.

`loader.cpp::loadSimCtrl` still refuses any existing PPV. `save_current.cpp`
still writes SimCtrl.KEY and then refuses a nonempty local PPV list. Thus no
public PPV file path is active yet. The Checkpoint 59 Open sequence helper was
made against a bounded profile and is not equivalent to full native
QTextStream decoding.

## Exact supported Open profile

Current reader source accepts:

- BOM-free ASCII bytes (the machine locale codec is irrelevant for ASCII);
- UTF-8 BOM followed by ASCII payload, after removing the BOM;
- well-formed UTF-16LE after `FF FE` BOM;
- LF and CRLF line endings, with CR stripped only when directly before LF.

It refuses BOM-less bytes `>= 0x80`, UTF-8 BOM with non-ASCII payload, other
codecs/BOMs, NUL and disallowed controls, odd-length or unpaired-surrogate
UTF-16, and embedded/unpaired CR. The ASCII-only UTF-8 branch is a bounded
profile, not general UTF-8 decoding.

The original reader path is statically qualified as `QFile(ReadOnly)` plus an
unconfigured default `QTextStream::readLine()`. Retained QtCore evidence
qualifies UTF-8/UTF-16LE BOM codec selection and CRLF line-ending handling.
For BOM-less high bytes, Qt uses `codecForLocale`; the original process's
effective locale codec/code page is not known. Malformed conversion output is
also unqualified. Do not make `readTextLines` the PPV reader: it uses
`ReadOnly|Text`, CP_ACP, strips all CR, and has different BOM handling.

When a file read succeeds but falls outside the adapter profile,
`readPpvOpenSequence` returns `UnsupportedInput` before list or count mutation.
This is an adapter refusal, not native decode behavior. Public Open must keep a
visible unsupported path for that result until the profile is expanded from
qualified evidence. A byte-open failure is distinct and follows native
caller-order semantics: the parser leaves local/shared/flags untouched, then
the caller still updates every present slot's count from the lists already in
memory.

## Open order and model aliases

Original `0x1400594d0` obtains the engine step, checks `SimCtrl.KEY`, and takes
the inherited-value early return when KEY is absent. For existing KEY it
imports the KEY only under the original operation flag condition, then checks
`SimCtrl.PPV`. Only that branch reads PPV. After the PPV reader call it ignores
the reader boolean, counts local points plus shared points when the
predecessor branch supplies a shared list, sets each present slot's `+0xf0`
count, and returns the SimCtrl.KEY result.

The current model aliases PPV into the native lists: local
`Operation::recoveryB`, shared root `recoveryListA(operation)` only when a
predecessor exists, and `Operation::recoveryFlags` in that same branch.
PPV-specific fields are appended to `RecoveryEntry`; the count setter aliases
the KEY `nrecvlCount` storage, rather than creating another persistent count.
The current reader binding and Open sequence implement these list and count
effects for the supported profile.

Path qualification still matters. Open's `SimCtrl.PPV` path is
`cleanPath(taskDirAbs(op) + separator + "SimCtrl.PPV")`, distinct from
`simCtrlPath(op)` (`SimCtrl.KEY`). The new file adapter reads through
`datafiles::readFileBytes`, which checks the project overlay before disk; the
original reader opens the path directly with QFile. Before public integration,
confirm that this overlay precedence is intended for PPV at this lifecycle
point, or use a direct-file mode for the sidecar. A test should cover an
overlay/disk collision if overlay precedence is retained.

## Save order and output behavior

Original `0x1400597d0` attempts the SimCtrl.KEY writer first, then calls the
PPV writer regardless of the KEY writer's return. The function's final result
is the KEY writer result; the PPV helper result is ignored. The native PPV
writer checks the local-list `QListData::isEmpty()` gate first. Local-empty
(including root-only state) returns false before QFile construction/open, so
the PPV path is left untouched. Otherwise it opens the operation-task
`SimCtrl.PPV` path as `QFile(WriteOnly|Text)`. Failed open returns false before
stream writes. On successful open it emits, in order:

1. `local.size() + shared.size()` if the shared pointer exists (even when
   flags is null), otherwise `local.size()`;
2. each shared row only when both shared and flags exist, in shared order;
3. each local row in local order, with its index restarted at zero.

Shared rows use `true` plus `flags[i]`; local rows use `false` plus literal 0.
Each `QTextStream::endl` flushes per line. QFile Text mode translates LF into
CRLF. On successful QFile open the writer closes and returns true without
checking stream status, file error, close status, or a write result. The caller
ignores this bool. Therefore post-open write/close errors must not become a new
Save exception or change the returned KEY result.

The exact bytes left after post-open failure are **not established** for this
PPV path. `analysis/resume59-ppv-save-next.md` records the limit. The existing
`writePpvPayload` produces all bytes in memory and its `PayloadReady` result
does not model truncation, per-`endl` flushes, retries, partial progress, or
close-time recovery. Do not infer a deterministic empty/partial file envelope
from the successful payload test. `mst::FileDevice` and `mst::writeTextMode`
provide a qualified project device/text-mode substrate, but an integrated PPV
writer must stream one native line/endl at a time through that device to keep
the original error and continuation order. A single bulk payload write would
flatten those effects.

## Path, codec, alias, and model guards

- **Path:** both Open and Save use the operation task directory plus
  `SimCtrl.PPV`, not `SimCtrl.KEY`. QFile accepts the QString path. Do not
  convert the PPV filename through the KEY writer's narrow `toLocal8Bit` path.
  Save's current adapter derives the task path but refuses an unknown path
  qualification and empty/relative paths.
- **Output alias:** for native Save, add a `sameFile(ppvPath, outPath)` check
  before `saveCurrent` performs the PPV write, parallel to the active
  SimCtrl.KEY guard. If public Open is enabled first, the output path may be
  the PPV input; reject that alias before Save can truncate it. Keep this
  converter-specific guard separate from original app behavior.
- **Locale codec:** Save payload text is ASCII for the bounded finite-number
  profile, but its actual QTextStream uses the process locale codec. The
  original locale codepage was not captured. Continue to require an explicit
  caller-provided ASCII-compatible codepage; do not label 1252 as the original
  profile. Open's BOM-less non-ASCII remains guarded for the same reason.
- **Owner shape:** the Save adapter guards local/root count representability
  and a non-null short flags vector before indexing. The original directly
  indexes `flags[i]` when shared and flags are both non-null; behavior for
  undersized flags is not qualified. Keep that malformed model state refused.
- **Root-only:** local list empty must short-circuit before reading path, codec,
  flags, or opening the file. Existing root-only tests cover this gate; do not
  broaden it to a shared-list-driven write.

## Smallest root-owned integration patch and tests

Treat Open and Save as a paired integration because public Open can create a
nonempty local list that the current Save path rejects. The minimal shared
patch should:

1. In `loadSimCtrl`, preserve the existing missing-KEY return and KEY import
   sequence. At the PPV-exists branch, call the isolated Open sequence. Keep
   the public unsupported result for unsupported decode/owner states. Allow
   `OpenFailed` and `ParserReturnedFalse` to continue to the original return
   using the KEY status, with partial mutations/counts already applied.
2. In `saveSimCtrl`, preserve SimCtrl.KEY first. After that writer attempt,
   call PPV writing regardless of the KEY status. Local-empty/open-failed
   writer results are ignored; only the KEY result is returned. Use direct
   `FileDevice` plus a QFile Text-mode adapter for nativeSave; use a captured
   ordered payload for effect-recording mode. Preserve the local-list gate and
   do not implement transactional replacement.
3. Add the PPV output alias check in the root-owned native Save preflight before
   PPV truncation. Keep it scoped to converter output safety.

Focused tests should establish: Open's KEY-missing early return; supported
ASCII, UTF-8-BOM-ASCII, and UTF-16LE-BOM/CRLF call-through; unknown codec
refusal before list mutation; open failure preserving lists but updating
counts; malformed row preserving partial mutation and still updating counts;
Save local-empty/root-nonempty no-open; KEY-before-PPV call order; PPV open
failure ignored while KEY result is preserved; successful file/path/payload
ordering; output-alias rejection before truncation. A device fault test may
assert writer returns/continues as the native helper does after injected
post-open errors, but must retain raw output and must not assert a specific
partial-file residue until that behavior is separately qualified.

No source, test, CMake, build, GUI, or mutable REA change was made for this
design audit.
