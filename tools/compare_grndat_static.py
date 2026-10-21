"""Execute the123 reviewed GRNDAT cases; native state comparison is separate."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess


def main():
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument('--driver', type=Path, required=True)
    p.add_argument('--report', type=Path)
    args = p.parse_args()
    root = Path(__file__).resolve().parents[1]
    plan_path = root / 'analysis/chat-a-grndat-fixture-plan.json'
    plan = json.loads(plan_path.read_bytes())
    assert hashlib.sha256(plan_path.read_bytes()).hexdigest() == 'e8e1219f4d5576d6b5da8f3ae10ef4426c0e970bca375e3c400bc9a130e869fb'
    for item in plan['files']:
        assert hashlib.sha256((root / item['path']).read_bytes()).hexdigest() == item['sha256']
    checks = 0
    cases = {}

    def equal(got, expected, where):
        nonlocal checks
        checks += 1
        assert got == expected, (where, got, expected)

    for case in plan['cases']:
        commands = ['initialized 3 20 0']
        commands += ['import ' + (root / f).read_bytes().hex()
                     for f in case['setup_imports'] + [case['input']]]
        proc = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands) + '\n',
                              text=True, capture_output=True, check=True)
        result = json.loads(proc.stdout.splitlines()[-1])
        cases[case['id']] = result
        equal(result['result']['outcome'], 'parsed', case['id'] + ' supported')
        handlers = [h for h in result['handler_states'] if h['id'] == 108]
        if handlers:
            equal(handlers[-1]['status'], case['expected']['handler_status'], case['id'] + ' status')
            equal(handlers[-1]['cursor'], case['stream']['expected_handler_return_position'], case['id'] + ' cursor')
        else:
            equal(bool(case['expected']['lifecycle']), True, case['id'] + ' lifecycle without handler')
    # Explicit checks for the two independent review findings and integer NEG.
    def material(case):
        return next(m for m in cases[case]['state']['materials'] if m['id'] == 20)
    partial = material('embedded_pair_eof')['grndat_extended']['embedded']
    equal(partial['integer_0x70'], 19, 'partial pair stores new integer')
    equal(partial['scalar_0x68'], struct.pack('<d', 8.5).hex(), 'partial pair retains old double')
    for suffix, count in [('minus1', -1), ('0', 0), ('1', 1), ('12', 12), ('13', 12)]:
        name = 'record_count_' + suffix
        equal(material(name)['grndat_extended']['embedded']['count_0x00'], count, name + ' stored count')
        diagnostic = bytes.fromhex(cases[name]['native_stdout_hex']).count(b'Error - SubbdryVarSize is incorrect.\n')
        equal(diagnostic, 1 if suffix == '13' else 0, name + ' clamp diagnostic')
    report = {'status': 'PASS_REPLACEMENT_STATUS_CURSOR_AND_SELECTED_MUTATIONS',
              'cases': len(cases), 'checks': checks,
              'qualification': 'All123 cases executed, including reserved cases. This is not independent held-out runtime validation or full-state native equivalence. Native capture and refresh/ownership comparisons remain outstanding.'}
    if args.report:
        args.report.write_text(json.dumps(report, indent=2) + '\n')
    print(json.dumps(report))


if __name__ == '__main__':
    main()
