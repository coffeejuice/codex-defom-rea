# Resume 57: PPV Open integration decision

## Decision

Keep the public `loadSimCtrl` PPV unsupported guard. Do not call
`readPpvForOperation` from this loader yet. The static parser contract and the
new model binding close point/list semantics, but three loader-level edges are
not represented by the current conversion source, and enabling only the
list mutation would leave incomplete Open/Save behavior.

## Qualified Open behavior

The source-qualified native body is `0x1400594d0`, described in
`source/docs/re/moproj2mst/mdt/L0/spec_A.md` section 2 and mirrored by current
`src/engine/loader.cpp::loadSimCtrl`:

1. Obtain the operation's engine step; return false if absent.
2. Build `SimCtrl.KEY`; if it does not exist, return `InheritSimCtrl` directly.
   This means that branch never reaches the PPV check.
3. Optionally import the KEY when the operation flag requires it.
4. Build the operation task folder's `SimCtrl.PPV` path and test existence.
5. If PPV exists, call the reader with local OPI `+0x20`; when there is a
   predecessor also pass root list `+0x19a8` and optional OPI flags `+0x28`.
   Without a predecessor pass null shared/flag arguments.
6. Ignore the PPV reader's bool, compute the local plus shared point count
   (local only without a predecessor), and assign that count to each present
   slot subobject's `ppvCount` at `+0xf0`.
7. Return the SimCtrl.KEY import result.

Current path construction at `loader.cpp:237` matches the qualified path
helper (`cleanPath(taskDirAbs(op) + separator + "SimCtrl.PPV")`). The reader's
open/clear/parser order is settled in `analysis/resume50-ppv-reader-model.md`:
QFile open failure leaves lists untouched; successful open clears local only;
later count/row failure keeps prior mutations; the loader still performs its
count update after the call.

## Blocking edges

### Per-slot count owner is absent from the model

The native caller's final step mutates every present operation slot's
subobject `ppvCount` (`+0xf0`). No `ppvCount` member/accessor or equivalent
effect appears in the current `include/m2m`/`src` model (a source search found
no `ppvCount` symbol). `loadSimCtrl` currently has no representation for this
observable mutation. Calling the parser from it would therefore not implement
the qualified Open behavior even on the no-predecessor branch. This needs a
separately owned model/accessor integration before the loader can be enabled.

### The predecessor branch is not safe with current Save behavior

The native predecessor branch mutates the shared root list and may insert
flags while leaving the operation-local point list empty. Current
`saveSimCtrl` (`src/engine/save_current.cpp`) guards only nonempty
`op.recoveryB`; its error explicitly says the SimCtrl.PPV writer is not ported.
It does not guard on `recoveryListA`/root points. Enabling a root-only PPV Open
could therefore allow a later Save to write SimCtrl.KEY and continue without
writing the modified shared PPV state. Keep the predecessor Open branch
unsupported until the paired Save behavior or a safe Save guard is reviewed.

### Exact file decoding/open profile is not bound

The native reader opens `QFile` with `QIODevice::ReadOnly` (flag 1), then
reads lines through `QTextStream`. The frozen `readPpvForOperation` accepts
already-decoded line strings and a Boolean modeling open success; it does no
filesystem access. The existing `readTextLines` helper in `src/engine/l5_slots.cpp`
uses `QFile ReadOnly | Text`, drops carriage returns, decodes via CP_ACP, and
rejects BOMs because that helper's QTextStream codec-detection behavior is not
reproduced. That is not source-qualified as equivalent to this PPV reader's
ReadOnly/no-Text path. A dedicated, bounded native-equivalent QFile/QTextStream
line source (or a retained source qualification for the same byte/decode
profile) is needed before the actual file call can be added.

## Guard and scope

`loader.cpp` remains unchanged, including the existing PPV unsupported guard.
This also preserves the KEY-missing early return and Save's separate writer
guard. No branch was enabled merely to make the Open refusal disappear. No
test was added because no loader behavior changed; no build, GUI, or mutable REA
operation was performed.

The next safe integration prerequisite is to qualify/model the native
per-slot `ppvCount` mutation and exact QFile/QTextStream input behavior, then
address the root-list Save guard before enabling predecessor PPV Open. Until
those are closed, the operation-level binding remains a callable model seam,
not a public loader integration.
