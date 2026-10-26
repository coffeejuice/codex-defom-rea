
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C82B70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C82B75: 48 83 EC 48        sub         rsp,48h
  0000000140C82B79: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C82B7E: 48 83 C1 18        add         rcx,18h
  0000000140C82B82: E8 89 48 68 FF     call        0000000140307410
  0000000140C82B87: 0F B6 C0           movzx       eax,al
  0000000140C82B8A: 85 C0              test        eax,eax
  0000000140C82B8C: 74 02              je          0000000140C82B90
  0000000140C82B8E: EB 68              jmp         0000000140C82BF8
  0000000140C82B90: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C82B95: 48 83 C1 18        add         rcx,18h
  0000000140C82B99: E8 02 F3 00 00     call        0000000140C91EA0
  0000000140C82B9E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C82BA3: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140C82BA9: 74 4D              je          0000000140C82BF8
  0000000140C82BAB: B2 01              mov         dl,1
  0000000140C82BAD: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C82BB2: E8 99 DF 56 00     call        00000001411F0B50
  0000000140C82BB7: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C82BBC: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C82BC1: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C82BC6: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C82BCB: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140C82BD1: 74 1C              je          0000000140C82BEF
  0000000140C82BD3: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C82BD8: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C82BDB: BA 01 00 00 00     mov         edx,1
  0000000140C82BE0: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140C82BE5: FF 50 18           call        qword ptr [rax+18h]
  0000000140C82BE8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C82BED: EB 09              jmp         0000000140C82BF8
  0000000140C82BEF: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140C82BF8: 48 83 C4 48        add         rsp,48h
  0000000140C82BFC: C3                 ret

  Summary

     1E02000 .text
