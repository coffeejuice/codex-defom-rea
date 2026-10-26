"""Offline Boolean selector/coverage preflight for retained Editor004 evidence.

Reads pinned local files and emits JSON to stdout. Never executes the original,
uses a debugger/GUI/REA, writes files, or admits a native timing witness. Exit 0
means a valid preflight, not timing acceptance; --require-witness exits 1 while
the witness is unobserved. Malformed, changed or unqualified inputs exit 2.
--self-test exercises rejection/coverage rules in memory only.
"""
import argparse
import copy
import hashlib
import json
from pathlib import Path
import sys


ROOT = Path(__file__).resolve().parents[1]
CAPTURE = 'build/original-editor-probe-004/capture-evidence.json'
RECEIPT = 'analysis/wave3-original-editor-probe-004-receipt.json'
PROBE = 'build/original-editor-probe-004/probe.js'
PINNED = {
    CAPTURE: 'e4d0a277c4bbb074fa1e038ce3870e64ebb87ec884665cb1bb0ddd0221f99ced',
    RECEIPT: '63f392d69d9def0699f963a3722b9f4e0b257c0192ad53f4097a3ea2f5250439',
    PROBE: '0251ee49c0378af50e0778c8a584a5243402b10f26ab64fc315466bf10c68203',
}
MODULES = {
    'DEF_GUI_MO_64.exe': 'fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc',
    'QtCore4_64.dll': 'bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160',
    'QtGui4_64.dll': 'c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887',
}
FIXTURE = r'C:\PROBLEM\m2m42\DB_deconstruction_02.moproj'
FORMING_HOOK = 'def_gui_mo_64.exe:eb26f0'
BOOLEAN_HOOK = 'def_gui_mo_64.exe:11e2480'


