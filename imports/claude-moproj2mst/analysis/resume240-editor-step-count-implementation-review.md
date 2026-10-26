# CP240 editor StepCount implementation review

**PASS — the value projection boundary preserves NSTART ordering and prior-checkbox behavior. No unresolved defect found.** This is an independent static source/test review; root owns full integration and freeze.

Reviewed the CP240 boundary proposal and PASS design review, implementation report, the five-file working diff/current bodies, StepCount page/context definitions, three-argument fillStepCount and complete NSTART toggle tests. Unrelated accumulated FormingExpress changes in shared model files were excluded from this leaf's assessment.

## Source contract

Namespace forward declarations and reference parameters avoid requiring the complete StepCount page in engine.hpp. The implementation includes the complete existing simcontrol300 header; the declaration and const definition agree. The output contains copied page scalars and the existing optional MoStepCountContext with Operation pointer. It returns no native SimControl/interior alias, but is not wholly pointer-free or an OPI ownership solution. Comments and implementation note state this accurately.

The model method calls simCtl exactly once, preserving its single geometry guard before nullable lookup. It performs no expected-null or context precheck before that call. It requires a non-null actual control and address equality, so both-null and live/null refuse; expected is never dereferenced. It calls exactly the existing three-argument fillStepCount with the supplied prior checkbox. Empty optional returns false before any output assignment; success moves the whole page into output. Geometry/fill exceptions are neither caught nor marked noexcept. There is no duplicated projection, fresh-fill overload, separate identity preflight or escaping native borrow.

The caller retains all binding, lifecycle, page/source model/remesh, context/OPI and cached-checkbox guards. It stores operation.nstartGiven and the OperationFlagStored report prefix before constructing initialized scratch and invoking the command. False returns that prefix without publishing the page. Success moves scratch to the cached page before recording Complete, pageRefilled and Completed. Repeated deliveries still refill. A reached parked exception leaves the operation flag stored and cached page unchanged; this exception-prefix ordering is established statically here, not by a new parked owner callback test.

## Test assessment

New direct cases check live/null, replaced expected control with original allocation retained, missing actual with null and old expected, invalid fill context and all-field unchanged output sentinels. Sentinel checks include start, steps, increment, primary die, optional engagement/operation pointer and checkbox. Success checks every projected field and the existing relevant SC/operation scalars. The caller-owned context is const through the method and fill; no mutation is introduced.

Prior-history tests exercise true and false for key and null-operation branches, then writing and nonwriting behavior. Their corrected expectations agree with the existing fill: with nstartGiven one, a writing class selects operation startStepNo, but the retained native rule negates SC.nstart plus one, yielding 6 from SC -7; the checkbox is true independently of supplied prior. Key/null-operation branches preserve prior and SC -7, while nonwriting clears the checkbox. The implementation report openly records the earlier failed expectations rather than describing them as implementation fixes. No source arithmetic or class policy was changed to satisfy those tests.

The parked-viewer case supplies valid mode-five/null-operation context and null expected, requires Error(M2M_E_UNSUPPORTED) through the existing refused helper, and checks every output sentinel afterward. A premature null rejection would fail that test. It directly exercises the model method guard; it does not dynamically prove the owner flag-store-before-exception prefix. Existing owner cases retain missing/replaced-control prefix, complete page publication, repeated delivery, wrap/clamp, classes, binding/context and partial/expired owner coverage.

Testkit entry point/assertions remain valid with no C assert. New quoted include resolves to the existing header, and existing target registration suffices. No new ABI or target is introduced. simcontrol300.cpp and stopping_cached test have no working diff.

The implementation owner reports the repaired r3 focused build and 2/2 CTest targets passing; this reviewer executed neither build nor tests. That focused result does not replace root's full source-stable run, stopping-cached integration or freeze.

## Reviewed source identity and limits

The five full-file SHA-256 values match the implementation report:

- engine.hpp: `17E32959813E1DDA81812038750141D0BF3D699EDC7FE43522F82A840490C45B`
- gui_model.cpp: `78F952C019FAB6F97B7823121681CA4061BA8ED8140EEA54BA06A75BF085C170`
- simcontrol300_owner.cpp: `C93BA03B9AFA731EA2C2DFC7263A4FF23EEC67D52A103EA55E484E85177C4DB3`
- test_simcontrol300_nstart_toggle.cpp: `039A5D1E9FA21105B0741FECED5D52DD1F3F59D555835BCFE0222173FD403CDE`
- test_viewer_box.cpp: `80D13019378E9477F11F4802915702648188C6B9BEF0B136E760F25E7A21A041`

The retained CP132/CP133 evidence qualifications apply to bounded NSTART ordering; this review supplies no fresh original execution, actual Qt signal/reentrancy evidence, native enumeration, ownership closure or complete editor/Save result. Stopping/cache1 continuation remains unchanged.

Only this report was written. No source/test/build/shared recovery state, original, GUI or REA state was changed by this reviewer.
