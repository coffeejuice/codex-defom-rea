Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006701CD90: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000006701CD95: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000006701CD9A: 89 4C 24 08        mov         dword ptr [rsp+8],ecx
  000000006701CD9E: 53                 push        rbx
  000000006701CD9F: 56                 push        rsi
  000000006701CDA0: 57                 push        rdi
  000000006701CDA1: 48 83 EC 40        sub         rsp,40h
  000000006701CDA5: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006701CDAE: 49 8B F8           mov         rdi,r8
  000000006701CDB1: 48 8B DA           mov         rbx,rdx
  000000006701CDB4: 8B F1              mov         esi,ecx
  000000006701CDB6: FF 15 9C B6 18 00  call        qword ptr [00000000671A8458h]
  000000006701CDBC: 84 C0              test        al,al
  000000006701CDBE: 74 12              je          000000006701CDD2
  000000006701CDC0: 4C 8B C7           mov         r8,rdi
  000000006701CDC3: 48 8B D3           mov         rdx,rbx
  000000006701CDC6: 8B CE              mov         ecx,esi
  000000006701CDC8: E8 33 FF FF FF     call        000000006701CD00
  000000006701CDCD: E9 CF 00 00 00     jmp         000000006701CEA1
  000000006701CDD2: 48 8D 05 27 3B 2D  lea         rax,[00000000672F0900h]
                    00
  000000006701CDD9: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000006701CDDE: F0 83 05 1A 3B 2D  lock add    dword ptr [00000000672F0900h],1
                    00 01
  000000006701CDE6: 48 85 DB           test        rbx,rbx
  000000006701CDE9: 0F 84 86 00 00 00  je          000000006701CE75
  000000006701CDEF: 48 8D 05 3A 3D 2D  lea         rax,[00000000672F0B30h]
                    00
  000000006701CDF6: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006701CDFB: F0 83 05 2D 3D 2D  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  000000006701CE03: 4C 8B C7           mov         r8,rdi
  000000006701CE06: 48 8B D3           mov         rdx,rbx
  000000006701CE09: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006701CE0E: E8 7D 81 05 00     call        0000000067074F90
  000000006701CE13: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006701CE18: 48 8B C8           mov         rcx,rax
  000000006701CE1B: E8 90 16 05 00     call        000000006706E4B0
  000000006701CE20: 90                 nop
  000000006701CE21: 48 8B D0           mov         rdx,rax
  000000006701CE24: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006701CE29: E8 12 06 01 00     call        000000006702D440
  000000006701CE2E: 90                 nop
  000000006701CE2F: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006701CE34: 83 CB FF           or          ebx,0FFFFFFFFh
  000000006701CE37: F0 01 19           lock add    dword ptr [rcx],ebx
  000000006701CE3A: 75 0B              jne         000000006701CE47
  000000006701CE3C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006701CE41: E8 6A 35 00 00     call        00000000670203B0
  000000006701CE46: 90                 nop
  000000006701CE47: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000006701CE4C: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006701CE50: 75 0A              jne         000000006701CE5C
  000000006701CE52: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006701CE57: E8 74 32 05 00     call        00000000670700D0
  000000006701CE5C: EB 1A              jmp         000000006701CE78
  000000006701CE5E: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  000000006701CE63: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006701CE67: 75 38              jne         000000006701CEA1
  000000006701CE69: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000006701CE6E: E8 3D 35 00 00     call        00000000670203B0
  000000006701CE73: EB 2C              jmp         000000006701CEA1
  000000006701CE75: 83 CB FF           or          ebx,0FFFFFFFFh
  000000006701CE78: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  000000006701CE7D: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  000000006701CE81: 8B CE              mov         ecx,esi
  000000006701CE83: E8 A8 FD FF FF     call        000000006701CC30
  000000006701CE88: 90                 nop
  000000006701CE89: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  000000006701CE8E: F0 0F C1 18        lock xadd   dword ptr [rax],ebx
  000000006701CE92: 83 C3 FF           add         ebx,0FFFFFFFFh
  000000006701CE95: 75 0A              jne         000000006701CEA1
  000000006701CE97: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000006701CE9C: E8 0F 35 00 00     call        00000000670203B0
  000000006701CEA1: 48 83 C4 40        add         rsp,40h
  000000006701CEA5: 5F                 pop         rdi
  000000006701CEA6: 5E                 pop         rsi
  000000006701CEA7: 5B                 pop         rbx
  000000006701CEA8: C3                 ret

  Summary

      1A7000 .text
