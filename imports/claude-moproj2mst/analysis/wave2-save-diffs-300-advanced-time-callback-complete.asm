Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408BFE60: mov         qword ptr [rsp+8],rcx
  00000001408BFE65: sub         rsp,48h
  00000001408BFE69: xorpd       xmm0,xmm0
  00000001408BFE6D: movsd       mmword ptr [rsp+28h],xmm0
  00000001408BFE73: mov         rcx,qword ptr [rsp+50h]
  00000001408BFE78: add         rcx,750h
  00000001408BFE7F: call        00000001408DFC30
  00000001408BFE84: movsd       mmword ptr [rsp+28h],xmm0
  00000001408BFE8A: mov         dword ptr [rsp+20h],0
  00000001408BFE92: mov         dword ptr [rsp+24h],0
  00000001408BFE9A: mov         qword ptr [rsp+30h],0
  00000001408BFEA3: mov         dword ptr [rsp+20h],0
  00000001408BFEAB: jmp         00000001408BFEB8
  00000001408BFEAD: mov         eax,dword ptr [rsp+20h]
  00000001408BFEB1: add         eax,1
  00000001408BFEB4: mov         dword ptr [rsp+20h],eax
  00000001408BFEB8: mov         rax,qword ptr [rsp+50h]
  00000001408BFEBD: mov         eax,dword ptr [rax+0000000000001378h]
  00000001408BFEC3: cmp         dword ptr [rsp+20h],eax
  00000001408BFEC7: jge         00000001408BFF27
  00000001408BFEC9: mov         edx,dword ptr [rsp+20h]
  00000001408BFECD: add         edx,1
  00000001408BFED0: mov         rcx,qword ptr [rsp+50h]
  00000001408BFED5: call        00000001408302A0
  00000001408BFEDA: mov         qword ptr [rsp+30h],rax
  00000001408BFEDF: cmp         qword ptr [rsp+30h],0
  00000001408BFEE5: jne         00000001408BFEE9
  00000001408BFEE7: jmp         00000001408BFEAD
  00000001408BFEE9: mov         rcx,qword ptr [rsp+30h]
  00000001408BFEEE: call        0000000140834FA0
  00000001408BFEF3: movzx       eax,al
  00000001408BFEF6: test        eax,eax
  00000001408BFEF8: jne         00000001408BFEFC
  00000001408BFEFA: jmp         00000001408BFEAD
  00000001408BFEFC: mov         rcx,qword ptr [rsp+30h]
  00000001408BFF01: add         rcx,2160h
  00000001408BFF08: call        0000000140892370
  00000001408BFF0D: movzx       eax,al
  00000001408BFF10: test        eax,eax
  00000001408BFF12: jne         00000001408BFF16
  00000001408BFF14: jmp         00000001408BFEAD
  00000001408BFF16: lea         rdx,[rsp+28h]
  00000001408BFF1B: mov         rcx,qword ptr [rsp+30h]
  00000001408BFF20: call        0000000140874630
  00000001408BFF25: jmp         00000001408BFEAD
  00000001408BFF27: add         rsp,48h
  00000001408BFF2B: ret
  00000001408BFF2C: int         3

  Summary

     1E02000 .text
