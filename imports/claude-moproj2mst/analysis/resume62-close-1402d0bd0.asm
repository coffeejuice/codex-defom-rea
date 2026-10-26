
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402D0BD0: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  00000001402D0BD5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001402D0BDA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402D0BDF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402D0BE4: 48 83 EC 48        sub         rsp,48h
  00000001402D0BE8: 48 8D 0D C9 5A AA  lea         rcx,[0000000143D766B8h]
                    03
  00000001402D0BEF: E8 9C 74 FF FF     call        00000001402C8090
  00000001402D0BF4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001402D0BF9: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001402D0BFF: 75 02              jne         00000001402D0C03
  00000001402D0C01: EB 68              jmp         00000001402D0C6B
  00000001402D0C03: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001402D0C08: FF 15 CA 7D B3 01  call        qword ptr [0000000141E089D8h]
  00000001402D0C0E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402D0C13: 83 7C 24 60 FF     cmp         dword ptr [rsp+60h],0FFFFFFFFh
  00000001402D0C18: 75 15              jne         00000001402D0C2F
  00000001402D0C1A: 45 33 C0           xor         r8d,r8d
  00000001402D0C1D: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001402D0C22: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001402D0C27: FF 15 A3 7D B3 01  call        qword ptr [0000000141E089D0h]
  00000001402D0C2D: EB 15              jmp         00000001402D0C44
  00000001402D0C2F: 44 8B 44 24 60     mov         r8d,dword ptr [rsp+60h]
  00000001402D0C34: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001402D0C39: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001402D0C3E: FF 15 8C 7D B3 01  call        qword ptr [0000000141E089D0h]
  00000001402D0C44: BA 02 00 00 00     mov         edx,2
  00000001402D0C49: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001402D0C4E: FF 15 64 B3 B3 01  call        qword ptr [0000000141E0BFB8h]
  00000001402D0C54: 4C 8D 4C 24 68     lea         r9,[rsp+68h]
  00000001402D0C59: 4C 8B C0           mov         r8,rax
  00000001402D0C5C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001402D0C61: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001402D0C66: E8 A5 02 00 00     call        00000001402D0F10
  00000001402D0C6B: 48 83 C4 48        add         rsp,48h
  00000001402D0C6F: C3                 ret

  Summary

     1E02000 .text
