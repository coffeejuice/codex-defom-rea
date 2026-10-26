Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014090A550: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014090A555: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014090A55A: 56                 push        rsi
  000000014090A55B: 57                 push        rdi
  000000014090A55C: 48 81 EC B8 0C 00  sub         rsp,0CB8h
                    00
  000000014090A563: 48 C7 84 24 70 09  mov         qword ptr [rsp+0000000000000970h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014090A56F: 0F 29 B4 24 A0 0C  movaps      xmmword ptr [rsp+0000000000000CA0h],xmm6
                    00 00
  000000014090A577: 0F 29 BC 24 90 0C  movaps      xmmword ptr [rsp+0000000000000C90h],xmm7
                    00 00
  000000014090A57F: 44 0F 29 84 24 80  movaps      xmmword ptr [rsp+0000000000000C80h],xmm8
                    0C 00 00
  000000014090A588: 44 0F 29 8C 24 70  movaps      xmmword ptr [rsp+0000000000000C70h],xmm9
                    0C 00 00
  000000014090A591: C7 84 24 2C 09 00  mov         dword ptr [rsp+000000000000092Ch],0
                    00 00 00 00 00
  000000014090A59C: C6 44 24 78 00     mov         byte ptr [rsp+78h],0
  000000014090A5A1: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090A5A9: E8 72 22 01 00     call        000000014091C820
  000000014090A5AE: 48 8B 94 24 D0 0C  mov         rdx,qword ptr [rsp+0000000000000CD0h]
                    00 00
  000000014090A5B6: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014090A5BE: FF 15 14 9A 4F 01  call        qword ptr [0000000141E03FD8h]
  000000014090A5C4: 90                 nop
  000000014090A5C5: 48 8D 84 24 8C 03  lea         rax,[rsp+000000000000038Ch]
                    00 00
  000000014090A5CD: 48 89 84 24 90 03  mov         qword ptr [rsp+0000000000000390h],rax
                    00 00
  000000014090A5D5: 41 B8 10 00 00 00  mov         r8d,10h
  000000014090A5DB: BA 02 00 00 00     mov         edx,2
  000000014090A5E0: 48 8B 8C 24 90 03  mov         rcx,qword ptr [rsp+0000000000000390h]
                    00 00
  000000014090A5E8: E8 93 70 72 FF     call        0000000140031680
  000000014090A5ED: 48 8B D0           mov         rdx,rax
  000000014090A5F0: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014090A5F8: FF 15 E2 99 4F 01  call        qword ptr [0000000141E03FE0h]
  000000014090A5FE: 0F B6 C0           movzx       eax,al
  000000014090A601: 85 C0              test        eax,eax
  000000014090A603: 0F 85 FB 00 00 00  jne         000000014090A704
  000000014090A609: B2 20              mov         dl,20h
  000000014090A60B: 48 8D 8C 24 A2 03  lea         rcx,[rsp+00000000000003A2h]
                    00 00
  000000014090A613: E8 48 82 6F FF     call        0000000140002860
  000000014090A618: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090A61B: 48 8D 8C 24 A0 03  lea         rcx,[rsp+00000000000003A0h]
                    00 00
  000000014090A623: FF 15 2F 9D 4F 01  call        qword ptr [0000000141E04358h]
  000000014090A629: 48 8D 15 30 09 EB  lea         rdx,[00000001427BAF60h]
                    01
  000000014090A630: 48 8D 8C 24 98 03  lea         rcx,[rsp+0000000000000398h]
                    00 00
  000000014090A638: FF 15 42 9E 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A63E: 48 89 84 24 78 09  mov         qword ptr [rsp+0000000000000978h],rax
                    00 00
  000000014090A646: 48 8B 84 24 78 09  mov         rax,qword ptr [rsp+0000000000000978h]
                    00 00
  000000014090A64E: 48 89 84 24 80 09  mov         qword ptr [rsp+0000000000000980h],rax
                    00 00
  000000014090A656: 48 8D 84 24 A0 03  lea         rax,[rsp+00000000000003A0h]
                    00 00
  000000014090A65E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014090A663: 45 33 C9           xor         r9d,r9d
  000000014090A666: 4C 8B 84 24 D0 0C  mov         r8,qword ptr [rsp+0000000000000CD0h]
                    00 00
  000000014090A66E: 48 8D 94 24 A8 03  lea         rdx,[rsp+00000000000003A8h]
                    00 00
  000000014090A676: 48 8B 8C 24 80 09  mov         rcx,qword ptr [rsp+0000000000000980h]
                    00 00
  000000014090A67E: FF 15 DC 9C 4F 01  call        qword ptr [0000000141E04360h]
  000000014090A684: 48 89 84 24 88 09  mov         qword ptr [rsp+0000000000000988h],rax
                    00 00
  000000014090A68C: 48 8B 84 24 88 09  mov         rax,qword ptr [rsp+0000000000000988h]
                    00 00
  000000014090A694: 48 89 84 24 90 09  mov         qword ptr [rsp+0000000000000990h],rax
                    00 00
  000000014090A69C: 48 8B 94 24 90 09  mov         rdx,qword ptr [rsp+0000000000000990h]
                    00 00
  000000014090A6A4: 48 8D 0D 85 F3 4C  lea         rcx,[0000000143DD9A30h]
                    03
  000000014090A6AB: FF 15 BF 9D 4F 01  call        qword ptr [0000000141E04470h]
  000000014090A6B1: 90                 nop
  000000014090A6B2: 48 8D 8C 24 A8 03  lea         rcx,[rsp+00000000000003A8h]
                    00 00
  000000014090A6BA: FF 15 20 B3 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A6C0: 90                 nop
  000000014090A6C1: 48 8D 8C 24 98 03  lea         rcx,[rsp+0000000000000398h]
                    00 00
  000000014090A6C9: FF 15 11 B3 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A6CF: 0F B6 44 24 78     movzx       eax,byte ptr [rsp+78h]
  000000014090A6D4: 88 84 24 B0 03 00  mov         byte ptr [rsp+00000000000003B0h],al
                    00
  000000014090A6DB: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014090A6E3: FF 15 37 99 4F 01  call        qword ptr [0000000141E04020h]
  000000014090A6E9: 90                 nop
  000000014090A6EA: 48 8B 8C 24 D0 0C  mov         rcx,qword ptr [rsp+0000000000000CD0h]
                    00 00
  000000014090A6F2: FF 15 E8 B2 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A6F8: 8A 84 24 B0 03 00  mov         al,byte ptr [rsp+00000000000003B0h]
                    00
  000000014090A6FF: E9

  Summary

     1E02000 .text
