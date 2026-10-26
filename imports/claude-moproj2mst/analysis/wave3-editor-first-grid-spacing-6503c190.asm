Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006503C190: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006503C194: 48 83 C1 10        add         rcx,10h
  000000006503C198: 89 90 20 01 00 00  mov         dword ptr [rax+0000000000000120h],edx
  000000006503C19E: 89 90 1C 01 00 00  mov         dword ptr [rax+000000000000011Ch],edx
  000000006503C1A4: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006503C1A7: 48 FF 60 58        jmp         qword ptr [rax+58h]
  000000006503C1AB: CC                 int         3
  000000006503C1AC: CC                 int         3
  000000006503C1AD: CC                 int         3
  000000006503C1AE: CC                 int         3
  000000006503C1AF: CC                 int         3
  000000006503C1B0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006503C1B5: 57                 push        rdi
  000000006503C1B6: 48 83 EC 20        sub         rsp,20h
  000000006503C1BA: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006503C1BE: 48 8B F9           mov         rdi,rcx
  000000006503C1C1: 8B 98 1C 01 00 00  mov         ebx,dword ptr [rax+000000000000011Ch]
  000000006503C1C7: 85 DB              test        ebx,ebx
  000000006503C1C9: 79 0C              jns         000000006503C1D7
  000000006503C1CB: BA 54 00 00 00     mov         edx,54h
  000000006503C1D0: E8 6B AE 00 00     call        0000000065047040
  000000006503C1D5: 8B D8              mov         ebx,eax
  000000006503C1D7: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006503C1DB: 8B 91 20 01 00 00  mov         edx,dword ptr [rcx+0000000000000120h]
  000000006503C1E1: 85 D2              test        edx,edx
  000000006503C1E3: 79 0F              jns         000000006503C1F4
  000000006503C1E5: BA 55 00 00 00     mov         edx,55h
  000000006503C1EA: 48 8B CF           mov         rcx,rdi
  000000006503C1ED: E8
  000000006503C1EE: 4E AE              scas        byte ptr [rdi]

  Summary

      695000 .text
