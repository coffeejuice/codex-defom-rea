Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B4AE0: 40 53              push        rbx
  00000000670B4AE2: 56                 push        rsi
  00000000670B4AE3: 57                 push        rdi
  00000000670B4AE4: 48 83 EC 40        sub         rsp,40h
  00000000670B4AE8: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B4AF1: 48 8B DA           mov         rbx,rdx
  00000000670B4AF4: 48 8B F1           mov         rsi,rcx
  00000000670B4AF7: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000670B4AFB: E8 E0 58 00 00     call        00000000670BA3E0
  00000000670B4B00: 84 C0              test        al,al
  00000000670B4B02: 74 62              je          00000000670B4B66
  00000000670B4B04: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670B4B09: 48 8B CE           mov         rcx,rsi
  00000000670B4B0C: E8 CF F9 FF FF     call        00000000670B44E0
  00000000670B4B11: 90                 nop
  00000000670B4B12: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  00000000670B4B17: 48 8B C8           mov         rcx,rax
  00000000670B4B1A: E8 91 99 FB FF     call        000000006706E4B0
  00000000670B4B1F: 90                 nop
  00000000670B4B20: 48 8B 00           mov         rax,qword ptr [rax]
  00000000670B4B23: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  00000000670B4B27: 48 8D 0D 32 55 17  lea         rcx,[000000006722A060h]
                    00
  00000000670B4B2E: E8 AD 83 F6 FF     call        000000006701CEE0
  00000000670B4B33: 90                 nop
  00000000670B4B34: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670B4B39: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670B4B3D: 75 0B              jne         00000000670B4B4A
  00000000670B4B3F: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670B4B44: E8 67 B8 F6 FF     call        00000000670203B0
  00000000670B4B49: 90                 nop
  00000000670B4B4A: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670B4B4F: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B4B53: 75 0A              jne         00000000670B4B5F
  00000000670B4B55: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670B4B5A: E8 71 B5 FB FF     call        00000000670700D0
  00000000670B4B5F: 32 C0              xor         al,al
  00000000670B4B61: E9 F2 00 00 00     jmp         00000000670B4C58
  00000000670B4B66: 8B 0B              mov         ecx,dword ptr [rbx]
  00000000670B4B68: 8B C1              mov         eax,ecx
  00000000670B4B6A: 83 E0 04           and         eax,4
  00000000670B4B6D: 74 05              je          00000000670B4B74
  00000000670B4B6F: 83 C9 02           or          ecx,2
  00000000670B4B72: 89 0B              mov         dword ptr [rbx],ecx
  00000000670B4B74: 48 8B CE           mov         rcx,rsi
  00000000670B4B77: E8 D4 FC FF FF     call        00000000670B4850
  00000000670B4B7C: 44 8B 1B           mov         r11d,dword ptr [rbx]
  00000000670B4B7F: 41 83 E3 03        and         r11d,3
  00000000670B4B83: 75 13              jne         00000000670B4B98
  00000000670B4B85: 48 8D 0D A4 54 17  lea         rcx,[000000006722A030h]
                    00
  00000000670B4B8C: E8 4F 83 F6 FF     call        000000006701CEE0
  00000000670B4B91: 32 C0              xor         al,al
  00000000670B4B93: E9 C0 00 00 00     jmp         00000000670B4C58
  00000000670B4B98: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000670B4B9B: 48 8B CE           mov         rcx,rsi
  00000000670B4B9E: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  00000000670B4BA4: 8B 0B              mov         ecx,dword ptr [rbx]
  00000000670B4BA6: 83 C9 20           or          ecx,20h
  00000000670B4BA9: 89 4C 24 60        mov         dword ptr [rsp+60h],ecx
  00000000670B4BAD: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000670B4BB0: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670B4BB5: 48 8B C8           mov         rcx,rax
  00000000670B4BB8: 41 FF 50 08        call        qword ptr [r8+8]
  00000000670B4BBC: 84 C0              test        al,al
  00000000670B4BBE: 74 40              je          00000000670B4C00
  00000000670B4BC0: 8B 03              mov         eax,dword ptr [rbx]
  00000000670B4BC2: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  00000000670B4BC6: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000000670B4BCB: 48 8B CE           mov         rcx,rsi
  00000000670B4BCE: E8 3D 58 00 00     call        00000000670BA410
  00000000670B4BD3: F6 03 04           test        byte ptr [rbx],4
  00000000670B4BD6: 74 24              je          00000000670B4BFC
  00000000670B4BD8: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000670B4BDB: 48 8B CE           mov         rcx,rsi
  00000000670B4BDE: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  00000000670B4BE4: 48 8B D8           mov         rbx,rax
  00000000670B4BE7: 48 8B 38           mov         rdi,qword ptr [rax]
  00000000670B4BEA: 48 8B 16           mov         rdx,qword ptr [rsi]
  00000000670B4BED: 48 8B CE           mov         rcx,rsi
  00000000670B4BF0: FF 52 78           call        qword ptr [rdx+78h]
  00000000670B4BF3: 48 8B D0           mov         rdx,rax
  00000000670B4BF6: 48 8B CB           mov         rcx,rbx
  00000000670B4BF9: FF 57 30           call        qword ptr [rdi+30h]
  00000000670B4BFC: B0 01              mov         al,1
  00000000670B4BFE: EB 58              jmp         00000000670B4C58
  00000000670B4C00: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B4C07: E8 04 2E FF FF     call        00000000670A7A10
  00000000670B4C0C: 8B D8              mov         ebx,eax
  00000000670B4C0E: B8 05 00 00 00     mov         eax,5
  00000000670B4C13: 83 FB 08           cmp         ebx,8
  00000000670B4C16: 0F 44 D8           cmove       ebx,eax
  00000000670B4C19: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000670B4C1E: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B4C25: E8 F6 2D FF FF     call        00000000670A7A20
  00000000670B4C2A: 90                 nop
  00000000670B4C2B: 89 9F 20 01 00 00  mov         dword ptr [rdi+0000000000000120h],ebx
  00000000670B4C31: 48 8D 8F 90 00 00  lea         rcx,[rdi+0000000000000090h]
                    00
  00000000670B4C38: 48 8B D0           mov         rdx,rax
  00000000670B4C3B: E8 90 B7 FB FF     call        00000000670703D0
  00000000670B4C40: 90                 nop
  00000000670B4C41: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670B4C46: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B4C4A: 75 0A              jne         00000000670B4C56
  00000000670B4C4C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670B4C51: E8 7A B4 FB FF     call        00000000670700D0
  00000000670B4C56: 32 C0              xor         al,al
  00000000670B4C58: 48 83 C4 40        add         rsp,40h
  00000000670B4C5C: 5F                 pop         rdi
  00000000670B4C5D: 5E                 pop         rsi
  00000000670B4C5E: 5B                 pop         rbx
  00000000670B4C5F: C3                 ret

  Summary

      1A7000 .text
