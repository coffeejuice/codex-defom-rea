
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653CCD80: 48 83 EC 28        sub         rsp,28h
  00000000653CCD84: 48 83 B9 F8 04 00  cmp         qword ptr [rcx+00000000000004F8h],0
                    00 00
  00000000653CCD8C: 74 22              je          00000000653CCDB0
  00000000653CCD8E: 48 8B 89 F8 04 00  mov         rcx,qword ptr [rcx+00000000000004F8h]
                    00
  00000000653CCD95: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000653CCD98: FF 50 60           call        qword ptr [rax+60h]
  00000000653CCD9B: 48 8D 0D D6 B9 38  lea         rcx,[0000000065758778h]
                    00
  00000000653CCDA2: 48 8B D0           mov         rdx,rax
  00000000653CCDA5: 48 83 C4 28        add         rsp,28h
  00000000653CCDA9: 48 FF 25 58 9C 2C  jmp         qword ptr [0000000065696A08h]
                    00
  00000000653CCDB0: 33 C0              xor         eax,eax
  00000000653CCDB2: 48 83 C4 28        add         rsp,28h
  00000000653CCDB6: C3                 ret

  Summary

      695000 .text
