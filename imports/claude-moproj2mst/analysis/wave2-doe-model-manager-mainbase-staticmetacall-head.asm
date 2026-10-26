
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CC5970: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140CC5975: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140CC597A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140CC597E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CC5983: 53                 push        rbx
  0000000140CC5984: 56                 push        rsi
  0000000140CC5985: 57                 push        rdi
  0000000140CC5986: 48 81 EC A0 02 00  sub         rsp,2A0h
                    00
  0000000140CC598D: 48 C7 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CC5999: 83 BC 24 C8 02 00  cmp         dword ptr [rsp+00000000000002C8h],0
                    00 00
  0000000140CC59A1: 0F 85 4F 18 00 00  jne         0000000140CC71F6
  0000000140CC59A7: E8 D4 F1 33 FF     call        0000000140004B80
  0000000140CC59AC: 48 8B 94 24 C0 02  mov         rdx,qword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140CC59B4: 48 89 54 24 50     mov         qword ptr [rsp+50h],rdx
  0000000140CC59B9: 8B 84 24 D0 02 00  mov         eax,dword ptr [rsp+00000000000002D0h]
                    00
  0000000140CC59C0: 89 84 24 D8 01 00  mov         dword ptr [rsp+00000000000001D8h],eax
                    00
  0000000140CC59C7: 81 BC 24 D8 01 00  cmp         dword ptr [rsp+00000000000001D8h],0B3h
                    00 B3 00 00 00
  0000000140CC59D2: 0F 87 1E 18 00 00  ja          0000000140CC71F6
  0000000140CC59D8: 48 63 84 24 D8 01  movsxd      rax,dword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140CC59E0: 48
  0000000140CC59E1: 8D
  0000000140CC59E2: 0D
  0000000140CC59E3: 19
  0000000140CC59E4: A6                 cmps        byte ptr [rsi],byte ptr [rdi]
  0000000140CC59E5: 33

  Summary

     1E02000 .text
