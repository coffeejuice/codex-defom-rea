"""Read-only, fail-closed continuation/recorded-completion check for this converter.

Default exit 0: valid queue (usually CONTINUE); 2: malformed records.
--require-complete exits 1 when the objective is incomplete. No file writes,
tool launches, original execution or independent parity claims occur here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
REQUIRED_GATES = (
    'reachable_scope_accounted', 'architecture_verified', 'lifecycle_integrated',
    'no_unexplained_discrepancies_or_refusals', 'fresh_independent_comparison',
    'failure_and_close_verified', 'abi_and_regression_verified', 'reproducible_delivery',
)


def contained_file(root, name):
    """Only accept evidence files in this converter tree, not sibling records."""
    if not isinstance(name, str) or not name:
        return False
    try:
        base = root.resolve(strict=True)
        candidate = (base / name).resolve(strict=True)
        return candidate.is_relative_to(base) and candidate.is_file()
    except (OSError, ValueError):
        return False


def file_sha256(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    return digest.hexdigest()


def candidate_binding(state, root):
    """Return the source hash and checkpoint pinned by the verified candidate receipt."""
    name = state.get('verified_candidate')
    if not contained_file(root, name):
        return None, None
    try:
        receipt = json.loads((root / name).read_text(encoding='utf-8'))
    except (OSError, ValueError, TypeError):
        return None, None
    source_hash = receipt.get('source_tree_sha256')
    validation = receipt.get('validation')
    match = re.search(r'(?:wave3-)?checkpoint[-_]?([0-9]+)',
                      Path(validation).name if isinstance(validation, str) else '')
    if (not isinstance(source_hash, str) or not re.fullmatch(r'[0-9a-fA-F]{64}', source_hash)
            or not match):
        return None, None
    return source_hash.lower(), int(match.group(1))


def validate_gate_evidence(gate_name, evidence, source_hash, checkpoint, root):
    """Validate typed, immutable evidence for a passed completion gate."""
    errors = []
    if source_hash is None or checkpoint is None:
        return [f'{gate_name}: verified candidate receipt lacks a source/checkpoint binding']
    if not isinstance(evidence, list) or not evidence:
        return [f'{gate_name}: passed gate requires typed evidence records']
    for index, record in enumerate(evidence):
        prefix = f'{gate_name}: evidence[{index}]'
        if not isinstance(record, dict):
            errors.append(f'{prefix} must be a typed evidence record')
            continue
        path = record.get('path')
        if not contained_file(root, path):
            errors.append(f'{prefix}: missing or outside converter evidence file {path!r}')
            continue
        resolved = (root / path).resolve()
        expected_hash = record.get('sha256')
        if not isinstance(expected_hash, str) or not re.fullmatch(r'[0-9a-fA-F]{64}', expected_hash):
            errors.append(f'{prefix}: invalid sha256')
        elif file_sha256(resolved) != expected_hash.lower():
            errors.append(f'{prefix}: sha256 mismatch for {path!r}')
        if record.get('checkpoint') != checkpoint:
            errors.append(f'{prefix}: checkpoint does not match verified candidate')
        if record.get('source_tree_sha256') != source_hash:
            errors.append(f'{prefix}: source_tree_sha256 does not match verified candidate')
        if record.get('claim') != gate_name:
            errors.append(f'{prefix}: claim must match gate {gate_name!r}')
        if gate_name == 'fresh_independent_comparison':
            try:
                payload = json.loads(resolved.read_text(encoding='utf-8'))
            except (OSError, ValueError, TypeError):
                errors.append(f'{prefix}: fresh comparison evidence must be a JSON receipt')
                continue
            if not isinstance(payload, dict):
                errors.append(f'{prefix}: fresh comparison evidence must be a JSON object')
                continue
            if payload.get('objective_complete') is False:
                errors.append(f'{prefix}: receipt says objective_complete is false')
            if payload.get('historical_only') is True or payload.get('fresh_capture_in_this_chat') is False:
                errors.append(f'{prefix}: receipt is historical-only or has no fresh capture')
            if payload.get('fresh_original_comparison') is not True:
                errors.append(f'{prefix}: receipt lacks fresh_original_comparison: true')
    return errors


def evaluate(state, root=ROOT):
    errors = []
    cursor = state.get('resume_cursor')
    if not isinstance(cursor, dict) or not isinstance(cursor.get('root'), str) or not cursor['root'].strip():
        errors.append('Missing resume_cursor object with executable root action')
    queue = state.get('recovery_queue')
    if not isinstance(queue, list) or not queue:
        return {'decision': 'INVALID', 'complete': False, 'errors': ['Missing nonempty recovery_queue']}
    ids = [x.get('id') for x in queue if isinstance(x, dict)]
    if len(ids) != len(queue) or any(not isinstance(x, str) or not x for x in ids) or len(set(ids)) != len(ids):
        return {'decision': 'INVALID', 'complete': False, 'errors': ['Invalid or duplicate item IDs']}
    by_id = dict(zip(ids, queue))
    for item in queue:
        label = item['id']
        for key in ('owner', 'next_action', 'implementation_status', 'integration_status', 'validation_status'):
            if not isinstance(item.get(key), str) or not item[key].strip():
                errors.append(f'{label}: missing {key}')
        if item.get('status') not in ('ready', 'active', 'blocked', 'verified'):
            errors.append(f'{label}: invalid status')
        if item.get('status') == 'blocked' and not item.get('blocker'):
            errors.append(f'{label}: blocked without concrete blocker')
        for key in ('evidence', 'required_validation', 'dependencies'):
            if not isinstance(item.get(key), list):
                errors.append(f'{label}: missing list {key}')
        if not item.get('required_validation'):
            errors.append(f'{label}: missing acceptance criteria')
        for dep in item.get('dependencies', []):
            if dep not in by_id:
                errors.append(f'{label}: unknown dependency {dep}')
        for name in item.get('evidence', []):
            if not contained_file(root, name):
                errors.append(f'{label}: missing evidence file {name!r}')
        if item.get('status') == 'verified':
            if not item.get('evidence') or not item.get('acceptance_evidence'):
                errors.append(f'{label}: verified without acceptance evidence')
            for name in item.get('acceptance_evidence', []):
                if not contained_file(root, name):
                    errors.append(f'{label}: missing acceptance evidence {name!r}')
            if any(item.get(k) != 'complete' for k in ('implementation_status', 'integration_status', 'validation_status')):
                errors.append(f'{label}: verified with incomplete implementation/integration/validation')
            if item.get('blocker'):
                errors.append(f'{label}: verified with blocker')
            if any(by_id.get(dep, {}).get('status') != 'verified' for dep in item.get('dependencies', [])):
                errors.append(f'{label}: verified before dependencies')
    visiting, visited = set(), set()
    def visit(key):
        if key in visiting:
            errors.append(f'{key}: dependency cycle')
            return
        if key in visited or key not in by_id:
            return
        visiting.add(key)
        for dep in by_id[key].get('dependencies', []):
            visit(dep)
        visiting.remove(key)
        visited.add(key)
    for key in ids:
        visit(key)
    unfinished = [x for x in queue if x.get('status') != 'verified']
    gates = state.get('completion_gates', {})
    pending_gates = [k for k in REQUIRED_GATES if not isinstance(gates.get(k), dict)
                     or gates[k].get('passed') is not True or not gates[k].get('evidence')]
    source_hash, checkpoint = candidate_binding(state, root)
    for key in REQUIRED_GATES:
        gate = gates.get(key, {})
        if isinstance(gate, dict) and gate.get('passed') is True:
            errors.extend(validate_gate_evidence(key, gate.get('evidence'), source_hash,
                                                 checkpoint, root))
    for key in ('checks_receipt', 'verified_candidate'):
        if key in state and not contained_file(root, state[key]):
            errors.append(f'{key}: missing or outside converter tree {state[key]!r}')
    if isinstance(cursor, dict) and 'last_verified_validation' in cursor:
        latest = cursor['last_verified_validation']
        if not contained_file(root, latest):
            errors.append(f'resume_cursor: missing or outside latest validation {latest!r}')
        checks = state.get('checks_receipt')
        if isinstance(latest, str) and isinstance(checks, str) and latest.replace('\\', '/') != checks.replace('\\', '/'):
            errors.append('resume_cursor: latest validation disagrees with checks_receipt')
    runnable = [x for x in unfinished if x.get('status') in ('ready', 'active') and
                all(by_id.get(d, {}).get('status') == 'verified' for d in x.get('dependencies', []))]
    complete = not errors and not unfinished and not pending_gates
    return {'decision': 'INVALID' if errors else 'COMPLETE' if complete else 'CONTINUE' if runnable else 'INVESTIGATE_BLOCKERS',
            'complete': complete, 'checkpoint_is_completion': False,
            'remaining_items': len(unfinished), 'pending_gates': pending_gates,
            'next_actions': [{'id':x['id'], 'owner':x['owner'], 'action':x['next_action']} for x in runnable[:4]],
            'blocked': [{'id':x['id'], 'blocker':x.get('blocker')} for x in unfinished if x.get('status') == 'blocked'],
            'errors': errors}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--state', type=Path, default=ROOT / 'analysis/takeover-state.json')
    parser.add_argument('--require-complete', action='store_true')
    args = parser.parse_args()
    try:
        state = json.loads(args.state.read_text(encoding='utf-8'))
        result = evaluate(state)
    except (OSError, ValueError, TypeError, AttributeError) as exc:
        result = {'decision':'INVALID', 'complete':False, 'errors':[str(exc)]}
    print(json.dumps(result))
    return 2 if result['decision'] == 'INVALID' else 1 if args.require_complete and not result['complete'] else 0


if __name__ == '__main__':
    raise SystemExit(main())
