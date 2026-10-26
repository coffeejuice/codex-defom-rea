Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001403D5780: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001403D5785: 48 83 EC 48        sub         rsp,48h
  00000001403D5789: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001403D578E: 48 83 B8 D0 00 00  cmp         qword ptr [rax+00000000000000D0h],0
                    00 00
  00000001403D5796: 74 57              je          00000001403D57EF
  00000001403D5798: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001403D579D: 48 8B 80 D0 00 00  mov         rax,qword ptr [rax+00000000000000D0h]
                    00
  00000001403D57A4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001403D57A9: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001403D57AE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001403D57B3: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001403D57B9: 74 1B              je          00000001403D57D6
  00000001403D57BB: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001403D57C0: 48 8B 00           mov         rax,qword ptr [rax]
  00000001403D57C3: BA 01 00 00 00     mov         edx,1
  00000001403D57C8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001403D57CD: FF 10              call        qword ptr [rax]
  00000001403D57CF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001403D57D4: EB 09              jmp         00000001403D57DF
  00000001403D57D6: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001403D57DF: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001403D57E4: 48 C7 80 D0 00 00  mov         qword ptr [rax+00000000000000D0h],0
                    00 00 00 00 00
  00000001403D57EF: 48 83 C4 48        add         rsp,48h
  00000001403D57F3: C3                 ret

  Summary

     1E02000 .text
