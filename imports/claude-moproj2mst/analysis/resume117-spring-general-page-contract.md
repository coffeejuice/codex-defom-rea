# CP117: reusable positive-count Spring page contract

Replace fixture-specific row/value/count predicates with a reusable typed-list admission and unchanged selected-row0 projection. The original loops are count-driven, not a table of one/two-row fixtures. A first generic internal profile can support complete ASCII finite records, absent companion/flag0, bounded successful allocation and no item click/edit/reselection. It remains below the actual Session palette gate and does not claim original runtime/UI occurrence.

## Source sequence and item ownership

Original140ec8110 first reads the scalar sidecars, then140ec8b00 populates both combo lists and140ec8e70 initializes text controls; only then does it clear the Spring list and invoke1410dab40. On reader success it constructs one custom0x78 list item per record in reader order. Each stores the copied0x40 record at+0x30 and preserves the passed context at+0x70. The QListWidgetItem constructor's zero argument is its item type, not insertion row0. No sorting/reordering is introduced into the bounded list model.

After insertion, live list count determines the page checkbox. Stored current row becomes-1; for a nonempty list, Open calls setCurrentRow(0), obtains item0 and explicitly invokes140ecdee0. The constructor connection at retained line109 is `itemClicked(QListWidgetItem*) -> slotSpringLoadDieSelected(QListWidgetItem*)`; it is not currentItemChanged. Do not invent an additional click/writeback from programmatic selection. The scoped no-click path contains the explicit initial load, then the Save-time explicit call; other Qt/event deliveries remain separately qualified.

The callback returns immediately for a null requested item. Otherwise, if the stored old row is not-1 and its item exists, it obtains current combo IDs, parses five visible numbers, derives checkbox0/1 and writes that full payload back to the old item BEFORE comparing the requested row to the old row. Same-row returns after writeback; changed-row loads the new payload, selects matching combo entries only, formats doubles with general precision6, sets the checkbox and records that row. Missing combo match leaves the prior selection unchanged; do not repair it to the record ID or default0.

## Smallest reusable state

Existing SpringSelectionState already contains the essential semantic model: one ordered vector of item payloads, page checkbox, stored/current row, two selected IDs, five visible texts and flag checkbox. For this no-click/no-edit profile:

1. Take complete typed reader records into the ordered item vector. Do not sort, deduplicate, renumber or normalize every item.
2. Set pageChecked from nonempty count and select row0. Require only row0's IDs to exist in its qualified combo item-data lists; load row0 IDs/texts/flag. Unselected rows never invoke the combo lookup on this path, so foreign or repeated IDs in those rows are not a native rejection condition. All-row membership would be an extra conservative candidate gate, not required original behavior.
3. At Save, require selected row0, unchanged selected IDs/texts/flag relative to its admitted projection. Copy the ordered payloads, write the selected visible row0 back into the first local payload, then serialize the entire snapshot. Leave rows1..N-1 untouched until the writer formats their fields.

This is an internal value/list projection, not one native QListWidgetItem object per C++ vector element. Padding bytes within the native0x40 record are not observed semantic fields; do not assert host padding or a raw native payload hash. Finite double values, signed IDs, order and flags are retained as typed fields. Copying/serializing duplicate records is valid; no uniqueness requirement appears in the reader/list/writer loop.

The current const page API materializes the selected writeback in a local Save snapshot. That is equivalent for the admitted no-edit/no-later-item-inspection output cut, including repeated idempotent Save, but is not a claim that native item memory was updated in place. Future click/edit/state-inspection integration needs an actual mutable item owner and commit/load operation at the same callback boundary. Do not infer those transitions from the writer snapshot.

## Admission without fixture literals

Coordinate with `resume117-spring-general-parse-contract.md`: complete supported ASCII stream grammar; source C numeric locale; all required fields consumed; absent SPRING2 supplies every flag0; stop after declared count and ignore remaining ASCII suffix rather than require EOF. Partial/malformed groups, unknown conversion/range/codec cases and original allocation cleanup remain guarded unless separately qualified. Finite output alone is not proof of every numeric token's native conversion; reuse only the reader's admitted conversion profile.

For the first implementation, use explicit candidate resource cap256, shared in meaning with the reader. It is not an original format maximum: the source accepts a positive signed count, attempts its allocations and loops; counts above the candidate cap must report unsupported, never clamp or silently truncate. Check count/size conversions and arithmetic before allocating. A count-generic algorithm with this operational cap eliminates a fixture per count while keeping resource failure qualified. Allocation failure is not a simulated successful empty page or rollback; preserve the explicit unsupported/resource boundary until native allocator/failure ownership is recovered.

