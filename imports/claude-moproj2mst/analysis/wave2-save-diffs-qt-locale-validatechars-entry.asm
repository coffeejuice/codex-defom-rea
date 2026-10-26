Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704A960: mov         qword ptr [rsp+20h],r9
  000000006704A965: mov         qword ptr [rsp+10h],rdx
  000000006704A96A: mov         qword ptr [rsp+8],rcx
  000000006704A96F: push        rbx
  000000006704A970: push        rbp
  000000006704A971: push        rsi
  000000006704A972: push        rdi
  000000006704A973: push        r12
  000000006704A975: push        r13
  000000006704A977: push        r14
  000000006704A979: push        r15
  000000006704A97B: sub         rsp,38h
  000000006704A97F: mov         rcx,r9
  000000006704A982: mov         rbx,r9
  000000006704A985: mov         r15d,r8d
  000000006704A988: mov         r12,rdx
  000000006704A98B: call        000000006702D0C0
  000000006704A990: mov         rax,qword ptr [rbx]
  000000006704A993: mov         r10d,dword ptr [rax]
  000000006704A996: mov         r11,qword ptr [r12]
  000000006704A99A: mov         edx,dword ptr [r11+8]
  000000006704A99E: cmp         r10d,1
  000000006704A9A2: jne         000000006704A9AC
  000000006704A9A4: mov         rax,qword ptr [rbx]
  000000006704A9A7: cmp         edx,dword ptr [rax+4]
  000000006704A9AA: jle         000000006704A9B4
  000000006704A9AC: mov         rcx,rbx
  000000006704A9AF: call        000000006702D710
  000000006704A9B4: mov         rax,qword ptr [r12]
  000000006704A9B8: cmp         r15d,2
  000000006704A9BC: sete        byte ptr [rsp+0000000000000090h]
  000000006704A9C4: xor         r13d,r13d
  000000006704A9C7: xor         dil,dil
  000000006704A9CA: xor         bl,bl
  000000006704A9CC: xor         bpl,bpl
  000000006704A9CF: cmp         dword ptr [rax+8],r13d
  000000006704A9D3: mov         esi,r13d
  000000006704A9D6: mov         r14d,r13d
  000000006704A9D9: mov         dword ptr [rsp+24h],r13d
  000000006704A9DE: mov         r12,r13
  000000006704A9E1: jle         000000006704AAE5
  000000006704A9E7: nop         word ptr [rax+rax+0000000000000000h]
  000000006704A9F0: mov         rax,qword ptr [rax+10h]
  000000006704A9F4: lea         rdx,[rsp+20h]
  000000006704A9F9: movzx       ecx,word ptr [rax+r12*2]
  000000006704A9FE: mov         word ptr [rsp+20h],cx
  000000006704AA03: mov         rcx,qword ptr [rsp+0000000000000080h]
  000000006704AA0B: call        000000006702D210
  000000006704AA10: movzx       edx,al
  000000006704AA13: lea         ecx,[rdx-30h]
  000000006704AA16: cmp         cl,9
  000000006704AA19: ja          000000006704AA3D
  000000006704AA1B: test        r15d,r15d
  000000006704AA1E: je          000000006704AA39
  000000006704AA20: test        bpl,bpl
  000000006704AA23: je          000000006704AA39
  000000006704AA25: mov         eax,dword ptr [rsp+00000000000000A0h]
  000000006704AA2C: cmp         eax,0FFFFFFFFh
  000000006704AA2F: je          000000006704AA39
  000000006704AA31: inc         r14d
  000000006704AA34: cmp         eax,r14d
  000000006704AA37: jl          000000006704AA8A
  000000006704AA39: mov         bl,1
  000000006704AA3B: jmp         000000006704AAAB
  000000006704AA3D: movsx       ecx,dl
  000000006704AA40: sub         ecx,2Bh
  000000006704AA43: je          000000006704AA8E
  000000006704AA45: sub         ecx,1
  000000006704AA48: je          000000006704AA82
  000000006704AA4A: sub         ecx,1
  000000006704AA4D: je          000000006704AA8E
  000000006704AA4F: sub         ecx,1
  000000006704AA52: je          000000006704AA71
  000000006704AA54: cmp         ecx,37h
  000000006704AA57: jne         000000006704AA8A
  000000006704AA59: cmp         byte ptr [rsp+0000000000000090h],0
  000000006704AA61: je          000000006704AA8A
  000000006704AA63: inc         r13d
  000000006704AA66: cmp         r13d,1
  000000006704AA6A: jg          000000006704AA8A
  000000006704AA6C: xor         bpl,bpl
  000000006704AA6F: jmp         000000006704AAA9
  000000006704AA71: test        r15d,r15d
  000000006704AA74: je          000000006704AA8A
  000000006704AA76: inc         esi
  000000006704AA78: cmp         esi,1
  000000006704AA7B: jg          000000006704AA8A
  000000006704AA7D: mov         bpl,1
  000000006704AA80: EB

  Summary

      1A7000 .text
