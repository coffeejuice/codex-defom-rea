Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EBB770: mov         qword ptr [rsp+10h],rdx
  0000000140EBB775: mov         qword ptr [rsp+8],rcx
  0000000140EBB77A: sub         rsp,68h
  0000000140EBB77E: mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
  0000000140EBB787: mov         rcx,qword ptr [rsp+70h]
  0000000140EBB78C: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBB793: mov         rcx,qword ptr [rcx+0000000000001718h]
  0000000140EBB79A: call        qword ptr [0000000141E06730h]
  0000000140EBB7A0: mov         dword ptr [rsp+20h],eax
  0000000140EBB7A4: mov         rcx,qword ptr [rsp+78h]
  0000000140EBB7A9: mov         eax,dword ptr [rsp+20h]
  0000000140EBB7AD: mov         dword ptr [rcx+0000000000000160h],eax
  0000000140EBB7B3: mov         rcx,qword ptr [rsp+70h]
  0000000140EBB7B8: call        0000000140130AF0
  0000000140EBB7BD: mov         qword ptr [rsp+28h],rax
  0000000140EBB7C2: lea         rcx,[0000000143D766B8h]
  0000000140EBB7C9: call        00000001402CB7A0
  0000000140EBB7CE: cmp         eax,5
  0000000140EBB7D1: jne         0000000140EBB895
  0000000140EBB7D7: cmp         qword ptr [rsp+28h],0
  0000000140EBB7DD: je          0000000140EBB895
  0000000140EBB7E3: mov         rcx,qword ptr [rsp+28h]
  0000000140EBB7E8: call        000000014005F230
  0000000140EBB7ED: movzx       eax,al
  0000000140EBB7F0: test        eax,eax
  0000000140EBB7F2: je          0000000140EBB813
  0000000140EBB7F4: mov         dl,1
  0000000140EBB7F6: mov         rcx,qword ptr [rsp+28h]
  0000000140EBB7FB: call        000000014005A2C0
  0000000140EBB800: mov         edx,dword ptr [rsp+20h]
  0000000140EBB804: mov         rcx,qword ptr [rsp+28h]
  0000000140EBB809: call        000000014005A350
  0000000140EBB80E: jmp         0000000140EBB895
  0000000140EBB813: mov         rcx,qword ptr [rsp+28h]
  0000000140EBB818: call        0000000140054E90
  0000000140EBB81D: mov         qword ptr [rsp+30h],rax
  0000000140EBB822: cmp         qword ptr [rsp+30h],0
  0000000140EBB828: je          0000000140EBB889
  0000000140EBB82A: mov         rax,qword ptr [rsp+30h]
  0000000140EBB82F: mov         rax,qword ptr [rax]
  0000000140EBB832: mov         rcx,qword ptr [rsp+30h]
  0000000140EBB837: call        qword ptr [rax+0000000000000200h]
  0000000140EBB83D: movzx       eax,al
  0000000140EBB840: test        eax,eax
  0000000140EBB842: je          0000000140EBB889
  0000000140EBB844: mov         rcx,qword ptr [rsp+70h]
  0000000140EBB849: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBB850: mov         rcx,qword ptr [rcx+00000000000016F0h]
  0000000140EBB857: call        qword ptr [0000000141E05D68h]
  0000000140EBB85D: mov         byte ptr [rsp+38h],al
  0000000140EBB861: movzx       edx,byte ptr [rsp+38h]
  0000000140EBB866: mov         rcx,qword ptr [rsp+28h]
  0000000140EBB86B: call        000000014005A2C0
  0000000140EBB870: movzx       eax,byte ptr [rsp+38h]
  0000000140EBB875: test        eax,eax
  0000000140EBB877: je          0000000140EBB887
  0000000140EBB879: mov         edx,dword ptr [rsp+20h]
  0000000140EBB87D: mov         rcx,qword ptr [rsp+28h]
  0000000140EBB882: call        000000014005A350
  0000000140EBB887: jmp         0000000140EBB895
  0000000140EBB889: xor         edx,edx
  0000000140EBB88B: mov         rcx,qword ptr [rsp+28h]
  0000000140EBB890: call        000000014005A2C0
  0000000140EBB895: mov         rcx,qword ptr [rsp+70h]
  0000000140EBB89A: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBB8A1: mov         rcx,qword ptr [rcx+0000000000001740h]
  0000000140EBB8A8: call        qword ptr [0000000141E06730h]
  0000000140EBB8AE: mov         r11d,eax
  0000000140EBB8B1: mov         rax,qword ptr [rsp+78h]
  0000000140EBB8B6: mov         dword ptr [rax+000000000000016Ch],r11d
  0000000140EBB8BD: mov         rcx,qword ptr [rsp+70h]
  0000000140EBB8C2: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBB8C9: mov         rcx,qword ptr [rcx+0000000000001750h]
  0000000140EBB8D0: call        qword ptr [0000000141E06730h]
  0000000140EBB8D6: mov         r11d,eax
  0000000140EBB8D9: mov         rax,qword ptr [rsp+78h]
  0000000140EBB8DE: mov         dword ptr [rax+0000000000000164h],r11d
  0000000140EBB8E5: mov         rcx,qword ptr [rsp+70h]
  0000000140EBB8EA: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EBB8F1: lea         rdx,[rsp+40h]
  0000000140EBB8F6: mov         rcx,qword ptr [rcx+00000000000016C8h]
  0000000140EBB8FD: call        qword ptr [0000000141E0B4A8h]
  0000000140EBB903: mov         qword ptr [rsp+50h],rax
  0000000140EBB908: mov         rax,qword ptr [rsp+50h]
  0000000140EBB90D: mov         qword ptr [rsp+58h],rax
  0000000140EBB912: mov         rdx,qword ptr [rsp+58h]
  0000000140EBB917: mov         rcx,qword ptr [rsp+70h]
  0000000140EBB91C: call        0000000140EC9980
  0000000140EBB921: mov         r11d,eax
  0000000140EBB924: mov         rax,qword ptr [rsp+78h]
  0000000140EBB929: mov         dword ptr [rax+0000000000000168h],r11d
  0000000140EBB930: lea         rcx,[rsp+40h]
  0000000140EBB935: call        qword ptr [0000000141E059E0h]
  0000000140EBB93B: add         rsp,68h
  0000000140EBB93F: ret
  0000000140EBB940: int         3

  Summary

     1E02000 .text
