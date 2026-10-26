"""Build fixed harmless twelve-prologue native fixture; never DEFORM."""
from pathlib import Path
import hashlib,json,os,struct,subprocess
ROOT=Path(__file__).resolve().parent.parent;BASE=ROOT/'scratch/cp214p02';OUT=ROOT/'scratch/cp215v01'
prior=json.loads((BASE/'metadata.json').read_text())
for n,v in prior['sourceHashes'].items():assert hashlib.sha256((BASE/n).read_bytes()).hexdigest()==v
OUT.mkdir(parents=True,exist_ok=False)
VC=Path(r'C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Auxiliary/Build/vcvars64.bat')
r=subprocess.run(f'cmd.exe /d /s /c ""{VC}" >nul && set"',capture_output=True,text=True,check=True);env=dict(os.environ)
for line in r.stdout.splitlines():
 k,sep,v=line.partition('=')
 if sep and k:
  for old in list(env):
   if old.lower()==k.lower():del env[old]
  env[k]=v
code=(BASE/'fixture.c').read_text().replace('p214a','p215a')
code=code.replace('unsigned char g_main[0x2000];','unsigned char g_main[0x2000];\nunsigned char g_project[0x100];\nuintptr_t g_view[8],g_vtable[64];\nunsigned char g_model[0x100];\nextern void modelbind(void);')
code=code.replace(' *(void**)(g_main+0x18d8)=&g_header;', ' *(void**)(g_main+0x18d8)=&g_header;\n g_project[0x55]=1;g_view[0]=(uintptr_t)g_vtable;g_vtable[55]=(uintptr_t)modelbind;')
code=code.replace('value!=0x100','value!=(uintptr_t)g_project')
asm=(BASE/'fixture.asm').read_text()
asm='PUBLIC viewbind_cont,viewaccess_cont,modelbind_cont,viewcurrent_cont\nEXTERN g_project:BYTE\nEXTERN g_view:QWORD\nEXTERN g_model:BYTE\n'+asm
asm=asm.replace('mov eax,100h','lea rax,g_project').replace('mov ecx,100h','lea rcx,g_project')
asm=asm.replace('cmp g_fail,8','cmp g_fail,10').replace('cmp g_fail,9','cmp g_fail,10')
asm=asm.replace('activation_cont::\npostcore_done:', 'activation_cont::\nlea rcx,g_main\nmov edx,1\ncall viewbind\nviewbind_cont::\npostcore_done:')
prefixes={h['name']:h['bytes'] for h in prior['hooks']}
prefixes.update(viewbind='8854241048894c24084881ecc8000000',viewaccess='48894c24084883ec28',modelbind='488954241048894c2408')
raw=lambda name:'db '+','.join('0'+prefixes[name][i:i+2]+'h' for i in range(0,len(prefixes[name]),2))
extra=['viewbind PROC',raw('viewbind'),'call current','viewcurrent_cont::','cmp g_fail,8','je viewbind_done','lea rcx,g_main','call viewaccess','viewaccess_cont::','mov rcx,rax','lea rdx,g_model+10h','call modelbind','modelbind_cont::','viewbind_done:','add rsp,0c8h','ret','viewbind ENDP',
 'viewaccess PROC',raw('viewaccess'),'lea rax,g_view','add rsp,28h','ret','viewaccess ENDP',
 'modelbind PROC',raw('modelbind'),'sub rsp,38h','cmp g_fail,9','jne modelbind_done','modelbind_loop:','mov ecx,100','call Sleep','jmp modelbind_loop','modelbind_done:','add rsp,38h','ret','modelbind ENDP']
asm=asm.replace('\nEND\n','\n'+'\n'.join(extra)+'\nEND\n')
(OUT/'fixture.c').write_text(code);(OUT/'fixture.asm').write_text(asm)
names=['duplicate','factory','cast','project','acceptance','postopen','current','activation','viewbind','viewaccess','modelbind']
bin=Path(env['VCToolsInstallDir'])/'bin/Hostx64/x64';exportsWanted=['synthetic_open','outer','open_cont','viewcurrent_cont']+names+[n+'_cont' for n in names]
commands=[[str(bin/'ml64.exe'),'/nologo','/c','/Fo'+str(OUT/'hooks.obj'),str(OUT/'fixture.asm')],[str(bin/'cl.exe'),'/nologo','/Od','/MT',str(OUT/'fixture.c'),str(OUT/'hooks.obj'),'/Fo'+str(OUT/'main.obj'),'/Fe'+str(OUT/'fixture.exe'),'/link','/INCREMENTAL:NO',*['/EXPORT:'+n for n in exportsWanted]]]
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
