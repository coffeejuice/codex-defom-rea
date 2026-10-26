
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653CB190: 40 55              push        rbp
  00000000653CB192: 56                 push        rsi
  00000000653CB193: 57                 push        rdi
  00000000653CB194: 48 83 EC 60        sub         rsp,60h
  00000000653CB198: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653CB1A1: 48 89 9C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbx
                    00 00
  00000000653CB1A9: 48 8B DA           mov         rbx,rdx
  00000000653CB1AC: 48 8B F1           mov         rsi,rcx
  00000000653CB1AF: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000653CB1B3: 48 8B CA           mov         rcx,rdx
  00000000653CB1B6: FF 15 AC B8 2C 00  call        qword ptr [0000000065696A68h]
  00000000653CB1BC: 83 C0 FE           add         eax,0FFFFFFFEh
  00000000653CB1BF: 3D B5 00 00 00     cmp         eax,0B5h
  00000000653CB1C4: 0F 87 76 02 00 00  ja          00000000653CB440
  00000000653CB1CA: 48 98              cdqe
  00000000653CB1CC: 48 8D 0D 2D 4E C3  lea         rcx,[0000000065000000h]
                    FF
  00000000653CB1D3: 0F B6 84 01 90 B4  movzx       eax,byte ptr [rcx+rax+00000000003CB490h]
                    3C 00
  00000000653CB1DB: 44 8B 84 81 5C B4  mov         r8d,dword ptr [rcx+rax*4+00000000003CB45Ch]
                    3C 00
  00000000653CB1E3: 4C 03 C1           add         r8,rcx
  00000000653CB1E6: 41 FF E0           jmp         r8
  00000000653CB1E9: 48 8D 56 10        lea         rdx,[rsi+10h]
  00000000653CB1ED: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000653CB1F5: E8 06 19 D8 FF     call        000000006514CB00
  00000000653CB1FA: 90                 nop
  00000000653CB1FB: 48 8D 53 28        lea         rdx,[rbx+28h]
  00000000653CB1FF: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000000653CB207: E8 84 64 DA FF     call        0000000065171690
  00000000653CB20C: 90                 nop
  00000000653CB20D: 48 8D 87 00 02 00  lea         rax,[rdi+0000000000000200h]
                    00
  00000000653CB214: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  00000000653CB21B: 48 81 C1 F8 00 00  add         rcx,0F8h
                    00
  00000000653CB222: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000653CB227: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  00000000653CB22F: 4C 8B C6           mov         r8,rsi
  00000000653CB232: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  00000000653CB23A: E8 51 A4 FD FF     call        00000000653A5690
  00000000653CB23F: 90                 nop
  00000000653CB240: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000000653CB248: E8 D3 E1 DB FF     call        0000000065189420
  00000000653CB24D: 90                 nop
  00000000653CB24E: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000653CB256: E8 75 10 D8 FF     call        000000006514C2D0
  00000000653CB25B: E9 E0 01 00 00     jmp         00000000653CB440
  00000000653CB260: 48 8D 53 18        lea         rdx,[rbx+18h]
  00000000653CB264: 48 8B CF           mov         rcx,rdi
  00000000653CB267: E8 F4 FB FF FF     call        00000000653CAE60
  00000000653CB26C: E9 CF 01 00 00     jmp         00000000653CB440
  00000000653CB271: B0 01              mov         al,1
  00000000653CB273: E9 D3 01 00 00     jmp         00000000653CB44B
  00000000653CB278: 45 33 C0           xor         r8d,r8d
  00000000653CB27B: 33 D2              xor         edx,edx
  00000000653CB27D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000653CB282: FF 15 80 CB 2C 00  call        qword ptr [0000000065697E08h]
  00000000653CB288: 48 8B D0           mov         rdx,rax
  00000000653CB28B: 48 8B CF           mov         rcx,rdi
  00000000653CB28E: E8 CD FB FF FF     call        00000000653CAE60
  00000000653CB293: B0 01              mov         al,1
  00000000653CB295: E9 B1 01 00 00     jmp         00000000653CB44B
  00000000653CB29A: 45 33 C0           xor         r8d,r8d
  00000000653CB29D: 33 D2              xor         edx,edx
  00000000653CB29F: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000000653CB2A4: FF 15 5E CB 2C 00  call        qword ptr [0000000065697E08h]
  00000000653CB2AA: 48 8B D0           mov         rdx,rax
  00000000653CB2AD: 48 8B CF           mov         rcx,rdi
  00000000653CB2B0: E8 AB FB FF FF     call        00000000653CAE60
  00000000653CB2B5: E9 86 01 00 00     jmp         00000000653CB440
  00000000653CB2BA: 83 7B 30 01        cmp         dword ptr [rbx+30h],1
  00000000653CB2BE: 0F 85 7C 01 00 00  jne         00000000653CB440
  00000000653CB2C4: 48 8D 53 20        lea         rdx,[rbx+20h]
  00000000653CB2C8: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  00000000653CB2CF: E8 DC 5B 00 00     call        00000000653D0EB0
  00000000653CB2D4: 84 C0              test        al,al
  00000000653CB2D6: 0F 84 64 01 00 00  je          00000000653CB440
  00000000653CB2DC: 48 8B CB           mov         rcx,rbx
  00000000653CB2DF: FF 15 BB BA 2C 00  call        qword ptr [0000000065696DA0h]
  00000000653CB2E5: B0 01              mov         al,1
  00000000653CB2E7: E9 5F 01 00 00     jmp         00000000653CB44B
  00000000653CB2EC: 48 8D 53 20        lea         rdx,[rbx+20h]
  00000000653CB2F0: 48 8B CF           mov         rcx,rdi
  00000000653CB2F3: E8 68 FB FF FF     call        00000000653CAE60
  00000000653CB2F8: F6 43 34 01        test        byte ptr [rbx+34h],1
  00000000653CB2FC: 0F 84 3E 01 00 00  je          00000000653CB440
  00000000653CB302: 48 8D 53 20        lea         rdx,[rbx+20h]
  00000000653CB306: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  00000000653CB30D: E8 3E 12 00 00     call        00000000653CC550
  00000000653CB312: 84 C0              test        al,al
  00000000653CB314: 0F 84 26 01 00 00  je          00000000653CB440
  00000000653CB31A: 48 8B CB           mov         rcx,rbx
  00000000653CB31D: FF 15 7D BA 2C 00  call        qword ptr [0000000065696DA0h]
  00000000653CB323: B0 01              mov         al,1
  00000000653CB325: E9 21 01 00 00     jmp         00000000653CB44B
  00000000653CB32A: 48 8D 53 20        lea         rdx,[rbx+20h]
  00000000653CB32E: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  00000000653CB335: E8 B6 2F 00 00     call        00000000653CE2F0
  00000000653CB33A: 84 C0              test        al,al
  00000000653CB33C: 0F 84 FE 00 00 00  je          00000000653CB440
  00000000653CB342: 48 8B CB           mov         rcx,rbx
  00000000653CB345: FF 15 55 BA 2C 00  call        qword ptr [0000000065696DA0h]
  00000000653CB34B: B0 01              mov         al,1
  00000000653CB34D: E9 F9 00 00 00     jmp         00000000653CB44B
  00000000653CB352: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  00000000653CB359: E8 02 10 00 00     call        00000000653CC360
  00000000653CB35E: B0 01              mov         al,1
  00000000653CB360: E9 E6 00 00 00     jmp         00000000653CB44B
  00000000653CB365: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  00000000653CB36C: E8 0F 1A 00 00     call        00000000653CCD80
  00000000653CB371: 48 8B F8           mov         rdi,rax
  00000000653CB374: 48 85 C0           test        rax,rax
  00000000653CB377: 74 30              je          00000000653CB3A9
  00000000653CB379: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  00000000653CB381: 48 8B CB           mov         rcx,rbx
  00000000653CB384: E8 27 29 21 00     call        00000000655DDCB0
  00000000653CB389: 90                 nop
  00000000653CB38A: 45 33 C0           xor         r8d,r8d
  00000000653CB38D: 48 8B D0           mov         rdx,rax
  00000000653CB390: 48 8B CF           mov         rcx,rdi
  00000000653CB393: E8 38 B6 03 00     call        00000000654069D0
  00000000653CB398: 90                 nop
  00000000653CB399: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000000653CB3A1: FF 15 B9 B3 2C 00  call        qword ptr [0000000065696760h]
  00000000653CB3A7: EB 09              jmp         00000000653CB3B2
  00000000653CB3A9: 48 8B CB           mov         rcx,rbx
  00000000653CB3AC: FF 15 E6 B9 2C 00  call        qword ptr [0000000065696D98h]
  00000000653CB3B2: B0 01              mov         al,1
  00000000653CB3B4: E9 92 00 00 00     jmp         00000000653CB44B
  00000000653CB3B9: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  00000000653CB3C0: 48 81 C1 F8 00 00  add         rcx,0F8h
                    00
  00000000653CB3C7: E8 E4 12 FE FF     call        00000000653AC6B0
  00000000653CB3CC: 80 BF F0 01 00 00  cmp         byte ptr [rdi+00000000000001F0h],0
                    00
  00000000653CB3D3: 75 6B              jne         00000000653CB440
  00000000653CB3D5: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000653CB3DA: FF 15 98 B3 2C 00  call        qword ptr [0000000065696778h]
  00000000653CB3E0: 48 8B D0           mov         rdx,rax
  00000000653CB3E3: 48 8B CE           mov         rcx,rsi
  00000000653CB3E6: E8 F5 E6 FF FF     call        00000000653C9AE0
  00000000653CB3EB: EB 53              jmp         00000000653CB440
  00000000653CB3ED: F6 87 10 02 00 00  test        byte ptr [rdi+0000000000000210h],2
                    02
  00000000653CB3F4: 74 4A              je          00000000653CB440
  00000000653CB3F6: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000653CB3FB: 48 8B CE           mov         rcx,rsi
  00000000653CB3FE: E8 3D 43 C9 FF     call        000000006505F740
  00000000653CB403: 90                 nop
  00000000653CB404: 48 8D 8F 08 02 00  lea         rcx,[rdi+0000000000000208h]
                    00
  00000000653CB40B: 48 8B D0           mov         rdx,rax
  00000000653CB40E: E8 CD 3D C6 FF     call        000000006502F1E0
  00000000653CB413: 90                 nop
  00000000653CB414: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000653CB419: E8 82 3D C6 FF     call        000000006502F1A0
  00000000653CB41E: BA 26 00 00 00     mov         edx,26h
  00000000653CB423: 48 8B CE           mov         rcx,rsi
  00000000653CB426: E8 E5 2B C9 FF     call        000000006505E010
  00000000653CB42B: 44 8A D8           mov         r11b,al
  00000000653CB42E: 44 33 9F 10 02 00  xor         r11d,dword ptr [rdi+0000000000000210h]
                    00
  00000000653CB435: 41 83 E3 01        and         r11d,1
  00000000653CB439: 44 31 9F 10 02 00  xor         dword ptr [rdi+0000000000000210h],r11d
                    00
  00000000653CB440: 48 8B D3           mov         rdx,rbx
  00000000653CB443: 48 8B CE           mov         rcx,rsi
  00000000653CB446: E8 05 15 CA FF     call        000000006506C950
  00000000653CB44B: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000653CB453: 48 83 C4 60        add         rsp,60h
  00000000653CB457: 5F                 pop         rdi
  00000000653CB458: 5E                 pop         rsi
  00000000653CB459: 5D                 pop         rbp
  00000000653CB45A: C3                 ret
  00000000653CB45B: 90                 nop
  00000000653CB45C: BA B2 3C 00 2A     mov         edx,2A003CB2h
  00000000653CB461: B3 3C              mov         bl,3Ch
  00000000653CB463: 00 EC              add         ah,ch
  00000000653CB465: B2 3C              mov         dl,3Ch
  00000000653CB467: 00 E9              add         cl,ch
  00000000653CB469: B1 3C              mov         cl,3Ch
  00000000653CB46B: 00 9A B2 3C 00 B9  add         byte ptr [rdx+FFFFFFFFB9003CB2h],bl
  00000000653CB471: B3 3C              mov         bl,3Ch
  00000000653CB473: 00 65 B3           add         byte ptr [rbp-4Dh],ah
  00000000653CB476: 3C 00              cmp         al,0
  00000000653CB478: 52                 push        rdx
  00000000653CB479: B3 3C              mov         bl,3Ch
  00000000653CB47B: 00 71 B2           add         byte ptr [rcx-4Eh],dh
  00000000653CB47E: 3C 00              cmp         al,0
  00000000653CB480: 78 B2              js          00000000653CB434
  00000000653CB482: 3C 00              cmp         al,0
  00000000653CB484: 60
  00000000653CB485: B2 3C              mov         dl,3Ch
  00000000653CB487: 00 ED              add         ch,ch
  00000000653CB489: B3 3C              mov         bl,3Ch
  00000000653CB48B: 00 40 B4           add         byte ptr [rax-4Ch],al
  00000000653CB48E: 3C 00              cmp         al,0
  00000000653CB490: 00 01              add         byte ptr [rcx],al
  00000000653CB492: 0C 02              or          al,2
  00000000653CB494: 0C 0C              or          al,0Ch
  00000000653CB496: 0C 0C              or          al,0Ch
  00000000653CB498: 0C 0C              or          al,0Ch
  00000000653CB49A: 03 0C 0C           add         ecx,dword ptr [rsp+rcx]
  00000000653CB49D: 0C 0C              or          al,0Ch
  00000000653CB49F: 0C 0C              or          al,0Ch
  00000000653CB4A1: 0C 0C              or          al,0Ch
  00000000653CB4A3: 0C 0C              or          al,0Ch
  00000000653CB4A5: 0C 0C              or          al,0Ch
  00000000653CB4A7: 0C 0C              or          al,0Ch
  00000000653CB4A9: 0C 0C              or          al,0Ch
  00000000653CB4AB: 0C 0C              or          al,0Ch
  00000000653CB4AD: 0C 0C              or          al,0Ch
  00000000653CB4AF: 0C 0C              or          al,0Ch
  00000000653CB4B1: 0C 0C              or          al,0Ch
  00000000653CB4B3: 0C 0C              or          al,0Ch
  00000000653CB4B5: 0C 0C              or          al,0Ch
  00000000653CB4B7: 0C 0C              or          al,0Ch
  00000000653CB4B9: 0C 0C              or          al,0Ch
  00000000653CB4BB: 0C 0C              or          al,0Ch
  00000000653CB4BD: 0C 0C              or          al,0Ch
  00000000653CB4BF: 0C 0C              or          al,0Ch
  00000000653CB4C1: 04 0C              add         al,0Ch
  00000000653CB4C3: 0C 0C              or          al,0Ch
  00000000653CB4C5: 0C 0C              or          al,0Ch
  00000000653CB4C7: 0C 0C              or          al,0Ch
  00000000653CB4C9: 0C 0C              or          al,0Ch
  00000000653CB4CB: 0C 0C              or          al,0Ch
  00000000653CB4CD: 0C 0C              or          al,0Ch
  00000000653CB4CF: 0C 0C              or          al,0Ch
  00000000653CB4D1: 0C 0C              or          al,0Ch
  00000000653CB4D3: 0C 0C              or          al,0Ch
  00000000653CB4D5: 0C 0C              or          al,0Ch
  00000000653CB4D7: 0C 0C              or          al,0Ch
  00000000653CB4D9: 0C 0C              or          al,0Ch
  00000000653CB4DB: 0C 0C              or          al,0Ch
  00000000653CB4DD: 0C 0C              or          al,0Ch
  00000000653CB4DF: 0C 0C              or          al,0Ch
  00000000653CB4E1: 0C 0C              or          al,0Ch
  00000000653CB4E3: 0C 0C              or          al,0Ch
  00000000653CB4E5: 0C 0C              or          al,0Ch
  00000000653CB4E7: 0C 0C              or          al,0Ch
  00000000653CB4E9: 0C 0C              or          al,0Ch
  00000000653CB4EB: 0C 0C              or          al,0Ch
  00000000653CB4ED: 0C 0C              or          al,0Ch
  00000000653CB4EF: 0C 0C              or          al,0Ch
  00000000653CB4F1: 0C 05              or          al,5
  00000000653CB4F3: 0C 0C              or          al,0Ch
  00000000653CB4F5: 0C 0C              or          al,0Ch
  00000000653CB4F7: 0C 0C              or          al,0Ch
  00000000653CB4F9: 0C 0C              or          al,0Ch
  00000000653CB4FB: 0C 0C              or          al,0Ch
  00000000653CB4FD: 0C 06              or          al,6
  00000000653CB4FF: 0C 0C              or          al,0Ch
  00000000653CB501: 0C 0C              or          al,0Ch
  00000000653CB503: 0C 0C              or          al,0Ch
  00000000653CB505: 0C 07              or          al,7
  00000000653CB507: 0C 0C              or          al,0Ch
  00000000653CB509: 0C 0C              or          al,0Ch
  00000000653CB50B: 0C 0C              or          al,0Ch
  00000000653CB50D: 08 09              or          byte ptr [rcx],cl
  00000000653CB50F: 0A 0C 0C           or          cl,byte ptr [rsp+rcx]
  00000000653CB512: 0C 0C              or          al,0Ch
  00000000653CB514: 0C 0C              or          al,0Ch
  00000000653CB516: 0C 0C              or          al,0Ch
  00000000653CB518: 0C 0C              or          al,0Ch
  00000000653CB51A: 0C 0C              or          al,0Ch
  00000000653CB51C: 0C 0C              or          al,0Ch
  00000000653CB51E: 0C 0C              or          al,0Ch
  00000000653CB520: 0C 0C              or          al,0Ch
  00000000653CB522: 0C 0C              or          al,0Ch
  00000000653CB524: 0C 0C              or          al,0Ch
  00000000653CB526: 0C 0C              or          al,0Ch
  00000000653CB528: 0C 0C              or          al,0Ch
  00000000653CB52A: 0C 0C              or          al,0Ch
  00000000653CB52C: 0C 0C              or          al,0Ch
  00000000653CB52E: 0C 0C              or          al,0Ch
  00000000653CB530: 0C 0C              or          al,0Ch
  00000000653CB532: 0C 0C              or          al,0Ch
  00000000653CB534: 0C 0C              or          al,0Ch
  00000000653CB536: 0C 0C              or          al,0Ch
  00000000653CB538: 0C 0C              or          al,0Ch
  00000000653CB53A: 0C 0C              or          al,0Ch
  00000000653CB53C: 0C 0C              or          al,0Ch
  00000000653CB53E: 0C 0C              or          al,0Ch
  00000000653CB540: 0C 0C              or          al,0Ch
  00000000653CB542: 0C 0C              or          al,0Ch
  00000000653CB544: 0C 0B              or          al,0Bh

  Summary

      695000 .text
