
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140025840: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140025845: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014002584A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014002584F: 48 83 EC 38        sub         rsp,38h
  0000000140025853: 33 C0              xor         eax,eax
  0000000140025855: 85 C0              test        eax,eax
  0000000140025857: 74 35              je          000000014002588E
  0000000140025859: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014002585E: 48 39 44 24 48     cmp         qword ptr [rsp+48h],rax
  0000000140025863: 74 27              je          000000014002588C
  0000000140025865: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014002586A: 48 83 E8 08        sub         rax,8
  000000014002586E: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140025873: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140025878: 48 8B 00           mov         rax,qword ptr [rax]
  000000014002587B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140025880: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140025885: E8 DE A3 4B 01     call        00000001414DFC68
  000000014002588A: EB CD              jmp         0000000140025859
  000000014002588C: EB 22              jmp         00000001400258B0
  000000014002588E: 33 C0              xor         eax,eax
  0000000140025890: 85 C0              test        eax,eax
  0000000140025892: 74 1C              je          00000001400258B0
  0000000140025894: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140025899: 48 39 44 24 48     cmp         qword ptr [rsp+48h],rax
  000000014002589E: 74 10              je          00000001400258B0
  00000001400258A0: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400258A5: 48 83 E8 08        sub         rax,8
  00000001400258A9: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400258AE: EB E4              jmp         0000000140025894
  00000001400258B0: 48 83 C4 38        add         rsp,38h
  00000001400258B4: C3                 ret

  Summary

     1E02000 .text
