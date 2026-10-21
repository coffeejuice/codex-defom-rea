"""Summarize recent local Codex timing metadata; never copy conversation content."""
import argparse
from collections import Counter
from datetime import datetime
import hashlib
import json
from pathlib import Path


def merged(intervals):
    result=[]
    for a,b in sorted(intervals):
        if b<=a:continue
        if result and a<=result[-1][1]:result[-1][1]=max(b,result[-1][1])
        else:result.append([a,b])
    return result


def length(intervals):return sum(b-a for a,b in merged(intervals))


def overlap(a,b):
    return sum(max(0,min(y,v)-max(x,u))for x,y in merged(a)for u,v in merged(b))


def milliseconds(timestamp):return datetime.fromisoformat(timestamp.replace('Z','+00:00')).timestamp()*1000


def audit(path,tail_mib=32,turns=4):
    size=path.stat().st_size
    rows=[]
    with path.open('rb')as stream:
        start=max(0,size-tail_mib*1024*1024);stream.seek(start)
        if start:stream.readline()
        selected_start=stream.tell()
        selected=stream.read(max(0,size-selected_start))
    for line in selected.splitlines():
        try:rows.append(json.loads(line))
        except ValueError:pass
    events=[r for r in rows if r.get('type')=='event_msg']
    starts={r['payload']['turn_id']:r for r in events if r['payload'].get('type')=='task_started'}
    completions=[r for r in events if r['payload'].get('type')=='task_complete'and r['payload'].get('turn_id')in starts][-turns:]
    result=[]
    tools={'CommandExecution','McpToolCall','FileChange','Extension'}
    for complete in completions:
        c=complete['payload'];s=starts[c['turn_id']];begin=milliseconds(s['timestamp']);end=milliseconds(complete['timestamp'])
        items=[r['payload']for r in events if r['payload'].get('type')=='item_completed'and r['payload'].get('turn_id')==c['turn_id']]
        spans={kind:[]for kind in ['tools','reasoning','messages','compaction']};counts=Counter();failed=truncated=0;slow=[]
        for x in items:
            item=x.get('item',{});typ=item.get('type');a=max(begin,x.get('started_at_ms',begin));b=min(end,x.get('completed_at_ms',end))
            category='tools'if typ in tools else {'Reasoning':'reasoning','AgentMessage':'messages','ContextCompaction':'compaction'}.get(typ)
            if category:spans[category].append((a,b))
            if typ in tools:
                name=item.get('tool',typ);counts[name]+=1
                failed+=int(item.get('status')=='failed'or item.get('exit_code')not in (None,0))
                output=item.get('aggregated_output','');truncated+=int('truncated' in output.lower())
                slow.append(dict(kind=typ,name=name,seconds=round((b-a)/1000,3),item_id=item.get('id')))
        usage=[r for r in events if r['payload'].get('type')=='token_count'and r['payload'].get('info')]
        before=[r for r in usage if milliseconds(r['timestamp'])<=begin]
        after=[r for r in usage if begin<=milliseconds(r['timestamp'])<=end]
        delta=None
        if before and after:
            old=before[-1]['payload']['info']['total_token_usage'];new=after[-1]['payload']['info']['total_token_usage']
            if all(new.get(k,0)>=old.get(k,0)for k in old):
                delta={k:new.get(k,0)-v for k,v in old.items()}
                delta['uncached_input_tokens']=delta['input_tokens']-delta['cached_input_tokens']
        common=overlap(spans['tools'],spans['reasoning'])
        occupied=length(sum(spans.values(),[]))
        result.append(dict(turn_id=c['turn_id'],started_at=s['timestamp'],completed_at=complete['timestamp'],elapsed_seconds=round((end-begin)/1000,3),
            tool_active_union_seconds=round(length(spans['tools'])/1000,3),reasoning_item_union_seconds=round(length(spans['reasoning'])/1000,3),
            tool_reasoning_overlap_seconds=round(common/1000,3),compaction_seconds=round(length(spans['compaction'])/1000,3),
            unattributed_seconds=round(max(0,end-begin-occupied)/1000,3),tool_items=dict(counts),failed_tool_items=failed,
            truncated_command_outputs=truncated,usage_delta=delta,slowest_tool_items=sorted(slow,key=lambda x:x['seconds'],reverse=True)[:5]))
    return dict(schema_version=1,source=dict(name=path.name,bytes_at_scan=size,tail_mib=tail_mib,start_offset=selected_start,end_offset=size,window_sha256=hashlib.sha256(selected).hexdigest()),turns=result,
        qualifications=['Only completed turns with a start present in the bounded tail; no whole-history claim.',
        'Intervals are UI item lifetimes, not CPU time or proof that Codex was blocked. Concurrent spans are unioned, not added.',
        'Reasoning item time is observed model activity; unattributed time can include generation, client/approval latency and gaps. No causal attribution is inferred.',
        'Usage is cumulative-ledger delta when both endpoints exist and are monotonic. Cached input is separate; no price or billing estimate.',
        'No messages, commands, arguments, reasoning text, credentials or rate-limit records are copied.'])


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--session',type=Path,required=True)
    p.add_argument('--tail-mib',type=int,default=32,choices=range(1,129))
    p.add_argument('--turns',type=int,default=4,choices=range(1,21))
    p.add_argument('--report',type=Path,required=True)
    a=p.parse_args();data=audit(a.session,a.tail_mib,a.turns)
    a.report.write_text(json.dumps(data,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps({'turns':[{k:r[k]for k in ['turn_id','elapsed_seconds','tool_active_union_seconds','reasoning_item_union_seconds','compaction_seconds','unattributed_seconds','failed_tool_items','truncated_command_outputs']}for r in data['turns']],'report':str(a.report)}))


if __name__=='__main__':main()
