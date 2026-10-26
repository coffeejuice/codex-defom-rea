"""Write the immutable, bounded successor observation proposal; no target calls."""
import hashlib
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / 'analysis/wave3-close-splitter-value-cache-observation-spec.json'
assert not OUT.exists()

def bound(name):
    p = ROOT / name
    return {'path': name, 'sha256': hashlib.sha256(p.read_bytes()).hexdigest()}

result = {
    'purpose': 'One passive, owned fresh-startup capture to bind current application font/palette cache identities and same-data preparation branches, plus the remaining directly consumed setParent tail inputs.',
    'status': 'Proposal only. No worker launch, GUI input, native getter calls or production changes.',
    'why_required': [
        'Probe002 observes final font/palette equality and data aliases, but not their upstream class-cache owner/query or all preparation inputs. Equality alone does not prove a lazy global cache was already present.',
        'QWidgetPrivate default QFont construction calls QApplication::font before widget registration, but the independent class cache has its own lazy getter. Earlier manager construction is style-conditional; desktop startup initialization has separate input predicates. No global absence/write-coverage assumption is substituted.',
        'Current application getters with nonnull cache owners perform const hash/class lookup and local value copies. Actual same-data inputs select sharing branches; no full QFont/QPalette recreation is needed for those qualified inputs.'
    ],
    'reuse': [
        'Use probe002 reviewed-v2 module observer, immutable original/Qt hashes, checked entry bytes, pointer-copy rule ptr(result.toString()), thread-scoped role/context selection and owned cwd/pause pattern.',
        'Use probe001 bounded raw readers for QObject filters, QInternal callback list3, pending guarded children and widget gesture contexts; do not repeat its broad event/child trace.',
        'Stop at the first selected View_Default d35a80 onLeave, before Primary. Retain every error, limit/truncation and call-context mismatch.'
    ],
    'native_calls': 'Only the already authorized owned-process SetCurrentDirectoryW/GetCurrentDirectoryW pair. No QApplication/QFont/QPalette/QLocale/metaobject methods, comparisons, native allocations or mutators. ACP/OEMCP need not be re-read because probe002 already directly observed them; do not expand scope.',
    'roles': 'Actual MainView, manager and SplitView; collect the same bounded two-placeholder/two-handle roles if reached before d35a80 return. Six selected parenting/value contexts maximum, with one additional diagnostic context allowance. Never inspect derived fields before derivedReady.',
    'hooks': [
        {'module': 'exe', 'rva': '0xd35a80', 'purpose': 'Arm actual manager/View_Default context and pause after its first selected return.'},
        {'module': 'exe', 'rva': '0xdab810', 'purpose': 'Identify the actual SplitView allocation receiver, without assigning derived QString/orientation defaults.'},
        {'module': 'gui', 'rva': '0x6dbd0', 'purpose': 'Selected actual setParent context: capture before/after child and actual old/new ancestor ownership, values, attributes and tail prerequisites.'},
        {'module': 'gui', 'rva': '0x611d0', 'purpose': 'Font preparation context: snapshot current receiver, parent, application cache owners and values on entry/return.'},
        {'module': 'gui', 'rva': '0x214d0', 'purpose': 'Only within selected font preparation and exact call return gui+0x5f38a: capture actual widget argument, output QFont object address onEnter, copied returned address/output value onLeave and cache before/after. Do not invoke the getter.'},
        {'module': 'gui', 'rva': '0x1f3ca0', 'purpose': 'Only inside selected font preparation: retain current QFont and other QFont inputs/masks/shared-data+38, output after natural return. At most two resolve calls per selected context. These input identities decide the source sharing branch.'},
        {'module': 'gui', 'rva': '0x68080', 'purpose': 'Palette preparation context; capture current/inherited fields and app cache state before/after.'},
        {'module': 'gui', 'rva': '0x20be0', 'purpose': 'Only within selected palette preparation and return gui+0x644cb: capture actual QWidget argument, natural output QPalette and class cache before/after.'},
        {'module': 'gui', 'rva': '0x4a2f0', 'purpose': 'Selected palette resolution inputs/output, at most two calls per context; raw group/resolve word is necessary. Do not call comparison methods.'},
        {'module': 'gui', 'rva': '0x5f940', 'purpose': 'Selected locale input/parent value and exact caller context; existing raw8-byte identity is sufficient for matching observed native equality branch, with an optional passive equality hook as in probe002.'},
        {'module': 'gui', 'rva': '0x65ee0', 'purpose': 'Final setParent opacity/cache tail only, return gui+0x6e219: capture child/ancestor private+d8, private+1d8 flags, extra flags and current palette alias before/after.'},
        {'module': 'core', 'rva': '0x119570', 'purpose': 'Only while selected setParent is active: raw0x44/0x45 to actual parent for selected child, raw0x15 to selected child. Read event type+10, child+18 where applicable, then bounded receiver/app callback/filter/gesture/layout inputs. No broad event trace.'},
        {'module': 'core', 'rva': '0x11ae90', 'purpose': 'Only selected setParent raw0x43: record an actual post if reached; absence alone is not proof no other pending events exist.'}
    ],
    'application_cache_fields': {
        'font_default_wrapper': 'QtGui+0x9c96b8 points to a QFont wrapper; if nonnull read wrapper data pointer+0 and resolveMask uint32+8.',
        'font_mutex': 'QtGui+0x9c9768 pointer; record destroyed byte+0x9c9770. No mutex method call or interpretation of live lock internals.',
        'font_classes': 'QtGui+0x9c97b0 QHash wrapper pointer; destroyed byte+0x9c97b8, guard word+0x9c97a8 and cleanup-reference pointer+0x9c97a0. Getter6501fea0 returns existing nonnull owner without writes.',
        'palette_default_wrapper': 'QtGui+0x9c9690 QPalette wrapper; same data pointer+0/raw group-and-resolve word+8.',
        'palette_classes': 'QtGui+0x9c9790 QHash wrapper; destroyed byte+0x9c9798, guard+0x9c9788, cleanup-reference+0x9c9780. Getter6501fd90 mirrors font cache ownership.',
        'hash_layout': 'wrapper[0]=QHashData sentinel. Data+8=bucket array, +0x10 signed refcount, +0x14 signed size, +0x20 signed bucket count. Each bucket is node*. Stop each next-pointer chain at sentinel. Node+0 next; +8 hash; +0x10 QByteArray key object; +0x18 QFont/QPalette value. Preserve bucket order and node identity; reject cycles/duplicates/malformed count.',
        'key_layout': 'QByteArray object[0]=data; signed size at data+8; char buffer pointer data+0x10. Read at most128 bytes and mark truncation; never call data()/constData()/hash.',
        'snapshot_policy': 'Full bounded hash inventory only on first selected preparation or changed wrapper/data/count signature; deduplicate identical states. Before/after every selected preparation still record wrapper/data/count/value identity signatures. Missing/null/changed/partial inventory remains qualified, not empty.',
        'bounds': {'maximum_buckets_per_hash': 256, 'maximum_unique_nodes_per_hash': 64, 'maximum_key_bytes': 128}
    },
    'widget_value_fields': {
        'font': 'private+0x180 QFont data pointer and uint32 mask+0x188; inherited mask+0x128. For each nonnull shared data, read signed reference count+0 and uint32 comparison field+0x38. Deduplicate by actual data pointer. Unknown payload fields stay unknown.',
        'palette': 'private+0x170 QPalette data pointer and raw uint32 group/resolve word+0x178; inherited resolve+0x12c. Record shared data reference count only; no colors/brush payload needed for same-data branch.',
        'locale': 'Read exact8 bytes at private+0x1a8; this is an opaque QLocale value, not necessarily a dereferenceable pointer. Read parent value only when actual widget parent exists.',
        'base_meta': 'Read current widget vtable and private+0x28 dynamic metaobject pointer. Known QWidget vtable is QtGui+0x6ab898. Null dynamic pointer selects static metaobject QtGui+0x9bd750; metaobject+8 is class-name pointer. Bounded raw string read only. Nonnull dynamic metaobject or another vtable remains a separate input, not assumed QWidget.',
        'attributes': 'WidgetData pointer must equal private+0x148 before reads. Read data+8 low attributes, data+0xc window flags; private+0x1c8/1cc/1d0 extended words and private+0x1d8 internal flags. These decide style97, explicit locale, layout direction, enabled/updates, hidden/created and opacity branches.',
        'style': 'private+0x88 extra pointer; if present extra+0x20 style pointer, extra+0x38 QString, extra+0x5c flags. QString object[0]=data with signed length data+8. Application stylesheet is QString at QtGui+0x9c9738. Read length and data identity, not methods.'
    },
    'set_parent_tail_fields': {
        'ownership': 'Child QObject private+0x10 actual parent, parent native identity, private+0x20 QObject flags, private+0xd0 polished-state pointer; old window and new actual parent chain identities retained separately.',
        'pending_children': 'Actual parent private+0x68 QVectorData: signed capacity+4, count+8, entries at+0x10 each16 bytes [guard-control*,child*]. Capture complete bounded prefix before/after raw44 and selected setParent return, maximum64 entries. Existing entries can be opaque identities; never replace them with an empty list or conflate with QObject children/logical splitter records.',
        'opacity_ancestors': 'Walk actual QObject parent chain to null (not merely first window), max16, cycle/invalid diagnostics. For each widget record private+0xd8 graphics-effect pointer, private+0x1d8 flags, parent and window flags. Native6505ebc0 sets bit16 on current and propagates to ancestors until an already dirty ancestor/window;6505eb50 independently walks all parents for effects. Nonnull graphics effects retain guard.',
        'opacity_receiver': 'Read child private+0xd8, low attrs bits4/8/9, extra+0x5c bit4, current palette pointer, private+0x1d8 bits16/17. For nonwindow/no auto-fill/no graphics-effect branch, source permits dirty-cache bit writes and unchanged false opaque bit; a true opaque transition calls97940 and stays guarded.',
        'backing_proxy': 'Old top-level QWidget private+0x88 extra and extra+0x10 proxy pointer, plus new parent chain; do not confuse extra+8 topExtra with extra+0x10 proxy. Nonnull proxy keeps exact later guard.',
        'events': 'At only raw44/45/15 snapshots use probe001 exact QInternal type3, live app/object weak filters with method module/RVA and affinity, app closing/thread, receiver installed layout and gesture contexts on actual widget ancestors. Known WindowsVistaStyle callback is QtGui+0x362520; do not replace observed nonempty filter list with empty. Unknown callbacks/gesture context population preserve guard.',
        'postcondition': 'Observe actual setParent return but do not treat it as a success bypass. Native ordering is values→direction→locale→enabled/updates→inheritStyle→raw44→pending child append→conditional raw45→raw15→hidden flag→opaque/cache invalidation→old proxy cleanup→inSetParent false.'
    },
    'typed_api_proposal': {
        'owner': 'Extend the existing Session-owned WidgetRuntimeState; do not introduce a second application or detached cache/flag snapshot.',
        'value_types': 'WidgetFontDataState and WidgetPaletteDataState are runtime-owned shared identity objects with explicitly unknown payload. WidgetFontValue owns shared_ptr<const WidgetFontDataState> plus resolveMask. WidgetPaletteValue owns shared_ptr<const WidgetPaletteDataState> plus exact groupResolveWord. Intern aliases within one qualified observation/runtime; reject foreign-runtime identities.',
        'cache_types': 'ApplicationFontCacheState/ApplicationPaletteCacheState hold actual wrapper identity qualification, optional complete class entries with exact key bytes/value aliases, and default value aliases. Incomplete population is explicit. Unknown class lookups guard; no successful-preparation boolean.',
        'binding': 'WidgetRuntimeState::observeStartupValues(const WidgetRuntimeValueObservation&) before SplitView allocation; observation binds actual MainView/manager WidgetState values and application owner/cache data from one immutable receipt. Fresh SplitView private values are copied from the same default owners at their original constructor point. Do not invent original pointer addresses in production; map observed alias relations to actual owned C++ value objects.',
        'operation': 'At ParentFontPreparationPending require the actual shared runtime, known cache query for current base metaobject, relevant style/window/parent state and exact data/masks. Compute parent/default selection and source sharing branch; use the same typed value objects for later palette/locale and widget ownership. Unsupported mismatch stops at the first actual source effect before mutation.'
    },
    'implementation_limit': 'Proposed cohesive increment is the exact observed same-data/font-palette branch through the real setParent bookkeeping return when callback, pending-list, graphics-effect and proxy prerequisites close. Otherwise retain a precise stage after all preceding effects. QFrame/QSplitter derived initialization, placeholders, grid item, View_Default name commit and public MainView aliases are not implied by this proposal.',
    'capture_limits': {'selected_parent_contexts': 7, 'value_contexts_each': 7, 'resolution_calls_each': 14, 'selected_events': 32, 'unique_states': 160, 'errors': 32, 'ancestor_depth': 16},
    'evidence': [bound(p) for p in [
        'analysis/wave3-close-splitter-probe-002-reconciliation.json',
        'analysis/wave3-close-splitter-focus-prefix-landing.json',
        'analysis/wave3-close-splitter-post-value-parent-contract.json',
        'analysis/wave3-close-splitter-parent-event-switches.json',
        'analysis/wave3-closure-next-close-splitter-font-global-getters-static.json',
        'analysis/wave3-closure-next-close-splitter-font-app-startup-static.json',
        'analysis/wave3-closure-next-close-splitter-font-local-resolution-static.json',
        'analysis/wave3-closure-next-close-splitter-palette-shared-preparation-static.json',
        'analysis/wave3-closure-next-close-splitter-palette-global-getter-static.json',
        'analysis/wave3-closure-next-close-splitter-cache-query-leaves-static.json',
        'analysis/wave3-closure-next-close-splitter-cache-key-data-static.json',
        'analysis/wave3-closure-next-close-splitter-base-widget-meta-static.json',
        'analysis/wave3-closure-next-close-splitter-parent-opacity-tail-static.json',
        'analysis/wave3-closure-next-close-splitter-parent-opacity-tail-body-static.json',
        'analysis/wave3-closure-next-close-splitter-parent-opacity-exit-static.json',
        'analysis/wave3-closure-next-650611d0.asm',
        'analysis/wave3-closure-next-6505f350.asm',
        'analysis/wave3-closure-next-650214d0.asm',
        'analysis/wave3-closure-next-65064490.asm',
        'analysis/wave3-closure-next-65068080.asm',
        'analysis/wave3-closure-next-6506dbd0.asm',
        'build/original-splitter-probe-002/probe-reviewed-v2.js',
        'build/original-splitter-probe-001/probe-reviewed.js'
    ]]
}
OUT.write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
print(json.dumps({'path': OUT.relative_to(ROOT).as_posix(), 'sha256': hashlib.sha256(OUT.read_bytes()).hexdigest()}))
