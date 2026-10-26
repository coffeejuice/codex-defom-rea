Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505F940: 40 53              push        rbx
  000000006505F942: 55                 push        rbp
  000000006505F943: 56                 push        rsi
  000000006505F944: 57                 push        rdi
  000000006505F945: 41 54              push        r12
  000000006505F947: 48 83 EC 40        sub         rsp,40h
  000000006505F94B: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006505F954: 48 8B F1           mov         rsi,rcx
  000000006505F957: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006505F95B: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006505F95F: 8B 90 CC 01 00 00  mov         edx,dword ptr [rax+00000000000001CCh]
  000000006505F965: C1 EA 17           shr         edx,17h
  000000006505F968: 80 E2 01           and         dl,1
  000000006505F96B: 0F 85 1C 01 00 00  jne         000000006505FA8D
  000000006505F971: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006505F975: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006505F979: A8 01              test        al,1
  000000006505F97B: 74 13              je          000000006505F990
  000000006505F97D: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006505F985: FF 15 FD 6F 63 00  call        qword ptr [0000000065696988h]
  000000006505F98B: 48 8B E8           mov         rbp,rax
  000000006505F98E: EB 21              jmp         000000006505F9B1
  000000006505F990: FF 15 D2 6E 63 00  call        qword ptr [0000000065696868h]
  000000006505F996: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  000000006505F99A: 48 81 C2 A8 01 00  add         rdx,1A8h
                    00
  000000006505F9A1: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006505F9A6: FF 15 E4 70 63 00  call        qword ptr [0000000065696A90h]
  000000006505F9AC: 48 8D 6C 24 78     lea         rbp,[rsp+78h]
  000000006505F9B1: 4C 8B 66 08        mov         r12,qword ptr [rsi+8]
  000000006505F9B5: 48 8B D5           mov         rdx,rbp
  000000006505F9B8: 48 8D 8E A8 01 00  lea         rcx,[rsi+00000000000001A8h]
                    00
  000000006505F9BF: FF 15 6B 79 63 00  call        qword ptr [0000000065697330h]
  000000006505F9C5: 84 C0              test        al,al
  000000006505F9C7: 0F 85 C0 00 00 00  jne         000000006505FA8D
  000000006505F9CD: 48 8B D5           mov         rdx,rbp
  000000006505F9D0: 48 8D 8E A8 01 00  lea         rcx,[rsi+00000000000001A8h]
                    00
  000000006505F9D7: FF 15 43 6F 63 00  call        qword ptr [0000000065696920h]
  000000006505F9DD: 48 83 C6 18        add         rsi,18h
  000000006505F9E1: 48 8B CE           mov         rcx,rsi
  000000006505F9E4: FF 15 E6 6E 63 00  call        qword ptr [00000000656968D0h]
  000000006505F9EA: 84 C0              test        al,al
  000000006505F9EC: 75 74              jne         000000006505FA62
  000000006505F9EE: 33 FF              xor         edi,edi
  000000006505F9F0: 48 8B CE           mov         rcx,rsi
  000000006505F9F3: FF 15 BF 6E 63 00  call        qword ptr [00000000656968B8h]
  000000006505F9F9: 85 C0              test        eax,eax
  000000006505F9FB: 7E 65              jle         000000006505FA62
  000000006505F9FD: 0F 1F 00           nop         dword ptr [rax]
  000000006505FA00: 8B D7              mov         edx,edi
  000000006505FA02: 48 8B CE           mov         rcx,rsi
  000000006505FA05: FF 15 35 6F 63 00  call        qword ptr [0000000065696940h]
  000000006505FA0B: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006505FA0E: 48 85 DB           test        rbx,rbx
  000000006505FA11: 74 40              je          000000006505FA53
  000000006505FA13: 48 8B CB           mov         rcx,rbx
  000000006505FA16: FF 15 3C 6E 63 00  call        qword ptr [0000000065696858h]
  000000006505FA1C: 84 C0              test        al,al
  000000006505FA1E: 74 33              je          000000006505FA53
  000000006505FA20: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006505FA24: 8B 81 CC 01 00 00  mov         eax,dword ptr [rcx+00000000000001CCh]
  000000006505FA2A: C1 E8 17           shr         eax,17h
  000000006505FA2D: 24 01              and         al,1
  000000006505FA2F: 75 22              jne         000000006505FA53
  000000006505FA31: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006505FA35: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006505FA39: A8 01              test        al,1
  000000006505FA3B: 74 0B              je          000000006505FA48
  000000006505FA3D: 0F B6 81 CE 01 00  movzx       eax,byte ptr [rcx+00000000000001CEh]
                    00
  000000006505FA44: 24 01              and         al,1
  000000006505FA46: 74 0B              je          000000006505FA53
  000000006505FA48: 45 33 C0           xor         r8d,r8d
  000000006505FA4B: 48 8B D5           mov         rdx,rbp
  000000006505FA4E: E8 CD FD FF FF     call        000000006505F820
  000000006505FA53: FF C7              inc         edi
  000000006505FA55: 48 8B CE           mov         rcx,rsi
  000000006505FA58: FF 15 5A 6E 63 00  call        qword ptr [00000000656968B8h]
  000000006505FA5E: 3B F8              cmp         edi,eax
  000000006505FA60: 7C 9E              jl          000000006505FA00
  000000006505FA62: BA 58 00 00 00     mov         edx,58h
  000000006505FA67: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505FA6C: FF 15 A6 6D 63 00  call        qword ptr [0000000065696818h]
  000000006505FA72: 90                 nop
  000000006505FA73: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006505FA78: 49 8B CC           mov         rcx,r12
  000000006505FA7B: FF 15 8F 6D 63 00  call        qword ptr [0000000065696810h]
  000000006505FA81: 90                 nop
  000000006505FA82: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006505FA87: FF 15 7B 6D 63 00  call        qword ptr [0000000065696808h]
  000000006505FA8D: 48 83 C4 40        add         rsp,40h
  000000006505FA91: 41 5C              pop         r12
  000000006505FA93: 5F                 pop         rdi
  000000006505FA94: 5E                 pop         rsi
  000000006505FA95: 5D                 pop         rbp
  000000006505FA96: 5B                 pop         rbx
  000000006505FA97: C3                 ret

  Summary

      695000 .text
