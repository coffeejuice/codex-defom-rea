# CP185 connector implementation

Implemented the exact retained QtCore4_64.dll instruction cut `[0x13123a,0x13123e)`, bytes `48 83 c4 30`, `add rsp,0x30`. The connector advances from `BeforePrivateStackAdjustment` to `BeforePrivateRdiRestore` and changes only live symbolic RSP displacement from `S-0x38` to `S-8`.

The retained saved-RBX structure keeps its historical MOV-input stack relation. Separate live RSP, saved-RDI slot `[S-8]`, and return slot `[S]` fields represent the CP185 contract without numeric native addresses or captured stack contents. The saved owner/control block, ordinal, module/caller/initializer provenance, allocation/private identity and restored task RBX are validated before mutation and at terminal reentry. Stack/slot corruption is refused without normalization. RAX/private identity and RBX/task identity remain separate.

The existing frame has no architectural arithmetic-flags register model. The source documents ADD's arithmetic-flag effect without fabricating concrete flags or repurposing historical CP182 `flags20Eax` and `private20` expressions.

Default instruction stop advances to ordinal 7. Ordinals 0 through 6 retain previous boundaries, including CP183 before MOV RBX and CP184 before ADD. Lowering a budget at CP185 retains the validated terminal; explicit test rewinds restore symbolic RSP consistently with their earlier phase and retain or clear RBX according to that phase.

Focused test source adds full snapshot preservation around ADD, both reentry keys and repeated reentry, 40 malformed predecessor/terminal cases, saved-slot corruption, distinct RAX/RBX checks, no completion/backlinks, retained earlier diagnostic budgets, CP185-to-CP184 rewind and resume, and all earlier flag-budget rewinds. Existing default-boundary assertions now expect 0x13123e; explicit CP183/CP184 tests retain their boundaries. Snapshot helpers include all three new symbolic stack fields.

No build, focused execution, full regression, GUI, REA database operation, freeze or shared-state update was performed by this implementation worker. Integration validation remains pending. No RDI value restoration, RET, public caller continuation or backlink publication is implemented.


## Focused integration correction

Root integration receipt `build/logs/resume185-dual-focused-ctest-01-03.log` reported 1 of 97 factory cases failing: the older CP182 invalid-stop fixture still treated ordinal 7 as invalid, although CP185 deliberately admits 7. All three new CP185 cases passed in that receipt. Updated only that fixture's first invalid value to 8, preserving the maximum-size invalid value, exact error expectation, preallocation assertions and retained-prefix full snapshot check. No build or rerun was performed by this worker; root will rerun integration validation.
