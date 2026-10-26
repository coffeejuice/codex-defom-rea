Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F0080: 4C 8B DC           mov         r11,rsp
  00000000670F0083: 57                 push        rdi
  00000000670F0084: 48 83 EC 40        sub         rsp,40h
  00000000670F0088: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F0091: 49 89 5B 10        mov         qword ptr [r11+10h],rbx
  00000000670F0095: 49 89 73 18        mov         qword ptr [r11+18h],rsi
  00000000670F0099: 48 8B FA           mov         rdi,rdx
  00000000670F009C: 48 8B D9           mov         rbx,rcx
  00000000670F009F: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670F00A2: 49 89 43 E0        mov         qword ptr [r11-20h],rax
  00000000670F00A6: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670F00AA: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000670F00AE: 49 89 43 E8        mov         qword ptr [r11-18h],rax
  00000000670F00B2: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670F00B6: 0F B7 41 10        movzx       eax,word ptr [rcx+10h]
  00000000670F00BA: 66 89 44 24 38     mov         word ptr [rsp+38h],ax
  00000000670F00BF: 0F B7 41 12        movzx       eax,word ptr [rcx+12h]
  00000000670F00C3: 66 89 44 24 3A     mov         word ptr [rsp+3Ah],ax
  00000000670F00C8: 0F B7 41 14        movzx       eax,word ptr [rcx+14h]
  00000000670F00CC: 66 89 44 24 3C     mov         word ptr [rsp+3Ch],ax
  00000000670F00D1: 49 C7 43 08 00 00  mov         qword ptr [r11+8],0
                    00 00
  00000000670F00D9: 45 33 C9           xor         r9d,r9d
  00000000670F00DC: 4D 8D 43 08        lea         r8,[r11+8]
  00000000670F00E0: 49 8D 4B E0        lea         rcx,[r11-20h]
  00000000670F00E4: E8 D7 FA FF FF     call        00000000670EFBC0
  00000000670F00E9: 84 C0              test        al,al
  00000000670F00EB: 74 3A              je          00000000670F0127
  00000000670F00ED: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000670F00F2: 48 8B CB           mov         rcx,rbx
  00000000670F00F5: E8 D6 02 F8 FF     call        00000000670703D0
  00000000670F00FA: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000670F00FF: 48 8D 4B 08        lea         rcx,[rbx+8]
  00000000670F0103: E8 C8 02 F8 FF     call        00000000670703D0
  00000000670F0108: 44 0F B7 5C 24 38  movzx       r11d,word ptr [rsp+38h]
  00000000670F010E: 66 44 89 5B 10     mov         word ptr [rbx+10h],r11w
  00000000670F0113: 0F B7 44 24 3A     movzx       eax,word ptr [rsp+3Ah]
  00000000670F0118: 66 89 43 12        mov         word ptr [rbx+12h],ax
  00000000670F011C: 0F B7 44 24 3C     movzx       eax,word ptr [rsp+3Ch]
  00000000670F0121: 66 89 43 14        mov         word ptr [rbx+14h],ax
  00000000670F0125: EB 06              jmp         00000000670F012D
  00000000670F0127: C7 07 00 00 00 00  mov         dword ptr [rdi],0
  00000000670F012D: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  00000000670F0132: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670F0137: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670F013B: 75 0B              jne         00000000670F0148
  00000000670F013D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670F0142: E8 89 FF F7 FF     call        00000000670700D0
  00000000670F0147: 90                 nop
  00000000670F0148: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670F014D: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670F0151: 75 0F              jne         00000000670F0162
  00000000670F0153: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670F0158: E8 73 FF F7 FF     call        00000000670700D0
  00000000670F015D: 48 8B C7           mov         rax,rdi
  00000000670F0160: EB 03              jmp         00000000670F0165
  00000000670F0162: 48 8B C7           mov         rax,rdi
  00000000670F0165: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670F016A: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  00000000670F016F: 48 83 C4 40        add         rsp,40h
  00000000670F0173: 5F                 pop         rdi
  00000000670F0174: C3                 ret

  Summary

      1A7000 .text
