# CP102: next actual-Study-owned DOE Open branch

## Recommendation

The next narrow public Open increment is deferred `LinkTo` resolution for an exact direct `DoeVariable` row on the actual active Type2 `Study`. This branch is independent of Save refresh: Type2 activation binds/reads the Study, `readLazyStudy` calls `readInputTable`, and the caller continues the existing sample file attempts after a normal parse. The current candidate has the record owner and pointer aliases, but `appendBoundedHeaderVariable` refuses any nonempty `LinkTo` before appending the row.

Bound implementation to two fresh, uniquely named, header-only direct variable rows in one input: the first row has a nonempty `LinkTo` naming the second; the second has no link; no other variable payload or foreign/prefix owner. Finish all row creation and normal input-table dispatch first, then resolve the recorded pair and set the source record's `Var.link` to the unique target. This exercises the native forward-reference order without changing Save/output admission or inventing duplicate-name policy. Keep the guard for unresolved, duplicate, or otherwise unqualified targets until those outcomes are separately represented.

## Native contract

Original GUI PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The retained complete reader capture `analysis/resume50-doe-variable-body-pe-compact.asm` binds the native order:

1. At `0x14090f357..0x14090f408`, read `LinkTo`; if empty, skip queueing. If nonempty, append the source variable and target-name values to two deferred arrays. It then continues through direct child payload handling and later rows.
2. After top-level input child traversal, `0x1409128b6` invokes manager vtable `+0x1e0` reindex before the deferred-link loop.
3. For each pending pair, `0x140912967` calls manager vtable `+0x180` by target name; `0x140912982` calls `+0x1a0` with the returned index; if both source and target are nonnull, `0x1409129ab` calls `0x140100a60` to assign the pointer. No match leaves the source unchanged/null and iteration continues.

Bounded PE captures in `analysis/resume102-doe-link-target-static.json` identify `+0x180` as first matching name index (`0x14091b5d0`) and `+0x1a0` as indexed variable lookup (`0x14091b950`). `0x140100a60` stores its pointer argument at receiver `+0x40`; getter `0x1401cc6b0` returns that same field. This proves static native operations and their order, not runtime invocation. Original static queue helper `0x140004b90` remains opaque; the proposed candidate may use a local typed vector and does not claim native Qt container/refcount/fault parity.

## Candidate seam and other branches considered

The public reader's first current LinkTo guard is `source/apps/moproj2mst/src/doe/variable_record.cpp`, in `appendBoundedHeaderVariable`: nonempty target returns false before record creation. The row is dispatched from `doe_study.cpp::readInputTable`. The minimal source seam is to return the created/current actual `Var*` plus target text for this narrow row profile, retain pending pairs locally in `readInputTable`, and resolve after normal child traversal and the existing reindex phase. Test through actual ConversionSession/Project/active Type2 Study Open/lazy read and assert pointer identity, target field, later PAR continuation, and earlier partial state if a later child guard fires. No Save success follows: nonempty-variable Save still stops at its separate dimension/refresh guards.

No smaller independent output or Type3 optimization child was established. DOE Session import still has default-part/Task factory and populated DBLIST/ReportList ownership guards; Type3 ShapeOptim retains Study/QFileInfo/file/geometry prerequisites. B122's geometry record/binding and isolated XML writer are explicitly candidate-only helpers; they do not supply public GEO ownership or activate public geometry import. Existing no-op Operations and no-row child decisions are already covered by CP83/88/89.

## Evidence qualification

Current source hashes: `doe_study.cpp` `a9096c1217ed6e782b7a785c114b3c2e58024f44201c44e12ea508b8cf276b7e`; `variable_record.cpp` `8ac71e4cef1dcf29d5ab66ad364b54be4f8cf307b8b6216489dd8afebacc0ff7`. PE capture receipt `resume102-doe-link-target-static.json` SHA-256 `63b06906c4e55306d8d0c4f848b6381ca6a9d027dcf413afa33c602cd833068e`; link-index and indexed-pointer captures SHA-256 `1a24ed468923a07e17f4b5ad1ce420fcd0b80979f997ea2c2468bd62fc6f72f1` and `b134f00bb278528f24f15a759e777eb8ee2c87446766b772f0851b9e97969386`; setter/getter captures SHA-256 `307e1c1dc81b87cfd61499266ef83a1af1d6bcc73b12309c1b21b17dcedb1be0` and `4c325eb41ddc796555b8c9ebc066a5ebf4c76e7682c12ab7c4e276949904b4e6`. `analysis/resume91-doe-variable-owner.md` records actual concrete Study owner and its constraints.

This was read-only source and retained-static review. No REA database, GUI, original runtime, source edit, test, or build was used.
