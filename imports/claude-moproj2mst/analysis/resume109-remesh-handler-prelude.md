# CP109: original default handler and B133 overlay occurrence

The imported original statically establishes an encoded-null invalid-parameter handler before main and, under that still-current fresh-default profile, a null remesh FILE reaches `TerminateProcess(GetCurrentProcess(), 0xC0000417)`. It does not establish that original B133 ever had enabled remesh or that the constructed overlay has executed this path in the original. CP108's candidate fixed endpoint is a different occurrence and does not recover the native handler prelude.

## Exact fresh startup profile

Read-only section mapping recomputed original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, image base140000000 and PE entrypoint141470dc0. Retained aligned disassembly binds:

1. Entry141470dc0 calls cookie initialization, then jumps141470c28. Startup calls141481708 at141470cce before application main140001410 at141470d7e.
2. 141481708 calls14146e9f4 at14148170e. The initializer calls1414813e0, which zeros ECX then jumps through IAT141e03420 to KERNEL32 EncodePointer. The encoded null is retained in RBX and supplied at14146ea15 to141470438.
3. Leaf141470438 stores RCX to this EXE's global1446ffb40 and returns. Dispatcher141470568 later loads that exact slot at14147057f, calls DecodePointer through1414813ec/IAT141e03428, and tests the decoded result. Its nonnull branch calls that handler and can return; the null branch calls1414834f0, then141470440.

This is a module CRT handler slot, not the host UCRT handler, a Qt setting or the Windows top-level exception-filter slot. The current dispatcher has a direct global decode/test; importing newer CRT thread-local-handler semantics would change the evidence. Existing bounded setter-reference scans find the aligned startup store and dispatcher read and one direct initializer setter call. They do not prove absence of computed aliases, later foreign modification or every indirect call. Therefore FreshDefault means startup assignment plus **no later replacement of that module slot**, not a measured current value inferred from a candidate enum.

## Null FILE and native prelude

Enabled Save098265 calls writer1400abcc0 with the singleton and relative LOCAL_REMESH.DAT. The unchecked fopen result is passed to first fprintf at1400abcf0. The fprintf entry14146da0c tests FILE; null reaches14146da3b errno getter, writes16h = **EINVAL22 decimal** at14146da40, clears all invalid-parameter arguments, and calls141470568 at14146da55. Numeric ERANGE34/22h is unrelated. First format attempt has reached its invalid-parameter gate; it is inaccurate to say zero attempts occurred.

For decoded null, debug leaf1414834f0 clears dword1450540e8 and returns; only bytes through its RET are interpreted. Native default141470440 then:

- zero-initializes the exception record and captures context with RtlCaptureContext;
- obtains the captured RIP, calls RtlLookupFunctionEntry through thunk14157eaae/IAT141e03320, and if an entry exists calls RtlVirtualUnwind through14157eac0/IAT141e03348; its alternate path adjusts the context from the caller return slot;
- installs exception codeC0000417, noncontinuable flag1 and the caller return address in its local record;
- records IsDebuggerPresent, clears the process top-level filter with SetUnhandledExceptionFilter(NULL), and calls UnhandledExceptionFilter with its exception/context pair;
- if the filter result is0 and no debugger was reported, calls the same debug leaf; both ordinary return branches join141470548;
- calls GetCurrentProcess, loads EDX=C0000417 and calls TerminateProcess at141470556.

The PE import table independently resolves all those API names. This is more than the candidate's fixed endpoint. Filter/debugger/context behavior is not established by matching only a final status. The native function even has an epilogue after TerminateProcess: the static call is certain on the qualified normally returning prelude, while process death/nonreturn follows ordinary successful Win32 termination. Unexpected external API failure, intervention, exception or nonreturning filter is not silently proven away. Unsigned expected status is3221226519 (signed-1073740777).

After successful termination there is no subsequent remesh print, remesh fclose, Save return/status mapping or normal Close. Earlier project/property/MST attempts and completed outputs remain. No exact arbitrary-failure remesh byte residue follows solely from null FILE; unchanged bytes require this fixture's denial before truncation.

## What B133 does and does not establish

Independently recomputed B133-before inventory:64 regular files; canonical sorted path/size/SHA manifest `0ff2038095f238744a0d61a1cbf2b3da01f061ba4080f3a0dff08aa248e0c5e6`. XML is25644 bytes/SHA8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79 with exactly one Sub_Project opening tag; DB is9216000 bytes/SHA707326e90e4f8fb0fee2aa1fa9f174e8d6e40b213fe2a7c0a828be00829671df. Neither remesh file exists in that immutable before tree. It supplies no historical enabled-remesh occurrence.

