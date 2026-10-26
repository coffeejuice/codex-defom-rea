# CP78 runtime app_pal owner join

Source is frozen/released for root integration. WidgetRuntimeState now owns one
canonical isolated QApplicationPaletteOwnerState bound by an explicit exact-module
slot observation and this runtime's interned palette data. Unknown/null/present
remain distinct. No getter read/provider, private palette copy, placeholder resume,
public callsite or counter mutation was introduced.

Root approved exclusive view_manager.hpp/.cpp and the new test after the read-only
composition report. Only those source files and this receipt were written. No
application_palette_owner or splitview_placeholder file, editor/session/public
code, CMake, shared plan, build/test execution, GUI, original execution or mutable
REA changes. Token usage is unavailable.

## API and source boundary

New WidgetApplicationPaletteSlotObservation carries:

- qualification: exact existing WidgetRuntimeState::qualification().
- moduleSha256: pinned QtGui module hash.
- optional slotRva: must explicitly equal9c9690.
- optional<bool> slotPresent: absent=unknown, false=null, true=present.
- optional paletteDataAlias and groupResolveWord: BOTH required for present;
  both absent for unknown/null. Explicit word0 is distinct from missing input.

`void WidgetRuntimeState::observeApplicationPaletteSlot(const observation&)`

`shared_ptr<const QApplicationPaletteOwnerState> applicationPaletteOwner() const noexcept`

The header forward-declares the owner rather than including its header, avoiding
the existing application_palette_owner.hpp -> view_manager.hpp dependency cycle.
The implementation includes that header and adds one Values-owned shared owner.
The accessor is immutable historical/current diagnostic state only; it cannot
invoke the nonconst getter read/provider API.

Source qualification is CP76/77 plus resume78-splitview-palette-compose.md:
actual native slot QtGui+9c9690 is distinct from shared palette data and the
class-key palette cache. A same-runtime value alias is not a native pointer.
The observation supplies explicit current slot knowledge, never derives it from
startup defaults or a prior receiver outcome.

## Observation semantics

Preflight requires a shared live runtime with nonempty qualification and an
existing Values registry, matching receipt, pinned module hash and exact slot RVA.
It rejects absent registry rather than silently preempting observeStartupValues'
one-shot setup with a new Values allocation. Envelope, alias and data identity
checks precede creation/mutation/publication of the app_pal owner.

Present data resolves through THIS runtime's palettes map. Alias0/unregistered
alias and missing word are errors. If current defaultPalette data is already
known, a different data owner is rejected; no default/source transition is
invented. The wrapper word is its own local QPalette value field: it can differ
from a preexisting value's word while sharing data, and neither is overwritten.
This preserves the separate private-output word0 edge for later work.

Unknown module-qualified observation creates only an Unknown owner. A later
explicit null observation refines that same owner to KnownNull with no wrapper
or initialization. A later explicit present observation from Unknown binds one
host wrapper around the SAME registered data/control block. Repeated equal
observations reuse exact owner and wrapper. Null/present replacement, changed
word and data substitution reject without changing the retained state. Unknown
observations never erase known state. KnownNull->present requires the separate
source initialization path; observation cannot pretend that path ran.

The internally created owner has stable qualification
runtimeQualification + /QtGui4_64.dll/app_pal@9c9690. No externally copied helper
can be installed. Explicit same-owner checks preserve control-block identity on
existing default/wrapper data, in addition to the registry alias join.

No read(), constructor provider or OutputCopied operation is called. No private
input record is created, and no receiver argument/circular completed-palette
prerequisite is needed for this module-global observation. The existing default
palette and class cache remain untouched. Escaped const diagnostic owners do not
hold the runtime alive and remain historical after it expires; this increment
provides no native read capability after runtime loss. A later retained native
consumer must revalidate its canonical runtime/owner origin on each continuation.

## Focused tests and static checks

New test_widget_application_palette_binding.cpp has11 unique TEST_CASEs,52 CHECK
expressions,5 REQUIRE expressions and one Testkit main. Cases cover:

1. Startup default/class cache do not infer app_pal; module-qualified Unknown has
   no wrapper/private input/count effect.
2. Null is distinct from Unknown; repeated/unknown observations preserve it and
   cannot fake initialization into present.
3. Present binds one wrapper to exact same runtime shared data/control block,
   preserves nonzero word and leaves the MainView local word0 unchanged.
4. Wrong/missing receipt/module/RVA preflight does not publish an owner.
5. Present requires explicit positive registered alias and word; explicit word0
   is accepted without overwriting the existing default value's word.
6. Unknown/null cannot carry present payload fields or publish partial owners.
7. Conflicting null/word/data preserve exact owner/wrapper.
8. Current default data cannot be replaced; a runtime without a known default
   can bind registered data without creating a new default value.
9. Identical local aliases in two runtimes retain distinct owners/wrappers/data.
10. Missing registry or unshared runtime refuses without publication.
11. Const diagnostic owner does not retain runtime or claim getter/private/count
    execution; expired-runtime diagnostics are explicitly historical.

Test fixtures use actual independent MainView/manager/runtime owners and explicit
isolated data observations. They do not fabricate a placeholder or original slot
state. No test executes QApplication getter/native initialization or public Open.

Static preflight passed: include resolution, header forward-declaration cycle
review, supported CHECK/REQUIRE/TEST_CASE arity,11 unique
cases, one main, no C assert/REQUIRE_EQ, balanced delimiters after comments/literals
were stripped, and no trailing whitespace/conflict markers. Direct comparison
against CP77 source.zip found15 added header lines and45 added implementation
lines, with zero removals. No production observation caller was added. No build,
compiler, focused-test run or CTest result is claimed. Root owns the automatically
registered target m2m.mdt.widget_application_palette_binding and grouped/full
validation/archive.

## Frozen files

Paths relative to source/apps/moproj2mst/:

| File | SHA-256 |
|---|---|
| include/m2m/engine/view_manager.hpp | e69f43da56b543c5fa5ca91f6660d43361d3724a4eb0b58b3e5b0541594e3615 |
| src/engine/view_manager.cpp | 0ab08f3cb4ac4a8dc24a9469153cdf518c5f8e8654072dc4080641b464e2ffb3 |
| tests/mdt/test_widget_application_palette_binding.cpp | e1db62bee6b3254bd1ccd8be5a944edcf7cf6c6a3fc015b6d9fb559a68ea62bc |

Unchanged helper hashes: application_palette_owner.hpp
ac6badf7155d8c1c7b1cfd816719e3685d5b3e65dbb2731f51f41a346b858e58;
application_palette_owner.cpp
58d6355b88150698b0af87128e8994f3595fa2c4d0a8db995aec1875d7851348;
splitview_placeholder.cpp
80427dacd058628cfd6baf0b699ee4cd43b861f1c041abf99e1f20e8a1ae0e04.

Pinned original QtGui remains
c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887.
No new original value was observed. Next dependency is a runtime-mediated retained
getter output on the actual placeholder, followed by source-ordered temporary
release/private word0/destination ref closure. Existing earlier SplitView, public
private-palette, font, counter-history and Save/Close guards remain unchanged.
