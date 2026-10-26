# CP278 one-slot child fixture validation

The fixed harmless Windows child proves a positive DR0 entry-to-dynamic-return handoff across two synthetic color-shaped calls. It does not run the original application, use GUI control, or establish original color reachability or full overlap coverage.

## Frozen inputs and checks

- `admin/color_one_slot_debug_fixture.py` SHA-256 `7c5c1d78bc0be940cf68c73a6bebf2577079b5ad4ac9274bbc4e037bbf0f1119`.
- `admin/test_color_one_slot_debug_fixture.py` SHA-256 `5dd5cf5fa936a9f6cc60ed6df44f4643203bb833c938420b007a2ef0ccfd6a6b`.
- Independent root run: `python -m unittest discover -s admin -p test_color_one_slot_debug_fixture.py -q`; 3 tests, 0.789 s, exit 0, OK.
- `git diff --check` returned 0 with no output.
- Independent reviewer: PASS for benign fixture only. The four fixed negative modes are real child actions: post-marker thread, duplicate marker, benign exception, and timeout. Each requires bounded cleanup and full event continuation.

The receipt checks logical handle-ledger closure, a signaled owned child, and observed process exit. It explicitly sets `osInvalidHandlesVerified=false`; it does not assert invalid-handle confirmation. The duplicate marker is not a nested machine-code entry. A single slot armed at a return can miss nested or concurrent entries. Return `RCX==VIEW` is synthetic-specific and must not be copied into an original observer.

## Successor and hold

CP279 is a read-only original preflight: exact encoded CLI fixture identity, PE/callsite bytes, base and object prerequisites, fresh disposable clone and manifests. Original execution remains HOLD pending that preflight, original-capable observer implementation, harmless lifecycle validation, and independent neutrality/policy review. The DOE/editor300/Save/Close and GUI-control gates are unchanged. The converter objective is incomplete.
