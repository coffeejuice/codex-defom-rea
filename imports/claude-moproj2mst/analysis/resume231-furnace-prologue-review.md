# CP231 furnace prologue review — PASS

## Review result

The bounded CP231 change preserves the required behavior. `furnaceLoadPrologue` still runs `mst::baseLoadPrologue(c, p, op)`, then `setTrans2(p, op, false)`, then `setTrans1(p, op, true)`, and only then resolves `opModel` and calls `setFurnaceLoadTnow3()`. The owner command writes only `step->sim_ctrl.tnow3 = 1` when the step exists. It does not create or replace a step, add a geometry refusal, or move the write into furnace post-load.

`engine_internal.hpp::withOpStep` is synchronous: it resolves `opModel` and passes `model ? model->step.get() : nullptr` to the callback. Its comment explicitly says it adds no geometry guard. The original furnace prologue callback null-checks before the same TNOW3 assignment. Thus the nullable no-op command matches the current wrapper/callback source. This is a source-based behavioral qualification, not a claim from original runtime capture or fresh original execution.

The focused test extension uses the existing Testkit `heat1_as_furnace` case. After checking that the full load set `tnow3` to 1, it temporarily moves the model's `unique_ptr` step out, calls the command, checks the step remains null, then restores it before fixture teardown. This safely exercises the command's null-step branch without a production seam. It does not exercise a full loader call with absent `opModel`/step or establish operation ordering through a separately instrumented trace; those are not needed to test the command contract. Existing loader assertions cover the full-load TNOW3 result.

No new geometry-barrier refusal is present. Inspection of the bounded diff shows no C ABI declaration/adapter or cache1 change. The implementation report records the include/Testkit preflight and `git diff --check` as passing. Its focused Release build receipt identifies target `m2m_mdt_loader_l2`, exit code 0, and source-stable before/after tree hash `0dc18833632c231a0366f61b49465b76e6a5ae760c92ff2c99a0e727c143f155`; the report records focused CTest `m2m.mdt.loader_l2` passing 1/1. I did not rerun the build or tests during this read-only review.

## Identity check

Current SHA-256 values for all four bounded files match the implementation report:

- `engine.hpp`: `807e741eb234c933ea7bf9eb53edd0d23d8f589360bdab4fed7fe18d77b14dce`
- `gui_model.cpp`: `5bfabd17c4f4d7ee0c9ccc1929497024dd7ee685d592401d41f88990548d78d7`
- `loader_l2.cpp`: `5aa1a6e574c52dfc9e22244380f7aedf90c78f297884772fb89bc678a80fdd32`
- `test_loader_l2.cpp`: `f4da9ef211f8f9653fc20d6241a675c06308c95ab86763c517b15d6db0e72db0`

No defects found in the reviewed CP231 change. The full suite and source-stable checkpoint freeze remain outside this focused review.
