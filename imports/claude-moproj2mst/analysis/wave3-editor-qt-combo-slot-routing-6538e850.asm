Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006538E850: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000006538E854: 55                 push        rbp
  000000006538E855: 56                 push        rsi
  000000006538E856: 57                 push        rdi
  000000006538E857: 41 54              push        r12
  000000006538E859: 41 55              push        r13
  000000006538E85B: 41 56              push        r14
  000000006538E85D: 41 57              push        r15
  000000006538E85F: 48 81 EC D0 00 00  sub         rsp,0D0h
                    00
  000000006538E866: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006538E86F: 48 89 9C 24 20 01  mov         qword ptr [rsp+0000000000000120h],rbx
                    00 00
  000000006538E877: 49 8B F1           mov         rsi,r9
  000000006538E87A: 4D 8B E0           mov         r12,r8
  000000006538E87D: 8B FA              mov         edi,edx
  000000006538E87F: 4C 8B 69 08        mov         r13,qword ptr [rcx+8]
  000000006538E883: 49 8B 85 E0 01 00  mov         rax,qword ptr [r13+00000000000001E0h]
                    00
  000000006538E88A: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538E88D: 49 8D 8D 50 02 00  lea         rcx,[r13+0000000000000250h]
                    00
  000000006538E894: FF 15 B6 9F 30 00  call        qword ptr [0000000065698850h]
  000000006538E89A: 48 8B D0           mov         rdx,rax
  000000006538E89D: 49 8B 8D E0 01 00  mov         rcx,qword ptr [r13+00000000000001E0h]
                    00
  000000006538E8A4: FF 53 68           call        qword ptr [rbx+68h]
  000000006538E8A7: 44 8B F0           mov         r14d,eax
  000000006538E8AA: 89 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],eax
                    00
  000000006538E8B1: 33 DB              xor         ebx,ebx
  000000006538E8B3: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  000000006538E8B7: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  000000006538E8BF: 48 8D 84 24 18 01  lea         rax,[rsp+0000000000000118h]
                    00 00
  000000006538E8C7: 44 3B F7           cmp         r14d,edi
  000000006538E8CA: 48 0F 4D D0        cmovge      rdx,rax
  000000006538E8CE: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006538E8D3: 39 1A              cmp         dword ptr [rdx],ebx
  000000006538E8D5: 48 0F 4F CA        cmovg       rcx,rdx
  000000006538E8D9: 44 8B 39           mov         r15d,dword ptr [rcx]
  000000006538E8DC: 45 3B BD 14 02 00  cmp         r15d,dword ptr [r13+0000000000000214h]
                    00
  000000006538E8E3: 0F 8D 1C 05 00 00  jge         000000006538EE05
  000000006538E8E9: 49 8B 95 E0 01 00  mov         rdx,qword ptr [r13+00000000000001E0h]
                    00
  000000006538E8F0: 48 8D 0D 89 87 63  lea         rcx,[00000000659C7080h]
                    00
  000000006538E8F7: FF 15 0B 81 30 00  call        qword ptr [0000000065696A08h]
  000000006538E8FD: 48 8B E8           mov         rbp,rax
  000000006538E900: 48 85 C0           test        rax,rax
  000000006538E903: 0F 84 9A 00 00 00  je          000000006538E9A3
  000000006538E909: 8D 4B 10           lea         ecx,[rbx+10h]
  000000006538E90C: E8 61 28 25 00     call        00000000655E1172
  000000006538E911: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  000000006538E919: 48 85 C0           test        rax,rax
  000000006538E91C: 74 0E              je          000000006538E92C
  000000006538E91E: 48 8B D6           mov         rdx,rsi
  000000006538E921: 48 8B C8           mov         rcx,rax
  000000006538E924: E8 E7 C4 19 00     call        000000006552AE10
  000000006538E929: 48 8B D8           mov         rbx,rax
  000000006538E92C: 49 8B CC           mov         rcx,r12
  000000006538E92F: E8 AC 20 D1 FF     call        00000000650A09E0
  000000006538E934: 84 C0              test        al,al
  000000006538E936: 75 2E              jne         000000006538E966
  000000006538E938: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006538E93D: 49 8B CC           mov         rcx,r12
  000000006538E940: E8 AB 1F D1 FF     call        00000000650A08F0
  000000006538E945: 90                 nop
  000000006538E946: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006538E949: 41 B8 01 00 00 00  mov         r8d,1
  000000006538E94F: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006538E954: 48 8B CB           mov         rcx,rbx
  000000006538E957: FF 50 10           call        qword ptr [rax+10h]
  000000006538E95A: 90                 nop
  000000006538E95B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538E960: FF 15 7A 7D 30 00  call        qword ptr [00000000656966E0h]
  000000006538E966: 48 8B BC 24 30 01  mov         rdi,qword ptr [rsp+0000000000000130h]
                    00 00
  000000006538E96E: 48 8B CF           mov         rcx,rdi
  000000006538E971: FF 15 69 98 30 00  call        qword ptr [00000000656981E0h]
  000000006538E977: 84 C0              test        al,al
  000000006538E979: 74 12              je          000000006538E98D
  000000006538E97B: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006538E97E: 41 B8 20 00 00 00  mov         r8d,20h
  000000006538E984: 48 8B D7           mov         rdx,rdi
  000000006538E987: 48 8B CB           mov         rcx,rbx
  000000006538E98A: FF 50 10           call        qword ptr [rax+10h]
  000000006538E98D: 4C 8B C3           mov         r8,rbx
  000000006538E990: 41 8B D7           mov         edx,r15d
  000000006538E993: 48 8B CD           mov         rcx,rbp
  000000006538E996: E8 75 B8 FF FF     call        000000006538A210
  000000006538E99B: 41 FF C6           inc         r14d
  000000006538E99E: E9 24 04 00 00     jmp         000000006538EDC7
  000000006538E9A3: 41 C6 85 1C 02 00  mov         byte ptr [r13+000000000000021Ch],1
                    00 01
  000000006538E9AB: 49 8B 85 E0 01 00  mov         rax,qword ptr [r13+00000000000001E0h]
                    00
  000000006538E9B2: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538E9B5: 49 8D 8D 50 02 00  lea         rcx,[r13+0000000000000250h]
                    00
  000000006538E9BC: FF 15 8E 9E 30 00  call        qword ptr [0000000065698850h]
  000000006538E9C2: 4C 8B C8           mov         r9,rax
  000000006538E9C5: 41 B8 01 00 00 00  mov         r8d,1
  000000006538E9CB: 41 8B D7           mov         edx,r15d
  000000006538E9CE: 49 8B 8D E0 01 00  mov         rcx,qword ptr [r13+00000000000001E0h]
                    00
  000000006538E9D5: FF 93 D0 00 00 00  call        qword ptr [rbx+00000000000000D0h]
  000000006538E9DB: 84 C0              test        al,al
  000000006538E9DD: 0F 84 DC 03 00 00  je          000000006538EDBF
  000000006538E9E3: 49 8B 85 E0 01 00  mov         rax,qword ptr [r13+00000000000001E0h]
                    00
  000000006538E9EA: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538E9ED: 49 8D 8D 50 02 00  lea         rcx,[r13+0000000000000250h]
                    00
  000000006538E9F4: FF 15 56 9E 30 00  call        qword ptr [0000000065698850h]
  000000006538E9FA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538E9FF: 45 8B 8D 18 02 00  mov         r9d,dword ptr [r13+0000000000000218h]
                    00
  000000006538EA06: 45 8B C7           mov         r8d,r15d
  000000006538EA09: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006538EA0E: 49 8B 8D E0 01 00  mov         rcx,qword ptr [r13+00000000000001E0h]
                    00
  000000006538EA15: FF 53 58           call        qword ptr [rbx+58h]
  000000006538EA18: 90                 nop
  000000006538EA19: 49 8B CC           mov         rcx,r12
  000000006538EA1C: E8 BF 1F D1 FF     call        00000000650A09E0
  000000006538EA21: 48 8B 9C 24 30 01  mov         rbx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000006538EA29: 84 C0              test        al,al
  000000006538EA2B: 74 4D              je          000000006538EA7A
  000000006538EA2D: 48 8B CB           mov         rcx,rbx
  000000006538EA30: FF 15 AA 97 30 00  call        qword ptr [00000000656981E0h]
  000000006538EA36: 84 C0              test        al,al
  000000006538EA38: 75 40              jne         000000006538EA7A
  000000006538EA3A: 48 8B D6           mov         rdx,rsi
  000000006538EA3D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538EA42: FF 15 18 88 30 00  call        qword ptr [0000000065697260h]
  000000006538EA48: 90                 nop
  000000006538EA49: 49 8B 8D E0 01 00  mov         rcx,qword ptr [r13+00000000000001E0h]
                    00
  000000006538EA50: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006538EA53: 41 B9 02 00 00 00  mov         r9d,2
  000000006538EA59: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  000000006538EA5E: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006538EA63: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  000000006538EA69: 90                 nop
  000000006538EA6A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538EA6F: FF 15 6B 7C 30 00  call        qword ptr [00000000656966E0h]
  000000006538EA75: E9 1A 03 00 00     jmp         000000006538ED94
  000000006538EA7A: 48 8B 0D 6F 7E 30  mov         rcx,qword ptr [00000000656968F0h]
                    00
  000000006538EA81: 48 89 8C 24 10 01  mov         qword ptr [rsp+0000000000000110h],rcx
                    00 00
  000000006538EA89: 48 83 C1 68        add         rcx,68h
  000000006538EA8D: FF 15 2D 7E 30 00  call        qword ptr [00000000656968C0h]
  000000006538EA93: 90                 nop
  000000006538EA94: 48 8B CE           mov         rcx,rsi
  000000006538EA97: FF 15 4B 89 30 00  call        qword ptr [00000000656973E8h]
  000000006538EA9D: 84 C0              test        al,al
  000000006538EA9F: 0F 85 D5 00 00 00  jne         000000006538EB7A
  000000006538EAA5: 48 8B D6           mov         rdx,rsi
  000000006538EAA8: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538EAAD: FF 15 AD 87 30 00  call        qword ptr [0000000065697260h]
  000000006538EAB3: 90                 nop
  000000006538EAB4: C7 84 24 18 01 00  mov         dword ptr [rsp+0000000000000118h],2
                    00 02 00 00 00
  000000006538EABF: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538EAC7: 48 83 C1 68        add         rcx,68h
  000000006538EACB: BA 01 00 00 00     mov         edx,1
  000000006538EAD0: FF 15 9A 7F 30 00  call        qword ptr [0000000065696A70h]
  000000006538EAD6: 84 C0              test        al,al
  000000006538EAD8: 74 0D              je          000000006538EAE7
  000000006538EADA: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000006538EAE2: E8 B9 81 EA FF     call        0000000065236CA0
  000000006538EAE7: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538EAEF: 4C 8B CA           mov         r9,rdx
  000000006538EAF2: 48 63 42 6C        movsxd      rax,dword ptr [rdx+6Ch]
  000000006538EAF6: 4C 8B C0           mov         r8,rax
  000000006538EAF9: 85 C0              test        eax,eax
  000000006538EAFB: 78 4D              js          000000006538EB4A
  000000006538EAFD: 0F 1F 00           nop         dword ptr [rax]
  000000006538EB00: 4B 8B 4C C1 08     mov         rcx,qword ptr [r9+r8*8+8]
  000000006538EB05: 48 3B CA           cmp         rcx,rdx
  000000006538EB08: 74 19              je          000000006538EB23
  000000006538EB0A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006538EB10: 83 79 E8 02        cmp         dword ptr [rcx-18h],2
  000000006538EB14: 7D 0D              jge         000000006538EB23
  000000006538EB16: 4C 8B C9           mov         r9,rcx
  000000006538EB19: 4A 8B 4C C1 08     mov         rcx,qword ptr [rcx+r8*8+8]
  000000006538EB1E: 48 3B CA           cmp         rcx,rdx
  000000006538EB21: 75 ED              jne         000000006538EB10
  000000006538EB23: 4E 89 4C C4 70     mov         qword ptr [rsp+r8*8+70h],r9
  000000006538EB28: 49 83 E8 01        sub         r8,1
  000000006538EB2C: 79 D2              jns         000000006538EB00
  000000006538EB2E: 48 3B CA           cmp         rcx,rdx
  000000006538EB31: 74 17              je          000000006538EB4A
  000000006538EB33: 83 79 E8 02        cmp         dword ptr [rcx-18h],2
  000000006538EB37: 7F 11              jg          000000006538EB4A
  000000006538EB39: 48 83 C1 F0        add         rcx,0FFFFFFFFFFFFFFF0h
  000000006538EB3D: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006538EB42: FF 15 48 7C 30 00  call        qword ptr [0000000065696790h]
  000000006538EB48: EB 25              jmp         000000006538EB6F
  000000006538EB4A: 48 8D 44 24 30     lea         rax,[rsp+30h]
  000000006538EB4F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538EB54: 4C 8D 8C 24 18 01  lea         r9,[rsp+0000000000000118h]
                    00 00
  000000006538EB5C: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  000000006538EB61: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000006538EB69: E8 32 7C EA FF     call        00000000652367A0
  000000006538EB6E: 90                 nop
  000000006538EB6F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538EB74: FF 15 66 7B 30 00  call        qword ptr [00000000656966E0h]
  000000006538EB7A: 49 8B CC           mov         rcx,r12
  000000006538EB7D: E8 5E 1E D1 FF     call        00000000650A09E0
  000000006538EB82: 84 C0              test        al,al
  000000006538EB84: 0F 85 D0 00 00 00  jne         000000006538EC5A
  000000006538EB8A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006538EB8F: 49 8B CC           mov         rcx,r12
  000000006538EB92: E8 59 1D D1 FF     call        00000000650A08F0
  000000006538EB97: 90                 nop
  000000006538EB98: C7 84 24 18 01 00  mov         dword ptr [rsp+0000000000000118h],1
                    00 01 00 00 00
  000000006538EBA3: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538EBAB: 48 83 C1 68        add         rcx,68h
  000000006538EBAF: BA 01 00 00 00     mov         edx,1
  000000006538EBB4: FF 15 B6 7E 30 00  call        qword ptr [0000000065696A70h]
  000000006538EBBA: 84 C0              test        al,al
  000000006538EBBC: 74 0D              je          000000006538EBCB
  000000006538EBBE: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000006538EBC6: E8 D5 80 EA FF     call        0000000065236CA0
  000000006538EBCB: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538EBD3: 4C 8B CA           mov         r9,rdx
  000000006538EBD6: 48 63 42 6C        movsxd      rax,dword ptr [rdx+6Ch]
  000000006538EBDA: 4C 8B C0           mov         r8,rax
  000000006538EBDD: 85 C0              test        eax,eax
  000000006538EBDF: 78 49              js          000000006538EC2A
  000000006538EBE1: 4B 8B 4C C1 08     mov         rcx,qword ptr [r9+r8*8+8]
  000000006538EBE6: 48 3B CA           cmp         rcx,rdx
  000000006538EBE9: 74 18              je          000000006538EC03
  000000006538EBEB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  000000006538EBF0: 83 79 E8 01        cmp         dword ptr [rcx-18h],1
  000000006538EBF4: 7D 0D              jge         000000006538EC03
  000000006538EBF6: 4C 8B C9           mov         r9,rcx
  000000006538EBF9: 4A 8B 4C C1 08     mov         rcx,qword ptr [rcx+r8*8+8]
  000000006538EBFE: 48 3B CA           cmp         rcx,rdx
  000000006538EC01: 75 ED              jne         000000006538EBF0
  000000006538EC03: 4E 89 4C C4 70     mov         qword ptr [rsp+r8*8+70h],r9
  000000006538EC08: 49 83 E8 01        sub         r8,1
  000000006538EC0C: 79 D3              jns         000000006538EBE1
  000000006538EC0E: 48 3B CA           cmp         rcx,rdx
  000000006538EC11: 74 17              je          000000006538EC2A
  000000006538EC13: 83 79 E8 01        cmp         dword ptr [rcx-18h],1
  000000006538EC17: 7F 11              jg          000000006538EC2A
  000000006538EC19: 48 83 C1 F0        add         rcx,0FFFFFFFFFFFFFFF0h
  000000006538EC1D: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006538EC22: FF 15 68 7B 30 00  call        qword ptr [0000000065696790h]
  000000006538EC28: EB 25              jmp         000000006538EC4F
  000000006538EC2A: 48 8D 44 24 30     lea         rax,[rsp+30h]
  000000006538EC2F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538EC34: 4C 8D 8C 24 18 01  lea         r9,[rsp+0000000000000118h]
                    00 00
  000000006538EC3C: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  000000006538EC41: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000006538EC49: E8 52 7B EA FF     call        00000000652367A0
  000000006538EC4E: 90                 nop
  000000006538EC4F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006538EC54: FF 15 86 7A 30 00  call        qword ptr [00000000656966E0h]
  000000006538EC5A: 48 8B CB           mov         rcx,rbx
  000000006538EC5D: FF 15 7D 95 30 00  call        qword ptr [00000000656981E0h]
  000000006538EC63: 84 C0              test        al,al
  000000006538EC65: 0F 84 AC 00 00 00  je          000000006538ED17
  000000006538EC6B: C7 84 24 18 01 00  mov         dword ptr [rsp+0000000000000118h],20h
                    00 20 00 00 00
  000000006538EC76: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538EC7E: 48 83 C1 68        add         rcx,68h
  000000006538EC82: BA 01 00 00 00     mov         edx,1
  000000006538EC87: FF 15 E3 7D 30 00  call        qword ptr [0000000065696A70h]
  000000006538EC8D: 84 C0              test        al,al
  000000006538EC8F: 74 0D              je          000000006538EC9E
  000000006538EC91: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000006538EC99: E8 02 80 EA FF     call        0000000065236CA0
  000000006538EC9E: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538ECA6: 4C 8B CA           mov         r9,rdx
  000000006538ECA9: 48 63 42 6C        movsxd      rax,dword ptr [rdx+6Ch]
  000000006538ECAD: 4C 8B C0           mov         r8,rax
  000000006538ECB0: 85 C0              test        eax,eax
  000000006538ECB2: 78 44              js          000000006538ECF8
  000000006538ECB4: 4B 8B 4C C1 08     mov         rcx,qword ptr [r9+r8*8+8]
  000000006538ECB9: 48 3B CA           cmp         rcx,rdx
  000000006538ECBC: 74 15              je          000000006538ECD3
  000000006538ECBE: 66 90              nop
  000000006538ECC0: 83 79 E8 20        cmp         dword ptr [rcx-18h],20h
  000000006538ECC4: 7D 0D              jge         000000006538ECD3
  000000006538ECC6: 4C 8B C9           mov         r9,rcx
  000000006538ECC9: 4A 8B 4C C1 08     mov         rcx,qword ptr [rcx+r8*8+8]
  000000006538ECCE: 48 3B CA           cmp         rcx,rdx
  000000006538ECD1: 75 ED              jne         000000006538ECC0
  000000006538ECD3: 4E 89 4C C4 70     mov         qword ptr [rsp+r8*8+70h],r9
  000000006538ECD8: 49 83 E8 01        sub         r8,1
  000000006538ECDC: 79 D6              jns         000000006538ECB4
  000000006538ECDE: 48 3B CA           cmp         rcx,rdx
  000000006538ECE1: 74 15              je          000000006538ECF8
  000000006538ECE3: 83 79 E8 20        cmp         dword ptr [rcx-18h],20h
  000000006538ECE7: 7F 0F              jg          000000006538ECF8
  000000006538ECE9: 48 83 C1 F0        add         rcx,0FFFFFFFFFFFFFFF0h
  000000006538ECED: 48 8B D3           mov         rdx,rbx
  000000006538ECF0: FF 15 9A 7A 30 00  call        qword ptr [0000000065696790h]
  000000006538ECF6: EB 1F              jmp         000000006538ED17
  000000006538ECF8: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  000000006538ECFD: 4C 8D 8C 24 18 01  lea         r9,[rsp+0000000000000118h]
                    00 00
  000000006538ED05: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  000000006538ED0A: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000006538ED12: E8 89 7A EA FF     call        00000000652367A0
  000000006538ED17: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538ED1F: 83 79 70 00        cmp         dword ptr [rcx+70h],0
  000000006538ED23: 74 25              je          000000006538ED4A
  000000006538ED25: 49 8B 8D E0 01 00  mov         rcx,qword ptr [r13+00000000000001E0h]
                    00
  000000006538ED2C: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006538ED2F: 4C 8D 84 24 10 01  lea         r8,[rsp+0000000000000110h]
                    00 00
  000000006538ED37: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006538ED3C: FF 90 A8 00 00 00  call        qword ptr [rax+00000000000000A8h]
  000000006538ED42: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538ED4A: 48 85 C9           test        rcx,rcx
  000000006538ED4D: 74 45              je          000000006538ED94
  000000006538ED4F: 48 83 C1 68        add         rcx,68h
  000000006538ED53: FF 15 1F 7C 30 00  call        qword ptr [0000000065696978h]
  000000006538ED59: 84 C0              test        al,al
  000000006538ED5B: 75 37              jne         000000006538ED94
  000000006538ED5D: 48 8B BC 24 10 01  mov         rdi,qword ptr [rsp+0000000000000110h]
                    00 00
  000000006538ED65: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  000000006538ED69: 48 3B DF           cmp         rbx,rdi
  000000006538ED6C: 74 18              je          000000006538ED86
  000000006538ED6E: 66 90              nop
  000000006538ED70: 48 8B CB           mov         rcx,rbx
  000000006538ED73: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  000000006538ED77: 48 83 C1 F0        add         rcx,0FFFFFFFFFFFFFFF0h
  000000006538ED7B: FF 15 5F 79 30 00  call        qword ptr [00000000656966E0h]
  000000006538ED81: 48 3B DF           cmp         rbx,rdi
  000000006538ED84: 75 EA              jne         000000006538ED70
  000000006538ED86: BA 18 00 00 00     mov         edx,18h
  000000006538ED8B: 48 8B CF           mov         rcx,rdi
  000000006538ED8E: FF 15 C4 7B 30 00  call        qword ptr [0000000065696958h]
  000000006538ED94: 41 C6 85 1C 02 00  mov         byte ptr [r13+000000000000021Ch],0
                    00 00
  000000006538ED9C: 49 8D 8D 50 02 00  lea         rcx,[r13+0000000000000250h]
                    00
  000000006538EDA3: FF 15 A7 9A 30 00  call        qword ptr [0000000065698850h]
  000000006538EDA9: 48 8B D0           mov         rdx,rax
  000000006538EDAC: 45 8B CF           mov         r9d,r15d
  000000006538EDAF: 45 8B C7           mov         r8d,r15d
  000000006538EDB2: 49 8B CD           mov         rcx,r13
  000000006538EDB5: E8 06 F5 FF FF     call        000000006538E2C0
  000000006538EDBA: 41 FF C6           inc         r14d
  000000006538EDBD: EB 08              jmp         000000006538EDC7
  000000006538EDBF: 41 C6 85 1C 02 00  mov         byte ptr [r13+000000000000021Ch],0
                    00 00
  000000006538EDC7: 45 3B B5 14 02 00  cmp         r14d,dword ptr [r13+0000000000000214h]
                    00
  000000006538EDCE: 7E 35              jle         000000006538EE05
  000000006538EDD0: 49 8B 85 E0 01 00  mov         rax,qword ptr [r13+00000000000001E0h]
                    00
  000000006538EDD7: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006538EDDA: 49 8D 8D 50 02 00  lea         rcx,[r13+0000000000000250h]
                    00
  000000006538EDE1: FF 15 69 9A 30 00  call        qword ptr [0000000065698850h]
  000000006538EDE7: 4C 8B C8           mov         r9,rax
  000000006538EDEA: 45 8B C6           mov         r8d,r14d
  000000006538EDED: 45 2B 85 14 02 00  sub         r8d,dword ptr [r13+0000000000000214h]
                    00
  000000006538EDF4: 41 8D 56 FF        lea         edx,[r14-1]
  000000006538EDF8: 49 8B 8D E0 01 00  mov         rcx,qword ptr [r13+00000000000001E0h]
                    00
  000000006538EDFF: FF 93 E0 00 00 00  call        qword ptr [rbx+00000000000000E0h]
  000000006538EE05: 48 8B 9C 24 20 01  mov         rbx,qword ptr [rsp+0000000000000120h]
                    00 00
  000000006538EE0D: 48 81 C4 D0 00 00  add         rsp,0D0h
                    00
  000000006538EE14: 41 5F              pop         r15
  000000006538EE16: 41 5E              pop         r14
  000000006538EE18: 41 5D              pop         r13
  000000006538EE1A: 41 5C              pop         r12
  000000006538EE1C: 5F                 pop         rdi
  000000006538EE1D: 5E                 pop         rsi
  000000006538EE1E: 5D                 pop         rbp
  000000006538EE1F: C3                 ret

  Summary

      695000 .text
