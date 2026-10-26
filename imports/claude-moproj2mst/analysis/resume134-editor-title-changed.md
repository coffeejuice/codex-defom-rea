# CP134: bound private General title delivery

The private StagedEditor300 owner now copies an explicitly delivered title change into its actual current simulation control immediately, without applying the rest of General. The admitted path preserves conversion-before-SC lookup and copies through the terminating zero without padding the remaining title buffer.

Production/test source is frozen and released to root. Only the assigned owner source, new focused test and this receipt were edited. Root supplied the shared owner-header declaration. No worker compiler/build/CTest, GUI, original execution, mutable REA operation or executable/deployment change occurred.

## Source qualification

Frozen design: `analysis/resume134-editor-next.md`, SHA-256 `b290fad3eedcc72e74c2677be3eee7c8a26d4a1ac6a0f56d9ebe18afa61b9784`. Original DEF_GUI_MO_64 v11.0.2 SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The constructor connects UI+128 textChanged(QString) to slotTitleChanged; slot-map index2 reaches140ecb8e0. That callback converts the delivered QString with toLocal8Bit at140ecb902, gets QByteArray::data atb90e, resolves current SC via130f90 atb91c, copies to SC+0 through14146fec0 atb927, destroys QByteArray atb932, then returns. Exact title callback `[140ecb8e0,140ecb93e)` SHA-256 is `ed05f3b29fd33c2e755739cf258dc7fcfda2cd3bd0d699080db76f02304c0058`; complete unbounded zero-terminated copy leaf `[14146fec0,14146ff77)` is `e764439361042e76186d7af1c85fae07782ab3d24f8456555719d88f55614030`. The design receipt records exact IAT names, connection/slot hashes and bounded static tool identity.

This establishes a static explicit private event, not observed native textChanged delivery or complete constructor behavior. Native QString/QByteArray allocation, reference/detach and exceptional conversion effects remain outside this value adapter. No operation-name/OPI/tree tail is inferred from the distinct title callback.

## Implementation

`ApplyResult StagedEditor300::titleChanged(Editor300Binding,const qt::String&)` requires the same live Project/Operation generation/GuiModel/remesh binding and completed private page owner, same source-model/remesh identities, and cached General title content matching the delivered event. It does not use pageState during admission, because that getter performs an early SC lookup.

The method admits only zero to80 printable ASCII code units20h..7eh, including empty/null text under the existing QString content equality. Oversize, embedded NUL, control and non-ASCII profiles refuse before model stores. These are candidate buffer/codepage admissions, not claimed native validation. The original unbounded overflow path is not silently truncated or enabled.

The admitted text is converted to local ASCII bytes, then the actual bound model's simCtl is queried. Missing or replaced SC returns UnsupportedSnapshot without dereferencing the retained source pointer or substituting the replacement. Later source exceptions propagate unchanged. A memcpy writes the admitted bytes plus their NUL into title[81], leaving the remaining bytes untouched, then returns Completed. The cached text, including its null/empty representation, is retained. No same-value shortcut is added.

The method does not call applyGeneral, change other pages, write SC operation-name/unit/process/mode fields, mutate OPI/tree/remesh, or perform file effects. Native-base-pending and partial factories still refuse. The public palette constructor/PrivatePaletteUnavailable gate and completeForPublicSave=false remain unchanged.

## Focused coverage and static checks

New build target `m2m_mdt_simcontrol300_title_changed`; CTest `m2m.mdt.simcontrol300_title_changed`. Eight Release-active Testkit cases cover:

1. Immediate SC title update from the actual bound private page, without General Apply; changed cached operation-name and differing live model fields remain untouched, along with OPI/remesh/other page and I/O counters.
2. Shorter replacement copies its NUL and preserves the old buffer tail.
3. Exactly80 ASCII bytes terminate at title[80], preserving the adjacent operation-name field.
4. Null and nonnull empty delivery store only byte0 and preserve cached nullness and every later model byte.
5. Repeated explicit delivery recopies after an external SC title change.
6. Length81, non-ASCII, embedded NUL/newline/tab, mismatched event and wrong Project binding refuse without model stores.
7. Destroyed or replaced actual model step refuses without stale-source use or replacement substitution.
8. Unconstructed/partial owner and an explicitly declared OPI identity boundary refuse; the diagnostic boundary is not labeled native destruction or public constructor completion.

Static preflight verified eight cases, exactly one main, no C assert or unsupported REQUIRE_EQ, resolvable quoted app includes, existing defmodel layout include and DB_STEP_DATA(3) constructor. Comparing CP133's frozen source.zip against the new owner source shows only the cstring include and new method. Existing CMake CONFIGURE_DEPENDS test discovery supplies the target; no build registration was edited. Root still must build and run the focused group and required regressions; no test pass is claimed here.

## Frozen files

| File | SHA-256 |
| --- | --- |
| source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp | `43a24def1e3bb5e1d9380e604c56dcc1d05f1c300c3cbfec110eeabf2553ec74` |
| source/apps/moproj2mst/tests/mdt/test_simcontrol300_title_changed.cpp | `5112bf42c01cf43f9fbbab24cb9acac753a47674f739696422ad64c87436a7e6` |

Files are held for root's source/binary binding, validation and archive freeze. Root integration is the immediate dependency. Actual frame/picking/tab/stack receivers and the selected constructor palette producer remain separate guarded work; this private event does not supply those owners.
