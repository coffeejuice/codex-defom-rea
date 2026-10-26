# CP98 — DBLIST admission `+0x1c` lifecycle and candidate owner review

## Decision

The `+0x1c` DBLIST gate still has no owner-bound public Open integration seam. The native gate's application-global receiver is known, but the runtime producer for its admission value is not: static evidence identifies a setter that writes `1`, without a qualified caller/receiver occurrence, and does not identify all stores or a reset-to-zero transition. Fresh-image zero-fill is only process-start initialization; it does not support defaulting a new candidate Session to admitted after native startup/Open/Close activity.

Candidate code correctly keeps the DBLIST/ReportList parser guard and keeps `ApplicationStateProjection::dbListAdmission()` unavailable. The CP75 projection is application-scope-shaped but detached: no current source path binds it to a native-equivalent application main window, DBLIST gate receiver, or Open/Close occurrence. CP64's actual Session binding does not solve that gap. Do not connect `SessionImportContext::actualSession`, `SessionNotificationState`, process type `+0x0c`, or the standalone projection's default to DBLIST admission.

## Reconciliation of retained evidence

1. The DoeOutput Type2 callsite `0x140651eba` passes global `0x143d766b8` to `FUN_1402cbec0`. The predicate reads receiver `+0x1c` and admits only when it equals zero. It runs before the DBLIST parser and before path/ModelManager effects. The sixth caller byte is separately zero on this recovered route.
2. `FUN_1402cbf30` stores constant `1` through its receiver at `+0x1c`. `FUN_1402cbf00` tests nonzero. These bodies do not establish who calls the setter, what receiver it receives, or what event the write means.
3. The target global lies in `.data`'s virtual zero-fill tail, so a fresh image begins with `+0x1c == 0`. This is not a later Open/Close/session value. The successful Open/normal Close trace recalculates the distinct `+0x0c` process-type DWORD; it does not qualify a `+0x1c` reset.
4. The bounded full-text search for direct E8/E9 references to the setter, exact code-pointer literals, and nearby stores through a register loaded with the global found no direct owner-qualified writer. This is useful negative evidence only: indirect/constructed calls, aliased bases, whole-object writes, and other register dataflow were not ruled out.
5. The candidate application-state projection models only source-qualified mode-0 process-type refresh values (5 or 1). Its DBLIST value remains unavailable and no API writes or clears it. That is the correct guard state, not a missing default that can be filled from Session creation.

## Exact missing native occurrence

The next evidence needed to admit any public DBLIST path is an occurrence binding all of:

- the actual caller or dispatch edge reaching `0x1402cbf30` (including an indirect/virtual call if applicable);
- the setter's runtime `rcx` receiver identity, proving whether it is the same `0x143d766b8` gate object;
- the triggering lifecycle/action and the before/after `+0x1c` values;
- every relevant write/reset to that same receiver across startup, Open, DBLIST dispatch, Close, and a later Open, or a demonstrably complete application-state provider that records equivalent facts.

The static search receipts do not satisfy the first two bullets. No public candidate seam should be added until this occurrence is recovered. If retained PE tooling cannot close it, the minimum external witness is an original application-scope runtime trace of setter caller/receiver plus field values at the lifecycle boundaries above; this task did not run the original application or use GUI/REA.

## Candidate checkpoints

- `source/apps/moproj2mst/src/doe/session_records.cpp` still refuses both direct `DBLIST` and `ReportList` children at the public parse boundary.
- `ApplicationStateProjection` is not owned by `ConversionSession`, but is currently an isolated value projection, not a native singleton or wired application owner.
- The detached DBLIST reader and manager prefix do not admit the public call path. In particular, manager lookup/create and warning behavior are downstream of this gate; implementing them cannot compensate for unknown admission state.

No source/build/test/GUI/original execution/mutable REA action was performed.

## Bounded receipts

Controlling original PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Receipt | SHA-256 |
|---|---|
| `analysis/resume66-dblist-global-lifecycle.md` | `e57b855d498e2cdc8d760663a829e7fbe849f73b925a2c4e64440d122bbbf90e` |
| `analysis/resume75-doe-dblist-global-owner.md` | `35ba1f127aff7a083b94a7a11b506e10e177e24bd9bc0dce911f1f4243e4d5e4` |
| `analysis/resume79-dblist-admission-writer.md` | `d53d4bc87812e242d669e2ad24f6f444bd646e04a19d20d638e60b7c065bd07b` |
| `analysis/resume65-dblist-global-owner.asm` | `5ce9990c8bc0e7cdcef3333c5a98c973b44e2ac83290d347764e39ebb455e62e` |
| `source/apps/moproj2mst/src/doe/application_state_projection.hpp` | `25d236d51f278ab7e57e3773d8f6e12df68c57900879d150cf3deba7d9cc597c` |
| `source/apps/moproj2mst/src/doe/application_state_projection.cpp` | `ba861fa708ff766c3ffaf5f8ccaabf5ee328ca5150e5e4fe6ae29289530f8803` |
| `source/apps/moproj2mst/src/doe/session_records.cpp` | `8e8410f7fb4596bc62e752b02969208e51c9cfcfab402ba72727885bb3f50794` |
