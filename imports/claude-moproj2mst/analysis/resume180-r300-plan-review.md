# CP180 R300 next-cut independent plan review

Accept `analysis/resume180-r300-next-cut.md` for the narrow HeatTransferExpress TMAX preservation cut. No blocking findings. This is a plan review, not build or runtime validation.

## Frozen-source binding

Read CP179 `build/wave3-checkpoint179-verified/source.zip` directly, without extracting or changing it. Its `apps/moproj2mst/src/engine/save_l3.cpp` hashes to `d63ac30c8df84a84aaab71890d8b2c9ca4595692801c77674ba51cf43af9668d`; its `apps/moproj2mst/tests/mdt/test_engine_l3.cpp` hashes to `14a0eb20d17918b92c684ce2b6236d97d536e83734b31d14688eb3e4f495fba4`. Both match the plan's baseline. CP179 root integration identifies the frozen source set as `8f60ace6e8b9ec15f2f543c910ea7d7e3d638d03b5e2fa3d3ec1cc1cd6418c6d` over 3,554 files. Current working files already contain a candidate cut; they were not substituted for the frozen baseline in this decision.

## Ordering and callback assessment

Frozen `hteSaveTmax` lines 142–148 establish: absent properties return first; `propsHeatingTime(*op.props)` executes before the single `opModel(p, op)` call; missing model or step returns before SimControl access; TNOW3 equal to zero selects TNOW1 plus parsed time, otherwise TNOW2 plus parsed time. Exactly one TMAX assignment executes for a present step. `propsHeatingTime` uses the common heating-time string (or empty string), category zero and `propsUnit(pr)` in `evalUnitString`; its local `ok` is not itself checked. The plan correctly preserves that call and its possible behavior rather than adding validation or moving it after lookup.

One immediately invoked `[t](DB_STEP_DATA* step) -> void` lambda, passed `m ? m->step.get() : nullptr`, preserves this sequence. Its null return precedes binding `step->sim_ctrl`; the selected assignment remains inline, with no pointer returned, stored, queued or captured beyond the call. There must be no second lookup and no new guard. `GuiModelAccess::withStep` explicitly calls `requireGeometrySyncAvailable()` before acquisition and would therefore introduce a refusal absent from this frozen function. `withOpStep` performs its own `opModel` lookup, so adding it after the retained lookup would duplicate lookup. The proposed local lambda avoids both changes.

## Fixture and acceptance assessment

The correct repository test path is `source/apps/moproj2mst/tests/mdt/test_engine_l3.cpp`, specifically `tests/mdt/test_engine_l3.cpp` under the application, not a different engine test directory. CP179 contains `air_convect_2d_active`, using the copied `49_MO2_Boolean_Oprations` corpus, admitted `SfOprHeatTransferExpress2D`, actual cached model and properties. It is an appropriate fixture for distinct TNOW1/TNOW2 values, zero/nonzero TNOW3, fresh TMAX sentinels and expected time computed with `propsHeatingTime`.

The synthetic null-step test must restore the moved actual step on both normal and exceptional exits, then check identity and the untouched sentinel. It demonstrates the internal helper's null behavior only. The plan correctly requires focused tests, new-case execution confirmation and the complete source-stable suite before freeze. Source review must still bind final hashes and verify no other production function changed; this review does not discharge those gates.

Only this report was written. No production source, build, test execution, GUI/app-control, live REA, deployment or shared state was changed.
