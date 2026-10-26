
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014064F420: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014064F425: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014064F42A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014064F42F: 48 83 EC 38        sub         rsp,38h
  000000014064F433: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014064F43C: 33 D2              xor         edx,edx
  000000014064F43E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F443: FF 15 77 4D 7B 01  call        qword ptr [0000000141E041C0h]
  000000014064F449: 90                 nop
  000000014064F44A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F44F: 48 8D 05 D2 52 E9  lea         rax,[00000001424E4728h]
                    01
  000000014064F456: 48 89 01           mov         qword ptr [rcx],rax
  000000014064F459: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F45E: 48 83 C1 18        add         rcx,18h
  000000014064F462: FF 15 58 D1 7B 01  call        qword ptr [0000000141E0C5C0h]
  000000014064F468: 90                 nop
  000000014064F469: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F46E: 48 83 C1 30        add         rcx,30h
  000000014064F472: E8 69 05 B4 FF     call        000000014018F9E0
  000000014064F477: 90                 nop
  000000014064F478: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F47D: 48 83 C1 38        add         rcx,38h
  000000014064F481: E8 EA DC B0 FF     call        000000014015D170
  000000014064F486: 90                 nop
  000000014064F487: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F48C: E8 DF 83 00 00     call        0000000140657870
  000000014064F491: 48 85 C0           test        rax,rax
  000000014064F494: 74 2E              je          000000014064F4C4
  000000014064F496: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F49B: E8 D0 83 00 00     call        0000000140657870
  000000014064F4A0: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014064F4A5: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014064F4AA: 48 8B 00           mov         rax,qword ptr [rax]
  000000014064F4AD: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014064F4B2: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  000000014064F4B8: 4C 8B D8           mov         r11,rax
  000000014064F4BB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014064F4C0: 4C 89 58 20        mov         qword ptr [rax+20h],r11
  000000014064F4C4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F4C9: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014064F4CE: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000014064F4D2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014064F4D7: C7 40 2C FF FF FF  mov         dword ptr [rax+2Ch],0FFFFFFFFh
                    FF
  000000014064F4DE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014064F4E3: C6 40 28 00        mov         byte ptr [rax+28h],0
  000000014064F4E7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014064F4EC: C6 40 29 00        mov         byte ptr [rax+29h],0
  000000014064F4F0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014064F4F5: C6 40 2A 00        mov         byte ptr [rax+2Ah],0
  000000014064F4F9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014064F4FE: 48 83 C1 38        add         rcx,38h
  000000014064F502: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  000000014064F507: E8 14 D4 66 00     call        0000000140CBC920
  000000014064F50C: 90                 nop
  000000014064F50D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014064F512: 48 83 C4 38        add         rsp,38h
  000000014064F516: C3                 ret

  Summary

     1E02000 .text