def extract_same_owner_frames(payloads):
    """Check explicit observer frame summaries, never derive them from labels.

    A boolean_witness_frame packet summarizes a matched enter/normal-leave pair
    using capture-wide ordinals. Open is a selected interval ending at the
    specified branch/handler completion, not a fabricated CLI Open function RET.
    selected_parent_id names the nearest *selected*
    ancestor, not an assertion about every native stack frame. The small seam
    represents one Open -> UpdateData -> Apply -> add chain on one thread only.
    Reentrancy/multiple candidates are deliberately unrepresented. Structural
    success is unadmitted: this function cannot authenticate a future capture.
    Lifetime generation is an observer identity, not an invented native field.
    Existing Editor004 has no such packets and supplies no Boolean occurrence.
    """
    roles = ('open', 'update_data', 'apply', 'add')
    owners_required = {
        'open': ('window', 'project'),
        'update_data': ('window', 'project', 'area', 'impl', 'editor'),
        'apply': ('project', 'editor', 'opi'),
        'add': ('project', 'opi'),
    }
    packets = [p for p in payloads if isinstance(p, dict)
               and p.get('kind') == 'boolean_witness_frame']
    result = {
        'status': 'unobserved', 'frame_packets': len(packets),
        'structurally_complete': False, 'witness_admitted': False,
        'missing_roles': list(roles), 'errors': [], 'postload_origin_packets_unadmitted': 0,
        'claim_scope': 'one positive same-owner chain only; no first/absence/timer-origin claim',
        'missing_capture': 'selected Open interval completion plus matched current-UpdateData/'
                           'Boolean-Apply/add normal returns, with same-process/thread intervals '
                           'and actual owner lifetime links',
    }
    if not packets:
        return result
    errors = result['errors']
    frames = {}
    frame_ids = set()
    ordinals = set()
    for packet in packets:
        frame = packet.get('frame')
        if not isinstance(frame, dict):
            errors.append('frame packet requires a frame object')
            continue
        role = frame.get('role')
        # postLoad normal return is a separate creation origin, never Apply.
        if role == 'postload':
            if (type(frame.get('caller_return_rva')) is int
                    and frame['caller_return_rva'] == 0x11df0ee):
                result['postload_origin_packets_unadmitted'] += 1
            else:
                errors.append('postLoad packet has an unqualified return site')
            continue
        if not isinstance(role, str) or role not in roles:
            errors.append('unknown selected frame role')
            continue
        if role in frames:
            errors.append(f'{role}: multiple/reentrant selected frames are unrepresented')
            continue
        frames[role] = frame
        identity = frame.get('frame_id')
        if not isinstance(identity, str) or not identity or identity in frame_ids:
            errors.append(f'{role}: frame id missing or reused')
        else:
            frame_ids.add(identity)
        enter, leave = frame.get('enter_ordinal'), frame.get('leave_ordinal')
        if (type(enter) is not int or type(leave) is not int
                or enter <= 0 or leave <= enter or enter in ordinals or leave in ordinals):
            errors.append(f'{role}: missing, duplicated or unordered enter/leave ordinal')
        else:
            ordinals.update((enter, leave))
        completion_key = 'normal_completion' if role == 'open' else 'normal_return'
        if frame.get(completion_key) is not True or frame.get('read_errors') != []:
            errors.append(f'{role}: unmatched/abnormal return or unknown/failed reads')
        if (type(frame.get('pid')) is not int or frame['pid'] <= 0
                or type(frame.get('tid')) is not int or frame['tid'] <= 0
                or frame.get('module_sha256') != MODULES['DEF_GUI_MO_64.exe']):
            errors.append(f'{role}: process/thread/module identity is unqualified')
        owners = frame.get('owners')
        if not isinstance(owners, dict):
            errors.append(f'{role}: owner map missing')
            continue
        for name in owners_required[role]:
            owner = owners.get(name)
            if not isinstance(owner, dict):
                errors.append(f'{role}: missing owner {name}')
                continue
            address, generation = owner.get('address'), owner.get('generation')
            try:
                address_ok = (isinstance(address, str) and address.startswith('0x')
                              and int(address, 16) > 0)
            except ValueError:
                address_ok = False
            if not address_ok or type(generation) is not int or generation <= 0:
                errors.append(f'{role}: {name} needs actual nonnull address and lifetime generation')
    result['missing_roles'] = [role for role in roles if role not in frames]
    if errors or result['missing_roles']:
        result['status'] = 'rejected' if errors else 'incomplete'
        return result
    opened, updated, applied, added = [frames[role] for role in roles]
    boundary = opened.get('boundary_kind')
    boundary_sites = {
        'CliStartupBranchCompleted': (0x1ca30, 0x1e4e),
        'GuiHandlerReturned': (0x271b0, 0x28514),
    }
    if not isinstance(boundary, str) or boundary not in boundary_sites:
        errors.append('loader/postOpen return is not an enclosing whole-Open completion')
    elif (opened.get('source_rva'), opened.get('completion_rva')) != boundary_sites[boundary]:
        errors.append('Open entry/completion does not match selected CLI/GUI boundary')
    for role, expected in [('update_data', 0xdae340), ('apply', 0x11e2870), ('add', 0x57ba0)]:
        if type(frames[role].get('source_rva')) is not int or frames[role]['source_rva'] != expected:
            errors.append(f'{role}: native source selector mismatch')
    if added.get('caller_return_rva') not in (0x11e295a, 0x11e2bcb):
        errors.append('add: return is not an Apply Workpiece/Cutter origin')
    if added.get('rcx_role') != 'OPI':
        errors.append('add: RCX must be the observed OPI, not GuiModel')
    returned = added.get('returned_object')
    if (not isinstance(returned, dict) or not isinstance(returned.get('address'), str)
            or type(returned.get('number')) is not int):
        errors.append('add: actual returned object address/number missing')
    else:
        try:
            if not returned['address'].startswith('0x') or int(returned['address'], 16) <= 0:
                errors.append('add: actual returned object is null/unqualified')
        except ValueError:
            errors.append('add: invalid returned object address')
    if (updated.get('owner_links_observed') is not True
            or applied.get('opi_guard_resolved') is not True):
        errors.append('actual window/area/impl/current-editor or E+38 OPI link is missing')
    for parent, child in zip((opened, updated, applied), (updated, applied, added)):
        if child.get('selected_parent_id') != parent['frame_id']:
            errors.append('selected ancestor frame id mismatch')
        if not (parent['enter_ordinal'] < child['enter_ordinal']
                < child['leave_ordinal'] < parent['leave_ordinal']):
            errors.append('selected frame intervals are not strictly nested')
        if (child['pid'], child['tid'], child['module_sha256']) != (
                parent['pid'], parent['tid'], parent['module_sha256']):
            errors.append('selected chain changes process/thread/module')
    for left, right, names in [
        (opened, updated, ('window', 'project')),
        (updated, applied, ('project', 'editor')),
        (applied, added, ('project', 'opi')),
    ]:
        for name in names:
            if left['owners'][name] != right['owners'][name]:
                errors.append(f'same-owner {name} address/lifetime mismatch')
    if errors:
        result['status'] = 'rejected'
        return result
    result.update(status='structurally_complete_unadmitted', structurally_complete=True,
                  missing_capture='future capture authority/coverage is outside this pinned preflight',
                  origin='applyWorkpiece' if added['caller_return_rva'] == 0x11e295a else 'applyCutter')
    return result


