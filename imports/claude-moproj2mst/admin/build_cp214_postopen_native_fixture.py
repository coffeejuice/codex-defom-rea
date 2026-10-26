"""Fixed harmless nine-prologue fixture, derived from retained synthetic CP213."""
from pathlib import Path
import hashlib,json,os,struct,subprocess
ROOT=Path(__file__).resolve().parent.parent
BASE=ROOT/'scratch/cp213e02'
prior=json.loads((BASE/'metadata.json').read_text())
for n,v in prior['sourceHashes'].items():assert hashlib.sha256((BASE/n).read_bytes()).hexdigest()==v
OUT=ROOT/'scratch/cp214p02';OUT.mkdir(parents=True,exist_ok=False)
VC=Path(r'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Auxiliary/Build/vcvars64.bat')
r=subprocess.run(f'cmd.exe /d /s /c ""{VC}" >nul && set"',capture_output=True,text=True,check=True)
env=dict(os.environ)
for line in r.stdout.splitlines():
 k,sep,v=line.partition('=')
 if sep and k:
  for old in list(env):
   if old.lower()==k.lower():del env[old]
  env[k]=v
code=(BASE/'fixture.c').read_text().replace('p213a','p214a').replace('g_fail>7','g_fail>9')
asm=(BASE/'fixture.asm').read_text()
asm='PUBLIC postopen_cont,current_cont,activation_cont\n'+asm
asm=asm.replace('EXTERN g_main:BYTE','EXTERN Sleep:PROC\nEXTERN g_main:BYTE')
asm=asm.replace('register_bad:\nmov rax,[rsp+28h]', 'register_bad:\nmov rdx,[rsp+28h]\nlea rcx,g_main\ncall postopen\npostopen_cont::\nmov rax,[rsp+28h]')
prefixes={h['name']:h['bytes'] for h in prior['hooks']}
prefixes.update(postopen='488954241048894c240856574881ec88040000',current='48894c24084883ec48',activation='8954241048894c24084883ec38')
raw=lambda name:'db '+','.join('0'+prefixes[name][i:i+2]+'h' for i in range(0,len(prefixes[name]),2))
extra=['postopen PROC',raw('postopen'),'call postcore','add rsp,488h','pop rdi','pop rsi','ret','postopen ENDP',
 'postcore PROC','sub rsp,38h','call current','current_cont::','test rax,rax','jz postcore_done','mov rcx,rax','xor edx,edx','call activation','activation_cont::','postcore_done:','add rsp,38h','ret','postcore ENDP',
 'current PROC',raw('current'),'xor eax,eax','cmp g_fail,8','je current_done','mov eax,100h','current_done:','add rsp,48h','ret','current ENDP',
 'activation PROC',raw('activation'),'cmp g_fail,9','jne activation_done','activation_loop:','mov ecx,100','call Sleep','jmp activation_loop','activation_done:','add rsp,38h','ret','activation ENDP']
asm=asm.replace('\nEND\n','\n'+'\n'.join(extra)+'\nEND\n')
(OUT/'fixture.c').write_text(code);(OUT/'fixture.asm').write_text(asm)
names=['duplicate','factory','cast','project','acceptance','postopen','current','activation']
bin=Path(env['VCToolsInstallDir'])/'bin/Hostx64/x64'
commands=[[str(bin/'ml64.exe'),'/nologo','/c','/Fo'+str(OUT/'hooks.obj'),str(OUT/'fixture.asm')],
 [str(bin/'cl.exe'),'/nologo','/Od','/MT',str(OUT/'fixture.c'),str(OUT/'hooks.obj'),'/Fo'+str(OUT/'main.obj'),'/Fe'+str(OUT/'fixture.exe'),'/link','/INCREMENTAL:NO',*['/EXPORT:'+n for n in ['synthetic_open','outer','open_cont']+names+[n+'_cont' for n in names]]]]
logs=[]
for c in commands:
 r=subprocess.run(c,cwd=OUT,env=env,capture_output=True,text=True);logs.append({'command':c,'exit':r.returncode,'stdout':r.stdout,'stderr':r.stderr});(OUT/'build.json').write_text(json.dumps(logs,indent=2))
 if r.returncode:raise SystemExit(r.stdout+r.stderr)
b=(OUT/'fixture.exe').read_bytes();pe=struct.unpack_from('<I',b,0x3c)[0];n=struct.unpack_from('<H',b,pe+6)[0];opt=pe+24;sections=opt+struct.unpack_from('<H',b,pe+20)[0];size=struct.unpack_from('<I',b,opt+56)[0]
def off(rva):
 for i in range(n):
  vs,va,rs,raw=struct.unpack_from('<IIII',b,sections+40*i+8)
  if va<=rva<va+max(vs,rs):return raw+rva-va
 raise ValueError(rva)
e=off(struct.unpack_from('<I',b,opt+112)[0]);count,functions,namesr,ordinals=struct.unpack_from('<IIII',b,e+24);exports={}
for i in range(count):
 nr=struct.unpack_from('<I',b,off(namesr)+i*4)[0];st=off(nr);name=b[st:b.index(0,st)].decode();ordinal=struct.unpack_from('<H',b,off(ordinals)+i*2)[0];exports[name]=struct.unpack_from('<I',b,off(functions)+ordinal*4)[0]
hooks=[]
for name in ['outer']+names:
 rva=exports[name];hooks.append({'name':name,'rva':rva,'bytes':b[off(rva):off(rva)+len(prefixes[name])//2].hex(),'callerRva':exports['open_cont' if name=='outer' else name+'_cont']})
assert all(h['bytes']==prefixes[h['name']] for h in hooks)
meta={'exe':str(OUT/'fixture.exe'),'size':size,'sha256':hashlib.sha256(b).hexdigest(),'hooks':hooks,'exports':exports,'sourceHashes':{n:hashlib.sha256((OUT/n).read_bytes()).hexdigest() for n in ['fixture.c','fixture.asm']}}
(OUT/'metadata.json').write_text(json.dumps(meta,indent=2));print(json.dumps({'outcome':'BUILT','exe':meta['exe'],'sha256':meta['sha256']}))
