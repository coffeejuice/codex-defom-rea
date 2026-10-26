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
  000000006704AA80: jmp         000000006704AAA9
  000000006704AA82: test        bl,bl
  000000006704AA84: je          000000006704AA8A
  000000006704AA86: test        esi,esi
  000000006704AA88: jle         000000006704AAA9
  000000006704AA8A: xor         al,al
  000000006704AA8C: jmp         000000006704AAE7
  000000006704AA8E: cmp         byte ptr [rsp+0000000000000090h],0
  000000006704AA96: je          000000006704AAA4
  000000006704AA98: test        r12,r12
  000000006704AA9B: je          000000006704AAA9
  000000006704AA9D: test        dil,dil
  000000006704AAA0: je          000000006704AA8A
  000000006704AAA2: jmp         000000006704AAA9
  000000006704AAA4: test        r12,r12
  000000006704AAA7: jne         000000006704AA8A
  000000006704AAA9: xor         bl,bl
  000000006704AAAB: cmp         dl,65h
  000000006704AAAE: sete        dil
  000000006704AAB2: cmp         dl,2Ch
  000000006704AAB5: je          000000006704AAC4
  000000006704AAB7: mov         rcx,qword ptr [rsp+0000000000000098h]
  000000006704AABF: call        000000006702DCB0
  000000006704AAC4: mov         ecx,dword ptr [rsp+24h]
  000000006704AAC8: mov         rax,qword ptr [rsp+0000000000000088h]
  000000006704AAD0: inc         r12
  000000006704AAD3: mov         rax,qword ptr [rax]
  000000006704AAD6: inc         ecx
  000000006704AAD8: cmp         ecx,dword ptr [rax+8]
  000000006704AADB: mov         dword ptr [rsp+24h],ecx
  000000006704AADF: jl          000000006704A9F0
  000000006704AAE5: mov         al,1
  000000006704AAE7: add         rsp,38h
  000000006704AAEB: pop         r15
  000000006704AAED: pop         r14
  000000006704AAEF: pop         r13
  000000006704AAF1: pop         r12
  000000006704AAF3: pop         rdi
  000000006704AAF4: pop         rsi
  000000006704AAF5: pop         rbp
  000000006704AAF6: pop         rbx
  000000006704AAF7: ret
  000000006704AAF8: int         3
  000000006704AAF9: int         3
  000000006704AAFA: int         3
  000000006704AAFB: int         3
  000000006704AAFC: int         3
  000000006704AAFD: int         3
  000000006704AAFE: int         3
  000000006704AAFF: int         3
  000000006704AB00: mov         qword ptr [rsp+8],rcx
  000000006704AB05: push        rbx
  000000006704AB06: sub         rsp,30h
  000000006704AB0A: 48
  000000006704AB0B: C7
  000000006704AB0C: and         al,20h
  000000006704AB0F: FE
  000000006704AB10: FF

  Summary

      1A7000 .text
