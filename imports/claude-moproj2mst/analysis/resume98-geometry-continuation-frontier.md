# CP98 geometry continuation and step-generation frontier

**Finding: no later continuation consumer or source-supported step allocation generation exists. Stop at the permanent pending barrier; do not add a speculative generation counter.**

## Retained-source trace

`GeometrySyncBarrier` is private to `viewer.cpp`. Its only construction is the low-directory-record branch; the frame is populated before install, then CP97 installs it and records the query result, post-version, and `QueryCommitted`. A source-wide search finds no later read of `frame->step`, stage, query outputs, receipt, or other saved frame data. `geometrySyncBarrier_` is only observed for presence by `GeometrySyncAccess::pending/requireAvailable`, installed once by `GeometrySyncAccess::install`, and checked by `GuiModel::requireGeometrySyncAvailable`. There is no resume, retry-consumer, retirement, or clearing path. Later geometry attempts refuse before dereferencing historical metadata. `GuiModel::reset` also refuses while the barrier is present.

This reconciles CP97's assumption: its borrow is valid only for the same synchronous initial invocation. It is not a way to authorize a future callback. CP97's public `GuiModel::step` remains a `std::unique_ptr<DB_STEP_DATA>`; construction initializes it once, and the normal production source search found no assignment, reset, swap, or release of that member after construction. That makes the synchronous path coherent in the current implementation, but does not establish an observer for public/external replacement of the member. Public code can still reset or replace the unique pointer without a model-mediated notification, and the saved address cannot distinguish a later allocation at the same address.

## Smallest safe conclusion

There is no narrow implementation increment for a resumed continuation until a real consumer and its required native effects are recovered. The exact missing witness is a continuation callsite that consumes the parked frame on a later invocation, with source/native evidence for how it revalidates the same model, live step allocation, Viewer/View/item, DB owner/reader, and current directory receipt before touching any saved pointer or repeating the query. Without that caller there is no meaningful allocation-generation check to implement or focused behavior to test.

If such a consumer is later established, the current public `unique_ptr` is not a generation seam: a private counter only advanced by new setters would miss direct public replacement. Any observer design must either intercept every supported replacement/destruction or avoid retaining and dereferencing the old pointer, and it must handle address reuse. That broader ownership/API migration is unsupported by current evidence and must not be implied by CP97's endpoint identity test. That test catches ordinary replacement during the same refresh but cannot prove lifetime across later invocations or exclude transient ABA.

No source, test, build, GUI, original execution, or REA mutation was performed for this audit.
