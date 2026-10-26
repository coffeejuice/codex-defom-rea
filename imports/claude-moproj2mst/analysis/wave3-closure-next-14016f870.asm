Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014016F870: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014016F875: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014016F87A: 53                 push        rbx
  000000014016F87B: 57                 push        rdi
  000000014016F87C: 48 81 EC 18 01 00  sub         rsp,118h
                    00
  000000014016F883: 48 C7 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014016F88F: 48 8B 94 24 38 01  mov         rdx,qword ptr [rsp+0000000000000138h]
                    00 00
  000000014016F897: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F89F: FF 15 5B 82 C9 01  call        qword ptr [0000000141E07B00h]
  000000014016F8A5: 90                 nop
  000000014016F8A6: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F8AE: 48 8D 05 E3 95 06  lea         rax,[00000001421D8E98h]
                    02
  000000014016F8B5: 48 89 01           mov         qword ptr [rcx],rax
  000000014016F8B8: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F8C0: 48 8D 05 91 95 06  lea         rax,[00000001421D8E58h]
                    02
  000000014016F8C7: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000014016F8CB: E8 40 2D 02 00     call        0000000140192610
  000000014016F8D0: 33 C0              xor         eax,eax
  000000014016F8D2: 85 C0              test        eax,eax
  000000014016F8D4: 75 F5              jne         000000014016F8CB
  000000014016F8D6: B9 C0 01 00 00     mov         ecx,1C0h
  000000014016F8DB: E8 DC F1 2F 01     call        000000014146EABC
  000000014016F8E0: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014016F8E5: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014016F8EB: 74 14              je          000000014016F901
  000000014016F8ED: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014016F8F2: E8 29 48 01 00     call        0000000140184120
  000000014016F8F7: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  000000014016F8FF: EB 0C              jmp         000000014016F90D
  000000014016F901: 48 C7 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],0
                    00 00 00 00 00 00
  000000014016F90D: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  000000014016F915: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014016F91A: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F922: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014016F927: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  000000014016F92B: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F933: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016F937: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F93F: 48 89 01           mov         qword ptr [rcx],rax
  000000014016F942: B9 10 00 00 00     mov         ecx,10h
  000000014016F947: E8 70 F1 2F 01     call        000000014146EABC
  000000014016F94C: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014016F951: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  000000014016F957: 74 31              je          000000014016F98A
  000000014016F959: 48 8B 94 24 30 01  mov         rdx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F961: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014016F966: FF 15 8C 81 C9 01  call        qword ptr [0000000141E07AF8h]
  000000014016F96C: 4C 8B 5C 24 48     mov         r11,qword ptr [rsp+48h]
  000000014016F971: 48 8D 05 E0 93 06  lea         rax,[00000001421D8D58h]
                    02
  000000014016F978: 49 89 03           mov         qword ptr [r11],rax
  000000014016F97B: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014016F980: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  000000014016F988: EB 0C              jmp         000000014016F996
  000000014016F98A: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0
                    00 00 00 00 00 00
  000000014016F996: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  000000014016F99E: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014016F9A3: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F9AB: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016F9AF: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014016F9B4: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000014016F9B8: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F9C0: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016F9C4: 48 C7 80 30 01 00  mov         qword ptr [rax+0000000000000130h],0
                    00 00 00 00 00
  000000014016F9CF: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F9D7: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016F9DB: 48 C7 80 38 01 00  mov         qword ptr [rax+0000000000000138h],0
                    00 00 00 00 00
  000000014016F9E6: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016F9EE: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016F9F2: C6 80 A0 01 00 00  mov         byte ptr [rax+00000000000001A0h],0
                    00
  000000014016F9F9: 48 8B 94 24 30 01  mov         rdx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA01: 48 8B 52 28        mov         rdx,qword ptr [rdx+28h]
  000000014016FA05: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014016FA09: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA11: FF 15 D9 80 C9 01  call        qword ptr [0000000141E07AF0h]
  000000014016FA17: 41 B0 01           mov         r8b,1
  000000014016FA1A: BA 4A 00 00 00     mov         edx,4Ah
  000000014016FA1F: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA27: FF 15 BB 80 C9 01  call        qword ptr [0000000141E07AE8h]
  000000014016FA2D: BA 02 00 00 00     mov         edx,2
  000000014016FA32: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA3A: FF 15 A0 80 C9 01  call        qword ptr [0000000141E07AE0h]
  000000014016FA40: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA48: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FA4C: BA FF FF FF FF     mov         edx,0FFFFFFFFh
  000000014016FA51: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014016FA55: FF 15 7D 80 C9 01  call        qword ptr [0000000141E07AD8h]
  000000014016FA5B: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA63: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FA67: 48 8B 94 24 30 01  mov         rdx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA6F: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014016FA73: FF 15 8F 5B C9 01  call        qword ptr [0000000141E05608h]
  000000014016FA79: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FA81: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FA85: F2 0F 10 05 B3 92  movsd       xmm0,mmword ptr [00000001421D8D40h]
                    06 02
  000000014016FA8D: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  000000014016FA93: F2 0F 10 1D A5 92  movsd       xmm3,mmword ptr [00000001421D8D40h]
                    06 02
  000000014016FA9B: F2 0F 10 15 E5 E3  movsd       xmm2,mmword ptr [00000001421CDE88h]
                    05 02
  000000014016FAA3: F2 0F 10 0D DD E3  movsd       xmm1,mmword ptr [00000001421CDE88h]
                    05 02
  000000014016FAAB: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000014016FAAF: FF 15 6B 79 C9 01  call        qword ptr [0000000141E07420h]
  000000014016FAB5: 48 8D 44 24 50     lea         rax,[rsp+50h]
  000000014016FABA: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014016FABF: 41 B8 02 00 00 00  mov         r8d,2
  000000014016FAC5: BA 01 00 00 00     mov         edx,1
  000000014016FACA: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014016FACF: E8 AC 1B EC FF     call        0000000140031680
  000000014016FAD4: 41 B8 04 00 00 00  mov         r8d,4
  000000014016FADA: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  000000014016FADF: 48 8B C8           mov         rcx,rax
  000000014016FAE2: E8 E9 1D EC FF     call        00000001400318D0
  000000014016FAE7: 48 8B D0           mov         rdx,rax
  000000014016FAEA: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FAF2: FF 15 D8 7F C9 01  call        qword ptr [0000000141E07AD0h]
  000000014016FAF8: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FB00: FF 15 12 7A C9 01  call        qword ptr [0000000141E07518h]
  000000014016FB06: 48 8B C8           mov         rcx,rax
  000000014016FB09: FF 15 01 7A C9 01  call        qword ptr [0000000141E07510h]
  000000014016FB0F: 48 8B C8           mov         rcx,rax
  000000014016FB12: FF 15 F0 79 C9 01  call        qword ptr [0000000141E07508h]
  000000014016FB18: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FB20: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FB24: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FB2B: 48 8B D0           mov         rdx,rax
  000000014016FB2E: FF 15 94 BA C9 01  call        qword ptr [0000000141E0B5C8h]
  000000014016FB34: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FB3C: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FB40: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FB47: FF 15 03 BA C9 01  call        qword ptr [0000000141E0B550h]
  000000014016FB4D: 05 FF 00 00 00     add         eax,0FFh
  000000014016FB52: 99                 cdq
  000000014016FB53: 2B C2              sub         eax,edx
  000000014016FB55: D1 F8              sar         eax,1
  000000014016FB57: 8B F8              mov         edi,eax
  000000014016FB59: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FB61: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FB65: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FB6C: FF 15 E6 B9 C9 01  call        qword ptr [0000000141E0B558h]
  000000014016FB72: 05 FF 00 00 00     add         eax,0FFh
  000000014016FB77: 99                 cdq
  000000014016FB78: 2B C2              sub         eax,edx
  000000014016FB7A: D1 F8              sar         eax,1
  000000014016FB7C: 8B D8              mov         ebx,eax
  000000014016FB7E: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FB86: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FB8A: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FB91: FF 15 C9 B9 C9 01  call        qword ptr [0000000141E0B560h]
  000000014016FB97: 05 FF 00 00 00     add         eax,0FFh
  000000014016FB9C: 99                 cdq
  000000014016FB9D: 2B C2              sub         eax,edx
  000000014016FB9F: D1 F8              sar         eax,1
  000000014016FBA1: C7 44 24 20 FF 00  mov         dword ptr [rsp+20h],0FFh
                    00 00
  000000014016FBA9: 44 8B CF           mov         r9d,edi
  000000014016FBAC: 44 8B C3           mov         r8d,ebx
  000000014016FBAF: 8B D0              mov         edx,eax
  000000014016FBB1: 48 8D 4C 24 64     lea         rcx,[rsp+64h]
  000000014016FBB6: FF 15 04 BE C9 01  call        qword ptr [0000000141E0B9C0h]
  000000014016FBBC: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FBC4: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FBC8: 48 81 C1 B8 00 00  add         rcx,0B8h
                    00
  000000014016FBCF: 48 8B D0           mov         rdx,rax
  000000014016FBD2: FF 15 F0 B9 C9 01  call        qword ptr [0000000141E0B5C8h]
  000000014016FBD8: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FBE0: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FBE4: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FBEB: FF 15 5F B9 C9 01  call        qword ptr [0000000141E0B550h]
  000000014016FBF1: 05 80 00 00 00     add         eax,80h
  000000014016FBF6: 99                 cdq
  000000014016FBF7: 2B C2              sub         eax,edx
  000000014016FBF9: D1 F8              sar         eax,1
  000000014016FBFB: 8B F8              mov         edi,eax
  000000014016FBFD: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FC05: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FC09: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FC10: FF 15 42 B9 C9 01  call        qword ptr [0000000141E0B558h]
  000000014016FC16: 05 80 00 00 00     add         eax,80h
  000000014016FC1B: 99                 cdq
  000000014016FC1C: 2B C2              sub         eax,edx
  000000014016FC1E: D1 F8              sar         eax,1
  000000014016FC20: 8B D8              mov         ebx,eax
  000000014016FC22: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FC2A: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FC2E: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FC35: FF 15 25 B9 C9 01  call        qword ptr [0000000141E0B560h]
  000000014016FC3B: 05 80 00 00 00     add         eax,80h
  000000014016FC40: 99                 cdq
  000000014016FC41: 2B C2              sub         eax,edx
  000000014016FC43: D1 F8              sar         eax,1
  000000014016FC45: C7 44 24 20 FF 00  mov         dword ptr [rsp+20h],0FFh
                    00 00
  000000014016FC4D: 44 8B CF           mov         r9d,edi
  000000014016FC50: 44 8B C3           mov         r8d,ebx
  000000014016FC53: 8B D0              mov         edx,eax
  000000014016FC55: 48 8D 4C 24 74     lea         rcx,[rsp+74h]
  000000014016FC5A: FF 15 60 BD C9 01  call        qword ptr [0000000141E0B9C0h]
  000000014016FC60: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FC68: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FC6C: 48 81 C1 C8 00 00  add         rcx,0C8h
                    00
  000000014016FC73: 48 8B D0           mov         rdx,rax
  000000014016FC76: FF 15 4C B9 C9 01  call        qword ptr [0000000141E0B5C8h]
  000000014016FC7C: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FC84: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FC88: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  000000014016FC8F: F2 0F 10 0D 41 01  movsd       xmm1,mmword ptr [00000001421BFDD8h]
                    05 02
  000000014016FC97: FF 15 03 7C C9 01  call        qword ptr [0000000141E078A0h]
  000000014016FC9D: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FCA5: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FCA9: 48 81 C1 B8 00 00  add         rcx,0B8h
                    00
  000000014016FCB0: F2 0F 10 0D 40 40  movsd       xmm1,mmword ptr [00000001421D3CF8h]
                    06 02
  000000014016FCB8: FF 15 E2 7B C9 01  call        qword ptr [0000000141E078A0h]
  000000014016FCBE: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FCC6: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FCCA: 48 81 C1 C8 00 00  add         rcx,0C8h
                    00
  000000014016FCD1: F2 0F 10 0D FF 00  movsd       xmm1,mmword ptr [00000001421BFDD8h]
                    05 02
  000000014016FCD9: FF 15 C1 7B C9 01  call        qword ptr [0000000141E078A0h]
  000000014016FCDF: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FCE7: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FCEB: C7 40 58 F8 FF FF  mov         dword ptr [rax+58h],0FFFFFFF8h
                    FF
  000000014016FCF2: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FCFA: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FCFE: C7 40 5C 07 00 00  mov         dword ptr [rax+5Ch],7
                    00
  000000014016FD05: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FD0D: 48 89 05 CC 24 C1  mov         qword ptr [0000000143D821E0h],rax
                    03
  000000014016FD14: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FD1C: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FD20: 48 81 C1 00 01 00  add         rcx,100h
                    00
  000000014016FD27: 48 8D 15 AA 7C 06  lea         rdx,[00000001421D79D8h]
                    02
  000000014016FD2E: FF 15 24 41 C9 01  call        qword ptr [0000000141E03E58h]
  000000014016FD34: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FD3C: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FD40: 48 81 C1 08 01 00  add         rcx,108h
                    00
  000000014016FD47: 48 8D 15 AA 7C 06  lea         rdx,[00000001421D79F8h]
                    02
  000000014016FD4E: FF 15 04 41 C9 01  call        qword ptr [0000000141E03E58h]
  000000014016FD54: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FD5C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FD60: C6 80 10 01 00 00  mov         byte ptr [rax+0000000000000110h],1
                    01
  000000014016FD67: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FD6F: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FD73: C6 80 24 01 00 00  mov         byte ptr [rax+0000000000000124h],0
                    00
  000000014016FD7A: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FD82: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FD86: C6 80 25 01 00 00  mov         byte ptr [rax+0000000000000125h],1
                    01
  000000014016FD8D: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FD95: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FD99: C7 80 28 01 00 00  mov         dword ptr [rax+0000000000000128h],1Eh
                    1E 00 00 00
  000000014016FDA3: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FDAB: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FDAF: C7 80 2C 01 00 00  mov         dword ptr [rax+000000000000012Ch],1Eh
                    1E 00 00 00
  000000014016FDB9: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FDC1: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FDC5: C6 80 40 01 00 00  mov         byte ptr [rax+0000000000000140h],1
                    01
  000000014016FDCC: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FDD4: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FDD8: C7 80 44 01 00 00  mov         dword ptr [rax+0000000000000144h],0
                    00 00 00 00
  000000014016FDE2: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FDEA: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FDEE: 48 C7 80 48 01 00  mov         qword ptr [rax+0000000000000148h],0
                    00 00 00 00 00
  000000014016FDF9: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FE01: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FE05: 48 C7 80 50 01 00  mov         qword ptr [rax+0000000000000150h],0
                    00 00 00 00 00
  000000014016FE10: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FE18: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FE1C: 48 C7 80 58 01 00  mov         qword ptr [rax+0000000000000158h],0
                    00 00 00 00 00
  000000014016FE27: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FE2F: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FE33: 48 C7 80 60 01 00  mov         qword ptr [rax+0000000000000160h],0
                    00 00 00 00 00
  000000014016FE3E: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FE46: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014016FE4A: C6 80 80 01 00 00  mov         byte ptr [rax+0000000000000180h],0
                    00
  000000014016FE51: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FE59: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014016FE5D: 48 81 C1 D8 00 00  add         rcx,0D8h
                    00
  000000014016FE64: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000014016FE6C: 4C 8D 0D 95 7B 06  lea         r9,[00000001421D7A08h]
                    02
  000000014016FE73: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FE7B: 48 8D 15 9E 7B 06  lea         rdx,[00000001421D7A20h]
                    02
  000000014016FE82: FF 15 48 43 C9 01  call        qword ptr [0000000141E041D0h]
  000000014016FE88: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  000000014016FE90: 4C 8D 0D A1 7B 06  lea         r9,[00000001421D7A38h]
                    02
  000000014016FE97: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FE9F: 48 8D 15 BA 7B 06  lea         rdx,[00000001421D7A60h]
                    02
  000000014016FEA6: 48 8D 0D 53 68 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FEAD: FF 15 1D 43 C9 01  call        qword ptr [0000000141E041D0h]
  000000014016FEB3: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FEB8: 4C 8D 0D C9 7B 06  lea         r9,[00000001421D7A88h]
                    02
  000000014016FEBF: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FEC7: 48 8D 15 DA 7B 06  lea         rdx,[00000001421D7AA8h]
                    02
  000000014016FECE: 48 8D 0D 2B 68 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FED5: E8 06 FD 15 00     call        00000001402CFBE0
  000000014016FEDA: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FEDF: 4C 8D 0D E2 7B 06  lea         r9,[00000001421D7AC8h]
                    02
  000000014016FEE6: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FEEE: 48 8D 15 EB 7B 06  lea         rdx,[00000001421D7AE0h]
                    02
  000000014016FEF5: 48 8D 0D 04 68 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FEFC: E8 DF FC 15 00     call        00000001402CFBE0
  000000014016FF01: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FF06: 4C 8D 0D F3 7B 06  lea         r9,[00000001421D7B00h]
                    02
  000000014016FF0D: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FF15: 48 8D 15 04 7C 06  lea         rdx,[00000001421D7B20h]
                    02
  000000014016FF1C: 48 8D 0D DD 67 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FF23: E8 B8 FC 15 00     call        00000001402CFBE0
  000000014016FF28: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FF2D: 4C 8D 0D 0C 7C 06  lea         r9,[00000001421D7B40h]
                    02
  000000014016FF34: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FF3C: 48 8D 15 1D 7C 06  lea         rdx,[00000001421D7B60h]
                    02
  000000014016FF43: 48 8D 0D B6 67 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FF4A: E8 91 FC 15 00     call        00000001402CFBE0
  000000014016FF4F: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FF54: 4C 8D 0D 25 7C 06  lea         r9,[00000001421D7B80h]
                    02
  000000014016FF5B: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FF63: 48 8D 15 3E 7C 06  lea         rdx,[00000001421D7BA8h]
                    02
  000000014016FF6A: 48 8D 0D 8F 67 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FF71: E8 6A FC 15 00     call        00000001402CFBE0
  000000014016FF76: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FF7B: 4C 8D 0D 4E 7C 06  lea         r9,[00000001421D7BD0h]
                    02
  000000014016FF82: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FF8A: 48 8D 15 67 7C 06  lea         rdx,[00000001421D7BF8h]
                    02
  000000014016FF91: 48 8D 0D 68 67 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FF98: E8 43 FC 15 00     call        00000001402CFBE0
  000000014016FF9D: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FFA2: 4C 8D 0D 77 7C 06  lea         r9,[00000001421D7C20h]
                    02
  000000014016FFA9: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FFB1: 48 8D 15 90 7C 06  lea         rdx,[00000001421D7C48h]
                    02
  000000014016FFB8: 48 8D 0D 41 67 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FFBF: E8 1C FC 15 00     call        00000001402CFBE0
  000000014016FFC4: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FFC9: 4C 8D 0D A0 7C 06  lea         r9,[00000001421D7C70h]
                    02
  000000014016FFD0: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FFD8: 48 8D 15 B9 7C 06  lea         rdx,[00000001421D7C98h]
                    02
  000000014016FFDF: 48 8D 0D 1A 67 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014016FFE6: E8 F5 FB 15 00     call        00000001402CFBE0
  000000014016FFEB: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014016FFF0: 4C 8D 0D C9 7C 06  lea         r9,[00000001421D7CC0h]
                    02
  000000014016FFF7: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014016FFFF: 48 8D 15 DA 7C 06  lea         rdx,[00000001421D7CE0h]
                    02
  0000000140170006: 48 8D 0D F3 66 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014017000D: E8 CE FB 15 00     call        00000001402CFBE0
  0000000140170012: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140170017: 4C 8D 0D EA 7C 06  lea         r9,[00000001421D7D08h]
                    02
  000000014017001E: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170026: 48 8D 15 F3 7C 06  lea         rdx,[00000001421D7D20h]
                    02
  000000014017002D: 48 8D 0D CC 66 C0  lea         rcx,[0000000143D76700h]
                    03
  0000000140170034: E8 A7 FB 15 00     call        00000001402CFBE0
  0000000140170039: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014017003E: 4C 8D 0D FB 7C 06  lea         r9,[00000001421D7D40h]
                    02
  0000000140170045: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017004D: 48 8D 15 04 7D 06  lea         rdx,[00000001421D7D58h]
                    02
  0000000140170054: 48 8D 0D A5 66 C0  lea         rcx,[0000000143D76700h]
                    03
  000000014017005B: E8 80 FB 15 00     call        00000001402CFBE0
  0000000140170060: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140170065: 4C 8D 0D 0C 7D 06  lea         r9,[00000001421D7D78h]
                    02
  000000014017006C: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170074: 48 8D 15 15 7D 06  lea         rdx,[00000001421D7D90h]
                    02
  000000014017007B: 48 8D 0D 7E 66 C0  lea         rcx,[0000000143D76700h]
                    03
  0000000140170082: E8 59 FB 15 00     call        00000001402CFBE0
  0000000140170087: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014017008C: 4C 8D 0D 1D 7D 06  lea         r9,[00000001421D7DB0h]
                    02
  0000000140170093: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017009B: 48 8D 15 26 7D 06  lea         rdx,[00000001421D7DC8h]
                    02
  00000001401700A2: 48 8D 0D 57 66 C0  lea         rcx,[0000000143D76700h]
                    03
  00000001401700A9: E8 32 FB 15 00     call        00000001402CFBE0
  00000001401700AE: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001401700B3: 4C 8D 0D 2E 7D 06  lea         r9,[00000001421D7DE8h]
                    02
  00000001401700BA: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001401700C2: 48 8D 15 37 7D 06  lea         rdx,[00000001421D7E00h]
                    02
  00000001401700C9: 48 8D 0D 30 66 C0  lea         rcx,[0000000143D76700h]
                    03
  00000001401700D0: E8 0B FB 15 00     call        00000001402CFBE0
  00000001401700D5: BA 02 00 00 00     mov         edx,2
  00000001401700DA: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001401700E2: FF 15 E0 79 C9 01  call        qword ptr [0000000141E07AC8h]
  00000001401700E8: 48 8D 84 24 84 00  lea         rax,[rsp+0000000000000084h]
                    00 00
  00000001401700F0: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001401700F8: BA 01 00 00 00     mov         edx,1
  00000001401700FD: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140170105: E8 56 49 E9 FF     call        0000000140004A60
  000000014017010A: 48 8B D0           mov         rdx,rax
  000000014017010D: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170115: FF 15 A5 79 C9 01  call        qword ptr [0000000141E07AC0h]
  000000014017011B: BA 0B 00 00 00     mov         edx,0Bh
  0000000140170120: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170128: FF 15 8A 79 C9 01  call        qword ptr [0000000141E07AB8h]
  000000014017012E: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170136: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014017013A: 48 C7 80 88 01 00  mov         qword ptr [rax+0000000000000188h],0
                    00 00 00 00 00
  0000000140170145: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017014D: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170151: 48 C7 80 90 01 00  mov         qword ptr [rax+0000000000000190h],0
                    00 00 00 00 00
  000000014017015C: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170164: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170168: C6 80 98 01 00 00  mov         byte ptr [rax+0000000000000198h],1
                    01
  000000014017016F: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170177: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014017017B: C6 80 99 01 00 00  mov         byte ptr [rax+0000000000000199h],0
                    00
  0000000140170182: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017018A: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014017018E: C6 80 9A 01 00 00  mov         byte ptr [rax+000000000000019Ah],0
                    00
  0000000140170195: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017019D: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401701A1: C6 80 9B 01 00 00  mov         byte ptr [rax+000000000000019Bh],0
                    00
  00000001401701A8: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001401701B0: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401701B4: C6 80 9C 01 00 00  mov         byte ptr [rax+000000000000019Ch],0
                    00
  00000001401701BB: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001401701C3: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401701C7: C6 80 9D 01 00 00  mov         byte ptr [rax+000000000000019Dh],0
                    00
  00000001401701CE: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001401701D6: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401701DA: C6 80 9E 01 00 00  mov         byte ptr [rax+000000000000019Eh],0
                    00
  00000001401701E1: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001401701E9: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401701ED: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  00000001401701F5: B9 48 00 00 00     mov         ecx,48h
  00000001401701FA: E8 BD E8 2F 01     call        000000014146EABC
  00000001401701FF: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140170207: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  0000000140170210: 0F 84 92 00 00 00  je          00000001401702A8
  0000000140170216: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  000000014017021E: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140170226: 48 8D 84 24 B0 00  lea         rax,[rsp+00000000000000B0h]
                    00 00
  000000014017022E: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140170236: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014017023E: FF 15 7C 42 C9 01  call        qword ptr [0000000141E044C0h]
  0000000140170244: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  000000014017024C: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140170254: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  000000014017025C: 48 8D 15 FC 3D 06  lea         rdx,[00000001421D405Fh]
                    02
  0000000140170263: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014017026B: FF 15 0F 42 C9 01  call        qword ptr [0000000141E04480h]
  0000000140170271: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140170279: 4C 8B 8C 24 F0 00  mov         r9,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140170281: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170289: 48 8B 94 24 F8 00  mov         rdx,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140170291: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140170299: E8 F2 A7 BE 00     call        0000000140D5AA90
  000000014017029E: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000001401702A6: EB 0C              jmp         00000001401702B4
  00000001401702A8: 48 C7 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00 00
  00000001401702B4: 48 8B 84 24 00 01  mov         rax,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001401702BC: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001401702C4: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001401702CC: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401702D0: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001401702D8: 48 89 81 A8 01 00  mov         qword ptr [rcx+00000000000001A8h],rax
                    00
  00000001401702DF: B9 40 00 00 00     mov         ecx,40h
  00000001401702E4: E8 D3 E7 2F 01     call        000000014146EABC
  00000001401702E9: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  00000001401702F1: 48 83 BC 24 C8 00  cmp         qword ptr [rsp+00000000000000C8h],0
                    00 00 00
  00000001401702FA: 74 1F              je          000000014017031B
  00000001401702FC: 48 8B 94 24 30 01  mov         rdx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140170304: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014017030C: E8 4F 33 01 00     call        0000000140183660
  0000000140170311: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140170319: EB 0C              jmp         0000000140170327
  000000014017031B: 48 C7 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],0
                    00 00 00 00 00 00
  0000000140170327: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  000000014017032F: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140170337: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017033F: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140170343: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014017034B: 48 89 81 B0 01 00  mov         qword ptr [rcx+00000000000001B0h],rax
                    00
  0000000140170352: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017035A: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014017035E: C6 80 B8 01 00 00  mov         byte ptr [rax+00000000000001B8h],1
                    01
  0000000140170365: 48 8B 84 24 30 01  mov         rax,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014017036D: 48 81 C4 18 01 00  add         rsp,118h
                    00
  0000000140170374: 5F                 pop         rdi
  0000000140170375: 5B                 pop         rbx
  0000000140170376: C3                 ret

  Summary

     1E02000 .text
