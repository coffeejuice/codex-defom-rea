"""Read-only fixture preflight. Passing preparation never authorizes a solver run."""
import argparse
from datetime import datetime, timezone
import hashlib
import json
from pathlib import Path
import re
import shutil
import time
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
LARGE_SOURCE = Path(r'C:\PROBLEM\TEST_REA\Inc718_D420_GFM_V_DIES_Angle2_18')
STALE_CONTROLS = {
    'def_mpienv.dat', 'def_mpip4penv.dat', 'defmpi.tmp', 'def_arm_ctl.dat',
    'def_multi.ini', 'remesh.scr', 'def_pre.tdb', 'def_pre3.tdb',
    'cog_sim_input.ini', 'def_pre3.inp', 'def_pre.inp', 'newmesh.key',
}
GENERATED_NAMES = (
    'SfOprConvert2Dto3D_TEMP_STEP.KEY', '_TEMP_STEP.KEY', 'DEF_MULTI.INI',
    'DEF_PRE.TDB', 'DEF_PRE3.TDB', 'DEF_PRE.MSG', 'DEF_PRE3.INP',
    'DEF_PRE3.OUT', 'DEF_ARM_CTL.DAT', 'DEF_SIMULATION.log', 'REMESH.SCR',
)


def digest(path):
    with path.open('rb') as handle:
        return hashlib.file_digest(handle, 'sha256').hexdigest()


def inventory(directory, hashes=True):
    result = {}
    if not directory.is_dir():
        return result
    for path in sorted(directory.rglob('*')):
        if path.is_file():
            row = {'size': path.stat().st_size}
            if hashes:
                row['sha256'] = digest(path)
            result[path.relative_to(directory).as_posix()] = row
    return result


def compare_manifest(expected, actual):
    return {
        'missing': sorted(set(expected) - set(actual)),
        'unexpected': sorted(set(actual) - set(expected)),
        'changed': sorted(k for k in set(expected) & set(actual) if expected[k] != actual[k]),
    }


def refs_in_text(text):
    """Explicit file-reading statements in the prepared master/KEY inputs only."""
    lines = text.splitlines()
    for index, line in enumerate(lines):
        if re.match(r'^\s*(KFREAD|IMPEQP)\s+', line):
            if index + 1 < len(lines):
                yield lines[index + 1].strip()


def local_reference(directory, text):
    target = directory / text.replace('\\', '/')
    try:
        target.resolve().relative_to(directory.resolve())
    except ValueError:
        return None
    return target


def scalar_lines(text, key):
    return re.findall(r'^\s*' + re.escape(key) + r'\s+([^\r\n]+)', text, re.M)