def selector_manifest():
    """Requirements for future instrumentation; not an implemented trace binder."""
    return {
        'schema': 'm2m.boolean-open-selector-preflight.v1',
        'scope': 'retained Editor004 only; no future trace admission',
        'image_base': '0x140000000',
        'expected_module_sha256': MODULES,
        'selected_profile': 'native CLI startup, current user settings',
        'selected_fixture': FIXTURE,
        'boundaries': [
            {'kind': 'CliLoaderReturned', 'rva': '0x1c94',
             'is_whole_open_completion': False},
            {'kind': 'CliPostOpenReturned', 'rva': '0x1cae',
             'is_whole_open_completion': False},
            {'kind': 'CliStartupBranchCompleted', 'rva': '0x1e4e',
             'is_whole_open_completion': True},
            {'kind': 'GuiHandlerReturned', 'entry_rva': '0x271b0',
             'rva': '0x28514', 'separate_from_cli_profile': True},
        ],
        'callback_selectors': [
            {'role': 'diagram modelUpdated', 'rva': '0x183160',
             'owner': 'diagram receiver D; attachment is not OPI/GuiModel'},
            {'role': 'actual registry invocation', 'rva': '0x2cf6c6',
             'requires': 'emission/list-entry/receiver-lifetime identity; not registration alone'},
            {'role': 'current widget UpdateData', 'rva': '0xdae340',
             'requires': 'actual impl current guard -> Boolean editor E'},
            {'role': 'Boolean Apply', 'rva': '0x11e2870',
             'requires': 'entry/normal return, frame/thread identity, editor lifetime'},
            {'role': 'editor guarded OPI resolver', 'rva': '0x130af0',
             'requires': 'E+38 resolution result OPI O, not GuiModel'},
        ],
        'creation_origins': [
            {'kind': 'postLoadZeroOpiRecords', 'call_rva': '0x11df0e9',
             'normal_return_rva': '0x11df0ee', 'primitive_rva': '0x57ba0',
             'rcx_role': 'OPI', 'proves_apply': False,
             'predicate': 'OPI slot-record count == 0; not engine object count'},
            {'kind': 'applyWorkpiece', 'call_rva': '0x11e2955',
             'normal_return_rva': '0x11e295a', 'primitive_rva': '0x57ba0',
             'rcx_role': 'OPI', 'requires': 'same nested Apply frame and returned object/number'},
            {'kind': 'applyCutter', 'call_rva': '0x11e2bc6',
             'normal_return_rva': '0x11e2bcb', 'primitive_rva': '0x57ba0',
             'rcx_role': 'OPI', 'requires': 'same nested Apply frame and returned object/number'},
        ],
        'creation_scope': 'three direct Boolean sites; shared load/row creation remains distinct (CP103)',
        'future_positive_witness_requirements': [
            'one process/module/fixture/profile and explicitly selected Open span',
            'per-thread nested invocation ordinals with missing-return/read-error rejection',
            'window/project -> actual current impl/editor -> guarded OPI lifetime links',
            'normal Apply return inside selected Open; add normal return for creation claim',
            'actual D/emission/receiver links if claiming the modelUpdated route',
            'actual timer/connection lifetime if claiming an Open-registered timer origin',
            'gap-free earlier selected coverage for first/absence claims; positive is narrower',
        ],
        'forbidden_inferences': [
            'loader return 1c94 means whole Open returned',
            'Forming eb26f0 means Boolean 11e2480',
            'registration or posted/wake record means a live Boolean Apply occurred',
            'postLoad object, final object count, or file parity identifies an Apply origin',
            'candidate declared schedule supplies original HWND/serial/postedWake readiness',
        ],
    }


