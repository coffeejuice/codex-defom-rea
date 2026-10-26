# CP90 independent review: all five recognized BOMs under the B_WEIGHT scalar gate

No blocker was found in the released source. The decoder follows the retained Qt BOM precedence, including four-byte UTF-32LE before the overlapping UTF-16LE prefix, and admits only ASCII-valued units after recognized markers. Filename/count/open/ASCII boundaries remain unchanged. Review of extension source began only after the owner explicitly froze and released it.

Only this report was written. No source/test edit, build, executable test, GUI, original runtime or mutable REA action occurred. Paths are relative to `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst`.

## Reviewed bindings

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp (311 lines) | c043d36ff5fe5bf54809c7316db01b3aa90f1783d6f848d807b468271f4755c7 |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp (628 lines) | 6a4a3741d59e0dd42abfb7e2a4908b095830ffb974bb88fb99761e3871647fe9 |
| analysis/resume90-bweight-all-bom-ascii.md | ddc81b9d2c400095c2b350d806adcb04586f65719204e8582ce4babbb7440496 |

The hashes match the owner's frozen release and receipt. Baseline comparison used the actual CP89 source archive, recorded source tree `2133579882cf4daf3cdf350df3da834756c31b1c0798bfa779b1105975fbc201`, archive SHA-256 `f44c4697da2b09a57fcb6598751d3f328d816e313bc95381de9a890a5c7c800e`. All translation-unit text outside asciiBytes is identical to CP89 after newline normalization.

The integer helper and form fill remain byte-identical, hashes `7784b177f2df89c2ea8ba413a40b51b5a985a2d40d7f5cae0e21a976ed05a8d7` and `3e796cc212cc02767485848eb67a6f8c44dd049161061ca01bd01a6eed45b7d4`. This is a source review, not an executable binding or runtime pass.

## Native codec precedence

Retained analysis/resume58-qt-codec-for-utftext-overload.asm (SHA-256 `c474c0c796b286d74e9d916e388bcad36ecfb6cbe935790cb771da6c8b12b3c0`) shows these exact signature decisions in QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`:

| Order | Marker | Native MIB branch | Candidate action |
|---:|---|---|---|
| 1 | 00 00 FE FF | 0x3fa at 0x6714e079 | offset 4, width 4, big endian |
| 2 | FF FE 00 00 | 0x3fb at 0x6714e09a | offset 4, width 4, little endian |
| 3 | FE FF | 0x3f5 at 0x6714e0bc | offset 2, width 2, big endian |
| 4 | FF FE | 0x3f6 at 0x6714e0d1 | offset 2, width 2, little endian |
| 5 | EF BB BF | 0x6a at 0x6714e0f2 | strip three bytes, retain ASCII scan |

The candidate's if/else chain has the same order and length preconditions. Longer signatures win before shorter overlapping prefixes. Ambiguous raw prefixes therefore follow the native signature decision rather than a caller-supplied encoding label. No UTF-7, alternative byte order, BOM-less Unicode, locale/ACP or generic codec auto-selection was added.

This expansion is explicitly broader than CP89's guarded UTF-32 boundary. The new receipt supersedes that old limitation and turns the previously guarded UTF-32LE fixture into a strict 1.5 positive check. That is a declared source-qualified increment, not a suppressed failure. Original B_WEIGHT count-one/default-QTextStream caller qualification remains the retained FUN_140ec8110/FUN_1410da510 evidence; no original process was run.

## Unit assembly and malformed protection

The private decodeAsciiUnits lambda is called only with offsets/widths (2,2) or (4,4), after the corresponding prefix length check. Its subtraction cannot underflow. It requires `(size-offset)%width==0` before indexed access, so each position+i refers to an existing complete unit. Width is never zero and the maximum shift is 24 bits.

Each byte is converted to unsigned char, then to uint32_t before shifting and OR. UTF-32 high bits remain unsigned; there is no signed extension or signed-shift overflow. Every assembled value above 0x7f throws unsupported before char conversion. Admitted values are representable as char and are identical ASCII characters under the corresponding Qt codec. UTF-16 surrogate/non-ASCII units and non-ASCII UTF-32 values remain rejected; no surrogate composition, replacement or truncation is attempted.

The lambda builds a separate string while reading the original bytes, then assignment replaces bytes only after decoding returns. Its reference capture does not invalidate an active read. The unchanged final high-byte guard still protects unmatched raw input and UTF-8 payloads. Marker selection happens only once, so a second marker is not recursively stripped; it produces non-ASCII units/bytes and is guarded.

Recognized markers with incomplete remaining units fail explicitly. Partial marker spellings containing high bytes fall through to the existing guard; ASCII-only ambiguous prefixes such as zero bytes retain prior raw-ASCII behavior and are not claimed as decoded truncated Unicode. This preserves the distinction between byte-pattern matching and a general malformed-encoding classifier.

## Exact outer gate and cursor

The unchanged readDoubles predicate is `values.size()==1 && path.u16()==u"B_WEIGHT.DAT"`. Other names/spellings/path forms, count greater than one and all integer paths pass false BOM allowance and retain their existing high-byte gate. Count zero still returns successful open before reading/decoding; missing/open-denied files still return false first. SPRING and later STRETCH double readers have not acquired codec support.

readReal still starts its cursor at zero in the normalized ASCII body. For each encoded 1.5 CRLF vector the decoded five characters are identical; the scanner leaves CR at decoded offset 3. That corresponds to raw offset 6 for UTF-8, 8 for UTF-16 and 16 for UTF-32. These are source-derived offsets, not observed QFile positions or a newly exposed cursor API. Each later request opens independently. The numeric parser, finite-value check, form formatting, integer parsing and Save/write paths remain unchanged.

## Focused coverage and release decision

Native IO case count rises from 22 to 23. The former UTF-16LE case is renamed/expanded to include a successful UTF-32LE vector. One new case adds UTF-16BE and UTF-32BE positive vectors plus a truncated UTF-32LE negative. The UTF-8 case is retained.

All five positive byte vectors are exact ASCII 1.5 CRLF encodings and require read success/value 1.5. They use explicit lengths or unsigned-byte arrays, preserving embedded NUL and marker bytes. Negative coverage retains malformed UTF-8 body, odd UTF-16LE length, non-ASCII UTF-16LE unit, non-one-value request and other filename; the new truncated UTF-32LE fixture requires unsupported. The common alignment/unit-value loop provides source protection for the other endian forms even where a separate malformed fixture is not added. No positive-case skip or success-or-unsupported allowance is present.

Existing quoted includes resolve under actual source/target include paths. Tests remain Release-active Testkit under the configured disposable Rig directory. Original-runtime parity, full Unicode/codec behavior, malformed-native replacement/status effects and complete page/lifecycle acceptance are not established. Root can proceed with the frozen hashes, focused native-IO execution and required full source-stable validation/archive. No passing-test, timing or token-saving claim is made; token measurements are unavailable.
