"""Retain one exact REA evidence record and summarize capture integrity."""
import argparse
import hashlib
import json
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('bundle', type=Path)
    parser.add_argument('evidence_id')
    parser.add_argument('output', type=Path)
    args = parser.parse_args()
    assert not args.output.exists(), 'Preserve existing capture records'
    bundle = json.loads(args.bundle.read_text(encoding='utf-8'))
    matches = [record for record in bundle['records'] if record['evidence_id'] == args.evidence_id]
    assert len(matches) == 1, 'Exact Evidence ID must occur once'
    record = matches[0]
    args.output.write_text(json.dumps(record, indent=2) + '\n', encoding='utf-8')
    result = record['normalized_result']
    messages = result['messages']
    errors = [message for message in messages if message.get('type') == 'error'
              or message.get('payload', {}).get('operation') == 'probe-error']
    print(json.dumps({'output': str(args.output), 'evidence_id': args.evidence_id,
                      'sha256': hashlib.sha256(args.output.read_bytes()).hexdigest(),
                      'messages': len(messages), 'errors': errors[:6],
                      'messages_truncated': result['messages_truncated'],
                      'state': result['state'], 'cleanup_error': result['cleanup_error']}))


if __name__ == '__main__':
    main()
