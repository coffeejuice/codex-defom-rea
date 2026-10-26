# CP98: Close's missing owner narrowed to constructor/lifetime integration

The original main-pointer publication and clearing sites are now located. This narrows CP89/90's producer gap, but does not make public Close executable: the candidate still has no actual shared process/main owner advanced through those constructor/lifetime phases and intervening Open/Save writers. CP88's actual notifier prefix and the source-closed QGenericArgument producer remain valid local pieces, not accepted-Close authority.

This report is the sole write. No source, build, test, GUI, original execution or mutable REA operation occurred. Evidence: current source, retained CP77/88/89/90 contracts and bounded offline PE/DUMPBIN reads. Immutable DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Current reconciliation

- CP88 `attemptCloseTreeNotificationPrefix` still internally obtains the Session's actual shared notifier. Suppressed skips without mutation; unsuppressed writes actual flag90=false and stops before2cf41f key QString construction. Retry does not replay the write. It has no public/core callsite and cannot certify earlier argument/root phases.
- CP89 closes that argument provider: QtCore670017f0 writes borrowed data+0/type+8,025080 forwards, and2caa63..2cac94 builds one false bool plus nine empty arguments before the synchronous notifier call. These stack views perform no allocation or event delivery. This is distinct from the later allocating/string provider at2cf41f.
- Current core/convert.cpp still has no normal-Close callsite before Session's expressly adapter-only cleanup. Close entry receives supplied busy/main observations; the true/false busy helpers mutate supplied isolated projections. Session's shared notifier143d76700 is not the separate application state143d766b8.

## Newly bounded original publication sequence

Native leaf2c8070 stores its second argument directly into receiver qword+0. It performs no guard registration or ownership retention. A bounded raw E8/E9 scan of the original .text found two direct candidates; both were decoded at their real enclosing function boundaries:

| Site | Proven sequence |
| --- | --- |
| c99530 initialization | First calls imported QCoreApplication::instance; nonnull invokesc993d0 with a true byte. Atc9956c it reloads its **same incoming this** into rdx, selects143d766b8 atc99574, and calls2c8070 atc9957b. Only afterward c99588 stores this+50=null. Native global main publication therefore precedes this main's ModelManager-slot initialization/publication. The QCore/c993d0 predecessor is not closed here. |
| c991e0 teardown body | Installs its base tables, frees this+118 through14dfc68 atc99237, then passes null to2c8070(143d766b8) atc99245. It next callsc99890 andca72b0. Do not move global-main clearing to project Close or before that predecessor free. Transitive teardown remains unqualified. |

Base constructorc98b00 callsc99530 atc98d23 on its saved incoming this. Retained `resume71-boolean-owner-population.md` and `resume77-boolean-mainwindow-qt-lineage.md` independently bind10490 ->c98b00 as the same SfMoMainWindow/SfMainWindowBase primary allocation. The second directc99530 call at01fc1b is within the large metacall body01e910; retained v6.c classifies its case0x45, so constructor execution is not established merely by that call target.

The singleton's own initialization is also located:141ca0bb0 selects143d766b8 and calls2c7ee0 at141ca0bbb. That constructor initializes members+10,+14,+20, then calls2c7f80 at2c7f27. In2c7f80, calls2cb760,2c81f0 and2cb900 **precede** explicit stores+9=0 at2c7fac,+A=1 at2c7fb6,+B=1 at2c7fbf. Later member/flag setup follows. Those earlier providers and the full relevant writer inventory are not closed here. Initial-image zero must not substitute for these constructor effects or a current post-Save value.

This corrects the overly broad remaining task “locate main publication”: that original static edge is now established. It does not establish a completed native constructor, the current same-instance main after Open/Save, or candidate production integration.

## Exact remaining dependency and next useful work

Current WidgetAreaPrimaryWindow is an isolated canonical primary-address **identity projection**; it has no Session binding, WidgetState/native constructor completion or original fields. Its requireQtBaseEffects/requireWidgetComponent refuse. The private DOE ApplicationStateProjection concerns separate observed process-type/DBLIST DWORDs, not this main pointer or busy bytes. Neither is a producer for Close.

The smallest useful integration prerequisite is therefore the **same process-owner/canonical primary-window binding at the actual c9957b publication phase**, after admitted QCore/base predecessors, followed in source order by main+50 and its real ModelManager factory. It must preserve the same mutable singleton through Open, explicit Save, nested Save and Close; account for subsequent+0/+9/+A/+B writers; and clear+0 only at the actual c99245 teardown phase. A new detached publication receipt would repeat the present gap. No implementation is source-ready until that actual constructor cursor/owner is available in the candidate.

Public Close must then read this actual owner's current+9/current-main. For nonnull main it needs current virtual+300 state (stacked widget+480 and main+270), not MainView or constructor-default facts. Successful .moproj Open retains nonnull moProject/process5: its default question, due-event effects and nested Save result still precede accepted true busy/setter/Tree delivery. A prepared-but-uncommitted null-Project failure seam is distinct from successful Open. Do not inject fixed unbusy facts, clear the global at project Close, call CP88 from Session destruction, or treat prior explicit Save as completed nested Save.

Once that constructor/root producer exists, the focused integration test must drive actual WRITE_SAVE/Open-to-Save-to-Close on one Project: verify identical process/main/manager/notifier identities; read current busy state; complete the qualified question/nested Save; perform true stores on that same owner; construct the now-qualified borrowed arguments; and observe CP88's real shared flag90 store before the next key-string guard. Preserve prior writes and owner state on refusal. Test main teardown clearing separately from Project Close. Current isolated tests cannot substitute for this sequence.

No narrow new **public** step is qualified by this bounded audit. No helper-only source work is recommended; the next query is constructor-cursor and complete writer binding for the newly located publication edge.

## Byte receipt

DUMPBIN14.44.35228.0, immutable PE section-aware mapping, end-exclusive spans. Raw call-scan scope was .text RVA1000/raw400/size1e01400; candidate calls were decoded, but the scan is not complete indirect-call coverage.

| Span | SHA-256 |
| --- | --- |
| 1402c8070..1402c8088 main setter | `dc6f5af7ad035e82f8c3c8397d73b6c187ec5a8d949b7a76ef4a31ef4407ca56` |
| 1402c7f80..1402c8057 initializer | `69933de76ee9cd162069c685e9b34575e7c44ca05940ebd2388becd6798c1991` |
| 140c99530..140c99590 publication through main+50-null | `d0cec6effeaeeed96144f74bfb264211ff8f617d65759d87afe628854e854d8c` |
| 140c991e0..140c9924a teardown through global clear | `c0745e5f8a5b6fd19383de4292c72cd72608e02df86a1a731a1eecf18335dd63` |

Retained inputs: resume88-close-tree-prefix-implementation.md, resume89-close-caller-root.md, resume90-close-singleton-handoff.md, resume66-dblist-global-lifecycle.md, resume77-boolean-primary-window-implementation.md, evidence/gui_mo/s1/mdt/L5/d8.c and evidence/gui_mo/s1/moproj_vt/v6.c. No new raw capture file or mutable REA session was needed.
