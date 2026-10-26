# Checkpoint 56: bounded DBLIST assembly contract

## Inputs and qualification

Inspected only `analysis/resume56-dblist-handler.asm`, `analysis/resume56-dblist-gate.asm`, and the retained `FUN_140650e60` call-site decompile in `analysis/wave2-doe-session-valid-existing-input-decompile.json`. The captures name the imported `evidence/original-binaries/DEF_GUI_MO_64.exe`; the related call-site receipt binds that original to SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This is static binary evidence, not an original runtime observation.

## Gate decision is now exact

At `0x140650e60`, the original code passes global pointer `0x143d766b8` to `FUN_1402cbec0`. The captured gate at `0x1402cbec0` reads the first argument object and compares its 32-bit field at offset `+0x1c` with zero. It returns byte `1` when the field is zero and `0` when nonzero. Thus the DBLIST handler dispatch condition is exactly `*(int32_t*)(0x143d766b8 + 0x1c) == 0`; the ReportList branch shown in the same caller uses the same predicate but a different handler. The field's domain meaning and which lifecycle states make it zero remain unknown. Do not substitute a headless-session predicate without separate evidence.

The gate's target function is complete in the capture: it returns at `0x1402cbeeb`; following bytes are padding and subsequent functions. The rest of the gate dump contains unrelated neighboring functions and ends partway through a different function, which does not affect coverage of `0x1402cbec0`.

## Handler: established work and unresolved owner

The call-site gives `FUN_140655df0` two arguments: the caller's part-level inner-model pointer (`param_1` in `FUN_140650e60`) and the direct DBLIST DOM node. The assembly confirms those incoming arguments are saved and that the model pointer is used in indirect calls. The function then performs a substantial DOM/string preparation sequence, constructs multiple generic arguments, calls `FUN_1402cf740` with global pointer `0x143d76700` and a string constant, and conditionally invokes a virtual method at vtable offset `+0x218` on an object held in a local slot. The exact role of the global target and the returned object/method context are not identified by this bounded dump.

This does **not** identify a concrete part field as the DBLIST owner. There are no direct stores to a stable offset of the incoming part pointer in the visible instructions; the final virtual call may mutate an object reached through an unresolved indirect path. A faithful port must not yet attach the parsed DBNAME to `SessionPart` or another guessed field.

The handler has a common visible cleanup/return path. At `0x14065561aa` it sets the returned byte to `1`, then cleans local objects and returns at `0x14065561eb`. Both the early branch to that block and the virtual-call path converge there; no visible branch writes a false return value. This establishes the ordinary visible return value, not behavior under exceptions or inside the called virtual method.

## Side-effect and callee boundary

The bounded body shows direct calls to `0x140025080`, `0x1400ac6f0`, and `0x1402cf740`, indirect import calls, and the final virtual call through vtable `+0x218`. The initial indirect calls receive the part pointer and/or DOM-related local values; the assembly alone does not give reliable high-level names for these targets. The `0x1402cf740` call receives locally constructed string/generic-argument data. No direct filesystem API call is visible in this function body. That is not proof of no side-file effects: the unresolved direct/indirect targets and virtual method may perform them, and their bodies were not part of these captures.

Coverage for the handler is complete: its return is at `0x14065561eb`. The bytes through `0x14065561ff` are padding; another function begins at `0x140656200` and returns at `0x14065620d`, followed by padding. The capture to `0x140656220` therefore includes the complete handler plus the next small function, not a truncated handler.

## Consequence for candidate work

The detached direct-child reader remains a safe structural seam only. The assembly narrows the gate and shows that native handling uses dynamic/virtual dispatch, but it still does not establish the owner field, DBLIST format validation, target method semantics, side-file behavior, or exception behavior. Keep public Type2/active B127 dispatch guarded. The next evidence step is to identify the `0x1402cf740` dispatch target and vtable `+0x218` dynamic receiver/implementation (and its relevant callees) from static targets, under the same original-binary hash qualification. Do not infer success/failure side effects from the candidate-only B127 probe.
