"""Source-contract guard test; does not execute the original's uninitialized read."""
import argparse
import json
from pathlib import Path
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--driver', type=Path, required=True)
    args = parser.parse_args()
    commands = ['initialized 3 20 0', 'import '+b'ELMNOD 10\n1\n'.hex(),
                'import '+b'HTMTHD 7\n'.hex()]
    result = subprocess.run([str(args.driver.resolve())], input='\n'.join(commands)+'\n',
                            text=True, capture_output=True, check=True)
    first, blocked = [json.loads(line) for line in result.stdout.splitlines()]
    assert first['result']['outcome'] == 'unsupported'
    assert 'uninitialized native payload' in first['result']['unsupported_reason']
    assert first['state']['element_layout_0x888'] == [1]+[None]*9
    assert first['state']['element_layout_known'] == [True]+[False]*9
    assert first['state']['grain_mode_0x898'] is None
    assert blocked['result']['outcome'] == 'unsupported'
    assert 'indeterminate native values' in blocked['result']['unsupported_reason']
    assert blocked['state']['heat_method_0x868'] == 0
    fresh = subprocess.run([str(args.driver.resolve())],
                           input='initialized 3 20 0\nimport '+b'ELMNOD 3\n1 2 3\n'.hex()+'\n',
                           text=True, capture_output=True, check=True)
    reset = json.loads(fresh.stdout)
    assert reset['result']['outcome'] == 'parsed'
    assert reset['state']['element_layout_0x888'] == [1,2,3]+[0]*7
    assert reset['state']['element_layout_known'] == [True]*10
    print(json.dumps(dict(status='PASS_ELMNOD_UNKNOWN_CONTROL_GUARD',
                          scope='Source-derived indeterminate-value metadata and reset behavior only; no native uninitialized-stack execution.')))


if __name__ == '__main__':
    main()
