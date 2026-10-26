# CP134: delivered General title change on the bound private editor

The next source-closed action is the title `textChanged(QString)` callback. Native code immediately copies the delivered text into the current simulation control's title. The private owner currently retains edited General text until Apply, so it has no equivalent delivered-event transition. This can be added to the existing bound StagedEditor300 page owner without constructing widgets or crossing the public palette boundary.

Only this new report was written. No source, test, header or CMake edit, build, GUI, original execution or mutable REA operation occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The investigation used retained REA decompiles/metadata, current source and bounded DUMPBIN against that immutable original. It establishes a static callback contract, not an observed fresh-session delivery.

## Native callback and its order

Constructor connection evidence line107 connects UI+128 `textChanged(const QString&)` to `slotTitleChanged(const QString&)`. The retained slot map index2 dispatches at1410db66e and calls140ecb8e0 at1410db67f. The complete callback is `[140ecb8e0,140ecb93e)`:

| Address | Effect |
| --- | --- |
| 140ecb902 | QString::toLocal8Bit of the delivered argument into a local QByteArray |
| 140ecb90e | QByteArray::data; retain its pointer |
| 140ecb91c | Resolve the editor's current simulation control via130f90 |
| 140ecb927 | Call14146fec0 with destination=current SC+0 and source=the retained byte pointer |
| 140ecb932 | Destroy the local QByteArray |
| 140ecb93d | Return |

Import-directory lookup independently resolves IAT141e03f50 to QtCore4_64 `?toLocal8Bit@QString@@QEBA?AVQByteArray@@XZ`,141e04228 to `?data@QByteArray@@QEAAPEADXZ`, and141e04290 to `??1QByteArray@@QEAA@XZ`. These are the actual three indirect calls above, not inferred function names from the slot alone.

The complete leaf14146fec0..14146ff76 copies bytes through the first zero and returns the original destination. Its unaligned byte loop stores before testing zero, and its aligned loop copies complete words only while no zero byte is present, then copies/test bytes individually. There is no destination length argument, truncation, padding or target-range test. The candidate layout declares `char title[0x51]` at SC+0; `oprnam[0x41]` begins at+51. Therefore this recovery must not introduce native truncation or clear the unused title tail.

The title callback has no OPI mutation, general-page Apply, refill, viewer refresh or file effect. It does not test whether text equals the old SC title; a new explicitly delivered event copies again. An empty converted string copies its terminating zero. Actual QString/QByteArray allocation, detach/reference counts and exceptional conversion behavior are outside the proposed scalar-value projection.

## Smallest private implementation contract

Add only this method to the shared owner header, with root ownership of that declaration:

```cpp
ApplyResult StagedEditor300::titleChanged(
    Editor300Binding, const qt::String& deliveredText);
```

The cached General title must already equal the delivered argument, representing the actual changed control value before declaring event delivery. Follow the current owner-generation checks rather than a free function receiving an unrelated SC. Admission requires the same live Project/Operation generation/GuiModel/remesh binding, completed private page state, same source-model/remesh identity and matching cached General title. A native-base-pending or partial owner cannot create pages through this method.

Admit only text whose UTF-16 code units are printable ASCII20h..7eh and whose length is at most80 bytes; zero length, including the existing null/empty QString content equivalence, is allowed. Reject non-ASCII, embedded NUL, other controls and length81+ before model stores. This is an explicit candidate safe-buffer/codepage profile, not original validation. It avoids inventing locale replacements or reproducing an unbounded overflow. Excluded native overflow/partial failure behavior remains unrecovered, rather than silently truncated.

After metadata/admission, convert to the local ASCII byte string, then resolve the actual current SC through the bound model and require the session's original sourceControl identity. Do not call `pageState()` before this sequence: it performs a current-SC lookup itself. Null/replaced SC is an UnsupportedSnapshot prerequisite guard before any title store; native null handling is not claimed graceful. Propagate later adapter exceptions unchanged.

Copy exactly the bytes plus terminating zero into SC.title, leaving bytes after that zero untouched, and return Completed. Do not call applyGeneral: that would also write unit, operation name, simulation type, process flags and update mode. Do not refill General or change the cached title's null/empty representation. Other pages, OPI fields, remesh and files remain unchanged. No actual widget sender, signals or constructor completion is inferred from this explicit private event.

Exclusive worker implementation set: existing `src/engine/simcontrol300_owner.cpp`, NEW `tests/mdt/test_simcontrol300_title_changed.cpp`, and a new implementation receipt. Root adds only the owner-header declaration. No simcontrol300.cpp, lifecycle, CMake, public loader or palette edits are needed. Existing test glob discovers the new file; expected target is `m2m_mdt_simcontrol300_title_changed` / CTest `m2m.mdt.simcontrol300_title_changed`.

