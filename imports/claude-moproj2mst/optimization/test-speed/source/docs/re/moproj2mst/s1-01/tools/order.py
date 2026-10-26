# predicts the MST operation order (spec s1-01 section "flow-chart order") and compares with OPRNAM/SIMTYP in saved MST
import sys,glob,os,re
import xml.etree.ElementTree as ET
def predict(path):
    r=ET.parse(path).getroot()
    ops={}
    for o in r.find('Operation_List').findall('Operation'):
        ops[int(o.findtext('Id'))]=(o.findtext('Name'),o.findtext('Type'))
    sub=r.findall('Sub_Project')[0]
    items=[]
    for it in sub.findall('DiagramItem'):
        cp=it.find('CellPos'); gi=it.find('GroupInfo')
        if cp is None: cp={'Row':'-999','Col':'-999'}
        succ=[int(x.text) for x in it.iter('ItemIndex')]
        items.append(dict(id=int(it.get('Operation_Id')),cls=it.findtext('ClassName'),row=int(cp.get('Row')),col=int(cp.get('Col')),succ=succ,
            grp=(int(gi.get('BeginRow')),int(gi.get('BeginCol')),int(gi.get('EndRow')),int(gi.get('EndCol'))) if gi is not None else None))
    nxt={};prv={}
    for k,it in enumerate(items):
        for s in it['succ']:
            if 0<=s<len(items): nxt[k]=s; prv[s]=k
    plain=[k for k,it in enumerate(items) if it['grp'] is None]
    if not plain: return [],ops
    plain.sort(key=lambda k:(items[k]['row'],items[k]['col']))
    h=plain[0]
    while h in prv: h=prv[h]
    chain=[h]
    while chain[-1] in nxt and nxt[chain[-1]] not in chain: chain.append(nxt[chain[-1]])
    # group map
    gmap={}
    for g in sorted([k for k,it in enumerate(items) if it['grp'] is not None],key=lambda k:items[k]['grp'][:2]):
        br,bc,er,ec=items[g]['grp']
        for k in plain:
            r_,c_=items[k]['row'],items[k]['col']
            if ((r_>br) or (r_==br and c_>=bc)) and ((r_<er) or (r_==er and c_<=ec)): gmap[items[k]['id']]=items[g]['id']
    out=[];last=-1
    for k in chain:
        i=items[k]['id']
        if i not in gmap: out.append(i); last=-1
        elif gmap[i]!=last: out.append(gmap[i]); last=gmap[i]
    return out,ops
for f in sorted(glob.glob(sys.argv[1]+'/*.moproj')):
    m=f[:-7]+'.MST'
    try: order,ops=predict(f)
    except Exception as e: print(os.path.basename(f),'ERR',e); continue
    pred=[ops[i][0] if i in ops else '?%d'%i for i in order]
    if not os.path.exists(m): print(os.path.basename(f),'noMST',len(pred)); continue
    t=open(m,encoding='latin-1').read().split('\n')
    seen=[];cur=[]
    for i,l in enumerate(t):
        if l.startswith('OPRNAM'): seen.append(t[i+1])
    dedup=[]
    for s in seen:
        if not dedup or dedup[-1]!=s: dedup.append(s)
    print(os.path.basename(f), 'OK' if dedup==pred else 'DIFF', len(pred), len(dedup), '' if dedup==pred else (pred[:8],dedup[:8]))
