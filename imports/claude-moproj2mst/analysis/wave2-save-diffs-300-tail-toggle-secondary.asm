Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ECD430: mov         byte ptr [rsp+10h],dl
  0000000140ECD434: mov         qword ptr [rsp+8],rcx
  0000000140ECD439: sub         rsp,38h
  0000000140ECD43D: movzx       eax,byte ptr [rsp+48h]
  0000000140ECD442: test        eax,eax
  0000000140ECD444: je          0000000140ECD4F7
  0000000140ECD44A: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD44F: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ECD456: xor         edx,edx
  0000000140ECD458: mov         rcx,qword ptr [rcx+00000000000009C0h]
  0000000140ECD45F: call        qword ptr [0000000141E08BB8h]
  0000000140ECD465: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD46A: call        0000000140130EE0
  0000000140ECD46F: movzx       eax,al
  0000000140ECD472: test        eax,eax
  0000000140ECD474: je          0000000140ECD49B
  0000000140ECD476: mov         rax,qword ptr [rsp+40h]
  0000000140ECD47B: mov         rax,qword ptr [rax+0000000000000150h]
  0000000140ECD482: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD487: mov         rcx,qword ptr [rcx+0000000000000150h]
  0000000140ECD48E: mov         rax,qword ptr [rax]
  0000000140ECD491: mov         edx,2
  0000000140ECD496: call        qword ptr [rax+58h]
  0000000140ECD499: jmp         0000000140ECD4E1
  0000000140ECD49B: mov         rax,qword ptr [rsp+40h]
  0000000140ECD4A0: mov         rax,qword ptr [rax+0000000000000150h]
  0000000140ECD4A7: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD4AC: mov         rcx,qword ptr [rcx+0000000000000150h]
  0000000140ECD4B3: mov         rax,qword ptr [rax]
  0000000140ECD4B6: mov         edx,8
  0000000140ECD4BB: call        qword ptr [rax+58h]
  0000000140ECD4BE: mov         rax,qword ptr [rsp+40h]
  0000000140ECD4C3: mov         rax,qword ptr [rax+0000000000000150h]
  0000000140ECD4CA: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD4CF: mov         rcx,qword ptr [rcx+0000000000000150h]
  0000000140ECD4D6: mov         rax,qword ptr [rax]
  0000000140ECD4D9: mov         edx,4
  0000000140ECD4DE: call        qword ptr [rax+60h]
  0000000140ECD4E1: mov         byte ptr [rsp+20h],1
  0000000140ECD4E6: lea         rdx,[rsp+20h]
  0000000140ECD4EB: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD4F0: call        0000000140EC0A00
  0000000140ECD4F5: jmp         0000000140ECD52B
  0000000140ECD4F7: mov         byte ptr [rsp+21h],0
  0000000140ECD4FC: lea         rdx,[rsp+21h]
  0000000140ECD501: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD506: call        0000000140EC0A00
  0000000140ECD50B: mov         rax,qword ptr [rsp+40h]
  0000000140ECD510: mov         rax,qword ptr [rax+0000000000000150h]
  0000000140ECD517: mov         rcx,qword ptr [rsp+40h]
  0000000140ECD51C: mov         rcx,qword ptr [rcx+0000000000000150h]
  0000000140ECD523: mov         rax,qword ptr [rax]
  0000000140ECD526: xor         edx,edx
  0000000140ECD528: call        qword ptr [rax+68h]
  0000000140ECD52B: add         rsp,38h
  0000000140ECD52F: ret
  0000000140ECD530: int         3

  Summary

     1E02000 .text
