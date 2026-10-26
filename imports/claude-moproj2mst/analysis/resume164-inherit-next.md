# CP164 RINHERIT: interactive payload attachment and Save branch boundary

The immutable original establishes one more effect of creating `SfSessionManager+0x30`: the new `SfInteractivePost` participates in creating a `SessionRoot` DOM element, which is appended beneath the document's current `+0x58` element. A second, conditional route from document Save to the lazy `+0x30` accessor exists inside `0x14064a830`, before Save tests `+0x2a`. It does **not** establish that a newly constructed Report document takes that route: its constructor initializes `+0x68=1`, whereas this route requires `+0x68!=1`. Current `+0x30` at an accepted Close remains unbound. No source-ready same-instance document or cleanup cut follows.

This is a read-only converter report. No source, test, shared plan/state, GUI/app-control, original-process, live REA or deployment action occurred. The analysis uses section-mapped original PE bytes, bounded DUMPBIN 14.44.35228.0 disassembly and retained decompilation as a qualified cross-check; no new REA Evidence ID is claimed. CP160's manager/Project/SubProject Close chain was not retraced.

## What publication of `+0x30` actually attaches

CP161 already binds `0x140645320`: on current raw `D+0x30` null it constructs `SfInteractivePost` from current `D+0x48`, stores its result at `D+0x30` (`0x14064538c`), calls `0x1406453c0(D,current D+0x30)` at `0x14064539e`, then rereads the slot. The new bounded cut resolves the latter call rather than treating it as an opaque notification.

`0x1406453c0` copies the current `D+0x58` QDomElement through QtXml's copy constructor at `0x1406453e5`. It passes the supplied interactive-post pointer to `0x14065b470` at `0x1406453f6`, supplying an output QDomElement local. It then calls QtXml `QDomNode::appendChild` at `0x14064540b`, with the copied `D+0x58` node as receiver and that output as child, and destroys the temporary QDomNode/QDomElement values. The imported symbols identify the operations, but QtXml's internal ownership/refcount effects are outside this PE cut.

In complete `0x14065b470`, the helper calls `QDomDocument::createElement` on the interactive-post object's stored `+0x18` DOM pointer at `0x14065b4b5`, with the literal `SessionRoot` at `0x1424f2d50`. It copies the resulting element, passes it to the current inner object returned by `0x14065b3e0` via `0x140657150` at `0x14065b503`, and copies the element into the caller's output at `0x14065b512`. This ties the newly published payload to a DOM node in the same document's `+0x48` DOM and `+0x58` element, subject to successful allocation/construction and the actual Qt calls. `0x140657150`'s transitive effect is not claimed here.

This matters for the Close cut: `D+0x30` cannot be projected as an isolated raw pointer whose creation leaves the document XML untouched. The helper can mutate the XML graph before subsequent Report Save and before the destructor's interactive-post/DOM releases. The original's actual current `D+0x58`, `D+0x48`, inner pointer and append result still require an occurrence witness.

## Save reaches another conditional `+0x30` call

`0x140645030(D,path,flag)` calls `0x14064a830(D,flag)` at `0x14064509d`, then tests `D+0x2a` at `0x1406450aa`; CP161 covered only the later direct `0x1406450ef -> 0x140645320` call. In complete `0x14064a830`, `D+0x68==1` selects `0x14064ab80` at `0x14064a891`; any other value selects `0x140646550` at `0x14064a8a0`. In complete `0x140646550`, current main/singleton must be nonnull after `0x14064c030`; its `vt+0x778` result and later callbacks are additional boundaries. Paths reaching `0x140646824` call the lazy accessor with this same incoming D at `0x14064682c`, then test the returned pointer and its inner pointer before further work. The `vt+0x778` result can bypass earlier work but still lands at `0x140646824`; a null current main exits before the accessor.

The constructor's complete `0x1406433a0` stores `D+0x68=1` at `0x1406433d5`. The selected Report constructor returns only after calling it, and `0x1400b5ad0` publishes the returned D into SUB+0x30 afterward (CP160/161). Thus a fresh constructor state selects `0x14064ab80` at this Save split. The selected `0x14064ab80` prefix confirms the nonnull-main gate but does not close all of its downstream indirect effects. An intervening `+0x68` writer/rebind, the state at the actual Report Save call, and the branch's callback returns remain unobserved. This corrects CP161's narrower wording: `+0x2a=0` excludes the *later direct* lazy call, but by itself does not exclude the earlier conditional route through `0x14064a830`.