At the page boundary require1..256 complete rows, all five doubles finite and flags0. Do not constrain row IDs/numbers/directions to CP115/116 values. Selected-row0 combo membership remains required. For the no-edit Apply branch, selected controls must match that row's initial formatted values; edited text, reselection and nonzero flags remain explicit excluded profiles. Empty/missing/nonpositive-reader behavior retains its separately implemented path.

## Writer and failure order

Original140ec9040 attempts NBC/SW2SP/STRETCH/B_WEIGHT/STRAIN effects before its Spring gate. At that gate it reads live list count and checkbox: off or count<1 removes both spring files. Otherwise it invokes the selected-item callback, allocates count*0x40 snapshot with checked multiplication and copies every item+0x30 by list index before1410db030.

The generic writer emits count+LF, then for each row firstObject+LF; firstValue,secondValue,secondObject,direction0+LF; direction1,direction2+LF. It adds TAB after every non-LF element and one finalCRLF. Integers use base10, doubles general precision6. There are1+3N separator LFs plus finalCRLF. Selected row0 has already passed display/parse; later rows retain their original numeric payload until this final formatting.

Flag0 means main SPRING.INI attempt first, then readable-open/delete of SPRING2 even when main open fails. Writes are direct, nontransactional; helper true reflects main open, not successful stream/Close. No rollback, atomic replacement or byte-residue claim follows. Late unreadable/locked companion can remain. Future nonzero flag support must preserve its different gate: write companion only if main opened; ignore result at the outer Save caller.

Current reconstruction preflight rejects excluded Spring state BEFORE scalar sidecar writes. That is a candidate admission guard, not source validation order and not original partial-effect fidelity for those excluded cases. Preserve this qualification explicitly. Any later failure/partial-state extension must move its reached-operation accounting to the original Spring point after earlier scalar effects and retain those earlier effects, rather than claim that early refusal reproduced them.

## Implementation/test cut after root source release

No shared-header change is required for the first count-generic no-edit slice. Reserve simcontrol300_io.cpp and its existing Spring page component test together; reader owner separately generalizes native input and tests. Keep lifecycle's deferred combo scan after scalar reads and public Session6504abb0 refusal unchanged. A broader mutable/click owner requires a separately reviewed shared-header/API change.

Meaningful tests replace literal admission with: three distinct arbitrary finite rows and a dynamic cap-sized list; mixed whitespace plus writer-normalized reread; selected row0 rounding while later payload bits stay unchanged; duplicate/unselected foreign IDs; selected missing ID refusal; count-cap+1, partial/range/codec/companion guards; no edit/reselection; output order; main-open failure followed by companion delete; post-open write/close continuation without invented residue. Test values beyond6 digits distinguish row0's display roundtrip from untouched later payloads. Expected writer bytes should be literal/source-derived for small cases and independently assembled from the stated stream grammar for parameterized cases, not generated by calling the candidate writer itself. Use short disposable C:/PROBLEM sidecars, retain raw diagnostics, and keep full Session/original acceptance separate.

## Evidence and release

Original SHAfd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. Retained Open140ec8110 SHAe531263876d6a9c2809f605f2ebee92ff04a526f29e7d97391560353a57ef97f; Save140ec9040 SHAd08b47a4529b3d50e7c1cf4a8247ba98c09f3f4d78efee8212b44f7dcc6e4b32; constructor connections SHA1621275823b3e0dfb75cc8822ae961a13a8e701de6a70f690bba3bda9ee50335. CP114 item/selection captures SHA022927c43dd8742634d95a02a5e3c7efb65a39327f741b9c8295e6ea2256d495 / fae61eecdc45c7e99e6d4ed9a19f8daE7c4864eee4b69ef934f1eb4493edc6c2. Peer CP117 parse contract SHAc4fce5d18aeae36f8fcdc972d4392857ed92b435f285035bf2fc985d0ef9e4f6; callback/writer audit SHAcee3caa6cbe0ce3a3b2907923c3438625bd9d1ff11895b22da4830483907802f. Its all-row combo statement was subsequently refined in direct coordination to selected-row membership for this no-click cut.

Only this report was written. No source/build/test, GUI, original execution or mutable REA database operation occurred. This is a reusable internal contract with explicit remaining gates, not full native/public editor completion.
