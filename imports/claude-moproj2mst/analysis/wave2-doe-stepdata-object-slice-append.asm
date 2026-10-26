
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411FE310: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  00000001411FE315: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411FE31A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411FE31F: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  00000001411FE326: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411FE32E: E8 DD 36 FF FF     call        00000001411F1A10
  00000001411FE333: 48 85 C0           test        rax,rax
  00000001411FE336: 74 2F              je          00000001411FE367
  00000001411FE338: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00
  00000001411FE343: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411FE34B: E8 C0 36 FF FF     call        00000001411F1A10
  00000001411FE350: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000001411FE358: 48 8B C8           mov         rcx,rax
  00000001411FE35B: E8 E0 06 FF FF     call        00000001411EEA40
  00000001411FE360: 0F B6 C0           movzx       eax,al
  00000001411FE363: 85 C0              test        eax,eax
  00000001411FE365: 75 0C              jne         00000001411FE373
  00000001411FE367: 0F B6 84 24 B0 00  movzx       eax,byte ptr [rsp+00000000000000B0h]
                    00 00
  00000001411FE36F: 85 C0              test        eax,eax
  00000001411FE371: 74 6E              je          00000001411FE3E1
  00000001411FE373: C6 44 24 58 00     mov         byte ptr [rsp+58h],0
  00000001411FE378: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001411FE380: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411FE388: E8 63 00 00 00     call        00000001411FE3F0
  00000001411FE38D: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001411FE392: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  00000001411FE398: 74 47              je          00000001411FE3E1
  00000001411FE39A: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411FE3A2: E8 D9 35 FF FF     call        00000001411F1980
  00000001411FE3A7: 48 8B D0           mov         rdx,rax
  00000001411FE3AA: C6 44 24 30 01     mov         byte ptr [rsp+30h],1
  00000001411FE3AF: 0F B6 44 24 58     movzx       eax,byte ptr [rsp+58h]
  00000001411FE3B4: 88 44 24 28        mov         byte ptr [rsp+28h],al
  00000001411FE3B8: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001411FE3C0: 4C 8B 4C 24 60     mov         r9,qword ptr [rsp+60h]
  00000001411FE3C5: 4C 8B 84 24 A0 00  mov         r8,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411FE3CD: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411FE3D5: 48 8B 89 38 03 00  mov         rcx,qword ptr [rcx+0000000000000338h]
                    00
  00000001411FE3DC: E8 3F A1 BF FF     call        0000000140DF8520
  00000001411FE3E1: 48 81 C4 98 00 00  add         rsp,98h
                    00
  00000001411FE3E8: C3                 ret

  Summary

     1E02000 .text