def contained_bytes(name):
    path = (ROOT / name).resolve(strict=True)
    if not path.is_relative_to(ROOT.resolve()) or not path.is_file():
        raise ValueError('input must be a regular file within this converter tree')
    if path.stat().st_size > 8 * 1024 * 1024:
        raise ValueError('input exceeds bounded 8 MiB preflight limit')
    return path.read_bytes()


def observations(document):
    normalized = document.get('normalized_result')
    if not isinstance(normalized, dict):
        raise ValueError('normalized_result must be an object')
    messages = normalized.get('messages')
    if not isinstance(messages, list) or len(messages) > 5000:
        raise ValueError('messages must be a bounded list')
    records = []
    payloads = []
    for index, message in enumerate(messages):
        if not isinstance(message, dict):
            raise ValueError('message must be an object')
        payload = message.get('payload', {})
        if not isinstance(payload, dict):
            raise ValueError('message payload must be an object')
        payloads.append(payload)
        if payload.get('kind') == 'observation':
            ordinal, tid = payload.get('ordinal'), payload.get('tid')
            if type(ordinal) is not int or ordinal <= 0 or type(tid) is not int or tid <= 0:
                raise ValueError('observations require positive integer ordinal and tid')
            if not isinstance(payload.get('stage'), str) or not isinstance(payload.get('data'), dict):
                raise ValueError('observation requires stage and data object')
            records.append({'message_index': index, **payload})
    return normalized, records, payloads


def inspect_capture(document):
    """Describe coverage only. Even synthetic event labels cannot admit a witness."""
    normalized, records, payloads = observations(document)
    issues = []
    extraction = extract_same_owner_frames(payloads)
    if extraction['errors']:
        issues.extend(f"frame extraction: {issue}" for issue in extraction['errors'])
    ordinals = [record['ordinal'] for record in records]
    if ordinals != list(range(1, len(records) + 1)):
        issues.append('observation ordinal coverage is not contiguous from 1')
    if normalized.get('messages_truncated') is not False:
        issues.append('message truncation state is unavailable or true')
    completions = [p for p in payloads if p.get('kind') == 'capture_complete_paused']
    hooks = []
    if len(completions) != 1:
        issues.append('exactly one bounded capture completion is required')
    else:
        complete = completions[0]
        hooks = complete.get('installedHooks', [])
        if not isinstance(hooks, list) or not all(isinstance(h, str) for h in hooks):
            raise ValueError('installedHooks must be a list of strings')
        totals = complete.get('totals', {})
        if (not isinstance(totals, dict)
                or totals.get('failedReads') != 0 or totals.get('callbackErrors') != 0
                or totals.get('records') != len(records) or complete.get('errorKinds') != 0
                or complete.get('truncations') != [] or complete.get('openContexts') != []):
            issues.append('capture completion has errors, truncation, open contexts or count mismatch')
    if any(p.get('kind') in ('error', 'read_error', 'callback_error') for p in payloads):
        issues.append('capture contains an explicit error record')
    stages = {}
    for record in records:
        stages.setdefault(record['stage'], []).append(record)
    expected = ['mainwindow_return', 'native_open_enter', 'project_read_enter',
                'project_read_return', 'native_open_return', 'post_open_enter',
                'editor_constructor_enter']
    if any(len(stages.get(stage, [])) != 1 for stage in expected):
        issues.append('selected CLI prefix is missing or ambiguous')
    else:
        prefix = [stages[s][0] for s in expected]
        if [r['ordinal'] for r in prefix] != sorted(r['ordinal'] for r in prefix):
            issues.append('selected CLI prefix is out of source order')
        if len({r['tid'] for r in prefix}) != 1:
            issues.append('selected CLI prefix crosses threads without a relation')
        window, entry, read, _, returned, post, editor = [r['data'] for r in prefix]
        if (not window.get('owner') or window.get('owner') != entry.get('owner')
                or entry.get('path') != FIXTURE or not read.get('project')
                or returned.get('project') != read.get('project')
                or returned.get('matches') is not True or post.get('project') != read.get('project')
                or not editor.get('editor') or not editor.get('operation')
                or editor.get('postOpen') is not True):
            issues.append('selected window/project/editor prefix links are incomplete')
    forming = FORMING_HOOK in hooks and BOOLEAN_HOOK not in hooks
    if not forming:
        issues.append('not the qualified Forming-only Editor004 constructor selector')
    endpoints = stages.get('selected_editor_first_grid_published', [])
    if len(endpoints) != 1 or not records or endpoints[0]['ordinal'] != records[-1]['ordinal']:
        issues.append('qualified first-grid endpoint is missing or not last')
    # No timing conclusion is inferred from absent hook coverage or stage labels.
    return {
        'coverage_issues': issues,
        'observation_count': len(records),
        'observed_editor_kind': 'Forming' if forming else 'unqualified',
        'constructor_rva': '0xeb26f0' if forming else None,
        'selected_prefix': [{k: r[k] for k in ('message_index', 'ordinal', 'tid', 'stage', 'data')}
                            for r in records if r['stage'] in expected],
        'observed_endpoint': 'selected_editor_first_grid_published' if endpoints else None,
        'open_boundary_coverage': {
            'CliLoaderReturned': 'observed' if 'native_open_return' in stages else 'unobserved',
            'CliPostOpenReturned': 'unobserved',
            'CliStartupBranchCompleted': 'unobserved',
            'GuiHandlerReturned': 'different profile; unobserved',
        },
        'boolean_apply_within_open': 'unobserved',
        'boolean_apply_creation_within_open': 'unobserved',
        'selected_whole_open_coverage': 'incomplete',
        'native_first_open_apply_timing': 'unknown',
        'witness_admitted': False,
        'first_or_absence_claim_admitted': False,
        'interpretation': 'unobserved is not proof of no original occurrence',
        'same_owner_frame_extraction': extraction,
    }