def check_controller(directory, metadata):
    errors, facts = [], {}
    def require(condition, message):
        if not condition:
            errors.append(message)
    stale = sorted(p.relative_to(directory).as_posix() for p in directory.rglob('*')
                   if p.is_file() and p.name.lower() in STALE_CONTROLS)
    facts['stale_controller_inputs'] = stale
    require(not stale, 'Stale controller/MPI/remesh inputs remain: ' + ', '.join(stale))
    require(metadata.get('deadline_seconds') == 120, 'Recorded job deadline must be 120 seconds')
    require(metadata.get('operations') == 1 and metadata.get('steps') == 1
            and metadata.get('workers') == 1 and metadata.get('remeshing') is False,
            'Controller manifest must require one operation, one step, one worker and no remeshing')
    facts['deadline_requirement_seconds'] = 120
    facts['deadline_enforcement'] = 'Runtime supervisor required; this read-only tool does not enforce a deadline'
    projects = list(directory.glob('*.moproj'))
    masters = list(directory.glob('*.MST'))
    require(len(projects) == 1 and len(masters) == 1, 'Exactly one MO project and master file required')
    if len(projects) != 1 or len(masters) != 1:
        return {'errors': errors, 'facts': facts}
    try:
        xml = ET.parse(projects[0]).getroot()
        operations = xml.find('Operation_List')
        require(operations is not None and len(operations) == 1 and operations.get('Number') == '1',
                'XML must contain exactly one declared operation')
        if operations is not None and len(operations):
            op = operations[0]
            require(op.findtext('Id') == '1' and op.findtext('Type') == 'SfOprHeatTransfer',
                    'Bounded fixture must select operation 1, SfOprHeatTransfer')
            objects = op.findall('Objects/Object')
            require(bool(objects) and all(o.get('Remesh') == '0' for o in objects),
                    'Every active object must explicitly disable remeshing')
            require(all(not ''.join(e.itertext()).strip() for tag in ('UserDefinedKey', 'RestartKey')
                        for e in op.findall(tag)), 'Unreviewed custom/restart input could override bounded controls')
            for name, expected in [('DTMAX', '0.1'), ('HeatingTime', '0.1 sec')]:
                properties = [p for p in op.iter('Property') if p.findtext('Name') == name]
                require(len(properties) == 1 and len(properties[0].findall('Value')) == 1
                        and properties[0].findtext('Value') == expected,
                        'XML requires exactly one ' + name + ' property with one value of ' + expected)
        sub = xml.find('Sub_Project')
        diagrams = [] if sub is None else sub.findall('DiagramItem')
        require(sub is not None and sub.get('Number') == '1' and len(diagrams) == 1
                and diagrams[0].get('Operation_Id') == '1', 'Diagram must contain only operation 1')
        require(all(c.get('Number') == '0' and len(c) == 0 for d in diagrams
                    for c in d.findall('Connectors/Connections')), 'Multi-operation diagram connections remain')
    except (ET.ParseError, OSError) as exc:
        errors.append('Unreadable controller XML: ' + str(exc))
    master = masters[0].read_text(encoding='latin-1')
    require(re.findall(r'^SIMULATION\s+(\d+)\s*$', master, re.M) == ['1'],
            'Master must contain exactly one SIMULATION 1 block')
    require(scalar_lines(master, 'DIM') == ['3'], 'Master must select DIM 3')
    require(scalar_lines(master, 'NSTART') == ['-1'], 'Master must start fresh with NSTART -1')
    # Inspect the actual include graph so a second later NSTEP cannot override the first.
    seen, pending, included_text = set(), [(masters[0], master)], []
    while pending:
        path, text = pending.pop()
        if path in seen:
            continue
        seen.add(path)
        included_text.append(text)
        for reference in refs_in_text(text):
            target = local_reference(directory, reference)
            if target is None or not target.is_file():
                errors.append('Missing/outside controller include: ' + reference)
                continue
            if target not in seen:
                if len(seen) > 128:
                    errors.append('Controller include graph exceeds bounded inspection limit')
                    pending.clear()
                    break
                pending.append((target, target.read_text(encoding='latin-1')))
    combined = '\n'.join(included_text)
    require(scalar_lines(combined, 'NSTEP') == ['1'], 'Effective input graph must contain exactly one NSTEP 1')
    for keyword in ('DTMAX', 'TMAX'):
        values = scalar_lines(combined, keyword)
        try:
            require(len(values) == 1 and float(values[0].split()[0]) == 0.1,
                    'Effective input graph must contain exactly one ' + keyword + ' 0.1')
        except (IndexError, ValueError):
            errors.append('Invalid numeric ' + keyword)
    facts['master_include_files'] = sorted(p.relative_to(directory).as_posix() for p in seen)
    config = directory / 'JobGroupSetupConfigureFile.txt'
    try:
        text = config.read_text(encoding='latin-1')
        expected = {'nSimultaneousSimulations': '1', 'nCoreForEachSimulation': '1',
                    'ComputerNum': '0', 'JobRunMode': 'E_RUN_ON_LOCALLY'}
        for key, value in expected.items():
            require(re.findall(r'^' + key + r'=(.*)$', text, re.M) == [value],
                    'Job configuration requires unique ' + key + '=' + value)
        facts['worker_configuration'] = expected
    except OSError as exc:
        errors.append('Unreadable worker configuration: ' + str(exc))
    require((directory / '64bit.dat').is_file(), '64bit.dat marker required for intended 64-bit selection')
    return {'errors': errors, 'facts': facts}


