Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654264F0: mov         rax,rsp
  00000000654264F3: push        rsi
  00000000654264F4: push        rdi
  00000000654264F5: push        r12
  00000000654264F7: sub         rsp,80h
  00000000654264FE: mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
  0000000065426507: mov         qword ptr [rax+10h],rbx
  000000006542650B: mov         qword ptr [rax+18h],rbp
  000000006542650F: movaps      xmmword ptr [rax-28h],xmm6
  0000000065426513: movaps      xmmword ptr [rax-38h],xmm7
  0000000065426517: mov         rbx,r9
  000000006542651A: mov         edi,r8d
  000000006542651D: mov         rsi,rdx
  0000000065426520: mov         r12,rcx
  0000000065426523: mov         rbp,qword ptr [rcx+8]
  0000000065426527: lea         rcx,[rax-60h]
  000000006542652B: call        qword ptr [0000000065696F68h]
  0000000065426531: nop
  0000000065426532: mov         rcx,rbx
  0000000065426535: call        qword ptr [0000000065698EE0h]
  000000006542653B: mov         rcx,rax
  000000006542653E: mov         eax,dword ptr [rbp+20h]
  0000000065426541: mov         dword ptr [rsp+20h],eax
  0000000065426545: lea         r9,[rsp+38h]
  000000006542654A: mov         r8d,edi
  000000006542654D: mov         rdx,rsi
  0000000065426550: call        qword ptr [0000000065698ED8h]
  0000000065426556: test        al,al
  0000000065426558: jne         000000006542656C
  000000006542655A: lea         rcx,[rsp+38h]
  000000006542655F: call        qword ptr [0000000065696A50h]
  0000000065426565: xor         eax,eax
  0000000065426567: jmp         000000006542676D
  000000006542656C: lea         rcx,[rsp+38h]
  0000000065426571: call        qword ptr [0000000065696AC8h]
  0000000065426577: test        al,al
  0000000065426579: je          0000000065426590
  000000006542657B: lea         rcx,[rsp+38h]
  0000000065426580: call        qword ptr [0000000065696A50h]
  0000000065426586: mov         eax,1
  000000006542658B: jmp         000000006542676D
  0000000065426590: movsd       xmm0,mmword ptr [rbp+10h]
  0000000065426595: xorpd       xmm7,xmm7
  0000000065426599: comisd      xmm0,xmm7
  000000006542659D: jb          00000000654265C2
  000000006542659F: mov         dl,2Dh
  00000000654265A1: lea         rcx,[rsp+38h]
  00000000654265A6: call        qword ptr [0000000065698ED0h]
  00000000654265AC: test        al,al
  00000000654265AE: je          00000000654265C2
  00000000654265B0: lea         rcx,[rsp+38h]
  00000000654265B5: call        qword ptr [0000000065696A50h]
  00000000654265BB: xor         eax,eax
  00000000654265BD: jmp         000000006542676D
  00000000654265C2: comisd      xmm7,mmword ptr [rbp+18h]
  00000000654265C7: jbe         00000000654265EC
  00000000654265C9: mov         dl,2Bh
  00000000654265CB: lea         rcx,[rsp+38h]
  00000000654265D0: call        qword ptr [0000000065698ED0h]
  00000000654265D6: test        al,al
  00000000654265D8: je          00000000654265EC
  00000000654265DA: lea         rcx,[rsp+38h]
  00000000654265DF: call        qword ptr [0000000065696A50h]
  00000000654265E5: xor         eax,eax
  00000000654265E7: jmp         000000006542676D
  00000000654265EC: lea         rcx,[rsp+38h]
  00000000654265F1: call        qword ptr [0000000065696A58h]
  00000000654265F7: mov         rcx,rax
  00000000654265FA: lea         r8,[rsp+00000000000000A0h]
  0000000065426602: lea         rdx,[rsp+30h]
  0000000065426607: call        qword ptr [0000000065698EC8h]
  000000006542660D: movapd      xmm6,xmm0
  0000000065426611: cmp         byte ptr [rsp+00000000000000A0h],0
  0000000065426619: je          000000006542662D
  000000006542661B: lea         rcx,[rsp+38h]
  0000000065426620: call        qword ptr [0000000065696A50h]
  0000000065426626: xor         eax,eax
  0000000065426628: jmp         000000006542676D
  000000006542662D: cmp         byte ptr [rsp+30h],0
  0000000065426632: jne         0000000065426649
  0000000065426634: lea         rcx,[rsp+38h]
  0000000065426639: call        qword ptr [0000000065696A50h]
  000000006542663F: mov         eax,1
  0000000065426644: jmp         000000006542676D
  0000000065426649: comisd      xmm0,mmword ptr [rbp+10h]
  000000006542664E: jb          0000000065426670
  0000000065426650: movsd       xmm0,mmword ptr [rbp+18h]
  0000000065426655: comisd      xmm0,xmm6
  0000000065426659: jb          0000000065426670
  000000006542665B: lea         rcx,[rsp+38h]
  0000000065426660: call        qword ptr [0000000065696A50h]
  0000000065426666: mov         eax,2
  000000006542666B: jmp         000000006542676D
  0000000065426670: cmp         dword ptr [r12+0000000000000090h],0
  0000000065426679: jne         000000006542675D
  000000006542667F: movsd       xmm1,mmword ptr [rbp+18h]
  0000000065426684: comisd      xmm1,xmm7
  0000000065426688: jb          0000000065426692
  000000006542668A: movsd       mmword ptr [rsp+40h],xmm1
  0000000065426690: jmp         00000000654266A0
  0000000065426692: xorpd       xmm1,xmmword ptr [0000000065699FC0h]
  000000006542669A: movsd       mmword ptr [rsp+40h],xmm1
  00000000654266A0: movsd       xmm0,mmword ptr [rbp+10h]
  00000000654266A5: comisd      xmm0,xmm7
  00000000654266A9: jb          00000000654266B3
  00000000654266AB: movsd       mmword ptr [rsp+48h],xmm0
  00000000654266B1: jmp         00000000654266C1
  00000000654266B3: xorpd       xmm0,xmmword ptr [0000000065699FC0h]
  00000000654266BB: movsd       mmword ptr [rsp+48h],xmm0
  00000000654266C1: comisd      xmm1,xmm0
  00000000654266C5: jbe         00000000654266CE
  00000000654266C7: lea         rax,[rsp+40h]
  00000000654266CC: jmp         00000000654266D3
  00000000654266CE: lea         rax,[rsp+48h]
  00000000654266D3: movsd       xmm1,mmword ptr [rax]
  00000000654266D7: movsd       xmm0,mmword ptr [00000000657124B0h]
  00000000654266DF: comisd      xmm0,xmm1
  00000000654266E3: jbe         000000006542675D
  00000000654266E5: cvttsd2si   rax,xmm1
  00000000654266EA: test        rax,rax
  00000000654266ED: jne         00000000654266F6
  00000000654266EF: mov         eax,1
  00000000654266F4: jmp         000000006542670A
  00000000654266F6: pxor        xmm0,xmm0
  00000000654266FA: cvtsi2sd    xmm0,rax
  00000000654266FF: call        00000000655E13F4
  0000000065426704: cvttsd2si   eax,xmm0
  0000000065426708: inc         eax
  000000006542670A: mov         ecx,1
  000000006542670F: test        eax,eax
  0000000065426711: jle         000000006542672E
  0000000065426713: mov         edx,eax
  0000000065426715: nop         word ptr [rax+rax+0000000000000000h]
  0000000065426720: lea         rax,[rcx+rcx*4]
  0000000065426724: lea         rcx,[rax+rax]
  0000000065426728: sub         rdx,1
  000000006542672C: jne         0000000065426720
  000000006542672E: dec         rcx
  0000000065426731: comisd      xmm6,xmm7
  0000000065426735: jae         000000006542673F
  0000000065426737: xorpd       xmm6,xmmword ptr [0000000065699FC0h]
  000000006542673F: pxor        xmm0,xmm0
  0000000065426743: cvtsi2sd    xmm0,rcx
  0000000065426748: comisd      xmm6,xmm0
  000000006542674C: jbe         000000006542675D
  000000006542674E: lea         rcx,[rsp+38h]
  0000000065426753: call        qword ptr [0000000065696A50h]
  0000000065426759: xor         eax,eax
  000000006542675B: jmp         000000006542676D
  000000006542675D: lea         rcx,[rsp+38h]
  0000000065426762: call        qword ptr [0000000065696A50h]
  0000000065426768: mov         eax,1
  000000006542676D: lea         r11,[rsp+0000000000000080h]
  0000000065426775: mov         rbx,qword ptr [r11+28h]
  0000000065426779: mov         rbp,qword ptr [r11+30h]
  000000006542677D: movaps      xmm6,xmmword ptr [rsp+70h]
  0000000065426782: movaps      xmm7,xmmword ptr [rsp+60h]
  0000000065426787: mov         rsp,r11
  000000006542678A: pop         r12
  000000006542678C: pop         rdi
  000000006542678D: pop         rsi
  000000006542678E: ret
  000000006542678F: int         3
  0000000065426790: push        rbx
  0000000065426792: sub         rsp,20h
  0000000065426796: mov         rbx,rdx
  0000000065426799: mov         rdx,qword ptr [rcx+8]
  000000006542679D: add         rdx,88h
  00000000654267A4: mov         rcx,rbx
  00000000654267A7: call        qword ptr [0000000065696A90h]
  00000000654267AD: mov         rax,rbx
  00000000654267B0: add         rsp,20h
  00000000654267B4: pop         rbx
  00000000654267B5: ret
  00000000654267B6: int         3
  00000000654267B7: int         3
  00000000654267B8: int         3
  00000000654267B9: int         3
  00000000654267BA: int         3
  00000000654267BB: int         3
  00000000654267BC: int         3
  00000000654267BD: int         3
  00000000654267BE: int         3
  00000000654267BF: int         3
  00000000654267C0: mov         rcx,qword ptr [rcx+8]
  00000000654267C4: add         rcx,88h
  00000000654267CB: jmp         qword ptr [0000000065696920h]
  00000000654267D2: int         3
  00000000654267D3: int         3
  00000000654267D4: int         3
  00000000654267D5: int         3
  00000000654267D6: int         3
  00000000654267D7: int         3
  00000000654267D8: int         3
  00000000654267D9: int         3
  00000000654267DA: int         3
  00000000654267DB: int         3
  00000000654267DC: int         3
  00000000654267DD: int         3
  00000000654267DE: int         3
  00000000654267DF: int         3
  00000000654267E0: 40

  Summary

      695000 .text
