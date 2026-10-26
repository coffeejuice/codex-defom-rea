# Resume 54: additive PPV fields on recovery entries

## Change

`include/m2m/model.hpp::RecoveryEntry` now retains its first two members,
`type` and `value`, and appends three default-initialized semantic PPV fields:
`ppvSecondInt`, `ppvFirstDouble`, and `ppvSecondDouble`. The evidence-backed
mapping is:

| Model member | PPV meaning | Native BC field |
|---|---|---|
| `type` | first parsed integer | `+0x00` |
| `ppvSecondInt` | second parsed integer | `+0x04` |
| `ppvFirstDouble` | first parsed double | `+0x08` |
| `ppvSecondDouble` | second parsed double | `+0x10` |
| `value` | KEY-visible value; PPV construction leaves it zero | `+0x18` |

The new fields do not change KEY serializer accesses to `type` and `value`.
They allow a future reader to retain the PPV equality key in the same list
entries that represent native local/root records. They are ordinary C++ model
fields; no binary reinterpretation or native-layout assertion is made.

## Focused coverage

Added `tests/mdt/test_ppv_recovery_entry.cpp`. It checks that existing
two-member aggregate initialization `{type, value}` still produces the same
KEY-visible values and default PPV keys; it checks PPV semantic initialization
with a zero KEY value and all three additional fields; and it checks default
construction. The test is not registered in CMake because the requested scope
excludes shared build configuration. It was not built or run.

## API impact and boundary

- Existing `RecoveryEntry{type, value}` aggregate initialization keeps its
  field order and meaning because all new members follow the existing members
  and have defaults.
- Existing named reads/writes of `type` and `value`, including KEY emission,
  retain their semantics.
- `sizeof(RecoveryEntry)` and its layout change. It must not be passed across
  a binary ABI boundary or treated as the native 32-byte BC layout.
- Code using two-element structured bindings will need adjustment because the
  aggregate now has five members; no such use was found in the targeted source
  search.
- This is only a model capability. It does not connect PPV parsing to
  `loadSimCtrl`, alter PPV Save, change list ownership, mutate flags, or remove
  the public unsupported guard. No custom deletion was added.

## Evidence basis

`analysis/resume54-ppv-owner.md` reconciles the PPV caller's OPI `+0x20`,
data-root `+0x19a8`, and OPI `+0x28` arguments with the KEY writer's use of
those same native BC lists and flags. `analysis/resume52-ppv-dumpbin.md`
documents the first-match equivalence key (two exact integers and two strict
`1e-6` double tolerances). The native PPV constructor initializes BC `+0x18`
to zero before the reader overwrites only `+0`, `+4`, `+8`, and `+0x10`.

No build, GUI, mutable REA operation, or loader/Save integration was performed.
