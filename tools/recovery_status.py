"""Print compact continuation and efficiency-review due status without native work."""
from datetime import datetime,timezone
import json
from pathlib import Path
import subprocess


def main():
    root=Path(__file__).resolve().parents[1]
    plan=json.loads((root/'analysis/recovery-plan.json').read_bytes())
    state_path=root/'analysis/efficiency-state.json'
    state=json.loads(state_path.read_bytes())if state_path.exists()else{}
    git=lambda *args:subprocess.check_output(['git',*args],cwd=root,text=True).strip()
    head=git('rev-parse','HEAD');base=state.get('last_review_head');commits=None
    if base:
        exists=subprocess.run(['git','merge-base','--is-ancestor',base,head],cwd=root,capture_output=True)
        if exists.returncode==0:commits=int(git('rev-list','--count',base+'..'+head,'--','src','include','tests/fixtures',':(glob)analysis/*contract*.json'))
    elapsed=None
    if state.get('last_review_utc'):
        elapsed=(datetime.now(timezone.utc)-datetime.fromisoformat(state['last_review_utc'])).total_seconds()/60
    due=not state or commits is None or commits>=3 or(elapsed is not None and elapsed>=60)
    print(json.dumps(dict(head=head,execution=plan.get('execution'),current_work=plan.get('current_work'),
        efficiency=dict(due=due,commits_since_review=commits,minutes_since_review=None if elapsed is None else round(elapsed,1),policy='analysis/efficiency-policy.md',state='analysis/efficiency-state.json'))))


if __name__=='__main__':main()
