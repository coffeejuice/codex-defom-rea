# Checkpoint 76: Report document and viewer dependency boundary

## Finding

There is no additional stateful Report writer prefix safe to implement while keeping the public Report path inactive. The only source-closed prefix is the pure sibling-path expression `reportDsPath(Project.path)`, already implemented and covered by path-focused tests. It computes a value; it does not set a Report document field or perform a write.

## Document owner and path writer

Native Report Save vt+0x68 first calls vt+0x80 (`0x1412aeb30`), then direct `0x1412af0c0` on the same receiver. The vt+0x80 body derives the sibling `.DS` filename and passes that path, the Report document, and mode `1` to `0x140645030`. That gives the writer's ordering and argument role, but its receiver is the lazy `SubProject+0x30` document, not an independent path value.

The retained owner getter `0x1400b5ad0` reads the slot, allocates a `0x70` object if empty, constructs it through `0x140643100(raw, mainWindow, mode, mode!=2, 0)`, and only then publishes the result back to the slot. The object is parented to the actual main window. Candidate source has no equivalent owner. Static QtCore and constructor evidence establish an ordered QObject/private/thread/parent/counter/member/connection sequence but do not provide the actual parent/thread branch, shared ownership semantics, constructor unwind, or normal-Close release order. Therefore setting a path into a fabricated document object would skip the earliest native effects and overstate parity.

## Viewer dependencies

CP63's bounded capture contains the complete machine-code body of `0x1412af0c0` through RET, correcting earlier CP63 text that described the body as absent. Its callsite sequence establishes that it obtains the current operation/model, resolves a Display from the model, passes `(display, model)` to `0x140d0d6f0`, asks the returned object for a count, and loops through returned entries. It also calls opaque helper/IAT targets, including `0x140819600`, `0x1408f37d0`, `0x1408f4760`, and `0x1408f3820`, with conditional cleanup paths.

This static body narrows the dependency: the returned model object from `0x140d0d6f0(display, model)` is the direct iteration source. It does not establish that object’s owner/creation semantics or the meaning, path/mode, first external mutation, and failure effects of the per-entry helper calls. The nearby Report Open coupon-geometry viewer route is distinct and cannot supply those facts. Nor can candidate `displayRoiModel`, whose native target differs (`0x140d0d670`).

## Smallest safe action and next producer

No new state mutation is justified. Keep Report Open and Save refusals before the document accessor and before the writer callbacks. The existing pure helper is the smallest closed writer prefix; extending it requires a real document owner and a qualified setter contract.

The next useful static producer should resolve the helpers used by `0x1412af0c0`: trace `0x140d0d6f0` to its concrete return object/owner and inspect `0x140819600`, `0x1408f37d0`, `0x1408f4760`, and `0x1408f3820` for per-entry input, path/mode and first side effect. In parallel, finish the exact Report QObject owner contract: actual main-window parent/thread behavior, `SP+0x30` shared slot semantics, constructor failure/unwind, and Close-time release relative to main-window child teardown. Only after these facts map to candidate owners should the project path be assigned or a writer prefix be activated.

A future isolated value test may continue to verify `reportDsPath` behavior. It cannot be presented as a Report document path write or Report Save acceptance. No original-vs-candidate Report Save result is established by this static review.

## Evidence

- `analysis/resume63-report-viewer-owner.md` — complete-body scope and qualified call/owner limits; SHA-256 `1cac192e2ac8a599c21e7e484b3f6d9f7e2ff41fe0275a47e355c9e814f52f43`.
- `analysis/resume63-report-viewer-bounded-capture.txt` — DUMPBIN ranges clipped at function RETs and retained constructor/getter excerpts; SHA-256 `0a0b5b87c1f034e2f0207f23864222098aeb2577af6d5f95def9d49f536b5559`.
- `analysis/resume71-report-owner-edge.md` — no source-closed owner/publication edge; SHA-256 `51ab8bfb2917607fb386d19be7ead3e42a8b1e4b2eaaa0e295e8f5ff963111b8`.
- `analysis/resume65-report-ds-path-implementation.md` — pure path helper scope; SHA-256 `e07d3e1584a85e3caf7098f497e21ffb09d81753286893281501abdf9c29c847`.
- `source/apps/moproj2mst/src/ops/report_ds_path.cpp` — helper implementation; SHA-256 `b67d64a300393f4975ccc03e0ad2cacb5e09593ea7a28eb29d55ad81bd4b7f34`.
- `source/apps/moproj2mst/src/ops/report.cpp` — current refusal before document construction/read; SHA-256 `e378d472550bbe9f6a109baa330f6612277328ac425853cf5e17ba316252b43a`.

Read-only retained evidence/source audit only. No source, build, test, GUI, runtime capture, or mutable REA action was performed.
