# CP279 original color observer preflight design

## Decision

**HOLD.** This is a read-only design for an exact future original color-observer preflight. It neither creates a clone nor launches the original, starts GUI control, uses Frida, or mutates REA. It does not claim that the CP278 one-slot surrogate demonstrates overlap coverage. A DR0 slot moved from an entry to its return leaves a blind interval at the former entry; the CP278 synthetic leaf has no nested or concurrent re-entry and cannot close that gap.

## Bound original and source fixture

Target the immutable installation `DEF_GUI_MO_64.exe` v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; retained reviews report a null PE version resource. The file is `evidence/original-binaries/DEF_GUI_MO_64.exe`. Recompute the hash immediately before any future preflight and compare exact bytes, file identity and size with a fresh receipt. This is a versioned original installation, not a reconstructed DEF_PRE build.

The concrete fresh disposable project identity currently retained is CP214's `C:\PROBLEM\p214a\Gear_blank.moproj`. Its exact CLI argument is the lowercase UTF-16LE code-unit hex string:

```text
0043003a005c00500052004f0042004c0045004d005c00700032003100340061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a
```

Decode by parsing each consecutive four-hex-digit group as one 16-bit code unit, then joining those code units as characters. Confirm the result equals the one project path above and re-encode it byte-for-byte. Do not decode pairs as UTF-8 or use UTF-16LE byte order on the hex digits: the CLI protocol is four ASCII hex digits per UTF-16 code unit. Required argument vector is exactly `[EXE, '-m', 'open', '-f', HEX]`; cwd is exactly `C:\PROBLEM\p214a`. Resolve the path and check every component stays beneath `C:\PROBLEM`, reject reparse traversal, and require all full project paths to be shorter than 100 characters.

CP214 independently retained a source-before and fresh-clone-before manifest with 33 entries (27 files), digest `55e2a58cf8155995f942f89ac0ebb91454308b521ae76a228edec0ffdc4cedbd`; the project file SHA-256 is `e769faf514b335d56ffbb22f2c76490d029f2714ada9b26a75496fbd62ade7fd`. The source-after digest matched the source-before digest. The captured clone-after manifest had 34 entries and digest `37ae637460c5f280b20a498018ea9d52fb47de2407b0ff511d7f177dd7c19c8a`; its sole delta was a newly created empty `Lubricants` directory. These are historical CP214 identities, not proof that a new clone exists or is fresh now. A future preflight must reserve a new absent destination exclusively, recreate all 27 files from the qualified source, verify every relative path, byte count, SHA-256 and directory entry, and independently compare full manifests before launch. Require source-before == source-after and clone-before == source-before; retain the expected post-capture delta only as a separately bounded comparison. Do not reuse CP214's already-run clone as a fresh fixture.

## Immutable PE and callsite byte gate

Before observer preparation, bind the original image SHA above and the exact installed Qt DLL hashes and versions from a new receipt. Bind each observer address to the image base plus RVA, and validate the bytes at each hook address against a checked-in extraction made from this exact PE. Reject mismatch, relocation ambiguity, changed prologue, insufficient patch footprint, or unreviewed instruction boundary. Record expected bytes, actual bytes, length, module hash, RVA, loaded base, runtime address, displaced instructions and their decoded length. Recompute any relative branches from raw bytes rather than trusting a previous receipt.

The CP214/CP277 sequence provides useful static candidate sites, not a color contract: outer main `0x14001ca30`, postOpen `0x14002a0e0`, current accessor `0x140045780`, activation `0x1400a7250`, selected activation return `0x14002aaa1`, acceptance `0x14008a9b0`, creator `0x14008c630`, shared helper `0x14081afb0`, and the five creator helper return PCs ending at `0x14008c77a`. Verify each against the retained disassembly and exact PE bytes before reuse. These addresses describe the prior Open capture and its known debugger conflict; they are not assumed to be the color callsites. The color-specific callee/callsite addresses, instruction spans and call ordering must first be independently extracted from the immutable PE and qualified against retained CP275/CP276 color reports. CP276's `FUN_1411f26e0` static description still contains unqualified helpers; do not treat helper names or a local call window as a recovered behavioral contract.

