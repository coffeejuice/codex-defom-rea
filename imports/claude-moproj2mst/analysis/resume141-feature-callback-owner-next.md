# CP141: nonnull callback and FLS ownership frontier

No next actual-owner implementation cut is ready after CP139. The next initializer is a closed encoding/store sequence, but its argument is the address of a real, callable termination/PTD routine that the candidate does not yet implement. An arbitrary candidate function, null encoding, original VA or guard-only callback would supply a different capability. Keep the current guard before14146ea3d ->14147e75c. FLS production is farther downstream and additionally requires locks and a distinct registered destructor callback.

This lane wrote only this report. No source/build/test/GUI/original execution, REA session or database access. I applied the reverse-engineer-anything evidence workflow using bounded DUMPBIN14.44.35228.0 and section-aware immutable PE/import reads. Original is retained `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, v11.0.2, verified SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Full VAs below retain the common141 prefix. No reconstructed executable/deployment was inspected or changed.

## Exact next occurrence and capability

At14146ea3a the startup caller reloads RCX from saved encoded-null RBX;14146ea3d calls14147e75c. That callee ignores this incoming argument. At14147e760 it loads **callback14147e714**, calls EncodePointer wrapper1414813d8 at14147e767, stores the result into role1446ffbe8 at14147e76c and returns14147e777. This differs from all eight CP139 stores; it requires another actual encoding with a different pointer argument.

The callback starts14147e714 ->14148156c at14147e718. The getter14148156c first calls PTD provider1414814e8 at141481572, and if null invokes14146e674 with argument16 at141481582. A null PTD is therefore not a callback no-op branch. After getter return,14147e71d loads raw optional callback from PTD+D0, conditionally calls it at14147e729, then **always** calls14146d94c at14147e72d. A null PTD+D0 skips only the indirect call, not this final termination path.

The bounded14146d94c prefix itself reads mutable process role143b5dc6c: bit1 conditionally calls14147e778 with10, then calls141474a0c and conditionally141474ce4 with22. Its later path is outside this bounded closure. Treating it as an existing CLI terminal-harness TerminateProcess call is unsupported; the isolated worker/supervisor calls in current source have different ownership and triggers. Full callback/termination behavior, mutable signal/error/default roles and any return/nonreturn outcomes require a separate qualification task.

Use supports the capability interpretation:14147e738 reads1446ffbe8, decodes through1414813ec at14147e743, conditionally calls that result, then calls14147e714 again at14147e751. Even a callback that merely records a staged guard and returns would change this consumer's behavior. Encoding an original executable address in the candidate process is not relocation or reconstruction.

After this initializer,14146ea42 loads a *second*, distinct nonnull callback14146e9c8;14146ea49 encodes it and14146ea4e stores role143b5e040 before returning14146ea5a.14146e9c8 sets R8=0, RDX=1 and tail-jumps14146e830 with incoming RCX retained. These callbacks cannot share CP137's null token or be skipped to reach FLS.

## FLS owner and predecessor order

141481708 calls the whole encoded-initializer chain14146e9f4 at14148170e; only after it returns does141481713 call lock setup14147f9cc. The lock setup visits16-byte records143b5e750..143b5e990, chooses entries with flag1, publishes corresponding40-byte critical-section backing beginning1446fff20, and initializes each through141481cb0 with spin count4000. It is not the private host SRW lock that currently protects FeatureCrtRuntime metadata.

If lock setup succeeds,14148171c loads destructor callback141481590 and141481723 calls imported FlsAlloc;141481729 stores the returned DWORD index into143b5e990. FFFFFFFF follows the failure cleanup14148177c ->14148140c. Otherwise it allocates one0x2c8-byte PTD via14147e440 at14148173e, binds through FlsSetValue at141481754, initializes via141481434 at141481763, then writes thread ID and PTD+8=-1. Failed allocation/binding enters the same cleanup. The registered141481590 callback's nonnull cleanup/locks/locale/refcounts are distinct from14147e714; its null fast return cannot stand in for the full registered function.

Current canonical FeatureCrtRuntime owns the actual candidate heap, CP137 encoding and nine candidate encoded-null roles, but has no FLS index, registered callback, native lock table, PTD allocator/initializer, thread binding or locale/refcount owner. The PTD initializer itself uses locks13 and12 and shared locale state, so allocating a zeroed block and assuming PTD+D0 null would not close the producer. Original index143b5e990 and the two callback slots are writable data; retained bytes are not actual candidate/original live ownership.

Import verification:141e03420 EncodePointer;141e03450 FlsAlloc;141e03430 FlsGetValue;141e03438 FlsSetValue;141e03448 SetLastError;141e035e8 GetLastError;141e033e8 GetCurrentThreadId, all KERNEL32.dll. PTD provider1414814e8 saves GetLastError at1414814f2 before index lookup, possibly allocates/binds/initializes PTD, then restores at141481558. Nothing here supplies the same-thread LastError snapshot for previously paused free failures. The retained mapper remains a pure explicit-input model, not an errno provider.

## Next prerequisite and test boundary

Recommended next work is evidence closure of14147e714's *complete candidate callback contract*: provider14148156c/14e8/1434, PTD+D0 writer/current-owner inventory and14146d94c termination/error/signal path. Distinguish that implementation dependency from a missing original runtime integer: this investigation does not prove a live-original capture is inherently necessary, but current static closure/source owners are insufficient. No helper-only address receipt or callback stub is proposed as lifecycle progress.

After that contract closes and root authorizes source, reserve the private feature runtime header/cpp plus a new dedicated callback-prefix test executable. The initializer can then require completed CP139 on the same actual canonical fresh process/module owner, capture the actual address of the fully implemented candidate function with matching ABI/lifetime, perform its actual EncodePointer once, publish/read back a separate canonical role and stop before the second callback initializer14146ea49. Tests must verify the distinct real function argument, token decode identity (test only), one-shot/fresh/foreign-lease gates, preserved null roles and exact next boundary. Invoking a terminal callback needs owned-process validation of its reconstructed terminal semantics; never invoke an original VA or a fabricated guard callback as proof.

FLS should remain a separate later task after both callback initializers and source lock setup: verify actual index/raw result/store order, same-thread PTD binding, failure cleanup and thread/process teardown using the real registered callback. No public Close, full ContextEnvelope destruction or Project cleanup is admitted by any of these private prerequisites.

## End-exclusive immutable byte receipts

| Range | SHA-256 |
| --- | --- |
|14147e714..14147e737 callback|`77127cc95ade1b4dc1329ad42580e2b3519fdb69a95ec463202e10fae6cc1f31`|
|14147e738..14147e75c decode/invoke consumer|`4c062d8b4587bc622f50857f12ab8250a7ca8454dbe36ee7dff3db0424744e8e`|
|14147e75c..14147e778 initializer|`fbfcfe635c3852a56b40939652b94dc92088071efdffb54b0ad3bedb6c089a85`|
|14146ea3a..14146ea5b caller tail|`226ab8aeb54222e1531ed830ccb092a41102cf379ad3b3e431b177d84ba3dfde`|
|14146e9c8..14146e9d4 second callback prefix|`1430f8e7f0aa71021cd6be3922c883c000363fd7a68596621514cab20c3c51b0`|
|14148156c..141481590 PTD getter|`ec9870638dd871c6d6f77ea235c4820a8ba0ce71b51e6daf577d2626d00c2e02`|
|1414814e8..14148156c PTD provider|`191922765c03256b33eb9d0b8c15ddbbd206a54449906a4f86409a1a11ffebfa`|
|141481708..141481789 startup producer|`2e60e5d74214a1cea2203d3f419be2bcc1a2434e8c0e69a15750a091631dda49`|
|14147f9cc..14147fa47 lock success prefix only|`b71d4097915ddb780d235359dd0145e8bdf1d462fb304d7f1733aaddc749f463`|
|14146d94c..14146d97a termination prefix only|`4a9a70059df62fa18542675a4bef6a3b6aa16b0aec3650755f627b2ba9260f64`|

Prior retained closure: `analysis/resume136-feature-error-owner-next.md` SHA `da58f8912165bbe0e105e84053447bdbac0f76227c0a249e5780f99730eb0e6f`; corrected next-role classification `analysis/resume139-feature-null-role-tail-next.md` SHA `7fb5aa436b49ac57e3da3f76b3022484d4017d5a5161992c8f13c78405dfcf03`. This is a static dependency report, not source implementation or a runtime witness.
