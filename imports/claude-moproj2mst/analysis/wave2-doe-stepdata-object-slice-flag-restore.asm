
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411EEAF0: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  00000001411EEAF5: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  00000001411EEAFA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001411EEAFE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411EEB03: 48 83 EC 28        sub         rsp,28h
  00000001411EEB07: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  00000001411EEB0C: 7C 07              jl          00000001411EEB15
  00000001411EEB0E: 83 7C 24 38 2E     cmp         dword ptr [rsp+38h],2Eh
  00000001411EEB13: 7C 02              jl          00000001411EEB17
  00000001411EEB15: EB 2B              jmp         00000001411EEB42
  00000001411EEB17: 48 63 54 24 38     movsxd      rdx,dword ptr [rsp+38h]
  00000001411EEB1C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411EEB21: 0F B6 44 24 40     movzx       eax,byte ptr [rsp+40h]
  00000001411EEB26: 88 44 11 08        mov         byte ptr [rcx+rdx+8],al
  00000001411EEB2A: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001411EEB2F: 85 C0              test        eax,eax
  00000001411EEB31: 74 0F              je          00000001411EEB42
  00000001411EEB33: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001411EEB38: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411EEB3D: E8 3E FF FF FF     call        00000001411EEA80
  00000001411EEB42: 48 83 C4 28        add         rsp,28h
  00000001411EEB46: C3                 ret

  Summary

     1E02000 .text
