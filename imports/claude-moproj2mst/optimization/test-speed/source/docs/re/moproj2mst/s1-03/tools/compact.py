# compact QTextStream chains: annot output -> readable "<<" chains
import sys,re
t=open(sys.argv[1]).read().split('\n')
out=[]
op={'PEBD':'str','H':'int','N':'dbl','AEBVQString__':'qs','I':'uint','M':'float','D':'char','J':'long','_J':'i64','K':'ulong'}
pat=re.compile(r'^(\s*)(?:(\w+) = )?__6QTextStream__QEAAAEAV0_(\w+?)_Z\((\w+),(.*)\);$')
endl=re.compile(r'^(\s*)FUN_140041bc0\((\w+),_endl__YAAEAVQTextStream__AEAV1__Z_exref\);$')
for l in t:
    m=pat.match(l)
    if m:
        ind,dst,ty,src,arg=m.groups()
        arg=re.sub(r'&UNK_\w+/\*S(.*?)\*/',r'\1',arg)
        head='' if src.startswith('uVar') and dst==src else '%s<< '%('STREAM(%s) '%src)
        out.append('%s%s<<%s:%s'%(ind,head,ty,arg)); continue
    m=endl.match(l)
    if m:
        out.append('%s<<endl;  [%s]'%(m.group(1),m.group(2))); continue
    out.append(l)
open(sys.argv[2],'w').write('\n'.join(out))
