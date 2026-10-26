# CP141: removal has a source-closed null leaf; browser callbacks remain a different owner

New bounded PE inspection followed the removal continuation requested by CP139, rather than repeat its snapshot order or the Qt timer/style census. Native removeProperty1413ddbc0 has a call-free null-argument leaf. For a present non-null property, its later browser-removal path reaches a concrete browser virtual+1b8 whose three constructor-produced targets are now resolved. Neither fact supplies Boolean E/OPI or a first-Open occurrence.

Only this report was written. Bounded immutable PE/DUMPBIN and retained-index reads were used; no source/shared edits, build/tests, GUI/app control, original process or mutable REA/import occurred.

## Newly bounded source branches

removeProperty1413ddbc0 stores its incoming browser/property arguments, tests the **property** at13ddbd7, and on null jumps13ddbdf ->13ddc90 ->RET13ddc94. That selected entry-to-return path contains no call and does not dereference browser B. It is a null-argument return, not proof that a browser list was empty or that a property was removed. Unknown and null property must stay distinct.

For non-null property the function copies the current top-level list at13ddbee and scans pointer equality at13ddc22. Exhaustion runs local-list cleanup13ddc8b and returns. A match erases the current list entry13ddc38, then calls private removal13dc8a0 at13ddc4e, private continuation13dcff0 at13ddc64 and local-list cleanup13ddc6f. This order is why CP139 counts normal removal returns rather than successful deletions.

The freshly inspected13dcff0 obtains related item/manager state, can return early at13dd070, otherwise filters a list and calls recursive removal13dd1b0 at13dd166. In13dd1b0, child items are processed recursively at13dd212 before the browser callback **virtual+1b8 at13dd231**. This is not a Boolean editor slot. Direct reads of CP138's three browser tables give:

| Browser table from constructor | virtual+1b8 target | Private continuation called by wrapper |
| --- | --- | --- |
|142e55988 /13da8d0|1413dab10|13d9800 at13dab2c|
|142e55308 /13d7510|1413d7750|13d6510 at13d776c|
|142e54dd8 /13d3080|1413d39d0|13d25f0 at13d39ec|

These are concrete conditional callback recipients on browser B. They do not establish B=Boolean editor E, nor that any callback occurred in the selected Open. Their transitive Qt/widget effects remain unclosed; no absence of Apply through them is claimed.

## Smallest private source cut

The next fully bounded private branch is an owner/request-bound **null removal leaf**. Given the same outstanding CP139 removal request and an explicitly admitted removal entry with property argument exactly0, a private projection may report that native13ddbc0's selected branch has no browser/list/callback effect and reaches its return suffix. Missing argument/entry/owner qualification stays pending; a non-null argument must stop before13ddbee's list-copy/scan work. Scope/lifetime/index/property/invocation tokens must match the actual request.

Such a helper must distinguish a **source-modeled null branch** from an observed original normal return. It should not manufacture CP139 PropertyBrowserRemovalObservation::Returned, CP136 clear completion, Qt delivery or public Apply readiness. Synthetic tests could cover unknown versus null, mismatched/expired request, exact null no-effect branch and non-null guarded continuation. No helper files were created here; there is no newly admissible public first-Open transition.

## Actual witness still missing

The CP124 retained-stream index was recomputed:39 message-bearing files,1,490 raw entries, unchanged digest **beac2e4189662280238f9eb4c29cf27d9f0891e7ab2b21265d919ebb5911b1b7**. It does not supply the native same-owner timing frame. CP139's candidate helper still ends CleanupPending and generates no normal clear return.

For a tree-timer origin, the smallest missing frame remains actual T/registration-record lifetime ->notifyInternal67119570 with caller return14231a ->matching654a895e/widget predicate8978 ->layout8998/normal return899e before complete first Open. For the separately recovered property-refresh route, the needed frame is actual notifier local3 emission ->live SfPropertyEditor P registration/thread dispatch ->slot1376590/interface1375950 ->D+8 browser B ->same removal/clear callback/normal return. A static connection, constructor table or null leaf is not that occurrence.

Either positive Boolean-creation claim additionally needs nested actual current-widget UpdateData ->Boolean E Apply1411e2870 ->E+38's resolved OPI O ->normal add return1411e295a or1411e2bcb with O, inside the same complete Open span. Keep synchronous zero-slot postLoad add1411df0e9/return0ee separately attributed. No currently retained owner relation equates the generic property browser's virtual+1b8 with this Boolean frame. The next source action, if desired, is one selected browser wrapper's private callback producer; the next original-evidence action is the precise same-owner nested frame above.

## Reproduction

Use the original EXE SHA256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc and verified DUMPBIN14.44.35228.0 path in CP135. Selected end-exclusive code bounds:

| Range | SHA256 |
| --- | --- |
|1413ddbc0..1413ddc95|0a400be5ebde58fc8aedb1cfef1a556de883a7886d5e06a19d984fbdddb33a92|
|1413dcff0..1413dd1a0|483209108e007fb80330c35b3125191242a3a58b256f9021ae998c8bbfbda872|
|1413dd1b0..1413dd342|3cf94f5d981fae289a0c8c0ace544a0469da22dbb7fca9beb68dd99291eb1ed2|
|1413dab10..1413dab36|77140347d0b47631f970e7fcef8f6cebde38804b2145bf590f30308e828f87ee|
|1413d7750..1413d7776|47b1d6d31e6d04227e2384e9fe069a367bd13436bdea3eb112eb9a33f9a5131e|
|1413d39d0..1413d39f6|5d432130b25bf6b20d3e9ed41a4e17d524a2f8bd00426bdba712dcd8b435f8de|
