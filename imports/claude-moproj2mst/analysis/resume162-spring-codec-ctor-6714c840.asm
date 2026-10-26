Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714C840: 40 57              push        rdi
  000000006714C842: 48 83 EC 30        sub         rsp,30h
  000000006714C846: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714C84F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006714C854: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  000000006714C859: 48 83 3D CF 7B 1A  cmp         qword ptr [00000000672F4430h],0
                    00 00
  000000006714C861: 0F 85 A0 02 00 00  jne         000000006714CB07
  000000006714C867: B9 08 00 00 00     mov         ecx,8
  000000006714C86C: E8 15 C2 02 00     call        0000000067178A86
  000000006714C871: 48 85 C0           test        rax,rax
  000000006714C874: 74 14              je          000000006714C88A
  000000006714C876: 48 8D 0D D3 41 1A  lea         rcx,[00000000672F0A50h]
                    00
  000000006714C87D: 48 89 08           mov         qword ptr [rax],rcx
  000000006714C880: F0 83 05 C8 41 1A  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  000000006714C888: EB 02              jmp         000000006714C88C
  000000006714C88A: 33 C0              xor         eax,eax
  000000006714C88C: 48 89 05 9D 7B 1A  mov         qword ptr [00000000672F4430h],rax
                    00
  000000006714C893: E8 A8 FB FF FF     call        000000006714C440
  000000006714C898: B9 08 00 00 00     mov         ecx,8
  000000006714C89D: E8 E4 C1 02 00     call        0000000067178A86
  000000006714C8A2: 48 8B D8           mov         rbx,rax
  000000006714C8A5: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C8AA: 48 85 C0           test        rax,rax
  000000006714C8AD: 74 12              je          000000006714C8C1
  000000006714C8AF: 48 8B C8           mov         rcx,rax
  000000006714C8B2: E8 E9 F8 FF FF     call        000000006714C1A0
  000000006714C8B7: 4C 8D 1D 22 93 0F  lea         r11,[0000000067245BE0h]
                    00
  000000006714C8BE: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714C8C1: 33 DB              xor         ebx,ebx
  000000006714C8C3: 48 8D 35 76 5F 0F  lea         rsi,[0000000067242840h]
                    00
  000000006714C8CA: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006714C8D0: B9 10 00 00 00     mov         ecx,10h
  000000006714C8D5: E8 AC C1 02 00     call        0000000067178A86
  000000006714C8DA: 48 8B F8           mov         rdi,rax
  000000006714C8DD: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C8E2: 48 85 C0           test        rax,rax
  000000006714C8E5: 74 0E              je          000000006714C8F5
  000000006714C8E7: 48 8B C8           mov         rcx,rax
  000000006714C8EA: E8 B1 F8 FF FF     call        000000006714C1A0
  000000006714C8EF: 48 89 37           mov         qword ptr [rdi],rsi
  000000006714C8F2: 89 5F 08           mov         dword ptr [rdi+8],ebx
  000000006714C8F5: FF C3              inc         ebx
  000000006714C8F7: 83 FB 09           cmp         ebx,9
  000000006714C8FA: 7C D4              jl          000000006714C8D0
  000000006714C8FC: 33 DB              xor         ebx,ebx
  000000006714C8FE: 66 90              nop
  000000006714C900: B9 18 00 00 00     mov         ecx,18h
  000000006714C905: E8 7C C1 02 00     call        0000000067178A86
  000000006714C90A: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C90F: 48 85 C0           test        rax,rax
  000000006714C912: 74 0B              je          000000006714C91F
  000000006714C914: 8B D3              mov         edx,ebx
  000000006714C916: 48 8B C8           mov         rcx,rax
  000000006714C919: E8 92 D7 FF FF     call        000000006714A0B0
  000000006714C91E: 90                 nop
  000000006714C91F: FF C3              inc         ebx
  000000006714C921: 83 FB 1E           cmp         ebx,1Eh
  000000006714C924: 7C DA              jl          000000006714C900
  000000006714C926: B9 08 00 00 00     mov         ecx,8
  000000006714C92B: E8 56 C1 02 00     call        0000000067178A86
  000000006714C930: 48 8B D8           mov         rbx,rax
  000000006714C933: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C938: 48 85 C0           test        rax,rax
  000000006714C93B: 74 12              je          000000006714C94F
  000000006714C93D: 48 8B C8           mov         rcx,rax
  000000006714C940: E8 5B F8 FF FF     call        000000006714C1A0
  000000006714C945: 4C 8D 1D 5C 92 0F  lea         r11,[0000000067245BA8h]
                    00
  000000006714C94C: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714C94F: B9 10 00 00 00     mov         ecx,10h
  000000006714C954: E8 2D C1 02 00     call        0000000067178A86
  000000006714C959: 48 8B D8           mov         rbx,rax
  000000006714C95C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C961: 48 85 C0           test        rax,rax
  000000006714C964: 74 19              je          000000006714C97F
  000000006714C966: 48 8B C8           mov         rcx,rax
  000000006714C969: E8 32 F8 FF FF     call        000000006714C1A0
  000000006714C96E: 4C 8D 1D E3 90 0F  lea         r11,[0000000067245A58h]
                    00
  000000006714C975: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714C978: C7 43 08 00 00 00  mov         dword ptr [rbx+8],0
                    00
  000000006714C97F: B9 10 00 00 00     mov         ecx,10h
  000000006714C984: E8 FD C0 02 00     call        0000000067178A86
  000000006714C989: 48 8B D8           mov         rbx,rax
  000000006714C98C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C991: 48 85 C0           test        rax,rax
  000000006714C994: 74 19              je          000000006714C9AF
  000000006714C996: 48 8B C8           mov         rcx,rax
  000000006714C999: E8 02 F8 FF FF     call        000000006714C1A0
  000000006714C99E: 4C 8D 1D EB 90 0F  lea         r11,[0000000067245A90h]
                    00
  000000006714C9A5: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714C9A8: C7 43 08 01 00 00  mov         dword ptr [rbx+8],1
                    00
  000000006714C9AF: B9 10 00 00 00     mov         ecx,10h
  000000006714C9B4: E8 CD C0 02 00     call        0000000067178A86
  000000006714C9B9: 48 8B D8           mov         rbx,rax
  000000006714C9BC: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C9C1: 48 85 C0           test        rax,rax
  000000006714C9C4: 74 19              je          000000006714C9DF
  000000006714C9C6: 48 8B C8           mov         rcx,rax
  000000006714C9C9: E8 D2 F7 FF FF     call        000000006714C1A0
  000000006714C9CE: 4C 8D 1D F3 90 0F  lea         r11,[0000000067245AC8h]
                    00
  000000006714C9D5: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714C9D8: C7 43 08 02 00 00  mov         dword ptr [rbx+8],2
                    00
  000000006714C9DF: B9 10 00 00 00     mov         ecx,10h
  000000006714C9E4: E8 9D C0 02 00     call        0000000067178A86
  000000006714C9E9: 48 8B D8           mov         rbx,rax
  000000006714C9EC: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714C9F1: 48 85 C0           test        rax,rax
  000000006714C9F4: 74 19              je          000000006714CA0F
  000000006714C9F6: 48 8B C8           mov         rcx,rax
  000000006714C9F9: E8 A2 F7 FF FF     call        000000006714C1A0
  000000006714C9FE: 4C 8D 1D FB 90 0F  lea         r11,[0000000067245B00h]
                    00
  000000006714CA05: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714CA08: C7 43 08 00 00 00  mov         dword ptr [rbx+8],0
                    00
  000000006714CA0F: B9 10 00 00 00     mov         ecx,10h
  000000006714CA14: E8 6D C0 02 00     call        0000000067178A86
  000000006714CA19: 48 8B D8           mov         rbx,rax
  000000006714CA1C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714CA21: 48 85 C0           test        rax,rax
  000000006714CA24: 74 19              je          000000006714CA3F
  000000006714CA26: 48 8B C8           mov         rcx,rax
  000000006714CA29: E8 72 F7 FF FF     call        000000006714C1A0
  000000006714CA2E: 4C 8D 1D 03 91 0F  lea         r11,[0000000067245B38h]
                    00
  000000006714CA35: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714CA38: C7 43 08 01 00 00  mov         dword ptr [rbx+8],1
                    00
  000000006714CA3F: B9 10 00 00 00     mov         ecx,10h
  000000006714CA44: E8 3D C0 02 00     call        0000000067178A86
  000000006714CA49: 48 8B D8           mov         rbx,rax
  000000006714CA4C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714CA51: 48 85 C0           test        rax,rax
  000000006714CA54: 74 19              je          000000006714CA6F
  000000006714CA56: 48 8B C8           mov         rcx,rax
  000000006714CA59: E8 42 F7 FF FF     call        000000006714C1A0
  000000006714CA5E: 4C 8D 1D 0B 91 0F  lea         r11,[0000000067245B70h]
                    00
  000000006714CA65: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714CA68: C7 43 08 02 00 00  mov         dword ptr [rbx+8],2
                    00
  000000006714CA6F: B9 08 00 00 00     mov         ecx,8
  000000006714CA74: E8 0D C0 02 00     call        0000000067178A86
  000000006714CA79: 48 8B D8           mov         rbx,rax
  000000006714CA7C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714CA81: 48 85 C0           test        rax,rax
  000000006714CA84: 74 12              je          000000006714CA98
  000000006714CA86: 48 8B C8           mov         rcx,rax
  000000006714CA89: E8 12 F7 FF FF     call        000000006714C1A0
  000000006714CA8E: 4C 8D 1D 73 5E 0F  lea         r11,[0000000067242908h]
                    00
  000000006714CA95: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714CA98: B9 08 00 00 00     mov         ecx,8
  000000006714CA9D: E8 E4 BF 02 00     call        0000000067178A86
  000000006714CAA2: 48 8B D8           mov         rbx,rax
  000000006714CAA5: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714CAAA: 48 85 C0           test        rax,rax
  000000006714CAAD: 74 12              je          000000006714CAC1
  000000006714CAAF: 48 8B C8           mov         rcx,rax
  000000006714CAB2: E8 E9 F6 FF FF     call        000000006714C1A0
  000000006714CAB7: 4C 8D 1D 12 5E 0F  lea         r11,[00000000672428D0h]
                    00
  000000006714CABE: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714CAC1: B9 08 00 00 00     mov         ecx,8
  000000006714CAC6: E8 BB BF 02 00     call        0000000067178A86
  000000006714CACB: 48 8B D8           mov         rbx,rax
  000000006714CACE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714CAD3: 48 85 C0           test        rax,rax
  000000006714CAD6: 74 12              je          000000006714CAEA
  000000006714CAD8: 48 8B C8           mov         rcx,rax
  000000006714CADB: E8 C0 F6 FF FF     call        000000006714C1A0
  000000006714CAE0: 4C 8D 1D 31 91 0F  lea         r11,[0000000067245C18h]
                    00
  000000006714CAE7: 4C 89 1B           mov         qword ptr [rbx],r11
  000000006714CAEA: 48 83 3D 46 79 1A  cmp         qword ptr [00000000672F4438h],0
                    00 00
  000000006714CAF2: 75 13              jne         000000006714CB07
  000000006714CAF4: 48 8D 0D 3D CA 0D  lea         rcx,[0000000067229538h]
                    00
  000000006714CAFB: E8 B0 54 F7 FF     call        00000000670C1FB0
  000000006714CB00: 48 89 05 31 79 1A  mov         qword ptr [00000000672F4438h],rax
                    00
  000000006714CB07: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006714CB0C: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  000000006714CB11: 48 83 C4 30        add         rsp,30h
  000000006714CB15: 5F                 pop         rdi
  000000006714CB16: C3                 ret

  Summary

      1A7000 .text
