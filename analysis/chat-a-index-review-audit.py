"""Independent frozen-rollout review; all writes are confined to own build tree."""
import hashlib
import importlib.util
import json
import math
import os
from pathlib import Path
import statistics
import subprocess
import sys

sys.dont_write_bytecode = True
MAIN = Path(r'C:\Users\alext\Projects\codex-deform-rea')
OWN = Path(__file__).resolve().parents[1]
OUT = OWN / 'build/chat-a-index-review'
OUT.mkdir(parents=True, exist_ok=True)
freeze_path = MAIN / 'analysis/bottleneck-pilot-freeze.json'
freeze_raw = freeze_path.read_bytes()
freeze = json.loads(freeze_raw)
sha = lambda b: hashlib.sha256(b).hexdigest()

def verify_freeze():
    assert len(freeze['files']) == 10
    for f in freeze['files']:
        raw = (MAIN / f['path']).read_bytes()
        assert len(raw) == f['bytes'] and sha(raw) == f['sha256'], f['path']
    watermark = sha(json.dumps(freeze['files'], sort_keys=True, separators=(',', ':')).encode())
    assert watermark == freeze['watermark_sha256'] == '4a79b16c13226599b3f9e1f038a42466826d358de69f3f81339c0027c0ef35dc'

verify_freeze()
for f in freeze['files']:
    if f['path'].endswith('.py'):
        dest = OUT / f['path']
        dest.parent.mkdir(parents=True, exist_ok=True)
        raw = (MAIN / f['path']).read_bytes()
        dest.write_bytes(raw)
        assert sha(dest.read_bytes()) == f['sha256']

run = subprocess.run([sys.executable, '-B', '-m', 'unittest', 'discover', '-s', str(OUT / 'tests'), '-p', 'test_evidence_*.py', '-v'], cwd=OUT, capture_output=True)
(OUT / 'existing-tests.stdout.txt').write_bytes(run.stdout)
(OUT / 'existing-tests.stderr.txt').write_bytes(run.stderr)
assert run.returncode == 0, run.stderr.decode()

def module(name):
    spec = importlib.util.spec_from_file_location(name, OUT / 'tools' / (name + '.py'))
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod

idx, sl = module('evidence_index'), module('evidence_slice')
report = {'freeze_manifest_sha256': sha(freeze_raw), 'watermark_sha256': freeze['watermark_sha256'], 'files': freeze['files'], 'existing_tests': {'exit_code': run.returncode, 'count': 12, 'log': str(OUT / 'existing-tests.stderr.txt')}, 'checks': []}

def check(name, **details):
    report['checks'].append({'name': name, 'result': 'PASS', **details})

bench = json.loads((MAIN / 'analysis/bottleneck-index-benchmark.json').read_bytes())
sources = [Path(s['path']) for s in bench['scope']]
rebuilt = idx.build(sources)
raw = idx.encode(rebuilt)
assert sha(raw) == bench['index_receipt']['index_sha256']
assert len(raw) == bench['index_receipt']['index_bytes']
assert rebuilt['sources'] == bench['scope']
(OUT / 'real-source-index.json').write_bytes(raw)

def pointer(doc, ptr):
    for part in ptr.split('/')[1:]:
        key = part.replace('~1', '/').replace('~0', '~')
        doc = doc[int(key)] if isinstance(doc, list) else doc[key]
    return doc

body_count = 0
for source in rebuilt['sources']:
    doc = json.loads(Path(source['path']).read_bytes())
    for entry in [e for e in rebuilt['entries'] if e['source_path'] == source['path']]:
        row = pointer(doc, entry['record_pointer'])
        assert row['evidence_id'] == entry['evidence_id']
        assert row['operation'] == entry['operation']
        assert hex(int(row['parameters']['procedure'], 16)) == entry['procedure']
        assert row['subject']['digest']['sha256'].lower() == entry['target_sha256']
        assert source['sha256'] == entry['source_sha256']
        for bp in entry['body_pointers']:
            assert isinstance(pointer(row, bp), str)
            body_count += 1
check('real_source_index_reproduction_and_independent_pointer_walk', records_seen=sum(s['records_seen'] for s in rebuilt['sources']), entries=len(rebuilt['entries']), skipped=sum(s['records_skipped'] for s in rebuilt['sources']), body_pointers=body_count, reproduced_sha256=sha(raw))
for q in bench['queries']:
    result = idx.query(OUT / 'real-source-index.json', sha(raw), q['procedure'], 'f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794')
    assert result['matches'] == q['matches'] and result['scope'] == bench['scope'] and result['status'] == q['status']
    for mode, samples in q['samples'].items():
        assert len(samples) == bench['repeats'] == 5
        assert all(s['exit_code'] == 0 and s['stderr_bytes'] == 0 for s in samples)
        assert statistics.median(s['seconds'] for s in samples) == q['median_seconds'][mode]
    direct, indexed = q['median_seconds']['direct'], q['median_seconds']['indexed']
    assert direct - indexed == q['saved_seconds_per_query']
    assert direct / indexed == q['direct_over_index_ratio']
    assert math.ceil(bench['setup']['seconds'] / (direct-indexed)) == q['queries_to_amortize_setup']
