# CP192 R300 htSubTypeDefaults implementation

The SimControl acquisition and entire consuming tail now run inside one immediately invoked, nonstored void callback. Owner admission stays outside. The callback captures only p/pr references and owner value, receives the existing single opSimCtl lookup result, and does not return or store an alias. propsSetHeatingTime remains before TMAX, ENVTMP ndim and ENVTMP value stores; Transfer/Rest/Dwell priority, constants and UI-only label statements are unchanged.

Both live files were compared byte-for-byte to CP191 build/wave3-checkpoint191-verified/source.zip before edits. Archive entries omit the source/ prefix. Baseline hashes: loader_l2.cpp 227fb4ee444716aabca6f0d62757f015eb29ce2105704b30a682e9e59b45da5e; test_loader_l2.cpp aed6f77b4414123a883e01c494428cc419e2ccc3a1df9d81cc4d2086ec88a950.

## Fixture correction approved by root

The preparation plan incorrectly described htSubTypeDefaults as externally linked. It is inside the anonymous namespace opened at loader_l2.cpp:92. Root explicitly directed retaining that architecture and using the existing same-owner postload slot instead of exporting a test seam or substituting a separate property owner. No production header/API or namespace boundary changes remain.

heat_transfer_subtype_defaults_step_scope opens corpus08, gets operation 1, loads its real model once, then calls the installed htPostLoad slot with the same owner, live model and step. Independent SI and unit-2 rows assert time 15, ambient 20/68, ndim zero and literal "15 sec". Each row reseeds distinct field/property sentinels and verifies cached model/step/control identity, owner/CommonBlock identity, unit and unrelated NSTEP. The retained units_table.inc kind 0 at kUnitNameSI/kUnitNameEN spells both time units "sec"; props_sync.cpp formats the supplied time without conversion. Expected text is literal and does not call the production formatter or unitName.

This slot sets Transfer when oprFlag is zero, which is also the condition admitting defaults. It cannot honestly test arbitrary Rest/Dwell/furnace or overlap strings through the same-owner route. A detached step encounters earlier object admission before defaults; null-owner standalone props have no existing direct invocation seam. Therefore the proposed direct matrix, null-owner and detached-step RAII cases are explicitly deferred coverage gaps, not implemented claims. No temporary step detachment occurs, and no restoration guard is necessary in the released fixture. Root approved this narrower coverage rather than changing internal linkage solely for tests.

## Verification and evidence limits

Static preflight passed: mechanically reversing only the callback wrapper exactly reproduces CP191 loader_l2.cpp; Testkit main/registration, supported CHECK/CHECK_EQ/REQUIRE macros and actual m2m/mst/loader_l2.hpp include resolve; no C assert or REQUIRE_EQ was added. git diff --check passed for the owner pair. No build or runtime tests were run by this worker. Root owns focused m2m.mdt.loader_l2 and source-stable full validation/freeze.

The L2 spec introduction and section 2 retain annotated native static-analysis qualifications (DEF_GUI_MO_64.exe, historical d0a..d5a.c and ht_post.asm, gproj_L2). This change preserves their documented subtype constants/order and setter/store sequence. It adds no fresh native observation or equivalence proof. Current reconstruction simCtl geometry admission and ordinary null-step early return remain unchanged; source review establishes that claim, not native evidence. The lexical cut does not pin the model, guard arbitrary reentry/destructive owner mutation, invalidate existing aliases, or close the general opSimCtl pointer seam.

Released source hashes: loader_l2.cpp 8832bee8a682e4bf20a5d37378c81780a5ac9cf4341b6b147b5d49df18c7528b; test_loader_l2.cpp 6a4745a4737e124448b66881d9036d9eba0c8381df3a31848831ca39e596831e.

Only the assigned production/test pair and this report were changed. No build/full suite, executable/deployment, GUI/app-control, live REA database, shared plan or shared state was changed. Files are released for root integration.
