"""Bind a passing candidate to immutable tests/contracts and a selected successor.

This records an implementation checkpoint, never objective completion.
"""
import argparse
import collections
import hashlib
import json
from pathlib import Path
import re
import shutil
import zipfile
from build_candidate import source_fingerprint

ROOT = Path(__file__).resolve().parents[1]


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def save(path, value):
    path.write_text(json.dumps(value, indent=2) + '\n', encoding='utf-8')


def archive_source(source, target, expected_fingerprint, expected_count):
    """Preserve exact source contents, and verify by reading the archive back."""
    with zipfile.ZipFile(target, 'x', compression=zipfile.ZIP_DEFLATED, compresslevel=6) as archive:
        for path in sorted(source.rglob('*')):
            if path.is_file():
                entry = zipfile.ZipInfo(path.relative_to(source).as_posix(), (1980, 1, 1, 0, 0, 0))
                entry.compress_type = zipfile.ZIP_DEFLATED
                entry.external_attr = 0o100644 << 16
                archive.writestr(entry, path.read_bytes())
    fingerprint = hashlib.sha256()
    with zipfile.ZipFile(target) as archive:
        entries = archive.infolist()
        for entry in entries:
            fingerprint.update(entry.filename.encode())
            fingerprint.update(b'\0')
            fingerprint.update(hashlib.sha256(archive.read(entry)).digest())
    assert (fingerprint.hexdigest(), len(entries)) == (expected_fingerprint, expected_count)
    assert source_fingerprint(source) == (expected_fingerprint, expected_count)
    return dict(path=str(target), sha256=sha(target), bytes=target.stat().st_size,
                source_tree_sha256=expected_fingerprint, files=expected_count,
                verification='Archive contents rehashed in source-fingerprint order; source remained stable')


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ['label', 'build-label', 'previous-tests', 'scope', 'next']:
        parser.add_argument('--' + name, required=True)
    parser.add_argument('--contract', action='append', default=[])
    parser.add_argument('--qualification', action='append', default=[])
    parser.add_argument('--focused', action='append', default=[])
    parser.add_argument('--allow-diagnostic-change', action='append', default=[],
                        help='Explicitly reviewed test whose logged diagnostic fields changed')
    args = parser.parse_args()
    state_path = ROOT / 'analysis/takeover-state.json'
    state = read(state_path)
    assert isinstance(state.get('resume_cursor'), dict), 'Resume cursor must be an object before freezing'
    for label in [args.label, args.build_label]:
        assert re.fullmatch(r'[a-z0-9_-]+', label)
    build_path = ROOT / f'analysis/{args.build_label}-build.json'
    build = read(build_path)
    tests_path = ROOT / 'analysis/test-relocation-results.json'
    tests = read(tests_path)
    assert build['source_stable_during_build'] and not build['targets']
    assert all(step['exit_code'] == 0 for step in build['steps'])
    fingerprint, count = source_fingerprint(ROOT / 'source')
    assert (fingerprint, count) == (build['source_tree_sha256'], build['source_file_count'])
    assert tests['exit_code'] == 0 and not tests['fixture_changes']
    assert not tests['unreported_tests'] and not tests['incomplete_output_tests']
    binding = tests.get('execution_binding')
    assert binding and binding.get('stable_during_test') is True, 'Test source/binary binding missing or unstable'
    assert binding['source_tree_before'] == {
        'sha256': build['source_tree_sha256'], 'files': build['source_file_count']
    }, 'Tests did not run against the selected build source'
    artifacts = {Path(a['path']).name: a for a in build['artifacts']}
    for name in ['moproj2mst.exe', 'moproj2mst.dll', 'deformdb.dll']:
        binary = ROOT / 'build/candidate/bin' / name
        assert sha(binary) == artifacts[name]['sha256']
        assert binding['binaries_before'][str(binary.resolve())]['sha256'] == artifacts[name]['sha256'], (
            'Tests did not run against the selected build artifact: ' + name)
    previous = {t['name']: t for t in read(ROOT / args.previous_tests)['tests']}
    compared = ['gap_events', 'diagnostic_differences', 'folder_sweep']
    changes = [t['name'] for t in tests['tests'] if t['name'] in previous and
               any(t[k] != previous[t['name']][k] for k in compared)]
    assert len(args.allow_diagnostic_change) == len(set(args.allow_diagnostic_change)), (
        'Duplicate diagnostic-change approval')
    assert set(changes) == set(args.allow_diagnostic_change), (
        'Review changed diagnostics before freezing: ' + str(changes))
    reviewed_changes = [dict(test=t['name'], before={k: previous[t['name']][k] for k in compared},
                             after={k: t[k] for k in compared})
                        for t in tests['tests'] if t['name'] in changes]
    folder = next(t for t in tests['tests'] if t['name'] == 'm2m.s.save_folder')
    focused = []
    for test in tests['tests']:
        if test['name'] in args.focused or test['name'].split('.')[-1] in args.focused:
            log = Path(test['log'])
            focused.append(dict(test=test['name'], log=str(log), sha256=sha(log), summary=[
                line for line in log.read_text(encoding='utf-8').splitlines()
                if re.search(r'\d+ of \d+ cases failed', line)]))
    retained_tests = ROOT / f'analysis/{args.build_label}-tests.json'
    validation_path = ROOT / f'analysis/{args.label}-validation.json'
    frozen = ROOT / f'build/{args.label}-verified'
    assert not any(p.exists() for p in [retained_tests, validation_path, frozen])
    contracts = [dict(path=p, sha256=sha(ROOT / p)) for p in args.contract]
    shutil.copy2(tests_path, retained_tests)
    validation = dict(scope=args.scope, objective_complete=False,
        build=str(build_path.relative_to(ROOT)), tests=str(retained_tests.relative_to(ROOT)),
        registered=tests['registered_tests'], counts=tests['counts'], fixture_changes=[],
        historical=dict(collections.Counter(c['result'] for c in folder['folder_sweep']['cases'])),
        historical_diffs=folder['diagnostic_differences'], changed_diagnostics=changes,
        reviewed_diagnostic_changes=reviewed_changes,
        focused=focused, contracts=contracts,
        qualifications=tests['qualifications'] + args.qualification, next_started=args.next)
    save(validation_path, validation)
    frozen.mkdir()
    source_archive = archive_source(ROOT / 'source', frozen / 'source.zip', fingerprint, count)
    copied = []
    for name in ['moproj2mst.exe', 'moproj2mst.dll', 'deformdb.dll']:
        target = frozen / name
        shutil.copy2(ROOT / 'build/candidate/bin' / name, target)
        copied.append(dict(path=str(target), sha256=sha(target), bytes=target.stat().st_size))
    receipt = frozen / 'receipt.json'
    save(receipt, dict(build=validation['build'], validation=str(validation_path.relative_to(ROOT)),
        source_tree_sha256=fingerprint, source_archive=source_archive,
        artifacts=copied, objective_complete=False))
    state.update(build_receipt=validation['build'], checks_receipt=str(validation_path.relative_to(ROOT)),
                 verified_candidate=str(receipt.relative_to(ROOT)))
    state['converter_ctest'].update(registered=tests['registered_tests'],
        passed=sum(v for k, v in tests['counts'].items() if k.startswith('passed')),
        failed=0, passed_without_logged_gaps=tests['counts']['passed'])
    state['efficiency']['completed_recovery_checkpoints'] += 1
    state['efficiency']['latest_ctest_seconds'] = tests['elapsed_seconds']
    state['resume_cursor'].update(root=args.next, last_verified_validation=str(validation_path.relative_to(ROOT)))
    save(state_path, state)
    print(json.dumps(dict(validation=str(validation_path), frozen=str(frozen),
        tests=tests['registered_tests'], historical=validation['historical'],
        focused={x['test']:x['summary'] for x in focused}, next=args.next)))


if __name__ == '__main__':
    main()
