Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F9090: 40 57              push        rdi
  00000000670F9092: 48 83 EC 40        sub         rsp,40h
  00000000670F9096: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F909F: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000670F90A4: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  00000000670F90A9: 48 8B F1           mov         rsi,rcx
  00000000670F90AC: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670F90B0: 33 C0              xor         eax,eax
  00000000670F90B2: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000000670F90B6: 89 44 24 54        mov         dword ptr [rsp+54h],eax
  00000000670F90BA: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670F90BF: E8 4C 5C FF FF     call        00000000670EED10
  00000000670F90C4: 90                 nop
  00000000670F90C5: 48 8D 4B 20        lea         rcx,[rbx+20h]
  00000000670F90C9: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  00000000670F90CE: 48 8B D0           mov         rdx,rax
  00000000670F90D1: E8 CA 65 00 00     call        00000000670FF6A0
  00000000670F90D6: 0F B6 D8           movzx       ebx,al
  00000000670F90D9: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000000670F90DE: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670F90E2: 75 0B              jne         00000000670F90EF
  00000000670F90E4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670F90E9: E8 E2 6F F7 FF     call        00000000670700D0
  00000000670F90EE: 90                 nop
  00000000670F90EF: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670F90F4: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F90F8: 75 0A              jne         00000000670F9104
  00000000670F90FA: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670F90FF: E8 CC 6F F7 FF     call        00000000670700D0
  00000000670F9104: 84 DB              test        bl,bl
  00000000670F9106: 75 3E              jne         00000000670F9146
  00000000670F9108: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670F910D: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000670F9112: E8 09 82 04 00     call        0000000067141320
  00000000670F9117: 90                 nop
  00000000670F9118: 4C 8B C0           mov         r8,rax
  00000000670F911B: BA 0E 00 00 00     mov         edx,0Eh
  00000000670F9120: 48 8B CE           mov         rcx,rsi
  00000000670F9123: E8 18 E9 FA FF     call        00000000670A7A40
  00000000670F9128: 90                 nop
  00000000670F9129: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000000670F912E: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F9132: 75 0E              jne         00000000670F9142
  00000000670F9134: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000000670F9139: E8 92 6F F7 FF     call        00000000670700D0
  00000000670F913E: 8A C3              mov         al,bl
  00000000670F9140: EB 06              jmp         00000000670F9148
  00000000670F9142: 8A C3              mov         al,bl
  00000000670F9144: EB 02              jmp         00000000670F9148
  00000000670F9146: 8A C3              mov         al,bl
  00000000670F9148: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670F914D: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  00000000670F9152: 48 83 C4 40        add         rsp,40h
  00000000670F9156: 5F                 pop         rdi
  00000000670F9157: C3                 ret

  Summary

      1A7000 .text
