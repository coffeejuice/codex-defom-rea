Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  0000000061017B90: 40 55              push        rbp
  0000000061017B92: 56                 push        rsi
  0000000061017B93: 57                 push        rdi
  0000000061017B94: 41 54              push        r12
  0000000061017B96: 41 55              push        r13
  0000000061017B98: 41 56              push        r14
  0000000061017B9A: 41 57              push        r15
  0000000061017B9C: 48 83 EC 40        sub         rsp,40h
  0000000061017BA0: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000061017BA9: 48 89 9C 24 98 00  mov         qword ptr [rsp+0000000000000098h],rbx
                    00 00
  0000000061017BB1: 4C 8B F9           mov         r15,rcx
  0000000061017BB4: 4C 8D 89 3C 03 00  lea         r9,[rcx+000000000000033Ch]
                    00
  0000000061017BBB: 4C 8D 81 38 03 00  lea         r8,[rcx+0000000000000338h]
                    00
  0000000061017BC2: 48 8D 91 2C 01 00  lea         rdx,[rcx+000000000000012Ch]
                    00
  0000000061017BC9: 48 81 C1 30 03 00  add         rcx,330h
                    00
  0000000061017BD0: E8 0B 87 FF FF     call        00000000610102E0
  0000000061017BD5: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000061017BDD: 49 8B CF           mov         rcx,r15
  0000000061017BE0: E8 FB D0 FF FF     call        0000000061014CE0
  0000000061017BE5: 90                 nop
  0000000061017BE6: 49 8D 97 30 03 00  lea         rdx,[r15+0000000000000330h]
                    00
  0000000061017BED: 48 8B C8           mov         rcx,rax
  0000000061017BF0: FF 15 12 BC 02 00  call        qword ptr [0000000061043808h]
  0000000061017BF6: 0F B6 D8           movzx       ebx,al
  0000000061017BF9: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000061017C01: FF 15 D9 BC 02 00  call        qword ptr [00000000610438E0h]
  0000000061017C07: 84 DB              test        bl,bl
  0000000061017C09: 74 4D              je          0000000061017C58
  0000000061017C0B: 48 8D 15 36 FD 02  lea         rdx,[0000000061047948h]
                    00
  0000000061017C12: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000061017C1A: FF 15 78 BC 02 00  call        qword ptr [0000000061043898h]
  0000000061017C20: 48 8B D0           mov         rdx,rax
  0000000061017C23: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000061017C2B: FF 15 8F BB 02 00  call        qword ptr [00000000610437C0h]
  0000000061017C31: 90                 nop
  0000000061017C32: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000061017C3A: 49 8B CF           mov         rcx,r15
  0000000061017C3D: E8 0E C2 FF FF     call        0000000061013E50
  0000000061017C42: 90                 nop
  0000000061017C43: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000061017C4B: FF 15 8F BC 02 00  call        qword ptr [00000000610438E0h]
  0000000061017C51: 32 C0              xor         al,al
  0000000061017C53: E9 AB 04 00 00     jmp         0000000061018103
  0000000061017C58: 49 83 BF E8 00 00  cmp         qword ptr [r15+00000000000000E8h],0
                    00 00
  0000000061017C60: 0F 84 ED 00 00 00  je          0000000061017D53
  0000000061017C66: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000061017C6E: FF 15 5C BC 02 00  call        qword ptr [00000000610438D0h]
  0000000061017C74: 90                 nop
  0000000061017C75: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000061017C7D: FF 15 4D BC 02 00  call        qword ptr [00000000610438D0h]
  0000000061017C83: 90                 nop
  0000000061017C84: 41 80 BF 80 00 00  cmp         byte ptr [r15+0000000000000080h],0
                    00 00
  0000000061017C8C: 74 2B              je          0000000061017CB9
  0000000061017C8E: 49 8D 8F B0 00 00  lea         rcx,[r15+00000000000000B0h]
                    00
  0000000061017C95: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  0000000061017C9D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000061017CA2: 4C 8D 8C 24 88 00  lea         r9,[rsp+0000000000000088h]
                    00 00
  0000000061017CAA: 45 33 C0           xor         r8d,r8d
  0000000061017CAD: 49 8D 97 30 03 00  lea         rdx,[r15+0000000000000330h]
                    00
  0000000061017CB4: E8 07 BF FF FF     call        0000000061013BC0
  0000000061017CB9: 49 8B 8F E8 00 00  mov         rcx,qword ptr [r15+00000000000000E8h]
                    00
  0000000061017CC0: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000061017CC3: 4D 8D 8F 30 03 00  lea         r9,[r15+0000000000000330h]
                    00
  0000000061017CCA: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  0000000061017CD2: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000061017CDA: FF 50 38           call        qword ptr [rax+38h]
  0000000061017CDD: 84 C0              test        al,al
  0000000061017CDF: 75 55              jne         0000000061017D36
  0000000061017CE1: 49 8B 8F E8 00 00  mov         rcx,qword ptr [r15+00000000000000E8h]
                    00
  0000000061017CE8: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000061017CEB: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000061017CF3: FF 50 60           call        qword ptr [rax+60h]
  0000000061017CF6: 90                 nop
  0000000061017CF7: 48 8B D0           mov         rdx,rax
  0000000061017CFA: 49 8B CF           mov         rcx,r15
  0000000061017CFD: E8 4E C1 FF FF     call        0000000061013E50
  0000000061017D02: 90                 nop
  0000000061017D03: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000061017D0B: FF 15 CF BB 02 00  call        qword ptr [00000000610438E0h]
  0000000061017D11: 90                 nop
  0000000061017D12: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000061017D1A: FF 15 C0 BB 02 00  call        qword ptr [00000000610438E0h]
  0000000061017D20: 90                 nop
  0000000061017D21: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000061017D29: FF 15 B1 BB 02 00  call        qword ptr [00000000610438E0h]
  0000000061017D2F: 32 C0              xor         al,al
  0000000061017D31: E9 CD 03 00 00     jmp         0000000061018103
  0000000061017D36: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000061017D3E: FF 15 9C BB 02 00  call        qword ptr [00000000610438E0h]
  0000000061017D44: 90                 nop
  0000000061017D45: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000061017D4D: FF 15 8D BB 02 00  call        qword ptr [00000000610438E0h]
  0000000061017D53: 41 80 BF 80 00 00  cmp         byte ptr [r15+0000000000000080h],0
                    00 00
  0000000061017D5B: 0F 84 A0 03 00 00  je          0000000061018101
  0000000061017D61: 48 8B 3D 60 B6 02  mov         rdi,qword ptr [00000000610433C8h]
                    00
  0000000061017D68: 48 89 BC 24 80 00  mov         qword ptr [rsp+0000000000000080h],rdi
                    00 00
  0000000061017D70: 48 8D 4F 68        lea         rcx,[rdi+68h]
  0000000061017D74: FF 15 B6 BA 02 00  call        qword ptr [0000000061043830h]
  0000000061017D7A: 90                 nop
  0000000061017D7B: 48 8B 1D 46 B6 02  mov         rbx,qword ptr [00000000610433C8h]
                    00
  0000000061017D82: 48 89 9C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbx
                    00 00
  0000000061017D8A: 48 8D 4B 68        lea         rcx,[rbx+68h]
  0000000061017D8E: FF 15 9C BA 02 00  call        qword ptr [0000000061043830h]
  0000000061017D94: 90                 nop
  0000000061017D95: 49 83 BF E8 00 00  cmp         qword ptr [r15+00000000000000E8h],0
                    00 00
  0000000061017D9D: 0F 84 96 00 00 00  je          0000000061017E39
  0000000061017DA3: 49 8B 87 B0 00 00  mov         rax,qword ptr [r15+00000000000000B0h]
                    00
  0000000061017DAA: 48 83 C0 08        add         rax,8
  0000000061017DAE: 4C 8B 20           mov         r12,qword ptr [rax]
  0000000061017DB1: 49 3B FC           cmp         rdi,r12
  0000000061017DB4: 0F 84 7F 00 00 00  je          0000000061017E39
  0000000061017DBA: 49 8D 4C 24 68     lea         rcx,[r12+68h]
  0000000061017DBF: FF 15 6B BA 02 00  call        qword ptr [0000000061043830h]
  0000000061017DC5: 48 8D 4F 68        lea         rcx,[rdi+68h]
  0000000061017DC9: FF 15 59 BA 02 00  call        qword ptr [0000000061043828h]
  0000000061017DCF: 84 C0              test        al,al
  0000000061017DD1: 75 3E              jne         0000000061017E11
  0000000061017DD3: 48 8B 6F 08        mov         rbp,qword ptr [rdi+8]
  0000000061017DD7: 48 3B EF           cmp         rbp,rdi
  0000000061017DDA: 74 27              je          0000000061017E03
  0000000061017DDC: 0F 1F 40 00        nop         dword ptr [rax]
  0000000061017DE0: 48 8B C5           mov         rax,rbp
  0000000061017DE3: 48 8B 6D 08        mov         rbp,qword ptr [rbp+8]
  0000000061017DE7: 48 8D 70 F0        lea         rsi,[rax-10h]
  0000000061017DEB: 48 8B CE           mov         rcx,rsi
  0000000061017DEE: FF 15 EC BA 02 00  call        qword ptr [00000000610438E0h]
  0000000061017DF4: 48 8D 4E 08        lea         rcx,[rsi+8]
  0000000061017DF8: FF 15 E2 BA 02 00  call        qword ptr [00000000610438E0h]
  0000000061017DFE: 48 3B EF           cmp         rbp,rdi
  0000000061017E01: 75 DD              jne         0000000061017DE0
  0000000061017E03: BA 10 00 00 00     mov         edx,10h
  0000000061017E08: 48 8B CF           mov         rcx,rdi
  0000000061017E0B: FF 15 0F B6 02 00  call        qword ptr [0000000061043420h]
  0000000061017E11: 49 8B FC           mov         rdi,r12
  0000000061017E14: 4C 89 A4 24 80 00  mov         qword ptr [rsp+0000000000000080h],r12
                    00 00
  0000000061017E1C: 41 F6 44 24 78 02  test        byte ptr [r12+78h],2
  0000000061017E22: 75 15              jne         0000000061017E39
  0000000061017E24: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000061017E2C: E8 BF 9D FF FF     call        0000000061011BF0
  0000000061017E31: 48 8B BC 24 80 00  mov         rdi,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000061017E39: 49 8D 8F B0 00 00  lea         rcx,[r15+00000000000000B0h]
                    00
  0000000061017E40: E8 FB F3 FF FF     call        0000000061017240
  0000000061017E45: 49 83 BF E8 00 00  cmp         qword ptr [r15+00000000000000E8h],0
                    00 00
  0000000061017E4D: 0F 84 05 02 00 00  je          0000000061018058
  0000000061017E53: 49 8B 87 B0 00 00  mov         rax,qword ptr [r15+00000000000000B0h]
                    00
  0000000061017E5A: 4C 8B 60 08        mov         r12,qword ptr [rax+8]
  0000000061017E5E: 49 3B DC           cmp         rbx,r12
  0000000061017E61: 74 78              je          0000000061017EDB
  0000000061017E63: 49 8D 4C 24 68     lea         rcx,[r12+68h]
  0000000061017E68: FF 15 C2 B9 02 00  call        qword ptr [0000000061043830h]
  0000000061017E6E: 48 8D 4B 68        lea         rcx,[rbx+68h]
  0000000061017E72: FF 15 B0 B9 02 00  call        qword ptr [0000000061043828h]
  0000000061017E78: 84 C0              test        al,al
  0000000061017E7A: 75 37              jne         0000000061017EB3
  0000000061017E7C: 48 8B 6B 08        mov         rbp,qword ptr [rbx+8]
  0000000061017E80: 48 3B EB           cmp         rbp,rbx
  0000000061017E83: 74 20              je          0000000061017EA5
  0000000061017E85: 48 8B F5           mov         rsi,rbp
  0000000061017E88: 48 8B 6D 08        mov         rbp,qword ptr [rbp+8]
  0000000061017E8C: 48 8D 4E F0        lea         rcx,[rsi-10h]
  0000000061017E90: FF 15 4A BA 02 00  call        qword ptr [00000000610438E0h]
  0000000061017E96: 48 8D 4E F8        lea         rcx,[rsi-8]
  0000000061017E9A: FF 15 40 BA 02 00  call        qword ptr [00000000610438E0h]
  0000000061017EA0: 48 3B EB           cmp         rbp,rbx
  0000000061017EA3: 75 E0              jne         0000000061017E85
  0000000061017EA5: BA 10 00 00 00     mov         edx,10h
  0000000061017EAA: 48 8B CB           mov         rcx,rbx
  0000000061017EAD: FF 15 6D B5 02 00  call        qword ptr [0000000061043420h]
  0000000061017EB3: 49 8B DC           mov         rbx,r12
  0000000061017EB6: 48 89 9C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbx
                    00 00
  0000000061017EBE: 41 F6 44 24 78 02  test        byte ptr [r12+78h],2
  0000000061017EC4: 75 15              jne         0000000061017EDB
  0000000061017EC6: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000061017ECE: E8 1D 9D FF FF     call        0000000061011BF0
  0000000061017ED3: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000061017EDB: 8B 47 70           mov         eax,dword ptr [rdi+70h]
  0000000061017EDE: 3B 43 70           cmp         eax,dword ptr [rbx+70h]
  0000000061017EE1: 0F 84 71 01 00 00  je          0000000061018058
  0000000061017EE7: 48 8B 77 08        mov         rsi,qword ptr [rdi+8]
  0000000061017EEB: 48 3B F7           cmp         rsi,rdi
  0000000061017EEE: 0F 84 64 01 00 00  je          0000000061018058
  0000000061017EF4: 48 8D 4E F0        lea         rcx,[rsi-10h]
  0000000061017EF8: FF 15 22 B9 02 00  call        qword ptr [0000000061043820h]
  0000000061017EFE: 84 C0              test        al,al
  0000000061017F00: 75 6F              jne         0000000061017F71
  0000000061017F02: 4C 8B EB           mov         r13,rbx
  0000000061017F05: 48 63 43 6C        movsxd      rax,dword ptr [rbx+6Ch]
  0000000061017F09: 4C 8B E0           mov         r12,rax
  0000000061017F0C: 85 C0              test        eax,eax
  0000000061017F0E: 78 4C              js          0000000061017F5C
  0000000061017F10: 4B 8B 6C E5 08     mov         rbp,qword ptr [r13+r12*8+8]
  0000000061017F15: 48 3B EB           cmp         rbp,rbx
  0000000061017F18: 74 25              je          0000000061017F3F
  0000000061017F1A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  0000000061017F20: 48 8D 4D F0        lea         rcx,[rbp-10h]
  0000000061017F24: 48 8D 56 F0        lea         rdx,[rsi-10h]
  0000000061017F28: FF 15 B2 B4 02 00  call        qword ptr [00000000610433E0h]
  0000000061017F2E: 84 C0              test        al,al
  0000000061017F30: 74 0D              je          0000000061017F3F
  0000000061017F32: 4C 8B ED           mov         r13,rbp
  0000000061017F35: 4A 8B 6C E5 08     mov         rbp,qword ptr [rbp+r12*8+8]
  0000000061017F3A: 48 3B EB           cmp         rbp,rbx
  0000000061017F3D: 75 E1              jne         0000000061017F20
  0000000061017F3F: 49 83 EC 01        sub         r12,1
  0000000061017F43: 79 CB              jns         0000000061017F10
  0000000061017F45: 48 3B EB           cmp         rbp,rbx
  0000000061017F48: 74 12              je          0000000061017F5C
  0000000061017F4A: 48 8D 55 F0        lea         rdx,[rbp-10h]
  0000000061017F4E: 48 8D 4E F0        lea         rcx,[rsi-10h]
  0000000061017F52: FF 15 88 B4 02 00  call        qword ptr [00000000610433E0h]
  0000000061017F58: 84 C0              test        al,al
  0000000061017F5A: 74 15              je          0000000061017F71
  0000000061017F5C: 49 8B 8F E8 00 00  mov         rcx,qword ptr [r15+00000000000000E8h]
                    00
  0000000061017F63: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000061017F66: 48 8D 56 F0        lea         rdx,[rsi-10h]
  0000000061017F6A: FF 50 28           call        qword ptr [rax+28h]
  0000000061017F6D: 84 C0              test        al,al
  0000000061017F6F: 74 12              je          0000000061017F83
  0000000061017F71: 48 8B 76 08        mov         rsi,qword ptr [rsi+8]
  0000000061017F75: 48 3B F7           cmp         rsi,rdi
  0000000061017F78: 0F 84 DA 00 00 00  je          0000000061018058
  0000000061017F7E: E9 71 FF FF FF     jmp         0000000061017EF4
  0000000061017F83: 49 8B 8F E8 00 00  mov         rcx,qword ptr [r15+00000000000000E8h]
                    00
  0000000061017F8A: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000061017F8D: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000061017F95: FF 50 60           call        qword ptr [rax+60h]
  0000000061017F98: 90                 nop
  0000000061017F99: 48 8B D0           mov         rdx,rax
  0000000061017F9C: 49 8B CF           mov         rcx,r15
  0000000061017F9F: E8 AC BE FF FF     call        0000000061013E50
  0000000061017FA4: 90                 nop
  0000000061017FA5: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000061017FAD: FF 15 2D B9 02 00  call        qword ptr [00000000610438E0h]
  0000000061017FB3: 90                 nop
  0000000061017FB4: 48 8D 4B 68        lea         rcx,[rbx+68h]
  0000000061017FB8: FF 15 6A B8 02 00  call        qword ptr [0000000061043828h]
  0000000061017FBE: 84 C0              test        al,al
  0000000061017FC0: 75 3D              jne         0000000061017FFF
  0000000061017FC2: 48 8B 6B 08        mov         rbp,qword ptr [rbx+8]
  0000000061017FC6: 48 3B EB           cmp         rbp,rbx
  0000000061017FC9: 74 25              je          0000000061017FF0
  0000000061017FCB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  0000000061017FD0: 48 8B F5           mov         rsi,rbp
  0000000061017FD3: 48 8B 6D 08        mov         rbp,qword ptr [rbp+8]
  0000000061017FD7: 48 8D 4E F0        lea         rcx,[rsi-10h]
  0000000061017FDB: FF 15 FF B8 02 00  call        qword ptr [00000000610438E0h]
  0000000061017FE1: 48 8D 4E F8        lea         rcx,[rsi-8]
  0000000061017FE5: FF 15 F5 B8 02 00  call        qword ptr [00000000610438E0h]
  0000000061017FEB: 48 3B EB           cmp         rbp,rbx
  0000000061017FEE: 75 E0              jne         0000000061017FD0
  0000000061017FF0: BA 10 00 00 00     mov         edx,10h
  0000000061017FF5: 48 8B CB           mov         rcx,rbx
  0000000061017FF8: FF 15 22 B4 02 00  call        qword ptr [0000000061043420h]
  0000000061017FFE: 90                 nop
  0000000061017FFF: 48 8D 4F 68        lea         rcx,[rdi+68h]
  0000000061018003: FF 15 1F B8 02 00  call        qword ptr [0000000061043828h]
  0000000061018009: 84 C0              test        al,al
  000000006101800B: 75 44              jne         0000000061018051
  000000006101800D: 48 8B 77 08        mov         rsi,qword ptr [rdi+8]
  0000000061018011: 48 3B F7           cmp         rsi,rdi
  0000000061018014: 74 2D              je          0000000061018043
  0000000061018016: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  0000000061018020: 48 8B C6           mov         rax,rsi
  0000000061018023: 48 8B 76 08        mov         rsi,qword ptr [rsi+8]
  0000000061018027: 48 8D 58 F0        lea         rbx,[rax-10h]
  000000006101802B: 48 8B CB           mov         rcx,rbx
  000000006101802E: FF 15 AC B8 02 00  call        qword ptr [00000000610438E0h]
  0000000061018034: 48 8D 4B 08        lea         rcx,[rbx+8]
  0000000061018038: FF 15 A2 B8 02 00  call        qword ptr [00000000610438E0h]
  000000006101803E: 48 3B F7           cmp         rsi,rdi
  0000000061018041: 75 DD              jne         0000000061018020
  0000000061018043: BA 10 00 00 00     mov         edx,10h
  0000000061018048: 48 8B CF           mov         rcx,rdi
  000000006101804B: FF 15 CF B3 02 00  call        qword ptr [0000000061043420h]
  0000000061018051: 32 C0              xor         al,al
  0000000061018053: E9 AB 00 00 00     jmp         0000000061018103
  0000000061018058: 48 85 DB           test        rbx,rbx
  000000006101805B: 74 55              je          00000000610180B2
  000000006101805D: 48 8D 4B 68        lea         rcx,[rbx+68h]
  0000000061018061: FF 15 C1 B7 02 00  call        qword ptr [0000000061043828h]
  0000000061018067: 84 C0              test        al,al
  0000000061018069: 75 47              jne         00000000610180B2
  000000006101806B: 48 8B 6B 08        mov         rbp,qword ptr [rbx+8]
  000000006101806F: 48 3B EB           cmp         rbp,rbx
  0000000061018072: 74 2F              je          00000000610180A3
  0000000061018074: 66 66 66 66 0F 1F  nop         word ptr [rax+rax+0000000000000000h]
                    84 00 00 00 00 00
  0000000061018080: 48 8B C5           mov         rax,rbp
  0000000061018083: 48 8B 6D 08        mov         rbp,qword ptr [rbp+8]
  0000000061018087: 48 8D 70 F0        lea         rsi,[rax-10h]
  000000006101808B: 48 8B CE           mov         rcx,rsi
  000000006101808E: FF 15 4C B8 02 00  call        qword ptr [00000000610438E0h]
  0000000061018094: 48 8D 4E 08        lea         rcx,[rsi+8]
  0000000061018098: FF 15 42 B8 02 00  call        qword ptr [00000000610438E0h]
  000000006101809E: 48 3B EB           cmp         rbp,rbx
  00000000610180A1: 75 DD              jne         0000000061018080
  00000000610180A3: BA 10 00 00 00     mov         edx,10h
  00000000610180A8: 48 8B CB           mov         rcx,rbx
  00000000610180AB: FF 15 6F B3 02 00  call        qword ptr [0000000061043420h]
  00000000610180B1: 90                 nop
  00000000610180B2: 48 85 FF           test        rdi,rdi
  00000000610180B5: 74 4A              je          0000000061018101
  00000000610180B7: 48 8D 4F 68        lea         rcx,[rdi+68h]
  00000000610180BB: FF 15 67 B7 02 00  call        qword ptr [0000000061043828h]
  00000000610180C1: 84 C0              test        al,al
  00000000610180C3: 75 3C              jne         0000000061018101
  00000000610180C5: 48 8B 77 08        mov         rsi,qword ptr [rdi+8]
  00000000610180C9: 48 3B F7           cmp         rsi,rdi
  00000000610180CC: 74 25              je          00000000610180F3
  00000000610180CE: 66 90              nop
  00000000610180D0: 48 8B C6           mov         rax,rsi
  00000000610180D3: 48 8B 76 08        mov         rsi,qword ptr [rsi+8]
  00000000610180D7: 48 8D 58 F0        lea         rbx,[rax-10h]
  00000000610180DB: 48 8B CB           mov         rcx,rbx
  00000000610180DE: FF 15 FC B7 02 00  call        qword ptr [00000000610438E0h]
  00000000610180E4: 48 8D 4B 08        lea         rcx,[rbx+8]
  00000000610180E8: FF 15 F2 B7 02 00  call        qword ptr [00000000610438E0h]
  00000000610180EE: 48 3B F7           cmp         rsi,rdi
  00000000610180F1: 75 DD              jne         00000000610180D0
  00000000610180F3: BA 10 00 00 00     mov         edx,10h
  00000000610180F8: 48 8B CF           mov         rcx,rdi
  00000000610180FB: FF 15 1F B3 02 00  call        qword ptr [0000000061043420h]
  0000000061018101: B0 01              mov         al,1
  0000000061018103: 48 8B 9C 24 98 00  mov         rbx,qword ptr [rsp+0000000000000098h]
                    00 00
  000000006101810B: 48 83 C4 40        add         rsp,40h
  000000006101810F: 41 5F              pop         r15
  0000000061018111: 41 5E              pop         r14
  0000000061018113: 41 5D              pop         r13
  0000000061018115: 41 5C              pop         r12
  0000000061018117: 5F                 pop         rdi
  0000000061018118: 5E                 pop         rsi
  0000000061018119: 5D                 pop         rbp
  000000006101811A: C3                 ret

  Summary

       42000 .text