def check_case(directory, expected, controller=None, dimension2=False, path_limit=100):
    actual = inventory(directory)
    mismatch = compare_manifest(expected, actual)
    errors = ['Manifest mismatch: ' + key + ' ' + ', '.join(value)
              for key, value in mismatch.items() if value]
    if not directory.is_dir():
        errors.append('Fixture directory missing')
    current = [p for p in directory.rglob('*')]
    planned = [directory / name for name in GENERATED_NAMES]
    for project in directory.glob('*.moproj'):
        planned += [directory / (project.stem + suffix) for suffix in
                    ('.DB', '.KEY', '.MSG', '.LOG', '.MST', '.DEF_ARM_CTL_LOG')]
    if dimension2:
        planned += [directory / ('RING' + suffix) for suffix in ('.DB', '.MSG', '.LOG')]
    long_paths = sorted(str(p) for p in current + planned if len(str(p)) >= path_limit)
    if long_paths:
        errors.append('Path length must be strictly below ' + str(path_limit))
    links = sorted(str(p) for p in current if p.is_symlink() or (hasattr(p, 'is_junction') and p.is_junction()))
    if links:
        errors.append('Fixture contains links or junctions')
    xml_refs = []
    for project in directory.glob('*.moproj'):
        try:
            for element in ET.parse(project).iter('Path'):
                reference = (element.text or '').strip()
                target = local_reference(directory, reference)
                valid = bool(reference and target is not None and target.is_file())
                xml_refs.append({'path': reference, 'resolved': valid})
                if not valid:
                    errors.append('Missing/outside XML path: ' + reference)
        except (ET.ParseError, OSError) as exc:
            errors.append('Unreadable XML: ' + str(exc))
    if dimension2:
        key = directory / 'RING.KEY'
        text = key.read_text(encoding='latin-1') if key.is_file() else ''
        if 'DEFORM-2D' not in text[:256] or scalar_lines(text, 'GEOTYP') != ['1']:
            errors.append('2D static identification requires DEFORM-2D header and unique GEOTYP 1')
    control = check_controller(directory, controller) if controller is not None else None
    if control:
        errors += control['errors']
    historical = sorted(name for name in actual if Path(name).suffix.lower() in ('.db', '.msg', '.log')
                        or name.lower().endswith('_log') or Path(name).name.lower().startswith('.p4p_tmp'))
    return {
        'path': str(directory), 'status': 'preflight_blocked_not_runtime_validated' if errors
        else 'prepared_not_runtime_validated', 'errors': errors,
        'files': len(actual), 'bytes_hashed': sum(v['size'] for v in actual.values()),
        'manifest_match': not any(mismatch.values()), 'manifest_differences': mismatch,
        'current_and_known_generated_max_path': max([len(str(directory))] + [len(str(p)) for p in current + planned]),
        'overlong_paths': long_paths, 'known_generated_paths': [str(p) for p in planned],
        'xml_references_checked': len(xml_refs), 'unresolved_xml_references': [r for r in xml_refs if not r['resolved']],
        'controller': control, 'historical_outputs_excluded_from_new_result_evidence': historical,
        'historical_job_metadata': [name for name in actual if Path(name).name.lower() == 'jobid'],
        'dimension_qualification': 'Static DEFORM-2D/GEOTYP 1 only; GUI verification pending' if dimension2 else 'GUI verification pending',
        'runtime_acceptance': False, 'execution_authorized_by_preflight': False,
        'qualification': 'Known paths only; capture-time checks must reject any unplanned generated path at or above 100 characters.',
    }, actual


def large_inventory(source, trial_root):
    files = inventory(source, hashes=False)
    projected = [trial_root / case / relative for case in ('OL', 'CL') for relative in files]
    generated = [trial_root / case / name for case in ('OL', 'CL') for name in GENERATED_NAMES]
    for relative in files:
        if Path(relative).suffix.lower() == '.moproj':
            generated += [trial_root / case / (Path(relative).stem + suffix)
                          for case in ('OL', 'CL') for suffix in ('.DB', '.KEY', '.MSG', '.LOG', '.MST', '.DEF_ARM_CTL_LOG')]
    existing_parent = trial_root
    while not existing_parent.exists() and existing_parent != existing_parent.parent:
        existing_parent = existing_parent.parent
    usage = shutil.disk_usage(existing_parent)
    total = sum(row['size'] for row in files.values())
    return {'source': str(source), 'status': 'metadata_only_not_copied_not_runtime_validated',
            'file_count': len(files), 'source_bytes': total, 'two_copy_bytes': total * 2,
            'free_bytes': usage.free, 'space_for_two_copies_only': usage.free >= total * 2,
            'space_qualification': 'Output growth and runtime scratch not reserved; recheck before copying/running.',
            'projected_max_path': max([0] + [len(str(p)) for p in projected + generated]),
            'overlong_paths': [str(p) for p in projected + generated if len(str(p)) >= 100],
            'files': files, 'large_file_content_read': False, 'runtime_acceptance': False}


