import pickle,sys,collections
W='C:/Users/alext/Projects/deform-rea-work/gui_mo/s1/'
IB=0x140000000
callees=pickle.load(open(W+'callees.pkl','rb'))
vt,label=pickle.load(open(W+'vtables.pkl','rb'))
roots0=[r-IB for r in (0x14008a9b0,0x14008afa0,0x14008b410,0x1401313c0)]
ops=[n for n in vt if n.startswith('SfOpr') and len(vt[n])>0x280//8]
roots0+=[vt[n][0x280//8] for n in ops]
def bfs(roots):
    par={r:None for r in roots}; st=list(roots)
    while st:
        f=st.pop(0)
        for g in callees.get(f,()):
            if g not in par: par[g]=f; st.append(g)
    return par
R0=bfs(roots0)
# every slot offset the port calls (grep vt+0x / vt[0x in src), on every Sf*/Def* class
SLOTS=[0x38,0x40,0x48,0x58,0x60,0x70,0x80,0x88,0x98,0xc0,0xe0,0x100,0x120,0x158,0x170,0x178,0x180,0x188,0x190,0x198,0x1a0,0x1a8,0x1b0,0x1b8,0x1c0,0x1c8,0x1e0,0x200,0x208,0x260,0x280,0x288,0x290,0x298,0xb8]
r1=list(roots0)
for n in vt:
    if n.startswith(('Sf','Def','ISf')):
        r1+=[vt[n][s//8] for s in SLOTS if s//8<len(vt[n])]
R1=bfs(r1)
def path(par,f):
    p=[]
    while f is not None and len(p)<8: p.append(hex(f+IB)); f=par[f]
    return '<'.join(p)
for ln in open(sys.argv[1],encoding='utf-8'):
    a=ln.split('\t')[1] if '\t' in ln else ln.strip()
    f=int(a,16)-IB
    k='direct' if f in R0 else ('virtual' if f in R1 else 'no')
    print(k, ln.rstrip()[:150], '|', path(R0 if f in R0 else R1,f) if k!='no' else '',sep='\t')