## Preflight validator contract

A static validator may accept paths to (a) the frozen original executable, (b) a checked-in callsite-byte manifest, (c) a proposed fresh clone, (d) its source directory, and (e) the literal command/cwd proposal. It must be read-only and must never spawn a process. It should emit a compact JSON receipt with `readyForIndependentReview` false by default and explicit pass/fail/unknown per predicate:

1. exact executable SHA, size and file identity; expected version qualification and PE resource qualification;
2. exact loaded dependency hashes/version requirements, when supplied;
3. byte-for-byte source and clone manifests over all regular files and directories, exactly 27 files for this fixture, no reparse points, no unexpected entries, and stable source before/after hashes;
4. UTF-16 code-unit hex parse, canonical lowercase re-encoding, exact project path, exact argv, exact cwd, containment, no path over 99 characters;
5. each hook's exact PE bytes, RVA/base arithmetic, instruction boundaries, complete displaced-byte coverage, and callsite identity;
6. exact creation-time identity representation as a decimal string (never a JSON floating-point number), plus image, argv and cwd bound together to one fresh suspended child in any later execution receipt;
7. explicit witness-scope obligations, each marked proven, monitored, unresolved, or outside this narrowed claim. A positive entry observation needs exact child/thread/breakpoint provenance, entry frame and qualified caller/arguments. A paired return additionally needs the selected entry TID, entry RSP, dynamic return PC, matching caller frame, and explicit timeout/unwind/thread-exit outcomes. Record unmonitored comparator intervals and withhold all absence, uniqueness, overlap, other-thread, callback-absence, complete-effect, and complete-Open claims unless separately proved. Outside-claim never passes the full CP214 contract.

The validator must not convert `unknown` into `pass`, infer clone freshness from a historical manifest, or state that a matching endpoint proves successful Open. It should state that process creation, a single resume, GUI neutrality, observer lifecycle, ownership cleanup and independent policy review are separate gates outside this static validator.

## Independent proof obligations and stop criteria

CP277 found a concrete unresolved problem in the prior four-slot proposal: five simultaneous postOpen obligations and a helper/creator overlap blind interval. Its local call-order evidence proves neither global unreachability nor absence of recursion, imported modal callbacks, exception paths, or other-thread entries. No positive CP214 chronology, directory delta, clone match or CP278 synthetic result repairs that proof gap. A successor observer design must use an independently reviewed mechanism that preserves the selected caller, frame, thread, ordering, exact clone/path and return predicates across every interval, or explicitly narrow the witness contract and obtain separate review. Do not copy CP278's synthetic `RCX==VIEW` condition into the original observer.

Keep the original execution gate closed until all of the following have independent evidence: (a) fresh 27-file clone and command preflight, (b) exact PE and callsite-byte gate, (c) an implemented observer with proof for the expressly selected positive predicate and explicit unmonitored intervals, (d) harmless lifecycle/API and cleanup validation, and (e) independent neutrality and policy review. The CP277 five-address conflict and overlap blind interval continue to block the full CP214/Open contract; they do not automatically block a separately reviewed positive-only color-entry witness. A return, color effect, or completed Open requires its own additional observation and review. CP214 established only its own synchronous postOpen occurrence and the one empty `Lubricants` directory side effect; it did not establish complete Open, Save, accepted Close, or color observer behavior. The root GUI-control release gate remains independent and must be satisfied before acquiring GUI control.

## Provenance and work performed

Read CP214's `resume214-original-postopen-review.md`, `resume214-original-postopen-2026-10-26T13-17-52-922Z/summary.json`, `source-fixture-reference.json`, and `runtime-identities.json`; CP277's `resume277-debugger-cfg-proof.md`; CP276's `resume276-color-3d-helper.md` and `resume276-original-cli-route-audit.md`; CP275's `resume275-viewer-color-tail-recovery.md`; and CP278's `resume278-color-one-slot-validation.md` plus the fixture source. No original process, GUI, Frida, mutable REA, clone, source, build, or test was run or changed for this preflight design. CP214's exact creation FILETIME is `134374942738319467` as raw decimal text; its JS summary numeric field is rounded and must not be used for ownership. Current status remains **HOLD**.