for path, digest in bench['code_sha256'].items():
    assert sha((MAIN / path).read_bytes()) == digest
check('recorded_benchmark_arithmetic_code_binding_and_query_fidelity', queries=3, samples=30, timing_rerun=False)
sb = json.loads((MAIN / 'analysis/bottleneck-pilot-benchmark.json').read_bytes())
for sample in sb['seconds'].values():
    assert len(sample['samples']) == 7 and statistics.median(sample['samples']) == sample['median']
assert sb['tool_sha256'] == sha((MAIN / 'tools/evidence_slice.py').read_bytes())
check('slice_benchmark_arithmetic_and_code_binding', timing_rerun=False)

record = {'evidence_id': 'synthetic-a', 'operation': 'procedure_pseudo_code', 'parameters': {'procedure': '0x0010'}, 'subject': {'digest': {'sha256': 'a'*64}}, 'normalized_result': ''.join(('keep' if i % 3 else 'drop') + str(i) + ('\r\n' if i % 2 else '\n') for i in range(403))}
record2 = json.loads(json.dumps(record))
record2['evidence_id'] = 'synthetic-b'
record2['subject']['digest']['sha256'] = 'b'*64
source = OUT / 'synthetic-evidence.json'
source.write_text(json.dumps({'records': [record, record2, None, {}, {'parameters': []}]}), encoding='utf-8')
original = source.read_bytes()
index = idx.build([source])
ip = OUT / 'synthetic-index.json'
ip.write_bytes(idx.encode(index))
for digest, expected in [('a'*64, 'synthetic-a'), ('b'*64, 'synthetic-b')]:
    q = idx.query(ip, sha(ip.read_bytes()), '0X00010', digest.upper())
    assert [x['evidence_id'] for x in q['matches']] == [expected]
    assert q['scope'][0]['records_skipped'] == 3
check('same_address_different_target_collision_and_malformed_record_skip_count', skipped=3)

for contains in [None, 'keep', 'not-present']:
    expected = [{'index': i, 'value': v} for i, v in enumerate(record['normalized_result'].splitlines(keepends=True)) if contains is None or contains in v]
    for count in [1, 40, 200]:
        start, actual, pages = 0, [], 0
        while True:
            page = sl.extract(source, '/normalized_result', start=start, count=count, contains=contains, expected_sha256=sha(original), record_pointer='/records/0')['selection']
            actual.extend(page['items']); pages += 1
            assert page['matching_items'] == len(expected)
            assert page['omitted_items'] == 403 - len(page['items'])
            if page['next_start'] is None:
                break
            assert page['next_start'] > start
            start = page['next_start']
        assert actual == expected
assert source.read_bytes() == original
check('complete_filtered_and_unfiltered_pagination', source_lines=403, page_sizes=[1,40,200], filters=[None,'keep','not-present'], cases=9)

# Reproduce alias handling only on disposable synthetic evidence, never real evidence.
alias = OUT / 'synthetic-output-hardlink.json'
if alias.exists():
    alias.unlink()
os.link(source, alias)
assert source.samefile(alias) and source.resolve() != alias.resolve()
run = subprocess.run([sys.executable, '-B', str(OUT / 'tools/evidence_index.py'), 'build', str(source), '--output', str(alias)], capture_output=True)
(OUT / 'hardlink.stdout.json').write_bytes(run.stdout)
(OUT / 'hardlink.stderr.txt').write_bytes(run.stderr)
report['hardlink_reproduction'] = {'exit_code': run.returncode, 'samefile_before': True, 'lexically_distinct_resolved_paths': True, 'source_sha256_before': sha(original), 'source_sha256_after': sha(source.read_bytes()), 'source_changed': source.read_bytes() != original, 'source': str(source), 'output_alias': str(alias), 'stdout_path': str(OUT / 'hardlink.stdout.json')}
assert run.returncode == 0 and source.read_bytes() != original

verify_freeze()
assert freeze_path.read_bytes() == freeze_raw
for source in bench['scope']:
    assert sha(Path(source['path']).read_bytes()) == source['sha256']
report['frozen_files_and_real_evidence_unchanged_after'] = True
(OUT / 'audit-results.json').write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
print(json.dumps({'audit_results': str(OUT / 'audit-results.json'), 'existing_tests': report['existing_tests'], 'checks': report['checks'], 'hardlink_reproduction': report['hardlink_reproduction'], 'unchanged_after': True}, indent=2))
