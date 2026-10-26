# CP207: static predicates for the `45780` null returns

## Scope and evidence

This is a converter-only static read of the copied original PE described in `analysis/resume205-rea-crash-chain.md`. It does not use REA, mutate a database, launch the application, or acquire GUI control. The copied image is the SHA-matched `evidence/original-binaries/DEF_GUI_MO_64.exe`; the bounded DUMPBIN slices retained as `analysis/wave3-closure-next-140045780.asm`, `analysis/wave3-closure-next-14001d510.asm`, and `analysis/wave3-boolean-close-1400133a0.asm` show the call and branch instructions summarized below.

## The two zero arms

`1d510` passes its incoming pointer to `133a0`, then passes `133a0`'s return in `RAX` as `RCX` to `45780`. In `133a0`, the only call shown is `146f8fc` with `RCX = [incoming+0x50]`, `EDX=0`, and two fixed addresses in `R8`/`R9`; the return value becomes `133a0`'s return. This establishes the syntactic input feeding the first `45780` call, without assigning a C++ receiver type or a semantic name to the lookup.

At `45780`:

1. It calls `c78ad0` with its incoming `RCX`. The return is saved and compared with zero (`4578e`, `45793`, `45798..457a2`). If zero, the helper returns zero immediately. Thus the first null predicate is **`c78ad0(input-from-133a0) == 0`**.
2. If that result is nonzero, it calls `146f8fc` with that result in `RCX`, `EDX=0`, and fixed pointers `0x143569448` / `0x143569470` in `R8` / `R9` (`457a4..457c1`). The return is saved and compared with zero (`457c6..457d5`). If zero, the helper returns zero. Thus the second null predicate is **`146f8fc(first-result, 0, fixed-arg-1, fixed-arg-2) == 0`**, gated on the first result being nonzero.

When both results are nonzero, control reaches `44a90` with the second result as `RCX` (`457d7..457dc`); that path is outside this request. These observations describe explicit zero-return paths, not proof of which was taken in CP205.

## What B98 evidence can establish

The CP205 capture qualification identifies the `C:\PROBLEM\p202a\Gear_blank.moproj` launch and records a byte-identical 27-file disposable fixture manifest before and after. That establishes fixture identity/integrity for the captured run. The fixture and retained static metadata do not record the runtime values returned by `c78ad0` or the second `146f8fc` call, nor enough pointer state to derive those values from file contents. No inspected static record maps the fixed lookup arguments to a B98 file predicate or proves that either lookup must be null for this fixture.

Therefore existing B98 material cannot establish either zero arm. It also cannot rule either arm out. The CP205 fault proves the later loaded owner field was zero at the faulting read path; it does not prove that `45780` produced that zero rather than a later state change.

## Minimum same-run operands if static proof is insufficient

To discriminate the two arms in one occurrence, capture the `c78ad0` return at `45793` and, only when nonzero, the second lookup return at `457c6`, associated with the same invocation of `45780`. To establish whether this producer explains the later fault, also capture the returned `45780`/`1d510` value at the store to `[O+0x30]+0x38` (`eb00f`), the exact `O` and `H=[O+0x30]` used there, and the corresponding `H` plus `[H+0x38]` at the later load (`eb1e3`), in the same run. Without these observations, static code plus fixture bytes cannot distinguish an initially null producer from a subsequent clear/replacement or owner change.
