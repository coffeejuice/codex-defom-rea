"""Prepare source-bound original console batches without splitting a case."""
import hashlib
import json
from pathlib import Path
import shutil


def main():
    root = Path(__file__).resolve().parents[1]
    plan_path = root / 'analysis/chat-a-grndat-fixture-plan.json'
    plan = json.loads(plan_path.read_bytes())
    assert hashlib.sha256(plan_path.read_bytes()).hexdigest() == 'e8e1219f4d5576d6b5da8f3ae10ef4426c0e970bca375e3c400bc9a130e869fb'
    for f in plan['files']:
        assert hashlib.sha256((root / f['path']).read_bytes()).hexdigest() == f['sha256']
    output = root / 'build/grndat-native-batches'
    output.mkdir(exist_ok=False)
    batches = []
    for index, case in enumerate(plan['cases']):
        required = 2 + len(case['setup_imports']) + 1
        if not batches or len(batches[-1]['sequence']) + required > 32:
            batch_id = f'grndat-batch-{len(batches):02}'
            directory = output / batch_id
            directory.mkdir()
            for dim in [2, 3]:
                (directory / f'reset{dim}.KEY').write_bytes(f'* DEFORM-{dim}D\n'.encode())
            batches.append({'id': batch_id, 'fixtures': str(directory), 'sequence': []})
        batch = batches[-1]
        for dim in [2, 3]:
            batch['sequence'].append({'file': f'reset{dim}.KEY', 'role': 'reset', 'case': case['id']})
        for j, source in enumerate(case['setup_imports'] + [case['input']]):
            name = f'c{index:03}s{j}.KEY'
            shutil.copyfile(root / source, Path(batch['fixtures']) / name)
            batch['sequence'].append({'file': name, 'role': 'action' if source == case['input'] else 'setup',
                                      'case': case['id'], 'cohort': case['cohort'], 'original': source})
    for b in batches:
        b['ordered_imports'] = [x['file'] for x in b['sequence']]
        b['inputs'] = [{'file': p.name, 'sha256': hashlib.sha256(p.read_bytes()).hexdigest()}
                       for p in sorted(Path(b['fixtures']).iterdir())]
    extra_path = root / 'analysis/grndat-evaluation-inputs.json'
    if extra_path.exists():
        extra = json.loads(extra_path.read_bytes())
        extra['fixtures'] = str(root / 'tests/fixtures/grndat/new_runtime')
        for f in extra['inputs']:
            assert hashlib.sha256((Path(extra['fixtures']) / f['file']).read_bytes()).hexdigest() == f['sha256']
        batches.append(extra)
    record = {'schema_version': 1, 'cases': len(plan['cases']), 'batches': batches,
              'qualification': 'Original prepared reserved cases already ran against replacement. They are not untouched runtime held-out evidence.'}
    (root / 'analysis/grndat-native-batch-plan.json').write_text(json.dumps(record, indent=2) + '\n')
    print(json.dumps({'batches': len(batches), 'cases': record['cases'],
                      'imports': sum(len(b['sequence']) for b in batches)}))


if __name__ == '__main__': main()
