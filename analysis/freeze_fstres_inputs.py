"""Freeze audited FSTRES cohorts before native evaluation; never overwrite."""
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[1]


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def read(path):
    return json.loads(path.read_text(encoding='utf-8'))


def main():
    candidate_path = ROOT / 'analysis/fstres-candidate-freeze.json'
    candidate = read(candidate_path)
    current = []
    for entry in candidate['files']:
        assert digest(ROOT / entry['path']) == entry['sha256'], entry['path']
        assert digest(ROOT / entry['archive']) == entry['sha256'], entry['archive']
        current.append({'path': entry['path'], 'matches_freeze': True})
    batches = ['training-a', 'training-b', 'held-out-a', 'held-out-b']
    prepared = []
    for batch in batches:
        manifest_path = ROOT / f'analysis/fstres-{batch}-cases.json'
        manifest = read(manifest_path)
        assert 'draft' not in manifest['status'] and 'pending' not in manifest['status'], manifest['status']
        output = ROOT / f'analysis/fstres-{batch}-input-freeze.json'
        archive_root = ROOT / f'analysis/oracle/frozen-fstres-{batch}-inputs'
        assert not output.exists() and not archive_root.exists(), batch
        for entry in manifest['cases'] + [manifest['probe']] + manifest['source_bindings']:
            assert digest(ROOT / entry['path']) == entry['sha256'], entry['path']
        prepared.append((batch, manifest_path, manifest, output, archive_root))
    for batch, manifest_path, manifest, output, archive_root in prepared:
        def archive(relative):
            dest = archive_root / relative
            dest.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, dest)
            return dest.relative_to(ROOT).as_posix()
        inputs = []
        for entry in manifest['cases'] + [manifest['probe']]:
            inputs.append({'path': entry['path'], 'sha256': entry['sha256'],
                           'archive': archive(entry['path'])})
        relative_manifest = manifest_path.relative_to(ROOT).as_posix()
        document = {
            'created_utc': datetime.now(timezone.utc).isoformat(),
            'candidate_manifest': {'path': candidate_path.relative_to(ROOT).as_posix(),
                                   'sha256': digest(candidate_path)},
            'fixture_manifest': {'path': relative_manifest, 'sha256': digest(manifest_path),
                                 'archive': archive(relative_manifest)},
            'candidate_current_all_match': True, 'candidate_current': current,
            'inputs': inputs, 'native_source_bindings': manifest['source_bindings'],
        }
        output.write_text(json.dumps(document, indent=2) + '\n', encoding='utf-8')
        print(json.dumps({'batch': batch, 'fixtures': len(manifest['cases']),
                          'freeze': output.relative_to(ROOT).as_posix(), 'sha256': digest(output)}))


if __name__ == '__main__':
    main()