## Focused test contract

1. Construct completed private pages with actual Project/Operation/GuiModel ownership. Edit cached title and declare delivery; assert immediate SC.title change without Apply, unchanged cached text/operation name/unit/trans/mode/OPI/remesh and no additional side-file reads/writes/removes.
2. Pre-fill SC.title with a long sentinel, deliver a shorter title and verify the byte after the new terminator retains its old value. Deliver an empty/null title and check only byte0 becomes zero. Test exactly80 ASCII bytes plus terminator without overwriting the adjacent operation-name buffer.
3. Change SC.title externally while cached delivered text remains the same; another explicitly delivered event copies the cached text again. Do not add a same-value shortcut based on page state.
4. Length81, non-ASCII, embedded NUL/control characters and a mismatched delivered/cache title refuse with all model bytes unchanged. These are guarded candidate profiles, not assertions that native conversion rejects them.
5. Destroy or replace the actual model step after retaining the private page pointer; event refuses without dereferencing stale SC or substituting the replacement. Wrong binding, expired/reused OPI generation and partial/unconstructed owner refuse before stores. Public mode300 construction remains Unsupported/PrivatePaletteUnavailable.

## Frame/constructor reconciliation and remaining public witness

The selected frame callback140ecb6f0 first calls140eb5500, then reads the actual navigation QListWidget currentRow. The enable path consumes current combo text and touches picking/model/view state. It subsequently resolves the tab index and enables/disables a real QTabWidget tab, then maps navigation rows0..9 to stacked indices `[0,8,1,2,3,4,5,6,7,9]`. A page enum or requested-enabled value does not establish these actual receivers, event state or earlier effects. A pure row-to-index mapping would make no faithful integrated progress and is deferred.

Operation-name callback140ecb950 initially resembles title, but after copying to SC+51 it rereads SC, creates a QString from that stored name, calls the actual OPI name setter050a90, then140eb5250 to update project/tree state. Do not silently combine it with the title method or label a buffer-only prefix complete. Unit callbacks invoke a separate current-unit/dialog path. CG normalization is already recovered; rewrapping it without resolving actual radio-delivery ownership does not close a new constructor dependency.

Private `constructPageState` explicitly composes recovered page blocks and stops AwaitingNativeBoundaries. The actual Session still reaches the earlier palette dependency: no current selected-frame private palette/data/refcount witness joins the runtime owner to the actual constructor call at6504abb0. The precise next public evidence remains the same selected receiver/frame's live palette data/ref history at that call, not a global startup profile or seeded count. The title event is therefore a useful private behavior increment only; it does not remove that public guard.

## Evidence identity

- Title callback `[140ecb8e0,140ecb93e)`: `ed05f3b29fd33c2e755739cf258dc7fcfda2cd3bd0d699080db76f02304c0058`.
- Complete copy leaf `[14146fec0,14146ff77)`: `e764439361042e76186d7af1c85fae07782ab3d24f8456555719d88f55614030`.
- Operation-name callback `[140ecb950,140ecba10)`: `7f724a0ac544a311e93d243fcd4ca2d47a7ade08e4cca1f2517aaa307db6d8a1`.
- Frame callback `[140ecb6f0,140ecb8cb)`: `021c34f53cfcd69635d63abb499a37463d922509f4abb85af4f3d5c5686ea33f`.
- Retained constructor connections JSON: `1621275823b3e0dfb75cc8822ae961a13a8e701de6a70f690bba3bda9ee50335`; slot map JSON: `9bbf83751e7f7606f039392b6e775f9a20cf3984fb1e3654b9e32c6d4a669661`.
- Retained140eb5500 decompile: `cee64835a04481c167ba4eb711e99c36defe02f6aa15ad5ba0861e8b71bbfbc3`.
- Read-time owner source: `65af624f02b7a322a041d0f114bdf2bfba1aa5d22a9da538c20dd061c4378e8c`; simcontrol300.cpp: `04b62ac1da494473cb26e1dcb3d1ae93ad087cf3c3fa4409f37e8ee6336f9a80`.

Static DUMPBIN was the pinned BuildTools14.44.35207 executable SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`, banner14.44.35228.0. Initial range attempts lacking a hexadecimal prefix were rejected by DUMPBIN and contributed no evidence; the successful bounded views used explicit0x ranges. Only this report retains the new results. No reconstructed executable/deployment was inspected or produced.
