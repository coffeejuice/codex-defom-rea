# CP203: RBOOL zero-slot predicate is closed; constructor and first-Open timing remain unbound

This converter-only reconciliation finds one source-ready static predicate, already implemented: Boolean `postLoad` creates a kind-5 slot when the current OPI has zero slot records. That predicate does not resolve Boolean constructor E reaching its notifier, or an Apply-origin add during a complete first Open. The latter claims require separate, correlated original occurrences.

Read-only audit. Only this report was written. No production/test source, build, original execution, GUI/app control, live or mutable REA database, parent DMO, or Claude source was changed. No new native observation is claimed. The retained evidence remains qualified to DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Resolved static predicate: zero OPI slot records in Boolean postLoad

The selected original Boolean `postLoad` body (`1411df020`) reads the OPI slot-record count at `11df0cb`. If it is zero, it calls `addObject(OPI)` at `11df0e9` (normal continuation `11df0ee`), then sets slot 0 to kind 5 at `11df0fb`. The branch tests the OPI slot-record count, not engine object count and not `cutter == 0`. In normal `baseLoad` order, this postLoad call returns before that invocation's `propsLoad` and `postLoadViews`.

The corresponding candidate code already exists in `source/apps/moproj2mst/src/engine/l5_slots.cpp` (`booleanPostLoad`): it checks `opi->objs.size() == 0`, requests `engine::addObject`, then calls `setSlotKind(..., 0, 5)`. It intentionally does not normalize Boolean workpiece/cutter values, gate the kind update on the add result, or roll back. The focused lifecycle regression `source/apps/moproj2mst/tests/mdt/test_boolean_postload_creation.cpp` distinguishes zero OPI records from a nonzero slot-record count with zero engine objects and checks the one-object/kind-5 prefix and unchanged Boolean values. This closes the conditional converter behavior; no further postLoad mutation is source-ready or justified.

This branch can explain a load-time object before `propsLoad`/`postLoadViews` without Apply, a timer, or a constructor-notifier inference. A static predicate plus candidate test does not establish that a particular native Open input reached it; that would require observing the actual OPI and branch in that load invocation. Nor does a postLoad add count as an Apply-origin creation.

## Constructor E remains a separate unresolved occurrence

The retained static chain constrains, but does not prove, the native occurrence: the Object factory allocates E at `11e065b`, then enters Boolean constructor `11e2480(E, OPI)` at `11e0677`; QWidget construction has normal continuation `377990`; wrapper/guard setup proceeds through `130830`; and its notifier call at `130ad1` returns to `130ad6`. Later Boolean fill, a Forming editor capture, the canonical Session notifier registry, or a constructor's eventual completion does not bind these stages to one E and one invocation.

A qualifying same-invocation constructor receipt must include process/module profile and TID; successful Object-branch allocation and E lifetime generation; matching constructor/frame identity and incoming OPI; same-E QWidget normal return `377990`; setup/guard normal returns through `130830`; actual E+38 OPI resolution; the conditional `05f770(OPI)` model return if taken; E+28 bind return from `377b20(E,S)` with nullable S preserved; and the notifier entry at `130ad1` specifically identified by caller continuation `130ad6`. It must also retain canonical notifier N identity, the single N+141 suppression read, and N+90 before/after. E+28 and E+38 are different fields. A later fill or another class's constructor cannot substitute. Static disassembly is useful for hook selectors only; it cannot supply allocation generation or normal returns.

## First-Open Apply/add remains a distinct unresolved occurrence

The postLoad continuation `11df0ee` is a separate creation origin from Boolean Apply Workpiece `11e295a` and Apply Cutter `11e2bcb`. A positive Apply-origin add before Open completion needs a single selected, complete Open frame (GUI handler `271b0..28514`, or selected CLI startup branch through `1e4e`), nested same-owner current-widget UpdateData on E, Apply `11e2870`, E+38 resolution to OPI O, then a normal `addObject(O)` return at `11e295a` or `11e2bcb`. Record process/thread/module, frame nesting, E/OPI/window/project lifetimes and the actual returned object/number. Keep postLoad origin independently tagged. Loader return `1c94` and postOpen return `1cae` alone are not complete Open boundaries.

One well-formed positive nested chain would establish that occurrence before the selected Open completion. Calling it the *first* occurrence, proving absence, or attributing it to an Open-registered timer needs gap-free selected coverage, reentrancy accounting, and (for timer attribution) actual timer/connection ownership. An add with no caller continuation or origin, final object count, file parity, constructor notification, editor fill, or postLoad object cannot establish Apply.

## Retained evidence and next admissible step

CP196's retained-capture audit indexes twelve original probe directories and 39 message-bearing streams without a qualifying Boolean Apply/add chain or same-invocation constructor frame. Editor004 selected a Forming constructor and did not cover the whole selected Open completion; its QWidget/notifier observations cannot be reassigned to Boolean E. This is a limit of retained coverage, not evidence that Boolean construction or Apply never occurs. CP164/171/196 agree on the missing joins; the zero-slot implementation and test do not supersede them.

No independent static predicate remains that would safely authorize a timing-dependent production change. Preserve the constructor and first-Open/PostPending guards. The next evidence action is a passive, naturally reached original occurrence for each claim, correlated as above. The current app-control lifecycle assessment reports `unsupported_release_api`/`release_unverified`, with no release method and verifiable receipt available; the recovery instruction therefore bars acquiring a GUI-control session. If that capability changes, capture the constructor and complete Open/Apply/add witnesses separately, then use the offline selector/checker only as preflight and independently validate source identity and healthy coverage. Checker acceptance alone is not original-runtime evidence.

## Evidence used

- `analysis/resume102-boolean-postload-creation-bound.md`: native zero-slot branch and base-load order.
- `analysis/resume102-boolean-postload-test-implementation.md`: candidate regression and frozen-source qualification.
- `analysis/resume103-boolean-creation-census.md`: three Boolean add origins and shared creation caveats.
- `analysis/resume164-boolean-next.md`, `analysis/resume171-rbool-next.md`, and `analysis/resume196-bool-witness-audit.md`: constructor and first-Open witness requirements and retained-capture limits.
- `analysis/resume202-boolean-static-successor.md`: prior reconciliation; this note narrows its source-readiness conclusion to the already implemented zero-slot predicate and restates the two unresolved occurrence requirements.

No source/build/deployed version of a reconstructed `DEF_PRE_64.exe` is discussed; no reconstructed executable instance is claimed.