def evaluate(capture_bytes, receipt_bytes, probe_bytes):
    errors = []
    blobs = {CAPTURE: capture_bytes, RECEIPT: receipt_bytes, PROBE: probe_bytes}
    bindings = []
    for name, data in blobs.items():
        digest = hashlib.sha256(data).hexdigest()
        bindings.append({'path': name, 'sha256': digest, 'matches_pin': digest == PINNED[name]})
        if digest != PINNED[name]:
            errors.append(f'changed or unqualified input: {name}')
    capture = json.loads(capture_bytes.decode('utf-8-sig'))
    receipt = json.loads(receipt_bytes.decode('utf-8-sig'))
    if not isinstance(capture, dict) or not isinstance(receipt, dict):
        raise ValueError('capture and receipt must be objects')
    coverage = inspect_capture(capture)
    normalized = capture['normalized_result']
    if (capture.get('evidence_id') != receipt.get('evidence_id')
            or normalized.get('pid') != receipt.get('pid')
            or normalized.get('session_id') != receipt.get('session_id')):
        errors.append('root receipt/capture process-session binding mismatch')
    raw_files = receipt.get('raw_files', {})
    if not isinstance(raw_files, dict) or raw_files.get('capture-evidence.json') != PINNED[CAPTURE]:
        errors.append('root receipt lacks the selected raw capture hash')
    module_records = receipt.get('modules')
    if not isinstance(module_records, list):
        raise ValueError('receipt modules must be a list')
    for name, expected_hash in MODULES.items():
        matching = [m for m in module_records if isinstance(m, dict)
                    and str(m.get('path', '')).replace('\\', '/').split('/')[-1] == name]
        if len(matching) != 1 or matching[0].get('sha256') != expected_hash:
            errors.append(f'root-verified module binding missing: {name}')
    if receipt.get('errors') != [] or receipt.get('truncations') != []:
        errors.append('root receipt records errors or truncations')
    errors.extend(coverage['coverage_issues'])
    return {
        'status': 'rejected_input' if errors else 'valid_preflight_witness_unobserved',
        'valid_preflight': not errors,
        'witness_admitted': False,
        'errors': errors,
        'input_bindings': bindings,
        'module_identity_authority': 'pinned root receipt; not raw expectedFileSha256 alone',
        'manifest': selector_manifest(),
        'coverage': coverage,
        'effects': 'read-only local bytes; stdout only; no original/GUI/debugger/REA calls',
    }