There is also a non-Report-qualified chain `0x14064e220 -> 0x14064ed40 -> 0x1406459d0 -> 0x140645320`; the first helper's current receiver/callback and its document identity have not been bound to Report's active SUB. It cannot substitute for an occurrence of this D. The bounded rel32/RIP scan is candidate discovery only; the named call sites above were checked at instruction boundaries. Indirect calls and callbacks limit any negative inference from the scan.

## Exact missing owner and occurrence witness

For an accepted Open/Save/Close on one selected Project P and active SubProject S, record D returned by the current `0x1400b5ad0(S)` and its accepted QObject parent. At Report's `0x1412b0d08`, bind the same current D to the produced/returned `+0x20` model. At the `0x140645030` Save call, capture current D identity, `+0x68`, `+0x2a`, `+0x30`, `+0x48` and `+0x58`; record the `0x14064a830` branch, singleton and callback returns, plus any invocation of `0x140645320` or alternate producer `0x140646370`. If lazy construction occurs, retain the resulting `SessionRoot` append and any later slot/DOM replacement. Finally bind the accepted Close chain's fresh S+0x30 reads to that current D or identify its earlier death; retain current `+0x20/+0x30/+0x40/+0x48` and QObject-parent deletion order. A constructor default or static call graph cannot choose those occurrence values.

The smallest further immutable query is the complete `0x14064ab80` branch and the actual Report `0x140645030` caller's intervening writers to D+0x68, followed by current-document/callback identity through that Save. If static closure cannot account for those mutable values, the same-instance original witness above is required after the supported GUI release gate is available. No public Report admission or private owner implementation is justified now.

## Immutable receipts and qualifications

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Section mapper `tools/doe_static_pe.py` SHA-256 `e949707799912f8079e58d60a2b02ca204412d381e9accba81cd52bd4ee03ca8`; import reader `tools/boolean_lifecycle_static.py` SHA-256 `f55e0b76d6c521c97507af84891563cb15cd884bd1ef93f913d14103de7a7a39`. The ranges below are half-open SHA-256 selections of original mapped bytes. Unwind bounds identify regions, not proof of all logical callees. The `0x14064ab80` entry is a selected prefix only.

| Range | SHA-256 |
| --- | --- |
| `1406453c0..140645439` post-publication DOM helper | `999a439b9396cc0254ba24f1b8821a2c64306750f4d455ccbee46ffdda38d1e8` |
| `14065b470..14065b539` SessionRoot producer | `d4f81ef895031037755eaac8f3e99313165da3c8314e17ef4b1f793736487658` |
| `14064a830..14064a9bc` Save branch helper | `b1018494f2be4004ec9d6e4b54858840229d5d47cfea7b76d7250fdb6a69d889` |
| `140646550..14064690d` alternate branch and lazy call | `41118d8351338c2ba055a5b9f5e117abcefd566baa2e8541d9b7a8fed8f6cffb` |
| `14064c030..14064c04a` singleton read | `bb174f25b9e0c7dbaa44176a4a66b18671b6319829ea09d05cf2b8e302794b03` |
| `14064ab80..14064ac40` selected default-branch prefix | `8ad819d6e33f164efe912da850eb6e0f47078d52d60d9a4e5c5050c6d66a1caa` |
| `14064ed40..14064eea2` independent lazy-caller chain | `62e29a655bb4f5dd81b165d18f2ec9c3ad56b18558ca9974f943b3f8f5f80454` |
| `14064e220..14064e342` selected predecessor | `0ab37a6d867abff0e25241ca21a87cfc2a13b8bd26c239a1de223a71fb0b5240` |
| `1406459d0..140645a4e` lazy-call prefix | `a26a9617c467c4fdc845f2445ca8f676a8c45c8ace5d3ba58c6f9f95f1d95c85` |

Read qualifications: CP161 `analysis/resume161-inherit-payload-next.md` SHA-256 `e1d1e95bc8ffb7e2805c4aac016dec09f8fdfe695abfbfec199c3ca509a09959`; CP160 `analysis/resume160-inherit-close-next.md` `f80e719ca754ad29d55e75c08077949eef33a556ed46ec77668f320d3de1c99f`; CP145 Report caller `analysis/resume145-report-document-edge.md` `d714c4f8d127710ef11350f582f2656f8e1e06c37996ca7aab165cb5a304ecba`; retained `L7/d2.c` decompile `256e16f538c67d7c88509d2fdff6b36057539a160e18337f677ee3f904007175` and `L7/s0.c` `66149afcfb363dbf81cd71d86549469d451b5ca2c169dc0cd7ea8a8ef5c2e935`. The decompiles are private snapshot analysis with unresolved indirect calls; the bounded original bytes above govern this report's static claims. No accepted-Close runtime observation is claimed.
