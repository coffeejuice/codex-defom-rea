"""Focused A-INDEX-01 recheck; execute frozen copies only on own disposable data."""
import ast
import hashlib
import json
from pathlib import Path
import subprocess
import sys

sys.dont_write_bytecode = True
OWN = Path(__file__).resolve().parents[1]
MAIN = Path(r'C:\Users\alext\Projects\codex-deform-rea')
SCRATCH = OWN / 'build/chat-a-index-recheck'
SCRATCH.mkdir(parents=True, exist_ok=True)
sha = lambda raw: hashlib.sha256(raw).hexdigest()
freeze_path = MAIN / 'analysis/bottleneck-pilot-freeze-v1-review2.json'
freeze_raw = freeze_path.read_bytes()
freeze = json.loads(freeze_raw)
assert sha(freeze_raw) == '53ea36287ddc3497fc54fd4ab965d499f16e6ff53e2d9a893911d4065a780cde'
assert len(freeze['files']) == 10
assert sha(json.dumps(freeze['files'], sort_keys=True, separators=(',', ':')).encode()) == freeze['watermark_sha256'] == '294824b7963a2334c02ee88cc3524414e31aa0bcf13467e6a2f639c323c68f3f'

def verify_frozen():
    for entry in freeze['files']:
        raw = (MAIN / entry['path']).read_bytes()
        assert len(raw) == entry['bytes'] and sha(raw) == entry['sha256'], entry['path']
    assert freeze_path.read_bytes() == freeze_raw

verify_frozen()
prior_path = OWN / 'analysis/chat-a-index-review.json'
prior_raw = prior_path.read_bytes()
assert sha(prior_raw) == 'cc34eb2f2f42980fdd00151a0d596b28cfbdefb12cc4ef1c7898fa53ea168b61'
prior = json.loads(prior_raw)
prior_freeze = Path(prior['freeze']['path'])
assert sha(prior_freeze.read_bytes()) == prior['freeze']['manifest_sha256']
old_files = {f['path']: f for f in prior['freeze']['files']}
changed = [f['path'] for f in freeze['files'] if f['sha256'] != old_files[f['path']]['sha256']]
assert set(changed) == set(freeze['correction']['changed_rollout_files'])
for entry in freeze['files']:
    if entry['path'].endswith('.py'):
        dest = SCRATCH / entry['path']
        dest.parent.mkdir(parents=True, exist_ok=True)
        raw = (MAIN / entry['path']).read_bytes()
        dest.write_bytes(raw)
        assert sha(dest.read_bytes()) == entry['sha256']

# Prove the lookup/parser implementation itself was not changed by this repair.
def functions(path):
    return {n.name: ast.dump(n, include_attributes=False) for n in ast.parse(path.read_text()).body if isinstance(n, ast.FunctionDef)}

before_functions = functions(OWN / 'build/chat-a-index-review/tools/evidence_index.py')
after_functions = functions(SCRATCH / 'tools/evidence_index.py')
assert before_functions.keys() == after_functions.keys()
unchanged_functions = [name for name in before_functions if name != 'main']
assert all(before_functions[name] == after_functions[name] for name in unchanged_functions)

run = subprocess.run([sys.executable, '-B', '-m', 'unittest', 'discover', '-s', str(SCRATCH / 'tests'), '-p', 'test_evidence_*.py', '-v'], cwd=SCRATCH, capture_output=True)
(SCRATCH / 'tests.stdout.txt').write_bytes(run.stdout)
(SCRATCH / 'tests.stderr.txt').write_bytes(run.stderr)
log = run.stderr.decode()
assert run.returncode == 0 and 'Ran 17 tests' in log and '\nOK' in log
assert 'skipped' not in log.lower()

# Early benchmark rejection must occur before accessing its absent real sources.
# This is a rejection-path smoke check, not a timing benchmark.
existing_report = SCRATCH / 'synthetic-existing-report.json'
report_bytes = b'{"synthetic":"preserve these report bytes"}\n'
existing_report.write_bytes(report_bytes)
smoke = subprocess.run([sys.executable, '-B', str(SCRATCH / 'tools/benchmark_evidence_index.py'), '--output', str(existing_report)], cwd=SCRATCH, capture_output=True)
(SCRATCH / 'benchmark-rejection.stderr.txt').write_bytes(smoke.stderr)
assert smoke.returncode == 2 and smoke.stdout == b''
assert b'Benchmark output already exists' in smoke.stderr
assert existing_report.read_bytes() == report_bytes
assert not (SCRATCH / 'analysis/oracle/import-snapshot.json').exists()

historical = json.loads((MAIN / 'analysis/bottleneck-index-benchmark.json').read_bytes())
for name, digest in historical['code_sha256'].items():
    assert sha((OWN / 'build/chat-a-index-review' / name).read_bytes()) == digest
    assert sha((MAIN / name).read_bytes()) != digest

