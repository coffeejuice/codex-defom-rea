Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014085E050: mov         qword ptr [rsp+10h],rdx
  000000014085E055: mov         qword ptr [rsp+8],rcx
  000000014085E05A: mov         rcx,qword ptr [rsp+8]
  000000014085E05F: mov         rax,qword ptr [rsp+10h]
  000000014085E064: movsd       xmm0,mmword ptr [rax+00000000000000B8h]
  000000014085E06C: movsd       mmword ptr [rcx+00000000000000B8h],xmm0
  000000014085E074: mov         rcx,qword ptr [rsp+8]
  000000014085E079: mov         rax,qword ptr [rsp+10h]
  000000014085E07E: movsd       xmm0,mmword ptr [rax+00000000000000C0h]
  000000014085E086: movsd       mmword ptr [rcx+00000000000000C0h],xmm0
  000000014085E08E: mov         rcx,qword ptr [rsp+8]
  000000014085E093: mov         rax,qword ptr [rsp+10h]
  000000014085E098: movsd       xmm0,mmword ptr [rax+00000000000000D0h]
  000000014085E0A0: movsd       mmword ptr [rcx+00000000000000D0h],xmm0
  000000014085E0A8: mov         rcx,qword ptr [rsp+8]
  000000014085E0AD: mov         rax,qword ptr [rsp+10h]
  000000014085E0B2: movsd       xmm0,mmword ptr [rax+00000000000000C8h]
  000000014085E0BA: movsd       mmword ptr [rcx+00000000000000C8h],xmm0
  000000014085E0C2: ret
  000000014085E0C3: int         3
  000000014085E0C4: int         3
  000000014085E0C5: int         3
  000000014085E0C6: int         3
  000000014085E0C7: int         3
  000000014085E0C8: int         3
  000000014085E0C9: int         3
  000000014085E0CA: int         3
  000000014085E0CB: int         3
  000000014085E0CC: int         3
  000000014085E0CD: int         3
  000000014085E0CE: int         3
  000000014085E0CF: int         3
  000000014085E0D0: mov         qword ptr [rsp+20h],r9
  000000014085E0D5: mov         qword ptr [rsp+18h],r8
  000000014085E0DA: mov         qword ptr [rsp+10h],rdx
  000000014085E0DF: mov         qword ptr [rsp+8],rcx
  000000014085E0E4: sub         rsp,58h
  000000014085E0E8: mov         qword ptr [rsp+30h],0
  000000014085E0F1: mov         rcx,qword ptr [rsp+60h]
  000000014085E0F6: call        000000014085D7A0
  000000014085E0FB: mov         dword ptr [rsp+24h],eax
  000000014085E0FF: mov         dword ptr [rsp+20h],0
  000000014085E107: mov         dword ptr [rsp+28h],0
  000000014085E10F: mov         rcx,qword ptr [rsp+78h]
  000000014085E114: call        000000014035B400
  000000014085E119: movzx       eax,al
  000000014085E11C: test        eax,eax
  000000014085E11E: je          000000014085E2BF
  000000014085E124: mov         dword ptr [rsp+38h],0
  000000014085E12C: jmp         000000014085E139
  000000014085E12E: mov         eax,dword ptr [rsp+38h]
  000000014085E132: add         eax,1
  000000014085E135: mov         dword ptr [rsp+38h],eax
  000000014085E139: mov         eax,dword ptr [rsp+24h]
  000000014085E13D: cmp         dword ptr [rsp+38h],eax
  000000014085E141: jge         000000014085E2BA
  000000014085E147: mov         edx,dword ptr [rsp+38h]
  000000014085E14B: add         edx,1
  000000014085E14E: mov         rcx,qword ptr [rsp+60h]
  000000014085E153: add         rcx,40h
  000000014085E157: call        0000000140A1CE30
  000000014085E15C: mov         qword ptr [rsp+30h],rax
  000000014085E161: cmp         qword ptr [rsp+30h],0
  000000014085E167: jne         000000014085E16B
  000000014085E169: jmp         000000014085E12E
  000000014085E16B: mov         rax,qword ptr [rsp+30h]
  000000014085E170: movsd       xmm0,mmword ptr [rax+0000000000000370h]
  000000014085E178: comisd      xmm0,mmword ptr [00000001421AD1D0h]
  000000014085E180: jbe         000000014085E2B5
  000000014085E186: mov         rax,qword ptr [rsp+30h]
  000000014085E18B: cmp         dword ptr [rax+0000000000000210h],1
  000000014085E192: je          000000014085E1A2
  000000014085E194: mov         rax,qword ptr [rsp+30h]
  000000014085E199: cmp         dword ptr [rax+0000000000000210h],2
  000000014085E1A0: jne         000000014085E1C9
  000000014085E1A2: mov         rax,qword ptr [rsp+68h]
  000000014085E1A7: mov         ecx,dword ptr [rax]
  000000014085E1A9: add         ecx,1
  000000014085E1AC: mov         rax,qword ptr [rsp+68h]
  000000014085E1B1: mov         dword ptr [rax],ecx
  000000014085E1B3: mov         rax,qword ptr [rsp+70h]
  000000014085E1B8: mov         ecx,dword ptr [rax]
  000000014085E1BA: add         ecx,8
  000000014085E1BD: mov         rax,qword ptr [rsp+70h]
  000000014085E1C2: mov         dword ptr [rax],ecx
  000000014085E1C4: jmp         000000014085E2B5
  000000014085E1C9: mov         rax,qword ptr [rsp+30h]
  000000014085E1CE: cmp         dword ptr [rax+0000000000000210h],3
  000000014085E1D5: jne         000000014085E261
  000000014085E1DB: mov         rax,qword ptr [rsp+68h]
  000000014085E1E0: mov         ecx,dword ptr [rax]
  000000014085E1E2: add         ecx,1
  000000014085E1E5: mov         rax,qword ptr [rsp+68h]
  000000014085E1EA: mov         dword ptr [rax],ecx
  000000014085E1EC: mov         rcx,qword ptr [rsp+30h]
  000000014085E1F1: add         rcx,220h
  000000014085E1F8: call        00000001400C34D0
  000000014085E1FD: mov         ecx,eax
  000000014085E1FF: mov         rax,qword ptr [rsp+70h]
  000000014085E204: add         ecx,dword ptr [rax]
  000000014085E206: mov         rax,qword ptr [rsp+70h]
  000000014085E20B: mov         dword ptr [rax],ecx
  000000014085E20D: mov         rcx,qword ptr [rsp+30h]
  000000014085E212: add         rcx,220h
  000000014085E219: call        00000001400C3500
  000000014085E21E: shl         eax,2
  000000014085E221: mov         dword ptr [rsp+20h],eax
  000000014085E225: mov         eax,dword ptr [rsp+20h]
  000000014085E229: cdq
  000000014085E22A: mov         ecx,3
  000000014085E22F: idiv        eax,ecx
  000000014085E231: mov         dword ptr [rsp+28h],eax
  000000014085E235: mov         eax,dword ptr [rsp+28h]
  000000014085E239: imul        eax,eax,3
  000000014085E23C: cmp         eax,dword ptr [rsp+20h]
  000000014085E240: jge         000000014085E24D
  000000014085E242: mov         eax,dword ptr [rsp+28h]
  000000014085E246: add         eax,1
  000000014085E249: mov         dword ptr [rsp+28h],eax
  000000014085E24D: mov         rax,qword ptr [rsp+70h]
  000000014085E252: mov         ecx,dword ptr [rax]
  000000014085E254: add         ecx,dword ptr [rsp+28h]
  000000014085E258: mov         rax,qword ptr [rsp+70h]
  000000014085E25D: mov         dword ptr [rax],ecx
  000000014085E25F: jmp         000000014085E2B5
  000000014085E261: mov         rax,qword ptr [rsp+30h]
  000000014085E266: cmp         dword ptr [rax+0000000000000210h],0Bh
  000000014085E26D: je          000000014085E28B
  000000014085E26F: 48
  000000014085E270: 8B

  Summary

     1E02000 .text
