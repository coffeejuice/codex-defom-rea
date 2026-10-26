# CP178 root integration

The retained Report constructor now carries qualified caller/version provenance from the public QObject call into the private initializer. At QtCore `0x1311f1..0x1311f9` it admits only the observed `EDX=0x40804` equal branch and advances to `0x13120b`, stopping before the private `+0x08` store. Missing, foreign or mismatched provenance is refused without modeling the mismatch fallthrough. Earlier private fields, same-root shared-null bindings and symbolic deltas stay unchanged. Independent source review accepted the decision and refusal tests.

In a disjoint editor300 lane, 13 L6 local SimControl raw-return uses became synchronous scoped callbacks. Each retains its own model lookup and `GuiModel::simCtl()` geometry refusal; Distortion's final `current` lookup and grouped default writes preserve source order. The direct null-step fixture checks untouched prologue fields. The Stopping cache1 barrier remains open.

`analysis/wave3-checkpoint178-validation.json` and `build/wave3-checkpoint178-verified/` freeze source SHA-256 `1c53de8b41e9c8299d3a6c6f76c09696080dc950a2ced0fe2310338ed1e2e47c` over 3,554 files: 361 registered, 354 clean passes, six logged-gap suites, one diagnostic-difference suite, zero failed/skipped/incomplete/fixture changes, and no changed logged diagnostics. Historical 93 SAME/five DIFF remains qualified. Focused connector, Session, L6, Stopping, Save and ABI tests passed.

CP179 was selected before freeze: a known zero store at private `+0x08` on the taken equal branch at `0x13120b..0x13120f`, plus a disjoint Diestress KEY Save step-borrow closure. Public lifecycle and fresh original comparison gates remain open. No GUI control, original application or live REA session was acquired.
