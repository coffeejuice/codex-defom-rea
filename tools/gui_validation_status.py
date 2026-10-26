"""Read-only GUI validation queue and acceptance checks; never acquires UI control."""
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
ACCEPTANCE = ('gui_invocation', 'candidate_identity', 'processing_success', 'reference_comparison', 'disk_reopen')


def validate(state):
    errors = []
    tasks = state['tasks']
    ids = [t['id'] for t in tasks]
    if len(ids) != len(set(ids)):
        errors.append('duplicate task ID')
    for t in tasks:
        if t['status'] not in ('ready', 'running', 'pending', 'blocked', 'complete', 'failed'):
            errors.append(f"{t['id']}: invalid status")
        for dep in t.get('depends_on', []):
            if dep not in ids:
                errors.append(f"{t['id']}: missing dependency {dep}")
        if t['status'] == 'blocked' and not t.get('blocker'):
            errors.append(f"{t['id']}: missing blocker")
        if t.get('kind') == 'backend_case' and t['status'] == 'complete':
            for key in ACCEPTANCE:
                evidence = t.get('acceptance', {}).get(key)
                if not isinstance(evidence, dict) or evidence.get('status') != 'pass' or not evidence.get('evidence'):
                    errors.append(f"{t['id']}: missing {key} pass evidence")
    if state['overall']['status'] == 'complete' and any(t['status'] != 'complete' for t in tasks):
        errors.append('overall complete with unfinished tasks')
    return errors


def snapshot(state):
    errors = validate(state)
    done = {t['id'] for t in state['tasks'] if t['status'] == 'complete'}
    release_ready = state['gui_control']['release_capability'] == 'supported_and_verified'
    available = [t for t in state['tasks'] if t['status'] in ('ready', 'pending', 'running')
                 and all(d in done for d in t.get('depends_on', []))
                 and (not t.get('requires_gui') or release_ready)]
    return dict(overall=state['overall']['status'], candidate_version=state['candidate']['version'],
        gui_control=state['gui_control']['status'], release_ready=release_ready,
        next_task=available[0]['id'] if available else None, available=[t['id'] for t in available],
        blocked=[dict(id=t['id'], reason=t['blocker']) for t in state['tasks'] if t['status'] == 'blocked'],
        accepted_backend_cases=sum(t.get('kind') == 'backend_case' and t['status'] == 'complete' for t in state['tasks']), errors=errors)


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--state', type=Path, default=ROOT/'analysis/def-pre-gui-validation-state.json')
    args = ap.parse_args()
    result = snapshot(json.loads(args.state.read_bytes()))
    print(json.dumps(result))
    return bool(result['errors'])


if __name__ == '__main__':
    raise SystemExit(main())
