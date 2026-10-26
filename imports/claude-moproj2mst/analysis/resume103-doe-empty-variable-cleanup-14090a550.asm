Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

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
  000000014090A6FF: E9 49 39 00 00     jmp         000000014090E04D
  000000014090A704: B9 08 00 00 00     mov         ecx,8
  000000014090A709: E8 AE 43 B6 00     call        000000014146EABC
  000000014090A70E: 48 89 84 24 C0 03  mov         qword ptr [rsp+00000000000003C0h],rax
                    00 00
  000000014090A716: 48 83 BC 24 C0 03  cmp         qword ptr [rsp+00000000000003C0h],0
                    00 00 00
  000000014090A71F: 74 47              je          000000014090A768
  000000014090A721: 48 8D 15 50 08 EB  lea         rdx,[00000001427BAF78h]
                    01
  000000014090A728: 48 8D 8C 24 C8 03  lea         rcx,[rsp+00000000000003C8h]
                    00 00
  000000014090A730: FF 15 4A 9D 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A736: 90                 nop
  000000014090A737: 8B 84 24 2C 09 00  mov         eax,dword ptr [rsp+000000000000092Ch]
                    00
  000000014090A73E: 83 C8 01           or          eax,1
  000000014090A741: 89 84 24 2C 09 00  mov         dword ptr [rsp+000000000000092Ch],eax
                    00
  000000014090A748: 48 8D 94 24 C8 03  lea         rdx,[rsp+00000000000003C8h]
                    00 00
  000000014090A750: 48 8B 8C 24 C0 03  mov         rcx,qword ptr [rsp+00000000000003C0h]
                    00 00
  000000014090A758: FF 15 12 1E 50 01  call        qword ptr [0000000141E0C570h]
  000000014090A75E: 48 89 84 24 98 09  mov         qword ptr [rsp+0000000000000998h],rax
                    00 00
  000000014090A766: EB 0C              jmp         000000014090A774
  000000014090A768: 48 C7 84 24 98 09  mov         qword ptr [rsp+0000000000000998h],0
                    00 00 00 00 00 00
  000000014090A774: 48 8B 84 24 98 09  mov         rax,qword ptr [rsp+0000000000000998h]
                    00 00
  000000014090A77C: 48 89 84 24 B8 03  mov         qword ptr [rsp+00000000000003B8h],rax
                    00 00
  000000014090A784: 48 8B 84 24 B8 03  mov         rax,qword ptr [rsp+00000000000003B8h]
                    00 00
  000000014090A78C: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014090A791: 8B 84 24 2C 09 00  mov         eax,dword ptr [rsp+000000000000092Ch]
                    00
  000000014090A798: 83 E0 01           and         eax,1
  000000014090A79B: 85 C0              test        eax,eax
  000000014090A79D: 74 16              je          000000014090A7B5
  000000014090A79F: 83 A4 24 2C 09 00  and         dword ptr [rsp+000000000000092Ch],0FFFFFFFEh
                    00 FE
  000000014090A7A7: 48 8D 8C 24 C8 03  lea         rcx,[rsp+00000000000003C8h]
                    00 00
  000000014090A7AF: FF 15 2B B2 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A7B5: 48 8D 15 D4 07 EB  lea         rdx,[00000001427BAF90h]
                    01
  000000014090A7BC: 48 8D 8C 24 D0 03  lea         rcx,[rsp+00000000000003D0h]
                    00 00
  000000014090A7C4: FF 15 B6 9C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A7CA: 90                 nop
  000000014090A7CB: 4C 8D 84 24 D0 03  lea         r8,[rsp+00000000000003D0h]
                    00 00
  000000014090A7D3: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  000000014090A7DB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090A7E0: FF 15 A2 1E 50 01  call        qword ptr [0000000141E0C688h]
  000000014090A7E6: 90                 nop
  000000014090A7E7: 48 8D 8C 24 D0 03  lea         rcx,[rsp+00000000000003D0h]
                    00 00
  000000014090A7EF: FF 15 EB B1 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A7F5: 4C 8D 84 24 98 00  lea         r8,[rsp+0000000000000098h]
                    00 00
  000000014090A7FD: 48 8D 94 24 D8 03  lea         rdx,[rsp+00000000000003D8h]
                    00 00
  000000014090A805: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090A80A: FF 15 90 1E 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090A810: 48 8D 8C 24 D8 03  lea         rcx,[rsp+00000000000003D8h]
                    00 00
  000000014090A818: FF 15 8A 1E 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090A81E: 48 8D 15 7B 07 EB  lea         rdx,[00000001427BAFA0h]
                    01
  000000014090A825: 48 8D 8C 24 E0 03  lea         rcx,[rsp+00000000000003E0h]
                    00 00
  000000014090A82D: FF 15 4D 9C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A833: 90                 nop
  000000014090A834: F2 0F 10 15 A4 A1  movsd       xmm2,mmword ptr [00000001421249E0h]
                    81 01
  000000014090A83C: 48 8D 94 24 E0 03  lea         rdx,[rsp+00000000000003E0h]
                    00 00
  000000014090A844: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090A84C: FF 15 36 1C 50 01  call        qword ptr [0000000141E0C488h]
  000000014090A852: 90                 nop
  000000014090A853: 48 8D 8C 24 E0 03  lea         rcx,[rsp+00000000000003E0h]
                    00 00
  000000014090A85B: FF 15 7F B1 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A861: 48 8D 15 40 07 EB  lea         rdx,[00000001427BAFA8h]
                    01
  000000014090A868: 48 8D 8C 24 E8 03  lea         rcx,[rsp+00000000000003E8h]
                    00 00
  000000014090A870: FF 15 0A 9C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A876: 90                 nop
  000000014090A877: 4C 8D 84 24 E8 03  lea         r8,[rsp+00000000000003E8h]
                    00 00
  000000014090A87F: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000014090A884: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090A889: FF 15 F9 1D 50 01  call        qword ptr [0000000141E0C688h]
  000000014090A88F: 90                 nop
  000000014090A890: 48 8D 8C 24 E8 03  lea         rcx,[rsp+00000000000003E8h]
                    00 00
  000000014090A898: FF 15 42 B1 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A89E: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  000000014090A8A3: 48 8D 94 24 F0 03  lea         rdx,[rsp+00000000000003F0h]
                    00 00
  000000014090A8AB: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090A8B3: FF 15 E7 1D 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090A8B9: 48 8D 8C 24 F0 03  lea         rcx,[rsp+00000000000003F0h]
                    00 00
  000000014090A8C1: FF 15 E1 1D 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090A8C7: 48 8D 15 EE 06 EB  lea         rdx,[00000001427BAFBCh]
                    01
  000000014090A8CE: 48 8D 8C 24 00 04  lea         rcx,[rsp+0000000000000400h]
                    00 00
  000000014090A8D6: FF 15 A4 9B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A8DC: 90                 nop
  000000014090A8DD: 48 8D 94 24 F8 03  lea         rdx,[rsp+00000000000003F8h]
                    00 00
  000000014090A8E5: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090A8ED: E8 4E 1D 7B FF     call        00000001400BC640
  000000014090A8F2: 48 89 84 24 A0 09  mov         qword ptr [rsp+00000000000009A0h],rax
                    00 00
  000000014090A8FA: 48 8B 84 24 A0 09  mov         rax,qword ptr [rsp+00000000000009A0h]
                    00 00
  000000014090A902: 48 89 84 24 A8 09  mov         qword ptr [rsp+00000000000009A8h],rax
                    00 00
  000000014090A90A: 4C 8B 84 24 A8 09  mov         r8,qword ptr [rsp+00000000000009A8h]
                    00 00
  000000014090A912: 48 8D 94 24 00 04  lea         rdx,[rsp+0000000000000400h]
                    00 00
  000000014090A91A: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090A91F: FF 15 F3 1C 50 01  call        qword ptr [0000000141E0C618h]
  000000014090A925: 90                 nop
  000000014090A926: 48 8D 8C 24 F8 03  lea         rcx,[rsp+00000000000003F8h]
                    00 00
  000000014090A92E: FF 15 AC B0 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A934: 90                 nop
  000000014090A935: 48 8D 8C 24 00 04  lea         rcx,[rsp+0000000000000400h]
                    00 00
  000000014090A93D: FF 15 9D B0 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A943: 48 8D 15 7E 06 EB  lea         rdx,[00000001427BAFC8h]
                    01
  000000014090A94A: 48 8D 8C 24 08 04  lea         rcx,[rsp+0000000000000408h]
                    00 00
  000000014090A952: FF 15 28 9B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A958: 90                 nop
  000000014090A959: 4C 8D 84 24 08 04  lea         r8,[rsp+0000000000000408h]
                    00 00
  000000014090A961: 48 8D 94 24 10 04  lea         rdx,[rsp+0000000000000410h]
                    00 00
  000000014090A969: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090A96E: FF 15 14 1D 50 01  call        qword ptr [0000000141E0C688h]
  000000014090A974: 48 89 84 24 B0 09  mov         qword ptr [rsp+00000000000009B0h],rax
                    00 00
  000000014090A97C: 48 8B 84 24 B0 09  mov         rax,qword ptr [rsp+00000000000009B0h]
                    00 00
  000000014090A984: 48 89 84 24 B8 09  mov         qword ptr [rsp+00000000000009B8h],rax
                    00 00
  000000014090A98C: 48 8B 94 24 B8 09  mov         rdx,qword ptr [rsp+00000000000009B8h]
                    00 00
  000000014090A994: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090A999: FF 15 D9 1B 50 01  call        qword ptr [0000000141E0C578h]
  000000014090A99F: 90                 nop
  000000014090A9A0: 48 8D 8C 24 10 04  lea         rcx,[rsp+0000000000000410h]
                    00 00
  000000014090A9A8: FF 15 02 1D 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090A9AE: 90                 nop
  000000014090A9AF: 48 8D 8C 24 08 04  lea         rcx,[rsp+0000000000000408h]
                    00 00
  000000014090A9B7: FF 15 23 B0 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090A9BD: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  000000014090A9C2: 48 8D 94 24 18 04  lea         rdx,[rsp+0000000000000418h]
                    00 00
  000000014090A9CA: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090A9D2: FF 15 C8 1C 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090A9D8: 48 8D 8C 24 18 04  lea         rcx,[rsp+0000000000000418h]
                    00 00
  000000014090A9E0: FF 15 C2 1C 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090A9E6: 48 8D 15 F3 05 EB  lea         rdx,[00000001427BAFE0h]
                    01
  000000014090A9ED: 48 8D 8C 24 28 04  lea         rcx,[rsp+0000000000000428h]
                    00 00
  000000014090A9F5: FF 15 85 9A 4F 01  call        qword ptr [0000000141E04480h]
  000000014090A9FB: 90                 nop
  000000014090A9FC: 48 8D 94 24 20 04  lea         rdx,[rsp+0000000000000420h]
                    00 00
  000000014090AA04: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AA0C: E8 4F C6 00 00     call        0000000140917060
  000000014090AA11: 48 89 84 24 C0 09  mov         qword ptr [rsp+00000000000009C0h],rax
                    00 00
  000000014090AA19: 48 8B 84 24 C0 09  mov         rax,qword ptr [rsp+00000000000009C0h]
                    00 00
  000000014090AA21: 48 89 84 24 C8 09  mov         qword ptr [rsp+00000000000009C8h],rax
                    00 00
  000000014090AA29: 4C 8B 84 24 C8 09  mov         r8,qword ptr [rsp+00000000000009C8h]
                    00 00
  000000014090AA31: 48 8D 94 24 28 04  lea         rdx,[rsp+0000000000000428h]
                    00 00
  000000014090AA39: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090AA3E: FF 15 D4 1B 50 01  call        qword ptr [0000000141E0C618h]
  000000014090AA44: 90                 nop
  000000014090AA45: 48 8D 8C 24 20 04  lea         rcx,[rsp+0000000000000420h]
                    00 00
  000000014090AA4D: FF 15 8D AF 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AA53: 90                 nop
  000000014090AA54: 48 8D 8C 24 28 04  lea         rcx,[rsp+0000000000000428h]
                    00 00
  000000014090AA5C: FF 15 7E AF 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AA62: 48 8D 15 87 05 EB  lea         rdx,[00000001427BAFF0h]
                    01
  000000014090AA69: 48 8D 8C 24 30 04  lea         rcx,[rsp+0000000000000430h]
                    00 00
  000000014090AA71: FF 15 09 9A 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AA77: 90                 nop
  000000014090AA78: 4C 8D 84 24 30 04  lea         r8,[rsp+0000000000000430h]
                    00 00
  000000014090AA80: 48 8D 94 24 38 04  lea         rdx,[rsp+0000000000000438h]
                    00 00
  000000014090AA88: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090AA8D: FF 15 F5 1B 50 01  call        qword ptr [0000000141E0C688h]
  000000014090AA93: 48 89 84 24 D0 09  mov         qword ptr [rsp+00000000000009D0h],rax
                    00 00
  000000014090AA9B: 48 8B 84 24 D0 09  mov         rax,qword ptr [rsp+00000000000009D0h]
                    00 00
  000000014090AAA3: 48 89 84 24 D8 09  mov         qword ptr [rsp+00000000000009D8h],rax
                    00 00
  000000014090AAAB: 48 8B 94 24 D8 09  mov         rdx,qword ptr [rsp+00000000000009D8h]
                    00 00
  000000014090AAB3: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090AAB8: FF 15 BA 1A 50 01  call        qword ptr [0000000141E0C578h]
  000000014090AABE: 90                 nop
  000000014090AABF: 48 8D 8C 24 38 04  lea         rcx,[rsp+0000000000000438h]
                    00 00
  000000014090AAC7: FF 15 E3 1B 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090AACD: 90                 nop
  000000014090AACE: 48 8D 8C 24 30 04  lea         rcx,[rsp+0000000000000430h]
                    00 00
  000000014090AAD6: FF 15 04 AF 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AADC: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  000000014090AAE1: 48 8D 94 24 40 04  lea         rdx,[rsp+0000000000000440h]
                    00 00
  000000014090AAE9: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090AAF1: FF 15 A9 1B 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090AAF7: 48 8D 8C 24 40 04  lea         rcx,[rsp+0000000000000440h]
                    00 00
  000000014090AAFF: FF 15 A3 1B 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090AB05: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AB0D: E8 7E 1B 7B FF     call        00000001400BC690
  000000014090AB12: 0F B6 C0           movzx       eax,al
  000000014090AB15: 85 C0              test        eax,eax
  000000014090AB17: 74 11              je          000000014090AB2A
  000000014090AB19: 48 8D 05 E4 04 EB  lea         rax,[00000001427BB004h]
                    01
  000000014090AB20: 48 89 84 24 E0 09  mov         qword ptr [rsp+00000000000009E0h],rax
                    00 00
  000000014090AB28: EB 0F              jmp         000000014090AB39
  000000014090AB2A: 48 8D 05 DB 04 EB  lea         rax,[00000001427BB00Ch]
                    01
  000000014090AB31: 48 89 84 24 E0 09  mov         qword ptr [rsp+00000000000009E0h],rax
                    00 00
  000000014090AB39: 48 8B 94 24 E0 09  mov         rdx,qword ptr [rsp+00000000000009E0h]
                    00 00
  000000014090AB41: 48 8D 8C 24 48 04  lea         rcx,[rsp+0000000000000448h]
                    00 00
  000000014090AB49: FF 15 31 99 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AB4F: 90                 nop
  000000014090AB50: 48 8D 15 BD 04 EB  lea         rdx,[00000001427BB014h]
                    01
  000000014090AB57: 48 8D 8C 24 50 04  lea         rcx,[rsp+0000000000000450h]
                    00 00
  000000014090AB5F: FF 15 1B 99 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AB65: 90                 nop
  000000014090AB66: 4C 8D 84 24 48 04  lea         r8,[rsp+0000000000000448h]
                    00 00
  000000014090AB6E: 48 8D 94 24 50 04  lea         rdx,[rsp+0000000000000450h]
                    00 00
  000000014090AB76: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090AB7B: FF 15 97 1A 50 01  call        qword ptr [0000000141E0C618h]
  000000014090AB81: 90                 nop
  000000014090AB82: 48 8D 8C 24 50 04  lea         rcx,[rsp+0000000000000450h]
                    00 00
  000000014090AB8A: FF 15 50 AE 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AB90: 90                 nop
  000000014090AB91: 48 8D 8C 24 48 04  lea         rcx,[rsp+0000000000000448h]
                    00 00
  000000014090AB99: FF 15 41 AE 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AB9F: 48 8D 15 7A 04 EB  lea         rdx,[00000001427BB020h]
                    01
  000000014090ABA6: 48 8D 8C 24 58 04  lea         rcx,[rsp+0000000000000458h]
                    00 00
  000000014090ABAE: FF 15 CC 98 4F 01  call        qword ptr [0000000141E04480h]
  000000014090ABB4: 90                 nop
  000000014090ABB5: 4C 8D 84 24 58 04  lea         r8,[rsp+0000000000000458h]
                    00 00
  000000014090ABBD: 48 8D 94 24 60 04  lea         rdx,[rsp+0000000000000460h]
                    00 00
  000000014090ABC5: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090ABCA: FF 15 B8 1A 50 01  call        qword ptr [0000000141E0C688h]
  000000014090ABD0: 48 89 84 24 E8 09  mov         qword ptr [rsp+00000000000009E8h],rax
                    00 00
  000000014090ABD8: 48 8B 84 24 E8 09  mov         rax,qword ptr [rsp+00000000000009E8h]
                    00 00
  000000014090ABE0: 48 89 84 24 F0 09  mov         qword ptr [rsp+00000000000009F0h],rax
                    00 00
  000000014090ABE8: 48 8B 94 24 F0 09  mov         rdx,qword ptr [rsp+00000000000009F0h]
                    00 00
  000000014090ABF0: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090ABF5: FF 15 7D 19 50 01  call        qword ptr [0000000141E0C578h]
  000000014090ABFB: 90                 nop
  000000014090ABFC: 48 8D 8C 24 60 04  lea         rcx,[rsp+0000000000000460h]
                    00 00
  000000014090AC04: FF 15 A6 1A 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090AC0A: 90                 nop
  000000014090AC0B: 48 8D 8C 24 58 04  lea         rcx,[rsp+0000000000000458h]
                    00 00
  000000014090AC13: FF 15 C7 AD 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AC19: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  000000014090AC1E: 48 8D 94 24 68 04  lea         rdx,[rsp+0000000000000468h]
                    00 00
  000000014090AC26: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090AC2E: FF 15 6C 1A 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090AC34: 48 8D 8C 24 68 04  lea         rcx,[rsp+0000000000000468h]
                    00 00
  000000014090AC3C: FF 15 66 1A 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090AC42: 48 8D 15 E3 03 EB  lea         rdx,[00000001427BB02Ch]
                    01
  000000014090AC49: 48 8D 8C 24 78 04  lea         rcx,[rsp+0000000000000478h]
                    00 00
  000000014090AC51: FF 15 29 98 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AC57: 90                 nop
  000000014090AC58: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AC60: E8 EB 58 FD FF     call        00000001408E0550
  000000014090AC65: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090AC6B: 8B D0              mov         edx,eax
  000000014090AC6D: 48 8D 8C 24 70 04  lea         rcx,[rsp+0000000000000470h]
                    00 00
  000000014090AC75: FF 15 45 91 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090AC7B: 48 89 84 24 F8 09  mov         qword ptr [rsp+00000000000009F8h],rax
                    00 00
  000000014090AC83: 48 8B 84 24 F8 09  mov         rax,qword ptr [rsp+00000000000009F8h]
                    00 00
  000000014090AC8B: 48 89 84 24 00 0A  mov         qword ptr [rsp+0000000000000A00h],rax
                    00 00
  000000014090AC93: 4C 8B 84 24 00 0A  mov         r8,qword ptr [rsp+0000000000000A00h]
                    00 00
  000000014090AC9B: 48 8D 94 24 78 04  lea         rdx,[rsp+0000000000000478h]
                    00 00
  000000014090ACA3: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090ACA8: FF 15 6A 19 50 01  call        qword ptr [0000000141E0C618h]
  000000014090ACAE: 90                 nop
  000000014090ACAF: 48 8D 8C 24 70 04  lea         rcx,[rsp+0000000000000470h]
                    00 00
  000000014090ACB7: FF 15 23 AD 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090ACBD: 90                 nop
  000000014090ACBE: 48 8D 8C 24 78 04  lea         rcx,[rsp+0000000000000478h]
                    00 00
  000000014090ACC6: FF 15 14 AD 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090ACCC: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090ACD4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090ACD7: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090ACDF: FF 90 C0 00 00 00  call        qword ptr [rax+00000000000000C0h]
  000000014090ACE5: 89 84 24 C4 00 00  mov         dword ptr [rsp+00000000000000C4h],eax
                    00
  000000014090ACEC: 83 BC 24 C4 00 00  cmp         dword ptr [rsp+00000000000000C4h],0
                    00 00
  000000014090ACF4: 0F 8E 99 01 00 00  jle         000000014090AE93
  000000014090ACFA: 48 8D 15 37 03 EB  lea         rdx,[00000001427BB038h]
                    01
  000000014090AD01: 48 8D 8C 24 80 04  lea         rcx,[rsp+0000000000000480h]
                    00 00
  000000014090AD09: FF 15 71 97 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AD0F: 90                 nop
  000000014090AD10: 4C 8D 84 24 80 04  lea         r8,[rsp+0000000000000480h]
                    00 00
  000000014090AD18: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  000000014090AD20: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090AD25: FF 15 5D 19 50 01  call        qword ptr [0000000141E0C688h]
  000000014090AD2B: 90                 nop
  000000014090AD2C: 48 8D 8C 24 80 04  lea         rcx,[rsp+0000000000000480h]
                    00 00
  000000014090AD34: FF 15 A6 AC 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AD3A: 4C 8D 84 24 C8 00  lea         r8,[rsp+00000000000000C8h]
                    00 00
  000000014090AD42: 48 8D 94 24 88 04  lea         rdx,[rsp+0000000000000488h]
                    00 00
  000000014090AD4A: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090AD52: FF 15 48 19 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090AD58: 48 8D 8C 24 88 04  lea         rcx,[rsp+0000000000000488h]
                    00 00
  000000014090AD60: FF 15 42 19 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090AD66: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  000000014090AD6E: EB 0B              jmp         000000014090AD7B
  000000014090AD70: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  000000014090AD74: 83 C0 01           add         eax,1
  000000014090AD77: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  000000014090AD7B: 8B 84 24 C4 00 00  mov         eax,dword ptr [rsp+00000000000000C4h]
                    00
  000000014090AD82: 39 44 24 68        cmp         dword ptr [rsp+68h],eax
  000000014090AD86: 0F 8D F9 00 00 00  jge         000000014090AE85
  000000014090AD8C: 48 8D 15 B9 02 EB  lea         rdx,[00000001427BB04Ch]
                    01
  000000014090AD93: 48 8D 8C 24 90 04  lea         rcx,[rsp+0000000000000490h]
                    00 00
  000000014090AD9B: FF 15 DF 96 4F 01  call        qword ptr [0000000141E04480h]
  000000014090ADA1: 90                 nop
  000000014090ADA2: 4C 8D 84 24 90 04  lea         r8,[rsp+0000000000000490h]
                    00 00
  000000014090ADAA: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000014090ADB2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090ADB7: FF 15 CB 18 50 01  call        qword ptr [0000000141E0C688h]
  000000014090ADBD: 90                 nop
  000000014090ADBE: 48 8D 8C 24 90 04  lea         rcx,[rsp+0000000000000490h]
                    00 00
  000000014090ADC6: FF 15 14 AC 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090ADCC: 4C 8D 84 24 D8 00  lea         r8,[rsp+00000000000000D8h]
                    00 00
  000000014090ADD4: 48 8D 94 24 98 04  lea         rdx,[rsp+0000000000000498h]
                    00 00
  000000014090ADDC: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014090ADE4: FF 15 B6 18 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090ADEA: 48 8D 8C 24 98 04  lea         rcx,[rsp+0000000000000498h]
                    00 00
  000000014090ADF2: FF 15 B0 18 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090ADF8: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AE00: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090AE03: 44 8B 44 24 68     mov         r8d,dword ptr [rsp+68h]
  000000014090AE08: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  000000014090AE10: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AE18: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  000000014090AE1E: 90                 nop
  000000014090AE1F: 48 8D 15 2E 02 EB  lea         rdx,[00000001427BB054h]
                    01
  000000014090AE26: 48 8D 8C 24 A0 04  lea         rcx,[rsp+00000000000004A0h]
                    00 00
  000000014090AE2E: FF 15 4C 96 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AE34: 90                 nop
  000000014090AE35: 4C 8D 84 24 D0 00  lea         r8,[rsp+00000000000000D0h]
                    00 00
  000000014090AE3D: 48 8D 94 24 A0 04  lea         rdx,[rsp+00000000000004A0h]
                    00 00
  000000014090AE45: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000014090AE4D: FF 15 C5 17 50 01  call        qword ptr [0000000141E0C618h]
  000000014090AE53: 90                 nop
  000000014090AE54: 48 8D 8C 24 A0 04  lea         rcx,[rsp+00000000000004A0h]
                    00 00
  000000014090AE5C: FF 15 7E AB 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AE62: 90                 nop
  000000014090AE63: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  000000014090AE6B: FF 15 6F AB 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AE71: 90                 nop
  000000014090AE72: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000014090AE7A: FF 15 30 18 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090AE80: E9 EB FE FF FF     jmp         000000014090AD70
  000000014090AE85: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014090AE8D: FF 15 1D 18 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090AE93: 48 8D 15 C6 01 EB  lea         rdx,[00000001427BB060h]
                    01
  000000014090AE9A: 48 8D 8C 24 A8 04  lea         rcx,[rsp+00000000000004A8h]
                    00 00
  000000014090AEA2: FF 15 D8 95 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AEA8: 90                 nop
  000000014090AEA9: 4C 8D 84 24 A8 04  lea         r8,[rsp+00000000000004A8h]
                    00 00
  000000014090AEB1: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000014090AEB6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090AEBB: FF 15 C7 17 50 01  call        qword ptr [0000000141E0C688h]
  000000014090AEC1: 90                 nop
  000000014090AEC2: 48 8D 8C 24 A8 04  lea         rcx,[rsp+00000000000004A8h]
                    00 00
  000000014090AECA: FF 15 10 AB 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AED0: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  000000014090AED5: 48 8D 94 24 B0 04  lea         rdx,[rsp+00000000000004B0h]
                    00 00
  000000014090AEDD: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090AEE5: FF 15 B5 17 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090AEEB: 48 8D 8C 24 B0 04  lea         rcx,[rsp+00000000000004B0h]
                    00 00
  000000014090AEF3: FF 15 AF 17 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090AEF9: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AF01: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090AF04: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AF0C: FF 90 78 01 00 00  call        qword ptr [rax+0000000000000178h]
  000000014090AF12: 89 84 24 C0 00 00  mov         dword ptr [rsp+00000000000000C0h],eax
                    00
  000000014090AF19: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  000000014090AF21: EB 0B              jmp         000000014090AF2E
  000000014090AF23: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  000000014090AF27: 83 C0 01           add         eax,1
  000000014090AF2A: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  000000014090AF2E: 8B 84 24 C0 00 00  mov         eax,dword ptr [rsp+00000000000000C0h]
                    00
  000000014090AF35: 39 44 24 68        cmp         dword ptr [rsp+68h],eax
  000000014090AF39: 0F 8D 38 27 00 00  jge         000000014090D677
  000000014090AF3F: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AF47: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090AF4A: 44 8B 44 24 68     mov         r8d,dword ptr [rsp+68h]
  000000014090AF4F: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  000000014090AF57: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AF5F: FF 90 90 01 00 00  call        qword ptr [rax+0000000000000190h]
  000000014090AF65: 90                 nop
  000000014090AF66: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AF6E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090AF71: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090AF75: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090AF7D: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  000000014090AF83: 89 84 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],eax
                    00
  000000014090AF8A: 48 8D 15 DF 00 EB  lea         rdx,[00000001427BB070h]
                    01
  000000014090AF91: 48 8D 8C 24 B8 04  lea         rcx,[rsp+00000000000004B8h]
                    00 00
  000000014090AF99: FF 15 E1 94 4F 01  call        qword ptr [0000000141E04480h]
  000000014090AF9F: 90                 nop
  000000014090AFA0: 4C 8D 84 24 B8 04  lea         r8,[rsp+00000000000004B8h]
                    00 00
  000000014090AFA8: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  000000014090AFB0: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090AFB5: FF 15 CD 16 50 01  call        qword ptr [0000000141E0C688h]
  000000014090AFBB: 90                 nop
  000000014090AFBC: 48 8D 8C 24 B8 04  lea         rcx,[rsp+00000000000004B8h]
                    00 00
  000000014090AFC4: FF 15 16 AA 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090AFCA: 4C 8D 84 24 F8 00  lea         r8,[rsp+00000000000000F8h]
                    00 00
  000000014090AFD2: 48 8D 94 24 C0 04  lea         rdx,[rsp+00000000000004C0h]
                    00 00
  000000014090AFDA: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014090AFDF: FF 15 BB 16 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090AFE5: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  000000014090AFED: FF 15 B5 16 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090AFF3: 48 8D 15 82 00 EB  lea         rdx,[00000001427BB07Ch]
                    01
  000000014090AFFA: 48 8D 8C 24 C8 04  lea         rcx,[rsp+00000000000004C8h]
                    00 00
  000000014090B002: FF 15 78 94 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B008: 90                 nop
  000000014090B009: 4C 8D 84 24 08 01  lea         r8,[rsp+0000000000000108h]
                    00 00
  000000014090B011: 48 8D 94 24 C8 04  lea         rdx,[rsp+00000000000004C8h]
                    00 00
  000000014090B019: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090B021: FF 15 F1 15 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B027: 90                 nop
  000000014090B028: 48 8D 8C 24 C8 04  lea         rcx,[rsp+00000000000004C8h]
                    00 00
  000000014090B030: FF 15 AA A9 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B036: 48 8D 15 4B 00 EB  lea         rdx,[00000001427BB088h]
                    01
  000000014090B03D: 48 8D 8C 24 D8 04  lea         rcx,[rsp+00000000000004D8h]
                    00 00
  000000014090B045: FF 15 35 94 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B04B: 90                 nop
  000000014090B04C: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090B052: 8B 94 24 E0 00 00  mov         edx,dword ptr [rsp+00000000000000E0h]
                    00
  000000014090B059: 48 8D 8C 24 D0 04  lea         rcx,[rsp+00000000000004D0h]
                    00 00
  000000014090B061: FF 15 59 8D 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090B067: 48 89 84 24 08 0A  mov         qword ptr [rsp+0000000000000A08h],rax
                    00 00
  000000014090B06F: 48 8B 84 24 08 0A  mov         rax,qword ptr [rsp+0000000000000A08h]
                    00 00
  000000014090B077: 48 89 84 24 10 0A  mov         qword ptr [rsp+0000000000000A10h],rax
                    00 00
  000000014090B07F: 4C 8B 84 24 10 0A  mov         r8,qword ptr [rsp+0000000000000A10h]
                    00 00
  000000014090B087: 48 8D 94 24 D8 04  lea         rdx,[rsp+00000000000004D8h]
                    00 00
  000000014090B08F: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090B097: FF 15 7B 15 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B09D: 90                 nop
  000000014090B09E: 48 8D 8C 24 D0 04  lea         rcx,[rsp+00000000000004D0h]
                    00 00
  000000014090B0A6: FF 15 34 A9 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B0AC: 90                 nop
  000000014090B0AD: 48 8D 8C 24 D8 04  lea         rcx,[rsp+00000000000004D8h]
                    00 00
  000000014090B0B5: FF 15 25 A9 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B0BB: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090B0C3: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090B0C6: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090B0CA: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090B0D2: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000014090B0D8: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  000000014090B0E0: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090B0E8: E8 93 58 7F FF     call        0000000140100980
  000000014090B0ED: 89 84 24 F0 00 00  mov         dword ptr [rsp+00000000000000F0h],eax
                    00
  000000014090B0F4: 48 8D 15 99 FF EA  lea         rdx,[00000001427BB094h]
                    01
  000000014090B0FB: 48 8D 8C 24 E8 04  lea         rcx,[rsp+00000000000004E8h]
                    00 00
  000000014090B103: FF 15 77 93 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B109: 90                 nop
  000000014090B10A: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090B110: 8B 94 24 F0 00 00  mov         edx,dword ptr [rsp+00000000000000F0h]
                    00
  000000014090B117: 48 8D 8C 24 E0 04  lea         rcx,[rsp+00000000000004E0h]
                    00 00
  000000014090B11F: FF 15 9B 8C 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090B125: 48 89 84 24 18 0A  mov         qword ptr [rsp+0000000000000A18h],rax
                    00 00
  000000014090B12D: 48 8B 84 24 18 0A  mov         rax,qword ptr [rsp+0000000000000A18h]
                    00 00
  000000014090B135: 48 89 84 24 20 0A  mov         qword ptr [rsp+0000000000000A20h],rax
                    00 00
  000000014090B13D: 4C 8B 84 24 20 0A  mov         r8,qword ptr [rsp+0000000000000A20h]
                    00 00
  000000014090B145: 48 8D 94 24 E8 04  lea         rdx,[rsp+00000000000004E8h]
                    00 00
  000000014090B14D: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090B155: FF 15 BD 14 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B15B: 90                 nop
  000000014090B15C: 48 8D 8C 24 E0 04  lea         rcx,[rsp+00000000000004E0h]
                    00 00
  000000014090B164: FF 15 76 A8 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B16A: 90                 nop
  000000014090B16B: 48 8D 8C 24 E8 04  lea         rcx,[rsp+00000000000004E8h]
                    00 00
  000000014090B173: FF 15 67 A8 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B179: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090B181: E8 CA 2D 30 00     call        0000000140C0DF50
  000000014090B186: 89 84 24 F4 00 00  mov         dword ptr [rsp+00000000000000F4h],eax
                    00
  000000014090B18D: 48 8D 15 0C FF EA  lea         rdx,[00000001427BB0A0h]
                    01
  000000014090B194: 48 8D 8C 24 F8 04  lea         rcx,[rsp+00000000000004F8h]
                    00 00
  000000014090B19C: FF 15 DE 92 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B1A2: 90                 nop
  000000014090B1A3: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090B1A9: 8B 94 24 F4 00 00  mov         edx,dword ptr [rsp+00000000000000F4h]
                    00
  000000014090B1B0: 48 8D 8C 24 F0 04  lea         rcx,[rsp+00000000000004F0h]
                    00 00
  000000014090B1B8: FF 15 02 8C 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090B1BE: 48 89 84 24 28 0A  mov         qword ptr [rsp+0000000000000A28h],rax
                    00 00
  000000014090B1C6: 48 8B 84 24 28 0A  mov         rax,qword ptr [rsp+0000000000000A28h]
                    00 00
  000000014090B1CE: 48 89 84 24 30 0A  mov         qword ptr [rsp+0000000000000A30h],rax
                    00 00
  000000014090B1D6: 4C 8B 84 24 30 0A  mov         r8,qword ptr [rsp+0000000000000A30h]
                    00 00
  000000014090B1DE: 48 8D 94 24 F8 04  lea         rdx,[rsp+00000000000004F8h]
                    00 00
  000000014090B1E6: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090B1EE: FF 15 24 14 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B1F4: 90                 nop
  000000014090B1F5: 48 8D 8C 24 F0 04  lea         rcx,[rsp+00000000000004F0h]
                    00 00
  000000014090B1FD: FF 15 DD A7 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B203: 90                 nop
  000000014090B204: 48 8D 8C 24 F8 04  lea         rcx,[rsp+00000000000004F8h]
                    00 00
  000000014090B20C: FF 15 CE A7 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B212: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090B21A: E8 91 14 8C FF     call        00000001401CC6B0
  000000014090B21F: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  000000014090B227: 48 83 BC 24 00 01  cmp         qword ptr [rsp+0000000000000100h],0
                    00 00 00
  000000014090B230: 74 7F              je          000000014090B2B1
  000000014090B232: 48 8D 15 73 FE EA  lea         rdx,[00000001427BB0ACh]
                    01
  000000014090B239: 48 8D 8C 24 08 05  lea         rcx,[rsp+0000000000000508h]
                    00 00
  000000014090B241: FF 15 39 92 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B247: 90                 nop
  000000014090B248: 48 8D 94 24 00 05  lea         rdx,[rsp+0000000000000500h]
                    00 00
  000000014090B250: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  000000014090B258: E8 63 95 9F FF     call        00000001403047C0
  000000014090B25D: 48 89 84 24 38 0A  mov         qword ptr [rsp+0000000000000A38h],rax
                    00 00
  000000014090B265: 48 8B 84 24 38 0A  mov         rax,qword ptr [rsp+0000000000000A38h]
                    00 00
  000000014090B26D: 48 89 84 24 40 0A  mov         qword ptr [rsp+0000000000000A40h],rax
                    00 00
  000000014090B275: 4C 8B 84 24 40 0A  mov         r8,qword ptr [rsp+0000000000000A40h]
                    00 00
  000000014090B27D: 48 8D 94 24 08 05  lea         rdx,[rsp+0000000000000508h]
                    00 00
  000000014090B285: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090B28D: FF 15 85 13 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B293: 90                 nop
  000000014090B294: 48 8D 8C 24 00 05  lea         rcx,[rsp+0000000000000500h]
                    00 00
  000000014090B29C: FF 15 3E A7 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B2A2: 90                 nop
  000000014090B2A3: 48 8D 8C 24 08 05  lea         rcx,[rsp+0000000000000508h]
                    00 00
  000000014090B2AB: FF 15 2F A7 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B2B1: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  000000014090B2B9: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090B2C1: E8 DA 3E 80 FF     call        000000014010F1A0
  000000014090B2C6: 90                 nop
  000000014090B2C7: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  000000014090B2CF: FF 15 F3 8C 4F 01  call        qword ptr [0000000141E03FC8h]
  000000014090B2D5: 0F B6 C0           movzx       eax,al
  000000014090B2D8: 85 C0              test        eax,eax
  000000014090B2DA: 0F 85 D2 00 00 00  jne         000000014090B3B2
  000000014090B2E0: 48 8D 15 D1 FD EA  lea         rdx,[00000001427BB0B8h]
                    01
  000000014090B2E7: 48 8D 8C 24 10 05  lea         rcx,[rsp+0000000000000510h]
                    00 00
  000000014090B2EF: FF 15 8B 91 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B2F5: 90                 nop
  000000014090B2F6: 4C 8D 84 24 10 05  lea         r8,[rsp+0000000000000510h]
                    00 00
  000000014090B2FE: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  000000014090B306: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090B30B: FF 15 77 13 50 01  call        qword ptr [0000000141E0C688h]
  000000014090B311: 90                 nop
  000000014090B312: 48 8D 8C 24 10 05  lea         rcx,[rsp+0000000000000510h]
                    00 00
  000000014090B31A: FF 15 C0 A6 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B320: 4C 8D 84 24 30 01  lea         r8,[rsp+0000000000000130h]
                    00 00
  000000014090B328: 48 8D 94 24 18 05  lea         rdx,[rsp+0000000000000518h]
                    00 00
  000000014090B330: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090B338: FF 15 62 13 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090B33E: 48 8D 8C 24 18 05  lea         rcx,[rsp+0000000000000518h]
                    00 00
  000000014090B346: FF 15 5C 13 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090B34C: 4C 8D 84 24 E8 00  lea         r8,[rsp+00000000000000E8h]
                    00 00
  000000014090B354: 48 8D 94 24 28 01  lea         rdx,[rsp+0000000000000128h]
                    00 00
  000000014090B35C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090B361: FF 15 C9 12 50 01  call        qword ptr [0000000141E0C630h]
  000000014090B367: 90                 nop
  000000014090B368: 4C 8D 84 24 28 01  lea         r8,[rsp+0000000000000128h]
                    00 00
  000000014090B370: 48 8D 94 24 20 05  lea         rdx,[rsp+0000000000000520h]
                    00 00
  000000014090B378: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  000000014090B380: FF 15 1A 13 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090B386: 48 8D 8C 24 20 05  lea         rcx,[rsp+0000000000000520h]
                    00 00
  000000014090B38E: FF 15 14 13 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090B394: 90                 nop
  000000014090B395: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  000000014090B39D: FF 15 7D 12 50 01  call        qword ptr [0000000141E0C620h]
  000000014090B3A3: 90                 nop
  000000014090B3A4: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  000000014090B3AC: FF 15 FE 12 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090B3B2: 48 8D 84 24 28 05  lea         rax,[rsp+0000000000000528h]
                    00 00
  000000014090B3BA: 48 89 84 24 30 05  mov         qword ptr [rsp+0000000000000530h],rax
                    00 00
  000000014090B3C2: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  000000014090B3CA: 48 8B 8C 24 30 05  mov         rcx,qword ptr [rsp+0000000000000530h]
                    00 00
  000000014090B3D2: FF 15 00 A6 4F 01  call        qword ptr [0000000141E059D8h]
  000000014090B3D8: 48 89 84 24 48 0A  mov         qword ptr [rsp+0000000000000A48h],rax
                    00 00
  000000014090B3E0: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090B3E8: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090B3EB: 48 8B 94 24 48 0A  mov         rdx,qword ptr [rsp+0000000000000A48h]
                    00 00
  000000014090B3F3: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090B3FB: FF 90 28 01 00 00  call        qword ptr [rax+0000000000000128h]
  000000014090B401: 89 84 24 10 01 00  mov         dword ptr [rsp+0000000000000110h],eax
                    00
  000000014090B408: C7 84 24 24 01 00  mov         dword ptr [rsp+0000000000000124h],0
                    00 00 00 00 00
  000000014090B413: EB 11              jmp         000000014090B426
  000000014090B415: 8B 84 24 24 01 00  mov         eax,dword ptr [rsp+0000000000000124h]
                    00
  000000014090B41C: 83 C0 01           add         eax,1
  000000014090B41F: 89 84 24 24 01 00  mov         dword ptr [rsp+0000000000000124h],eax
                    00
  000000014090B426: 8B 84 24 10 01 00  mov         eax,dword ptr [rsp+0000000000000110h]
                    00
  000000014090B42D: 39 84 24 24 01 00  cmp         dword ptr [rsp+0000000000000124h],eax
                    00
  000000014090B434: 0F 8D 1B 11 00 00  jge         000000014090C555
  000000014090B43A: 48 8D 15 87 FC EA  lea         rdx,[00000001427BB0C8h]
                    01
  000000014090B441: 48 8D 8C 24 38 05  lea         rcx,[rsp+0000000000000538h]
                    00 00
  000000014090B449: FF 15 31 90 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B44F: 90                 nop
  000000014090B450: 4C 8D 84 24 38 05  lea         r8,[rsp+0000000000000538h]
                    00 00
  000000014090B458: 48 8D 94 24 68 01  lea         rdx,[rsp+0000000000000168h]
                    00 00
  000000014090B460: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090B465: FF 15 1D 12 50 01  call        qword ptr [0000000141E0C688h]
  000000014090B46B: 90                 nop
  000000014090B46C: 48 8D 8C 24 38 05  lea         rcx,[rsp+0000000000000538h]
                    00 00
  000000014090B474: FF 15 66 A5 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B47A: 4C 8D 84 24 68 01  lea         r8,[rsp+0000000000000168h]
                    00 00
  000000014090B482: 48 8D 94 24 40 05  lea         rdx,[rsp+0000000000000540h]
                    00 00
  000000014090B48A: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090B492: FF 15 08 12 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090B498: 48 8D 8C 24 40 05  lea         rcx,[rsp+0000000000000540h]
                    00 00
  000000014090B4A0: FF 15 02 12 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090B4A6: 48 8D 84 24 48 05  lea         rax,[rsp+0000000000000548h]
                    00 00
  000000014090B4AE: 48 89 84 24 50 05  mov         qword ptr [rsp+0000000000000550h],rax
                    00 00
  000000014090B4B6: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  000000014090B4BE: 48 8B 8C 24 50 05  mov         rcx,qword ptr [rsp+0000000000000550h]
                    00 00
  000000014090B4C6: FF 15 0C A5 4F 01  call        qword ptr [0000000141E059D8h]
  000000014090B4CC: 48 89 84 24 50 0A  mov         qword ptr [rsp+0000000000000A50h],rax
                    00 00
  000000014090B4D4: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090B4DC: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090B4DF: 44 8B 84 24 24 01  mov         r8d,dword ptr [rsp+0000000000000124h]
                    00 00
  000000014090B4E7: 48 8B 94 24 50 0A  mov         rdx,qword ptr [rsp+0000000000000A50h]
                    00 00
  000000014090B4EF: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090B4F7: FF 90 38 01 00 00  call        qword ptr [rax+0000000000000138h]
  000000014090B4FD: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  000000014090B505: 48 8D 94 24 60 01  lea         rdx,[rsp+0000000000000160h]
                    00 00
  000000014090B50D: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B515: E8 16 64 01 00     call        0000000140921930
  000000014090B51A: 90                 nop
  000000014090B51B: 48 8D 15 AE FB EA  lea         rdx,[00000001427BB0D0h]
                    01
  000000014090B522: 48 8D 8C 24 58 05  lea         rcx,[rsp+0000000000000558h]
                    00 00
  000000014090B52A: FF 15 50 8F 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B530: 90                 nop
  000000014090B531: 4C 8D 84 24 60 01  lea         r8,[rsp+0000000000000160h]
                    00 00
  000000014090B539: 48 8D 94 24 58 05  lea         rdx,[rsp+0000000000000558h]
                    00 00
  000000014090B541: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B549: FF 15 C9 10 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B54F: 90                 nop
  000000014090B550: 48 8D 8C 24 58 05  lea         rcx,[rsp+0000000000000558h]
                    00 00
  000000014090B558: FF 15 82 A4 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B55E: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B566: E8 35 47 7F FF     call        00000001400FFCA0
  000000014090B56B: 89 84 24 70 01 00  mov         dword ptr [rsp+0000000000000170h],eax
                    00
  000000014090B572: 48 8D 15 5F FB EA  lea         rdx,[00000001427BB0D8h]
                    01
  000000014090B579: 48 8D 8C 24 68 05  lea         rcx,[rsp+0000000000000568h]
                    00 00
  000000014090B581: FF 15 F9 8E 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B587: 90                 nop
  000000014090B588: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090B58E: 8B 94 24 70 01 00  mov         edx,dword ptr [rsp+0000000000000170h]
                    00
  000000014090B595: 48 8D 8C 24 60 05  lea         rcx,[rsp+0000000000000560h]
                    00 00
  000000014090B59D: FF 15 1D 88 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090B5A3: 48 89 84 24 58 0A  mov         qword ptr [rsp+0000000000000A58h],rax
                    00 00
  000000014090B5AB: 48 8B 84 24 58 0A  mov         rax,qword ptr [rsp+0000000000000A58h]
                    00 00
  000000014090B5B3: 48 89 84 24 60 0A  mov         qword ptr [rsp+0000000000000A60h],rax
                    00 00
  000000014090B5BB: 4C 8B 84 24 60 0A  mov         r8,qword ptr [rsp+0000000000000A60h]
                    00 00
  000000014090B5C3: 48 8D 94 24 68 05  lea         rdx,[rsp+0000000000000568h]
                    00 00
  000000014090B5CB: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B5D3: FF 15 3F 10 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B5D9: 90                 nop
  000000014090B5DA: 48 8D 8C 24 60 05  lea         rcx,[rsp+0000000000000560h]
                    00 00
  000000014090B5E2: FF 15 F8 A3 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B5E8: 90                 nop
  000000014090B5E9: 48 8D 8C 24 68 05  lea         rcx,[rsp+0000000000000568h]
                    00 00
  000000014090B5F1: FF 15 E9 A3 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B5F7: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B5FF: E8 8C CB 7C FF     call        00000001400D8190
  000000014090B604: 0F B6 C0           movzx       eax,al
  000000014090B607: 85 C0              test        eax,eax
  000000014090B609: 74 6D              je          000000014090B678
  000000014090B60B: 48 8D 15 D6 FA EA  lea         rdx,[00000001427BB0E8h]
                    01
  000000014090B612: 48 8D 8C 24 70 05  lea         rcx,[rsp+0000000000000570h]
                    00 00
  000000014090B61A: FF 15 60 8E 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B620: 90                 nop
  000000014090B621: 48 8D 15 D0 FA EA  lea         rdx,[00000001427BB0F8h]
                    01
  000000014090B628: 48 8D 8C 24 78 05  lea         rcx,[rsp+0000000000000578h]
                    00 00
  000000014090B630: FF 15 4A 8E 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B636: 90                 nop
  000000014090B637: 4C 8D 84 24 70 05  lea         r8,[rsp+0000000000000570h]
                    00 00
  000000014090B63F: 48 8D 94 24 78 05  lea         rdx,[rsp+0000000000000578h]
                    00 00
  000000014090B647: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B64F: FF 15 C3 0F 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B655: 90                 nop
  000000014090B656: 48 8D 8C 24 78 05  lea         rcx,[rsp+0000000000000578h]
                    00 00
  000000014090B65E: FF 15 7C A3 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B664: 90                 nop
  000000014090B665: 48 8D 8C 24 70 05  lea         rcx,[rsp+0000000000000570h]
                    00 00
  000000014090B66D: FF 15 6D A3 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B673: E9 E6 00 00 00     jmp         000000014090B75E
  000000014090B678: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B680: E8 FB 45 7F FF     call        00000001400FFC80
  000000014090B685: 0F B6 C0           movzx       eax,al
  000000014090B688: 85 C0              test        eax,eax
  000000014090B68A: 74 6A              je          000000014090B6F6
  000000014090B68C: 48 8D 15 71 FA EA  lea         rdx,[00000001427BB104h]
                    01
  000000014090B693: 48 8D 8C 24 80 05  lea         rcx,[rsp+0000000000000580h]
                    00 00
  000000014090B69B: FF 15 DF 8D 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B6A1: 90                 nop
  000000014090B6A2: 48 8D 15 67 FA EA  lea         rdx,[00000001427BB110h]
                    01
  000000014090B6A9: 48 8D 8C 24 88 05  lea         rcx,[rsp+0000000000000588h]
                    00 00
  000000014090B6B1: FF 15 C9 8D 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B6B7: 90                 nop
  000000014090B6B8: 4C 8D 84 24 80 05  lea         r8,[rsp+0000000000000580h]
                    00 00
  000000014090B6C0: 48 8D 94 24 88 05  lea         rdx,[rsp+0000000000000588h]
                    00 00
  000000014090B6C8: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B6D0: FF 15 42 0F 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B6D6: 90                 nop
  000000014090B6D7: 48 8D 8C 24 88 05  lea         rcx,[rsp+0000000000000588h]
                    00 00
  000000014090B6DF: FF 15 FB A2 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B6E5: 90                 nop
  000000014090B6E6: 48 8D 8C 24 80 05  lea         rcx,[rsp+0000000000000580h]
                    00 00
  000000014090B6EE: FF 15 EC A2 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B6F4: EB 68              jmp         000000014090B75E
  000000014090B6F6: 48 8D 15 1F FA EA  lea         rdx,[00000001427BB11Ch]
                    01
  000000014090B6FD: 48 8D 8C 24 90 05  lea         rcx,[rsp+0000000000000590h]
                    00 00
  000000014090B705: FF 15 75 8D 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B70B: 90                 nop
  000000014090B70C: 48 8D 15 15 FA EA  lea         rdx,[00000001427BB128h]
                    01
  000000014090B713: 48 8D 8C 24 98 05  lea         rcx,[rsp+0000000000000598h]
                    00 00
  000000014090B71B: FF 15 5F 8D 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B721: 90                 nop
  000000014090B722: 4C 8D 84 24 90 05  lea         r8,[rsp+0000000000000590h]
                    00 00
  000000014090B72A: 48 8D 94 24 98 05  lea         rdx,[rsp+0000000000000598h]
                    00 00
  000000014090B732: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B73A: FF 15 D8 0E 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B740: 90                 nop
  000000014090B741: 48 8D 8C 24 98 05  lea         rcx,[rsp+0000000000000598h]
                    00 00
  000000014090B749: FF 15 91 A2 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B74F: 90                 nop
  000000014090B750: 48 8D 8C 24 90 05  lea         rcx,[rsp+0000000000000590h]
                    00 00
  000000014090B758: FF 15 82 A2 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B75E: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B766: E8 75 BF 49 00     call        0000000140DA76E0
  000000014090B76B: 89 84 24 40 01 00  mov         dword ptr [rsp+0000000000000140h],eax
                    00
  000000014090B772: 48 8D 15 BF F9 EA  lea         rdx,[00000001427BB138h]
                    01
  000000014090B779: 48 8D 8C 24 A8 05  lea         rcx,[rsp+00000000000005A8h]
                    00 00
  000000014090B781: FF 15 F9 8C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B787: 90                 nop
  000000014090B788: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090B78E: 8B 94 24 40 01 00  mov         edx,dword ptr [rsp+0000000000000140h]
                    00
  000000014090B795: 48 8D 8C 24 A0 05  lea         rcx,[rsp+00000000000005A0h]
                    00 00
  000000014090B79D: FF 15 1D 86 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090B7A3: 48 89 84 24 68 0A  mov         qword ptr [rsp+0000000000000A68h],rax
                    00 00
  000000014090B7AB: 48 8B 84 24 68 0A  mov         rax,qword ptr [rsp+0000000000000A68h]
                    00 00
  000000014090B7B3: 48 89 84 24 70 0A  mov         qword ptr [rsp+0000000000000A70h],rax
                    00 00
  000000014090B7BB: 4C 8B 84 24 70 0A  mov         r8,qword ptr [rsp+0000000000000A70h]
                    00 00
  000000014090B7C3: 48 8D 94 24 A8 05  lea         rdx,[rsp+00000000000005A8h]
                    00 00
  000000014090B7CB: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B7D3: FF 15 3F 0E 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B7D9: 90                 nop
  000000014090B7DA: 48 8D 8C 24 A0 05  lea         rcx,[rsp+00000000000005A0h]
                    00 00
  000000014090B7E2: FF 15 F8 A1 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B7E8: 90                 nop
  000000014090B7E9: 48 8D 8C 24 A8 05  lea         rcx,[rsp+00000000000005A8h]
                    00 00
  000000014090B7F1: FF 15 E9 A1 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B7F7: 48 8D 94 24 88 01  lea         rdx,[rsp+0000000000000188h]
                    00 00
  000000014090B7FF: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B807: E8 B4 8F 9F FF     call        00000001403047C0
  000000014090B80C: 90                 nop
  000000014090B80D: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  000000014090B815: FF 15 AD 87 4F 01  call        qword ptr [0000000141E03FC8h]
  000000014090B81B: 0F B6 C0           movzx       eax,al
  000000014090B81E: 85 C0              test        eax,eax
  000000014090B820: 75 44              jne         000000014090B866
  000000014090B822: 48 8D 15 1F F9 EA  lea         rdx,[00000001427BB148h]
                    01
  000000014090B829: 48 8D 8C 24 B0 05  lea         rcx,[rsp+00000000000005B0h]
                    00 00
  000000014090B831: FF 15 49 8C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B837: 90                 nop
  000000014090B838: 4C 8D 84 24 88 01  lea         r8,[rsp+0000000000000188h]
                    00 00
  000000014090B840: 48 8D 94 24 B0 05  lea         rdx,[rsp+00000000000005B0h]
                    00 00
  000000014090B848: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B850: FF 15 C2 0D 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B856: 90                 nop
  000000014090B857: 48 8D 8C 24 B0 05  lea         rcx,[rsp+00000000000005B0h]
                    00 00
  000000014090B85F: FF 15 7B A1 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B865: 90                 nop
  000000014090B866: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  000000014090B86E: FF 15 6C A1 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B874: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B87C: E8 FF 43 7F FF     call        00000001400FFC80
  000000014090B881: 0F B6 C0           movzx       eax,al
  000000014090B884: 85 C0              test        eax,eax
  000000014090B886: 0F 84 9E 00 00 00  je          000000014090B92A
  000000014090B88C: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B894: E8 47 5D 01 00     call        00000001409215E0
  000000014090B899: 89 84 24 90 01 00  mov         dword ptr [rsp+0000000000000190h],eax
                    00
  000000014090B8A0: 48 8D 15 B1 F8 EA  lea         rdx,[00000001427BB158h]
                    01
  000000014090B8A7: 48 8D 8C 24 C0 05  lea         rcx,[rsp+00000000000005C0h]
                    00 00
  000000014090B8AF: FF 15 CB 8B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B8B5: 90                 nop
  000000014090B8B6: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090B8BC: 8B 94 24 90 01 00  mov         edx,dword ptr [rsp+0000000000000190h]
                    00
  000000014090B8C3: 48 8D 8C 24 B8 05  lea         rcx,[rsp+00000000000005B8h]
                    00 00
  000000014090B8CB: FF 15 EF 84 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090B8D1: 48 89 84 24 78 0A  mov         qword ptr [rsp+0000000000000A78h],rax
                    00 00
  000000014090B8D9: 48 8B 84 24 78 0A  mov         rax,qword ptr [rsp+0000000000000A78h]
                    00 00
  000000014090B8E1: 48 89 84 24 80 0A  mov         qword ptr [rsp+0000000000000A80h],rax
                    00 00
  000000014090B8E9: 4C 8B 84 24 80 0A  mov         r8,qword ptr [rsp+0000000000000A80h]
                    00 00
  000000014090B8F1: 48 8D 94 24 C0 05  lea         rdx,[rsp+00000000000005C0h]
                    00 00
  000000014090B8F9: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B901: FF 15 11 0D 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B907: 90                 nop
  000000014090B908: 48 8D 8C 24 B8 05  lea         rcx,[rsp+00000000000005B8h]
                    00 00
  000000014090B910: FF 15 CA A0 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B916: 90                 nop
  000000014090B917: 48 8D 8C 24 C0 05  lea         rcx,[rsp+00000000000005C0h]
                    00 00
  000000014090B91F: FF 15 BB A0 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B925: E9 A0 00 00 00     jmp         000000014090B9CA
  000000014090B92A: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B932: E8 59 43 7F FF     call        00000001400FFC90
  000000014090B937: F2 0F 11 84 24 98  movsd       mmword ptr [rsp+0000000000000198h],xmm0
                    01 00 00
  000000014090B940: 48 8D 15 21 F8 EA  lea         rdx,[00000001427BB168h]
                    01
  000000014090B947: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  000000014090B94F: FF 15 2B 8B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090B955: 90                 nop
  000000014090B956: 41 B9 06 00 00 00  mov         r9d,6
  000000014090B95C: 41 B0 67           mov         r8b,67h
  000000014090B95F: F2 0F 10 8C 24 98  movsd       xmm1,mmword ptr [rsp+0000000000000198h]
                    01 00 00
  000000014090B968: 48 8D 8C 24 C8 05  lea         rcx,[rsp+00000000000005C8h]
                    00 00
  000000014090B970: FF 15 22 8E 4F 01  call        qword ptr [0000000141E04798h]
  000000014090B976: 48 89 84 24 88 0A  mov         qword ptr [rsp+0000000000000A88h],rax
                    00 00
  000000014090B97E: 48 8B 84 24 88 0A  mov         rax,qword ptr [rsp+0000000000000A88h]
                    00 00
  000000014090B986: 48 89 84 24 90 0A  mov         qword ptr [rsp+0000000000000A90h],rax
                    00 00
  000000014090B98E: 4C 8B 84 24 90 0A  mov         r8,qword ptr [rsp+0000000000000A90h]
                    00 00
  000000014090B996: 48 8D 94 24 D0 05  lea         rdx,[rsp+00000000000005D0h]
                    00 00
  000000014090B99E: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090B9A6: FF 15 6C 0C 50 01  call        qword ptr [0000000141E0C618h]
  000000014090B9AC: 90                 nop
  000000014090B9AD: 48 8D 8C 24 C8 05  lea         rcx,[rsp+00000000000005C8h]
                    00 00
  000000014090B9B5: FF 15 25 A0 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B9BB: 90                 nop
  000000014090B9BC: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  000000014090B9C4: FF 15 16 A0 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090B9CA: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B9D2: E8 B9 C7 7C FF     call        00000001400D8190
  000000014090B9D7: 0F B6 C0           movzx       eax,al
  000000014090B9DA: 85 C0              test        eax,eax
  000000014090B9DC: 0F 84 A1 00 00 00  je          000000014090BA83
  000000014090B9E2: 48 8D 94 24 A8 01  lea         rdx,[rsp+00000000000001A8h]
                    00 00
  000000014090B9EA: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090B9F2: E8 99 57 01 00     call        0000000140921190
  000000014090B9F7: 89 84 24 A0 01 00  mov         dword ptr [rsp+00000000000001A0h],eax
                    00
  000000014090B9FE: 48 8D 15 73 F7 EA  lea         rdx,[00000001427BB178h]
                    01
  000000014090BA05: 48 8D 8C 24 E0 05  lea         rcx,[rsp+00000000000005E0h]
                    00 00
  000000014090BA0D: FF 15 6D 8A 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BA13: 90                 nop
  000000014090BA14: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090BA1A: 8B 94 24 A0 01 00  mov         edx,dword ptr [rsp+00000000000001A0h]
                    00
  000000014090BA21: 48 8D 8C 24 D8 05  lea         rcx,[rsp+00000000000005D8h]
                    00 00
  000000014090BA29: FF 15 91 83 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090BA2F: 48 89 84 24 98 0A  mov         qword ptr [rsp+0000000000000A98h],rax
                    00 00
  000000014090BA37: 48 8B 84 24 98 0A  mov         rax,qword ptr [rsp+0000000000000A98h]
                    00 00
  000000014090BA3F: 48 89 84 24 A0 0A  mov         qword ptr [rsp+0000000000000AA0h],rax
                    00 00
  000000014090BA47: 4C 8B 84 24 A0 0A  mov         r8,qword ptr [rsp+0000000000000AA0h]
                    00 00
  000000014090BA4F: 48 8D 94 24 E0 05  lea         rdx,[rsp+00000000000005E0h]
                    00 00
  000000014090BA57: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090BA5F: FF 15 B3 0B 50 01  call        qword ptr [0000000141E0C618h]
  000000014090BA65: 90                 nop
  000000014090BA66: 48 8D 8C 24 D8 05  lea         rcx,[rsp+00000000000005D8h]
                    00 00
  000000014090BA6E: FF 15 6C 9F 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BA74: 90                 nop
  000000014090BA75: 48 8D 8C 24 E0 05  lea         rcx,[rsp+00000000000005E0h]
                    00 00
  000000014090BA7D: FF 15 5D 9F 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BA83: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090BA8B: E8 F0 41 7F FF     call        00000001400FFC80
  000000014090BA90: 0F B6 C0           movzx       eax,al
  000000014090BA93: 85 C0              test        eax,eax
  000000014090BA95: 0F 84 F2 01 00 00  je          000000014090BC8D
  000000014090BA9B: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  000000014090BAA3: E8 38 99 75 FF     call        00000001400653E0
  000000014090BAA8: 90                 nop
  000000014090BAA9: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  000000014090BAB1: E8 2A 3F 88 FF     call        000000014018F9E0
  000000014090BAB6: 90                 nop
  000000014090BAB7: 4C 8D 84 24 B0 01  lea         r8,[rsp+00000000000001B0h]
                    00 00
  000000014090BABF: 48 8D 94 24 B8 01  lea         rdx,[rsp+00000000000001B8h]
                    00 00
  000000014090BAC7: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090BACF: E8 6C 5A 01 00     call        0000000140921540
  000000014090BAD4: C7 84 24 C0 01 00  mov         dword ptr [rsp+00000000000001C0h],0
                    00 00 00 00 00
  000000014090BADF: EB 11              jmp         000000014090BAF2
  000000014090BAE1: 8B 84 24 C0 01 00  mov         eax,dword ptr [rsp+00000000000001C0h]
                    00
  000000014090BAE8: 83 C0 01           add         eax,1
  000000014090BAEB: 89 84 24 C0 01 00  mov         dword ptr [rsp+00000000000001C0h],eax
                    00
  000000014090BAF2: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  000000014090BAFA: E8 E1 8F 6F FF     call        0000000140004AE0
  000000014090BAFF: 39 84 24 C0 01 00  cmp         dword ptr [rsp+00000000000001C0h],eax
                    00
  000000014090BB06: 0F 8D 66 01 00 00  jge         000000014090BC72
  000000014090BB0C: 48 8D 15 75 F6 EA  lea         rdx,[00000001427BB188h]
                    01
  000000014090BB13: 48 8D 8C 24 E8 05  lea         rcx,[rsp+00000000000005E8h]
                    00 00
  000000014090BB1B: FF 15 5F 89 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BB21: 90                 nop
  000000014090BB22: 4C 8D 84 24 E8 05  lea         r8,[rsp+00000000000005E8h]
                    00 00
  000000014090BB2A: 48 8D 94 24 C8 01  lea         rdx,[rsp+00000000000001C8h]
                    00 00
  000000014090BB32: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BB37: FF 15 4B 0B 50 01  call        qword ptr [0000000141E0C688h]
  000000014090BB3D: 90                 nop
  000000014090BB3E: 48 8D 8C 24 E8 05  lea         rcx,[rsp+00000000000005E8h]
                    00 00
  000000014090BB46: FF 15 94 9E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BB4C: 4C 8D 84 24 C8 01  lea         r8,[rsp+00000000000001C8h]
                    00 00
  000000014090BB54: 48 8D 94 24 F0 05  lea         rdx,[rsp+00000000000005F0h]
                    00 00
  000000014090BB5C: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090BB64: FF 15 36 0B 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090BB6A: 48 8D 8C 24 F0 05  lea         rcx,[rsp+00000000000005F0h]
                    00 00
  000000014090BB72: FF 15 30 0B 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090BB78: 48 8D 15 15 F6 EA  lea         rdx,[00000001427BB194h]
                    01
  000000014090BB7F: 48 8D 8C 24 F8 05  lea         rcx,[rsp+00000000000005F8h]
                    00 00
  000000014090BB87: FF 15 F3 88 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BB8D: 90                 nop
  000000014090BB8E: 8B 94 24 C0 01 00  mov         edx,dword ptr [rsp+00000000000001C0h]
                    00
  000000014090BB95: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  000000014090BB9D: E8 9E 8F 6F FF     call        0000000140004B40
  000000014090BBA2: 4C 8B C0           mov         r8,rax
  000000014090BBA5: 48 8D 94 24 F8 05  lea         rdx,[rsp+00000000000005F8h]
                    00 00
  000000014090BBAD: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  000000014090BBB5: FF 15 5D 0A 50 01  call        qword ptr [0000000141E0C618h]
  000000014090BBBB: 90                 nop
  000000014090BBBC: 48 8D 8C 24 F8 05  lea         rcx,[rsp+00000000000005F8h]
                    00 00
  000000014090BBC4: FF 15 16 9E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BBCA: 48 8D 15 CB F5 EA  lea         rdx,[00000001427BB19Ch]
                    01
  000000014090BBD1: 48 8D 8C 24 08 06  lea         rcx,[rsp+0000000000000608h]
                    00 00
  000000014090BBD9: FF 15 A1 88 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BBDF: 90                 nop
  000000014090BBE0: 8B 94 24 C0 01 00  mov         edx,dword ptr [rsp+00000000000001C0h]
                    00
  000000014090BBE7: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  000000014090BBEF: E8 CC 94 71 FF     call        00000001400250C0
  000000014090BBF4: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090BBFA: 8B 10              mov         edx,dword ptr [rax]
  000000014090BBFC: 48 8D 8C 24 00 06  lea         rcx,[rsp+0000000000000600h]
                    00 00
  000000014090BC04: FF 15 B6 81 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090BC0A: 48 89 84 24 A8 0A  mov         qword ptr [rsp+0000000000000AA8h],rax
                    00 00
  000000014090BC12: 48 8B 84 24 A8 0A  mov         rax,qword ptr [rsp+0000000000000AA8h]
                    00 00
  000000014090BC1A: 48 89 84 24 B0 0A  mov         qword ptr [rsp+0000000000000AB0h],rax
                    00 00
  000000014090BC22: 4C 8B 84 24 B0 0A  mov         r8,qword ptr [rsp+0000000000000AB0h]
                    00 00
  000000014090BC2A: 48 8D 94 24 08 06  lea         rdx,[rsp+0000000000000608h]
                    00 00
  000000014090BC32: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  000000014090BC3A: FF 15 D8 09 50 01  call        qword ptr [0000000141E0C618h]
  000000014090BC40: 90                 nop
  000000014090BC41: 48 8D 8C 24 00 06  lea         rcx,[rsp+0000000000000600h]
                    00 00
  000000014090BC49: FF 15 91 9D 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BC4F: 90                 nop
  000000014090BC50: 48 8D 8C 24 08 06  lea         rcx,[rsp+0000000000000608h]
                    00 00
  000000014090BC58: FF 15 82 9D 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BC5E: 90                 nop
  000000014090BC5F: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  000000014090BC67: FF 15 43 0A 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090BC6D: E9 6F FE FF FF     jmp         000000014090BAE1
  000000014090BC72: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  000000014090BC7A: E8 21 C5 8F FF     call        00000001402081A0
  000000014090BC7F: 90                 nop
  000000014090BC80: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  000000014090BC88: E8 43 8C 6F FF     call        00000001400048D0
  000000014090BC8D: 48 8D 94 24 38 01  lea         rdx,[rsp+0000000000000138h]
                    00 00
  000000014090BC95: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090BC9D: E8 4E 98 75 FF     call        00000001400654F0
  000000014090BCA2: 90                 nop
  000000014090BCA3: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  000000014090BCAB: FF 15 17 83 4F 01  call        qword ptr [0000000141E03FC8h]
  000000014090BCB1: 0F B6 C0           movzx       eax,al
  000000014090BCB4: 85 C0              test        eax,eax
  000000014090BCB6: 0F 85 4D 01 00 00  jne         000000014090BE09
  000000014090BCBC: 48 8D 15 E5 F4 EA  lea         rdx,[00000001427BB1A8h]
                    01
  000000014090BCC3: 48 8D 8C 24 10 06  lea         rcx,[rsp+0000000000000610h]
                    00 00
  000000014090BCCB: FF 15 AF 87 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BCD1: 90                 nop
  000000014090BCD2: 4C 8D 84 24 10 06  lea         r8,[rsp+0000000000000610h]
                    00 00
  000000014090BCDA: 48 8D 94 24 D8 01  lea         rdx,[rsp+00000000000001D8h]
                    00 00
  000000014090BCE2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BCE7: FF 15 9B 09 50 01  call        qword ptr [0000000141E0C688h]
  000000014090BCED: 90                 nop
  000000014090BCEE: 48 8D 8C 24 10 06  lea         rcx,[rsp+0000000000000610h]
                    00 00
  000000014090BCF6: FF 15 E4 9C 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BCFC: 4C 8D 84 24 D8 01  lea         r8,[rsp+00000000000001D8h]
                    00 00
  000000014090BD04: 48 8D 94 24 18 06  lea         rdx,[rsp+0000000000000618h]
                    00 00
  000000014090BD0C: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090BD14: FF 15 86 09 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090BD1A: 48 8D 8C 24 18 06  lea         rcx,[rsp+0000000000000618h]
                    00 00
  000000014090BD22: FF 15 80 09 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090BD28: 48 8D 15 85 F4 EA  lea         rdx,[00000001427BB1B4h]
                    01
  000000014090BD2F: 48 8D 8C 24 20 06  lea         rcx,[rsp+0000000000000620h]
                    00 00
  000000014090BD37: FF 15 43 87 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BD3D: 90                 nop
  000000014090BD3E: 4C 8D 84 24 20 06  lea         r8,[rsp+0000000000000620h]
                    00 00
  000000014090BD46: 48 8D 94 24 D0 01  lea         rdx,[rsp+00000000000001D0h]
                    00 00
  000000014090BD4E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BD53: FF 15 2F 09 50 01  call        qword ptr [0000000141E0C688h]
  000000014090BD59: 90                 nop
  000000014090BD5A: 48 8D 8C 24 20 06  lea         rcx,[rsp+0000000000000620h]
                    00 00
  000000014090BD62: FF 15 78 9C 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BD68: 4C 8D 84 24 D0 01  lea         r8,[rsp+00000000000001D0h]
                    00 00
  000000014090BD70: 48 8D 94 24 28 06  lea         rdx,[rsp+0000000000000628h]
                    00 00
  000000014090BD78: 48 8D 8C 24 D8 01  lea         rcx,[rsp+00000000000001D8h]
                    00 00
  000000014090BD80: FF 15 1A 09 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090BD86: 48 8D 8C 24 28 06  lea         rcx,[rsp+0000000000000628h]
                    00 00
  000000014090BD8E: FF 15 14 09 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090BD94: 4C 8D 84 24 38 01  lea         r8,[rsp+0000000000000138h]
                    00 00
  000000014090BD9C: 48 8D 94 24 E0 01  lea         rdx,[rsp+00000000000001E0h]
                    00 00
  000000014090BDA4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BDA9: FF 15 81 08 50 01  call        qword ptr [0000000141E0C630h]
  000000014090BDAF: 90                 nop
  000000014090BDB0: 4C 8D 84 24 E0 01  lea         r8,[rsp+00000000000001E0h]
                    00 00
  000000014090BDB8: 48 8D 94 24 30 06  lea         rdx,[rsp+0000000000000630h]
                    00 00
  000000014090BDC0: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  000000014090BDC8: FF 15 D2 08 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090BDCE: 48 8D 8C 24 30 06  lea         rcx,[rsp+0000000000000630h]
                    00 00
  000000014090BDD6: FF 15 CC 08 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090BDDC: 90                 nop
  000000014090BDDD: 48 8D 8C 24 E0 01  lea         rcx,[rsp+00000000000001E0h]
                    00 00
  000000014090BDE5: FF 15 35 08 50 01  call        qword ptr [0000000141E0C620h]
  000000014090BDEB: 90                 nop
  000000014090BDEC: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  000000014090BDF4: FF 15 B6 08 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090BDFA: 90                 nop
  000000014090BDFB: 48 8D 8C 24 D8 01  lea         rcx,[rsp+00000000000001D8h]
                    00 00
  000000014090BE03: FF 15 A7 08 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090BE09: 48 8D 94 24 58 01  lea         rdx,[rsp+0000000000000158h]
                    00 00
  000000014090BE11: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090BE19: E8 A2 C1 7C FF     call        00000001400D7FC0
  000000014090BE1E: 90                 nop
  000000014090BE1F: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  000000014090BE27: FF 15 9B 81 4F 01  call        qword ptr [0000000141E03FC8h]
  000000014090BE2D: 0F B6 C0           movzx       eax,al
  000000014090BE30: 85 C0              test        eax,eax
  000000014090BE32: 0F 85 4D 01 00 00  jne         000000014090BF85
  000000014090BE38: 48 8D 15 81 F3 EA  lea         rdx,[00000001427BB1C0h]
                    01
  000000014090BE3F: 48 8D 8C 24 38 06  lea         rcx,[rsp+0000000000000638h]
                    00 00
  000000014090BE47: FF 15 33 86 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BE4D: 90                 nop
  000000014090BE4E: 4C 8D 84 24 38 06  lea         r8,[rsp+0000000000000638h]
                    00 00
  000000014090BE56: 48 8D 94 24 F0 01  lea         rdx,[rsp+00000000000001F0h]
                    00 00
  000000014090BE5E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BE63: FF 15 1F 08 50 01  call        qword ptr [0000000141E0C688h]
  000000014090BE69: 90                 nop
  000000014090BE6A: 48 8D 8C 24 38 06  lea         rcx,[rsp+0000000000000638h]
                    00 00
  000000014090BE72: FF 15 68 9B 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BE78: 4C 8D 84 24 F0 01  lea         r8,[rsp+00000000000001F0h]
                    00 00
  000000014090BE80: 48 8D 94 24 40 06  lea         rdx,[rsp+0000000000000640h]
                    00 00
  000000014090BE88: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090BE90: FF 15 0A 08 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090BE96: 48 8D 8C 24 40 06  lea         rcx,[rsp+0000000000000640h]
                    00 00
  000000014090BE9E: FF 15 04 08 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090BEA4: 48 8D 15 21 F3 EA  lea         rdx,[00000001427BB1CCh]
                    01
  000000014090BEAB: 48 8D 8C 24 48 06  lea         rcx,[rsp+0000000000000648h]
                    00 00
  000000014090BEB3: FF 15 C7 85 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BEB9: 90                 nop
  000000014090BEBA: 4C 8D 84 24 48 06  lea         r8,[rsp+0000000000000648h]
                    00 00
  000000014090BEC2: 48 8D 94 24 E8 01  lea         rdx,[rsp+00000000000001E8h]
                    00 00
  000000014090BECA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BECF: FF 15 B3 07 50 01  call        qword ptr [0000000141E0C688h]
  000000014090BED5: 90                 nop
  000000014090BED6: 48 8D 8C 24 48 06  lea         rcx,[rsp+0000000000000648h]
                    00 00
  000000014090BEDE: FF 15 FC 9A 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BEE4: 4C 8D 84 24 E8 01  lea         r8,[rsp+00000000000001E8h]
                    00 00
  000000014090BEEC: 48 8D 94 24 50 06  lea         rdx,[rsp+0000000000000650h]
                    00 00
  000000014090BEF4: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  000000014090BEFC: FF 15 9E 07 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090BF02: 48 8D 8C 24 50 06  lea         rcx,[rsp+0000000000000650h]
                    00 00
  000000014090BF0A: FF 15 98 07 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090BF10: 4C 8D 84 24 58 01  lea         r8,[rsp+0000000000000158h]
                    00 00
  000000014090BF18: 48 8D 94 24 F8 01  lea         rdx,[rsp+00000000000001F8h]
                    00 00
  000000014090BF20: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BF25: FF 15 05 07 50 01  call        qword ptr [0000000141E0C630h]
  000000014090BF2B: 90                 nop
  000000014090BF2C: 4C 8D 84 24 F8 01  lea         r8,[rsp+00000000000001F8h]
                    00 00
  000000014090BF34: 48 8D 94 24 58 06  lea         rdx,[rsp+0000000000000658h]
                    00 00
  000000014090BF3C: 48 8D 8C 24 E8 01  lea         rcx,[rsp+00000000000001E8h]
                    00 00
  000000014090BF44: FF 15 56 07 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090BF4A: 48 8D 8C 24 58 06  lea         rcx,[rsp+0000000000000658h]
                    00 00
  000000014090BF52: FF 15 50 07 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090BF58: 90                 nop
  000000014090BF59: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  000000014090BF61: FF 15 B9 06 50 01  call        qword ptr [0000000141E0C620h]
  000000014090BF67: 90                 nop
  000000014090BF68: 48 8D 8C 24 E8 01  lea         rcx,[rsp+00000000000001E8h]
                    00 00
  000000014090BF70: FF 15 3A 07 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090BF76: 90                 nop
  000000014090BF77: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  000000014090BF7F: FF 15 2B 07 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090BF85: 48 8D 94 24 48 01  lea         rdx,[rsp+0000000000000148h]
                    00 00
  000000014090BF8D: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090BF95: E8 66 C0 7C FF     call        00000001400D8000
  000000014090BF9A: 90                 nop
  000000014090BF9B: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  000000014090BFA3: FF 15 1F 80 4F 01  call        qword ptr [0000000141E03FC8h]
  000000014090BFA9: 0F B6 C0           movzx       eax,al
  000000014090BFAC: 85 C0              test        eax,eax
  000000014090BFAE: 0F 85 4D 01 00 00  jne         000000014090C101
  000000014090BFB4: 48 8D 15 1D F2 EA  lea         rdx,[00000001427BB1D8h]
                    01
  000000014090BFBB: 48 8D 8C 24 60 06  lea         rcx,[rsp+0000000000000660h]
                    00 00
  000000014090BFC3: FF 15 B7 84 4F 01  call        qword ptr [0000000141E04480h]
  000000014090BFC9: 90                 nop
  000000014090BFCA: 4C 8D 84 24 60 06  lea         r8,[rsp+0000000000000660h]
                    00 00
  000000014090BFD2: 48 8D 94 24 08 02  lea         rdx,[rsp+0000000000000208h]
                    00 00
  000000014090BFDA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090BFDF: FF 15 A3 06 50 01  call        qword ptr [0000000141E0C688h]
  000000014090BFE5: 90                 nop
  000000014090BFE6: 48 8D 8C 24 60 06  lea         rcx,[rsp+0000000000000660h]
                    00 00
  000000014090BFEE: FF 15 EC 99 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090BFF4: 4C 8D 84 24 08 02  lea         r8,[rsp+0000000000000208h]
                    00 00
  000000014090BFFC: 48 8D 94 24 68 06  lea         rdx,[rsp+0000000000000668h]
                    00 00
  000000014090C004: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090C00C: FF 15 8E 06 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C012: 48 8D 8C 24 68 06  lea         rcx,[rsp+0000000000000668h]
                    00 00
  000000014090C01A: FF 15 88 06 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C020: 48 8D 15 BD F1 EA  lea         rdx,[00000001427BB1E4h]
                    01
  000000014090C027: 48 8D 8C 24 70 06  lea         rcx,[rsp+0000000000000670h]
                    00 00
  000000014090C02F: FF 15 4B 84 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C035: 90                 nop
  000000014090C036: 4C 8D 84 24 70 06  lea         r8,[rsp+0000000000000670h]
                    00 00
  000000014090C03E: 48 8D 94 24 00 02  lea         rdx,[rsp+0000000000000200h]
                    00 00
  000000014090C046: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090C04B: FF 15 37 06 50 01  call        qword ptr [0000000141E0C688h]
  000000014090C051: 90                 nop
  000000014090C052: 48 8D 8C 24 70 06  lea         rcx,[rsp+0000000000000670h]
                    00 00
  000000014090C05A: FF 15 80 99 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C060: 4C 8D 84 24 00 02  lea         r8,[rsp+0000000000000200h]
                    00 00
  000000014090C068: 48 8D 94 24 78 06  lea         rdx,[rsp+0000000000000678h]
                    00 00
  000000014090C070: 48 8D 8C 24 08 02  lea         rcx,[rsp+0000000000000208h]
                    00 00
  000000014090C078: FF 15 22 06 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C07E: 48 8D 8C 24 78 06  lea         rcx,[rsp+0000000000000678h]
                    00 00
  000000014090C086: FF 15 1C 06 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C08C: 4C 8D 84 24 48 01  lea         r8,[rsp+0000000000000148h]
                    00 00
  000000014090C094: 48 8D 94 24 10 02  lea         rdx,[rsp+0000000000000210h]
                    00 00
  000000014090C09C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090C0A1: FF 15 89 05 50 01  call        qword ptr [0000000141E0C630h]
  000000014090C0A7: 90                 nop
  000000014090C0A8: 4C 8D 84 24 10 02  lea         r8,[rsp+0000000000000210h]
                    00 00
  000000014090C0B0: 48 8D 94 24 80 06  lea         rdx,[rsp+0000000000000680h]
                    00 00
  000000014090C0B8: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  000000014090C0C0: FF 15 DA 05 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C0C6: 48 8D 8C 24 80 06  lea         rcx,[rsp+0000000000000680h]
                    00 00
  000000014090C0CE: FF 15 D4 05 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C0D4: 90                 nop
  000000014090C0D5: 48 8D 8C 24 10 02  lea         rcx,[rsp+0000000000000210h]
                    00 00
  000000014090C0DD: FF 15 3D 05 50 01  call        qword ptr [0000000141E0C620h]
  000000014090C0E3: 90                 nop
  000000014090C0E4: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  000000014090C0EC: FF 15 BE 05 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090C0F2: 90                 nop
  000000014090C0F3: 48 8D 8C 24 08 02  lea         rcx,[rsp+0000000000000208h]
                    00 00
  000000014090C0FB: FF 15 AF 05 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090C101: 48 8D 94 24 78 01  lea         rdx,[rsp+0000000000000178h]
                    00 00
  000000014090C109: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090C111: E8 0A 4D 7C FF     call        00000001400D0E20
  000000014090C116: 90                 nop
  000000014090C117: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  000000014090C11F: E8 EC B2 9F FF     call        0000000140307410
  000000014090C124: 0F B6 C0           movzx       eax,al
  000000014090C127: 85 C0              test        eax,eax
  000000014090C129: 0F 85 C6 01 00 00  jne         000000014090C2F5
  000000014090C12F: 48 8D 15 BA F0 EA  lea         rdx,[00000001427BB1F0h]
                    01
  000000014090C136: 48 8D 8C 24 88 06  lea         rcx,[rsp+0000000000000688h]
                    00 00
  000000014090C13E: FF 15 3C 83 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C144: 90                 nop
  000000014090C145: 4C 8D 84 24 88 06  lea         r8,[rsp+0000000000000688h]
                    00 00
  000000014090C14D: 48 8D 94 24 18 02  lea         rdx,[rsp+0000000000000218h]
                    00 00
  000000014090C155: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090C15A: FF 15 28 05 50 01  call        qword ptr [0000000141E0C688h]
  000000014090C160: 90                 nop
  000000014090C161: 48 8D 8C 24 88 06  lea         rcx,[rsp+0000000000000688h]
                    00 00
  000000014090C169: FF 15 71 98 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C16F: 4C 8D 84 24 18 02  lea         r8,[rsp+0000000000000218h]
                    00 00
  000000014090C177: 48 8D 94 24 90 06  lea         rdx,[rsp+0000000000000690h]
                    00 00
  000000014090C17F: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090C187: FF 15 13 05 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C18D: 48 8D 8C 24 90 06  lea         rcx,[rsp+0000000000000690h]
                    00 00
  000000014090C195: FF 15 0D 05 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C19B: C7 84 24 20 02 00  mov         dword ptr [rsp+0000000000000220h],0
                    00 00 00 00 00
  000000014090C1A6: EB 11              jmp         000000014090C1B9
  000000014090C1A8: 8B 84 24 20 02 00  mov         eax,dword ptr [rsp+0000000000000220h]
                    00
  000000014090C1AF: 83 C0 01           add         eax,1
  000000014090C1B2: 89 84 24 20 02 00  mov         dword ptr [rsp+0000000000000220h],eax
                    00
  000000014090C1B9: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  000000014090C1C1: E8 1A 89 6F FF     call        0000000140004AE0
  000000014090C1C6: 39 84 24 20 02 00  cmp         dword ptr [rsp+0000000000000220h],eax
                    00
  000000014090C1CD: 0F 8D 14 01 00 00  jge         000000014090C2E7
  000000014090C1D3: 48 8D 15 26 F0 EA  lea         rdx,[00000001427BB200h]
                    01
  000000014090C1DA: 48 8D 8C 24 98 06  lea         rcx,[rsp+0000000000000698h]
                    00 00
  000000014090C1E2: FF 15 98 82 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C1E8: 90                 nop
  000000014090C1E9: 4C 8D 84 24 98 06  lea         r8,[rsp+0000000000000698h]
                    00 00
  000000014090C1F1: 48 8D 94 24 28 02  lea         rdx,[rsp+0000000000000228h]
                    00 00
  000000014090C1F9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090C1FE: FF 15 84 04 50 01  call        qword ptr [0000000141E0C688h]
  000000014090C204: 90                 nop
  000000014090C205: 48 8D 8C 24 98 06  lea         rcx,[rsp+0000000000000698h]
                    00 00
  000000014090C20D: FF 15 CD 97 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C213: 4C 8D 84 24 28 02  lea         r8,[rsp+0000000000000228h]
                    00 00
  000000014090C21B: 48 8D 94 24 A0 06  lea         rdx,[rsp+00000000000006A0h]
                    00 00
  000000014090C223: 48 8D 8C 24 18 02  lea         rcx,[rsp+0000000000000218h]
                    00 00
  000000014090C22B: FF 15 6F 04 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C231: 48 8D 8C 24 A0 06  lea         rcx,[rsp+00000000000006A0h]
                    00 00
  000000014090C239: FF 15 69 04 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C23F: 48 8D 15 FE E6 EA  lea         rdx,[00000001427BA944h]
                    01
  000000014090C246: 48 8D 8C 24 B0 06  lea         rcx,[rsp+00000000000006B0h]
                    00 00
  000000014090C24E: FF 15 2C 82 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C254: 90                 nop
  000000014090C255: 8B 94 24 20 02 00  mov         edx,dword ptr [rsp+0000000000000220h]
                    00
  000000014090C25C: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  000000014090C264: E8 57 8E 71 FF     call        00000001400250C0
  000000014090C269: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090C26F: 8B 10              mov         edx,dword ptr [rax]
  000000014090C271: 48 8D 8C 24 A8 06  lea         rcx,[rsp+00000000000006A8h]
                    00 00
  000000014090C279: FF 15 41 7B 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090C27F: 48 89 84 24 B8 0A  mov         qword ptr [rsp+0000000000000AB8h],rax
                    00 00
  000000014090C287: 48 8B 84 24 B8 0A  mov         rax,qword ptr [rsp+0000000000000AB8h]
                    00 00
  000000014090C28F: 48 89 84 24 C0 0A  mov         qword ptr [rsp+0000000000000AC0h],rax
                    00 00
  000000014090C297: 4C 8B 84 24 C0 0A  mov         r8,qword ptr [rsp+0000000000000AC0h]
                    00 00
  000000014090C29F: 48 8D 94 24 B0 06  lea         rdx,[rsp+00000000000006B0h]
                    00 00
  000000014090C2A7: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  000000014090C2AF: FF 15 63 03 50 01  call        qword ptr [0000000141E0C618h]
  000000014090C2B5: 90                 nop
  000000014090C2B6: 48 8D 8C 24 A8 06  lea         rcx,[rsp+00000000000006A8h]
                    00 00
  000000014090C2BE: FF 15 1C 97 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C2C4: 90                 nop
  000000014090C2C5: 48 8D 8C 24 B0 06  lea         rcx,[rsp+00000000000006B0h]
                    00 00
  000000014090C2CD: FF 15 0D 97 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C2D3: 90                 nop
  000000014090C2D4: 48 8D 8C 24 28 02  lea         rcx,[rsp+0000000000000228h]
                    00 00
  000000014090C2DC: FF 15 CE 03 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090C2E2: E9 C1 FE FF FF     jmp         000000014090C1A8
  000000014090C2E7: 48 8D 8C 24 18 02  lea         rcx,[rsp+0000000000000218h]
                    00 00
  000000014090C2EF: FF 15 BB 03 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090C2F5: 48 8D 94 24 50 01  lea         rdx,[rsp+0000000000000150h]
                    00 00
  000000014090C2FD: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014090C305: E8 D6 4A 7C FF     call        00000001400D0DE0
  000000014090C30A: 90                 nop
  000000014090C30B: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  000000014090C313: E8 F8 B0 9F FF     call        0000000140307410
  000000014090C318: 0F B6 C0           movzx       eax,al
  000000014090C31B: 85 C0              test        eax,eax
  000000014090C31D: 0F 85 C7 01 00 00  jne         000000014090C4EA
  000000014090C323: 48 8D 15 E6 EE EA  lea         rdx,[00000001427BB210h]
                    01
  000000014090C32A: 48 8D 8C 24 B8 06  lea         rcx,[rsp+00000000000006B8h]
                    00 00
  000000014090C332: FF 15 48 81 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C338: 90                 nop
  000000014090C339: 4C 8D 84 24 B8 06  lea         r8,[rsp+00000000000006B8h]
                    00 00
  000000014090C341: 48 8D 94 24 38 02  lea         rdx,[rsp+0000000000000238h]
                    00 00
  000000014090C349: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090C34E: FF 15 34 03 50 01  call        qword ptr [0000000141E0C688h]
  000000014090C354: 90                 nop
  000000014090C355: 48 8D 8C 24 B8 06  lea         rcx,[rsp+00000000000006B8h]
                    00 00
  000000014090C35D: FF 15 7D 96 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C363: 4C 8D 84 24 38 02  lea         r8,[rsp+0000000000000238h]
                    00 00
  000000014090C36B: 48 8D 94 24 C0 06  lea         rdx,[rsp+00000000000006C0h]
                    00 00
  000000014090C373: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090C37B: FF 15 1F 03 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C381: 48 8D 8C 24 C0 06  lea         rcx,[rsp+00000000000006C0h]
                    00 00
  000000014090C389: FF 15 19 03 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C38F: C7 84 24 30 02 00  mov         dword ptr [rsp+0000000000000230h],0
                    00 00 00 00 00
  000000014090C39A: EB 11              jmp         000000014090C3AD
  000000014090C39C: 8B 84 24 30 02 00  mov         eax,dword ptr [rsp+0000000000000230h]
                    00
  000000014090C3A3: 83 C0 01           add         eax,1
  000000014090C3A6: 89 84 24 30 02 00  mov         dword ptr [rsp+0000000000000230h],eax
                    00
  000000014090C3AD: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  000000014090C3B5: E8 26 87 6F FF     call        0000000140004AE0
  000000014090C3BA: 39 84 24 30 02 00  cmp         dword ptr [rsp+0000000000000230h],eax
                    00
  000000014090C3C1: 0F 8D 14 01 00 00  jge         000000014090C4DB
  000000014090C3C7: 48 8D 15 52 EE EA  lea         rdx,[00000001427BB220h]
                    01
  000000014090C3CE: 48 8D 8C 24 C8 06  lea         rcx,[rsp+00000000000006C8h]
                    00 00
  000000014090C3D6: FF 15 A4 80 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C3DC: 90                 nop
  000000014090C3DD: 4C 8D 84 24 C8 06  lea         r8,[rsp+00000000000006C8h]
                    00 00
  000000014090C3E5: 48 8D 94 24 40 02  lea         rdx,[rsp+0000000000000240h]
                    00 00
  000000014090C3ED: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090C3F2: FF 15 90 02 50 01  call        qword ptr [0000000141E0C688h]
  000000014090C3F8: 90                 nop
  000000014090C3F9: 48 8D 8C 24 C8 06  lea         rcx,[rsp+00000000000006C8h]
                    00 00
  000000014090C401: FF 15 D9 95 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C407: 4C 8D 84 24 40 02  lea         r8,[rsp+0000000000000240h]
                    00 00
  000000014090C40F: 48 8D 94 24 D0 06  lea         rdx,[rsp+00000000000006D0h]
                    00 00
  000000014090C417: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  000000014090C41F: FF 15 7B 02 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C425: 48 8D 8C 24 D0 06  lea         rcx,[rsp+00000000000006D0h]
                    00 00
  000000014090C42D: FF 15 75 02 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C433: 48 8D 15 6A E5 EA  lea         rdx,[00000001427BA9A4h]
                    01
  000000014090C43A: 48 8D 8C 24 E0 06  lea         rcx,[rsp+00000000000006E0h]
                    00 00
  000000014090C442: FF 15 38 80 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C448: 90                 nop
  000000014090C449: 8B 94 24 30 02 00  mov         edx,dword ptr [rsp+0000000000000230h]
                    00
  000000014090C450: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  000000014090C458: E8 63 8C 71 FF     call        00000001400250C0
  000000014090C45D: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090C463: 8B 10              mov         edx,dword ptr [rax]
  000000014090C465: 48 8D 8C 24 D8 06  lea         rcx,[rsp+00000000000006D8h]
                    00 00
  000000014090C46D: FF 15 4D 79 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090C473: 48 89 84 24 C8 0A  mov         qword ptr [rsp+0000000000000AC8h],rax
                    00 00
  000000014090C47B: 48 8B 84 24 C8 0A  mov         rax,qword ptr [rsp+0000000000000AC8h]
                    00 00
  000000014090C483: 48 89 84 24 D0 0A  mov         qword ptr [rsp+0000000000000AD0h],rax
                    00 00
  000000014090C48B: 4C 8B 84 24 D0 0A  mov         r8,qword ptr [rsp+0000000000000AD0h]
                    00 00
  000000014090C493: 48 8D 94 24 E0 06  lea         rdx,[rsp+00000000000006E0h]
                    00 00
  000000014090C49B: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  000000014090C4A3: FF 15 6F 01 50 01  call        qword ptr [0000000141E0C618h]
  000000014090C4A9: 90                 nop
  000000014090C4AA: 48 8D 8C 24 D8 06  lea         rcx,[rsp+00000000000006D8h]
                    00 00
  000000014090C4B2: FF 15 28 95 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C4B8: 90                 nop
  000000014090C4B9: 48 8D 8C 24 E0 06  lea         rcx,[rsp+00000000000006E0h]
                    00 00
  000000014090C4C1: FF 15 19 95 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C4C7: 90                 nop
  000000014090C4C8: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  000000014090C4D0: FF 15 DA 01 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090C4D6: E9 C1 FE FF FF     jmp         000000014090C39C
  000000014090C4DB: 48 8D 8C 24 38 02  lea         rcx,[rsp+0000000000000238h]
                    00 00
  000000014090C4E3: FF 15 C7 01 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090C4E9: 90                 nop
  000000014090C4EA: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  000000014090C4F2: E8 A9 BC 8F FF     call        00000001402081A0
  000000014090C4F7: 90                 nop
  000000014090C4F8: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  000000014090C500: E8 9B BC 8F FF     call        00000001402081A0
  000000014090C505: 90                 nop
  000000014090C506: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  000000014090C50E: FF 15 CC 94 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C514: 90                 nop
  000000014090C515: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  000000014090C51D: FF 15 BD 94 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C523: 90                 nop
  000000014090C524: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  000000014090C52C: FF 15 AE 94 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C532: 90                 nop
  000000014090C533: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  000000014090C53B: FF 15 9F 94 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C541: 90                 nop
  000000014090C542: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  000000014090C54A: FF 15 60 01 50 01  call        qword ptr [0000000141E0C6B0h]
  000000014090C550: E9 C0 EE FF FF     jmp         000000014090B415
  000000014090C555: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090C55D: E8 DE B2 F0 FF     call        0000000140817840
  000000014090C562: 89 84 24 20 01 00  mov         dword ptr [rsp+0000000000000120h],eax
                    00
  000000014090C569: C7 84 24 24 01 00  mov         dword ptr [rsp+0000000000000124h],0
                    00 00 00 00 00
  000000014090C574: EB 11              jmp         000000014090C587
  000000014090C576: 8B 84 24 24 01 00  mov         eax,dword ptr [rsp+0000000000000124h]
                    00
  000000014090C57D: 83 C0 01           add         eax,1
  000000014090C580: 89 84 24 24 01 00  mov         dword ptr [rsp+0000000000000124h],eax
                    00
  000000014090C587: 8B 84 24 20 01 00  mov         eax,dword ptr [rsp+0000000000000120h]
                    00
  000000014090C58E: 39 84 24 24 01 00  cmp         dword ptr [rsp+0000000000000124h],eax
                    00
  000000014090C595: 0F 8D AB 10 00 00  jge         000000014090D646
  000000014090C59B: 48 8D 15 8E EC EA  lea         rdx,[00000001427BB230h]
                    01
  000000014090C5A2: 48 8D 8C 24 E8 06  lea         rcx,[rsp+00000000000006E8h]
                    00 00
  000000014090C5AA: FF 15 D0 7E 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C5B0: 90                 nop
  000000014090C5B1: 4C 8D 84 24 E8 06  lea         r8,[rsp+00000000000006E8h]
                    00 00
  000000014090C5B9: 48 8D 94 24 78 02  lea         rdx,[rsp+0000000000000278h]
                    00 00
  000000014090C5C1: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090C5C6: FF 15 BC 00 50 01  call        qword ptr [0000000141E0C688h]
  000000014090C5CC: 90                 nop
  000000014090C5CD: 48 8D 8C 24 E8 06  lea         rcx,[rsp+00000000000006E8h]
                    00 00
  000000014090C5D5: FF 15 05 94 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C5DB: 4C 8D 84 24 78 02  lea         r8,[rsp+0000000000000278h]
                    00 00
  000000014090C5E3: 48 8D 94 24 F0 06  lea         rdx,[rsp+00000000000006F0h]
                    00 00
  000000014090C5EB: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090C5F3: FF 15 A7 00 50 01  call        qword ptr [0000000141E0C6A0h]
  000000014090C5F9: 48 8D 8C 24 F0 06  lea         rcx,[rsp+00000000000006F0h]
                    00 00
  000000014090C601: FF 15 A1 00 50 01  call        qword ptr [0000000141E0C6A8h]
  000000014090C607: 48 8D 8C 24 A0 02  lea         rcx,[rsp+00000000000002A0h]
                    00 00
  000000014090C60F: FF 15 AB 7E 4F 01  call        qword ptr [0000000141E044C0h]
  000000014090C615: 90                 nop
  000000014090C616: 48 8D 8C 24 98 02  lea         rcx,[rsp+0000000000000298h]
                    00 00
  000000014090C61E: FF 15 9C 7E 4F 01  call        qword ptr [0000000141E044C0h]
  000000014090C624: 90                 nop
  000000014090C625: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  000000014090C62D: FF 15 8D 7E 4F 01  call        qword ptr [0000000141E044C0h]
  000000014090C633: 90                 nop
  000000014090C634: 4C 8D 8C 24 98 02  lea         r9,[rsp+0000000000000298h]
                    00 00
  000000014090C63C: 4C 8D 84 24 A0 02  lea         r8,[rsp+00000000000002A0h]
                    00 00
  000000014090C644: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090C64B: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090C653: E8 E8 B2 F0 FF     call        0000000140817940
  000000014090C658: 4C 8D 84 24 90 02  lea         r8,[rsp+0000000000000290h]
                    00 00
  000000014090C660: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090C667: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090C66F: E8 0C B4 F0 FF     call        0000000140817A80
  000000014090C674: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090C67B: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090C683: E8 D8 B4 F0 FF     call        0000000140817B60
  000000014090C688: 89 84 24 48 02 00  mov         dword ptr [rsp+0000000000000248h],eax
                    00
  000000014090C68F: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090C696: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090C69E: E8 1D B5 F0 FF     call        0000000140817BC0
  000000014090C6A3: 89 84 24 70 02 00  mov         dword ptr [rsp+0000000000000270h],eax
                    00
  000000014090C6AA: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090C6B2: E8 B9 B1 F0 FF     call        0000000140817870
  000000014090C6B7: 89 84 24 84 02 00  mov         dword ptr [rsp+0000000000000284h],eax
                    00
  000000014090C6BE: 48 8D 15 7B EB EA  lea         rdx,[00000001427BB240h]
                    01
  000000014090C6C5: 48 8D 8C 24 00 07  lea         rcx,[rsp+0000000000000700h]
                    00 00
  000000014090C6CD: FF 15 AD 7D 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C6D3: 90                 nop
  000000014090C6D4: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090C6DA: 8B 94 24 48 02 00  mov         edx,dword ptr [rsp+0000000000000248h]
                    00
  000000014090C6E1: 48 8D 8C 24 F8 06  lea         rcx,[rsp+00000000000006F8h]
                    00 00
  000000014090C6E9: FF 15 D1 76 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090C6EF: 48 89 84 24 D8 0A  mov         qword ptr [rsp+0000000000000AD8h],rax
                    00 00
  000000014090C6F7: 48 8B 84 24 D8 0A  mov         rax,qword ptr [rsp+0000000000000AD8h]
                    00 00
  000000014090C6FF: 48 89 84 24 E0 0A  mov         qword ptr [rsp+0000000000000AE0h],rax
                    00 00
  000000014090C707: 4C 8B 84 24 E0 0A  mov         r8,qword ptr [rsp+0000000000000AE0h]
                    00 00
  000000014090C70F: 48 8D 94 24 00 07  lea         rdx,[rsp+0000000000000700h]
                    00 00
  000000014090C717: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090C71F: FF 15 F3 FE 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090C725: 90                 nop
  000000014090C726: 48 8D 8C 24 F8 06  lea         rcx,[rsp+00000000000006F8h]
                    00 00
  000000014090C72E: FF 15 AC 92 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C734: 90                 nop
  000000014090C735: 48 8D 8C 24 00 07  lea         rcx,[rsp+0000000000000700h]
                    00 00
  000000014090C73D: FF 15 9D 92 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C743: 48 8D 15 06 EB EA  lea         rdx,[00000001427BB250h]
                    01
  000000014090C74A: 48 8D 8C 24 10 07  lea         rcx,[rsp+0000000000000710h]
                    00 00
  000000014090C752: FF 15 28 7D 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C758: 90                 nop
  000000014090C759: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090C75F: 8B 94 24 70 02 00  mov         edx,dword ptr [rsp+0000000000000270h]
                    00
  000000014090C766: 48 8D 8C 24 08 07  lea         rcx,[rsp+0000000000000708h]
                    00 00
  000000014090C76E: FF 15 4C 76 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090C774: 48 89 84 24 E8 0A  mov         qword ptr [rsp+0000000000000AE8h],rax
                    00 00
  000000014090C77C: 48 8B 84 24 E8 0A  mov         rax,qword ptr [rsp+0000000000000AE8h]
                    00 00
  000000014090C784: 48 89 84 24 F0 0A  mov         qword ptr [rsp+0000000000000AF0h],rax
                    00 00
  000000014090C78C: 4C 8B 84 24 F0 0A  mov         r8,qword ptr [rsp+0000000000000AF0h]
                    00 00
  000000014090C794: 48 8D 94 24 10 07  lea         rdx,[rsp+0000000000000710h]
                    00 00
  000000014090C79C: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090C7A4: FF 15 6E FE 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090C7AA: 90                 nop
  000000014090C7AB: 48 8D 8C 24 08 07  lea         rcx,[rsp+0000000000000708h]
                    00 00
  000000014090C7B3: FF 15 27 92 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C7B9: 90                 nop
  000000014090C7BA: 48 8D 8C 24 10 07  lea         rcx,[rsp+0000000000000710h]
                    00 00
  000000014090C7C2: FF 15 18 92 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C7C8: 48 8D 15 91 EA EA  lea         rdx,[00000001427BB260h]
                    01
  000000014090C7CF: 48 8D 8C 24 20 07  lea         rcx,[rsp+0000000000000720h]
                    00 00
  000000014090C7D7: FF 15 A3 7C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C7DD: 90                 nop
  000000014090C7DE: 48 8D 94 24 A0 02  lea         rdx,[rsp+00000000000002A0h]
                    00 00
  000000014090C7E6: 48 8D 8C 24 18 07  lea         rcx,[rsp+0000000000000718h]
                    00 00
  000000014090C7EE: E8 1D C7 F0 FF     call        0000000140818F10
  000000014090C7F3: 48 89 84 24 F8 0A  mov         qword ptr [rsp+0000000000000AF8h],rax
                    00 00
  000000014090C7FB: 48 8B 84 24 F8 0A  mov         rax,qword ptr [rsp+0000000000000AF8h]
                    00 00
  000000014090C803: 48 89 84 24 00 0B  mov         qword ptr [rsp+0000000000000B00h],rax
                    00 00
  000000014090C80B: 4C 8B 84 24 00 0B  mov         r8,qword ptr [rsp+0000000000000B00h]
                    00 00
  000000014090C813: 48 8D 94 24 20 07  lea         rdx,[rsp+0000000000000720h]
                    00 00
  000000014090C81B: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090C823: FF 15 EF FD 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090C829: 90                 nop
  000000014090C82A: 48 8D 8C 24 18 07  lea         rcx,[rsp+0000000000000718h]
                    00 00
  000000014090C832: FF 15 A8 91 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C838: 90                 nop
  000000014090C839: 48 8D 8C 24 20 07  lea         rcx,[rsp+0000000000000720h]
                    00 00
  000000014090C841: FF 15 99 91 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C847: 48 8D 15 22 EA EA  lea         rdx,[00000001427BB270h]
                    01
  000000014090C84E: 48 8D 8C 24 30 07  lea         rcx,[rsp+0000000000000730h]
                    00 00
  000000014090C856: FF 15 24 7C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C85C: 90                 nop
  000000014090C85D: 48 8D 94 24 98 02  lea         rdx,[rsp+0000000000000298h]
                    00 00
  000000014090C865: 48 8D 8C 24 28 07  lea         rcx,[rsp+0000000000000728h]
                    00 00
  000000014090C86D: E8 9E C6 F0 FF     call        0000000140818F10
  000000014090C872: 48 89 84 24 08 0B  mov         qword ptr [rsp+0000000000000B08h],rax
                    00 00
  000000014090C87A: 48 8B 84 24 08 0B  mov         rax,qword ptr [rsp+0000000000000B08h]
                    00 00
  000000014090C882: 48 89 84 24 10 0B  mov         qword ptr [rsp+0000000000000B10h],rax
                    00 00
  000000014090C88A: 4C 8B 84 24 10 0B  mov         r8,qword ptr [rsp+0000000000000B10h]
                    00 00
  000000014090C892: 48 8D 94 24 30 07  lea         rdx,[rsp+0000000000000730h]
                    00 00
  000000014090C89A: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090C8A2: FF 15 70 FD 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090C8A8: 90                 nop
  000000014090C8A9: 48 8D 8C 24 28 07  lea         rcx,[rsp+0000000000000728h]
                    00 00
  000000014090C8B1: FF 15 29 91 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C8B7: 90                 nop
  000000014090C8B8: 48 8D 8C 24 30 07  lea         rcx,[rsp+0000000000000730h]
                    00 00
  000000014090C8C0: FF 15 1A 91 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C8C6: 48 8D 15 B3 E9 EA  lea         rdx,[00000001427BB280h]
                    01
  000000014090C8CD: 48 8D 8C 24 40 07  lea         rcx,[rsp+0000000000000740h]
                    00 00
  000000014090C8D5: FF 15 A5 7B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C8DB: 90                 nop
  000000014090C8DC: 48 8D 94 24 90 02  lea         rdx,[rsp+0000000000000290h]
                    00 00
  000000014090C8E4: 48 8D 8C 24 38 07  lea         rcx,[rsp+0000000000000738h]
                    00 00
  000000014090C8EC: E8 1F C6 F0 FF     call        0000000140818F10
  000000014090C8F1: 48 89 84 24 18 0B  mov         qword ptr [rsp+0000000000000B18h],rax
                    00 00
  000000014090C8F9: 48 8B 84 24 18 0B  mov         rax,qword ptr [rsp+0000000000000B18h]
                    00 00
  000000014090C901: 48 89 84 24 20 0B  mov         qword ptr [rsp+0000000000000B20h],rax
                    00 00
  000000014090C909: 4C 8B 84 24 20 0B  mov         r8,qword ptr [rsp+0000000000000B20h]
                    00 00
  000000014090C911: 48 8D 94 24 40 07  lea         rdx,[rsp+0000000000000740h]
                    00 00
  000000014090C919: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090C921: FF 15 F1 FC 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090C927: 90                 nop
  000000014090C928: 48 8D 8C 24 38 07  lea         rcx,[rsp+0000000000000738h]
                    00 00
  000000014090C930: FF 15 AA 90 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C936: 90                 nop
  000000014090C937: 48 8D 8C 24 40 07  lea         rcx,[rsp+0000000000000740h]
                    00 00
  000000014090C93F: FF 15 9B 90 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C945: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090C94C: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090C954: E8 C7 B4 F0 FF     call        0000000140817E20
  000000014090C959: 0F B6 C0           movzx       eax,al
  000000014090C95C: 85 C0              test        eax,eax
  000000014090C95E: 0F 84 81 00 00 00  je          000000014090C9E5
  000000014090C964: 48 8D 15 2D E9 EA  lea         rdx,[00000001427BB298h]
                    01
  000000014090C96B: 48 8D 8C 24 50 07  lea         rcx,[rsp+0000000000000750h]
                    00 00
  000000014090C973: FF 15 07 7B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C979: 90                 nop
  000000014090C97A: 48 8D 15 0F E9 EA  lea         rdx,[00000001427BB290h]
                    01
  000000014090C981: 48 8D 8C 24 48 07  lea         rcx,[rsp+0000000000000748h]
                    00 00
  000000014090C989: FF 15 F1 7A 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C98F: 48 89 84 24 28 0B  mov         qword ptr [rsp+0000000000000B28h],rax
                    00 00
  000000014090C997: 48 8B 84 24 28 0B  mov         rax,qword ptr [rsp+0000000000000B28h]
                    00 00
  000000014090C99F: 48 89 84 24 30 0B  mov         qword ptr [rsp+0000000000000B30h],rax
                    00 00
  000000014090C9A7: 4C 8B 84 24 30 0B  mov         r8,qword ptr [rsp+0000000000000B30h]
                    00 00
  000000014090C9AF: 48 8D 94 24 50 07  lea         rdx,[rsp+0000000000000750h]
                    00 00
  000000014090C9B7: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090C9BF: FF 15 53 FC 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090C9C5: 90                 nop
  000000014090C9C6: 48 8D 8C 24 48 07  lea         rcx,[rsp+0000000000000748h]
                    00 00
  000000014090C9CE: FF 15 0C 90 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C9D4: 90                 nop
  000000014090C9D5: 48 8D 8C 24 50 07  lea         rcx,[rsp+0000000000000750h]
                    00 00
  000000014090C9DD: FF 15 FD 8F 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090C9E3: EB 7F              jmp         000000014090CA64
  000000014090C9E5: 48 8D 15 C4 E8 EA  lea         rdx,[00000001427BB2B0h]
                    01
  000000014090C9EC: 48 8D 8C 24 60 07  lea         rcx,[rsp+0000000000000760h]
                    00 00
  000000014090C9F4: FF 15 86 7A 4F 01  call        qword ptr [0000000141E04480h]
  000000014090C9FA: 90                 nop
  000000014090C9FB: 48 8D 15 A6 E8 EA  lea         rdx,[00000001427BB2A8h]
                    01
  000000014090CA02: 48 8D 8C 24 58 07  lea         rcx,[rsp+0000000000000758h]
                    00 00
  000000014090CA0A: FF 15 70 7A 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CA10: 48 89 84 24 38 0B  mov         qword ptr [rsp+0000000000000B38h],rax
                    00 00
  000000014090CA18: 48 8B 84 24 38 0B  mov         rax,qword ptr [rsp+0000000000000B38h]
                    00 00
  000000014090CA20: 48 89 84 24 40 0B  mov         qword ptr [rsp+0000000000000B40h],rax
                    00 00
  000000014090CA28: 4C 8B 84 24 40 0B  mov         r8,qword ptr [rsp+0000000000000B40h]
                    00 00
  000000014090CA30: 48 8D 94 24 60 07  lea         rdx,[rsp+0000000000000760h]
                    00 00
  000000014090CA38: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090CA40: FF 15 D2 FB 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090CA46: 90                 nop
  000000014090CA47: 48 8D 8C 24 58 07  lea         rcx,[rsp+0000000000000758h]
                    00 00
  000000014090CA4F: FF 15 8B 8F 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CA55: 90                 nop
  000000014090CA56: 48 8D 8C 24 60 07  lea         rcx,[rsp+0000000000000760h]
                    00 00
  000000014090CA5E: FF 15 7C 8F 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CA64: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090CA6B: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090CA73: E8 38 B4 F0 FF     call        0000000140817EB0
  000000014090CA78: 0F B6 C0           movzx       eax,al
  000000014090CA7B: 85 C0              test        eax,eax
  000000014090CA7D: 0F 84 81 00 00 00  je          000000014090CB04
  000000014090CA83: 48 8D 15 3E E8 EA  lea         rdx,[00000001427BB2C8h]
                    01
  000000014090CA8A: 48 8D 8C 24 70 07  lea         rcx,[rsp+0000000000000770h]
                    00 00
  000000014090CA92: FF 15 E8 79 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CA98: 90                 nop
  000000014090CA99: 48 8D 15 20 E8 EA  lea         rdx,[00000001427BB2C0h]
                    01
  000000014090CAA0: 48 8D 8C 24 68 07  lea         rcx,[rsp+0000000000000768h]
                    00 00
  000000014090CAA8: FF 15 D2 79 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CAAE: 48 89 84 24 48 0B  mov         qword ptr [rsp+0000000000000B48h],rax
                    00 00
  000000014090CAB6: 48 8B 84 24 48 0B  mov         rax,qword ptr [rsp+0000000000000B48h]
                    00 00
  000000014090CABE: 48 89 84 24 50 0B  mov         qword ptr [rsp+0000000000000B50h],rax
                    00 00
  000000014090CAC6: 4C 8B 84 24 50 0B  mov         r8,qword ptr [rsp+0000000000000B50h]
                    00 00
  000000014090CACE: 48 8D 94 24 70 07  lea         rdx,[rsp+0000000000000770h]
                    00 00
  000000014090CAD6: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090CADE: FF 15 34 FB 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090CAE4: 90                 nop
  000000014090CAE5: 48 8D 8C 24 68 07  lea         rcx,[rsp+0000000000000768h]
                    00 00
  000000014090CAED: FF 15 ED 8E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CAF3: 90                 nop
  000000014090CAF4: 48 8D 8C 24 70 07  lea         rcx,[rsp+0000000000000770h]
                    00 00
  000000014090CAFC: FF 15 DE 8E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CB02: EB 7F              jmp         000000014090CB83
  000000014090CB04: 48 8D 15 D5 E7 EA  lea         rdx,[00000001427BB2E0h]
                    01
  000000014090CB0B: 48 8D 8C 24 80 07  lea         rcx,[rsp+0000000000000780h]
                    00 00
  000000014090CB13: FF 15 67 79 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CB19: 90                 nop
  000000014090CB1A: 48 8D 15 B7 E7 EA  lea         rdx,[00000001427BB2D8h]
                    01
  000000014090CB21: 48 8D 8C 24 78 07  lea         rcx,[rsp+0000000000000778h]
                    00 00
  000000014090CB29: FF 15 51 79 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CB2F: 48 89 84 24 58 0B  mov         qword ptr [rsp+0000000000000B58h],rax
                    00 00
  000000014090CB37: 48 8B 84 24 58 0B  mov         rax,qword ptr [rsp+0000000000000B58h]
                    00 00
  000000014090CB3F: 48 89 84 24 60 0B  mov         qword ptr [rsp+0000000000000B60h],rax
                    00 00
  000000014090CB47: 4C 8B 84 24 60 0B  mov         r8,qword ptr [rsp+0000000000000B60h]
                    00 00
  000000014090CB4F: 48 8D 94 24 80 07  lea         rdx,[rsp+0000000000000780h]
                    00 00
  000000014090CB57: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090CB5F: FF 15 B3 FA 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090CB65: 90                 nop
  000000014090CB66: 48 8D 8C 24 78 07  lea         rcx,[rsp+0000000000000778h]
                    00 00
  000000014090CB6E: FF 15 6C 8E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CB74: 90                 nop
  000000014090CB75: 48 8D 8C 24 80 07  lea         rcx,[rsp+0000000000000780h]
                    00 00
  000000014090CB7D: FF 15 5D 8E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CB83: 48 8D 15 66 E7 EA  lea         rdx,[00000001427BB2F0h]
                    01
  000000014090CB8A: 48 8D 8C 24 90 07  lea         rcx,[rsp+0000000000000790h]
                    00 00
  000000014090CB92: FF 15 E8 78 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CB98: 90                 nop
  000000014090CB99: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090CB9F: 8B 94 24 84 02 00  mov         edx,dword ptr [rsp+0000000000000284h]
                    00
  000000014090CBA6: 48 8D 8C 24 88 07  lea         rcx,[rsp+0000000000000788h]
                    00 00
  000000014090CBAE: FF 15 0C 72 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090CBB4: 48 89 84 24 68 0B  mov         qword ptr [rsp+0000000000000B68h],rax
                    00 00
  000000014090CBBC: 48 8B 84 24 68 0B  mov         rax,qword ptr [rsp+0000000000000B68h]
                    00 00
  000000014090CBC4: 48 89 84 24 70 0B  mov         qword ptr [rsp+0000000000000B70h],rax
                    00 00
  000000014090CBCC: 4C 8B 84 24 70 0B  mov         r8,qword ptr [rsp+0000000000000B70h]
                    00 00
  000000014090CBD4: 48 8D 94 24 90 07  lea         rdx,[rsp+0000000000000790h]
                    00 00
  000000014090CBDC: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090CBE4: FF 15 2E FA 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090CBEA: 90                 nop
  000000014090CBEB: 48 8D 8C 24 88 07  lea         rcx,[rsp+0000000000000788h]
                    00 00
  000000014090CBF3: FF 15 E7 8D 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CBF9: 90                 nop
  000000014090CBFA: 48 8D 8C 24 90 07  lea         rcx,[rsp+0000000000000790h]
                    00 00
  000000014090CC02: FF 15 D8 8D 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CC08: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  000000014090CC10: E8 CB 2D 88 FF     call        000000014018F9E0
  000000014090CC15: 90                 nop
  000000014090CC16: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  000000014090CC1E: E8 BD 2D 88 FF     call        000000014018F9E0
  000000014090CC23: 90                 nop
  000000014090CC24: 4C 8D 84 24 68 02  lea         r8,[rsp+0000000000000268h]
                    00 00
  000000014090CC2C: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090CC33: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090CC3B: E8 E0 B7 F0 FF     call        0000000140818420
  000000014090CC40: 4C 8D 84 24 50 02  lea         r8,[rsp+0000000000000250h]
                    00 00
  000000014090CC48: 8B 94 24 24 01 00  mov         edx,dword ptr [rsp+0000000000000124h]
                    00
  000000014090CC4F: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014090CC57: E8 34 B8 F0 FF     call        0000000140818490
  000000014090CC5C: 48 8D 15 9D E6 EA  lea         rdx,[00000001427BB300h]
                    01
  000000014090CC63: 48 8D 8C 24 98 07  lea         rcx,[rsp+0000000000000798h]
                    00 00
  000000014090CC6B: FF 15 0F 78 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CC71: 90                 nop
  000000014090CC72: 4C 8D 84 24 98 07  lea         r8,[rsp+0000000000000798h]
                    00 00
  000000014090CC7A: 48 8D 94 24 58 02  lea         rdx,[rsp+0000000000000258h]
                    00 00
  000000014090CC82: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090CC87: FF 15 FB F9 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090CC8D: 90                 nop
  000000014090CC8E: 48 8D 8C 24 98 07  lea         rcx,[rsp+0000000000000798h]
                    00 00
  000000014090CC96: FF 15 44 8D 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CC9C: 4C 8D 84 24 58 02  lea         r8,[rsp+0000000000000258h]
                    00 00
  000000014090CCA4: 48 8D 94 24 A0 07  lea         rdx,[rsp+00000000000007A0h]
                    00 00
  000000014090CCAC: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090CCB4: FF 15 E6 F9 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090CCBA: 48 8D 8C 24 A0 07  lea         rcx,[rsp+00000000000007A0h]
                    00 00
  000000014090CCC2: FF 15 E0 F9 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090CCC8: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  000000014090CCD0: E8 0B 7E 6F FF     call        0000000140004AE0
  000000014090CCD5: 89 84 24 80 02 00  mov         dword ptr [rsp+0000000000000280h],eax
                    00
  000000014090CCDC: C7 84 24 8C 02 00  mov         dword ptr [rsp+000000000000028Ch],0
                    00 00 00 00 00
  000000014090CCE7: EB 11              jmp         000000014090CCFA
  000000014090CCE9: 8B 84 24 8C 02 00  mov         eax,dword ptr [rsp+000000000000028Ch]
                    00
  000000014090CCF0: 83 C0 01           add         eax,1
  000000014090CCF3: 89 84 24 8C 02 00  mov         dword ptr [rsp+000000000000028Ch],eax
                    00
  000000014090CCFA: 8B 84 24 80 02 00  mov         eax,dword ptr [rsp+0000000000000280h]
                    00
  000000014090CD01: 39 84 24 8C 02 00  cmp         dword ptr [rsp+000000000000028Ch],eax
                    00
  000000014090CD08: 0F 8D 06 04 00 00  jge         000000014090D114
  000000014090CD0E: 48 8D 15 03 E6 EA  lea         rdx,[00000001427BB318h]
                    01
  000000014090CD15: 48 8D 8C 24 A8 07  lea         rcx,[rsp+00000000000007A8h]
                    00 00
  000000014090CD1D: FF 15 5D 77 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CD23: 90                 nop
  000000014090CD24: 4C 8D 84 24 A8 07  lea         r8,[rsp+00000000000007A8h]
                    00 00
  000000014090CD2C: 48 8D 94 24 D8 02  lea         rdx,[rsp+00000000000002D8h]
                    00 00
  000000014090CD34: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090CD39: FF 15 49 F9 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090CD3F: 90                 nop
  000000014090CD40: 48 8D 8C 24 A8 07  lea         rcx,[rsp+00000000000007A8h]
                    00 00
  000000014090CD48: FF 15 92 8C 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090CD4E: 4C 8D 84 24 D8 02  lea         r8,[rsp+00000000000002D8h]
                    00 00
  000000014090CD56: 48 8D 94 24 B0 07  lea         rdx,[rsp+00000000000007B0h]
                    00 00
  000000014090CD5E: 48 8D 8C 24 58 02  lea         rcx,[rsp+0000000000000258h]
                    00 00
  000000014090CD66: FF 15 34 F9 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090CD6C: 48 8D 8C 24 B0 07  lea         rcx,[rsp+00000000000007B0h]
                    00 00
  000000014090CD74: FF 15 2E F9 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090CD7A: 8B 94 24 8C 02 00  mov         edx,dword ptr [rsp+000000000000028Ch]
                    00
  000000014090CD81: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  000000014090CD89: E8 F2 B4 7C FF     call        00000001400D8280
  000000014090CD8E: 48 8D BC 24 60 09  lea         rdi,[rsp+0000000000000960h]
                    00 00
  000000014090CD96: 48 8B F0           mov         rsi,rax
  000000014090CD99: B9 10 00 00 00     mov         ecx,10h
  000000014090CD9E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090CDA0: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  000000014090CDA8: 48 8D B4 24 60 09  lea         rsi,[rsp+0000000000000960h]
                    00 00
  000000014090CDB0: B9 10 00 00 00     mov         ecx,10h
  000000014090CDB5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090CDB7: 8B 94 24 8C 02 00  mov         edx,dword ptr [rsp+000000000000028Ch]
                    00
  000000014090CDBE: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  000000014090CDC6: E8 B5 B4 7C FF     call        00000001400D8280
  000000014090CDCB: 48 8D BC 24 50 09  lea         rdi,[rsp+0000000000000950h]
                    00 00
  000000014090CDD3: 48 8D 70 10        lea         rsi,[rax+10h]
  000000014090CDD7: B9 10 00 00 00     mov         ecx,10h
  000000014090CDDC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090CDDE: 48 8D BC 24 A8 02  lea         rdi,[rsp+00000000000002A8h]
                    00 00
  000000014090CDE6: 48 8D B4 24 50 09  lea         rsi,[rsp+0000000000000950h]
                    00 00
  000000014090CDEE: B9 10 00 00 00     mov         ecx,10h
  000000014090CDF3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090CDF5: 48 8D 8C 24 D0 02  lea         rcx,[rsp+00000000000002D0h]
                    00 00
  000000014090CDFD: FF 15 BD 76 4F 01  call        qword ptr [0000000141E044C0h]
  000000014090CE03: 90                 nop
  000000014090CE04: B2 20              mov         dl,20h
  000000014090CE06: 48 8D 8C 24 F2 07  lea         rcx,[rsp+00000000000007F2h]
                    00 00
  000000014090CE0E: E8 4D 5A 6F FF     call        0000000140002860
  000000014090CE13: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090CE16: 48 8D 8C 24 F0 07  lea         rcx,[rsp+00000000000007F0h]
                    00 00
  000000014090CE1E: FF 15 34 75 4F 01  call        qword ptr [0000000141E04358h]
  000000014090CE24: B2 20              mov         dl,20h
  000000014090CE26: 48 8D 8C 24 E2 07  lea         rcx,[rsp+00000000000007E2h]
                    00 00
  000000014090CE2E: E8 2D 5A 6F FF     call        0000000140002860
  000000014090CE33: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090CE36: 48 8D 8C 24 E0 07  lea         rcx,[rsp+00000000000007E0h]
                    00 00
  000000014090CE3E: FF 15 14 75 4F 01  call        qword ptr [0000000141E04358h]
  000000014090CE44: B2 20              mov         dl,20h
  000000014090CE46: 48 8D 8C 24 D2 07  lea         rcx,[rsp+00000000000007D2h]
                    00 00
  000000014090CE4E: E8 0D 5A 6F FF     call        0000000140002860
  000000014090CE53: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090CE56: 48 8D 8C 24 D0 07  lea         rcx,[rsp+00000000000007D0h]
                    00 00
  000000014090CE5E: FF 15 F4 74 4F 01  call        qword ptr [0000000141E04358h]
  000000014090CE64: B2 20              mov         dl,20h
  000000014090CE66: 48 8D 8C 24 C2 07  lea         rcx,[rsp+00000000000007C2h]
                    00 00
  000000014090CE6E: E8 ED 59 6F FF     call        0000000140002860
  000000014090CE73: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090CE76: 48 8D 8C 24 C0 07  lea         rcx,[rsp+00000000000007C0h]
                    00 00
  000000014090CE7E: FF 15 D4 74 4F 01  call        qword ptr [0000000141E04358h]
  000000014090CE84: 48 8D 8C 24 A8 02  lea         rcx,[rsp+00000000000002A8h]
                    00 00
  000000014090CE8C: FF 15 0E 79 4F 01  call        qword ptr [0000000141E047A0h]
  000000014090CE92: 66 44 0F 28 C8     movapd      xmm9,xmm0
  000000014090CE97: 48 8D 8C 24 A8 02  lea         rcx,[rsp+00000000000002A8h]
                    00 00
  000000014090CE9F: FF 15 EB 78 4F 01  call        qword ptr [0000000141E04790h]
  000000014090CEA5: 66 44 0F 28 C0     movapd      xmm8,xmm0
  000000014090CEAA: 48 8D 8C 24 C0 02  lea         rcx,[rsp+00000000000002C0h]
                    00 00
  000000014090CEB2: FF 15 E8 78 4F 01  call        qword ptr [0000000141E047A0h]
  000000014090CEB8: 66 0F 28 F8        movapd      xmm7,xmm0
  000000014090CEBC: 48 8D 8C 24 C0 02  lea         rcx,[rsp+00000000000002C0h]
                    00 00
  000000014090CEC4: FF 15 C6 78 4F 01  call        qword ptr [0000000141E04790h]
  000000014090CECA: 66 0F 28 F0        movapd      xmm6,xmm0
  000000014090CECE: 48 8D 15 53 E4 EA  lea         rdx,[00000001427BB328h]
                    01
  000000014090CED5: 48 8D 8C 24 B8 07  lea         rcx,[rsp+00000000000007B8h]
                    00 00
  000000014090CEDD: FF 15 9D 75 4F 01  call        qword ptr [0000000141E04480h]
  000000014090CEE3: 48 89 84 24 78 0B  mov         qword ptr [rsp+0000000000000B78h],rax
                    00 00
  000000014090CEEB: 48 8B 84 24 78 0B  mov         rax,qword ptr [rsp+0000000000000B78h]
                    00 00
  000000014090CEF3: 48 89 84 24 80 0B  mov         qword ptr [rsp+0000000000000B80h],rax
                    00 00
  000000014090CEFB: 48 8D 84 24 C0 07  lea         rax,[rsp+00000000000007C0h]
                    00 00
  000000014090CF03: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090CF08: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090CF10: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090CF15: 45 33 C9           xor         r9d,r9d
  000000014090CF18: 66 0F 28 D6        movapd      xmm2,xmm6
  000000014090CF1C: 48 8D 94 24 C8 07  lea         rdx,[rsp+00000000000007C8h]
                    00 00
  000000014090CF24: 48 8B 8C 24 80 0B  mov         rcx,qword ptr [rsp+0000000000000B80h]
                    00 00
  000000014090CF2C: FF 15 DE 77 4F 01  call        qword ptr [0000000141E04710h]
  000000014090CF32: 48 89 84 24 88 0B  mov         qword ptr [rsp+0000000000000B88h],rax
                    00 00
  000000014090CF3A: 48 8B 84 24 88 0B  mov         rax,qword ptr [rsp+0000000000000B88h]
                    00 00
  000000014090CF42: 48 89 84 24 90 0B  mov         qword ptr [rsp+0000000000000B90h],rax
                    00 00
  000000014090CF4A: 48 8D 84 24 D0 07  lea         rax,[rsp+00000000000007D0h]
                    00 00
  000000014090CF52: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090CF57: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090CF5F: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090CF64: 45 33 C9           xor         r9d,r9d
  000000014090CF67: 66 0F 28 D7        movapd      xmm2,xmm7
  000000014090CF6B: 48 8D 94 24 D8 07  lea         rdx,[rsp+00000000000007D8h]
                    00 00
  000000014090CF73: 48 8B 8C 24 90 0B  mov         rcx,qword ptr [rsp+0000000000000B90h]
                    00 00
  000000014090CF7B: FF 15 8F 77 4F 01  call        qword ptr [0000000141E04710h]
  000000014090CF81: 48 89 84 24 98 0B  mov         qword ptr [rsp+0000000000000B98h],rax
                    00 00
  000000014090CF89: 48 8B 84 24 98 0B  mov         rax,qword ptr [rsp+0000000000000B98h]
                    00 00
  000000014090CF91: 48 89 84 24 A0 0B  mov         qword ptr [rsp+0000000000000BA0h],rax
                    00 00
  000000014090CF99: 48 8D 84 24 E0 07  lea         rax,[rsp+00000000000007E0h]
                    00 00
  000000014090CFA1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090CFA6: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090CFAE: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090CFB3: 45 33 C9           xor         r9d,r9d
  000000014090CFB6: 66 41 0F 28 D0     movapd      xmm2,xmm8
  000000014090CFBB: 48 8D 94 24 E8 07  lea         rdx,[rsp+00000000000007E8h]
                    00 00
  000000014090CFC3: 48 8B 8C 24 A0 0B  mov         rcx,qword ptr [rsp+0000000000000BA0h]
                    00 00
  000000014090CFCB: FF 15 3F 77 4F 01  call        qword ptr [0000000141E04710h]
  000000014090CFD1: 48 89 84 24 A8 0B  mov         qword ptr [rsp+0000000000000BA8h],rax
                    00 00
  000000014090CFD9: 48 8B 84 24 A8 0B  mov         rax,qword ptr [rsp+0000000000000BA8h]
                    00 00
  000000014090CFE1: 48 89 84 24 B0 0B  mov         qword ptr [rsp+0000000000000BB0h],rax
                    00 00
  000000014090CFE9: 48 8D 84 24 F0 07  lea         rax,[rsp+00000000000007F0h]
                    00 00
  000000014090CFF1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090CFF6: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090CFFE: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090D003: 45 33 C9           xor         r9d,r9d
  000000014090D006: 66 41 0F 28 D1     movapd      xmm2,xmm9
  000000014090D00B: 48 8D 94 24 F8 07  lea         rdx,[rsp+00000000000007F8h]
                    00 00
  000000014090D013: 48 8B 8C 24 B0 0B  mov         rcx,qword ptr [rsp+0000000000000BB0h]
                    00 00
  000000014090D01B: FF 15 EF 76 4F 01  call        qword ptr [0000000141E04710h]
  000000014090D021: 48 89 84 24 B8 0B  mov         qword ptr [rsp+0000000000000BB8h],rax
                    00 00
  000000014090D029: 48 8B 84 24 B8 0B  mov         rax,qword ptr [rsp+0000000000000BB8h]
                    00 00
  000000014090D031: 48 89 84 24 C0 0B  mov         qword ptr [rsp+0000000000000BC0h],rax
                    00 00
  000000014090D039: 48 8B 94 24 C0 0B  mov         rdx,qword ptr [rsp+0000000000000BC0h]
                    00 00
  000000014090D041: 48 8D 8C 24 D0 02  lea         rcx,[rsp+00000000000002D0h]
                    00 00
  000000014090D049: FF 15 21 74 4F 01  call        qword ptr [0000000141E04470h]
  000000014090D04F: 90                 nop
  000000014090D050: 48 8D 8C 24 F8 07  lea         rcx,[rsp+00000000000007F8h]
                    00 00
  000000014090D058: FF 15 82 89 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D05E: 90                 nop
  000000014090D05F: 48 8D 8C 24 E8 07  lea         rcx,[rsp+00000000000007E8h]
                    00 00
  000000014090D067: FF 15 73 89 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D06D: 90                 nop
  000000014090D06E: 48 8D 8C 24 D8 07  lea         rcx,[rsp+00000000000007D8h]
                    00 00
  000000014090D076: FF 15 64 89 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D07C: 90                 nop
  000000014090D07D: 48 8D 8C 24 C8 07  lea         rcx,[rsp+00000000000007C8h]
                    00 00
  000000014090D085: FF 15 55 89 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D08B: 90                 nop
  000000014090D08C: 48 8D 8C 24 B8 07  lea         rcx,[rsp+00000000000007B8h]
                    00 00
  000000014090D094: FF 15 46 89 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D09A: 4C 8D 84 24 D0 02  lea         r8,[rsp+00000000000002D0h]
                    00 00
  000000014090D0A2: 48 8D 94 24 B8 02  lea         rdx,[rsp+00000000000002B8h]
                    00 00
  000000014090D0AA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090D0AF: FF 15 7B F5 4F 01  call        qword ptr [0000000141E0C630h]
  000000014090D0B5: 90                 nop
  000000014090D0B6: 4C 8D 84 24 B8 02  lea         r8,[rsp+00000000000002B8h]
                    00 00
  000000014090D0BE: 48 8D 94 24 00 08  lea         rdx,[rsp+0000000000000800h]
                    00 00
  000000014090D0C6: 48 8D 8C 24 D8 02  lea         rcx,[rsp+00000000000002D8h]
                    00 00
  000000014090D0CE: FF 15 CC F5 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090D0D4: 48 8D 8C 24 00 08  lea         rcx,[rsp+0000000000000800h]
                    00 00
  000000014090D0DC: FF 15 C6 F5 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090D0E2: 90                 nop
  000000014090D0E3: 48 8D 8C 24 B8 02  lea         rcx,[rsp+00000000000002B8h]
                    00 00
  000000014090D0EB: FF 15 2F F5 4F 01  call        qword ptr [0000000141E0C620h]
  000000014090D0F1: 90                 nop
  000000014090D0F2: 48 8D 8C 24 D0 02  lea         rcx,[rsp+00000000000002D0h]
                    00 00
  000000014090D0FA: FF 15 E0 88 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D100: 90                 nop
  000000014090D101: 48 8D 8C 24 D8 02  lea         rcx,[rsp+00000000000002D8h]
                    00 00
  000000014090D109: FF 15 A1 F5 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090D10F: E9 D5 FB FF FF     jmp         000000014090CCE9
  000000014090D114: 48 8D 15 1D E2 EA  lea         rdx,[00000001427BB338h]
                    01
  000000014090D11B: 48 8D 8C 24 08 08  lea         rcx,[rsp+0000000000000808h]
                    00 00
  000000014090D123: FF 15 57 73 4F 01  call        qword ptr [0000000141E04480h]
  000000014090D129: 90                 nop
  000000014090D12A: 4C 8D 84 24 08 08  lea         r8,[rsp+0000000000000808h]
                    00 00
  000000014090D132: 48 8D 94 24 60 02  lea         rdx,[rsp+0000000000000260h]
                    00 00
  000000014090D13A: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090D13F: FF 15 43 F5 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090D145: 90                 nop
  000000014090D146: 48 8D 8C 24 08 08  lea         rcx,[rsp+0000000000000808h]
                    00 00
  000000014090D14E: FF 15 8C 88 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D154: 4C 8D 84 24 60 02  lea         r8,[rsp+0000000000000260h]
                    00 00
  000000014090D15C: 48 8D 94 24 10 08  lea         rdx,[rsp+0000000000000810h]
                    00 00
  000000014090D164: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090D16C: FF 15 2E F5 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090D172: 48 8D 8C 24 10 08  lea         rcx,[rsp+0000000000000810h]
                    00 00
  000000014090D17A: FF 15 28 F5 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090D180: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  000000014090D188: E8 53 79 6F FF     call        0000000140004AE0
  000000014090D18D: 89 84 24 88 02 00  mov         dword ptr [rsp+0000000000000288h],eax
                    00
  000000014090D194: C7 84 24 8C 02 00  mov         dword ptr [rsp+000000000000028Ch],0
                    00 00 00 00 00
  000000014090D19F: EB 11              jmp         000000014090D1B2
  000000014090D1A1: 8B 84 24 8C 02 00  mov         eax,dword ptr [rsp+000000000000028Ch]
                    00
  000000014090D1A8: 83 C0 01           add         eax,1
  000000014090D1AB: 89 84 24 8C 02 00  mov         dword ptr [rsp+000000000000028Ch],eax
                    00
  000000014090D1B2: 8B 84 24 88 02 00  mov         eax,dword ptr [rsp+0000000000000288h]
                    00
  000000014090D1B9: 39 84 24 8C 02 00  cmp         dword ptr [rsp+000000000000028Ch],eax
                    00
  000000014090D1C0: 0F 8D 06 04 00 00  jge         000000014090D5CC
  000000014090D1C6: 48 8D 15 83 E1 EA  lea         rdx,[00000001427BB350h]
                    01
  000000014090D1CD: 48 8D 8C 24 18 08  lea         rcx,[rsp+0000000000000818h]
                    00 00
  000000014090D1D5: FF 15 A5 72 4F 01  call        qword ptr [0000000141E04480h]
  000000014090D1DB: 90                 nop
  000000014090D1DC: 4C 8D 84 24 18 08  lea         r8,[rsp+0000000000000818h]
                    00 00
  000000014090D1E4: 48 8D 94 24 10 03  lea         rdx,[rsp+0000000000000310h]
                    00 00
  000000014090D1EC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090D1F1: FF 15 91 F4 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090D1F7: 90                 nop
  000000014090D1F8: 48 8D 8C 24 18 08  lea         rcx,[rsp+0000000000000818h]
                    00 00
  000000014090D200: FF 15 DA 87 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D206: 4C 8D 84 24 10 03  lea         r8,[rsp+0000000000000310h]
                    00 00
  000000014090D20E: 48 8D 94 24 20 08  lea         rdx,[rsp+0000000000000820h]
                    00 00
  000000014090D216: 48 8D 8C 24 60 02  lea         rcx,[rsp+0000000000000260h]
                    00 00
  000000014090D21E: FF 15 7C F4 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090D224: 48 8D 8C 24 20 08  lea         rcx,[rsp+0000000000000820h]
                    00 00
  000000014090D22C: FF 15 76 F4 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090D232: 8B 94 24 8C 02 00  mov         edx,dword ptr [rsp+000000000000028Ch]
                    00
  000000014090D239: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  000000014090D241: E8 3A B0 7C FF     call        00000001400D8280
  000000014090D246: 48 8D BC 24 40 09  lea         rdi,[rsp+0000000000000940h]
                    00 00
  000000014090D24E: 48 8B F0           mov         rsi,rax
  000000014090D251: B9 10 00 00 00     mov         ecx,10h
  000000014090D256: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090D258: 48 8D BC 24 F8 02  lea         rdi,[rsp+00000000000002F8h]
                    00 00
  000000014090D260: 48 8D B4 24 40 09  lea         rsi,[rsp+0000000000000940h]
                    00 00
  000000014090D268: B9 10 00 00 00     mov         ecx,10h
  000000014090D26D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090D26F: 8B 94 24 8C 02 00  mov         edx,dword ptr [rsp+000000000000028Ch]
                    00
  000000014090D276: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  000000014090D27E: E8 FD AF 7C FF     call        00000001400D8280
  000000014090D283: 48 8D BC 24 30 09  lea         rdi,[rsp+0000000000000930h]
                    00 00
  000000014090D28B: 48 8D 70 10        lea         rsi,[rax+10h]
  000000014090D28F: B9 10 00 00 00     mov         ecx,10h
  000000014090D294: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090D296: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  000000014090D29E: 48 8D B4 24 30 09  lea         rsi,[rsp+0000000000000930h]
                    00 00
  000000014090D2A6: B9 10 00 00 00     mov         ecx,10h
  000000014090D2AB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014090D2AD: 48 8D 8C 24 08 03  lea         rcx,[rsp+0000000000000308h]
                    00 00
  000000014090D2B5: FF 15 05 72 4F 01  call        qword ptr [0000000141E044C0h]
  000000014090D2BB: 90                 nop
  000000014090D2BC: B2 20              mov         dl,20h
  000000014090D2BE: 48 8D 8C 24 62 08  lea         rcx,[rsp+0000000000000862h]
                    00 00
  000000014090D2C6: E8 95 55 6F FF     call        0000000140002860
  000000014090D2CB: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090D2CE: 48 8D 8C 24 60 08  lea         rcx,[rsp+0000000000000860h]
                    00 00
  000000014090D2D6: FF 15 7C 70 4F 01  call        qword ptr [0000000141E04358h]
  000000014090D2DC: B2 20              mov         dl,20h
  000000014090D2DE: 48 8D 8C 24 52 08  lea         rcx,[rsp+0000000000000852h]
                    00 00
  000000014090D2E6: E8 75 55 6F FF     call        0000000140002860
  000000014090D2EB: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090D2EE: 48 8D 8C 24 50 08  lea         rcx,[rsp+0000000000000850h]
                    00 00
  000000014090D2F6: FF 15 5C 70 4F 01  call        qword ptr [0000000141E04358h]
  000000014090D2FC: B2 20              mov         dl,20h
  000000014090D2FE: 48 8D 8C 24 42 08  lea         rcx,[rsp+0000000000000842h]
                    00 00
  000000014090D306: E8 55 55 6F FF     call        0000000140002860
  000000014090D30B: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090D30E: 48 8D 8C 24 40 08  lea         rcx,[rsp+0000000000000840h]
                    00 00
  000000014090D316: FF 15 3C 70 4F 01  call        qword ptr [0000000141E04358h]
  000000014090D31C: B2 20              mov         dl,20h
  000000014090D31E: 48 8D 8C 24 32 08  lea         rcx,[rsp+0000000000000832h]
                    00 00
  000000014090D326: E8 35 55 6F FF     call        0000000140002860
  000000014090D32B: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090D32E: 48 8D 8C 24 30 08  lea         rcx,[rsp+0000000000000830h]
                    00 00
  000000014090D336: FF 15 1C 70 4F 01  call        qword ptr [0000000141E04358h]
  000000014090D33C: 48 8D 8C 24 E0 02  lea         rcx,[rsp+00000000000002E0h]
                    00 00
  000000014090D344: FF 15 56 74 4F 01  call        qword ptr [0000000141E047A0h]
  000000014090D34A: 66 44 0F 28 C8     movapd      xmm9,xmm0
  000000014090D34F: 48 8D 8C 24 E0 02  lea         rcx,[rsp+00000000000002E0h]
                    00 00
  000000014090D357: FF 15 33 74 4F 01  call        qword ptr [0000000141E04790h]
  000000014090D35D: 66 44 0F 28 C0     movapd      xmm8,xmm0
  000000014090D362: 48 8D 8C 24 F8 02  lea         rcx,[rsp+00000000000002F8h]
                    00 00
  000000014090D36A: FF 15 30 74 4F 01  call        qword ptr [0000000141E047A0h]
  000000014090D370: 66 0F 28 F8        movapd      xmm7,xmm0
  000000014090D374: 48 8D 8C 24 F8 02  lea         rcx,[rsp+00000000000002F8h]
                    00 00
  000000014090D37C: FF 15 0E 74 4F 01  call        qword ptr [0000000141E04790h]
  000000014090D382: 66 0F 28 F0        movapd      xmm6,xmm0
  000000014090D386: 48 8D 15 D3 DF EA  lea         rdx,[00000001427BB360h]
                    01
  000000014090D38D: 48 8D 8C 24 28 08  lea         rcx,[rsp+0000000000000828h]
                    00 00
  000000014090D395: FF 15 E5 70 4F 01  call        qword ptr [0000000141E04480h]
  000000014090D39B: 48 89 84 24 C8 0B  mov         qword ptr [rsp+0000000000000BC8h],rax
                    00 00
  000000014090D3A3: 48 8B 84 24 C8 0B  mov         rax,qword ptr [rsp+0000000000000BC8h]
                    00 00
  000000014090D3AB: 48 89 84 24 D0 0B  mov         qword ptr [rsp+0000000000000BD0h],rax
                    00 00
  000000014090D3B3: 48 8D 84 24 30 08  lea         rax,[rsp+0000000000000830h]
                    00 00
  000000014090D3BB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090D3C0: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090D3C8: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090D3CD: 45 33 C9           xor         r9d,r9d
  000000014090D3D0: 66 0F 28 D6        movapd      xmm2,xmm6
  000000014090D3D4: 48 8D 94 24 38 08  lea         rdx,[rsp+0000000000000838h]
                    00 00
  000000014090D3DC: 48 8B 8C 24 D0 0B  mov         rcx,qword ptr [rsp+0000000000000BD0h]
                    00 00
  000000014090D3E4: FF 15 26 73 4F 01  call        qword ptr [0000000141E04710h]
  000000014090D3EA: 48 89 84 24 D8 0B  mov         qword ptr [rsp+0000000000000BD8h],rax
                    00 00
  000000014090D3F2: 48 8B 84 24 D8 0B  mov         rax,qword ptr [rsp+0000000000000BD8h]
                    00 00
  000000014090D3FA: 48 89 84 24 E0 0B  mov         qword ptr [rsp+0000000000000BE0h],rax
                    00 00
  000000014090D402: 48 8D 84 24 40 08  lea         rax,[rsp+0000000000000840h]
                    00 00
  000000014090D40A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090D40F: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090D417: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090D41C: 45 33 C9           xor         r9d,r9d
  000000014090D41F: 66 0F 28 D7        movapd      xmm2,xmm7
  000000014090D423: 48 8D 94 24 48 08  lea         rdx,[rsp+0000000000000848h]
                    00 00
  000000014090D42B: 48 8B 8C 24 E0 0B  mov         rcx,qword ptr [rsp+0000000000000BE0h]
                    00 00
  000000014090D433: FF 15 D7 72 4F 01  call        qword ptr [0000000141E04710h]
  000000014090D439: 48 89 84 24 E8 0B  mov         qword ptr [rsp+0000000000000BE8h],rax
                    00 00
  000000014090D441: 48 8B 84 24 E8 0B  mov         rax,qword ptr [rsp+0000000000000BE8h]
                    00 00
  000000014090D449: 48 89 84 24 F0 0B  mov         qword ptr [rsp+0000000000000BF0h],rax
                    00 00
  000000014090D451: 48 8D 84 24 50 08  lea         rax,[rsp+0000000000000850h]
                    00 00
  000000014090D459: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090D45E: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090D466: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090D46B: 45 33 C9           xor         r9d,r9d
  000000014090D46E: 66 41 0F 28 D0     movapd      xmm2,xmm8
  000000014090D473: 48 8D 94 24 58 08  lea         rdx,[rsp+0000000000000858h]
                    00 00
  000000014090D47B: 48 8B 8C 24 F0 0B  mov         rcx,qword ptr [rsp+0000000000000BF0h]
                    00 00
  000000014090D483: FF 15 87 72 4F 01  call        qword ptr [0000000141E04710h]
  000000014090D489: 48 89 84 24 F8 0B  mov         qword ptr [rsp+0000000000000BF8h],rax
                    00 00
  000000014090D491: 48 8B 84 24 F8 0B  mov         rax,qword ptr [rsp+0000000000000BF8h]
                    00 00
  000000014090D499: 48 89 84 24 00 0C  mov         qword ptr [rsp+0000000000000C00h],rax
                    00 00
  000000014090D4A1: 48 8D 84 24 60 08  lea         rax,[rsp+0000000000000860h]
                    00 00
  000000014090D4A9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014090D4AE: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  000000014090D4B6: C6 44 24 20 67     mov         byte ptr [rsp+20h],67h
  000000014090D4BB: 45 33 C9           xor         r9d,r9d
  000000014090D4BE: 66 41 0F 28 D1     movapd      xmm2,xmm9
  000000014090D4C3: 48 8D 94 24 68 08  lea         rdx,[rsp+0000000000000868h]
                    00 00
  000000014090D4CB: 48 8B 8C 24 00 0C  mov         rcx,qword ptr [rsp+0000000000000C00h]
                    00 00
  000000014090D4D3: FF 15 37 72 4F 01  call        qword ptr [0000000141E04710h]
  000000014090D4D9: 48 89 84 24 08 0C  mov         qword ptr [rsp+0000000000000C08h],rax
                    00 00
  000000014090D4E1: 48 8B 84 24 08 0C  mov         rax,qword ptr [rsp+0000000000000C08h]
                    00 00
  000000014090D4E9: 48 89 84 24 10 0C  mov         qword ptr [rsp+0000000000000C10h],rax
                    00 00
  000000014090D4F1: 48 8B 94 24 10 0C  mov         rdx,qword ptr [rsp+0000000000000C10h]
                    00 00
  000000014090D4F9: 48 8D 8C 24 08 03  lea         rcx,[rsp+0000000000000308h]
                    00 00
  000000014090D501: FF 15 69 6F 4F 01  call        qword ptr [0000000141E04470h]
  000000014090D507: 90                 nop
  000000014090D508: 48 8D 8C 24 68 08  lea         rcx,[rsp+0000000000000868h]
                    00 00
  000000014090D510: FF 15 CA 84 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D516: 90                 nop
  000000014090D517: 48 8D 8C 24 58 08  lea         rcx,[rsp+0000000000000858h]
                    00 00
  000000014090D51F: FF 15 BB 84 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D525: 90                 nop
  000000014090D526: 48 8D 8C 24 48 08  lea         rcx,[rsp+0000000000000848h]
                    00 00
  000000014090D52E: FF 15 AC 84 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D534: 90                 nop
  000000014090D535: 48 8D 8C 24 38 08  lea         rcx,[rsp+0000000000000838h]
                    00 00
  000000014090D53D: FF 15 9D 84 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D543: 90                 nop
  000000014090D544: 48 8D 8C 24 28 08  lea         rcx,[rsp+0000000000000828h]
                    00 00
  000000014090D54C: FF 15 8E 84 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D552: 4C 8D 84 24 08 03  lea         r8,[rsp+0000000000000308h]
                    00 00
  000000014090D55A: 48 8D 94 24 F0 02  lea         rdx,[rsp+00000000000002F0h]
                    00 00
  000000014090D562: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090D567: FF 15 C3 F0 4F 01  call        qword ptr [0000000141E0C630h]
  000000014090D56D: 90                 nop
  000000014090D56E: 4C 8D 84 24 F0 02  lea         r8,[rsp+00000000000002F0h]
                    00 00
  000000014090D576: 48 8D 94 24 70 08  lea         rdx,[rsp+0000000000000870h]
                    00 00
  000000014090D57E: 48 8D 8C 24 10 03  lea         rcx,[rsp+0000000000000310h]
                    00 00
  000000014090D586: FF 15 14 F1 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090D58C: 48 8D 8C 24 70 08  lea         rcx,[rsp+0000000000000870h]
                    00 00
  000000014090D594: FF 15 0E F1 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090D59A: 90                 nop
  000000014090D59B: 48 8D 8C 24 F0 02  lea         rcx,[rsp+00000000000002F0h]
                    00 00
  000000014090D5A3: FF 15 77 F0 4F 01  call        qword ptr [0000000141E0C620h]
  000000014090D5A9: 90                 nop
  000000014090D5AA: 48 8D 8C 24 08 03  lea         rcx,[rsp+0000000000000308h]
                    00 00
  000000014090D5B2: FF 15 28 84 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D5B8: 90                 nop
  000000014090D5B9: 48 8D 8C 24 10 03  lea         rcx,[rsp+0000000000000310h]
                    00 00
  000000014090D5C1: FF 15 E9 F0 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090D5C7: E9 D5 FB FF FF     jmp         000000014090D1A1
  000000014090D5CC: 48 8D 8C 24 60 02  lea         rcx,[rsp+0000000000000260h]
                    00 00
  000000014090D5D4: FF 15 D6 F0 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090D5DA: 90                 nop
  000000014090D5DB: 48 8D 8C 24 58 02  lea         rcx,[rsp+0000000000000258h]
                    00 00
  000000014090D5E3: FF 15 C7 F0 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090D5E9: 90                 nop
  000000014090D5EA: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  000000014090D5F2: E8 39 2A CD FF     call        00000001405E0030
  000000014090D5F7: 90                 nop
  000000014090D5F8: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  000000014090D600: E8 2B 2A CD FF     call        00000001405E0030
  000000014090D605: 90                 nop
  000000014090D606: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  000000014090D60E: FF 15 CC 83 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D614: 90                 nop
  000000014090D615: 48 8D 8C 24 98 02  lea         rcx,[rsp+0000000000000298h]
                    00 00
  000000014090D61D: FF 15 BD 83 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D623: 90                 nop
  000000014090D624: 48 8D 8C 24 A0 02  lea         rcx,[rsp+00000000000002A0h]
                    00 00
  000000014090D62C: FF 15 AE 83 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D632: 90                 nop
  000000014090D633: 48 8D 8C 24 78 02  lea         rcx,[rsp+0000000000000278h]
                    00 00
  000000014090D63B: FF 15 6F F0 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090D641: E9 30 EF FF FF     jmp         000000014090C576
  000000014090D646: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  000000014090D64E: FF 15 8C 83 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D654: 90                 nop
  000000014090D655: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  000000014090D65D: FF 15 4D F0 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090D663: 90                 nop
  000000014090D664: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  000000014090D66C: FF 15 6E 83 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D672: E9 AC D8 FF FF     jmp         000000014090AF23
  000000014090D677: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090D67F: E8 6C 1D AD 00     call        00000001413DF3F0
  000000014090D684: 90                 nop
  000000014090D685: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090D68D: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090D690: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000014090D695: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090D69D: FF 90 D8 01 00 00  call        qword ptr [rax+00000000000001D8h]
  000000014090D6A3: 90                 nop
  000000014090D6A4: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  000000014090D6AC: EB 0B              jmp         000000014090D6B9
  000000014090D6AE: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  000000014090D6B2: 83 C0 01           add         eax,1
  000000014090D6B5: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  000000014090D6B9: 8B 84 24 C0 00 00  mov         eax,dword ptr [rsp+00000000000000C0h]
                    00
  000000014090D6C0: 39 44 24 68        cmp         dword ptr [rsp+68h],eax
  000000014090D6C4: 0F 8D 5F 01 00 00  jge         000000014090D829
  000000014090D6CA: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090D6D2: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090D6D5: 44 8B 44 24 68     mov         r8d,dword ptr [rsp+68h]
  000000014090D6DA: 48 8D 94 24 18 03  lea         rdx,[rsp+0000000000000318h]
                    00 00
  000000014090D6E2: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090D6EA: FF 90 90 01 00 00  call        qword ptr [rax+0000000000000190h]
  000000014090D6F0: 90                 nop
  000000014090D6F1: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090D6F5: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014090D6FA: E8 11 35 85 FF     call        0000000140160C10
  000000014090D6FF: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090D702: 48 89 84 24 20 03  mov         qword ptr [rsp+0000000000000320h],rax
                    00 00
  000000014090D70A: 48 83 BC 24 20 03  cmp         qword ptr [rsp+0000000000000320h],0
                    00 00 00
  000000014090D713: 0F 84 FD 00 00 00  je          000000014090D816
  000000014090D719: 48 8B 8C 24 20 03  mov         rcx,qword ptr [rsp+0000000000000320h]
                    00 00
  000000014090D721: E8 DA A7 7C FF     call        00000001400D7F00
  000000014090D726: 89 84 24 28 03 00  mov         dword ptr [rsp+0000000000000328h],eax
                    00
  000000014090D72D: C7 84 24 2C 03 00  mov         dword ptr [rsp+000000000000032Ch],0
                    00 00 00 00 00
  000000014090D738: EB 11              jmp         000000014090D74B
  000000014090D73A: 8B 84 24 2C 03 00  mov         eax,dword ptr [rsp+000000000000032Ch]
                    00
  000000014090D741: 83 C0 01           add         eax,1
  000000014090D744: 89 84 24 2C 03 00  mov         dword ptr [rsp+000000000000032Ch],eax
                    00
  000000014090D74B: 8B 84 24 28 03 00  mov         eax,dword ptr [rsp+0000000000000328h]
                    00
  000000014090D752: 39 84 24 2C 03 00  cmp         dword ptr [rsp+000000000000032Ch],eax
                    00
  000000014090D759: 0F 8D B7 00 00 00  jge         000000014090D816
  000000014090D75F: 8B 94 24 2C 03 00  mov         edx,dword ptr [rsp+000000000000032Ch]
                    00
  000000014090D766: 48 8B 8C 24 20 03  mov         rcx,qword ptr [rsp+0000000000000320h]
                    00 00
  000000014090D76E: E8 6D 93 F0 FF     call        0000000140816AE0
  000000014090D773: 89 84 24 30 03 00  mov         dword ptr [rsp+0000000000000330h],eax
                    00
  000000014090D77A: 48 8D 8C 24 38 03  lea         rcx,[rsp+0000000000000338h]
                    00 00
  000000014090D782: E8 59 22 88 FF     call        000000014018F9E0
  000000014090D787: 90                 nop
  000000014090D788: 48 8D 94 24 30 03  lea         rdx,[rsp+0000000000000330h]
                    00 00
  000000014090D790: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090D798: E8 83 5E 33 00     call        0000000140C43620
  000000014090D79D: 0F B6 C0           movzx       eax,al
  000000014090D7A0: 85 C0              test        eax,eax
  000000014090D7A2: 74 25              je          000000014090D7C9
  000000014090D7A4: 48 8D 94 24 30 03  lea         rdx,[rsp+0000000000000330h]
                    00 00
  000000014090D7AC: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090D7B4: E8 27 9C 00 00     call        00000001409173E0
  000000014090D7B9: 48 8B D0           mov         rdx,rax
  000000014090D7BC: 48 8D 8C 24 38 03  lea         rcx,[rsp+0000000000000338h]
                    00 00
  000000014090D7C4: E8 F7 B2 77 FF     call        0000000140088AC0
  000000014090D7C9: 48 8D 94 24 18 03  lea         rdx,[rsp+0000000000000318h]
                    00 00
  000000014090D7D1: 48 8D 8C 24 38 03  lea         rcx,[rsp+0000000000000338h]
                    00 00
  000000014090D7D9: E8 B2 73 6F FF     call        0000000140004B90
  000000014090D7DE: 48 8D 94 24 30 03  lea         rdx,[rsp+0000000000000330h]
                    00 00
  000000014090D7E6: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090D7EE: E8 ED 9B 00 00     call        00000001409173E0
  000000014090D7F3: 48 8D 94 24 38 03  lea         rdx,[rsp+0000000000000338h]
                    00 00
  000000014090D7FB: 48 8B C8           mov         rcx,rax
  000000014090D7FE: E8 BD B2 77 FF     call        0000000140088AC0
  000000014090D803: 90                 nop
  000000014090D804: 48 8D 8C 24 38 03  lea         rcx,[rsp+0000000000000338h]
                    00 00
  000000014090D80C: E8 EF 72 6F FF     call        0000000140004B00
  000000014090D811: E9 24 FF FF FF     jmp         000000014090D73A
  000000014090D816: 48 8D 8C 24 18 03  lea         rcx,[rsp+0000000000000318h]
                    00 00
  000000014090D81E: FF 15 BC 81 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D824: E9 85 FE FF FF     jmp         000000014090D6AE
  000000014090D829: 48 8D 15 40 DB EA  lea         rdx,[00000001427BB370h]
                    01
  000000014090D830: 48 8D 8C 24 78 08  lea         rcx,[rsp+0000000000000878h]
                    00 00
  000000014090D838: FF 15 42 6C 4F 01  call        qword ptr [0000000141E04480h]
  000000014090D83E: 90                 nop
  000000014090D83F: 4C 8D 84 24 78 08  lea         r8,[rsp+0000000000000878h]
                    00 00
  000000014090D847: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000014090D84F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090D854: FF 15 2E EE 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090D85A: 90                 nop
  000000014090D85B: 48 8D 8C 24 78 08  lea         rcx,[rsp+0000000000000878h]
                    00 00
  000000014090D863: FF 15 77 81 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D869: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  000000014090D871: 48 8D 94 24 80 08  lea         rdx,[rsp+0000000000000880h]
                    00 00
  000000014090D879: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090D881: FF 15 19 EE 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090D887: 48 8D 8C 24 80 08  lea         rcx,[rsp+0000000000000880h]
                    00 00
  000000014090D88F: FF 15 13 EE 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090D895: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090D89D: E8 DE C5 A8 00     call        0000000141399E80
  000000014090D8A2: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  000000014090D8A9: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  000000014090D8B1: EB 0B              jmp         000000014090D8BE
  000000014090D8B3: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  000000014090D8B7: 83 C0 01           add         eax,1
  000000014090D8BA: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  000000014090D8BE: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  000000014090D8C5: 39 44 24 68        cmp         dword ptr [rsp+68h],eax
  000000014090D8C9: 0F 8D DD 02 00 00  jge         000000014090DBAC
  000000014090D8CF: 48 8D 15 AA DA EA  lea         rdx,[00000001427BB380h]
                    01
  000000014090D8D6: 48 8D 8C 24 88 08  lea         rcx,[rsp+0000000000000888h]
                    00 00
  000000014090D8DE: FF 15 9C 6B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090D8E4: 90                 nop
  000000014090D8E5: 4C 8D 84 24 88 08  lea         r8,[rsp+0000000000000888h]
                    00 00
  000000014090D8ED: 48 8D 94 24 48 03  lea         rdx,[rsp+0000000000000348h]
                    00 00
  000000014090D8F5: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090D8FA: FF 15 88 ED 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090D900: 90                 nop
  000000014090D901: 48 8D 8C 24 88 08  lea         rcx,[rsp+0000000000000888h]
                    00 00
  000000014090D909: FF 15 D1 80 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D90F: 4C 8D 84 24 48 03  lea         r8,[rsp+0000000000000348h]
                    00 00
  000000014090D917: 48 8D 94 24 90 08  lea         rdx,[rsp+0000000000000890h]
                    00 00
  000000014090D91F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014090D927: FF 15 73 ED 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090D92D: 48 8D 8C 24 90 08  lea         rcx,[rsp+0000000000000890h]
                    00 00
  000000014090D935: FF 15 6D ED 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090D93B: 48 8D 15 4A D1 EA  lea         rdx,[00000001427BAA8Ch]
                    01
  000000014090D942: 48 8D 8C 24 A8 08  lea         rcx,[rsp+00000000000008A8h]
                    00 00
  000000014090D94A: FF 15 30 6B 4F 01  call        qword ptr [0000000141E04480h]
  000000014090D950: 90                 nop
  000000014090D951: 48 8D 94 24 98 08  lea         rdx,[rsp+0000000000000898h]
                    00 00
  000000014090D959: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090D961: E8 6A 9B 00 00     call        00000001409174D0
  000000014090D966: 48 89 84 24 18 0C  mov         qword ptr [rsp+0000000000000C18h],rax
                    00 00
  000000014090D96E: 48 8B 84 24 18 0C  mov         rax,qword ptr [rsp+0000000000000C18h]
                    00 00
  000000014090D976: 48 89 84 24 20 0C  mov         qword ptr [rsp+0000000000000C20h],rax
                    00 00
  000000014090D97E: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090D982: 48 8B 8C 24 20 0C  mov         rcx,qword ptr [rsp+0000000000000C20h]
                    00 00
  000000014090D98A: E8 31 77 71 FF     call        00000001400250C0
  000000014090D98F: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000014090D995: 8B 10              mov         edx,dword ptr [rax]
  000000014090D997: 48 8D 8C 24 A0 08  lea         rcx,[rsp+00000000000008A0h]
                    00 00
  000000014090D99F: FF 15 1B 64 4F 01  call        qword ptr [0000000141E03DC0h]
  000000014090D9A5: 48 89 84 24 28 0C  mov         qword ptr [rsp+0000000000000C28h],rax
                    00 00
  000000014090D9AD: 48 8B 84 24 28 0C  mov         rax,qword ptr [rsp+0000000000000C28h]
                    00 00
  000000014090D9B5: 48 89 84 24 30 0C  mov         qword ptr [rsp+0000000000000C30h],rax
                    00 00
  000000014090D9BD: 4C 8B 84 24 30 0C  mov         r8,qword ptr [rsp+0000000000000C30h]
                    00 00
  000000014090D9C5: 48 8D 94 24 A8 08  lea         rdx,[rsp+00000000000008A8h]
                    00 00
  000000014090D9CD: 48 8D 8C 24 48 03  lea         rcx,[rsp+0000000000000348h]
                    00 00
  000000014090D9D5: FF 15 3D EC 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090D9DB: 90                 nop
  000000014090D9DC: 48 8D 8C 24 A0 08  lea         rcx,[rsp+00000000000008A0h]
                    00 00
  000000014090D9E4: FF 15 F6 7F 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090D9EA: 90                 nop
  000000014090D9EB: 48 8D 8C 24 98 08  lea         rcx,[rsp+0000000000000898h]
                    00 00
  000000014090D9F3: E8 A8 A7 8F FF     call        00000001402081A0
  000000014090D9F8: 90                 nop
  000000014090D9F9: 48 8D 8C 24 A8 08  lea         rcx,[rsp+00000000000008A8h]
                    00 00
  000000014090DA01: FF 15 D9 7F 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DA07: 48 8D 94 24 B0 08  lea         rdx,[rsp+00000000000008B0h]
                    00 00
  000000014090DA0F: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090DA17: E8 B4 9A 00 00     call        00000001409174D0
  000000014090DA1C: 48 89 84 24 38 0C  mov         qword ptr [rsp+0000000000000C38h],rax
                    00 00
  000000014090DA24: 48 8B 84 24 38 0C  mov         rax,qword ptr [rsp+0000000000000C38h]
                    00 00
  000000014090DA2C: 48 89 84 24 40 0C  mov         qword ptr [rsp+0000000000000C40h],rax
                    00 00
  000000014090DA34: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090DA38: 48 8B 8C 24 40 0C  mov         rcx,qword ptr [rsp+0000000000000C40h]
                    00 00
  000000014090DA40: E8 7B 76 71 FF     call        00000001400250C0
  000000014090DA45: 48 8B D0           mov         rdx,rax
  000000014090DA48: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090DA50: E8 8B 99 00 00     call        00000001409173E0
  000000014090DA55: 48 8B D0           mov         rdx,rax
  000000014090DA58: 48 8D 8C 24 50 03  lea         rcx,[rsp+0000000000000350h]
                    00 00
  000000014090DA60: E8 1B 70 6F FF     call        0000000140004A80
  000000014090DA65: 90                 nop
  000000014090DA66: 48 8D 8C 24 B0 08  lea         rcx,[rsp+00000000000008B0h]
                    00 00
  000000014090DA6E: E8 2D A7 8F FF     call        00000001402081A0
  000000014090DA73: 48 8D 8C 24 50 03  lea         rcx,[rsp+0000000000000350h]
                    00 00
  000000014090DA7B: E8 60 70 6F FF     call        0000000140004AE0
  000000014090DA80: 89 84 24 40 03 00  mov         dword ptr [rsp+0000000000000340h],eax
                    00
  000000014090DA87: C7 84 24 58 03 00  mov         dword ptr [rsp+0000000000000358h],0
                    00 00 00 00 00
  000000014090DA92: EB 11              jmp         000000014090DAA5
  000000014090DA94: 8B 84 24 58 03 00  mov         eax,dword ptr [rsp+0000000000000358h]
                    00
  000000014090DA9B: 83 C0 01           add         eax,1
  000000014090DA9E: 89 84 24 58 03 00  mov         dword ptr [rsp+0000000000000358h],eax
                    00
  000000014090DAA5: 8B 84 24 40 03 00  mov         eax,dword ptr [rsp+0000000000000340h]
                    00
  000000014090DAAC: 39 84 24 58 03 00  cmp         dword ptr [rsp+0000000000000358h],eax
                    00
  000000014090DAB3: 0F 8D D2 00 00 00  jge         000000014090DB8B
  000000014090DAB9: 48 8D 15 D0 D8 EA  lea         rdx,[00000001427BB390h]
                    01
  000000014090DAC0: 48 8D 8C 24 B8 08  lea         rcx,[rsp+00000000000008B8h]
                    00 00
  000000014090DAC8: FF 15 B2 69 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DACE: 90                 nop
  000000014090DACF: 4C 8D 84 24 B8 08  lea         r8,[rsp+00000000000008B8h]
                    00 00
  000000014090DAD7: 48 8D 94 24 60 03  lea         rdx,[rsp+0000000000000360h]
                    00 00
  000000014090DADF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090DAE4: FF 15 9E EB 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090DAEA: 90                 nop
  000000014090DAEB: 48 8D 8C 24 B8 08  lea         rcx,[rsp+00000000000008B8h]
                    00 00
  000000014090DAF3: FF 15 E7 7E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DAF9: 4C 8D 84 24 60 03  lea         r8,[rsp+0000000000000360h]
                    00 00
  000000014090DB01: 48 8D 94 24 C0 08  lea         rdx,[rsp+00000000000008C0h]
                    00 00
  000000014090DB09: 48 8D 8C 24 48 03  lea         rcx,[rsp+0000000000000348h]
                    00 00
  000000014090DB11: FF 15 89 EB 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090DB17: 48 8D 8C 24 C0 08  lea         rcx,[rsp+00000000000008C0h]
                    00 00
  000000014090DB1F: FF 15 83 EB 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090DB25: 48 8D 15 74 D8 EA  lea         rdx,[00000001427BB3A0h]
                    01
  000000014090DB2C: 48 8D 8C 24 C8 08  lea         rcx,[rsp+00000000000008C8h]
                    00 00
  000000014090DB34: FF 15 46 69 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DB3A: 90                 nop
  000000014090DB3B: 8B 94 24 58 03 00  mov         edx,dword ptr [rsp+0000000000000358h]
                    00
  000000014090DB42: 48 8D 8C 24 50 03  lea         rcx,[rsp+0000000000000350h]
                    00 00
  000000014090DB4A: E8 F1 6F 6F FF     call        0000000140004B40
  000000014090DB4F: 4C 8B C0           mov         r8,rax
  000000014090DB52: 48 8D 94 24 C8 08  lea         rdx,[rsp+00000000000008C8h]
                    00 00
  000000014090DB5A: 48 8D 8C 24 60 03  lea         rcx,[rsp+0000000000000360h]
                    00 00
  000000014090DB62: FF 15 B0 EA 4F 01  call        qword ptr [0000000141E0C618h]
  000000014090DB68: 90                 nop
  000000014090DB69: 48 8D 8C 24 C8 08  lea         rcx,[rsp+00000000000008C8h]
                    00 00
  000000014090DB71: FF 15 69 7E 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DB77: 90                 nop
  000000014090DB78: 48 8D 8C 24 60 03  lea         rcx,[rsp+0000000000000360h]
                    00 00
  000000014090DB80: FF 15 2A EB 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090DB86: E9 09 FF FF FF     jmp         000000014090DA94
  000000014090DB8B: 48 8D 8C 24 50 03  lea         rcx,[rsp+0000000000000350h]
                    00 00
  000000014090DB93: E8 68 6F 6F FF     call        0000000140004B00
  000000014090DB98: 90                 nop
  000000014090DB99: 48 8D 8C 24 48 03  lea         rcx,[rsp+0000000000000348h]
                    00 00
  000000014090DBA1: FF 15 09 EB 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090DBA7: E9 07 FD FF FF     jmp         000000014090D8B3
  000000014090DBAC: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DBB4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090DBB7: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DBBF: FF 50 28           call        qword ptr [rax+28h]
  000000014090DBC2: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  000000014090DBC6: 83 7C 24 58 00     cmp         dword ptr [rsp+58h],0
  000000014090DBCB: 0F 8E B0 02 00 00  jle         000000014090DE81
  000000014090DBD1: 48 8D 15 D0 D7 EA  lea         rdx,[00000001427BB3A8h]
                    01
  000000014090DBD8: 48 8D 8C 24 D0 08  lea         rcx,[rsp+00000000000008D0h]
                    00 00
  000000014090DBE0: FF 15 9A 68 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DBE6: 90                 nop
  000000014090DBE7: 4C 8D 84 24 D0 08  lea         r8,[rsp+00000000000008D0h]
                    00 00
  000000014090DBEF: 48 8D 94 24 68 03  lea         rdx,[rsp+0000000000000368h]
                    00 00
  000000014090DBF7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090DBFC: FF 15 86 EA 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090DC02: 90                 nop
  000000014090DC03: 48 8D 8C 24 D0 08  lea         rcx,[rsp+00000000000008D0h]
                    00 00
  000000014090DC0B: FF 15 CF 7D 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DC11: 4C 8D 84 24 68 03  lea         r8,[rsp+0000000000000368h]
                    00 00
  000000014090DC19: 48 8D 94 24 D8 08  lea         rdx,[rsp+00000000000008D8h]
                    00 00
  000000014090DC21: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090DC29: FF 15 71 EA 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090DC2F: 48 8D 8C 24 D8 08  lea         rcx,[rsp+00000000000008D8h]
                    00 00
  000000014090DC37: FF 15 6B EA 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090DC3D: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  000000014090DC45: EB 0B              jmp         000000014090DC52
  000000014090DC47: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  000000014090DC4B: 83 C0 01           add         eax,1
  000000014090DC4E: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  000000014090DC52: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  000000014090DC56: 39 44 24 68        cmp         dword ptr [rsp+68h],eax
  000000014090DC5A: 0F 8D 13 02 00 00  jge         000000014090DE73
  000000014090DC60: 48 8D 15 51 D7 EA  lea         rdx,[00000001427BB3B8h]
                    01
  000000014090DC67: 48 8D 8C 24 E0 08  lea         rcx,[rsp+00000000000008E0h]
                    00 00
  000000014090DC6F: FF 15 0B 68 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DC75: 90                 nop
  000000014090DC76: 4C 8D 84 24 E0 08  lea         r8,[rsp+00000000000008E0h]
                    00 00
  000000014090DC7E: 48 8D 94 24 70 03  lea         rdx,[rsp+0000000000000370h]
                    00 00
  000000014090DC86: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090DC8B: FF 15 F7 E9 4F 01  call        qword ptr [0000000141E0C688h]
  000000014090DC91: 90                 nop
  000000014090DC92: 48 8D 8C 24 E0 08  lea         rcx,[rsp+00000000000008E0h]
                    00 00
  000000014090DC9A: FF 15 40 7D 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DCA0: 4C 8D 84 24 70 03  lea         r8,[rsp+0000000000000370h]
                    00 00
  000000014090DCA8: 48 8D 94 24 E8 08  lea         rdx,[rsp+00000000000008E8h]
                    00 00
  000000014090DCB0: 48 8D 8C 24 68 03  lea         rcx,[rsp+0000000000000368h]
                    00 00
  000000014090DCB8: FF 15 E2 E9 4F 01  call        qword ptr [0000000141E0C6A0h]
  000000014090DCBE: 48 8D 8C 24 E8 08  lea         rcx,[rsp+00000000000008E8h]
                    00 00
  000000014090DCC6: FF 15 DC E9 4F 01  call        qword ptr [0000000141E0C6A8h]
  000000014090DCCC: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DCD4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090DCD7: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090DCDB: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DCE3: FF 50 40           call        qword ptr [rax+40h]
  000000014090DCE6: 89 84 24 88 03 00  mov         dword ptr [rsp+0000000000000388h],eax
                    00
  000000014090DCED: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DCF5: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090DCF8: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090DCFC: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DD04: FF 50 50           call        qword ptr [rax+50h]
  000000014090DD07: F2 0F 11 84 24 80  movsd       mmword ptr [rsp+0000000000000380h],xmm0
                    03 00 00
  000000014090DD10: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DD18: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090DD1B: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090DD1F: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DD27: FF 50 70           call        qword ptr [rax+70h]
  000000014090DD2A: 89 84 24 78 03 00  mov         dword ptr [rsp+0000000000000378h],eax
                    00
  000000014090DD31: 48 8B 84 24 D8 0C  mov         rax,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DD39: 48 8B 00           mov         rax,qword ptr [rax]
  000000014090DD3C: 8B 54 24 68        mov         edx,dword ptr [rsp+68h]
  000000014090DD40: 48 8B 8C 24 D8 0C  mov         rcx,qword ptr [rsp+0000000000000CD8h]
                    00 00
  000000014090DD48: FF 50 60           call        qword ptr [rax+60h]
  000000014090DD4B: 89 84 24 7C 03 00  mov         dword ptr [rsp+000000000000037Ch],eax
                    00
  000000014090DD52: 48 8D 15 6B D3 EA  lea         rdx,[00000001427BB0C4h]
                    01
  000000014090DD59: 48 8D 8C 24 F0 08  lea         rcx,[rsp+00000000000008F0h]
                    00 00
  000000014090DD61: FF 15 19 67 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DD67: 90                 nop
  000000014090DD68: 44 8B 84 24 88 03  mov         r8d,dword ptr [rsp+0000000000000388h]
                    00 00
  000000014090DD70: 48 8D 94 24 F0 08  lea         rdx,[rsp+00000000000008F0h]
                    00 00
  000000014090DD78: 48 8D 8C 24 70 03  lea         rcx,[rsp+0000000000000370h]
                    00 00
  000000014090DD80: FF 15 32 E9 4F 01  call        qword ptr [0000000141E0C6B8h]
  000000014090DD86: 90                 nop
  000000014090DD87: 48 8D 8C 24 F0 08  lea         rcx,[rsp+00000000000008F0h]
                    00 00
  000000014090DD8F: FF 15 4B 7C 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DD95: 48 8D 15 28 D6 EA  lea         rdx,[00000001427BB3C4h]
                    01
  000000014090DD9C: 48 8D 8C 24 F8 08  lea         rcx,[rsp+00000000000008F8h]
                    00 00
  000000014090DDA4: FF 15 D6 66 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DDAA: 90                 nop
  000000014090DDAB: F2 0F 10 94 24 80  movsd       xmm2,mmword ptr [rsp+0000000000000380h]
                    03 00 00
  000000014090DDB4: 48 8D 94 24 F8 08  lea         rdx,[rsp+00000000000008F8h]
                    00 00
  000000014090DDBC: 48 8D 8C 24 70 03  lea         rcx,[rsp+0000000000000370h]
                    00 00
  000000014090DDC4: FF 15 BE E6 4F 01  call        qword ptr [0000000141E0C488h]
  000000014090DDCA: 90                 nop
  000000014090DDCB: 48 8D 8C 24 F8 08  lea         rcx,[rsp+00000000000008F8h]
                    00 00
  000000014090DDD3: FF 15 07 7C 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DDD9: 48 8D 15 F0 D5 EA  lea         rdx,[00000001427BB3D0h]
                    01
  000000014090DDE0: 48 8D 8C 24 00 09  lea         rcx,[rsp+0000000000000900h]
                    00 00
  000000014090DDE8: FF 15 92 66 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DDEE: 90                 nop
  000000014090DDEF: 44 8B 84 24 78 03  mov         r8d,dword ptr [rsp+0000000000000378h]
                    00 00
  000000014090DDF7: 48 8D 94 24 00 09  lea         rdx,[rsp+0000000000000900h]
                    00 00
  000000014090DDFF: 48 8D 8C 24 70 03  lea         rcx,[rsp+0000000000000370h]
                    00 00
  000000014090DE07: FF 15 AB E8 4F 01  call        qword ptr [0000000141E0C6B8h]
  000000014090DE0D: 90                 nop
  000000014090DE0E: 48 8D 8C 24 00 09  lea         rcx,[rsp+0000000000000900h]
                    00 00
  000000014090DE16: FF 15 C4 7B 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DE1C: 48 8D 15 BD D5 EA  lea         rdx,[00000001427BB3E0h]
                    01
  000000014090DE23: 48 8D 8C 24 08 09  lea         rcx,[rsp+0000000000000908h]
                    00 00
  000000014090DE2B: FF 15 4F 66 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DE31: 90                 nop
  000000014090DE32: 44 8B 84 24 7C 03  mov         r8d,dword ptr [rsp+000000000000037Ch]
                    00 00
  000000014090DE3A: 48 8D 94 24 08 09  lea         rdx,[rsp+0000000000000908h]
                    00 00
  000000014090DE42: 48 8D 8C 24 70 03  lea         rcx,[rsp+0000000000000370h]
                    00 00
  000000014090DE4A: FF 15 68 E8 4F 01  call        qword ptr [0000000141E0C6B8h]
  000000014090DE50: 90                 nop
  000000014090DE51: 48 8D 8C 24 08 09  lea         rcx,[rsp+0000000000000908h]
                    00 00
  000000014090DE59: FF 15 81 7B 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DE5F: 90                 nop
  000000014090DE60: 48 8D 8C 24 70 03  lea         rcx,[rsp+0000000000000370h]
                    00 00
  000000014090DE68: FF 15 42 E8 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090DE6E: E9 D4 FD FF FF     jmp         000000014090DC47
  000000014090DE73: 48 8D 8C 24 68 03  lea         rcx,[rsp+0000000000000368h]
                    00 00
  000000014090DE7B: FF 15 2F E8 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090DE81: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  000000014090DE89: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014090DE91: FF 15 41 5F 4F 01  call        qword ptr [0000000141E03DD8h]
  000000014090DE97: 90                 nop
  000000014090DE98: 41 B8 01 00 00 00  mov         r8d,1
  000000014090DE9E: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014090DEA3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014090DEA8: FF 15 62 E7 4F 01  call        qword ptr [0000000141E0C610h]
  000000014090DEAE: 90                 nop
  000000014090DEAF: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014090DEB4: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014090DEBC: FF 15 2E 5F 4F 01  call        qword ptr [0000000141E03DF0h]
  000000014090DEC2: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014090DECA: FF 15 48 61 4F 01  call        qword ptr [0000000141E04018h]
  000000014090DED0: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014090DED8: FF 15 D2 68 4F 01  call        qword ptr [0000000141E047B0h]
  000000014090DEDE: 85 C0              test        eax,eax
  000000014090DEE0: 0F 84 C8 00 00 00  je          000000014090DFAE
  000000014090DEE6: B2 20              mov         dl,20h
  000000014090DEE8: 48 8D 8C 24 1A 09  lea         rcx,[rsp+000000000000091Ah]
                    00 00
  000000014090DEF0: E8 6B 49 6F FF     call        0000000140002860
  000000014090DEF5: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014090DEF8: 48 8D 8C 24 18 09  lea         rcx,[rsp+0000000000000918h]
                    00 00
  000000014090DF00: FF 15 52 64 4F 01  call        qword ptr [0000000141E04358h]
  000000014090DF06: 48 8D 15 E3 D4 EA  lea         rdx,[00000001427BB3F0h]
                    01
  000000014090DF0D: 48 8D 8C 24 10 09  lea         rcx,[rsp+0000000000000910h]
                    00 00
  000000014090DF15: FF 15 65 65 4F 01  call        qword ptr [0000000141E04480h]
  000000014090DF1B: 48 89 84 24 48 0C  mov         qword ptr [rsp+0000000000000C48h],rax
                    00 00
  000000014090DF23: 48 8B 84 24 48 0C  mov         rax,qword ptr [rsp+0000000000000C48h]
                    00 00
  000000014090DF2B: 48 89 84 24 50 0C  mov         qword ptr [rsp+0000000000000C50h],rax
                    00 00
  000000014090DF33: 48 8D 84 24 18 09  lea         rax,[rsp+0000000000000918h]
                    00 00
  000000014090DF3B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014090DF40: 45 33 C9           xor         r9d,r9d
  000000014090DF43: 4C 8B 84 24 D0 0C  mov         r8,qword ptr [rsp+0000000000000CD0h]
                    00 00
  000000014090DF4B: 48 8D 94 24 20 09  lea         rdx,[rsp+0000000000000920h]
                    00 00
  000000014090DF53: 48 8B 8C 24 50 0C  mov         rcx,qword ptr [rsp+0000000000000C50h]
                    00 00
  000000014090DF5B: FF 15 FF 63 4F 01  call        qword ptr [0000000141E04360h]
  000000014090DF61: 48 89 84 24 58 0C  mov         qword ptr [rsp+0000000000000C58h],rax
                    00 00
  000000014090DF69: 48 8B 84 24 58 0C  mov         rax,qword ptr [rsp+0000000000000C58h]
                    00 00
  000000014090DF71: 48 89 84 24 60 0C  mov         qword ptr [rsp+0000000000000C60h],rax
                    00 00
  000000014090DF79: 48 8B 94 24 60 0C  mov         rdx,qword ptr [rsp+0000000000000C60h]
                    00 00
  000000014090DF81: 48 8D 0D A8 BA 4C  lea         rcx,[0000000143DD9A30h]
                    03
  000000014090DF88: FF 15 E2 64 4F 01  call        qword ptr [0000000141E04470h]
  000000014090DF8E: 90                 nop
  000000014090DF8F: 48 8D 8C 24 20 09  lea         rcx,[rsp+0000000000000920h]
                    00 00
  000000014090DF97: FF 15 43 7A 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DF9D: 90                 nop
  000000014090DF9E: 48 8D 8C 24 10 09  lea         rcx,[rsp+0000000000000910h]
                    00 00
  000000014090DFA6: FF 15 34 7A 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DFAC: EB 05              jmp         000000014090DFB3
  000000014090DFAE: C6 44 24 78 01     mov         byte ptr [rsp+78h],1
  000000014090DFB3: 0F B6 44 24 78     movzx       eax,byte ptr [rsp+78h]
  000000014090DFB8: 88 84 24 28 09 00  mov         byte ptr [rsp+0000000000000928h],al
                    00
  000000014090DFBF: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014090DFC4: FF 15 16 7A 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090DFCA: 90                 nop
  000000014090DFCB: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014090DFD3: FF 15 1F 5E 4F 01  call        qword ptr [0000000141E03DF8h]
  000000014090DFD9: 90                 nop
  000000014090DFDA: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014090DFE2: FF 15 C8 E6 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090DFE8: 90                 nop
  000000014090DFE9: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014090DFEE: E8 AD A1 8F FF     call        00000001402081A0
  000000014090DFF3: 90                 nop
  000000014090DFF4: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014090DFFC: E8 0F 92 00 00     call        0000000140917210
  000000014090E001: 90                 nop
  000000014090E002: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014090E007: FF 15 A3 E6 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090E00D: 90                 nop
  000000014090E00E: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014090E013: FF 15 97 E6 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090E019: 90                 nop
  000000014090E01A: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014090E022: FF 15 88 E6 4F 01  call        qword ptr [0000000141E0C6B0h]
  000000014090E028: 90                 nop
  000000014090E029: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014090E031: FF 15 E9 5F 4F 01  call        qword ptr [0000000141E04020h]
  000000014090E037: 90                 nop
  000000014090E038: 48 8B 8C 24 D0 0C  mov         rcx,qword ptr [rsp+0000000000000CD0h]
                    00 00
  000000014090E040: FF 15 9A 79 4F 01  call        qword ptr [0000000141E059E0h]
  000000014090E046: 8A 84 24 28 09 00  mov         al,byte ptr [rsp+0000000000000928h]
                    00
  000000014090E04D: 0F 28 B4 24 A0 0C  movaps      xmm6,xmmword ptr [rsp+0000000000000CA0h]
                    00 00
  000000014090E055: 0F 28 BC 24 90 0C  movaps      xmm7,xmmword ptr [rsp+0000000000000C90h]
                    00 00
  000000014090E05D: 44 0F 28 84 24 80  movaps      xmm8,xmmword ptr [rsp+0000000000000C80h]
                    0C 00 00
  000000014090E066: 44 0F 28 8C 24 70  movaps      xmm9,xmmword ptr [rsp+0000000000000C70h]
                    0C 00 00
  000000014090E06F: 48 81 C4 B8 0C 00  add         rsp,0CB8h
                    00
  000000014090E076: 5F                 pop         rdi
  000000014090E077: 5E                 pop         rsi
  000000014090E078: C3                 ret

  Summary

     1E02000 .text
