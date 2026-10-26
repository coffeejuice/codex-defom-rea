# CP104 DOE Geometry test reconciliation

Updated only the two assigned DOE tests to reflect the already-frozen Geometry-entry prefix contract. Direct Geometry is no longer grouped with pre-append unsupported profiles: its reached header owner/state is asserted, followed by the deliberate refusal at `Geometry entry 0x140911532` before QFileInfo/path/geometry ownership.

In `test_variable_record.cpp`, the direct Geometry case now checks the header record alias, name, sample count, ID, dimension, unset index, selection index, unchanged table bytes, fresh invalid flags, and absence of Description/files/items/ops/geometry payload. The other malformed/numeric pre-append guards remain unchanged.

In `test_variable_deferred_link.cpp`, Geometry was removed from the generic no-mutation guard vector and given its own test: a `LinkTo` request remains unresolved when Geometry refuses after header installation. The later-row duplicate test now expects the second row's ID `10` to be retained before the same refusal; the earlier Description and index `73` remain, and its pending link still is not resolved. Numeric, foreign-owner, wrong-mode, Keyword, prior-container-link and Save-guard assertions remain as before.

## Frozen files

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/tests/doe/test_variable_deferred_link.cpp` | `5BB7DA363AFC8B70A31E7B999752A3143358B62EF8C5081CDCB2F700D0BF78AD` |
| `source/apps/moproj2mst/tests/doe/test_variable_record.cpp` | `0F080A9A122106C0D300959F49D89CE08A662BE70EC812A0EE463C910A359CB6` |

Static receipt: searched both files for Geometry expectations and confirmed no remaining direct-Geometry no-mutation assertion; checked both files for trailing whitespace (none). No build or tests were run, as assigned. Both test files are currently untracked in this checkout; their exact content hashes above are the release receipt. No production source or other tests were edited.
