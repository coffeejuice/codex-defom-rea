# CP135 independent proxy-A free review

No blocking discrepancy found in the frozen isolated proxy-free continuation. It frees the actual still-live A once through the retained candidate heap, preserves the native dangling M.proxy bits as history, and closes only the explicitly selected isolated map-destructor tail. Project/default context construction, native enclosing C cleanup and public Close remain guarded.

## Source order and ownership

Reviewed `resumeProxyFreePrefix`, its result recorder, declared-failure model, both focused test files, all older resume-entry guards and the frozen implementation/frontier receipts. Original map-destructor ASM `resume116-feature-map-unwind-tail-1412f0cf0.asm` shows inner cleanup at `1412f0d06`, actual M.proxy load at `1412f0d0c` and tail jump at `1412f0d14`; it contains no proxy-null store. Its SHA-256 is `81a3fcc307651a95473c93dda2ffb73805268a43f0bc0cc9c1c7cc7dcc49d5e7`. Frontier receipt `resume135-feature-proxy-free-next.md`, SHA-256 `c5548eb7450556dc2324cf5c7c9665c3770743f10ff0956adb29557c747eda47`, qualifies the current-heap/free/result branch at `141471b7c..141471bb9` against original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The new API accepts no external allocation/heap/report. Under owner-before-call locks it requires the canonical runtime control block, completed producer, successful H retirement and inner cleanup return, head-null/count-zero, the original A identity and its still-live backpointer. The current heap must equal the real A allocation heap. It then latches started before the kernel call and rereads the current candidate slot for this occurrence. This is candidate heap-role provenance, not observation of the original EXE global.

Raw BOOL is preserved. Zero records five events, parks before errno-provider `141471b9c`, and certifies neither A retirement nor destructor return. Nonzero records the single CRT RET at `141471bb8` as the tail destructor return. There is no invented second native return, M.proxy-null store, subsequent A/H/M source-field access, HeapDestroy or context callback. The slot remains process-retained; passive M/C backing and historical reports survive inspection-lease expiry. Host-return LastError/TLS continuity and a resumable native failure occurrence remain explicitly unqualified.

## Replay and focused regression

The proxy started latch precedes all source-field validation. Old constructor/sentinel methods test stage before backpointer access; empty-clear/sentinel-free methods test their earlier started latches first; context-prefix tests cleanup-started before adopting or replaying its state. Therefore success and failed-free retries cannot validate or dereference retired H/A. Passive pointer-valued history is copied only as metadata and is never used as a new source capability.

The standalone and embedded tests require actual nonzero HeapFree results, compare heap/A/flags-zero and pre-retirement identity tokens, assert the six-event success order, one H free/one A free, unchanged allocation/context history and no replay through old entries. Later cases reacquire the process-retained frame after local strong leases expire. Static assertions retain no-proxy-null, no errno/GetLastError, no native C destructor and no public-Close claims. The declared zero-result test has model origin, zero actual addresses and no retirement/return; it is not an executed kernel-failure test. This limitation is accurately stated in the receipt and is nonblocking for this bounded success path.

The new APIs occur only in private runtime source/header and the two focused tests; no Project/Session/default hook was added. No blocker or required source correction identified. Root remains responsible for compiling and running the focused targets `m2m_model_feature_crt_runtime` and `m2m_model_feature_context_prefix`.

## Frozen hash binding

All four actual source/test hashes match `analysis/resume135-feature-proxy-free.md` (receipt SHA-256 `9a54c503e71699011adcc52fab93550f7b54f4eda861ca06a021fb2214a76c6a`):

- Runtime header: `5f09cddc39cff686024d1e1ceaa434eff564f73391b4b3a7654212659726e33e`.
- Runtime implementation: `3a3e4ccc7254ba38af6382237c00bd882c90a168c37b31e031a24e269291f85a`.
- Runtime test: `59f286cdf735a4c4c1dd3b44567f169856b407777018be7095672c3cb167c9fc`.
- Context test: `7bb1e4ac45eeb0ad8475c4d76fd92a24be7468192d585676c4fe510787151e48`.

Only this report was written. No source/test edits, build/test execution, GUI/original process execution or mutable REA actions were performed.
