# CP136: CRT failure and partial-context frontier

The smallest source-ready producer is upstream of errno/FLS: actual EncodePointer(nullptr), followed by the first encoded new-handler role store, stopping before the next initializer. It must run after the actual canonical heap producer in a fresh startup profile, before any child-map frame. Existing paused failures still cannot resume with invented LastError or host errno. Full ContextEnvelope destruction remains unavailable.

Read-only lane: only this report was written; no source/build/test/GUI/original execution or mutable REA. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Addresses use the common14 prefix. Evidence is retained native exports plus bounded DUMPBIN14.44.35228.0, section-mapped PE bytes/imports and unwind metadata.

## Closed startup slice

CRT1470c28 calls heap producer1484800 at1470ca3, then1481708 at1470cce after success.1481708 first calls146e9f4 at148170e.146e9fa calls14813e0, which passes null to imported Kernel32 EncodePointer (IAT141e03420) at14813e2. The actual result is preserved in RBX and passed at146ea05 to1481ce0. That leaf performs exactly one qword store to1447006e0 at1481ce0 and returns1481ce7.1481ce8's later handler lookup reads that same role, decodes and conditionally invokes it, supporting its encoded new-handler identity.

A bounded existing-runtime method may observe the real candidate EncodePointer result and publish it to that canonical runtime's corresponding candidate role, then guard **before146ea0d ->1481ca8**, the next initializer. This does not write the host CRT new-handler, read an original address, invoke/decode a handler, or establish complete CRT/FLS startup. The encoded token need not be nonzero; never infer encoded null from an on-disk/default zero. Existing old heap receipts remain historical and do not auto-drive this step.

Require no retained child-map frame before this source startup slice; it cannot reset a handler role retrospectively after an isolated map lifetime. Use a separate fresh test executable for positive source order. Existing map tests can verify late-profile refusal. Same-process DecodePointer verification may occur in the test only, after the production prefix; no production extra decode/callback belongs to the slice.

## FLS/PTD producer and remaining failure witness

1481708 next performs a chain of encoded-role initialization, then147f9cc lock setup before the first FLS call.1481723 calls **FlsAlloc(callback1481590)**, storing the actual index into143b5e990 at1481729. IndexFFFFFFFF enters failure cleanup. Otherwise it requests one0x2c8-byte thread-data block through147e440 at148173e, calls FlsSetValue at1481754, initializes through1481434 and writes GetCurrentThreadId/default state before returning1. Failure calls148140c, which conditionally FlsFree's the owned index, sets the slotFFFFFFFF and performs additional lock cleanup.14816c8 separately detaches current PTD using FlsGetValue/FlsSetValue and1481590.

The existing canonical heap runtime owns none of that index, callback, lock table, PTD allocator/initializer or per-thread lifetime. Do not borrow the host UCRT's index/callback or call FlsAlloc with null instead of source callback1481590. Those are explicit missing owners, not a missing error integer alone.

For a failed free,1474134 calls14814e8; it returns PTD+10 or fallback143b5e2c8.14814e8 saves GetLastError at14814f2, reads module FLS index143b5e990, gets/possibly creates thread data, then restores saved LastError through SetLastError at1481558. Only afterward does outer1471ba4 call GetLastError for mapping/storage. Old parked failures lack the original same-thread error snapshot. A newly admitted real zero occurrence could execute the provider's first source GetLastError and stop before the unbound FLS-index read, but that must happen inline in that occurrence; it cannot relabel a later host read as the old error. Full provider completion remains blocked.

The mapper14740ec is independently closed as a pure **retained-initial-table** model:45 ordered pairs at143b5e160..143b5e2c8, first match wins. Duplicate key6 maps first to9, with later6→22 ignored. If unmatched, unsigned32 subtraction selects19..36→13,188..202→8,else22. Its table is writable.data, so this does not establish a current original-module table observation. A pure explicit-input model can test that quirk/range arithmetic without LastError/errno claims; it cannot complete a failed free or replace the PTD owner.

## Context lifetime boundary

CP132's pre-string receiver has a real embedded M but unknown strings/C+1450; optional-null is written only at12f116e. Full destructor12f0d20 first gates through12e6910, then reads C+1450 before map cleanup. It is not admissible on this partial receiver or after CP135 already retired that M/A/H. Passive C/M host backing may outlive the logical source map without becoming a constructed native context.

New unwind qualification:12f10d0's record at14347f420 has flags0 and four unwind slots—no local language-handler action in that record.12f1060 and12f0d20 instead have flags2 and handler14146e20c with distinct handler data. This does not prove constructors can never throw; it rules out inventing a context-constructor cleanup action from a staged guard. Current ContextEnvelope/MapHeader are trivially destructible host types, with no automatic native destructor/provider/free hook. A later full context task requires its remaining constructor effects and an actual destructor/EH occurrence; accepted Close still requires its original main/Project/manager chain.

## Bounded receipts

| End-exclusive native range | SHA-256 |
| --- | --- |
|1414813e0..1414813e9|`ddca6c63787621d57c9502905dce7225addfc84bb0508f6acdd3b30918b6e258`|
|14146e9f4..14146ea0d|`2de82ca3b6011537f7db3ef428c2a5028f453f1bbf9f0aef5317d3ef1afeb854`|
|141481ce0..141481ce8|`077a491a73df9b5ebf668dd0f818bbfeb3673d40f153a15157171d52ee24e54a`|
|141481708..141481789|`2e60e5d74214a1cea2203d3f419be2bcc1a2434e8c0e69a15750a091631dda49`|
|14148140c..141481431|`4140787b4d9aad759a7972a53a265fad031beb40e6013d47675c9784649e1f80`|
|1414816c8..141481706|`c18587a65cf406e61a547c44fe7da9330615f54c801ba12316484f55556975fd`|
|1414740ec..141474133|`f44e86936fa3b398df977c4dbe968eff2c5fec42fd3c07a7dc4adc02d61bce17`|
|143b5e160..143b5e2c8 initial mapper table|`dc5aaf854d47b0fa4deddacb6ff02e9a7d99cb1ac06c16e03e587d4599067fbe`|
|14347f420..14347f42c context-ctor unwind|`39455c244cd3d4de53471a0adec96067c74300ddc0ba6db9733fe8f8e3a9ffc4`|

CP135 implementation receipt `9a54c503e71699011adcc52fab93550f7b54f4eda861ca06a021fb2214a76c6a`; current runtime header/cpp read hashes `5f09cddc39cff686024d1e1ceaa434eff564f73391b4b3a7654212659726e33e` / `3a3e4ccc7254ba38af6382237c00bd882c90a168c37b31e031a24e269291f85a`. No reconstructed executable/deployment was inspected or changed.
