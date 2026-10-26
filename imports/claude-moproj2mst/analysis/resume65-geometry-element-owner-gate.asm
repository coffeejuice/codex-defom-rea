Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082BF60: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014082BF65: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014082BF6A: 0F B6 80 59 1B 00  movzx       eax,byte ptr [rax+0000000000001B59h]
                    00
  000000014082BF71: 85 C0              test        eax,eax
  000000014082BF73: 75 04              jne         000000014082BF79
  000000014082BF75: B0 01              mov         al,1
  000000014082BF77: EB 02              jmp         000000014082BF7B
  000000014082BF79: B0 01              mov         al,1
  000000014082BF7B: F3 C3              rep ret

  Summary

     1E02000 .text
