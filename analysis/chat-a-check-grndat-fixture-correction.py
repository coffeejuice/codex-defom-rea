"""Check only the two requested fixture revisions; never run a native importer."""
from pathlib import Path
import hashlib
import json
import re

ROOT = Path(__file__).resolve().parents[1]
BEFORE = ROOT / 'build/fix-grndat-fixtures-1/before'
REVIEW = Path(r'C:\Users\alext\.codex\worktrees\chat-b\codex-deform-rea\analysis\chat-b-grndat-fixture-review.json')
REVIEW_HASH = '484c83ed81ba870fa0b67e75e1f15edef67e2138b7e69bfea660ee19872ad76f'
sha = lambda raw: hashlib.sha256(raw).hexdigest()
assert sha(REVIEW.read_bytes()) == REVIEW_HASH
old = json.loads((BEFORE / 'analysis/chat-a-grndat-fixture-plan.json').read_bytes())
new = json.loads((ROOT / 'analysis/chat-a-grndat-fixture-plan.json').read_bytes())
ready = json.loads((ROOT / 'analysis/chat-a-grndat-fixture-readiness.json').read_bytes())
cases = {c['id']: c for c in new['cases']}
old_cases = {c['id']: c for c in old['cases']}
files = {f['path']: f for f in new['files']}
assert cases.keys() == old_cases.keys()
assert new['contract'] == old['contract'] and new['source_refs'] == old['source_refs']
assert new['unsupported_or_uncovered'] == old['unsupported_or_uncovered']
assert new['execution'] == old['execution']
assert new['cohort_policy'] == old['cohort_policy']
assert sha((ROOT / new['contract']['path']).read_bytes()) == new['contract']['sha256']
for f in new['files']:
    raw = (ROOT / f['path']).read_bytes()
    assert sha(raw) == f['sha256'] and len(raw) == f['bytes']
    assert b'\r' not in raw
    raw.decode('ascii')
complete_stages = 0
for c in new['cases']:
    prior = old_cases[c['id']]
    assert c['cohort'] == prior['cohort'] and c['setup_imports'] == prior['setup_imports']
    assert c['test_groups'] == prior['test_groups']
    assert c['expected']['handler_status'] == prior['expected']['handler_status']
    assert c['expected']['postbranch_refresh0'] == prior['expected']['postbranch_refresh0']
    assert c['execution_status'] == 'NOT_RUN' and c['runtime_oracle'] is None
    raw = (ROOT / c['input']).read_bytes()
    assert len(raw) == c['stream']['file_length']
    for s in c['stream']['stages']:
        piece = raw[s['byte_start']:s['byte_end_exclusive']]
        assert piece.hex() == s['expected_input_bytes_hex']
        for token in s.get('tokens', []):
            assert raw[token['start']:token['end']].decode() == token['lexeme']
        if s['stage'].endswith('count_inline_scalar_pair') or s['stage'] == 'embedded.fields':
            pair = piece.decode().split()[-2:]
            assert pair == (['59','60.5'] if c['cohort']=='held_out_reserved' else ['23','24.5'])
            complete_stages += 1

def setup_pair(name):
    text = (ROOT / ('analysis/chat-a-grndat-fixtures/setup/' + name)).read_text()
    match = re.search(r'(?m)^GRNDAT 20 2 [^\n]*\n([^\n]+)', text)
    assert match
    return match.group(1).split()[-2:]

assert setup_pair('seed_all_branches.KEY') == ['7','8.5']
assert setup_pair('held_out_seed.KEY') == ['47','48.5']
for name in ['embedded_rank1.KEY','embedded_rank2.KEY']:
    assert setup_pair(name) == ['7','8.5']

expected_tuples = [(-1,-1,0,False),(0,0,0,False),(1,1,1,False),(12,12,12,False),(13,12,12,True)]
for n, stored, inline, diagnostic in expected_tuples:
    c = cases['record_count_' + str(n).replace('-','minus')]
    expected = c['expected']['embedded_count']
    assert tuple(expected[k] for k in ['declared','stored','inline_conversions','clamp_diagnostic_present']) == (n,stored,inline,diagnostic)
    stage = next(s for s in c['stream']['stages'] if s['stage']=='embedded.count_inline_scalar_pair')
    tokens = bytes.fromhex(stage['expected_input_bytes_hex']).decode().split()
    assert int(tokens[0]) == n and len(tokens)-3 == inline
    assert tokens[-2:] == ['23','24.5']
    pair = c['expected']['embedded_scalar_pair']
    assert pair['before'] == {'int_0x70':7,'double_0x68':8.5}
    assert pair['after'] == {'int_0x70':23,'double_0x68':24.5}
    assert pair['pair_scan_conversions'] == 2

partial = cases['embedded_pair_eof']
sp = next(s for s in partial['stream']['stages'] if s['stage']=='embedded.count_inline_scalar_prefix')
tokens = bytes.fromhex(sp['expected_input_bytes_hex']).decode().split()
assert tokens == ['2','8200.25','8201.25','19']
assert partial['stream']['stages'][-1]['reader_kind'] == 'EOF'
pair = partial['expected']['embedded_scalar_pair']
assert pair['before'] == {'int_0x70':7,'double_0x68':8.5}
assert pair['after'] == {'int_0x70':19,'double_0x68':8.5} and pair['pair_scan_conversions'] == 1
assert partial['expected']['handler_status'] == 2 and partial['expected']['postbranch_refresh0'] is False
held = cases['h_lists_replacement']['expected']['embedded_scalar_pair']
assert held['before'] == {'int_0x70':47,'double_0x68':48.5}
assert held['after'] == {'int_0x70':59,'double_0x68':60.5} and held['pair_scan_conversions'] == 2

