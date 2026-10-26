"""Produce a bounded, evidence-qualified closure inventory; never run original code.

Lexical sites and inherited ledger rows are discovery indexes, not reachability proofs.
Writes only analysis/closure-audit.{json,md}; paths are relative to the takeover root.
"""
from __future__ import annotations
import collections
import csv
import hashlib
import json
import re
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
APP = Path('source/apps/moproj2mst')
LEDGER = Path('source/docs/re/moproj2mst/ledger.tsv')

def sha(p):
    return hashlib.sha256(p.read_bytes()).hexdigest()

def evidence(path, line=None, address=None, qualification='retained static source; not an execution observation'):
    out = {'path': path, 'qualification': qualification}
    if line is not None: out['line'] = line
    if address: out['original_address'] = address
    return out

def finding(id, priority, title, classification, addresses, sources, dependencies, unknowns, validation):
    return dict(id=id, priority=priority, title=title, classification=classification,
                status='open', original_addresses=addresses, evidence=sources,
                dependencies=dependencies, unknowns=unknowns, required_validation=validation)

def main():
    ledger = list(csv.DictReader((ROOT/LEDGER).open(encoding='utf-8-sig'), delimiter='\t'))
    counts = dict(collections.Counter(r['status'] for r in ledger))
    by_addr = {r['address'].lower().removeprefix('0x'):r for r in ledger}
    subsidiary = []
    for path in sorted((ROOT/'source/docs/re/moproj2mst').rglob('*ledger.tsv')):
        if path == ROOT/LEDGER: continue
        rows = list(csv.DictReader(path.open(encoding='utf-8-sig'), delimiter='\t'))
        sts = collections.Counter(r.get('status','<missing>') for r in rows)
        disagreements=[]
        for row in rows:
            addr=row.get('address','').lower().removeprefix('0x')
            other=by_addr.get(addr)
            if other and row.get('status') and row['status'] != other['status']:
                disagreements.append({'address':addr, 'packet_status':row['status'], 'main_status':other['status'], 'row':row})
        subsidiary.append({'path':path.relative_to(ROOT).as_posix(), 'rows':len(rows),
                           'statuses':dict(sts), 'main_status_disagreements':disagreements})

    # Retain every matching site on disk, but never present it as a count of defects/functions.
    site_patterns = {
        'explicit_refusal':r'\b(?:unsupported|notPorted|not_ported|M2M_STUB)\s*\(|throw\s+Error\(M2M_E_UNSUPPORTED',
        'omission_or_qualification_comment':r'not ported|unported|not traced|not modelled|dropped|deviation|known deviation|not in the original|silently|TODO|FIXME|omitt',
        'persistent_state_candidate':r'\bthread_local\b|\bstatic\s+(?!cast|assert|constexpr|const\b)|\bg_[A-Za-z0-9_]+\s*=',
    }
    sites={k:[] for k in site_patterns}
    source_files=list((ROOT/APP/'src').rglob('*.cpp'))+list((ROOT/APP/'include').rglob('*.hpp'))
    for p in sorted(source_files):
        lines=p.read_text(encoding='utf-8-sig',errors='replace').splitlines()
        for n,line in enumerate(lines,1):
            for kind,pattern in site_patterns.items():
                if re.search(pattern,line,re.I):
                    sites[kind].append({'path':p.relative_to(ROOT).as_posix(),'line':n,'text':line.strip(),
                                        'addresses_on_line':re.findall(r'0x14[0-9a-fA-F]{7}',line)})
    shared=[]
    for module in ['deformdb','port/defrt','port/defmodel','port/defkey','port/defdbio','port/defcmd','port/def_pre']:
        base=ROOT/'source'/module
        files=list((base/'src').rglob('*.cpp'))
        candidates=[]
        for p in sorted(files):
            for n,line in enumerate(p.read_text(errors='replace').splitlines(),1):
                if re.search(r'not[_ ]ported|unsupported|stub|TODO|FIXME',line,re.I):
                    candidates.append({'path':p.relative_to(ROOT).as_posix(),'line':n,'text':line.strip()[:1000]})
        shared.append({'module':module,'cpp_files_scanned':len(files),'candidate_sites':candidates,
                       'qualification':'Lexical scan of shared source only; no per-call reachability proof or engine twin-equivalence proof.'})

    # Same commit does not imply same working files. Compare preserved copies only.
    left=ROOT/'working-snapshots/charming-goldwasser-426a42'
    right=ROOT/'working-snapshots/funny-bun-e8c703'
    snapshot_diff=[]
    if left.exists() and right.exists():
        paths=set(p.relative_to(left) for p in left.rglob('*') if p.is_file())|set(p.relative_to(right) for p in right.rglob('*') if p.is_file())
        for rel in sorted(paths):
            a,b=left/rel,right/rel
            if a.exists() and b.exists():
                ah,bh=sha(a),sha(b)
                if ah==bh: continue
            else: ah=sha(a) if a.exists() else None; bh=sha(b) if b.exists() else None
            snapshot_diff.append({'path':rel.as_posix(),'left_sha256':ah,'right_sha256':bh})

    e=lambda p,n=None,a=None,q='retained static source; not an execution observation': evidence(p,n,a,q)
    findings=[
      finding('C01',1,'Restore original connector validation before linking parsed diagram items','confirmed port omission; preserved fix is partial',
       ['0x14008f530','0x140075050','0x140074f50','0x140161470','0x14015ef80'],
       [e(str(APP/'src/model/subproject.cpp'),330),e('working-snapshots/charming-maxwell-9333be/apps/moproj2mst/src/model/subproject.cpp',177),
        e('evidence/gui_mo/L5d/75050.c',182,'0x140075050'),e('evidence/gui_mo/L5d/74f50.c',17,'0x140074f50'),
        e('evidence/gui_mo/L5d/conn2.c',1,'0x140161470'),e('evidence/gui_mo/L5d/82760.c',264,'0x14015ef80'),
        e('evidence/gui_mo/s1/s1-01/dc/sub0.c',925,'0x14008f530'),e('evidence/gui_mo/s1/s1-01/dc/diag0.c',424,'0x140091f00')],
       ['Preserve baseline cellless-item placement, refreshDiagramItems and view-height changes. Resolve literal addresses against exact target.'],
       ['Type-only L5S helper omits existing-link condition. For parse only, each source output is attempted once after fresh item creation; an occupied target input must therefore reject. Guard types plus an unoccupied target input. Full reconnect endpoint-index branch is outside this slice.','Virtual slot bindings and constructor flags must remain source-qualified; no original runtime comparison in this audit.'],
       ['Real parse tests: equal 2D/3D, incompatible directions, 2D3D bridge, Convert2Dto3D asymmetric ports, missing class info, duplicate incoming/outgoing targets and overwrite order.','Original-first fresh mixed-dimension fixture; compare chain, MST omissions/order and rewritten Connectors. Existing p03/test_subproject.cpp and test_order.cpp are useful regression hosts.']),
      finding('C02',1,'Recover Save file ordering and each writer failure contract','confirmed fidelity difference; correction requires writer-specific recovery',
       ['0x1400980d0','0x1400983c0','0x1400982e0','0x1401313c0','0x1407617e0'],
       [e('analysis/save-semantics.md',None,None,'static decompilation plus byte-verified original disassembly; representative runtime failure scenarios pending'),e(str(APP/'src/core/convert.cpp'),62),e(str(APP/'src/core/convert.cpp'),238)],
       ['Trace caller status/messages, property writer error contracts, direct stream flushes, original open effects.'],
       ['Late deferred FileEffects and atomic replacement alter partial effects. mkdir/delete throw contracts may differ. Open effects are suppressed if later generation refuses.','Do not replace atomic helper globally before recovering per-stage continuation and status semantics.'],
       ['Disposable project before/after manifests for failed-open/failed-write at each stage. Compare later-stage continuation, retained/truncated files, messages, status and cleanup.']),
      finding('C03',1,'Finish geometry bounds and scene state used by active-operation Save','explicit refusals on relevant dependencies',
       ['0x1411f58e0','0x141233cd0','0x140c80280','0x140c7ff80'],
       [e(str(APP/'src/engine/viewer.cpp'),544),e(str(APP/'src/engine/viewer.cpp'),720),e('source/docs/re/moproj2mst/mdt/V/ledger.tsv')],
       ['Geometry representation item ownership and list ordering; shared engine geometry twins.'],
       ['Viewer-only labels do not exclude boxes/camera state read by writers. Packet ported labels can contain refusing branches.'],
       ['Nonempty 3D geometry-only/mesh-only/mixed inputs; independently compare rects, camera and saved properties as well as MST.']),
      finding('C04',1,'Trace close/destructor and shared process-global state for isolated repeated conversions','partially reconstructed ownership; lifecycle closure unproved',
       ['0x140089880','0x140c7a870','0x1408246a0','0x140cda460'],
       [e(str(APP/'src/core/convert.cpp'),147),e(str(APP/'src/engine/gui_model.cpp'),141),e(str(APP/'src/engine/db_load.cpp'),96),e('source/port/defmodel/src/step/db_info.cpp',42),e(str(APP/'src/ops/multiblow_props.cpp'),458),e(str(APP/'src/model/simcount.cpp'),30)],
       ['Build complete ownership graph and original Close action roots; enumerate global/TLS/Fortran units/header caches.'],
       ['Project RAII and mutex do not prove reset of engine globals; engine version cache and GUI engine variant persist.','Project member destruction order puts dataDb/display after some referencing objects; lifetime safety needs trace, not inference from unique_ptr.','Exception exits may leave TLS cycle naming or scratch files changed; no positive leak/reset test in reviewed baseline.'],
       ['Same DLL process: A,A; A,B,A; failure,A compared with independent fresh A sessions. Cross-project history is test stimulus only; every call must satisfy isolated one-project contract.','Manifest scratch, handle/file units and process resources after success/refusal/exception; verify required Close effects against original.']),
      finding('C05',1,'Rebase historical tests onto fresh single-project oracle qualification','historical validation cannot establish current scope',[],
       [e('analysis/baseline-checks.json',None,None,'11 inherited MST-only cases; 9 raw equal, 2 waiver-adjusted; no original runs'),e('analysis/baseline-audit.md'),e('TAKEOVER.md')],
       ['Relocate paths, qualify session histories, freeze candidate and held-out cases.'],
       ['Historical full Save 16/49 identical and resave 0/49 are version-qualified. Latest full Save denominator is not measured here.','Release samples test missing-input error; ABI test can accept unsupported.'],
       ['Successful w/utf8/C++/C#/Python ABI cases; MST-only input immutability; full Save side-file tree; report refusals/waivers separately.']),
      finding('C06',1,'Reconcile main and packet ledger into actual Open→Save→Close graph','inherited inventory is not complete reachability',[],
       [e(str(LEDGER)),e('evidence/gui_mo/s1/reach.py',54),e('evidence/gui_mo/s1/closure.py',1)],
       ['Add Save and Close roots, virtual callbacks, external library boundaries, shared twin functions and unresolved indirect edges.'],
       ['Historical reach roots are OpenProject/read/walk, writeMainMst and op vt+0x280 only; Save/Close roots absent.','callees scan searches E8/E9 bytes rather than decoding instruction boundaries; false call edges possible, indirect calls absent.','Ledger percentage cannot be a subsystem completion denominator.'],
       ['Per-function address→qualified evidence→reconstruction→state/side-effects→validation map; classify every reachable exclusion/library/spec row.']),
      finding('C07',2,'Restore missing operation property-save dispatch and MST-time model updates','static-source omission candidates; call-chain verification required',
       ['0x1411e09a0','0x1411e1820','0x1401c4440','0x1401ce590','0x140e24e80','0x140e2d230'],
       [e(str(APP/'src/core/convert.cpp'),208),e(str(APP/'src/ops/convert23.cpp'),133),e(str(APP/'src/ops/copymirror.cpp'),119),e(str(LEDGER))],
       ['Inspect original per-class vt+0xb8 dispatch plus original MST writer props vt+0x60 calls.'],
       ['Driver visits Cogging and L6/Express property saves; BooleanProperties save remains spec. Need prove which default-dialog cases reach it.','Convert/CopyMirror writeMst comments identify omitted propsToEngine calls even though newer load/save slots exist. Old comments can be stale; inspect original bodies before fixing.','CONV_STATUS.DAT in Convert2Dto3D and model temporaries explicitly omitted; full effect trace pending.'],
       ['Fresh Boolean, CopyMirror and Convert2Dto3D active/inactive cases with nondefault properties; compare side files, mutated models and final MST.']),
      finding('C08',2,'Account for DOE/optimisation branches in full headless scope','explicitly unsupported; no scope waiver accepted',
       ['0x14090a550','0x1400d2b60','0x14090ee20','0x140912a20','0x140912b00','0x140912d10','0x140913270'],
       [e(str(APP/'src/core/convert.cpp'),162),e(str(APP/'src/doe/doe_study.cpp'),162),e(str(APP/'src/model/subproject.cpp'))],
       ['Trace ordinary Save study output separately from adapter DOE_TEMP flag; defaults only.'],
       ['DOE_TEMP is refused at adapter; variables/constraints/nonempty studies refuse elsewhere. Which UI edits are beyond default action sequence needs branch evidence.'],
       ['Fresh ordinary Save cases with existing studies plus separate .MST.TEMP trigger qualification; no new scope waiver inferred.']),
      finding('C09',2,'Recover remaining class-specific Save/property files and defensive refusals','explicit refusal families; heterogeneous contracts',
       ['0x14095cd90','0x1401c5fa0','0x140a9e8f0','0x1403056a0','0x140d67080'],
       [e(str(APP/'src/engine/l5_slots.cpp'),920),e(str(APP/'src/ops/heattreat_htwz.cpp'),317),e(str(APP/'src/engine/viewer_camera.cpp'),190),e('source/docs/re/moproj2mst/mdt/L5/ledger.tsv',39)],
       ['Shared geometry and engine coverage; inspect each refusal condition in source.'],
       ['Includes Convert2Dto3D 3D mesh windows, HeatTreat region records, mirror planes, active class gates, null objects and malformed ranges. Not all are reachable under selected default answers.'],
       ['Partition successful supported branches from original crashes/refusals, default-dialog paths and excluded user actions; record denominators.']),
      finding('C10',2,'Recover Cogging multiple-reheat and failed-open behavior','explicit refusal plus intentional error-policy deviation',
       ['0x14076ac03','0x14076ac72','0x1407617e0'],
       [e(str(APP/'src/datafiles/mdt_write.cpp'),422),e(str(APP/'src/datafiles/mdt_write.cpp'),590),e('source/docs/re/moproj2mst/mdt/spec.md',1)],
       ['Validate disassembly loop indexing and ownership bounds; reproduce original failed-open continuation.'],
       ['NoReheats>=2 refusal is not equivalent to original array behavior. Do not call undefined outputs reproducible without capture evidence.','Code-page path refusal replaces original silent failed fopen.'],
       ['Disposable multi-reheat cases with full error/output retention; represent unstable original results explicitly.']),
      finding('C11',2,'Resolve default-dialog branches and strict-warning behavior','adapter/refusal policy distinct from recovered original',
       ['0x1400b8fb0'],
       [e(str(APP/'src/engine/save_project.cpp'),23),e(str(APP/'src/core/convert.cpp'),218),e(str(APP/'src/ops/multiblow.cpp'),114)],
       ['Recover actual QMessageBox default button from call arguments and UI code.'],
       ['Last-operation warning always refuses in port, whereas selected contract requires original default answer.','M2M_FLAG_STRICT can turn extra port warnings into failed conversion; retain ABI but disclose adapter policy and separate default faithful path.'],
       ['Invalid last-operation placement with default answer; original status/messages and surviving writes.']),
      finding('C12',2,'Preserve alternative working files without overwriting newer baseline','same-head divergent snapshots; not automatically integrated',[],
       [e('admin/worktrees.json'),e('admin/refs.json'),e('admin/import-receipt.json')],
       ['Diff each necessary hunk against original evidence and selected bb869387 baseline.'],
       ['charming-goldwasser and funny-bun both reference 9f128389 yet differ on disk. Snapshot hashes prove difference, not semantic superiority.','Preserved L5S full file has older placement/view code; blanket overlay loses later work.'],
       ['Selective hunk integration and focused regressions; preserve original snapshot hashes.']),
      finding('C13',3,'Account for silent XML/property/GUI omissions and inherited waivers','bounded discovery candidates; not all defects',
       ['0x14015ef80','0x140091f00','0x140099040'],
       [e('source/docs/re/moproj2mst/moproj/layout.md',46),e(str(APP/'src/xml/dom.cpp'),16),e(str(APP/'src/ops/express_tree.cpp'),74),e(str(APP/'include/m2m/model.hpp'),297)],
       ['Inspect source-qualified comments individually and classify effect/reachability.'],
       ['Cycle Expanded is uninitialized in original; historical yes policy is a declared deviation, not new-user authorization.','Recovery list filler untraced; XML parser supports narrower grammar; comments about omitted UI updates need writer-state trace.'],
       ['Preserve nondeterminism/known-deviation labels; saved DOM and property comparisons, not MST-only equality.']),
      finding('C14',2,'Prove reused DEF_PRE dependencies match GUI twins','shared engine closure not validated by converter-only tests',[],
       [e(str(APP/'CMakeLists.txt'),41),e(str(APP/'src/engine/gui_model.cpp'),128),e(str(APP/'src/engine/db_load.cpp'),1)],
       ['Map concrete GUI call sites to DEF_PRE twins and original layouts/initialization.'],
       ['Converter CMake sets DEF_PORT_SKIP_TESTS for shared targets; converter build success does not validate them.','GUI-specific sim-control initialization differs from DEF_PRE and is patched locally; other twin differences remain audit questions.'],
       ['Run relocated relevant engine tests plus operation-level original comparisons; track unresolved imported engine functions separately from GUI ledger.']),
    ]
    findings.sort(key=lambda x:(x['priority'],x['id']))
    refs=dict(json.loads((ROOT/'admin/refs.json').read_text()))
    identity_paths=[LEDGER,Path('evidence/original-binaries/DEF_GUI_MO_64.exe'),Path('evidence/gui_mo/s1/reach.py'),Path('evidence/gui_mo/s1/closure.py')]
    identity_paths += [Path(e['path']) for f in findings for e in f['evidence']]
    identities=[{'path':p.as_posix(),'sha256':sha(ROOT/p)} for p in sorted(set(identity_paths)) if (ROOT/p).is_file()]
    report={
      'schema_version':1,'generated_utc':datetime.now(timezone.utc).isoformat(),
      'scope':'One project in fresh-equivalent state: Open→Save→Close, original default dialog answers, internal/external fidelity, unchanged m2m C ABI adapter.',
      'baseline_commit':'bb869387a85a89193f3fdd78b69442a6ea567866',
      'qualification':'Read-only retained source/evidence audit. No original process, native analysis session, build or converter execution. This is an actionable discovery inventory, not complete closure or verified reconstruction.',
      'ledger':{'path':str(LEDGER),'rows':len(ledger),'statuses':counts,'unresolved_or_excluded_rows':[r for r in ledger if r['status']!='ported'],
                'duplicate_address_counts':{k:v for k,v in collections.Counter(r['address'] for r in ledger).items() if v>1},'packet_ledgers':subsidiary},
      'lifecycle_spine':[
        {'phase':'Open','original_addresses':['0x14001ca30','0x140089880','0x14006dba0','0x14008a9b0','0x14008afa0','0x14008b410','0x14008f530','0x1400a7250','0x14003a3c0'],'reconstruction':[str(APP/'src/core/convert.cpp'),str(APP/'src/model/project.cpp'),str(APP/'src/model/subproject.cpp'),str(APP/'src/engine/loader.cpp')],'closure':'partial; indirect slots, engine dependencies and default dialogs need branch-level closure'},
        {'phase':'Save','original_addresses':['0x1400980d0','0x1400b8fb0','0x140052ec0','0x1400983c0','0x1400982e0','0x1400a20d0','0x1401313c0'],'reconstruction':[str(APP/'src/engine/save_project.cpp'),str(APP/'src/engine/save_current.cpp'),str(APP/'src/core/convert.cpp')],'closure':'partial; deferred file effects and per-class writers/failure handling remain'},
        {'phase':'Close','original_addresses':[],'reconstruction':[str(APP/'src/engine/gui_model.cpp'),str(APP/'src/engine/db_load.cpp'),str(APP/'src/ops/multiblow_props.cpp')],'closure':'unresolved; original Close entry/call graph not established; RAII proves only source ownership actions'},
      ],
      'source_site_inventory':{'qualification':'Lexical candidates only: includes comments, definitions, unreachable/error paths and stale notes. Counts are neither unsupported scenarios nor defects. Searches cannot prove absence.','scanned_converter_files':len(source_files),'sites':sites},
      'shared_dependency_discovery':shared,
      'preserved_work':{'L5S_ref':refs.get('refs/heads/claude/m2m-mdt-L5S'),'same_head':refs.get('refs/heads/claude/m2m-mdt-L6T'),'same_head_left':'working-snapshots/charming-goldwasser-426a42','same_head_right':'working-snapshots/funny-bun-e8c703','different_files':snapshot_diff},
      'first_correction':'C01; recover parser connector admission (type check plus unoccupied target input), using L5S type logic selectively and preserving newer baseline placement/view code. General reconnect/edit API is outside this slice.',
      'backlog':findings,'source_identities':identities,
      'remaining_global_unknowns':['No exhaustive instruction-decoded interprocedural graph including virtual edges and Close roots exists in this audit.','Original target hash is linked to imported evidence by preservation provenance; Ghidra snapshots are not guaranteed transaction-consistent or independently reopened.','No fresh reference captures or new behavioral verification were performed here.','No complete shared-engine per-function map, error side-effect denominator or default-dialog branch matrix is yet established.']}
    out=ROOT/'analysis/closure-audit.json'
    out.write_text(json.dumps(report,indent=2,ensure_ascii=False)+'\n',encoding='utf-8')
    md=['# Converter closure audit','',
        'The selected baseline is not closed over the requested Open→Save→Close lifecycle. The first small correction is the parsed-diagram connector check (C01); it affects chain selection and therefore MST and saved Connectors without requiring a broad redesign.','',
        'This report uses retained static sources only. No original process, REA session, build or converter was run. Original addresses identify hypotheses tied to qualified decompilation; they are not execution observations. The JSON preserves source hashes, every inherited non-ported row, packet disagreements, discovery sites and divergent snapshot file hashes.','',
        f'The main ledger has {len(ledger)} rows: '+', '.join(f'{n} {s}' for s,n in counts.items())+'. These statuses are inherited author assertions, not audited end-to-end completion. The old reach.py seeds Open/read/walk, main MST and operation slot +0x280. Save and Close entry roots are absent; closure.py scans E8/E9 bytes without instruction decoding and omits indirect targets.','',
        '## First correction: connector validation','',
        'The current parser connects any valid target index. The retained L5S change at 1671c942a297f590ee63acb93a9fe87581543929 adds class-flag connector types and compatibility checks. Original 0x140075050 calls model slot +0x88 before clearing either connector ([75050.c](../evidence/gui_mo/L5d/75050.c)); 0x140074f50 calls input connector slot +0x70 ([74f50.c](../evidence/gui_mo/L5d/74f50.c)); the compatibility body is [conn2.c](../evidence/gui_mo/L5d/conn2.c). Resolve its literal addresses against the preserved original before treating string labels as established.','',
        'The L5S helper is partial: after type acceptance, 0x140074f50 accepts an unlinked input, but an already linked input requires an already linked output and matching endpoint-index tests. The parse loop in sub0.c lines 925–948 attempts each fresh item output once after all items are constructed (diag0.c lines 424–427). Earlier edges can occupy that item\'s input, but cannot occupy its output. Thus this parse slice needs the type predicate plus rejection of an already occupied target input; the general reconnect branch is outside the correction. Constructor flag branching is retained in 82760.c from line 264, including the asymmetric port path. Default dialog actions are irrelevant to this parser check. Retain current cellless placement, item refresh and database-dependent view height; replacing the older snapshot file would remove later recovery.','',
        'Use the real parser in p03/test_subproject.cpp for equal, incompatible, bridging and Convert2Dto3D asymmetric port types; exercise duplicate targets and link replacement order. Keep p03/test_order.cpp as chain-order regression. Freeze the candidate and obtain original-first mixed-dimension and duplicate-link fixtures before claiming parity. A successful correction closes this slice only.','',
        '## Reachable lifecycle and shared engine','',
        'convert.cpp constructs a Project/registry, parses and activates subproject 0, loads the active operation, saves current state, optionally saves the project, visits property-save methods, generates MST and finally applies deferred writes. The original static Save source instead performs direct stage writes and continues after some failures; see [save-semantics.md](save-semantics.md). Open-time effects are also deferred, so a later refusal can erase effects the original already made. Recover writer-specific order and status semantics before changing the generic write helper.','',
        'RAII restores the Project TLS pointer and current-directory scope; GuiModel owns its engine step and DbHandle owns a reader whose destructor resets allocated state. Those source observations do not establish original Close behavior or engine-global reset. The engine mutex serializes calls but cannot reset persistent version state, GUI variant flags, shared Fortran units/caches or exceptional TLS state. The original Close root remains unknown. Repeated-call tests are a way to prove isolation of each single-project call; accumulated cross-project original state is outside the acceptance boundary.','',
        'CMake links defrt, defcmd, def_pre_core and transitive model/key/database libraries while setting DEF_PORT_SKIP_TESTS for those dependencies. The GUI sim-control initializer contains explicit differences from its DEF_PRE twin. Shared function/twin closure therefore requires its own address/layout/initialization map and tests. Lexical shared-source sites in JSON are discovery candidates only.','',
        '## Prioritized backlog','',
        '| ID | Priority | Work | Qualification |','|---|---:|---|---|']
    for f in findings: md.append(f"| {f['id']} | {f['priority']} | {f['title']} | {f['classification']} |")
    md += ['', 'Each JSON backlog item includes original addresses, source/evidence links, dependencies, explicit unknowns and focused validation. Priority 1 items block broad fidelity claims; priority 2 items complete function families and contracts; priority 3 is bounded omission triage. No priority implies permission to redesign original behavior.', '',
           '## Preserved alternatives and validation limits','',
           f'The preserved charming-goldwasser-426a42 and funny-bun-e8c703 working copies both point to 9f1283895be488a1e31ebd47637c06b06ad80f6d but differ in {len(snapshot_diff)} files (exact paths and SHA-256 pairs in JSON). Their snapshots and the L5S connector change remain preserved; neither is an automatic overlay.', '',
           'The current baseline checked 11 historical MST-only cases: nine raw matches and two waiver-adjusted passes. It did not validate full Save, fresh original sessions, Close, nonzero successful ABI calls through every binding, or all shared engine tests. Historical 16/49 Save matches and 0/49 fresh-corpus resave matches belong to earlier binaries. Qualify reference session history, preserve candidate freezes, and record every refusal, mismatch, waiver and unavailable expected result.', '',
           'Silent-omission candidates include BooleanProperties save dispatch, Convert2Dto3D/CopyMirror MST-time property-to-engine calls, Convert status files, recovery-list filling, XML grammar, UI-derived state and the inherited group Expanded policy. These require original call/branch evidence; comments or missing strings alone do not establish a defect. Explicit refusals include geometry-list boxes, scene subobjects, perspective projection, DOE variables/constraints/studies, HeatTreat records and Cogging reheats. Their exact site lists are retained, but a site count is not a scenario denominator.', '',
           'No DMO build or deployment belongs to this converter audit. No source or preserved snapshot was edited.']
    (ROOT/'analysis/closure-audit.md').write_text('\n'.join(md)+'\n',encoding='utf-8')
    print(json.dumps({'report':str(out),'ledger_rows':len(ledger),'statuses':counts,'backlog_items':len(findings),'same_head_different_files':len(snapshot_diff),'lexical_site_counts':{k:len(v) for k,v in sites.items()},'shared_candidate_sites':sum(len(m['candidate_sites']) for m in shared)},indent=2))

if __name__=='__main__': main()
