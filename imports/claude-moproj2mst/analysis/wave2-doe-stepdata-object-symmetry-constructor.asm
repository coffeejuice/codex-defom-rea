
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141249140: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141249145: 48 83 EC 48        sub         rsp,48h
  0000000141249149: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000141249152: 48 8D 05 07 E9 FF  lea         rax,[0000000141247A60h]
                    FF
  0000000141249159: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014124915E: 4C 8D 0D 7B E8 FF  lea         r9,[00000001412479E0h]
                    FF
  0000000141249165: 41 B8 80 00 00 00  mov         r8d,80h
  000000014124916B: BA 08 01 00 00     mov         edx,108h
  0000000141249170: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000141249175: E8 D6 5B 22 00     call        000000014146ED50
  000000014124917A: 90                 nop
  000000014124917B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000141249180: 48 81 C1 00 84 00  add         rcx,8400h
                    00
  0000000141249187: E8 D4 6A 02 00     call        000000014126FC60
  000000014124918C: 4C 8B 5C 24 50     mov         r11,qword ptr [rsp+50h]
  0000000141249191: 41 C7 83 68 84 00  mov         dword ptr [r11+0000000000008468h],0
                    00 00 00 00 00
  000000014124919C: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000001412491A4: EB 0B              jmp         00000001412491B1
  00000001412491A6: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001412491AA: 83 C0 01           add         eax,1
  00000001412491AD: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001412491B1: 83 7C 24 30 03     cmp         dword ptr [rsp+30h],3
  00000001412491B6: 7D 34              jge         00000001412491EC
  00000001412491B8: 48 63 4C 24 30     movsxd      rcx,dword ptr [rsp+30h]
  00000001412491BD: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001412491C2: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001412491C6: F2 0F 11 84 C8 38  movsd       mmword ptr [rax+rcx*8+0000000000008438h],xmm0
                    84 00 00
  00000001412491CF: 48 63 4C 24 30     movsxd      rcx,dword ptr [rsp+30h]
  00000001412491D4: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001412491D9: F2 0F 10 05 FF B7  movsd       xmm0,mmword ptr [00000001421249E0h]
                    ED 00
  00000001412491E1: F2 0F 11 84 C8 50  movsd       mmword ptr [rax+rcx*8+0000000000008450h],xmm0
                    84 00 00
  00000001412491EA: EB BA              jmp         00000001412491A6
  00000001412491EC: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001412491F1: C7 80 6C 84 00 00  mov         dword ptr [rax+000000000000846Ch],0FFFFFFFFh
                    FF FF FF FF
  00000001412491FB: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000141249200: C7 80 70 84 00 00  mov         dword ptr [rax+0000000000008470h],0FFFFFFFFh
                    FF FF FF FF
  000000014124920A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014124920F: 48 83 C4 48        add         rsp,48h
  0000000141249213: C3                 ret
  0000000141249214: CC                 int         3
  0000000141249215: CC                 int         3
  0000000141249216: CC                 int         3
  0000000141249217: CC                 int         3
  0000000141249218: CC                 int         3
  0000000141249219: CC                 int         3
  000000014124921A: CC                 int         3
  000000014124921B: CC                 int         3
  000000014124921C: CC                 int         3
  000000014124921D: CC                 int         3
  000000014124921E: CC                 int         3
  000000014124921F: CC                 int         3

  Summary

     1E02000 .text
