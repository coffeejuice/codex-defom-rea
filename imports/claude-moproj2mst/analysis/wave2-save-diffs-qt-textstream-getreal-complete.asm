Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C59B0: push        rbp
  00000000670C59B2: push        rsi
  00000000670C59B3: push        rdi
  00000000670C59B4: push        r12
  00000000670C59B6: push        r13
  00000000670C59B8: push        r14
  00000000670C59BA: push        r15
  00000000670C59BC: sub         rsp,110h
  00000000670C59C3: mov         qword ptr [rsp+70h],0FFFFFFFFFFFFFFFEh
  00000000670C59CC: mov         qword ptr [rsp+0000000000000160h],rbx
  00000000670C59D4: mov         rax,qword ptr [00000000672F3BA0h]
  00000000670C59DB: xor         rax,rsp
  00000000670C59DE: mov         qword ptr [rsp+0000000000000100h],rax
  00000000670C59E6: mov         qword ptr [rsp+60h],rdx
  00000000670C59EB: mov         rdi,rcx
  00000000670C59EE: mov         dword ptr [rsp+30h],0
  00000000670C59F6: xor         r12d,r12d
  00000000670C59F9: xor         r14d,r14d
  00000000670C59FC: xor         bpl,bpl
  00000000670C59FF: cmp         qword ptr [rcx],r12
  00000000670C5A02: je          00000000670C5A0C
  00000000670C5A04: mov         ebx,dword ptr [rcx+00000000000000A0h]
  00000000670C5A0A: jmp         00000000670C5A0F
  00000000670C5A0C: mov         ebx,dword ptr [rcx+30h]
  00000000670C5A0F: mov         r15b,1
  00000000670C5A12: cmp         qword ptr [rdi],0
  00000000670C5A16: je          00000000670C5A29
  00000000670C5A18: mov         rax,qword ptr [rdi+0000000000000098h]
  00000000670C5A1F: mov         rdx,qword ptr [rax+10h]
  00000000670C5A23: mov         r13d,dword ptr [rax+8]
  00000000670C5A27: jmp         00000000670C5A38
  00000000670C5A29: mov         rcx,qword ptr [rdi+28h]
  00000000670C5A2D: mov         rax,qword ptr [rcx]
  00000000670C5A30: mov         rdx,qword ptr [rax+10h]
  00000000670C5A34: mov         r13d,dword ptr [rax+8]
  00000000670C5A38: movsxd      rax,ebx
  00000000670C5A3B: lea         rsi,[rdx+rax*2]
  00000000670C5A3F: nop
  00000000670C5A40: cmp         ebx,r13d
  00000000670C5A43: jge         00000000670C5A74
  00000000670C5A45: movzx       eax,word ptr [rsi]
  00000000670C5A48: mov         word ptr [rsp+2Ch],ax
  00000000670C5A4D: add         rsi,2
  00000000670C5A51: inc         r12d
  00000000670C5A54: lea         rcx,[rsp+2Ch]
  00000000670C5A59: call        0000000067069C80
  00000000670C5A5E: test        al,al
  00000000670C5A60: jne         00000000670C5A6B
  00000000670C5A62: mov         bpl,1
  00000000670C5A65: mov         r14d,1
  00000000670C5A6B: inc         ebx
  00000000670C5A6D: test        bpl,bpl
  00000000670C5A70: jne         00000000670C5AC7
  00000000670C5A72: jmp         00000000670C5A40
  00000000670C5A74: cmp         qword ptr [rdi],0
  00000000670C5A78: je          00000000670C5A8E
  00000000670C5A7A: or          rdx,0FFFFFFFFFFFFFFFFh
  00000000670C5A7E: mov         rcx,rdi
  00000000670C5A81: call        00000000670C2C40
  00000000670C5A86: movzx       r15d,al
  00000000670C5A8A: test        al,al
  00000000670C5A8C: jne         00000000670C5A12
  00000000670C5A8E: test        r12d,r12d
  00000000670C5A91: je          00000000670C5AD1
  00000000670C5A93: mov         rcx,qword ptr [rdi+28h]
  00000000670C5A97: test        rcx,rcx
  00000000670C5A9A: je          00000000670C5AAD
  00000000670C5A9C: mov         rcx,qword ptr [rcx]
  00000000670C5A9F: mov         edx,dword ptr [rcx+8]
  00000000670C5AA2: mov         eax,dword ptr [rdi+30h]
  00000000670C5AA5: lea         ecx,[r12+rax]
  00000000670C5AA9: cmp         ecx,edx
  00000000670C5AAB: jl          00000000670C5AD1
  00000000670C5AAD: mov         rcx,qword ptr [rdi]
  00000000670C5AB0: test        rcx,rcx
  00000000670C5AB3: je          00000000670C5AC7
  00000000670C5AB5: mov         rax,qword ptr [rcx]
  00000000670C5AB8: call        qword ptr [rax+0000000000000088h]
  00000000670C5ABE: test        al,al
  00000000670C5AC0: jne         00000000670C5AC7
  00000000670C5AC2: test        r15b,r15b
  00000000670C5AC5: jne         00000000670C5AD1
  00000000670C5AC7: sub         r12d,r14d
  00000000670C5ACA: mov         dword ptr [rdi+000000000000008Ch],r12d
  00000000670C5AD1: mov         edx,dword ptr [rdi+000000000000008Ch]
  00000000670C5AD7: test        edx,edx
  00000000670C5AD9: je          00000000670C5AE3
  00000000670C5ADB: mov         rcx,rdi
  00000000670C5ADE: call        00000000670C3380
  00000000670C5AE3: xor         ebx,ebx
  00000000670C5AE5: mov         dword ptr [rdi+000000000000008Ch],ebx
  00000000670C5AEB: mov         r12d,ebx
  00000000670C5AEE: mov         word ptr [rsp+20h],bx
  00000000670C5AF3: mov         rbp,rbx
  00000000670C5AF6: lea         r14,[0000000067000000h]
  00000000670C5AFD: mov         r15d,0FFh
  00000000670C5B03: mov         rax,qword ptr [rdi+28h]
  00000000670C5B07: test        rax,rax
  00000000670C5B0A: je          00000000670C5B1B
  00000000670C5B0C: mov         rax,qword ptr [rax]
  00000000670C5B0F: mov         ecx,dword ptr [rax+8]
  00000000670C5B12: cmp         dword ptr [rdi+30h],ecx
  00000000670C5B15: je          00000000670C5DA2
  00000000670C5B1B: cmp         qword ptr [rdi],0
  00000000670C5B1F: je          00000000670C5B4D
  00000000670C5B21: mov         rax,qword ptr [rdi+0000000000000098h]
  00000000670C5B28: cmp         dword ptr [rax+8],0
  00000000670C5B2C: jne         00000000670C5B47
  00000000670C5B2E: or          r13,0FFFFFFFFFFFFFFFFh
  00000000670C5B32: mov         rdx,r13
  00000000670C5B35: mov         rcx,rdi
  00000000670C5B38: call        00000000670C2C40
  00000000670C5B3D: test        al,al
  00000000670C5B3F: je          00000000670C5DA6
  00000000670C5B45: jmp         00000000670C5B51
  00000000670C5B47: or          r13,0FFFFFFFFFFFFFFFFh
  00000000670C5B4B: jmp         00000000670C5B51
  00000000670C5B4D: or          r13,0FFFFFFFFFFFFFFFFh
  00000000670C5B51: mov         rax,qword ptr [rdi+28h]
  00000000670C5B55: test        rax,rax
  00000000670C5B58: je          00000000670C5B6B
  00000000670C5B5A: mov         rax,qword ptr [rax]
  00000000670C5B5D: mov         rcx,qword ptr [rax+10h]
  00000000670C5B61: movsxd      rax,dword ptr [rdi+30h]
  00000000670C5B65: lea         rdx,[rcx+rax*2]
  00000000670C5B69: jmp         00000000670C5B81
  00000000670C5B6B: mov         rax,qword ptr [rdi+0000000000000098h]
  00000000670C5B72: mov         rcx,qword ptr [rax+10h]
  00000000670C5B76: movsxd      rax,dword ptr [rdi+00000000000000A0h]
  00000000670C5B7D: lea         rdx,[rcx+rax*2]
  00000000670C5B81: movzx       eax,word ptr [rdx]
  00000000670C5B84: mov         word ptr [rsp+20h],ax
  00000000670C5B89: mov         edx,1
  00000000670C5B8E: mov         rcx,rdi
  00000000670C5B91: call        00000000670C3380
  00000000670C5B96: movzx       r11d,word ptr [rsp+20h]
  00000000670C5B9C: add         r11d,0FFFFFFD0h
  00000000670C5BA0: cmp         r11d,44h
  00000000670C5BA4: ja          00000000670C5BFB
  00000000670C5BA6: movsxd      rax,r11d
  00000000670C5BA9: movzx       eax,byte ptr [r14+rax+00000000000C5FF4h]
  00000000670C5BB2: mov         ecx,dword ptr [r14+rax*4+00000000000C5FD8h]
  00000000670C5BBA: add         rcx,r14
  00000000670C5BBD: jmp         rcx
  00000000670C5BBF: mov         edx,2
  00000000670C5BC4: jmp         00000000670C5D58
  00000000670C5BC9: mov         edx,5
  00000000670C5BCE: jmp         00000000670C5D58
  00000000670C5BD3: mov         edx,6
  00000000670C5BD8: jmp         00000000670C5D58
  00000000670C5BDD: mov         edx,7
  00000000670C5BE2: jmp         00000000670C5D58
  00000000670C5BE7: mov         edx,8
  00000000670C5BEC: jmp         00000000670C5D58
  00000000670C5BF1: mov         edx,9
  00000000670C5BF6: jmp         00000000670C5D58
  00000000670C5BFB: lea         rdx,[rsp+24h]
  00000000670C5C00: lea         rcx,[rsp+20h]
  00000000670C5C05: call        000000006706A650
  00000000670C5C0A: lea         rdx,[rsp+4Ah]
  00000000670C5C0F: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5C16: call        000000006704A740
  00000000670C5C1B: mov         rcx,rax
  00000000670C5C1E: lea         rdx,[rsp+3Eh]
  00000000670C5C23: call        000000006706A650
  00000000670C5C28: movzx       eax,word ptr [rax]
  00000000670C5C2B: cmp         word ptr [rsp+24h],ax
  00000000670C5C30: jne         00000000670C5C3C
  00000000670C5C32: mov         edx,3
  00000000670C5C37: jmp         00000000670C5D58
  00000000670C5C3C: lea         rdx,[rsp+3Ch]
  00000000670C5C41: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5C48: call        000000006704A8C0
  00000000670C5C4D: mov         rcx,rax
  00000000670C5C50: lea         rdx,[rsp+44h]
  00000000670C5C55: call        000000006706A650
  00000000670C5C5A: movzx       eax,word ptr [rax]
  00000000670C5C5D: cmp         word ptr [rsp+24h],ax
  00000000670C5C62: jne         00000000670C5C6E
  00000000670C5C64: mov         edx,4
  00000000670C5C69: jmp         00000000670C5D58
  00000000670C5C6E: lea         rdx,[rsp+40h]
  00000000670C5C73: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5C7A: call        000000006704A840
  00000000670C5C7F: mov         rcx,rax
  00000000670C5C82: lea         rdx,[rsp+48h]
  00000000670C5C87: call        000000006706A650
  00000000670C5C8C: movzx       eax,word ptr [rax]
  00000000670C5C8F: cmp         word ptr [rsp+24h],ax
  00000000670C5C94: je          00000000670C5D53
  00000000670C5C9A: lea         rdx,[rsp+38h]
  00000000670C5C9F: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5CA6: call        000000006704A880
  00000000670C5CAB: mov         rcx,rax
  00000000670C5CAE: lea         rdx,[rsp+46h]
  00000000670C5CB3: call        000000006706A650
  00000000670C5CB8: movzx       eax,word ptr [rax]
  00000000670C5CBB: cmp         word ptr [rsp+24h],ax
  00000000670C5CC0: je          00000000670C5D53
  00000000670C5CC6: xor         r8d,r8d
  00000000670C5CC9: lea         edx,[r8+1]
  00000000670C5CCD: lea         rcx,[rsp+50h]
  00000000670C5CD2: call        000000006704D5A0
  00000000670C5CD7: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5CDE: call        0000000067049340
  00000000670C5CE3: mov         rbx,rax
  00000000670C5CE6: lea         rcx,[rsp+50h]
  00000000670C5CEB: call        0000000067049340
  00000000670C5CF0: cmp         rbx,rax
  00000000670C5CF3: jne         00000000670C5D1B
  00000000670C5CF5: lea         rdx,[rsp+58h]
  00000000670C5CFA: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5D01: call        000000006704A340
  00000000670C5D06: mov         ebx,dword ptr [rax]
  00000000670C5D08: lea         rdx,[rsp+68h]
  00000000670C5D0D: lea         rcx,[rsp+50h]
  00000000670C5D12: call        000000006704A340
  00000000670C5D17: cmp         ebx,dword ptr [rax]
  00000000670C5D19: je          00000000670C5D4C
  00000000670C5D1B: lea         rdx,[rsp+42h]
  00000000670C5D20: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5D27: call        000000006704A780
  00000000670C5D2C: mov         rcx,rax
  00000000670C5D2F: lea         rdx,[rsp+3Ah]
  00000000670C5D34: call        000000006706A650
  00000000670C5D39: movzx       eax,word ptr [rax]
  00000000670C5D3C: cmp         word ptr [rsp+24h],ax
  00000000670C5D41: jne         00000000670C5D4C
  00000000670C5D43: mov         edx,2
  00000000670C5D48: xor         ebx,ebx
  00000000670C5D4A: jmp         00000000670C5D58
  00000000670C5D4C: xor         ebx,ebx
  00000000670C5D4E: mov         rdx,rbx
  00000000670C5D51: jmp         00000000670C5D58
  00000000670C5D53: mov         edx,1
  00000000670C5D58: movsxd      rax,dword ptr [rsp+30h]
  00000000670C5D5D: lea         rcx,[rax+rax*4]
  00000000670C5D61: lea         rax,[rdx+rcx*2]
  00000000670C5D65: movzx       eax,byte ptr [rax+r14+000000000022C880h]
  00000000670C5D6E: mov         dword ptr [rsp+30h],eax
  00000000670C5D72: test        eax,eax
  00000000670C5D74: je          00000000670C5DB0
  00000000670C5D76: cmp         eax,0Dh
  00000000670C5D79: je          00000000670C5DB0
  00000000670C5D7B: cmp         rbp,7Bh
  00000000670C5D7F: jg          00000000670C5DB0
  00000000670C5D81: movzx       eax,word ptr [rsp+20h]
  00000000670C5D86: movzx       ecx,al
  00000000670C5D89: cmp         ax,r15w
  00000000670C5D8D: cmova       ecx,ebx
  00000000670C5D90: mov         byte ptr [rsp+rbp+0000000000000080h],cl
  00000000670C5D97: inc         r12d
  00000000670C5D9A: inc         rbp
  00000000670C5D9D: jmp         00000000670C5B03
  00000000670C5DA2: or          r13,0FFFFFFFFFFFFFFFFh
  00000000670C5DA6: mov         word ptr [rsp+20h],bx
  00000000670C5DAB: jmp         00000000670C5E69
  00000000670C5DB0: lea         rdx,[rsp+20h]
  00000000670C5DB5: mov         rcx,rdi
  00000000670C5DB8: call        00000000670C20F0
  00000000670C5DBD: cmp         r12d,7Bh
  00000000670C5DC1: jle         00000000670C5E69
  00000000670C5DC7: mov         rax,qword ptr [rdi+28h]
  00000000670C5DCB: test        rax,rax
  00000000670C5DCE: je          00000000670C5DDB
  00000000670C5DD0: mov         rax,qword ptr [rax]
  00000000670C5DD3: mov         ecx,dword ptr [rax+8]
  00000000670C5DD6: cmp         dword ptr [rdi+30h],ecx
  00000000670C5DD9: je          00000000670C5E55
  00000000670C5DDB: cmp         qword ptr [rdi],0
  00000000670C5DDF: je          00000000670C5DFD
  00000000670C5DE1: mov         rax,qword ptr [rdi+0000000000000098h]
  00000000670C5DE8: cmp         dword ptr [rax+8],0
  00000000670C5DEC: jne         00000000670C5DFD
  00000000670C5DEE: mov         rdx,r13
  00000000670C5DF1: mov         rcx,rdi
  00000000670C5DF4: call        00000000670C2C40
  00000000670C5DF9: test        al,al
  00000000670C5DFB: je          00000000670C5E55
  00000000670C5DFD: mov         rax,qword ptr [rdi+28h]
  00000000670C5E01: test        rax,rax
  00000000670C5E04: je          00000000670C5E17
  00000000670C5E06: mov         rax,qword ptr [rax]
  00000000670C5E09: mov         rcx,qword ptr [rax+10h]
  00000000670C5E0D: movsxd      rax,dword ptr [rdi+30h]
  00000000670C5E11: lea         rdx,[rcx+rax*2]
  00000000670C5E15: jmp         00000000670C5E2D
  00000000670C5E17: mov         rax,qword ptr [rdi+0000000000000098h]
  00000000670C5E1E: mov         rcx,qword ptr [rax+10h]
  00000000670C5E22: movsxd      rax,dword ptr [rdi+00000000000000A0h]
  00000000670C5E29: lea         rdx,[rcx+rax*2]
  00000000670C5E2D: movzx       eax,word ptr [rdx]
  00000000670C5E30: mov         word ptr [rsp+20h],ax
  00000000670C5E35: mov         edx,1
  00000000670C5E3A: mov         rcx,rdi
  00000000670C5E3D: call        00000000670C3380
  00000000670C5E42: lea         rcx,[rsp+20h]
  00000000670C5E47: call        0000000067069E70
  00000000670C5E4C: test        al,al
  00000000670C5E4E: je          00000000670C5E5C
  00000000670C5E50: jmp         00000000670C5DC7
  00000000670C5E55: mov         word ptr [rsp+20h],bx
  00000000670C5E5A: jmp         00000000670C5E69
  00000000670C5E5C: lea         rdx,[rsp+20h]
  00000000670C5E61: mov         rcx,rdi
  00000000670C5E64: call        00000000670C20F0
  00000000670C5E69: test        r12d,r12d
  00000000670C5E6C: jne         00000000670C5E75
  00000000670C5E6E: xor         al,al
  00000000670C5E70: jmp         00000000670C5FAC
  00000000670C5E75: mov         rbx,qword ptr [rsp+60h]
  00000000670C5E7A: test        rbx,rbx
  00000000670C5E7D: jne         00000000670C5E86
  00000000670C5E7F: mov         al,1
  00000000670C5E81: jmp         00000000670C5FAC
  00000000670C5E86: movsxd      rax,r12d
  00000000670C5E89: mov         byte ptr [rsp+rax+0000000000000080h],0
  00000000670C5E91: lea         rdx,[00000000671F14BCh]
  00000000670C5E98: lea         rcx,[rsp+0000000000000080h]
  00000000670C5EA0: call        000000006702CB90
  00000000670C5EA5: test        eax,eax
  00000000670C5EA7: je          00000000670C5FA1
  00000000670C5EAD: lea         rdx,[000000006722C870h]
  00000000670C5EB4: lea         rcx,[rsp+0000000000000080h]
  00000000670C5EBC: call        000000006702CB90
  00000000670C5EC1: test        eax,eax
  00000000670C5EC3: je          00000000670C5FA1
  00000000670C5EC9: lea         rdx,[000000006722C868h]
  00000000670C5ED0: lea         rcx,[rsp+0000000000000080h]
  00000000670C5ED8: call        000000006702CB90
  00000000670C5EDD: test        eax,eax
  00000000670C5EDF: je          00000000670C5FA1
  00000000670C5EE5: lea         rdx,[00000000671F14B4h]
  00000000670C5EEC: lea         rcx,[rsp+0000000000000080h]
  00000000670C5EF4: call        000000006702CB90
  00000000670C5EF9: test        eax,eax
  00000000670C5EFB: je          00000000670C5F94
  00000000670C5F01: lea         rdx,[00000000671F14B0h]
  00000000670C5F08: lea         rcx,[rsp+0000000000000080h]
  00000000670C5F10: call        000000006702CB90
  00000000670C5F15: test        eax,eax
  00000000670C5F17: je          00000000670C5F94
  00000000670C5F19: lea         rdx,[00000000671F14A8h]
  00000000670C5F20: lea         rcx,[rsp+0000000000000080h]
  00000000670C5F28: call        000000006702CB90
  00000000670C5F2D: test        eax,eax
  00000000670C5F2F: jne         00000000670C5F46
  00000000670C5F31: call        00000000670205F0
  00000000670C5F36: xorpd       xmm0,xmmword ptr [00000000671A9220h]
  00000000670C5F3E: movsd       mmword ptr [rbx],xmm0
  00000000670C5F42: mov         al,1
  00000000670C5F44: jmp         00000000670C5FAC
  00000000670C5F46: mov         r8d,r13d
  00000000670C5F49: lea         rdx,[rsp+0000000000000080h]
  00000000670C5F51: lea         rcx,[rsp+30h]
  00000000670C5F56: call        00000000670741E0
  00000000670C5F5B: nop
  00000000670C5F5C: lea         rcx,[rdi+00000000000000D0h]
  00000000670C5F63: lea         r8,[rsp+28h]
  00000000670C5F68: mov         rdx,rax
  00000000670C5F6B: call        000000006704DCE0
  00000000670C5F70: movsd       mmword ptr [rbx],xmm0
  00000000670C5F74: mov         rax,qword ptr [rsp+30h]
  00000000670C5F79: lock xadd   dword ptr [rax],r13d
  00000000670C5F7E: add         r13d,0FFFFFFFFh
  00000000670C5F82: jne         00000000670C5F8E
  00000000670C5F84: mov         rcx,qword ptr [rsp+30h]
  00000000670C5F89: call        00000000670700D0
  00000000670C5F8E: mov         al,byte ptr [rsp+28h]
  00000000670C5F92: jmp         00000000670C5FAC
  00000000670C5F94: call        00000000670205F0
  00000000670C5F99: movsd       mmword ptr [rbx],xmm0
  00000000670C5F9D: mov         al,1
  00000000670C5F9F: jmp         00000000670C5FAC
  00000000670C5FA1: call        00000000670205D0
  00000000670C5FA6: movsd       mmword ptr [rbx],xmm0
  00000000670C5FAA: mov         al,1
  00000000670C5FAC: mov         rcx,qword ptr [rsp+0000000000000100h]
  00000000670C5FB4: xor         rcx,rsp
  00000000670C5FB7: call        0000000067178C10
  00000000670C5FBC: mov         rbx,qword ptr [rsp+0000000000000160h]
  00000000670C5FC4: add         rsp,110h
  00000000670C5FCB: pop         r15
  00000000670C5FCD: pop         r14
  00000000670C5FCF: pop         r13
  00000000670C5FD1: pop         r12
  00000000670C5FD3: pop         rdi
  00000000670C5FD4: pop         rsi
  00000000670C5FD5: pop         rbp
  00000000670C5FD6: ret
  00000000670C5FD7: nop
  00000000670C5FD8: mov         edi,0E7000C5Bh
  00000000670C5FDD: pop         rbx
  00000000670C5FDE: or          al,0
  00000000670C5FE0: fstp        qword ptr [rbx+0Ch]
  00000000670C5FE3: add         cl,cl
  00000000670C5FE5: pop         rbx
  00000000670C5FE6: or          al,0
  00000000670C5FE8: rcr         dword ptr [rbx+0Ch],cl
  00000000670C5FEB: add         cl,dh
  00000000670C5FED: pop         rbx
  00000000670C5FEE: or          al,0
  00000000670C5FF0: sti
  00000000670C5FF1: pop         rbx
  00000000670C5FF2: or          al,0
  00000000670C5FF4: add         byte ptr [rax],al
  00000000670C5FF6: add         byte ptr [rax],al
  00000000670C5FF8: add         byte ptr [rax],al
  00000000670C5FFA: add         byte ptr [rax],al
  00000000670C5FFC: add         byte ptr [rax],al
  00000000670C5FFE: 06
  00000000670C5FFF: 06
  00000000670C6000: 06
  00000000670C6001: 06
  00000000670C6002: 06
  00000000670C6003: 06
  00000000670C6004: 06
  00000000670C6005: add         dword ptr [rsi],eax
  00000000670C6007: 06
  00000000670C6008: 06
  00000000670C6009: 06
  00000000670C600A: add         al,byte ptr [rsi]
  00000000670C600C: 06
  00000000670C600D: add         eax,dword ptr [rsi]
  00000000670C600F: 06
  00000000670C6010: 06
  00000000670C6011: 06
  00000000670C6012: add         al,6
  00000000670C6014: 06
  00000000670C6015: 06
  00000000670C6016: 06
  00000000670C6017: 06
  00000000670C6018: add         eax,6060606h
  00000000670C601D: 06
  00000000670C601E: 06
  00000000670C601F: 06
  00000000670C6020: 06
  00000000670C6021: 06
  00000000670C6022: 06
  00000000670C6023: 06
  00000000670C6024: 06
  00000000670C6025: add         dword ptr [rsi],eax
  00000000670C6027: 06
  00000000670C6028: 06
  00000000670C6029: 06
  00000000670C602A: add         al,byte ptr [rsi]
  00000000670C602C: 06
  00000000670C602D: add         eax,dword ptr [rsi]
  00000000670C602F: 06
  00000000670C6030: 06
  00000000670C6031: 06
  00000000670C6032: add         al,6
  00000000670C6034: 06
  00000000670C6035: 06
  00000000670C6036: 06
  00000000670C6037: 06
  00000000670C6038: 05
  00000000670C6039: int         3

  Summary

      1A7000 .text