The CP107/108 overlay deliberately adds primary `DONE\n` (SHA8221ac66be71558c921fb44cfb66f7997699aea754d917763882d6d9eddc836e) and fallback `FILTER_DISTANCE 7\nDONE\n` (SHAbb28f25ddbd98300249e3a867f78ebc5714d16640996a6059ef4441e6bd81d50). Retained original Open source sets the project CWD, obtains fixed singleton143d91238, invokes reader0ab170 and assigns its returned AL through0ab150 to singleton+29, before the later event pump. The unmarked primary scan/rewind/close followed by a readable fallback predicts loaded=true. A pre-acquired read handle sharing READ but not WRITE is compatible with the qualified native rt access and denies the later wt GENERIC_WRITE/CREATE_ALWAYS open before truncation **if that Save resolves to the same locked primary**.

This is a conditional source prediction for a newly constructed input, not a replay of historical B133 remesh. Original enabled byte at Save, later mutations during event/editor callbacks, actual current directory at098265 and the continued default module handler must belong to the same fresh one-project run. Save uses a fixed relative path at its source point; Open setting the project CWD does not alone prove no intervening callback changes it. Candidate successful source order and its actual worker receipt cannot substitute for these original observations. CP108 also explicitly labels its endpoint `candidate-fixed-endpoint; native-default-handler-prelude-unimplemented`.

## Exact next witness

No additional numeric variant or module-symbol registration closes this gap. A separately permitted original witness would need one fresh owned original process and one disposable B133 overlay, recording: immutable/overlay hashes; parent denial handle and lifetime; actual Open singleton/result assignment; Save-time singleton enabled byte and resolved CWD/path; actual fopen result and first FILE argument; the same module's handler slot decoded null at dispatch; reach of native141470440 and native141470556; and resulting child OS status. The trace must bind identities/module hash/load bases and account for any debugger/probe perturbation of IsDebuggerPresent/filter behavior. Process status alone, a candidate event, or an arbitrary process with the same exit code is insufficient. Later print/Save/Close absence must be qualified by the terminal occurrence, not inferred from a missing marker alone. This report authorizes no such execution.

## Bounded byte receipts

Recomputed half-open PE byte ranges:

| Range | SHA-256 |
| --- | --- |
|141470dc0..141470dd2|42ae9783bb15e68c9ef167a8f602401116289a05b5138fb3183e7cbff6bb9a79|
|141470cce..141470d83|bf7d0a7116f6b5b526c2be8fb8d152820cde9123017df64175689495ed9f039b|
|141481708..141481713|6d5a6eb9ae43e84e6ae924df050bcce77aa2141c08c28a33ffcf158caa0137ed|
|14146e9f4..14146ea1a|49122bd8860375a844082895d1954bcb323bdfe1785841c604f646969869b11a|
|1414813e0..1414813e9|ddca6c63787621d57c9502905dce7225addfc84bb0508f6acdd3b30918b6e258|
|141470438..141470440|b7b64b177a40336007a903652e97d3b4b83b110e65366b920551a429764817f5|
|1400abcc0..1400abcf5|5912b7bffd5aa69439a8336d5ae40085a25ceb42d81dac9e5a59e7eedde21ca8|
|14146da31..14146da5a|1c4bdc5062fbc422738f4b0d4c5381d3794667dc063b553a6eefc5744f58b92b|
|141470568..1414705ed|455022bff2e4016f1b9f4709d08488112c95e19ea191aecf60020fe41b82836c|
|1414834f0..1414834f8|9b3bff723d01eb2308873bb025da7be6af26ed8173d82f9b28affe6e7d75c7cb|
|141470440..141470565|056a315910535d2876efecf1b5ff2533dcf7ffb15a514edaa6785bca9eb18041|
|14157eaae..14157eab4|cac64d77b3d7d3fb38f361f30c6b6db05ce03949c702f6b75b2849180b73d0be|
|14157eac0..14157eac6|e8992690ae6ca1c791511c1dcddb8602bd0c9e0e838045e17f15e332ec31abd0|

Retained startup contract SHAd53cfdfedf9d4fe8ec2fd8941739ed48f12e6173db6d1c1b805a9cffbc1f343b; Open-lifetime contract SHAba7883b3d2dea182c7052fee38196af1dcc11c578cd407664839c0b45987d0ff. CP107 fixture report SHA5dd3178f8e4762a45b431f02de793c402f29d2bd9996211f071b92775c9a8139; CP108 independent review SHAcee1fbd5ff91fb29c78c20c5f31341ab358dd0e8365dfed7950bdc17182a787d.

Only this report was written. No source/test changes, build/test, GUI, original execution or mutable REA operation occurred. Native termination remains an established qualified prediction with a runtime-unverified B133 occurrence; public legacy and selected-palette guards remain separate.
