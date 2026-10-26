# CP158 cached Stopping node Fill implementation

The private Stopping Fill now completes the original high-version coordinate getter when the actual id0x0b reader is cached. It preserves the reached allocation/key0 COW effects, then selects the actual module tables and checks real bounds/cache state before returning the current coordinate pointer. Cache misses remain CoordinateSync before timers or DB/body work.

Source/test ownership is released to root for grouped build and independent review. Only these two source/test files and this receipt were written; no lifecycle/interface/CMake/public guard edits, build, CTest, GUI, original execution or mutable REA operation occurred.

## Bound implementation

Frozen contract: `analysis/resume158-editor300-next.md` SHA `b04d632bdacb2e04be227e4a276220ffccd41b05784e335a332334aecf244c41`. Independent accepted plan: `analysis/resume158-stopping-cached-plan-review.md`. The report binds the retained original GUI SHA, complete native reader/outer normal-function hashes, actual getter chain and CP69 table producer. No new target execution or EH/allocator-failure equivalence is claimed.

`stoppingCoordinates` keeps its old allocation and real `f140329e60` call. Immediately afterwards it rechecks actual node/step identity. The newly admitted high-version path requires positive set.n and the ordinary28 registered IDs/dynamic types using nonmutating list reads. Only then it invokes real `select_index_tables(&dimension)`; current3 preserves actual cells. `g_mar_index[3]` must be in0..139, signed `g_a_index[128]` must exceed11, and actual step.u26c4[11] must be nonzero. Marker-1 is valid. Invalid bounds or cache0 retains committed allocation/COW/selector effects and refuses before reader timers, DB, body copies or cache writes.

The unused native MAR address calculation is folded: no MAR/A pointee is touched or invented. Null MAR/A bodies remain permitted. Native cached AL=false is ignored by outer which0 even at signed versions>=11000. Each reached coordinate getter still performs its own real dispatch; no memoization or extra replay is introduced. Old non-sync routes, index/finite/owner guards, changed-spin callbacks, formatting order and public completeness flags are retained.

A static comparison against CP157 `build/wave3-checkpoint157-verified/source.zip` confirms the entire production file is otherwise identical after removing the five added includes, the new anonymous qualifier and the replacement of the old final synchronization refusal. Initial comparison mistakenly compared normalized source newlines with raw zip CRLF; normalizing both resolved that tooling mismatch. No source change was made to satisfy that check.

## Focused tests

NEW `test_simcontrol300_stopping_cached.cpp` has four Testkit cases and one main:

- 36 actual Fill combinations: versions6000/11000/INT32_MAX, cache1/-1, allocated/new coordinates and first/second/both references. Checks8/4/20 getters and real dispatches, one shared COW detach, source arguments, old global dimension during the first real dispatch and3 on later dispatches, exact stored coordinate text, unchanged model/SC values, null MAR/A and same-value repeat zero getters. The observation hook always forwards the real default dispatcher and restores itself on unwind.
- 21 module/pending combinations: initial dimension0/2/3, valid altered cells, invalid MAR-1/140, signed limits11/0/-1 and cache0. Checks actual selector behavior, first-prefix allocation/COW and partial widget retention, no cache/body changes and byte-exact unchanged reader timers.
- Changed ID, wrong dynamic type and foreign owner controls refuse at their reached boundaries; restore borrowed fixture pointers/IDs before actual object destruction. No native list repair is performed.
- Private constructor cache-hit/cache-miss controls with actual model/object/node owner and explicit label/locale observations. Hit completes the supported existing private page lifecycle, retains same-step node storage through remembered Fill and Apply/repeated Advanced remaps, and restores rounded MDS values. Miss stops InitialFill/Stopping before temporary-copy/extra-I/O. The public completeness flag remains false.

Test-only RAII restores prior module dimension/MAR/A cells and reader timers. Production never resets those process globals. Tests do not force allocation exhaustion, emulate an original GUI owner or claim public Save success. Existing snapshot/lifecycle negatives remain unchanged and should run in the grouped gate.

## Static checks and frozen files

Performed only source review, include/declaration/signature/const-lookup checks, scoped CP157 source comparison, four-case/one-main inventory and whitespace check. In particular `fun_14032a160` uses const CowList::at; it does not detach. No compilation or test execution was performed, so root still owns the executable validation.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300.cpp` | `3435741e29068aea9e336069f5388efe68a5056b1d54d70d626b6f0b385508d0` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_stopping_cached.cpp` | `cb2a84a88d16626e871c49e2c84b42ceaa8c837a01c58bed07474b1644c3d634` |

CMake's existing per-packet CONFIGURE_DEPENDS discovery selects target `m2m_mdt_simcontrol300_stopping_cached`, CTest `m2m.mdt.simcontrol300_stopping_cached`, linked to m2m_core/testkit. Required existing regression targets are `m2m_mdt_simcontrol300_snapshot` and `m2m_mdt_simcontrol300_lifecycle`; their source is unchanged. Source is frozen after this receipt.
