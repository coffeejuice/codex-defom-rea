# CP202: passive Close writer/reader cross-reference audit

The retained original closes the conditional action-to-Close-to-Project/SUB destruction chain. This pass finds no new source-ready cleanup cut: the missing dependency is a same-owner accepted occurrence and its mutable-field history, rather than another destructor address. Keep the current entry/nonempty-tail and feature/property/QPointer guards.

Scope: only this report is written. No source/build/test, original process, GUI/app-control acquisition, mutable REA, parent DMO, Claude, deployment or shared recovery-state changes. This uses the reverse-engineer-anything static-evidence workflow and the restrictions in `resume202-close-static-successor.md`.

## Evidence identity and qualification

Read the copied original `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, original **v11.0.2**, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, preferred base `0x140000000`. No reconstructed executable/version or deployed instance was inspected. Section mapping was independently parsed with `tools/doe_static_pe.py::PE`; `.text` RVA `0x1000`, raw offset `0x400`, raw size `0x1e01400`. Every code interval below is file-backed; ranges are half-open VAs. Full-byte hashes were recomputed directly from this copy, matching retained CP153/142/143/50 receipts where applicable. These are ordinary immutable-byte receipts, not REA Evidence IDs or observed execution.

Cross-checked addressed instruction byte prefixes in retained DUMPBIN disassembly against the copy: `wave3-boolean-close-{140031f10,1400133a0,140c78300,140044bf0,140045000}.asm` (6/13/122/25/104 addressed lines), `wave3-closure-next-14008a3c0.asm` (40), `resume160-type3-sub-destroy-1400b58d0.asm` (62), `resume160-type3-sub-owner-1400b5bb0.asm` (29), `wave3-closure-next-{140cbc920,140cbc900}.asm` (13/6), and `wave3-editor-native-parent-140130830.asm` (137). All 657 addressed prefixes match. Wrapped continuation bytes are covered by interval hashes, not counted as separately decoded instructions. A matching listing does not establish transitive callee completeness.

## Bounded xref/dependency table

M = primary main receiver; W = separately mutable application current-main; G = current M+50 manager; Mo = selected SfMoModel child; P = current Mo+19c0 Project; S = SUB selected from P+58. These names do not assert runtime equality across callbacks.

| Field/route | Qualified writer or producer | Reader/destructor dependency | Remaining limit |
| --- | --- | --- | --- |
| Main action bundle and Close action | CP153: main setup `14001087a ->1400121c0`; publish M+218 at `14001221c`; bundle constructor stores M into B+10 at `14000533d`; action return into B+18 at `140005798` | `1400057cb..1400057ff` connects current B+18, `2triggered()`, current B+10, `1slotActionFileCloseAll()`, type 0. `1400db5ff ->140031f10`; `140031f1e ->1400299a0` | The imported connect result is untested. Construction/publication and registered arguments establish intent; connection success, delivery and accepted state remain unknown. |
| W/application busy and actual M+50 | CP101 retains distinct application-current publication and busy writers; CP153 retains M-owned manager production. Close itself does not take a saved Project/Save result | `1400299cb ->1402ca920` queries singleton/current W; `1400299e6 ->140c9b8c0` queries current M+50; after accepted effects `140029a6e ->1400133a0` rereads/casts M+50, then `140029a76 ->140c78300` | Do not identify W with M or reuse the earlier G across modal/notifier callbacks. Startup defaults and prior false-busy are not current admission values. |
| G child list/current | Current list is walked in `140c78300..140c7854b`; child call `140c784f2` is **vt+18**, flag 1; table `142112878+18 =140045350` | `140045350 ->140044040 ->1400440c0 ->140044bf0`; list clear at `140c7852b`, explicit current-null at `140c7853e` follow child calls | A child's QObject guard death may precede explicit current-null. Shared lease release/current reset cannot replace the deleting occurrence. |
| Mo+19c0 Project | Earlier Open publication is a separate producer. Release `140044bf0` reads P, invokes `1400982c0 ->1400aab30 ->1412f1190(P+60)` | After feature callbacks, reread at `140044c1e`; Project wrapper call at `140044c46`; zero Mo+19c0 at `140044c60` only after return | First P and reread P need not be equal. 29 feature-key operations operate on that first actual P+60; map/provider effects and subsequent owner identity must be retained. |
| P diagram/SUB/context/members | Project destructor `14008a3c0..14008a466` resolves current diagram and calls vt+1b8(null) at `14008a3f8` | `14008a403 ->1400a80e0`: ascending SUB deletion through `1400ac330 ->1400b58d0`; then P+60 destruction at `14008a412`, P+58 storage, QStrings +30/+28/+18, base | Diagram detach is not receiver destruction. Neither null rebind nor SUB release discharges feature cleanup or later Mo slots. |
| S+48 and Report S+30 guards | S+30 lazy producer `1400b5ad0` conditionally constructs document, guard-publishes, then rereads (CP169). Exact construction/accepted parent remains conditional | S+48 reads `1400b58eb/1400b58fe`, delete `1400b592c`, assign null `1400b594a`; Report helper reads S+30 at `1400b5bc2/1400b5bd5`, delete second value `1400b5c03`, assign null `1400b5c21` | Fresh Close reads are not tied to Open/Save's document. The second null result skips delete but still reaches explicit null assignment; the first null skips both. |
| Guard slot update/read/destruction | `140cbc920..140cbc957` compares stored value, calls IAT `141e058b0` changeGuard only when different; `140c1aef0..140c1aefe` returns current stored qword | `140cbc900..140cbc919` calls IAT `141e042a8` for registration teardown. SUB destroys +48 guard, +40 embedded, +38 QString, +30 guard after raw +0 deletion | Target deletion, slot-null assignment, registration teardown and QObject incoming-guard clearing are distinct operations. No incoming target death follows merely from destroying this guard registration. |
| OPI property and incoming current/editor guards | Raw Props accessor `140051230..14005123f` reads OPI+10; `1400512c0` deletes nonnull Props through vt+18 at `140051307`, then zeros OPI+10 at `14005131f`. Editor writer `140130857` assigns editor+38 through changeGuard; reads at `140130871/140130887` follow | CP50/120: Props before engine-cache/object/position/recovery/item-model cleanup; outgoing members before OPI QObject base. OpClass current setter `1405ef280` and reader `1405ef2b0` use the guard helpers; QObject clearGuards writes incoming guard slots null before admitted destroyed recipients | No property deletion is supplied by shared_ptr reset, and no native OPI expiry follows from adapter row/storage death. Derived property payloads and actual editor/registry registration generations remain required. |
| Later Mo slots and manager state | `1400440c0` calls Project release before `140045000`; five slots +19c8/+19d0/+19d8/+19e0/+19e8 delete/clear in order | Operation manager/row/OPI/QStandardItemModel effects nest in these calls, before G list/current reset | Static wrappers do not prove row populations, signal recipients, posted events or completed Qt payloads. |

The complete SUB body reads guard +48, guard +30 and raw +0, then embedded members; it has no direct +20/+28 DOE/optimizer read/delete. CP142/143 already qualify this omission. It supplies neither a lifetime guarantee nor an invented relative deletion order for those published allocations.

## Exact byte receipts

Representative instruction bytes independently read from the copied image: connect at `1400057f9`: `ff 15 d1 e9 df 01`; metadata call `1400db5ff`: `e8 0c 69 f5 ff`; slot call `140031f1e`: `e8 7d 7a ff ff`; manager call `140029a76`: `e8 85 e8 c4 00`; deleting virtual `140c784f2`: `ff 50 18`; Mo backlink zero `140044c60`: `48 c7 80 c0 19 00 00 00 00 00 00`; Props zero `14005131f`: `48 c7 40 10 00 00 00 00`. SfMoModel slot `142112890` contains `50 53 04 40 01 00 00 00`.

The retained `wave3-closure-next-140c1aef0.asm` stops before RET; the copy independently supplies full 14-byte leaf `48 89 4c 24 08 48 8b 44 24 08 48 8b 00 c3`. Likewise Props accessor is `48 89 4c 24 08 48 8b 44 24 08 48 8b 40 10 c3`. This small completeness correction establishes pure readers, without establishing current slot values.

| Half-open VA range | SHA-256 |
| --- | --- |
|1400057cb..1400057ff|e805150ebbecf98ba6cf449e61da062942e89f314d9141c52bbdff2e01a66e56|
|1400db5fa..1400db609|c293b36f9578bf82064afbdb0c56400ecf93fa63431cc50237d013e38119976a|
|140031f10..140031f28|04dd3035662edf8caff271e6a476b625294ce8e76f63c09b4eb43edabbaee3d7|
|1400299a0..140029d04|8a3490e8c9007cd78fa2c3d876f2e071ee2e8bb113292ae70f944cb4a3a30d9a|
|1400133a0..1400133de|6b6904774a8f9b2108dc414f05ea8357bc09d7ac47f04d43727fcc027898ac69|
|140c78300..140c7854b|b2c665e2182ec5050431091d4859aafd7d86e1ca1ca2e4e7d14a36c5bf7b1487|
|140044bf0..140044c70|5a6af1df347276b2f42521f51aac2bddd65d6496d60a745d5c922f86170cf939|
|1412f1190..1412f134e|95532f69394a3f61e1eeb5cd30ff19a2cdc261bd535e67af06dc5fbbf2aff574|
|14008a3c0..14008a466|d33ef435aea6da822f0aa985431d82cba18f8a18374e724d0d76030e986614f9|
|1400a80e0..1400a818a|17d446ec2ab4fca352f9353858a184ae63a07c6d1f1e60df0c8931b89c1d9fa4|
|1400b58d0..1400b59de|2b35e06273c333eb3c9ee445daf5e8d27c2e54245c22236345e3bc2474beb3de|
|1400b5bb0..1400b5c2b|2817658eebd73a75f2829881363c149b68bf4b31501a5123a9543e874806b1c9|
|140cbc920..140cbc957|fadb5223030c46fcc06d80a5eacec37408b8a697f2249bf949755d9ea84af6b8|
|140cbc900..140cbc919|13e827f0805e0c97ff985371040318551bf7f8a5ce980fb5a83de5ceca316e4f|
|140c1aef0..140c1aefe|5bc1bfe5b72daf957f388c00813c416d4a2b8f719b9667d675ec3db8b1150732|
|140051230..14005123f|4e623613993e4445c9f912a73d466a162ee8aed60cc34ef7d382d30e278bbcc8|
|1400512c0..14005132c|57471f6e75d1c985a5f199541e467501ca51da8583ab7a9ee01fe64f31ab38d6|
|140130830..14013085c assignment prefix|7085775e70429ff53d053d69619549ec0a7d848a6303a64638f9327a11feca47|

## Source readiness and precise remaining occurrence

The pure guard/property readers add byte completeness only; their behavior is already represented/qualified in existing owner seams. No new production Close state, helper, public coordinator or storage cleanup edit follows. CP169/172 QObject-private/QVector prefixes do not produce M, G, Mo, P or their accepted teardown history. An isolated null leaf is not a source-ready integrated cut when its owner/entry predecessors remain missing.

A future permitted occurrence must retain concrete M/W/G/Mo/P/S allocation generations and action/slot delivery; fresh busy and can-close values; modal/nested Save answers separately from writer returns; accepted callbacks and their writes; both pre/post-callback M+50 and Mo+19c0 reads; P+60 constructor/Open registration/provider history and completed release; diagram detach; each S guard's publication, rebind, target death, both Close reads, explicit clear and deregistration; property/OPI payload and QObject guard-expiry ordering; five Mo slots, then manager list/current cleanup. If any callback selects a different owner, record the transition rather than assuming equality. Static reachability supplies no runtime values or completion witness for these edges.

Controlling qualifications read: CP153 `resume153-accepted-close-trigger.md`; CP101 `resume101-close-live-owner-chain.md`; CP142/143 `resume142-close-sub-disposer.md` / `resume143-close-indirect-owner.md`; CP144 `resume144-close-candidate-crosswalk.md`; CP50 `resume50-close-destructor-next.md`; CP54 `resume54-opi-expiry-owner.md`; CP120 `resume120-close-ownership-order.md`; CP169 `resume169-report-owner-next.md`; `wave3-normal-close-diagram-lifetime-contract.json`. CP81's older wording about an unlocated accepted-Close target is superseded by CP143/153's static closure, while its live-occurrence limitation remains. QtCore clearGuards internals here are retained CP50/120 qualification against their separately identified shipped DLL, not newly rehashed Qt bytes or a fresh runtime observation.
