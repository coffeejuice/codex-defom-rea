# CP159: Stopping cache-miss timer plan review

Source-ready for the proposed two timer calls followed by CoordinateSync refusal before loader entry. No blocker was found in the bounded plan. This is a committed partial native prefix, not cache-miss getter completion or public Save admission.

Only this report was written. No source/test/CMake edit, build/test, GUI/original execution or REA operation occurred. Review covers `resume159-editor-cachemiss-evidence.md` SHA-256 `317df60b47552d3a64183a0861fa9d51a3bafe3631d3d794eddbd909b2ed79d8`, the six ASM artifacts, current Stopping source and existing StepTimer implementation.

## Native order and actual timer owner

Reader82d7d0 checks selected cache11 after module selection and id range. Cache0 reaches LEA literal1427a6e6c, LEA global143dbe000, then start832340 at82d895. It next passes null sub, literal1427a6e78 and the **same global** to checkpoint832370 at82d8ab. Only afterward does82d8c0 invoke828060(S,&id). The later arrays-timer checkpoint at82d8d6 belongs to different global143dcdae0 and is outside this prefix.

Independent PE string reads confirm the first two labels are exactly `Start` and `ReadSingleAArrayFromDB`. Initializer141d32af0 constructs143dbe000 with flag0 and the node-reader timer name, matching existing `g_timer_read_obj_node_state_variable`; this is not a per-page timer or the arrays timer. No new timer/state owner is needed.

Start832340 first calls reset8324a0, then checkpoint with null sub. Reset changes count only. Checkpoint samples time, stores low32 time at4ee8+4*i, copies label+NUL atc8+200*i, stores sub at5078+8*i and increments signed count5398. Its condition is label!=null/count<100, without a flag gate. Start resets before indexing, so this prefix has no prerequisite on the timer's old count, even if negative or>=100. Count after both calls is2. Name/flag/padding, unwritten label tails and slots2..99 survive.

Existing StepTimer methods match those structural effects. Its actual host CRT clock and native-shaped conversion are an expressly qualified semantic adaptation; preserve samples as opaque values. Do not invent timestamps, assert cross-process epoch equality or strengthen tests to require monotonic values. Native OOM/EH/thread continuity is not recovered by this reuse.

## Exact source seam and stopping boundary

Current Stopping combines bad MAR/id bounds and cache0 into one refusal. Split those decisions without moving the existing owner/COW/selection prefix:

1. Keep actual same-step N owner, positive count, exact canonical variables, zero sync byte,3D and signed-high-version qualification from CP158.
2. After real allocation/key0 COW and actual module selection, invalid MAR index or signed id limit still refuses before timer access.
3. Nonzero cache returns through the existing cached path unchanged.
4. Valid cache0 calls actual node timer start/checkpoint and returns CoordinateSync before invoking loader or reading companion owner/body.

The production path must retain these effects on refusal. Every genuinely delivered fresh cache0 invocation resets/writes its first two slots again; do not add a resume cursor or force a callback when remembered node-spin state would not emit one. Same-value completed Fill retains its zero-getter behavior.

Null companion does not prove whole-reader completion: native loader can return normally, but reader then checkpoints the arrays timer and enters later object assignment. Opaque nonnull companion immediately lacks a bounded Stopping-to-directory owner join. Both controls should stop identically before loader under this cut, without probing the DB pointer. No query/version/A/cache write, arrays checkpoint, Stop/report, object lookup or decoded-body copy is authorized here. The other four ASM receipts correctly qualify these later dependencies; they are not extra implementation admission.

## Required tests and file scope

The existing Stopping source/test pair is sufficient; no shared header, lifecycle or timer-library change is needed. Test RAII must restore actual process-global tables and all observed timers; production must never restore them.

Seed a distinguishable node-timer history and show the forwarding real COW dispatcher sees the old bytes before timer entry. After valid cache0, assert count2, exact two labels/null subs, and unchanged name/flag/padding, label bytes after each new NUL, later slots and other timers. Do not compare the two sampled times with invented expected constants. Cover initial module dimensions0/2/3 and preservation of altered already3 tables.

Cached1/-1, invalid MAR-1/140 and signed limits11/0/-1 must retain byte-identical timers. The same applies to foreign owner, noncanonical registry, parked/no-node and old fast profiles. Cache0 must retain one getter/dispatch/allocation prefix, unchanged cache/version/MAR/A bodies and reached widget state. Test null and safely restored opaque companion pointers without dereferencing or adopting them; restore sentinels through RAII before destruction even on a failing assertion.

Repeated **freshly delivered** miss and cached→miss→cached cases must distinguish timer reset from append/Stop and must not erase real table/COW history. Existing CP158 assertions that all misses leave timers unchanged need an intentional narrow expectation update; invalid-index and cached negatives must remain unchanged. Private constructor cache0 still stops InitialFill/Stopping, before later phases, with public completeness false. No physical allocation failure, fake clock or original-runtime test is required to qualify this prefix.

## Independent evidence binding

Original DEF_GUI_MO_64 v11.0.2 SHA-256 independently matches `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All six published native-byte hashes **and all six ASM text hashes** were independently recomputed and match the proposal: reader82d7d0..82dc8b, loader828060..8283d5, query825440..825532, start832340..83236f, checkpoint832370..83242f and allocator9c83b0..9c8408. No rejected/misaligned range is used for this decision.

Source StepTimer.cpp hash independently matches `f72dd675db7b7028afe957628497708e67d6b215eba32edbf643d96634afb136`; header matches `8a3fceb49f24342f1ff83b5b8a8ec211eb041bc366889c9c9c17411da454dcc9`. Current seam is CP158 simcontrol300.cpp `3435741e29068aea9e336069f5388efe68a5056b1d54d70d626b6f0b385508d0`. CP71's retained owner/clock qualification agrees. No reconstructed executable/deployment was inspected. A released source patch still needs independent review and root compiler/test binding.
