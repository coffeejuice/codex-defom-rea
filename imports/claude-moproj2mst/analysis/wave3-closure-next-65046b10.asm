Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065046B10: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000065046B15: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065046B1A: 53                 push        rbx
  0000000065046B1B: 56                 push        rsi
  0000000065046B1C: 57                 push        rdi
  0000000065046B1D: 41 54              push        r12
  0000000065046B1F: 41 55              push        r13
  0000000065046B21: 48 83 EC 40        sub         rsp,40h
  0000000065046B25: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065046B2E: 4D 8B E9           mov         r13,r9
  0000000065046B31: 49 8B F8           mov         rdi,r8
  0000000065046B34: 4C 8B E1           mov         r12,rcx
  0000000065046B37: 4D 8B C1           mov         r8,r9
  0000000065046B3A: 48 85 FF           test        rdi,rdi
  0000000065046B3D: 4C 0F 45 C7        cmovne      r8,rdi
  0000000065046B41: FF 15 E9 FE 64 00  call        qword ptr [0000000065696A30h]
  0000000065046B47: 90                 nop
  0000000065046B48: 41 C7 44 24 18 00  mov         dword ptr [r12+18h],0
                    00 00 00
  0000000065046B51: 48 8D 05 E0 E8 65  lea         rax,[00000000656A5438h]
                    00
  0000000065046B58: 49 89 04 24        mov         qword ptr [r12],rax
  0000000065046B5C: 48 8D 05 55 E8 65  lea         rax,[00000000656A53B8h]
                    00
  0000000065046B63: 49 89 44 24 10     mov         qword ptr [r12+10h],rax
  0000000065046B68: 49 8B 74 24 08     mov         rsi,qword ptr [r12+8]
  0000000065046B6D: 48 85 FF           test        rdi,rdi
  0000000065046B70: 74 13              je          0000000065046B85
  0000000065046B72: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000065046B75: 49 8D 54 24 10     lea         rdx,[r12+10h]
  0000000065046B7A: 48 8B CF           mov         rcx,rdi
  0000000065046B7D: FF 50 58           call        qword ptr [rax+58h]
  0000000065046B80: E9 00 01 00 00     jmp         0000000065046C85
  0000000065046B85: 4D 85 ED           test        r13,r13
  0000000065046B88: 0F 84 F7 00 00 00  je          0000000065046C85
  0000000065046B8E: 49 8B CD           mov         rcx,r13
  0000000065046B91: E8 8A 73 01 00     call        000000006505DF20
  0000000065046B96: 48 85 C0           test        rax,rax
  0000000065046B99: 0F 84 C9 00 00 00  je          0000000065046C68
  0000000065046B9F: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065046BA4: 49 8B CD           mov         rcx,r13
  0000000065046BA7: FF 15 A3 02 65 00  call        qword ptr [0000000065696E50h]
  0000000065046BAD: 90                 nop
  0000000065046BAE: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000065046BB3: 48 8B C8           mov         rcx,rax
  0000000065046BB6: FF 15 8C 02 65 00  call        qword ptr [0000000065696E48h]
  0000000065046BBC: 48 8B D8           mov         rbx,rax
  0000000065046BBF: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000065046BC7: 49 8B CC           mov         rcx,r12
  0000000065046BCA: FF 15 80 02 65 00  call        qword ptr [0000000065696E50h]
  0000000065046BD0: 90                 nop
  0000000065046BD1: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000065046BD9: 48 8B C8           mov         rcx,rax
  0000000065046BDC: FF 15 66 02 65 00  call        qword ptr [0000000065696E48h]
  0000000065046BE2: 48 8B F0           mov         rsi,rax
  0000000065046BE5: 48 8B CB           mov         rcx,rbx
  0000000065046BE8: FF 15 52 02 65 00  call        qword ptr [0000000065696E40h]
  0000000065046BEE: 48 8B F8           mov         rdi,rax
  0000000065046BF1: 49 8B 55 00        mov         rdx,qword ptr [r13]
  0000000065046BF5: 49 8B CD           mov         rcx,r13
  0000000065046BF8: FF 12              call        qword ptr [rdx]
  0000000065046BFA: 48 8B C8           mov         rcx,rax
  0000000065046BFD: FF 15 65 01 65 00  call        qword ptr [0000000065696D68h]
  0000000065046C03: 48 8B D8           mov         rbx,rax
  0000000065046C06: 48 8B CE           mov         rcx,rsi
  0000000065046C09: FF 15 49 FE 64 00  call        qword ptr [0000000065696A58h]
  0000000065046C0F: 48 8B D0           mov         rdx,rax
  0000000065046C12: 4C 8B CF           mov         r9,rdi
  0000000065046C15: 4C 8B C3           mov         r8,rbx
  0000000065046C18: 48 8D 0D 11 EB 65  lea         rcx,[00000000656A5730h]
                    00
  0000000065046C1F: FF 15 73 FD 64 00  call        qword ptr [0000000065696998h]
  0000000065046C25: 90                 nop
  0000000065046C26: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000065046C2E: FF 15 1C FE 64 00  call        qword ptr [0000000065696A50h]
  0000000065046C34: 90                 nop
  0000000065046C35: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000065046C3D: FF 15 1D FB 64 00  call        qword ptr [0000000065696760h]
  0000000065046C43: 90                 nop
  0000000065046C44: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000065046C49: FF 15 01 FE 64 00  call        qword ptr [0000000065696A50h]
  0000000065046C4F: 90                 nop
  0000000065046C50: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065046C55: FF 15 05 FB 64 00  call        qword ptr [0000000065696760h]
  0000000065046C5B: 33 D2              xor         edx,edx
  0000000065046C5D: 49 8B CC           mov         rcx,r12
  0000000065046C60: FF 15 AA FE 64 00  call        qword ptr [0000000065696B10h]
  0000000065046C66: EB 1D              jmp         0000000065046C85
  0000000065046C68: 83 8E 9C 00 00 00  or          dword ptr [rsi+000000000000009Ch],1
                    01
  0000000065046C6F: 49 8B 45 08        mov         rax,qword ptr [r13+8]
  0000000065046C73: 4C 89 A0 A8 00 00  mov         qword ptr [rax+00000000000000A8h],r12
                    00
  0000000065046C7A: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  0000000065046C7F: E8 DC FA FF FF     call        0000000065046760
  0000000065046C84: 90                 nop
  0000000065046C85: 49 8B C4           mov         rax,r12
  0000000065046C88: 48 83 C4 40        add         rsp,40h
  0000000065046C8C: 41 5D              pop         r13
  0000000065046C8E: 41 5C              pop         r12
  0000000065046C90: 5F                 pop         rdi
  0000000065046C91: 5E                 pop         rsi
  0000000065046C92: 5B                 pop         rbx
  0000000065046C93: C3                 ret

  Summary

      695000 .text
