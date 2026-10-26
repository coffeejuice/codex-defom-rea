"""Reconcile checkpoint42 and the completed native editor capture; review efficiency."""
import json
from datetime import datetime, timezone
from pathlib import Path

root = Path(__file__).resolve().parents[1]
def read(p): return json.loads((root/p).read_text(encoding='utf-8-sig'))
def write(p,v): (root/p).write_text(json.dumps(v,indent=2)+'\n',encoding='utf-8')
state = read('analysis/takeover-state.json')
assert state['efficiency']['completed_recovery_checkpoints'] == 42
now = datetime.now(timezone.utc)
previous = datetime.fromisoformat(state['efficiency']['last_review_utc'])
build = read('analysis/wave3-widget-task-ownership-v1-build.json')
validation = read('analysis/wave3-widget-task-ownership-validation.json')
estimate_path = 'analysis/progress-estimate-checkpoint42.json'
review_path = 'analysis/wave3-checkpoint42-efficiency-review.json'
assert not (root/estimate_path).exists() and not (root/review_path).exists()
estimate = dict(at_utc=now.isoformat(),scope=state['scope'],estimate_percent=70,
    uncertainty_range_percent=[60,80],
    method='Subjective engineering judgment, not measured branch coverage or remaining-time forecast; no complete weighted branch denominator exists.',
    verified=state['converter_ctest'],historical=state['historical_full_save'],
    remaining=['Public editor300 completion and actual base/event dependencies',
        'Active DOE factory initialization and lifecycle integration',
        'Reachable geometry/operation gaps', 'Failure and Close ordering',
        'Fresh independent Open/Save/Close acceptance and architecture audit'],
    qualifications=['146 passing test programs and 93/98 historical matches are baseline evidence, not objective completion.',
        'Probe003 establishes selected startup parent behavior; editor002 establishes native CLI Open to early setup only.',
        'No original full Open/Save/Close acceptance added by these captures; all eight objective gates remain pending.',
        'Unbuilt successor work is excluded. Small verified increments do not justify a percentage increase.'])
write(estimate_path,estimate)
review = dict(at_utc=now.isoformat(),trigger='One hour since previous review; safe boundary after owned capture cleanup',
    prior_review=state['efficiency']['last_review'],prior_checkpoint=state['efficiency']['last_review_checkpoint'],
    checkpoint=42,elapsed_clock_seconds=(now-previous).total_seconds(),
    builds=1,configure_build_seconds=sum(x['elapsed_seconds'] for x in build['steps']),
    latest_ctest_seconds=state['efficiency']['latest_ctest_seconds'],
    captures=[
        {'receipt':'analysis/wave3-original-splitter-probe-003-receipt.json','outcome':'selected startup parent capture completed'},
        {'receipt':'analysis/wave3-original-editor-probe-001-receipt.json','outcome':'probe phase guard stopped before Open; corrected'},
        {'receipt':'analysis/wave3-original-editor-probe-002-receipt.json','outcome':'native CLI Open reached selected editor setup endpoint'}],
    token_measurement=None,speedup_claim=None,
    interpretation='Elapsed clock includes research, tool latency and integration; it is not measured active coding time or agent speedup.',
    decisions=['Retain one root plus three workers with exclusive ownership and root-only builds/captures.',
        'Use source-confirmed native CLI Open for bounded editor investigations; qualify its timing separately from interactive Open.',
        'Bundle next font/device, widget counters, Create/postEvent and style observations; avoid one-field runtime probes.',
        'Use one actual WidgetRuntime-owned event queue across startup/editor lanes; no duplicate queue or event-count substitutes.',
        'Limit file inventory to relevant analysis paths; avoid broad scans of deliberately restricted test scratch.',
        'Continue implementation immediately; no acceptance weakening from passing checkpoint42.'])
write(review_path,review)
state['completion_estimate'].update(at_checkpoint=42,record=estimate_path)
state['efficiency'].update(last_review=review_path,last_review_checkpoint=42,last_review_utc=now.isoformat(),
    next_review='After checkpoint45 or one hour at next safe boundary',
    checkpoint_count_qualification='42 frozen checkpoints; count is not recovery percentage',
    latest_review_configure_build_seconds=review['configure_build_seconds'])
actions={
    'R300':'42 native setup prefix verified; native CLI editor capture002 completed and cleaned. Bind captured setup/property dispatch without replay or bypassing base constructor/posted-event dependencies; next parented grid has distinct ChildAdded/publication ordering.',
    'RDOE':'42 Task/Session allocation retention and weak observer lifetimes verified. Recover actual 17-prototype lazy factory prefix, first Report metadata/font/model children and global registrations in constructor order.',
    'RBOOL':'42 source-bound startup parent/value/queue-append prefix verified through WidgetFontDevicePending. Bundle remaining font/device/counter/Create/postEvent/style observations and represent one actual runtime-owned queue.',
    'RIO':'42 verified146 programs,139 clean/6 logged gaps/1 logged differences;93/98 historical unchanged. Both editor capture attempts cleaned; integrate next ready source contracts and preserve all eight acceptance gates.'}
for item in state['recovery_queue']:
    if item['id'] in actions: item['next_action']=actions[item['id']]
    if item['id'] in ['R300','RDOE','RBOOL','RIO']:
        receipt='analysis/wave3-widget-task-ownership-validation.json'
        if receipt not in item['evidence']: item['evidence'].append(receipt)
state['next']=actions['RIO']
state['resume_cursor'].update(root='42 frozen146pass;43 active. Editor002 native CLI setup capture succeeded/cleaned (56messages,no issues,files unchanged); editor001 phase-guard failure retained. Efficiency review42 complete. Integrate next actual event/constructor owner contracts; no GUI control or live owned process.',
    remesh_native=actions['R300'],reference_harness=actions['RDOE'],closure_next=actions['RBOOL'])
write('analysis/takeover-state.json',state)
print(json.dumps({'checkpoint':42,'estimate':70,'range':[60,80],'efficiency_review':review_path,
    'elapsed_clock_seconds':review['elapsed_clock_seconds'],'configure_build_seconds':review['configure_build_seconds'],
    'live_owned_process':False,'next_checkpoint':43}))
