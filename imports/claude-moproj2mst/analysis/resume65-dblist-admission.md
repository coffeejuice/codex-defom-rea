# Checkpoint 65: DBLIST admission facts after Session binding

## Result

The retained DoeOutput Type 2 load chain establishes the original caller's
sixth DBLIST argument as zero for the recovered route. This removes one
unknown condition, but the DBLIST child is dispatched only if a second,
independent runtime gate also passes. The retained evidence does not bind
that gate, or the later warning-suppression state, to the candidate's actual
`ConversionSession`. The CP64 `SessionImportContext::actualSession` is an
identity pointer, not a recovered representation of either native global
field. Keep the public B127 DBLIST refusal unchanged.

## Caller argument trace

The retained `FUN_140e2ec40` decompile calls
`FUN_140644440(lStack_278, 1, 0, 0)`. The latter's retained signature is
`FUN_140644440(longlong param_1, char param_2, undefined1 param_3,
undefined1 param_4)`. Its DOM traversal forwards `param_3,param_4` unchanged
to `FUN_140659d90`. That function's recovered call to the shared task-I/O
loader is:

```text
FUN_140650e60(uVar3, uStack_10, uStack_20, 0xffffffff, param_4, param_5)
```

Thus the sixth argument received by `FUN_140650e60` is `FUN_140659d90`'s
`param_5`, which is `FUN_140644440`'s final `param_4`. On the stated
Type 2 DoeOutput route, it is literal zero. At `0x140651eaa`, the caller
tests this byte and skips the DBLIST handler only when it is nonzero. The
byte therefore permits continuation on this route; it does not alone admit
the handler.

## Remaining independent native state

After the zero-byte test, the caller invokes
`FUN_1402cbec0(0x143d766b8)`. Its retained predicate reads the DWORD at
`global + 0x1c` and returns true exactly when that DWORD is zero. Only a true
result reaches the corrected `DBLIST -> FUN_1406554a0` dispatch. None of the
retained DoeOutput call arguments or CP64 owner-binding facts supplies this
DWORD or qualifies it as a field of `ConversionSession`, `Project`,
`SessionImportContext`, or `SessionNotificationState`.

The missing-file warning has a separate, later admission state: the DBLIST
handler reads the DWORD at `global + 0x0c`; value 5 suppresses the warning
after existing-path manager work and missing-path accumulation. The source
binding does not supply this value either. The `global + 0` main-window
pointer used as a QMessageBox parent is also not derived from the candidate
Session; the CP64 report explicitly notes that a ModelManager owner is not
a QWidget identity.

Do not infer either global DWORD from the literal-zero caller byte, the
presence of `actualSession`, Save mode, active subproject, manager binding,
or notification suppression. The byte is a per-call parameter; the two
DWORD reads are distinct process-global state. A safe release would require
retained evidence of the globals' owning object/setter and a candidate-side
binding that supplies the same runtime values (plus the needed widget and
manager/path behavior for full warning execution). Those facts are absent
here, so no source or test changes are justified in CP65.

## Bounded evidence retained and hashes

The exact excerpts used above are already present in the following immutable
analysis receipts; they were read without reopening the binary or running
REA:

| Evidence | SHA-256 |
|---|---|
| `analysis/wave2-doe-type2-session-load-dom-decompile.json` — `FUN_140e2ec40` call with `(1,0,0)` | `fb638b8e6574306a37cd1987842c7eb73a4823859625b0b6de07227839f7a80e` |
| `analysis/wave2-doe-session-postparse-final-order-decompile.json` — `FUN_140644440` signature and forwarding | `f555a03ded278e63ede4a2205660f9ee62e9ab11125dc9f0526171375d4112f4` |
| `analysis/wave2-doe-session-root-task-io-decompile.json` — `FUN_140659d90` to `FUN_140650e60` forwarding | `fe234e79d42ea999d4067d27bea3c99eb876f29321f87d9c122ecaa7e8e6f6b5` |
| `analysis/resume62-dblist-register-and-caller.asm` — caller byte and global gate at `0x140651eaa` | `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1` |
| `analysis/resume62-dblist-owner-leaves.asm` — `global + 0x0c` warning read and related leaves | `86dc35ca471d9e6fc541d05b48c2ce3f2206879a739bf8eb0f7ed5c5beb0a2ac` |
| `analysis/resume64-dblist-session-binding-implementation.md` — candidate actual-session identity scope | `0e1f046a6d57de1e06f940c3acdcc726aa44a7ce0f7af320dd85923531bf7d8c` |

The original binary qualification recorded by the warning receipt is
`evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The decompile trace establishes a literal argument for the recovered route,
not every possible call profile or the runtime values of the global state.

No source, build, test, GUI, project, or mutable REA database was changed or
run for this checkpoint.
