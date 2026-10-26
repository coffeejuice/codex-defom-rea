# CP196 QObject import and Report-call binding

## Result

The retained original PE evidence binds the exact Report TaskBase call through IAT slot `0x141e041c0` to the public QObject constructor `??0QObject@@QEAA@PEAV0@@Z` in `QtCore4_64.dll`. The bounded export listing begins at `0x671313d0`, matching that constructor's export address recorded in the PE import/export qualification. This is a static import/export and instruction-byte result; it does not establish execution.

The Report callsite sets `EDX` to zero at `0x141403222`, reloads only `RCX` at `0x141403224`, then calls through the IAT at `0x141403229`. The retained contiguous instruction listing contains no intervening RDX write. Therefore the selected Report prototype caller supplies a null second argument to this public constructor. This supports the caller-nullness distinction in `resume196-parent-slot-issuer.md`; it does not qualify generic nonnull-parent behavior or runtime state.

## Identity and evidence

Read-only SHA-256 of the exact retained PE inputs:

| File | SHA-256 |
| --- | --- |
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |

The original-image import-directory receipt `analysis/resume46-doe-input-imports.json` identifies IAT `0x141e041c0`, DLL `QtCore4_64.dll`, and symbol `??0QObject@@QEAA@PEAV0@@Z`; its stated qualification is import-slot identity only, with no execution or semantic-equivalence claim. The existing bounded QtCore export-entry listing and qualification `analysis/resume163-connector-qobject-671313d0.asm` / `analysis/resume163-connector-qobject-static.json` identify entry `0x671313d0` in the same QtCore image. The ownership contract `analysis/wave2-doe-task-public-qobject-ownership-contract.json` records this mapping as `actual_export`.

The callsite artifact `analysis/wave2-doe-task-base-public-qobject-call.asm` is a contiguous dump of `0x141403210` onward. Relevant bytes:

```text
0x141403222  33 D2                 xor edx,edx
0x141403224  48 8B 4C 24 40       mov rcx,[rsp+40h]
0x141403229  FF 15 91 0F A0 00    call qword ptr [0x141e041c0]
```

The retained constructor-entry qualification gives QtCore image base `0x67000000`; `0x671313d0 - 0x67000000 = RVA 0x1313d0`. The PE import lookup establishes the symbol/slot, and the bounded entry capture establishes bytes at the stated public export address. The two findings agree on the requested binding.

## Limits

This check consumes the retained, hash-qualified static PE receipts and instruction listing; it does not independently parse or execute the images in this report. It does not prove Windows loader resolution at runtime, constructor execution, dynamic rebinding, reachability of the caller, or any parent object/private-slot contents. The null argument conclusion is limited to the statically listed instruction sequence and the x64 register semantics of `xor edx,edx`; it says nothing about other QObject callers. No application, GUI, build, test, or REA database was run or changed.
