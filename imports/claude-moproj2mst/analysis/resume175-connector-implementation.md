# CP175 connector implementation

Implemented QtCore RVA `[0x1311de,0x1311e6)` only: one additional symbolic DWORD QList reference increment on the allocation-store-owned root, a separate once-only second-reference marker, and `BeforePrivateZero78`. The private `+0x78` store remains outside the sparse state. No `view_manager.hpp` change was required.

Before editing, independently parsed the shipped QtCore PE section table and export directory using read-only Python. `.text` RVA 0x1000/raw 0x400 maps the instruction to raw 0x1305de. Eight bytes `f083056af81b0001` hash to `42761f1f0be20d86fa8ecaaa4a6cfad56f59f6c3f58031e1535c02381ab7fcb9`; signed RIP displacement plus next RVA 0x1311e6 resolves to 0x2f0a50. The export at this RVA is `?shared_null@QListData@@2UData@1@A`, ordinal 4511. Prior bytes `48894b70` bind private+0x70; next bytes `48897b78` are the excluded zero store. DLL SHA-256 remains `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

Admission reuses the retained Report/private/root owner qualification, task/private sizes and address points, first QList and QString markers, owner/pointee identity, incomplete base/backlinks, and four qualified zero fields. It independently qualifies the existing QVector binding/marker and exact second QList owner/pointee identity, requiring an uncommitted second marker before the noexcept root increment. There is no fixed root-delta or absolute-count prerequisite. Reentry at 0x1311e6 reaches only the stop diagnostic.

Added CP175 comprehensive corruption matrix (including missing/expired/foreign/aliasing/wrong-control-block second QList binding), owner-pair refusal cases, and an actual ConversionSession positive fixture for nonzero history, unknown projection, supplied observation and 0xffffffff-to-zero wrap. The matrix snapshots deltas, available observations and qualification, bindings/control-block identity, markers, private fields/identity, sizes/address points, completion/backlinks and factory/store counts before refusals.

Static migration audit: final-stop assertions now use 0x1311e6 / BeforePrivateZero78; one fresh frame contributes QList delta 2 while QString/QVector remain 1, two frames contribute QList delta 4. Deliberately rewound successful older fixtures account for an additional CP175 event (CP168 delta 3; CP173 and CP174 history delta +1). Mixed declarations group only matching uint32_t delta or optional<uint32_t> observation types; QString has no absolute observation fields and no such fields were added. Existing negative-seam expectations retain pre-call deltas.

Qualification: this is a source implementation receipt, not a frozen candidate or build/test result. Native evidence qualifies the selected normal non-null allocation path only; it does not establish live absolute counts, native timing/atomic scheduling, exception paths, release, full QObject completion, or original-comparison acceptance. No build, test execution, freeze, GUI session, live mutable REA operation, deployment, or shared recovery-state edit occurred. The REA skill was read; verification used retained artifacts without acquiring REA target state.

## Source SHA-256

- `source/apps/moproj2mst/src/doe/task_factory.cpp`: `e6c073e8d01733d1a62fff9a0eefaeb630416ec2b1ff6cecaf2706ecbf17953c`
- `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`: `6ac6da3fc895808cd82003352f15fc5a9335caa9e329ed9a73aa7e38e38581ed`
- `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`: `c1dff37a4afc583e79a901aee8e5b678b8ab69756d11c0634c66a56c8f43ee5c`

Exclusive edit ownership of these three source files and this report is released to the parent for source review, build, focused/full regression and comparison gates. No other files were intentionally changed by this worker.
