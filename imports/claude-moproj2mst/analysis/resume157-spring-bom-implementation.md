# CP157 SPRING2 BOM auto-detection implementation

## Change

The `SPRING2.INI` callsite now opts into the existing bounded ASCII-only Unicode BOM decoder. It accepts UTF-8, UTF-16LE/BE, and UTF-32LE/BE BOMs in Qt's source-qualified precedence, then passes only ASCII decoded bytes to the unchanged flag parser. UTF-32LE is checked before its overlapping UTF-16LE prefix. BOM-less high bytes remain rejected. The B_WEIGHT path still enables BOM decoding only through its existing one-value path gate; other `asciiBytes` callers keep the default BOM-disabled behavior.

The previous CP119 explicit UTF-8 BOM refusal is superseded for these five BOM profiles only. This change does not decode arbitrary Unicode, infer a BOM-less ACP, or broaden numeric/status behavior.

## Focused tests

The SPRING2 reader test now covers a two-row primary with distinct companion flags `[1, -2]` encoded under each of the five BOMs, verifies returned values and byte-for-byte preservation of both files, and checks a UTF-16LE BOM companion containing only one flag leaves the second pre-zeroed flag at zero. Truncated UTF-16 code units and a decoded non-ASCII UTF-16 code point remain rejected with both files unchanged. Existing BOM-less non-ASCII and malformed numeric guards remain in place.

## Static preflight

- Inspected the BOM branch order and verified it matches `analysis/resume58-qt-codec-for-utftext-overload.asm`.
- Confirmed the SPRING2 source call is the only newly enabled callsite; the B_WEIGHT boolean remains tied to its existing one-value route.
- Reviewed the test's UTF-8/16/32 byte construction and input immutability assertions.
- No build or CTest was run. No GUI or mutable REA state was used; root owns integration validation.

## Frozen hashes

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `8C1A89B9390E6803E38E66A7BB1B9742A6F87DF7A48433C80CFC90253D92B628`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `677005EE55152D933F5108DDBB0353F162E0B8E813DE13DA2264A913DC89A079`
