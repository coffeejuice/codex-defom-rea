
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001415868AF: 98                 cwde
  00000001415868B0: 9A
  00000001415868B1: 01 FF              add         edi,edi
  00000001415868B3: 15 F8 D5 87 00     adc         eax,87D5F8h
  00000001415868B8: 48 8B F8           mov         rdi,rax
  00000001415868BB: 48 8D 0D 7E 98 9A  lea         rcx,[0000000142F30140h]
                    01
  00000001415868C2: FF 15 E8 D5 87 00  call        qword ptr [0000000141E03EB0h]
  00000001415868C8: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001415868CD: 4C 8B CF           mov         r9,rdi
  00000001415868D0: 4C 8B 84 24 30 03  mov         r8,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001415868D8: 48 8B D0           mov         rdx,rax
  00000001415868DB: 48 8D 0D 1E FE 7E  lea         rcx,[0000000143D76700h]
                    02
  00000001415868E2: E8 F9 92 D4 FE     call        00000001402CFBE0
  00000001415868E7: 4C
  00000001415868E8: 8D
  00000001415868E9: 84 24 88           test        byte ptr [rax+rcx*4],ah
  00000001415868EC: 00 00              add         byte ptr [rax],al

  Summary

     1E02000 .text