def run_preflight(manifest, protocol, large_source=LARGE_SOURCE):
    start = time.perf_counter()
    source = Path(manifest['source'])
    source_actual = inventory(source)
    source_diff = compare_manifest(manifest['source_manifest'], source_actual)
    results, actuals = {}, {}
    for name, item in manifest['copies'].items():
        results[name], actuals[name] = check_case(Path(item['path']), item['manifest'],
                                               item.get('controller'), name in ('O2', 'C2'))
    pairs = {a + '/' + b: actuals.get(a) == actuals.get(b) for a, b in
             (('O3', 'C3'), ('OS', 'CS'), ('O2', 'C2'))}
    errors = []
    if any(source_diff.values()):
        errors.append('Original small source differs from preparation manifest')
    if not all(pairs.values()):
        errors.append('One or more intended original/candidate input pairs differ')
    for required in ('O3', 'C3', 'OS', 'CS', 'O2', 'C2'):
        if required not in results:
            errors.append('Required fixture missing from manifest: ' + required)
    excluded = {str(Path(e['path']).resolve()).lower() for e in manifest.get('excluded_trials', [])}
    for name, item in manifest['copies'].items():
        if str(Path(item['path']).resolve().parent).lower() in excluded:
            errors.append('Excluded partial trial selected: ' + name)
    comparator = Path(protocol['comparator']['path'])
    comparator_ok = comparator.is_file() and digest(comparator) == protocol['comparator']['sha256']
    if not comparator_ok:
        errors.append('Existing comparator binary is missing or hash changed')
    large = large_inventory(large_source, Path(manifest['root']))
    blocked = bool(errors or any(r['errors'] for r in results.values()))
    return {'schema_version': 1, 'utc': datetime.now(timezone.utc).isoformat(),
            'status': 'preflight_blocked_not_runtime_validated' if blocked else 'prepared_not_runtime_validated',
            'errors': errors, 'root': manifest['root'], 'source_unchanged': not any(source_diff.values()),
            'source_manifest_differences': source_diff, 'cases': results, 'paired_input_equality': pairs,
            'comparator': {'path': str(comparator), 'manifest_hash_match': comparator_ok},
            'large_fixture': large, 'elapsed_seconds': round(time.perf_counter() - start, 4),
            'runtime_acceptance': False, 'execution_authorized_by_preflight': False,
            'remaining_gates': ['Supported GUI control release and verification', 'Fresh GUI load and generation',
                                'Captured invoked process version/hash/command/CWD', 'Independent original replay and comparisons',
                                'Correct GUI reopen', 'Runtime supervisor enforcing 120 seconds and trial-owned process cleanup'],
            'evidence_qualification': 'All existing DB/log/message files are baseline history. This checks preparation only.'}


def main():
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument('--manifest', type=Path, default=ROOT / 'analysis/gui-validation-fixtures.json')
    ap.add_argument('--protocol', type=Path, default=ROOT / 'analysis/gui-validation-comparator-contract.json')
    ap.add_argument('--output', type=Path)
    args = ap.parse_args()
    report = run_preflight(json.loads(args.manifest.read_text()), json.loads(args.protocol.read_text()))
    report['inputs'] = {str(p): digest(p) for p in (args.manifest, args.protocol)}
    if args.output:
        # The only write is an explicitly selected report inside this checkout.
        args.output.resolve().relative_to(ROOT.resolve())
        args.output.write_text(json.dumps(report, indent=2) + '\n', encoding='utf-8')
    print(json.dumps({key: report[key] for key in ('status', 'root', 'source_unchanged', 'paired_input_equality',
                                                 'errors', 'elapsed_seconds', 'runtime_acceptance')}))
    return 1 if report['status'].startswith('preflight_blocked') else 0


if __name__ == '__main__':
    raise SystemExit(main())
