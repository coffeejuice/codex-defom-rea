Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C62C0: mov         qword ptr [rsp+10h],rbx
  00000000670C62C5: push        rdi
  00000000670C62C6: sub         rsp,20h
  00000000670C62CA: mov         rbx,rcx
  00000000670C62CD: mov         rcx,qword ptr [rcx+8]
  00000000670C62D1: mov         rdi,rdx
  00000000670C62D4: cmp         qword ptr [rcx+28h],0
  00000000670C62D9: jne         00000000670C62FB
  00000000670C62DB: cmp         qword ptr [rcx],0
  00000000670C62DF: jne         00000000670C62FB
  00000000670C62E1: lea         rcx,[000000006722C3C8h]
  00000000670C62E8: call        000000006701CEE0
  00000000670C62ED: mov         rax,rbx
  00000000670C62F0: mov         rbx,qword ptr [rsp+38h]
  00000000670C62F5: add         rsp,20h
  00000000670C62F9: pop         rdi
  00000000670C62FA: ret
  00000000670C62FB: lea         rdx,[rsp+30h]
  00000000670C6300: call        00000000670C59B0
  00000000670C6305: test        al,al
  00000000670C6307: je          00000000670C6321
  00000000670C6309: movsd       xmm0,mmword ptr [rsp+30h]
  00000000670C630F: mov         rax,rbx
  00000000670C6312: movsd       mmword ptr [rdi],xmm0
  00000000670C6316: mov         rbx,qword ptr [rsp+38h]
  00000000670C631B: add         rsp,20h
  00000000670C631F: pop         rdi
  00000000670C6320: ret
  00000000670C6321: mov         rcx,rbx
  00000000670C6324: mov         qword ptr [rdi],0
  00000000670C632B: call        00000000670C2660
  00000000670C6330: neg         al
  00000000670C6332: mov         rax,qword ptr [rbx+8]
  00000000670C6336: sbb         ecx,ecx
  00000000670C6338: add         ecx,2
  00000000670C633B: cmp         dword ptr [rax+00000000000000CCh],0
  00000000670C6342: jne         00000000670C634A
  00000000670C6344: mov         dword ptr [rax+00000000000000CCh],ecx
  00000000670C634A: mov         rax,rbx
  00000000670C634D: mov         rbx,qword ptr [rsp+38h]
  00000000670C6352: add         rsp,20h
  00000000670C6356: pop         rdi
  00000000670C6357: ret
  00000000670C6358: int         3
  00000000670C6359: int         3
  00000000670C635A: int         3
  00000000670C635B: int         3
  00000000670C635C: int         3
  00000000670C635D: int         3
  00000000670C635E: int         3
  00000000670C635F: int         3
  00000000670C6360: mov         qword ptr [rsp+10h],rdx
  00000000670C6365: mov         qword ptr [rsp+8],rcx
  00000000670C636A: push        rbx
  00000000670C636B: push        rbp
  00000000670C636C: push        rsi
  00000000670C636D: push        rdi
  00000000670C636E: push        r12
  00000000670C6370: push        r13
  00000000670C6372: push        r14
  00000000670C6374: push        r15
  00000000670C6376: sub         rsp,48h
  00000000670C637A: mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
  00000000670C6383: mov         rax,rdx
  00000000670C6386: mov         rbx,rcx
  00000000670C6389: mov         rdi,qword ptr [rcx+8]
  00000000670C638D: cmp         qword ptr [rdi+28h],0
  00000000670C6392: jne         00000000670C63AE
  00000000670C6394: cmp         qword ptr [rdi],0
  00000000670C6398: jne         00000000670C63AE
  00000000670C639A: lea         rcx,[000000006722C3C8h]
  00000000670C63A1: call        000000006701CEE0
  00000000670C63A6: mov         rax,rbx
  00000000670C63A9: jmp         00000000670C6696
  00000000670C63AE: lea         rcx,[00000000672F0B30h]
  00000000670C63B5: cmp         qword ptr [rdx],rcx
  00000000670C63B8: je          00000000670C63EA
  00000000670C63BA: mov         qword ptr [rsp+28h],rcx
  00000000670C63BF: lock add    dword ptr [00000000672F0B30h],1
  00000000670C63C7: lea         rdx,[rsp+28h]
  00000000670C63CC: mov         rcx,rax
  00000000670C63CF: call        00000000670703D0
  00000000670C63D4: nop
  00000000670C63D5: mov         rax,qword ptr [rsp+28h]
  00000000670C63DA: lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C63DE: jne         00000000670C63EA
  00000000670C63E0: 48

  Summary

      1A7000 .text
