# CP116 — DBLIST setter and AppState construction slice

## Finding

The narrow constructor call graph proves a startup reset that earlier summaries missed: AppState constructor `0x1402c7f80` stores DWORD zero at `this+0x1c` (`0x1402c8030`). Its caller chain proves `this` is the DBLIST gate singleton `0x143d766b8`: CRT initializer `0x141ca0bb0` loads that exact address into `rcx`, calls `0x1402c7ee0`, which forwards the same `this` to `0x1402c7f80`. The constructor therefore leaves singleton `+0x1c` zero at completion of the pre-main AppState initialization.

This corrects the limited prior statement that the retained AppState constructor prefix did not establish a `+0x1c` store. The earlier prefix ended before the later store at `0x1402c8030`; it was not a complete constructor-field inventory.

The setter lifecycle is still not closed. The helper `0x1402cbf30` writes one at its receiver `+0x1c`, but no direct caller has been found and there is no receiver binding showing `rcx == 0x143d766b8`. Consequently the value is known zero immediately after the native global constructor, while its value at later DoeOutput dispatch remains unresolved if any later path invokes the setter or performs another write.

## Static call chain and field writes

1. CRT initialization table slot `0x141e0c950` (index 31) contains `0x141ca0bb0`. The retained table bytes and initialization walker are recorded in CP106.
2. `0x141ca0bb0..0x141ca0bd0` loads singleton `0x143d766b8` into `rcx`, calls `0x1402c7ee0`, then schedules the registered global destructor. This is an initializer occurrence before native `main`, not an Open/Close event.
3. `0x1402c7ee0..0x1402c7f36` prepares the default QString member subobjects and forwards its original `this` in `rcx` to `0x1402c7f80`.
4. In `0x1402c7f80..0x1402c8056`, the constructor initializes other application fields, then at `0x1402c8030` executes `mov dword ptr [rax+1Ch],0` where `rax` was loaded from the saved `this`. The zero store is a direct owner-qualified write, unlike the unrelated same-offset zero initializer on global `0x143d763a0`.
5. The setter body `0x1402cbf30..0x1402cbf41` remains the already captured `mov dword ptr [rax+1Ch],1` with `rax=rcx`. Retained whole-image DUMPBIN scans found no direct call to this entry; the expanded CP113 local window also found no same-register `+0x1c` store within 63 instructions after any exact `lea reg,[0x143d766b8]`. These searches do not cover arbitrary aliases or indirect dispatch and therefore do not identify the setter's caller.

The constructor reset means a new native image's initial zero-fill is not the only reason the field is zero at startup: the explicit AppState construction also writes zero. Neither fact proves a zero value later in the process. The independent `+0x0c` process-type field is initialized/refreshed separately and cannot predict setter history.

## Acceptance boundary

For a strictly fresh native startup prefix that reaches the CRT initializer, the source-backed initial AppState observation can now include `+0x1c = 0` **after** `0x1402c7f80` completes. This finding alone does not qualify a candidate public DBLIST integration: the setter's post-constructor caller, whether it ever targets this singleton, and any later writes/resets remain unknown. Keep the public parser guarded until a later same-instance import value is determined, or until a precisely bounded fresh-startup-to-first-import path proves no intervening writer can occur.

The smallest next action is not another generic singleton-xref scan. It is to identify the entry point/target construction of `0x1402cbf30` (including any relative or indirect table target) and then trace that occurrence's receiver and lifecycle; if that remains unbound, obtain a same-instance startup-to-DoeOutput trace of this field. No evidence in this slice supports setting the candidate field to zero on each conversion run.

## Range hashes and evidence identity

All raw-range hashes below were computed from original PE bytes using the PE section mapping (ImageBase `0x140000000`, `.text` RVA `0x1000`, raw pointer `0x400`) and corroborated by bounded DUMPBIN disassembly. End addresses are exclusive.

| Original PE range | Role | Raw file offset / length | SHA-256 |
|---|---|---:|---|
| `0x141ca0bb0..0x141ca0bd0` | CRT global initializer caller | `0x1c9ffb0 / 0x20` | `f7997584131260a65332e3867f0814ffc94a144a3d5aaec85fe81c00a8551034` |
| `0x1402c7ee0..0x1402c7f36` | constructor/member-subobject prefix forwarding `this` | `0x2c72e0 / 0x56` | `ae91064418d3a57bd7ee4ac32066551ceb25176be39bd7c7270eeafdbbd979cd` |
| `0x1402c7f80..0x1402c8056` | AppState constructor including `+0x1c=0` | `0x2c7380 / 0xd6` | `b287a17161767c8d2b5d2075262e74a036301d292a861e9a0512c1d557dc3a7f` |
| `0x1402cbf30..0x1402cbf42` | Setter body, including return byte | `0x2cb330 / 0x12` | `de177617ea3d0cac94f5f5be7737aa5b55817bc9757f23ac37bfcfad6fb5f3ed` |

Controlling executable SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. DUMPBIN path and binary SHA-256 are recorded in `analysis/resume81-dblist-setter-pe.md`; CP106 records the CRT table/caller. No REA session was opened for this slice. No mutable analysis database, original runtime, GUI, source, or build was used.
