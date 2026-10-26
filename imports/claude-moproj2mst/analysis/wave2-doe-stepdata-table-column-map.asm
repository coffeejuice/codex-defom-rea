
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D7B3F0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140D7B3F4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7B3F9: 48 83 EC 18        sub         rsp,18h
  0000000140D7B3FD: C7 04 24 FF FF FF  mov         dword ptr [rsp],0FFFFFFFFh
                    FF
  0000000140D7B404: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140D7B408: 89 44 24 04        mov         dword ptr [rsp+4],eax
  0000000140D7B40C: 8B 44 24 04        mov         eax,dword ptr [rsp+4]
  0000000140D7B410: 83 E8 01           sub         eax,1
  0000000140D7B413: 89 44 24 04        mov         dword ptr [rsp+4],eax
  0000000140D7B417: 83 7C 24 04 05     cmp         dword ptr [rsp+4],5
  0000000140D7B41C: 77 4C              ja          0000000140D7B46A
  0000000140D7B41E: 48 63 44 24 04     movsxd      rax,dword ptr [rsp+4]
  0000000140D7B423: 48 8D 0D D6 4B 28  lea         rcx,[0000000140000000h]
                    FF
  0000000140D7B42A: 8B 84 81 74 B4 D7  mov         eax,dword ptr [rcx+rax*4+0000000000D7B474h]
                    00
  0000000140D7B431: 48 03 C1           add         rax,rcx
  0000000140D7B434: FF E0              jmp         rax
  0000000140D7B436: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000140D7B43D: EB 2B              jmp         0000000140D7B46A
  0000000140D7B43F: C7 04 24 01 00 00  mov         dword ptr [rsp],1
                    00
  0000000140D7B446: EB 22              jmp         0000000140D7B46A
  0000000140D7B448: C7 04 24 02 00 00  mov         dword ptr [rsp],2
                    00
  0000000140D7B44F: EB 19              jmp         0000000140D7B46A
  0000000140D7B451: C7 04 24 03 00 00  mov         dword ptr [rsp],3
                    00
  0000000140D7B458: EB 10              jmp         0000000140D7B46A
  0000000140D7B45A: C7 04 24 04 00 00  mov         dword ptr [rsp],4
                    00
  0000000140D7B461: EB 07              jmp         0000000140D7B46A
  0000000140D7B463: C7 04 24 05 00 00  mov         dword ptr [rsp],5
                    00
  0000000140D7B46A: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000140D7B46D: 48 83 C4 18        add         rsp,18h
  0000000140D7B471: C3                 ret
  0000000140D7B472: 66 90              nop
  0000000140D7B474: 36 B4 D7           mov         ah,0D7h
  0000000140D7B477: 00 3F              add         byte ptr [rdi],bh
  0000000140D7B479: B4 D7              mov         ah,0D7h
  0000000140D7B47B: 00 48 B4           add         byte ptr [rax-4Ch],cl
  0000000140D7B47E: D7                 xlat        byte ptr [rbx]
  0000000140D7B47F: 00 51 B4           add         byte ptr [rcx-4Ch],dl
  0000000140D7B482: D7                 xlat        byte ptr [rbx]
  0000000140D7B483: 00 5A B4           add         byte ptr [rdx-4Ch],bl
  0000000140D7B486: D7                 xlat        byte ptr [rbx]
  0000000140D7B487: 00 63 B4           add         byte ptr [rbx-4Ch],ah
  0000000140D7B48A: D7                 xlat        byte ptr [rbx]
  0000000140D7B48B: 00

  Summary

     1E02000 .text
