Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650A1D10: 40 53              push        rbx
  00000000650A1D12: 57                 push        rdi
  00000000650A1D13: 48 83 EC 38        sub         rsp,38h
  00000000650A1D17: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650A1D20: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  00000000650A1D28: 48 8B 05 29 99 92  mov         rax,qword ptr [00000000659CB658h]
                    00
  00000000650A1D2F: 48 85 C0           test        rax,rax
  00000000650A1D32: 0F 85 00 01 00 00  jne         00000000650A1E38
  00000000650A1D38: 38 05 22 99 92 00  cmp         byte ptr [00000000659CB660h],al
  00000000650A1D3E: 0F 85 EB 00 00 00  jne         00000000650A1E2F
  00000000650A1D44: 8D 48 10           lea         ecx,[rax+10h]
  00000000650A1D47: E8 26 F4 53 00     call        00000000655E1172
  00000000650A1D4C: 48 8B D8           mov         rbx,rax
  00000000650A1D4F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000650A1D54: 48 85 C0           test        rax,rax
  00000000650A1D57: 74 4E              je          00000000650A1DA7
  00000000650A1D59: 48 8D 15 70 F8 60  lea         rdx,[00000000656B15D0h]
                    00
  00000000650A1D60: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000650A1D65: FF 15 75 4C 5F 00  call        qword ptr [00000000656969E0h]
  00000000650A1D6B: 48 8B D0           mov         rdx,rax
  00000000650A1D6E: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000650A1D73: FF 15 8F 4E 5F 00  call        qword ptr [0000000065696C08h]
  00000000650A1D79: 90                 nop
  00000000650A1D7A: BF 01 00 00 00     mov         edi,1
  00000000650A1D7F: 89 7C 24 50        mov         dword ptr [rsp+50h],edi
  00000000650A1D83: 45 33 C9           xor         r9d,r9d
  00000000650A1D86: 4C 8D 44 24 58     lea         r8,[rsp+58h]
  00000000650A1D8B: 48 8D 15 7E F4 60  lea         rdx,[00000000656B1210h]
                    00
  00000000650A1D92: 48 8B CB           mov         rcx,rbx
  00000000650A1D95: FF 15 E5 57 5F 00  call        qword ptr [0000000065697580h]
  00000000650A1D9B: 4C 8D 1D D6 F7 60  lea         r11,[00000000656B1578h]
                    00
  00000000650A1DA2: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000650A1DA5: EB 06              jmp         00000000650A1DAD
  00000000650A1DA7: 33 DB              xor         ebx,ebx
  00000000650A1DA9: 8B 7C 24 50        mov         edi,dword ptr [rsp+50h]
  00000000650A1DAD: 40 F6 C7 01        test        dil,1
  00000000650A1DB1: 74 0B              je          00000000650A1DBE
  00000000650A1DB3: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000650A1DB8: FF 15 A2 49 5F 00  call        qword ptr [0000000065696760h]
  00000000650A1DBE: 33 C0              xor         eax,eax
  00000000650A1DC0: F0 48 0F B1 1D 8F  lock cmpxchg qword ptr [00000000659CB658h],rbx
                    98 92 00
  00000000650A1DC9: 74 25              je          00000000650A1DF0
  00000000650A1DCB: 48 85 DB           test        rbx,rbx
  00000000650A1DCE: 74 17              je          00000000650A1DE7
  00000000650A1DD0: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650A1DD3: BA 01 00 00 00     mov         edx,1
  00000000650A1DD8: 48 8B CB           mov         rcx,rbx
  00000000650A1DDB: FF 50 18           call        qword ptr [rax+18h]
  00000000650A1DDE: 48 8B 05 73 98 92  mov         rax,qword ptr [00000000659CB658h]
                    00
  00000000650A1DE5: EB 58              jmp         00000000650A1E3F
  00000000650A1DE7: 48 8B 05 6A 98 92  mov         rax,qword ptr [00000000659CB658h]
                    00
  00000000650A1DEE: EB 4F              jmp         00000000650A1E3F
  00000000650A1DF0: 8B 05 5A 98 92 00  mov         eax,dword ptr [00000000659CB650h]
  00000000650A1DF6: A8 01              test        al,1
  00000000650A1DF8: 75 2C              jne         00000000650A1E26
  00000000650A1DFA: 83 C8 01           or          eax,1
  00000000650A1DFD: 89 05 4D 98 92 00  mov         dword ptr [00000000659CB650h],eax
  00000000650A1E03: 48 8D 05 4E 98 92  lea         rax,[00000000659CB658h]
                    00
  00000000650A1E0A: 48 89 05 37 98 92  mov         qword ptr [00000000659CB648h],rax
                    00
  00000000650A1E11: 48 8D 0D 78 2D 5F  lea         rcx,[0000000065694B90h]
                    00
  00000000650A1E18: E8 0F F2 53 00     call        00000000655E102C
  00000000650A1E1D: 48 8B 05 34 98 92  mov         rax,qword ptr [00000000659CB658h]
                    00
  00000000650A1E24: EB 19              jmp         00000000650A1E3F
  00000000650A1E26: 48 8B 05 2B 98 92  mov         rax,qword ptr [00000000659CB658h]
                    00
  00000000650A1E2D: EB 10              jmp         00000000650A1E3F
  00000000650A1E2F: 48 8B 05 22 98 92  mov         rax,qword ptr [00000000659CB658h]
                    00
  00000000650A1E36: EB 07              jmp         00000000650A1E3F
  00000000650A1E38: 48 8B 05 19 98 92  mov         rax,qword ptr [00000000659CB658h]
                    00
  00000000650A1E3F: 48 83 C4 38        add         rsp,38h
  00000000650A1E43: 5F                 pop         rdi
  00000000650A1E44: 5B                 pop         rbx
  00000000650A1E45: C3                 ret

  Summary

      695000 .text