verify_frozen()
assert prior_path.read_bytes() == prior_raw
assert sha(prior_freeze.read_bytes()) == prior['freeze']['manifest_sha256']
result = {
    'schema_version':1,'task_id':'recheck-index-alias-1','reviewer':'Chat A','date':'2026-10-21',
    'verdict':'ACCEPT','verdict_scope':'A-INDEX-01 correction and focused regressions in the exact review2 freeze; accept for Coordinator integration decision. No runtime-compatibility or end-to-end performance validation implied.',
    'worktree':str(OWN),'base_commit':'d546058bde8c7c245485959dca8608b507be5ce4','reviewed_root':str(MAIN),
    'freeze':{'path':str(freeze_path),'sha256':sha(freeze_raw),'watermark_sha256':freeze['watermark_sha256'],'files':freeze['files'],'verified_before_and_after':True},
    'prior_review':{'path':str(prior_path),'sha256':sha(prior_raw),'unchanged':True},
    'original_freeze':{'path':str(prior_freeze),'sha256':sha(prior_freeze.read_bytes()),'unchanged':True},
    'changed_rollout_files':changed,
    'finding_disposition':{
        'id':'A-INDEX-01','status':'RESOLVED','source':'tools/evidence_index.py','lines':[124,138],
        'source_sha256':'d13c21d1052441dd8acac761ca7fd79f43dd40450622bf6b2e0eeb5532d1b242',
        'reason':'The builder rejects every existing final path using lexists, without resolving away final-component links. It then opens with xb, so a hard link or independent output created after the preflight causes exclusive creation to fail rather than truncate an existing inode. The failure path returns2 and emits no success receipt. Fresh output creation remains supported.',
        'policy':'Existing outputs, including unrelated caches, must be rejected. Rebuild to a fresh path.',
        'tests':[
            {'case':'Preexisting real hard-link alias','result':'PASS','observed':'Exit2, empty stdout, exact source and alias bytes preserved.'},
            {'case':'Source itself as output','result':'PASS','observed':'Exit2, empty stdout, exact source bytes preserved.'},
            {'case':'Existing independent cache','result':'PASS','observed':'Exit2, empty stdout, source and cache preserved.'},
            {'case':'Fresh independent output','result':'PASS','observed':'Exit0; output matches expected index bytes; receipt hash valid; query succeeds; source unchanged.'},
            {'case':'Hard link created after preflight','result':'PASS','observed':'Deterministic interleaving at build return; exclusive open fails, status2, no stdout receipt, source/alias bytes preserved.'},
            {'case':'Independent file created after preflight','result':'PASS','observed':'Deterministic interleaving at build return; exclusive open fails, status2, no stdout receipt, concurrent writer bytes preserved.'}
        ]
    },
    'regressions':{'existing_tests_passed':17,'skipped':0,'exit_code':run.returncode,
        'command':[sys.executable,'-B','-m','unittest','discover','-s',str(SCRATCH/'tests'),'-p','test_evidence_*.py','-v'],
        'method':'Byte-identical frozen tool/test copies under own scratch; test paths resolve to own synthetic-data tree. No real source JSON was passed to either tool.',
        'log':str(SCRATCH/'tests.stderr.txt'),'log_sha256':sha(run.stderr),
        'unchanged_index_function_ASTs':unchanged_functions,
        'coverage':'All prior six index and six slice tests remain; five added tests cover six output cases through two deterministic interleaving subtests.'},
    'commands_and_runner':{
        'assessment':'PASS for revised usage. Workflow allocates a GUID-suffixed index path, uses its returned path/digest for querying, retains target filtering and pins source digests for extraction. Benchmark defaults allocate separate UUID-suffixed index and report paths; report preflight rejects existing paths, and final x mode prevents truncating a path created meanwhile.',
        'existing_report_rejection_smoke':{'result':'PASS','exit_code':smoke.returncode,'stdout_bytes':len(smoke.stdout),'report_preserved':True,'real_sources_present':False,'stderr':str(SCRATCH/'benchmark-rejection.stderr.txt'),'stderr_sha256':sha(smoke.stderr)},
        'benchmark_rerun':False,
        'limits':'No full benchmark or race test for the benchmark report writer was run; its final exclusive mode was inspected. A late report collision may surface a FileExistsError traceback after work completes, but the existing report is preserved.'},
    'historical_timing_caveats':{
        'result':'PASS','report':'analysis/bottleneck-index-benchmark.json','sha256':'647805061be45bdb5ca3a8dd20b8249aecc21906c29e011d87f3341a94960f5a',
        'assessment':'The historical report and original freeze remain byte-for-byte unchanged. Its three recorded code hashes match the preserved pre-correction code, not this revision. Workflow explicitly labels timings pre-correction and says the benchmark was not rerun. Historical approximately8.3x fixed-file discovery and two-query amortization must not be represented as measurements of review2 or overall recovery work.',
        'retained_limits':['Likely warm cache','Fixed explicit three-source scope','Full source hashes per query','No agent-token/tool-roundtrip/end-to-end measurement','Known-record direct extraction remains an alternative']},
    'remaining_limits':['No protection claim against concurrent replacement of parent directories or changing source contents; output-file creation interleavings are the tested boundary.','An I/O failure after creating a fresh file may leave an incomplete output; no success receipt is emitted. A subsequent retry must choose a fresh path.','No symlink or dangling-link creation test was executed; lexists and exclusive-create handling of those names was inspected.','Prior review scope limitations for schema coverage, qualification selection and bounded misses remain.'],
    'new_blocking_findings':[],
    'validation_artifact':{'path':str(Path(__file__).resolve()),'sha256':sha(Path(__file__).read_bytes())},
    'scope_and_release':{'shared_files_mutated':False,'real_evidence_passed_to_helpers':False,'rea_sessions':0,'original_app_runs':0,'production_changes':False,'merge_or_push':False,'writes':'Own review artifact and disposable scratch only','resource_held':False},
    'next_action':'Coordinator may accept this exact revision for the separately authorized tooling integration workflow. Report delivered; stop.'
}
path=OWN/'analysis/chat-a-index-recheck.json'
path.write_text(json.dumps(result,indent=2)+'\n',encoding='utf-8')
print(json.dumps({'path':str(path),'sha256':sha(path.read_bytes()),'verdict':result['verdict'],'tests':17,'skipped':0,'changed_rollout_files':changed,'test_log':log[-100:]},indent=2))
