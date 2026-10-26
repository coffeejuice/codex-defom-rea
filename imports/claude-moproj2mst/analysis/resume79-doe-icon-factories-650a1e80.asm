Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650A1E80: 40 53              push        rbx
  00000000650A1E82: 57                 push        rdi
  00000000650A1E83: 48 83 EC 38        sub         rsp,38h
  00000000650A1E87: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650A1E90: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  00000000650A1E98: 48 8B 05 D9 97 92  mov         rax,qword ptr [00000000659CB678h]
                    00
  00000000650A1E9F: 48 85 C0           test        rax,rax
  00000000650A1EA2: 0F 85 00 01 00 00  jne         00000000650A1FA8
  00000000650A1EA8: 38 05 D2 97 92 00  cmp         byte ptr [00000000659CB680h],al
  00000000650A1EAE: 0F 85 EB 00 00 00  jne         00000000650A1F9F
  00000000650A1EB4: 8D 48 10           lea         ecx,[rax+10h]
  00000000650A1EB7: E8 B6 F2 53 00     call        00000000655E1172
  00000000650A1EBC: 48 8B D8           mov         rbx,rax
  00000000650A1EBF: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000650A1EC4: 48 85 C0           test        rax,rax
  00000000650A1EC7: 74 4E              je          00000000650A1F17
  00000000650A1EC9: 48 8D 15 00 F7 60  lea         rdx,[00000000656B15D0h]
                    00
  00000000650A1ED0: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000650A1ED5: FF 15 05 4B 5F 00  call        qword ptr [00000000656969E0h]
  00000000650A1EDB: 48 8B D0           mov         rdx,rax
  00000000650A1EDE: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000650A1EE3: FF 15 1F 4D 5F 00  call        qword ptr [0000000065696C08h]
  00000000650A1EE9: 90                 nop
  00000000650A1EEA: BF 01 00 00 00     mov         edi,1
  00000000650A1EEF: 89 7C 24 50        mov         dword ptr [rsp+50h],edi
  00000000650A1EF3: 45 33 C9           xor         r9d,r9d
  00000000650A1EF6: 4C 8D 44 24 58     lea         r8,[rsp+58h]
  00000000650A1EFB: 48 8D 15 5E F3 60  lea         rdx,[00000000656B1260h]
                    00
  00000000650A1F02: 48 8B CB           mov         rcx,rbx
  00000000650A1F05: FF 15 75 56 5F 00  call        qword ptr [0000000065697580h]
  00000000650A1F0B: 4C 8D 1D 66 F6 60  lea         r11,[00000000656B1578h]
                    00
  00000000650A1F12: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000650A1F15: EB 06              jmp         00000000650A1F1D
  00000000650A1F17: 33 DB              xor         ebx,ebx
  00000000650A1F19: 8B 7C 24 50        mov         edi,dword ptr [rsp+50h]
  00000000650A1F1D: 40 F6 C7 01        test        dil,1
  00000000650A1F21: 74 0B              je          00000000650A1F2E
  00000000650A1F23: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000650A1F28: FF 15 32 48 5F 00  call        qword ptr [0000000065696760h]
  00000000650A1F2E: 33 C0              xor         eax,eax
  00000000650A1F30: F0 48 0F B1 1D 3F  lock cmpxchg qword ptr [00000000659CB678h],rbx
                    97 92 00
  00000000650A1F39: 74 25              je          00000000650A1F60
  00000000650A1F3B: 48 85 DB           test        rbx,rbx
  00000000650A1F3E: 74 17              je          00000000650A1F57
  00000000650A1F40: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650A1F43: BA 01 00 00 00     mov         edx,1
  00000000650A1F48: 48 8B CB           mov         rcx,rbx
  00000000650A1F4B: FF 50 18           call        qword ptr [rax+18h]
  00000000650A1F4E: 48 8B 05 23 97 92  mov         rax,qword ptr [00000000659CB678h]
                    00
  00000000650A1F55: EB 58              jmp         00000000650A1FAF
  00000000650A1F57: 48 8B 05 1A 97 92  mov         rax,qword ptr [00000000659CB678h]
                    00
  00000000650A1F5E: EB 4F              jmp         00000000650A1FAF
  00000000650A1F60: 8B 05 0A 97 92 00  mov         eax,dword ptr [00000000659CB670h]
  00000000650A1F66: A8 01              test        al,1
  00000000650A1F68: 75 2C              jne         00000000650A1F96
  00000000650A1F6A: 83 C8 01           or          eax,1
  00000000650A1F6D: 89 05 FD 96 92 00  mov         dword ptr [00000000659CB670h],eax
  00000000650A1F73: 48 8D 05 FE 96 92  lea         rax,[00000000659CB678h]
                    00
  00000000650A1F7A: 48 89 05 E7 96 92  mov         qword ptr [00000000659CB668h],rax
                    00
  00000000650A1F81: 48 8D 0D 48 2C 5F  lea         rcx,[0000000065694BD0h]
                    00
  00000000650A1F88: E8 9F F0 53 00     call        00000000655E102C
  00000000650A1F8D: 48 8B 05 E4 96 92  mov         rax,qword ptr [00000000659CB678h]
                    00
  00000000650A1F94: EB 19              jmp         00000000650A1FAF
  00000000650A1F96: 48 8B 05 DB 96 92  mov         rax,qword ptr [00000000659CB678h]
                    00
  00000000650A1F9D: EB 10              jmp         00000000650A1FAF
  00000000650A1F9F: 48 8B 05 D2 96 92  mov         rax,qword ptr [00000000659CB678h]
                    00
  00000000650A1FA6: EB 07              jmp         00000000650A1FAF
  00000000650A1FA8: 48 8B 05 C9 96 92  mov         rax,qword ptr [00000000659CB678h]
                    00
  00000000650A1FAF: 48 83 C4 38        add         rsp,38h
  00000000650A1FB3: 5F                 pop         rdi
  00000000650A1FB4: 5B                 pop         rbx
  00000000650A1FB5: C3                 ret

  Summary

      695000 .text
