# CP62: public PPV root-only Save contract

## Test added

Extended `tests/mdt/test_ppv_public_lifecycle.cpp` with a second, independent
scratch-copy C ABI call. It selects operation 2 (`Task00002`, whose retained
`SimCtrl.KEY` exists and whose prior diagram item supplies a predecessor) by
changing only the copied project's zero-based `Sub_Project Active` index from
2 to 1. It injects this one-row PPV input in that task folder:

```text
1\r\n0 true 7 8 9.5 10.5\r\n
```

With a predecessor, the source-qualified reader routes `true` rows to the
active subproject's shared root list and leaves the operation-local list
empty. Six fields are intentional: the reader's optional flag-list mutation
requires exactly seven split fields. The test invokes `m2m_convert_w` once
with `M2M_FLAG_WRITE_SAVE`, expects `M2M_OK` and an MST, then checks that the
preexisting PPV bytes remain unchanged. It also verifies the retained fixture
manifest is unchanged. The converter test never writes to the retained input.

## What the assertion establishes

The native Save behavior is source-qualified in
`analysis/resume58-ppv-save-root-gate.md`: its PPV writer checks the local
list's `isEmpty()` before opening the path, so local-empty/root-nonempty
skips PPV open and leaves any existing PPV bytes untouched. The caller writes
SimCtrl.KEY first and ignores the PPV helper's false return. The C ABI test
checks the matching public outcome for the root-only input.

This does not establish that the current converter dispatches the native
writer for this case. Current `save_current.cpp::saveSimCtrl` only enters its
PPV path when local recovery-B is nonempty; it therefore has the same
externally visible PPV omission for root-only state without exercising the
native helper gate. The test does not infer hidden list state from status or
claim that the root point was serialized. It is a focused public contract
regression for the skip effect, not proof of full root-list Save integration.

## Evidence and limits

- `analysis/resume58-ppv-save-root-gate.md`: immutable original writer gate,
  caller order, and ignored return.
- `analysis/resume50-ppv-reader-model.md` and
  `include/m2m/engine/simctrl_ppv_reader.hpp`: six-field row routing, shared
  list ownership, and exact-seven flag branch.
- `src/model/subproject.cpp::parseSubProject`: `Active` is a zero-based
  `DiagramItem` index; `src/model/operation_rescue.cpp::setOperationId` maps
  operation id 2 to `Operations/Task00002`.
- `src/engine/loader.cpp::loadSimCtrl`: the task-local PPV read follows
  `SimCtrl.KEY` existence; `src/engine/save_current.cpp::saveSimCtrl` writes
  the KEY before considering its local-only PPV path.
- The test was not built or run in this lane. Root owns validation. A passing
  result would establish this one synthetic public outcome only; it would not
  establish original output parity or broader shared-point serialization.
