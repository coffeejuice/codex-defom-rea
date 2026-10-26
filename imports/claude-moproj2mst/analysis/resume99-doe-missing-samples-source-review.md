# CP99 missing `numSamples` source review

The frozen bounded source/test matches the native value result and owner branch for its admitted profile. Static review found no blocker within that value-model scope; no build or tests were run here.

## Native chain and reached effects

The retained reader body (`resume50-doe-variable-body-pe-compact.asm`, original GUI PE hash in its receipt) constructs a default QString and calls `QDomElement::attribute("numSamples", default)` at `0x14090efae..0x14090eff1` without a `hasAttribute` branch. It calls `QString::toInt` at `0x14090f01f` with `ok == nullptr` and base 10, then passes the returned integer directly to manager `vt+f8` at `0x14090f0ac` before optional ID/dimension and later reader work.

For an absent attribute, the CP97 QtXml chain returns a copy of the caller's default-null QString. CP99's QtCore captures bind `QString::toInt` at `0x6706c280` to `toLongLong` at `0x6706c120`, and `QLocalePrivate::stringToLongLong` at `0x6704c460` to its length-trimming path at `0x6704c190`. The empty QString path (`length <= 0`) returns parse failure before digit parsing; the conversion wrapper retries the C locale after default-locale failure, which also fails on empty input. The resulting numeric value is 0, accepted by `toInt`'s int-range wrapper. The caller discards conversion validity, so it continues with 0.

One material side effect remains outside the value abstraction: `toLongLong` constructs/loads the default `QLocale` before the empty-string check. Its constructor may initialize and publish Qt's process-global locale cache (`0x672f3d40`, with lazy initialization through `0x6704cd30`). The implementation's direct zero value reproduces the record value but does not replay or prove that runtime global initialization. This limitation is explicitly called out in the source comment/header and is consistent with the existing host value seam; do not describe the conversion as locale-free.

On a fresh name, manager helper `0x140919de0` allocates/constructs the `0x48` record with the argument at `+0x10`, then appends and performs its normal name/selection/invalidation effects. Thus the record's `numSamples` is 0. Under the source's mode-0 restriction the helper skips mode-2 design acquisition. On a duplicate name, the helper exits before allocation/constructor and ignores the supplied sample value; the reader nevertheless proceeds to apply later supported setters to the last record. This remains true for the tested A,B,A sequence, which preserves sample counts 3 and 5 while applying the duplicate row's ID/Description to B.

## Frozen source and tests

The source maps absent `numSamples` to `optional<int>(0)` while keeping present values subject to the existing canonical-positive restriction. The missing-value branch is admitted only with explicit nonempty name, positive `varId`, and exact dimension 3; other required mode/owner/LinkTo/payload bounds remain. That narrower profile is an intentional reconstruction admission limit, not native validation. It prevents this one-field increment from admitting bare `<DoeVariable/>` or rows missing other fields. Explicit empty/null names remain outside the missing-samples profile, while CP97/98 behavior for those values with present positive samples remains separate.

The five new tests cover fresh zero-count ownership and constructor defaults; duplicate argument ignore and last-record targeting; reached zero record before variable-list/sibling guards; unqualified/malformed/header/payload/mode/foreign guards; and actual Session-owned lazy sample continuation followed by the existing Save reconciliation refusal before XML/notifier effects. The actual-owner fixture confirms the sample files are still read despite the record count being zero, within the admitted mode-0 slice. Existing CP97/98 empty/null-name paths remain isolated from this profile.

This review is static. Root owns grouped build/test validation. No original execution, GUI, source edit, or mutable REA action was performed.
