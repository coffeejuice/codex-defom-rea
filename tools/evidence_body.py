"""Select a bounded, source-bound function body across retained REA JSON schemas."""
import argparse
import hashlib
import json
from pathlib import Path


def select(path,procedure,start=0,count=40):
    if start<0 or not 1<=count<=200:raise ValueError('start>=0 and count1..200 required')
    raw=path.read_bytes();document=json.loads(raw.decode('utf-8-sig'))
    records=[]
    def visit(value,pointer=''):
        if not isinstance(value,dict):return
        if 'evidence_id'in value:records.append((pointer,value));return
        if isinstance(value.get('records'),list):
            for i,r in enumerate(value['records']):visit(r,pointer+'/records/'+str(i))
        if isinstance(value.get('evidence_bundle'),dict):visit(value['evidence_bundle'],pointer+'/evidence_bundle')
    visit(document)
    wanted=hex(int(procedure,16));matches=[]
    for pointer,record in records:
        parameters=record.get('parameters')or{}
        procedure_record=record.get('procedure')
        address=parameters.get('procedure')or(procedure_record.get('address')if isinstance(procedure_record,dict)else procedure_record)
        try:address=hex(int(address,16))
        except (ValueError,TypeError):continue
        if address!=wanted:continue
        body=None;body_pointer=None
        for key in ['pseudocode','normalized_result','raw_result']:
            value=record.get(key)
            if isinstance(value,str)and(key=='pseudocode'or record.get('operation')=='procedure_pseudo_code'):
                body=value;body_pointer=pointer+'/'+key;break
            if isinstance(value,dict):
                if isinstance(value.get('pseudocode'),str):body=value['pseudocode'];body_pointer=pointer+'/'+key+'/pseudocode';break
                child=value.get('value')
                if isinstance(child,dict)and isinstance(child.get('pseudocode'),str):body=child['pseudocode'];body_pointer=pointer+'/'+key+'/value/pseudocode';break
                if isinstance(child,str)and record.get('operation')=='procedure_pseudo_code':body=child;body_pointer=pointer+'/'+key+'/value';break
        if body is None:continue
        lines=body.replace('\r\n','\n').replace('\r','\n').splitlines()
        qualifications=[]
        for location in ['limitations','relationship_coverage','normalized_result/limitations','normalized_result/relationship_coverage','raw_result/limitations','raw_result/relationship_coverage']:
            value=record
            for key in location.split('/'):
                if not isinstance(value,dict)or key not in value:break
                value=value[key]
            else:qualifications.append(pointer+'/'+location)
        matches.append(dict(evidence_id=record['evidence_id'],procedure=address,record_pointer=pointer,body_pointer=body_pointer,
            target_sha256=record.get('subject',{}).get('digest',{}).get('sha256'),authority=record.get('authority'),confidence=record.get('confidence'),
            qualification_pointers=qualifications,
            total_lines=len(lines),start=start,lines=lines[start:start+count],omitted_lines=max(0,len(lines)-len(lines[start:start+count]))))
    return dict(source=dict(path=str(path.resolve()),bytes=len(raw),sha256=hashlib.sha256(raw).hexdigest()),procedure=wanted,
        status='matches'if matches else'not_found_in_selected_file',matches=matches,
        notice='Retained text only, not new analysis. Absence is scoped to supported records in this file. Read the source-bound qualifications before making behavioral claims.')


def main():
    p=argparse.ArgumentParser(description=__doc__);p.add_argument('source',type=Path);p.add_argument('--procedure',required=True)
    p.add_argument('--start',type=int,default=0);p.add_argument('--count',type=int,default=40)
    p.add_argument('--max-output-bytes',type=int,default=16000)
    a=p.parse_args();result=select(a.source,a.procedure,a.start,a.count);out=json.dumps(result,ensure_ascii=True,indent=2)
    if len(out.encode())>a.max_output_bytes:
        print(json.dumps(dict(status='output_budget_exceeded',procedure=a.procedure,matches=len(result['matches']),hint='Use fewer lines or select a compact source; no body was printed.')));return 2
    print(out);return 0


if __name__=='__main__':raise SystemExit(main())
