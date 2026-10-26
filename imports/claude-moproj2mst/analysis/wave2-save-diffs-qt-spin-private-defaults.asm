Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653FE140: mov         qword ptr [rsp+8],rcx
  00000000653FE145: push        rdi
  00000000653FE146: sub         rsp,40h
  00000000653FE14A: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  00000000653FE153: mov         qword ptr [rsp+58h],rbx
  00000000653FE158: mov         rdi,rcx
  00000000653FE15B: call        0000000065375EE0
  00000000653FE160: nop
  00000000653FE161: lea         rax,[000000006570A4E8h]
  00000000653FE168: mov         qword ptr [rdi],rax
  00000000653FE16B: xor         edx,edx
  00000000653FE16D: lea         rcx,[rsp+28h]
  00000000653FE172: call        qword ptr [0000000065697258h]
  00000000653FE178: nop
  00000000653FE179: mov         rdx,rax
  00000000653FE17C: lea         rcx,[rdi+0000000000000210h]
  00000000653FE183: call        qword ptr [0000000065696790h]
  00000000653FE189: nop
  00000000653FE18A: lea         rcx,[rsp+28h]
  00000000653FE18F: call        qword ptr [00000000656966E0h]
  00000000653FE195: mov         edx,63h
  00000000653FE19A: lea         rcx,[rsp+28h]
  00000000653FE19F: call        qword ptr [0000000065697258h]
  00000000653FE1A5: nop
  00000000653FE1A6: lea         rcx,[rdi+0000000000000220h]
  00000000653FE1AD: mov         rdx,rax
  00000000653FE1B0: call        qword ptr [0000000065696790h]
  00000000653FE1B6: nop
  00000000653FE1B7: lea         rcx,[rsp+28h]
  00000000653FE1BC: call        qword ptr [00000000656966E0h]
  00000000653FE1C2: lea         rcx,[rdi+0000000000000200h]
  00000000653FE1C9: lea         rdx,[rdi+0000000000000210h]
  00000000653FE1D0: call        qword ptr [0000000065696790h]
  00000000653FE1D6: mov         edx,1
  00000000653FE1DB: lea         rcx,[rsp+28h]
  00000000653FE1E0: call        qword ptr [0000000065697258h]
  00000000653FE1E6: nop
  00000000653FE1E7: lea         rcx,[rdi+0000000000000230h]
  00000000653FE1EE: mov         rdx,rax
  00000000653FE1F1: call        qword ptr [0000000065696790h]
  00000000653FE1F7: nop
  00000000653FE1F8: lea         rcx,[rsp+28h]
  00000000653FE1FD: call        qword ptr [00000000656966E0h]
  00000000653FE203: mov         dword ptr [rdi+0000000000000240h],2
  00000000653FE20D: mov         rax,rdi
  00000000653FE210: mov         rbx,qword ptr [rsp+58h]
  00000000653FE215: add         rsp,40h
  00000000653FE219: pop         rdi
  00000000653FE21A: ret
  00000000653FE21B: int         3
  00000000653FE21C: int         3
  00000000653FE21D: int         3
  00000000653FE21E: int         3
  00000000653FE21F: int         3
  00000000653FE220: cmp         edx,2
  00000000653FE223: je          00000000653FE2AF
  00000000653FE229: push        rdi
  00000000653FE22A: sub         rsp,30h
  00000000653FE22E: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  00000000653FE237: mov         qword ptr [rsp+48h],rbx
  00000000653FE23C: mov         rbx,rcx
  00000000653FE23F: mov         rdi,qword ptr [rcx+8]
  00000000653FE243: and         dword ptr [rcx+000000000000028Ch],0FFFFFFFEh
  00000000653FE24A: cmp         edx,1
  00000000653FE24D: je          00000000653FE263
  00000000653FE24F: add         rcx,200h
  00000000653FE256: mov         rdx,r8
  00000000653FE259: call        qword ptr [0000000065698918h]
  00000000653FE25F: test        al,al

  Summary

      695000 .text
