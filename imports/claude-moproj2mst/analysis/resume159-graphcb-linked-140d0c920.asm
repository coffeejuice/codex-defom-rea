Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D0C920: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D0C925: 48 83 EC 48        sub         rsp,48h
  0000000140D0C929: C6 44 24 30 00     mov         byte ptr [rsp+30h],0
  0000000140D0C92E: 0F B6 54 24 30     movzx       edx,byte ptr [rsp+30h]
  0000000140D0C933: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D0C938: E8 13 02 00 00     call        0000000140D0CB50
  0000000140D0C93D: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D0C942: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140D0C948: 75 04              jne         0000000140D0C94E
  0000000140D0C94A: 32 C0              xor         al,al
  0000000140D0C94C: EB 51              jmp         0000000140D0C99F
  0000000140D0C94E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D0C953: E8 08 D0 FE FF     call        0000000140CF9960
  0000000140D0C958: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D0C95D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D0C962: E8 89 CB FE FF     call        0000000140CF94F0
  0000000140D0C967: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D0C96C: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140D0C972: 74 08              je          0000000140D0C97C
  0000000140D0C974: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D0C97A: 75 04              jne         0000000140D0C980
  0000000140D0C97C: 32 C0              xor         al,al
  0000000140D0C97E: EB 1F              jmp         0000000140D0C99F
  0000000140D0C980: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D0C985: E8 56 F7 F6 FF     call        0000000140C7C0E0
  0000000140D0C98A: 0F B6 C0           movzx       eax,al
  0000000140D0C98D: 85 C0              test        eax,eax
  0000000140D0C98F: 75 04              jne         0000000140D0C995
  0000000140D0C991: 32 C0              xor         al,al
  0000000140D0C993: EB 0A              jmp         0000000140D0C99F
  0000000140D0C995: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D0C99A: E8 21 A0 03 00     call        0000000140D469C0
  0000000140D0C99F: 48 83 C4 48        add         rsp,48h
  0000000140D0C9A3: C3                 ret

  Summary

     1E02000 .text
