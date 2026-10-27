# CP449 safe-boundary efficiency review

Three recovery checkpoints followed the CP446 review: CP447 and CP449 were static evidence, while CP448 changed source. The successful CP448 grouped build measured 87.269 seconds and its one bound 374-test launcher 22.047 seconds. The initial CP448 focused compile failed on a missing test declaration before the corrected grouped run. This was useful early detection but still cost an extra configure/compile attempt; preflight cannot replace an actual compiler check.

The CP450 Boolean test-only candidate, already underway at this review boundary, measured 20.569 seconds for configure, focused preflight and full incremental build; its bound launcher measured 25.391 seconds. A separate diagnostic focused build preceded that grouped run and duplicated a small amount of work. For later test-only changes, use source review followed by one grouped builder with focused CTest, then one bound launcher and freeze. Extra focused builds remain appropriate to diagnose a concrete compiler/runtime failure.

These are wall-clock measurements for different source-change sizes and cache states, not a controlled speedup comparison or a token-cost result. Keep raw fixture hashes, diagnostic differences, candidate freeze and public guards intact. No change to original-app capture or GUI-control policy follows from these timings.
