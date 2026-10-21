"""Reproducible local output-volume and warm-process timing comparison."""
import argparse
import hashlib
import json
from pathlib import Path
import statistics
import time
from evidence_slice import extract


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('source')
    parser.add_argument('--contains', required=True)
    parser.add_argument('--output', required=True)
    args = parser.parse_args()
    path = Path(args.source).resolve()
    samples = {'whole_json_read': [], 'focused_pseudocode_read': [], 'bound_slice': []}
    for _ in range(7):
        started = time.perf_counter()
        raw = path.read_bytes()
        samples['whole_json_read'].append(time.perf_counter() - started)
        started = time.perf_counter()
        code = json.loads(path.read_bytes().decode('utf-8-sig'))['normalized_result']['pseudocode']
        samples['focused_pseudocode_read'].append(time.perf_counter() - started)
        started = time.perf_counter()
        view = extract(path, '/normalized_result/pseudocode', count=200, contains=args.contains)
        encoded = json.dumps(view, ensure_ascii=False, indent=2).encode('utf-8')
        samples['bound_slice'].append(time.perf_counter() - started)
    # Independent selection oracle from direct JSON parsing, preserving original positions.
    expected = [{'index': i, 'value': line} for i, line in enumerate(code.splitlines(keepends=True)) if args.contains in line]
    assert view['selection']['items'] == expected[:200]
    assert view['selection']['matching_items'] == len(expected)
    assert view['source']['sha256'] == hashlib.sha256(raw).hexdigest()
    assert path.read_bytes() == raw
    result = {
        'schema_version': 1, 'source': view['source'], 'query': args.contains,
        'runs': 7, 'timing_scope': 'warm process; file read/parse/selection only; excludes tool latency and agent reasoning',
        'seconds': {name: {'samples': values, 'median': statistics.median(values)} for name, values in samples.items()},
        'output_bytes': {'whole_json': len(raw), 'focused_pseudocode': len(code.encode('utf-8')), 'bound_slice': len(encoded)},
        'matches': len(expected), 'fidelity_check': 'PASS: direct-parser selection, source hash, unchanged bytes',
        'limits': ['Output bytes are not measured tokens.', 'Whole-file output is a wasteful baseline; focused extraction is the stronger baseline.',
                   'Smaller output does not prove end-to-end time savings.', 'Parsing still reads the complete source; selected lines do not establish control-flow context.'],
        'tool_sha256': hashlib.sha256(Path(__file__).with_name('evidence_slice.py').read_bytes()).hexdigest(),
    }
    Path(args.output).write_text(json.dumps(result, indent=2) + '\n', encoding='utf-8')
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
