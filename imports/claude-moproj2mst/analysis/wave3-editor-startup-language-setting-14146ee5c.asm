Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146EE5C: 40 53              push        rbx
  000000014146EE5E: 48 83 EC 30        sub         rsp,30h
  000000014146EE62: 48 8B D9           mov         rbx,rcx
  000000014146EE65: 33 C0              xor         eax,eax
  000000014146EE67: 48 85 C9           test        rcx,rcx
  000000014146EE6A: 0F 95 C0           setne       al
  000000014146EE6D: 85 C0              test        eax,eax
  000000014146EE6F: 75 24              jne         000000014146EE95
  000000014146EE71: E8 BE 52 00 00     call        0000000141474134
  000000014146EE76: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146EE7C: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146EE82: 45 33 C9           xor         r9d,r9d
  000000014146EE85: 45 33 C0           xor         r8d,r8d
  000000014146EE88: 33 D2              xor         edx,edx
  000000014146EE8A: 33 C9              xor         ecx,ecx
  000000014146EE8C: E8 D7 16 00 00     call        0000000141470568
  000000014146EE91: 33 C0              xor         eax,eax
  000000014146EE93: EB 60              jmp         000000014146EEF5
  000000014146EE95: BA FF 7F 00 00     mov         edx,7FFFh
  000000014146EE9A: E8 B9 31 01 00     call        0000000141482058
  000000014146EE9F: 33 C9              xor         ecx,ecx
  000000014146EEA1: 48 3D FF 7F 00 00  cmp         rax,7FFFh
  000000014146EEA7: 0F 92 C1           setb        cl
  000000014146EEAA: 85 C9              test        ecx,ecx
  000000014146EEAC: 75 24              jne         000000014146EED2
  000000014146EEAE: E8 81 52 00 00     call        0000000141474134
  000000014146EEB3: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146EEB9: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146EEBF: 45 33 C9           xor         r9d,r9d
  000000014146EEC2: 45 33 C0           xor         r8d,r8d
  000000014146EEC5: 33 D2              xor         edx,edx
  000000014146EEC7: 33 C9              xor         ecx,ecx
  000000014146EEC9: E8 9A 16 00 00     call        0000000141470568
  000000014146EECE: 33 C0              xor         eax,eax
  000000014146EED0: EB 23              jmp         000000014146EEF5
  000000014146EED2: B9 07 00 00 00     mov         ecx,7
  000000014146EED7: E8 00 0D 01 00     call        000000014147FBDC
  000000014146EEDC: 90                 nop
  000000014146EEDD: 48 8B CB           mov         rcx,rbx
  000000014146EEE0: E8 D3 FE FF FF     call        000000014146EDB8
  000000014146EEE5: 48 8B D8           mov         rbx,rax
  000000014146EEE8: B9 07 00 00 00     mov         ecx,7
  000000014146EEED: E8 EA 0B 01 00     call        000000014147FADC
  000000014146EEF2: 48 8B C3           mov         rax,rbx
  000000014146EEF5: 48 83 C4 30        add         rsp,30h
  000000014146EEF9: 5B                 pop         rbx
  000000014146EEFA: C3                 ret

  Summary

     1E02000 .text
