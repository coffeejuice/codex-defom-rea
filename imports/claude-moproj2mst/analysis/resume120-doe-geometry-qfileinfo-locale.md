# CP120 private InputTable Geometry: QFileInfo and locale boundary

## Result

The retained evidence supports one safe private projection: for a named, bounded absolute-drive or UNC InputTable filename, compute the **conditional normal-return value** of `QFileInfo::path()` and the three literal Geometry path joins. It does not support claiming that a native QFileInfo constructor completed for a parser occurrence, or admitting a complete Geometry child even in the private parser. The public Geometry guard must remain.

The numeric side has no general source-closed host implementation. Original `QString::toDouble(nullptr)` first parses with the process default QLocale, and only on failure retries with C locale. The default profile is supplied through process QSystemLocale/cache state; the retained evidence does not bind that profile for an original converter process. A C-only parse changes behavior whenever the default locale accepts a different spelling or value first.

## QFileInfo value versus constructor completion

The retained QtCore image is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the original GUI PE remains SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The Geometry reader constructs QFileInfo from the actual source filename at `0x140911542` and calls `path()` at `0x140911559`, before reading GeoOperation (`analysis/resume116-doe-geometry-payload-boundary.md`). In retained QtCore, the normal QString constructor resolves an engine/handler, builds private caches, and clears the private default-object bit before return. The normal getter then takes the file-entry parent path route (`0x670b7a70` -> `0x670ef4b0`), rather than consulting engine-name/cache state. The file-entry parent calculation is a lexical last-slash operation; the custom resolver can rewrite an entry for other filename profiles.

The resolver evidence closes a useful *value-only* domain: after native separator conversion, an absolute drive path (`C:/...`) or leading-slash UNC path takes the no-search-prefix branch; with existence checking disabled, the normal resolver path returns without rewriting the copied entry. The projection receipt therefore bounds filenames to those forms and to fewer than 32768 UTF-16 code units, and calls its result conditional on normal construction/return. Other colon-prefixed names may enter search-path resolution and rewrite the entry. See `analysis/resume113-doe-geometry-path-projection.md` and its retained `resume113-doe-fileinfo-path-static.json` / `resume113-doe-fileentry-parent-static.json` captures.

That condition is material. Static code proves the sequence and normal-return value selection; it does not prove that the constructor's handler traversal, engine/ref/cache setup, allocations, or the parser's actual constructor invocation succeeded. A helper over `InputTableSource::path` can return a conditional value but cannot stand in for those native effects or report successful QFileInfo admission. No exception/failure behavior for those effects is source-closed in the retained evidence.

For that reason, the narrow safe private operation remains a **value projection only**: accept the named-source value and exact Geometry attributes; restrict the filename to the qualified profile; return parent plus `"/"` plus Upper/Lower/Nominal attribute strings, preserving raw text and order. It performs no I/O, Qt ownership effects, row mutation, numeric conversion, or success claim. An explicit internal helper can test this contract without being called by `parseInputTable`.

## Numeric conversion contract

Original Geometry code converts GeoOperation with `QString::toInt(ok=null, base=10)` before the later PointPair child work. Each correspondence string is split using literal `", "`, KeepEmptyParts and case-sensitive mode; token indices 0–3 are accessed, then four `QString::toDouble(ok=null)` calls occur before QPointF construction and pair append (`analysis/resume116-doe-geometry-payload-boundary.md`).

The retained QtCore wrapper at `0x6706c340` creates a default QLocale and calls its string-to-double parser first. If that parse fails, it creates `(language=1,country=0)` and retries with that C locale. The fallback identity is established; the default locale is not hard-coded. Its default constructor can query a QSystemLocale provider and select data from a process cache. Retained static evidence does not bind the provider's values or the complete numeric fields used by the selected record for a fresh original process (`analysis/resume114-qt-default-locale-trace.md`).

Consequently, replacing the call with the host converter's C-locale helper is not equivalent. It skips the preferred parse and can change both success and value. The evidence also does not justify accepting only a hand-picked decimal spelling as a locale-independent subset: no cross-profile equivalence proof for that subset is present. The safe private projection stops before numeric conversion. GeoOperation and correspondence values remain unsupported until the locale/result contract is supplied or a permitted original-process witness binds it.

## Integration disposition

`readInputTableFromSource` passes a named `InputTableSource` to `parseInputTable`, but the parser source parameter is currently unused and the Geometry branch explicitly refuses before QFileInfo ownership (`source/apps/moproj2mst/src/doe/doe_study.cpp:427–431`; `source/apps/moproj2mst/src/doe/variable_record.cpp:183–188`). CP114/115 receiver helpers already model selected row-prefix and pair-append effects on owned objects, but their input values are post-conversion values and their isolated existence does not close this entry boundary.

No private full-child admission is source-closed. Do not wire the path projection into public parsing as if its normal-return condition were a completed native constructor; do not call the editor's stored-GEO loader from XML traversal; and do not parse using C locale as a proxy for default QLocale. Keep both private and public Geometry child admission guarded at the current boundary. The next decisive evidence is either a bounded original-process observation of QFileInfo normal completion plus default-locale/provider and conversion results for one admitted case, or a fuller source chain that closes those exact process-dependent effects. The separate editor-triggered GEO payload consumer and project Save callback remain independent lifecycle questions.

## Scope

This is a read-only reconciliation of retained CP113/114 and CP116 evidence and current parser call sites. No GUI, mutable REA database, source edits, build, or tests were used.
