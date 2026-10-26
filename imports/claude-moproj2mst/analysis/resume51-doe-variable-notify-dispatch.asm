Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CF3C0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001402CF3C5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001402CF3CA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402CF3CF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CF3D4: 56                 push        rsi
  00000001402CF3D5: 57                 push        rdi
  00000001402CF3D6: 48 81 EC 88 01 00  sub         rsp,188h
                    00
  00000001402CF3DD: 48 C7 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001402CF3E9: 48 8B 84 24 A0 01  mov         rax,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001402CF3F1: 0F B6 80 41 01 00  movzx       eax,byte ptr [rax+0000000000000141h]
                    00
  00000001402CF3F8: 85 C0              test        eax,eax
  00000001402CF3FA: 0F 85 2E 03 00 00  jne         00000001402CF72E
  00000001402CF400: 48 8B 84 24 A0 01  mov         rax,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001402CF408: C6 80 90 00 00 00  mov         byte ptr [rax+0000000000000090h],0
                    00
  00000001402CF40F: 48 8B 94 24 A8 01  mov         rdx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001402CF417: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402CF41F: FF 15 5B 50 B3 01  call        qword ptr [0000000141E04480h]
  00000001402CF425: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  00000001402CF42D: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001402CF435: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001402CF43D: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001402CF445: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CF44C: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001402CF454: E8 97 0F 00 00     call        00000001402D03F0
  00000001402CF459: 48 8B D0           mov         rdx,rax
  00000001402CF45C: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF461: E8 0A FC 01 00     call        00000001402EF070
  00000001402CF466: 90                 nop
  00000001402CF467: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402CF46F: FF 15 6B 65 B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CF475: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF47A: E8 61 56 D3 FF     call        0000000140004AE0
  00000001402CF47F: 89 44 24 7C        mov         dword ptr [rsp+7Ch],eax
  00000001402CF483: C7 44 24 78 00 00  mov         dword ptr [rsp+78h],0
                    00 00
  00000001402CF48B: EB 0B              jmp         00000001402CF498
  00000001402CF48D: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  00000001402CF491: 83 C0 01           add         eax,1
  00000001402CF494: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  00000001402CF498: 8B 44 24 7C        mov         eax,dword ptr [rsp+7Ch]
  00000001402CF49C: 39 44 24 78        cmp         dword ptr [rsp+78h],eax
  00000001402CF4A0: 0F 8D 7E 02 00 00  jge         00000001402CF724
  00000001402CF4A6: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001402CF4AA: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF4AF: E8 5C F9 01 00     call        00000001402EEE10
  00000001402CF4B4: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402CF4B7: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001402CF4BF: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF4C7: E8 24 BA 94 00     call        0000000140C1AEF0
  00000001402CF4CC: 48 85 C0           test        rax,rax
  00000001402CF4CF: 0F 84 0E 02 00 00  je          00000001402CF6E3
  00000001402CF4D5: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF4DD: E8 0E BA 94 00     call        0000000140C1AEF0
  00000001402CF4E2: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  00000001402CF4EA: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001402CF4F2: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402CF4F5: 48 8B 8C 24 68 01  mov         rcx,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001402CF4FD: FF 10              call        qword ptr [rax]
  00000001402CF4FF: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001402CF507: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  00000001402CF50F: 48 8B B4 24 F8 01  mov         rsi,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001402CF517: B9 10 00 00 00     mov         ecx,10h
  00000001402CF51C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF51E: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  00000001402CF526: 48 8B B4 24 F0 01  mov         rsi,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001402CF52E: B9 10 00 00 00     mov         ecx,10h
  00000001402CF533: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF535: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  00000001402CF53D: 48 8B B4 24 E8 01  mov         rsi,qword ptr [rsp+00000000000001E8h]
                    00 00
  00000001402CF545: B9 10 00 00 00     mov         ecx,10h
  00000001402CF54A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF54C: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  00000001402CF554: 48 8B B4 24 E0 01  mov         rsi,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001402CF55C: B9 10 00 00 00     mov         ecx,10h
  00000001402CF561: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF563: 48 8D BC 24 00 01  lea         rdi,[rsp+0000000000000100h]
                    00 00
  00000001402CF56B: 48 8B B4 24 D8 01  mov         rsi,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001402CF573: B9 10 00 00 00     mov         ecx,10h
  00000001402CF578: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF57A: 48 8D BC 24 F0 00  lea         rdi,[rsp+00000000000000F0h]
                    00 00
  00000001402CF582: 48 8B B4 24 D0 01  mov         rsi,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001402CF58A: B9 10 00 00 00     mov         ecx,10h
  00000001402CF58F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF591: 48 8D BC 24 E0 00  lea         rdi,[rsp+00000000000000E0h]
                    00 00
  00000001402CF599: 48 8B B4 24 C8 01  mov         rsi,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001402CF5A1: B9 10 00 00 00     mov         ecx,10h
  00000001402CF5A6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF5A8: 48 8D BC 24 D0 00  lea         rdi,[rsp+00000000000000D0h]
                    00 00
  00000001402CF5B0: 48 8B B4 24 C0 01  mov         rsi,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001402CF5B8: B9 10 00 00 00     mov         ecx,10h
  00000001402CF5BD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF5BF: 48 8D BC 24 C0 00  lea         rdi,[rsp+00000000000000C0h]
                    00 00
  00000001402CF5C7: 48 8B B4 24 B8 01  mov         rsi,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000001402CF5CF: B9 10 00 00 00     mov         ecx,10h
  00000001402CF5D4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF5D6: 48 8D BC 24 B0 00  lea         rdi,[rsp+00000000000000B0h]
                    00 00
  00000001402CF5DE: 48 8B B4 24 B0 01  mov         rsi,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001402CF5E6: B9 10 00 00 00     mov         ecx,10h
  00000001402CF5EB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF5ED: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF5F5: 48 83 C1 08        add         rcx,8
  00000001402CF5F9: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000001402CF601: FF 15 19 62 B3 01  call        qword ptr [0000000141E05820h]
  00000001402CF607: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  00000001402CF60F: 48 8B 84 24 70 01  mov         rax,qword ptr [rsp+0000000000000170h]
                    00 00
  00000001402CF617: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  00000001402CF61F: 48 8B 8C 24 78 01  mov         rcx,qword ptr [rsp+0000000000000178h]
                    00 00
  00000001402CF627: FF 15 FB 4B B3 01  call        qword ptr [0000000141E04228h]
  00000001402CF62D: 48 8B F8           mov         rdi,rax
  00000001402CF630: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF638: E8 B3 B8 94 00     call        0000000140C1AEF0
  00000001402CF63D: 48 8B C8           mov         rcx,rax
  00000001402CF640: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  00000001402CF648: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001402CF64D: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  00000001402CF655: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001402CF65A: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  00000001402CF662: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001402CF667: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  00000001402CF66F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001402CF674: 48 8D 84 24 00 01  lea         rax,[rsp+0000000000000100h]
                    00 00
  00000001402CF67C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001402CF681: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  00000001402CF689: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001402CF68E: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  00000001402CF696: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001402CF69B: 48 8D 84 24 D0 00  lea         rax,[rsp+00000000000000D0h]
                    00 00
  00000001402CF6A3: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001402CF6A8: 48 8D 84 24 C0 00  lea         rax,[rsp+00000000000000C0h]
                    00 00
  00000001402CF6B0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402CF6B5: 4C 8D 8C 24 B0 00  lea         r9,[rsp+00000000000000B0h]
                    00 00
  00000001402CF6BD: 41 B8 01 00 00 00  mov         r8d,1
  00000001402CF6C3: 48 8B D7           mov         rdx,rdi
  00000001402CF6C6: FF 15 8C 5F B3 01  call        qword ptr [0000000141E05658h]
  00000001402CF6CC: 88 84 24 8C 00 00  mov         byte ptr [rsp+000000000000008Ch],al
                    00
  00000001402CF6D3: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001402CF6DB: FF 15 AF 4B B3 01  call        qword ptr [0000000141E04290h]
  00000001402CF6E1: EB 3C              jmp         00000001402CF71F
  00000001402CF6E3: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001402CF6E7: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF6EC: E8 FF 91 54 00     call        00000001408188F0
  00000001402CF6F1: 8B 44 24 7C        mov         eax,dword ptr [rsp+7Ch]
  00000001402CF6F5: 83 E8 01           sub         eax,1
  00000001402CF6F8: 89 44 24 7C        mov         dword ptr [rsp+7Ch],eax
  00000001402CF6FC: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  00000001402CF700: 83 E8 01           sub         eax,1
  00000001402CF703: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  00000001402CF707: 83 7C 24 78 FF     cmp         dword ptr [rsp+78h],0FFFFFFFFh
  00000001402CF70C: 7D 08              jge         00000001402CF716
  00000001402CF70E: C7 44 24 78 FF FF  mov         dword ptr [rsp+78h],0FFFFFFFFh
                    FF FF
  00000001402CF716: 83 7C 24 7C 00     cmp         dword ptr [rsp+7Ch],0
  00000001402CF71B: 7F 02              jg          00000001402CF71F
  00000001402CF71D: EB 05              jmp         00000001402CF724
  00000001402CF71F: E9 69 FD FF FF     jmp         00000001402CF48D
  00000001402CF724: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF729: E8 72 8A F3 FF     call        00000001402081A0
  00000001402CF72E: 48 81 C4 88 01 00  add         rsp,188h
                    00
  00000001402CF735: 5F                 pop         rdi
  00000001402CF736: 5E                 pop         rsi
  00000001402CF737: C3                 ret

  Summary

     1E02000 .text
