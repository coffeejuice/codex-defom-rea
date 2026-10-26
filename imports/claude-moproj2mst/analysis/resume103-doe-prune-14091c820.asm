Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091C820: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091C825: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  000000014091C82C: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014091C838: C6 44 24 30 01     mov         byte ptr [rsp+30h],1
  000000014091C83D: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C845: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091C848: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C850: FF 90 78 01 00 00  call        qword ptr [rax+0000000000000178h]
  000000014091C856: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  000000014091C85A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C85F: E8 7C 8B 74 FF     call        00000001400653E0
  000000014091C864: 90                 nop
  000000014091C865: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091C86D: EB 0B              jmp         000000014091C87A
  000000014091C86F: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091C873: 83 C0 01           add         eax,1
  000000014091C876: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091C87A: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  000000014091C87E: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091C882: 0F 8D C9 00 00 00  jge         000000014091C951
  000000014091C888: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C890: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091C893: 44 8B 44 24 20     mov         r8d,dword ptr [rsp+20h]
  000000014091C898: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000014091C89D: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C8A5: FF 90 90 01 00 00  call        qword ptr [rax+0000000000000190h]
  000000014091C8AB: 90                 nop
  000000014091C8AC: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C8B4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091C8B7: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091C8BB: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C8C3: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000014091C8C9: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014091C8CE: 48 8D 44 24 60     lea         rax,[rsp+60h]
  000000014091C8D3: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014091C8D8: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000014091C8DD: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  000000014091C8E2: FF 15 F0 90 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091C8E8: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000014091C8F0: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C8F8: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091C8FB: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014091C903: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C90B: FF 90 28 01 00 00  call        qword ptr [rax+0000000000000128h]
  000000014091C911: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000014091C915: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014091C91A: E8 21 AF EF FF     call        0000000140817840
  000000014091C91F: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000014091C923: 83 7C 24 40 01     cmp         dword ptr [rsp+40h],1
  000000014091C928: 7D 17              jge         000000014091C941
  000000014091C92A: 83 7C 24 50 01     cmp         dword ptr [rsp+50h],1
  000000014091C92F: 7D 10              jge         000000014091C941
  000000014091C931: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000014091C936: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C93B: E8 50 82 6E FF     call        0000000140004B90
  000000014091C940: 90                 nop
  000000014091C941: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014091C946: FF 15 94 90 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091C94C: E9 1E FF FF FF     jmp         000000014091C86F
  000000014091C951: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091C959: EB 0B              jmp         000000014091C966
  000000014091C95B: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091C95F: 83 C0 01           add         eax,1
  000000014091C962: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091C966: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C96B: E8 70 81 6E FF     call        0000000140004AE0
  000000014091C970: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091C974: 0F 8D D2 00 00 00  jge         000000014091CA4C
  000000014091C97A: 48 8D 44 24 70     lea         rax,[rsp+70h]
  000000014091C97F: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014091C984: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091C988: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C98D: E8 AE 81 6E FF     call        0000000140004B40
  000000014091C992: 48 8B D0           mov         rdx,rax
  000000014091C995: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000014091C99A: FF 15 38 90 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091C9A0: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  000000014091C9A8: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C9B0: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091C9B3: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014091C9BB: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C9C3: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091C9C9: 89 44 24 54        mov         dword ptr [rsp+54h],eax
  000000014091C9CD: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C9D5: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091C9D8: 8B 54 24 54        mov         edx,dword ptr [rsp+54h]
  000000014091C9DC: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091C9E4: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  000000014091C9EA: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014091C9EF: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  000000014091C9F7: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014091C9FF: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091CA03: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091CA08: E8 33 81 6E FF     call        0000000140004B40
  000000014091CA0D: 48 8B D0           mov         rdx,rax
  000000014091CA10: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091CA18: FF 15 BA 8F 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091CA1E: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  000000014091CA26: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091CA2E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091CA31: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091CA39: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091CA41: FF 90 00 01 00 00  call        qword ptr [rax+0000000000000100h]
  000000014091CA47: E9 0F FF FF FF     jmp         000000014091C95B
  000000014091CA4C: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  000000014091CA51: 88 84 24 90 00 00  mov         byte ptr [rsp+0000000000000090h],al
                    00
  000000014091CA58: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091CA5D: E8 6E 7E 6E FF     call        00000001400048D0
  000000014091CA62: 8A 84 24 90 00 00  mov         al,byte ptr [rsp+0000000000000090h]
                    00
  000000014091CA69: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  000000014091CA70: C3                 ret

  Summary

     1E02000 .text
