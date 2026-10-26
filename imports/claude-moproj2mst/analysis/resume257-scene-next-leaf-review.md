# CP257 scene-refresh next leaf independent review

## Verdict: PASS

The proposed `fit` leaf targets the actual omission: retain the shown-main refresh, then visit shown secondary and shown third in that order before the later scene-box/radius/camera work. Current `fit` refreshes only main. The retained native `0x140cfe9c0` scene-camera path (`Display+0x708 == 0`) has three separate guard/flag/call sequences in main, secondary, third order, followed by the global scene check and box/camera work. The proposed code uses the existing display guards and shown flags, keeps independent slot visits even for aliases, and leaves named models and `sceneBox` union untouched.

The pending-refresh qualification is correct. Current `viewerRefresh` enters `refresh`, which returns immediately for an installed geometry barrier; other refresh paths may also return before full view synchronization. The proposal routes each slot through that existing behavior and correctly rejects a new per-slot preflight or invented stop/continuation condition. A parked barrier therefore is not a deterministic throwing refresh fixture and cannot support a claimed partial-prefix exception test.

The suggested test seam is meaningful and appropriately caller-level. `displayRefreshScene` performs its existing admission/display-show then calls `fit`. The existing `geometryLine` helper binds curve-only 3D items and the cited cache test confirms that mutating bound curve coordinates changes a per-model cached box only after refresh. Distinguishable main/secondary/third cache extents can verify the shown flags independently without relying on the still-incomplete final camera box union. The plan distinguishes stale cache reads from refresh calls, keeps local third-model ownership alive, tests expired guards without dereferencing destroyed models, and makes named-model coverage optional. It does not treat per-model cache refresh as proof that secondary/third boxes contribute to final camera values.

The report explicitly keeps `sceneBox` union/radius arithmetic, reset-view selection, pending geometry continuation, aliases/lifetimes outside the bounded prefix, and CopyMirror replacement behavior separate. The retained PE/decompile hash and source qualification establish static native call order, not a fresh secondary/third occurrence, Qt reentrancy, or whole camera parity. The proposed source/test ownership and focused target are clear; no tests or builds are claimed.

No source, tests, build, GUI, or mutable REA state was changed or run for this review.
