# Resume 49: Original two-reheat Cogging contract

## Static source qualification

The original executable identity in the retained Cogging writer evidence is `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (`analysis/wave3-cogging-native-io-contract.json`). The bounded retained assembly `analysis/wave3-cogging-verified-1407617e0.asm` starts at native `0x1407617e0`; it is the Process Data writer and shows a call to `0x14076a8c0`, the master-data writer. It does not include the loop body at `0x14076ac03..0x14076ac72`.

The best retained address-level analysis for that loop is `source/docs/re/moproj2mst/mdt/spec.md`, section 3, cross-referenced by queue C10 in `analysis/closure-audit.json`. It says the native master writer outputs SIMLST, then `NoReheats`, then enters the REHEAT loop; the pointer is cumulative, with 400-byte REHEAT records and zero-based destination index `n - 1 + i(i + 1)/2`. C10 explicitly qualifies this as retained static evidence, not execution observation, and says undefined output must not be called reproducible. No retained disassembly artifact in the current Cogging analysis set contains instructions at the loop addresses themselves. Therefore the loop formula is an existing reverse-engineering conclusion in the spec; it is not independently re-decoded from PE bytes in this item.

From that retained formula:

- `n=0`: the loop has no iterations; there is no REHEAT record dereference.
- `n=1`: its only destination is element 0, inside the one-element array.
- `n=2`: iteration 0 targets element 1, which is inside the array and is the second reheat while the output record label is `Reheat 1`. Iteration 1 targets element 2, outside the two-element array. The invalid read's contents, any subsequent formatting, process termination, and final/partial file bytes are not specified by the static evidence.
- `n>2`: the triangular advance grows faster than the available contiguous elements; the loop eventually addresses outside the array. Do not extrapolate a stable output or failure mode.

## Port comparison

The current writer `source/apps/moproj2mst/src/datafiles/mdt_write.cpp::masterWrite` validates a positive count against vector size, emits the Simulation Data count and lines, writes `NoReheats`, and then explicitly throws `M2M_E_UNSUPPORTED` for any `nReheats >= 2`. For exactly one it calls `reheatWrite` on element 0; for zero it writes no REHEAT block. This preserves the defined one-reheat branch and the safe no-record branch. The current refusal is a deliberate guard; it is not an original-observed error/status.

The synthetic candidate test `tests/mdt/test_mdt_write.cpp::two_reheats_refused` proves only that this source throws for a model with two allocated reheats. The new root-added test in `tests/mdt/test_cogging_save.cpp` similarly checks candidate output order/refusal. Neither supplies native execution evidence.

## Safe implementation boundary

No complete native multi-reheat serialization branch is safe to implement from the current evidence. For `n>=2`, the native loop leaves the valid array at iteration 1 for two reheats, before a deterministic file contract is established. Modeling a first `Reheat 1` block from element 1 and then guessing whether/how to stop would introduce a policy not supported by observed native effects. The deterministic SIMLST calculation and emission before the unsafe loop is bounded and already present in the port, but it does not make subsequent record output safe. Keep the explicit guard as a documented unsupported boundary until original behavior can be captured and shown stable; if a future capture varies or exposes undefined data, preserve that as a limitation rather than inventing a stable serializer.

The smallest additional PE-only evidence that would strengthen, but not resolve, the claim is a bounded disassembly export from the exact identified PE covering `0x14076ac03` through `0x14076ac72`, retaining instruction addresses and bytes and verifying the pointer update, loop index, record stride and count bound. It cannot determine the value read from outside the array. Runtime evidence for that value/output would remain exploratory and could not turn undefined memory access into a portable deterministic contract. No source, build, GUI, or mutable REA state was changed for this note.
