Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F08D0: 40 55              push        rbp
  00000001412F08D2: 56                 push        rsi
  00000001412F08D3: 57                 push        rdi
  00000001412F08D4: 41 54              push        r12
  00000001412F08D6: 41 55              push        r13
  00000001412F08D8: 41 56              push        r14
  00000001412F08DA: 41 57              push        r15
  00000001412F08DC: 48 83 EC 60        sub         rsp,60h
  00000001412F08E0: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412F08E9: 48 89 9C 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rbx
                    00 00
  00000001412F08F1: 45 0F B6 E8        movzx       r13d,r8b
  00000001412F08F5: 48 8B F2           mov         rsi,rdx
  00000001412F08F8: 48 8B D9           mov         rbx,rcx
  00000001412F08FB: 83 B9 98 14 00 00  cmp         dword ptr [rcx+0000000000001498h],0
                    00
  00000001412F0902: 7D 0B              jge         00000001412F090F
  00000001412F0904: E8 07 E5 FF FF     call        00000001412EEE10
  00000001412F0909: 89 83 98 14 00 00  mov         dword ptr [rbx+0000000000001498h],eax
  00000001412F090F: 48 85 F6           test        rsi,rsi
  00000001412F0912: 0F 84 76 01 00 00  je          00000001412F0A8E
  00000001412F0918: 33 C0              xor         eax,eax
  00000001412F091A: 48 83 C9 FF        or          rcx,0FFFFFFFFFFFFFFFFh
  00000001412F091E: 48 8B FE           mov         rdi,rsi
  00000001412F0921: F2 AE              repne scas  byte ptr [rdi]
  00000001412F0923: 48 F7 D1           not         rcx
  00000001412F0926: 48 83 F9 01        cmp         rcx,1
  00000001412F092A: 0F 84 5E 01 00 00  je          00000001412F0A8E
  00000001412F0930: 48 8B D6           mov         rdx,rsi
  00000001412F0933: 48 8B CB           mov         rcx,rbx
  00000001412F0936: E8 A5 72 FF FF     call        00000001412E7BE0
  00000001412F093B: 48 8B E8           mov         rbp,rax
  00000001412F093E: 48 85 C0           test        rax,rax
  00000001412F0941: 0F 84 43 01 00 00  je          00000001412F0A8A
  00000001412F0947: 33 C0              xor         eax,eax
  00000001412F0949: 48 83 C9 FF        or          rcx,0FFFFFFFFFFFFFFFFh
  00000001412F094D: 48 8B FD           mov         rdi,rbp
  00000001412F0950: F2 AE              repne scas  byte ptr [rdi]
  00000001412F0952: 48 F7 D1           not         rcx
  00000001412F0955: 48 83 F9 01        cmp         rcx,1
  00000001412F0959: 0F 84 2B 01 00 00  je          00000001412F0A8A
  00000001412F095F: 88 84 24 A0 00 00  mov         byte ptr [rsp+00000000000000A0h],al
                    00
  00000001412F0966: C7 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],2
                    00 02 00 00 00
  00000001412F0971: 4C 8D 8C 24 A8 00  lea         r9,[rsp+00000000000000A8h]
                    00 00
  00000001412F0979: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  00000001412F0981: 48 8B D5           mov         rdx,rbp
  00000001412F0984: 48 8B CB           mov         rcx,rbx
  00000001412F0987: E8 34 6D FF FF     call        00000001412E76C0
  00000001412F098C: 4C 8B E0           mov         r12,rax
  00000001412F098F: 80 BC 24 A0 00 00  cmp         byte ptr [rsp+00000000000000A0h],0
                    00 00
  00000001412F0997: 75 07              jne         00000001412F09A0
  00000001412F0999: 32 C0              xor         al,al
  00000001412F099B: E9 F0 00 00 00     jmp         00000001412F0A90
  00000001412F09A0: 48 8D 3D 49 4D AC  lea         rdi,[0000000142DB56F0h]
                    01
  00000001412F09A7: 48 8D 05 32 4D AC  lea         rax,[0000000142DB56E0h]
                    01
  00000001412F09AE: 83 BC 24 A8 00 00  cmp         dword ptr [rsp+00000000000000A8h],2
                    00 02
  00000001412F09B6: 48 0F 45 F8        cmovne      rdi,rax
  00000001412F09BA: E8 E1 4E 01 00     call        00000001413058A0
  00000001412F09BF: 44 8B F0           mov         r14d,eax
  00000001412F09C2: E8 29 CE 00 00     call        00000001412FD7F0
  00000001412F09C7: 44 8B F8           mov         r15d,eax
  00000001412F09CA: B9 A0 3E 00 00     mov         ecx,3EA0h
  00000001412F09CF: E8 E8 E0 17 00     call        000000014146EABC
  00000001412F09D4: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001412F09DC: 48 85 C0           test        rax,rax
  00000001412F09DF: 74 23              je          00000001412F0A04
  00000001412F09E1: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  00000001412F09E9: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  00000001412F09EE: 45 8B CF           mov         r9d,r15d
  00000001412F09F1: 45 8B C6           mov         r8d,r14d
  00000001412F09F4: 48 8B D7           mov         rdx,rdi
  00000001412F09F7: 48 8B C8           mov         rcx,rax
  00000001412F09FA: E8 A1 DC FF FF     call        00000001412EE6A0
  00000001412F09FF: 48 8B F8           mov         rdi,rax
  00000001412F0A02: EB 02              jmp         00000001412F0A06
  00000001412F0A04: 33 FF              xor         edi,edi
  00000001412F0A06: C7 87 9C 3E 00 00  mov         dword ptr [rdi+0000000000003E9Ch],1
                    01 00 00 00
  00000001412F0A10: 8B 83 98 14 00 00  mov         eax,dword ptr [rbx+0000000000001498h]
  00000001412F0A16: 89 87 B8 35 00 00  mov         dword ptr [rdi+00000000000035B8h],eax
  00000001412F0A1C: 48 C7 05 C9 D2 3F  mov         qword ptr [00000001446EDCF0h],0
                    03 00 00 00 00
  00000001412F0A27: 48 8B CF           mov         rcx,rdi
  00000001412F0A2A: E8 81 C1 FF FF     call        00000001412ECBB0
  00000001412F0A2F: 8B F0              mov         esi,eax
  00000001412F0A31: 85 C0              test        eax,eax
  00000001412F0A33: 78 08              js          00000001412F0A3D
  00000001412F0A35: 48 8B CF           mov         rcx,rdi
  00000001412F0A38: E8 33 DE FF FF     call        00000001412EE870
  00000001412F0A3D: 85 F6              test        esi,esi
  00000001412F0A3F: 40 0F 99 C6        setns       sil
  00000001412F0A43: 45 84 ED           test        r13b,r13b
  00000001412F0A46: 74 05              je          00000001412F0A4D
  00000001412F0A48: E8 63 14 00 00     call        00000001412F1EB0
  00000001412F0A4D: 41 88 34 24        mov         byte ptr [r12],sil
  00000001412F0A51: 40 84 F6           test        sil,sil
  00000001412F0A54: 74 22              je          00000001412F0A78
  00000001412F0A56: 48 89 6C 24 38     mov         qword ptr [rsp+38h],rbp
  00000001412F0A5B: 48 89 7C 24 40     mov         qword ptr [rsp+40h],rdi
  00000001412F0A60: 48 8D 8B 58 14 00  lea         rcx,[rbx+0000000000001458h]
                    00
  00000001412F0A67: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000001412F0A6C: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001412F0A71: E8 5A F7 FF FF     call        00000001412F01D0
  00000001412F0A76: EB 0D              jmp         00000001412F0A85
  00000001412F0A78: 48 8B 07           mov         rax,qword ptr [rdi]
  00000001412F0A7B: BA 01 00 00 00     mov         edx,1
  00000001412F0A80: 48 8B CF           mov         rcx,rdi
  00000001412F0A83: FF 10              call        qword ptr [rax]
  00000001412F0A85: 40 8A C6           mov         al,sil
  00000001412F0A88: EB 06              jmp         00000001412F0A90
  00000001412F0A8A: 32 C0              xor         al,al
  00000001412F0A8C: EB 02              jmp         00000001412F0A90
  00000001412F0A8E: 32 C0              xor         al,al
  00000001412F0A90: 48 8B 9C 24 B0 00  mov         rbx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001412F0A98: 48 83 C4 60        add         rsp,60h
  00000001412F0A9C: 41 5F              pop         r15
  00000001412F0A9E: 41 5E              pop         r14
  00000001412F0AA0: 41 5D              pop         r13
  00000001412F0AA2: 41 5C              pop         r12
  00000001412F0AA4: 5F                 pop         rdi
  00000001412F0AA5: 5E                 pop         rsi
  00000001412F0AA6: 5D                 pop         rbp
  00000001412F0AA7: C3                 ret

  Summary

     1E02000 .text