def self_tests(blobs):
    capture_bytes, receipt_bytes, probe_bytes = blobs
    document = json.loads(capture_bytes.decode('utf-8-sig'))
    checks = []

    def check(name, condition):
        if not condition:
            raise ValueError(f'self-test failed: {name}')
        checks.append(name)

    valid = evaluate(*blobs)
    check('retained Forming evidence is valid preflight, never witness',
          valid['valid_preflight'] and not valid['witness_admitted']
          and valid['coverage']['observed_editor_kind'] == 'Forming')
    check('loader return does not close whole Open',
          valid['coverage']['open_boundary_coverage']['CliLoaderReturned'] == 'observed'
          and valid['coverage']['selected_whole_open_coverage'] == 'incomplete')
    origins = selector_manifest()['creation_origins']
    check('postLoad and two Apply origins have distinct normal returns',
          len({o['normal_return_rva'] for o in origins}) == 3
          and origins[0]['proves_apply'] is False
          and all(o['rcx_role'] == 'OPI' for o in origins))
    for name, mutate in [
        ('ordinal gap rejected', lambda d: d['normalized_result']['messages'][54]['payload'].update(ordinal=99)),
        ('truncated stream rejected', lambda d: d['normalized_result'].update(messages_truncated=True)),
        ('read failure rejected', lambda d: d['normalized_result']['messages'][-1]['payload']['totals'].update(failedReads=1)),
        ('false Boolean constructor selector rejected', lambda d: d['normalized_result']['messages'][-1]['payload']['installedHooks'].append(BOOLEAN_HOOK)),
        ('foreign Open owner rejected', lambda d: d['normalized_result']['messages'][54]['payload']['data'].update(owner='0xdeadbeef')),
    ]:
        changed = copy.deepcopy(document)
        mutate(changed)
        inspected = inspect_capture(changed)
        check(name, bool(inspected['coverage_issues']) and not inspected['witness_admitted'])
    forged = copy.deepcopy(document)
    forged['normalized_result']['messages'].append({'payload': {
        'kind': 'observation', 'stage': 'boolean_apply_return', 'ordinal': 74,
        'tid': 30444, 'data': {'editor': '0x383e780', 'operation': '0x37bdbe0'}}})
    rejected = evaluate(json.dumps(forged).encode(), receipt_bytes, probe_bytes)
    check('injected Apply label cannot manufacture a witness',
          not rejected['valid_preflight'] and not rejected['witness_admitted'])
    for index, label in [(1, 'changed root receipt rejected'), (2, 'changed probe rejected')]:
        changed = list(blobs)
        changed[index] += b' '
        rejected = evaluate(*changed)
        check(label, not rejected['valid_preflight'] and not rejected['witness_admitted'])
    malformed = copy.deepcopy(document)
    malformed['normalized_result']['messages'] = None
    try:
        inspect_capture(malformed)
    except ValueError:
        check('malformed message shape rejected', True)
    else:
        check('malformed message shape rejected', False)
    return {'status': 'passed', 'count': len(checks), 'cases': checks,
            'scope': 'offline checker rules only; synthetic mutations are not native evidence'}


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--capture', default=CAPTURE,
                        help='converter-local capture; only pinned Editor004 bytes qualify')
    parser.add_argument('--self-test', action='store_true', help='also run in-memory rejection tests')
    parser.add_argument('--require-witness', action='store_true',
                        help='exit 1 for the valid but unobserved native witness')
    args = parser.parse_args(argv)
    try:
        blobs = (contained_bytes(args.capture), contained_bytes(RECEIPT), contained_bytes(PROBE))
        result = evaluate(*blobs)
        result['requested_capture'] = args.capture
        result['tool_sha256'] = hashlib.sha256(Path(__file__).read_bytes()).hexdigest()
        if args.self_test:
            result['self_tests'] = self_tests(blobs)
        print(json.dumps(result, indent=2))
        return 2 if not result['valid_preflight'] else 1 if args.require_witness else 0
    except (OSError, ValueError, TypeError, KeyError) as error:
        print(json.dumps({'status': 'rejected_input', 'valid_preflight': False,
                          'witness_admitted': False, 'error': str(error)}, indent=2))
        return 2


if __name__ == '__main__':
    sys.exit(main())