discovery = [c for c in cases.values() if c['cohort']=='discovery']
reserved = [c for c in cases.values() if c['cohort']=='held_out_reserved']
assert (len(cases),len(discovery),len(reserved),len(files)) == (123,112,11,131)
assert not {files[c['input']]['sha256'] for c in discovery} & {files[c['input']]['sha256'] for c in reserved}
assert ready['plan_sha256'] == sha((ROOT/'analysis/chat-a-grndat-fixture-plan.json').read_bytes())
assert ready['generator_sha256'] == sha((ROOT/'analysis/chat-a-build-grndat-fixtures.py').read_bytes())
assert ready['original_runs'] == ready['candidate_runs'] == 0 and ready['native_session_opened'] is False
baseline = json.loads((BEFORE / 'manifest.json').read_bytes())
changes = []
for f in baseline:
    assert sha((BEFORE / f['path']).read_bytes()) == f['sha256']
    raw = (ROOT / f['path']).read_bytes()
    if sha(raw) != f['sha256']:
        changes.append({'path':f['path'],'before_sha256':f['sha256'],'after_sha256':sha(raw),'before_bytes':f['bytes'],'after_bytes':len(raw)})
assert sha(REVIEW.read_bytes()) == REVIEW_HASH
receipt = {
    'schema_version':1,'task_id':'fix-grndat-fixtures-1','status':'CORRECTED_PENDING_B_FOCUSED_RECHECK',
    'scope':['B-GRNDAT-01','B-GRNDAT-02'],'worktree':str(ROOT),'base_commit':new['base_commit'],
    'prior_review':{'path':str(REVIEW),'sha256':REVIEW_HASH,'unchanged':True},
    'prior_package_snapshot':{'path':str(BEFORE),'manifest_sha256':sha((BEFORE/'manifest.json').read_bytes()),'preserved_files':len(baseline)},
    'corrections':[
        {'finding':'B-GRNDAT-01','discovery_seed':[7,8.5],'discovery_complete_action':[23,24.5],
         'partial_action_integer':19,'partial_expected_pair':[19,8.5],'pair_scan_conversions':1,
         'following_failure':'nested table header EOF','handler_status':2,'list_clear':False,'refresh0':False,
         'reserved_seed':[47,48.5],'reserved_complete_action':[59,60.5],
         'expectation_cases':['embedded_pair_eof','record_count_minus1','record_count_0','record_count_1','record_count_12','record_count_13','h_lists_replacement'],
         'source_pointer':'/record_reader/steps/4','evidence_ids':pair['evidence_ids']},
        {'finding':'B-GRNDAT-02','tuple_fields':['declared','stored','inline_conversions','clamp_diagnostic_present'],
         'expected_tuples':expected_tuples,'source_pointer':'/record_reader/steps/2',
         'evidence_ids':cases['record_count_0']['expected']['embedded_count']['evidence_ids']}
    ],
    'contract':new['contract'],'changed_files':changes,'changed_file_count':len(changes),
    'unchanged_prior_package_files':len(baseline)-len(changes),
    'additional_artifact':{'path':str(Path(__file__).resolve()),'sha256':sha(Path(__file__).read_bytes())},
    'validation':{'result':'PASS_STATIC_ONLY','cases':123,'discovery':112,'held_out_reserved':11,'fixture_files':131,
        'complete_action_pair_stages_checked':complete_stages,
        'checks':['All package bytes/hashes and lexical spans/token offsets match','Exact five count tuples agree with actual input arity',
                  'Distinct seed/action scalar markers in actual bytes','Partial pair new integer and retained old double explicitly asserted',
                  'Complete reused pair replaces both fields in discovery and reserved cases','All prior handler/refresh expectations unchanged',
                  'Case IDs, cohorts, setup ordering, group assignments, unsupported domains and source bindings unchanged',
                  'All cases NOT_RUN, all runtime oracles null, discovery/reserved bytes disjoint','Prior package snapshot and B review receipt unchanged'],
        'not_validated':['Original behavior','Candidate behavior','Runtime stream positions','Whole-import status']},
    'execution':{'original_runs':0,'candidate_runs':0,'rea_sessions':0,'production_changes':False,'shared_tooling_changes':False,'merge_or_push':False},
    'remaining_limits':'B review nonblocking coverage gaps remain; no new cases or claims added. Reserved inputs still require a future freeze before evaluation.',
    'next_action':'B focused recheck of B-GRNDAT-01/02; no runtime or implementation authorization inferred.'
}
out=ROOT/'analysis/chat-a-grndat-fixture-correction.json'
out.write_text(json.dumps(receipt,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'receipt':str(out),'sha256':sha(out.read_bytes()),'changed_files':len(changes),'validation':receipt['validation']},indent=2))
