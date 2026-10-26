Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654F10D0: 48 8B C4           mov         rax,rsp
  00000000654F10D3: 56                 push        rsi
  00000000654F10D4: 57                 push        rdi
  00000000654F10D5: 41 54              push        r12
  00000000654F10D7: 48 83 EC 60        sub         rsp,60h
  00000000654F10DB: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654F10E4: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  00000000654F10E8: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  00000000654F10EC: 4D 8B E0           mov         r12,r8
  00000000654F10EF: 48 8B DA           mov         rbx,rdx
  00000000654F10F2: 48 8B E9           mov         rbp,rcx
  00000000654F10F5: 33 FF              xor         edi,edi
  00000000654F10F7: 89 78 08           mov         dword ptr [rax+8],edi
  00000000654F10FA: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  00000000654F10FE: 48 81 C6 A0 00 00  add         rsi,0A0h
                    00
  00000000654F1105: 48 8B CE           mov         rcx,rsi
  00000000654F1108: FF 15 42 77 1A 00  call        qword ptr [0000000065698850h]
  00000000654F110E: 44 8B 0B           mov         r9d,dword ptr [rbx]
  00000000654F1111: 44 39 08           cmp         dword ptr [rax],r9d
  00000000654F1114: 75 49              jne         00000000654F115F
  00000000654F1116: 48 8B 53 08        mov         rdx,qword ptr [rbx+8]
  00000000654F111A: 48 39 50 08        cmp         qword ptr [rax+8],rdx
  00000000654F111E: 75 3F              jne         00000000654F115F
  00000000654F1120: 8B 4B 04           mov         ecx,dword ptr [rbx+4]
  00000000654F1123: 39 48 04           cmp         dword ptr [rax+4],ecx
  00000000654F1126: 75 37              jne         00000000654F115F
  00000000654F1128: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000000654F112C: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  00000000654F1130: 75 2D              jne         00000000654F115F
  00000000654F1132: 41 8B 04 24        mov         eax,dword ptr [r12]
  00000000654F1136: 85 C0              test        eax,eax
  00000000654F1138: 0F 84 5B 02 00 00  je          00000000654F1399
  00000000654F113E: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  00000000654F1145: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000654F1149: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  00000000654F1151: 48 8B D3           mov         rdx,rbx
  00000000654F1154: 48 8B CD           mov         rcx,rbp
  00000000654F1157: FF 50 60           call        qword ptr [rax+60h]
  00000000654F115A: E9 3A 02 00 00     jmp         00000000654F1399
  00000000654F115F: 48 8B D6           mov         rdx,rsi
  00000000654F1162: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F116A: FF 15 B8 76 1A 00  call        qword ptr [0000000065698828h]
  00000000654F1170: 90                 nop
  00000000654F1171: 48 8B D3           mov         rdx,rbx
  00000000654F1174: 48 8B CE           mov         rcx,rsi
  00000000654F1177: FF 15 BB 76 1A 00  call        qword ptr [0000000065698838h]
  00000000654F117D: 45 8B 1C 24        mov         r11d,dword ptr [r12]
  00000000654F1181: 45 85 DB           test        r11d,r11d
  00000000654F1184: 74 26              je          00000000654F11AC
  00000000654F1186: 44 89 9C 24 80 00  mov         dword ptr [rsp+0000000000000080h],r11d
                    00 00
  00000000654F118E: 48 8B 5D 00        mov         rbx,qword ptr [rbp]
  00000000654F1192: 48 8B CE           mov         rcx,rsi
  00000000654F1195: FF 15 B5 76 1A 00  call        qword ptr [0000000065698850h]
  00000000654F119B: 48 8B D0           mov         rdx,rax
  00000000654F119E: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  00000000654F11A6: 48 8B CD           mov         rcx,rbp
  00000000654F11A9: FF 53 60           call        qword ptr [rbx+60h]
  00000000654F11AC: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F11B4: FF 15 96 76 1A 00  call        qword ptr [0000000065698850h]
  00000000654F11BA: 48 8B D8           mov         rbx,rax
  00000000654F11BD: 48 8B CE           mov         rcx,rsi
  00000000654F11C0: FF 15 8A 76 1A 00  call        qword ptr [0000000065698850h]
  00000000654F11C6: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000000654F11CF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000000654F11D4: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000000654F11D9: 4C 8D 4C 24 28     lea         r9,[rsp+28h]
  00000000654F11DE: 41 B8 01 00 00 00  mov         r8d,1
  00000000654F11E4: 48 8D 15 8D 59 4D  lea         rdx,[00000000659C6B78h]
                    00
  00000000654F11EB: 48 8B CD           mov         rcx,rbp
  00000000654F11EE: FF 15 BC 56 1A 00  call        qword ptr [00000000656968B0h]
  00000000654F11F4: 48 8B CE           mov         rcx,rsi
  00000000654F11F7: FF 15 43 76 1A 00  call        qword ptr [0000000065698840h]
  00000000654F11FD: 8B D8              mov         ebx,eax
  00000000654F11FF: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F1207: FF 15 33 76 1A 00  call        qword ptr [0000000065698840h]
  00000000654F120D: 3B D8              cmp         ebx,eax
  00000000654F120F: 75 5A              jne         00000000654F126B
  00000000654F1211: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000654F1216: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F121E: FF 15 34 76 1A 00  call        qword ptr [0000000065698858h]
  00000000654F1224: 48 8B D8           mov         rbx,rax
  00000000654F1227: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],1
                    00 01 00 00 00
  00000000654F1232: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000654F1237: 48 8B CE           mov         rcx,rsi
  00000000654F123A: FF 15 18 76 1A 00  call        qword ptr [0000000065698858h]
  00000000654F1240: BF 03 00 00 00     mov         edi,3
  00000000654F1245: 8B 08              mov         ecx,dword ptr [rax]
  00000000654F1247: 39 0B              cmp         dword ptr [rbx],ecx
  00000000654F1249: 75 20              jne         00000000654F126B
  00000000654F124B: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000000654F124F: 48 39 4B 08        cmp         qword ptr [rbx+8],rcx
  00000000654F1253: 75 16              jne         00000000654F126B
  00000000654F1255: 8B 48 04           mov         ecx,dword ptr [rax+4]
  00000000654F1258: 39 4B 04           cmp         dword ptr [rbx+4],ecx
  00000000654F125B: 75 0E              jne         00000000654F126B
  00000000654F125D: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000000654F1261: 48 39 43 10        cmp         qword ptr [rbx+10h],rax
  00000000654F1265: 75 04              jne         00000000654F126B
  00000000654F1267: 32 C0              xor         al,al
  00000000654F1269: EB 02              jmp         00000000654F126D
  00000000654F126B: B0 01              mov         al,1
  00000000654F126D: 40 F6 C7 02        test        dil,2
  00000000654F1271: 74 03              je          00000000654F1276
  00000000654F1273: 83 E7 FD           and         edi,0FFFFFFFDh
  00000000654F1276: 40 F6 C7 01        test        dil,1
  00000000654F127A: 74 03              je          00000000654F127F
  00000000654F127C: 83 E7 FE           and         edi,0FFFFFFFEh
  00000000654F127F: 84 C0              test        al,al
  00000000654F1281: 74 48              je          00000000654F12CB
  00000000654F1283: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F128B: FF 15 BF 75 1A 00  call        qword ptr [0000000065698850h]
  00000000654F1291: 48 8B D8           mov         rbx,rax
  00000000654F1294: 48 8B CE           mov         rcx,rsi
  00000000654F1297: FF 15 B3 75 1A 00  call        qword ptr [0000000065698850h]
  00000000654F129D: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000000654F12A6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000000654F12AB: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000000654F12B0: 4C 8D 4C 24 28     lea         r9,[rsp+28h]
  00000000654F12B5: 41 B8 02 00 00 00  mov         r8d,2
  00000000654F12BB: 48 8D 15 B6 58 4D  lea         rdx,[00000000659C6B78h]
                    00
  00000000654F12C2: 48 8B CD           mov         rcx,rbp
  00000000654F12C5: FF 15 E5 55 1A 00  call        qword ptr [00000000656968B0h]
  00000000654F12CB: 48 8B CE           mov         rcx,rsi
  00000000654F12CE: FF 15 74 75 1A 00  call        qword ptr [0000000065698848h]
  00000000654F12D4: 8B D8              mov         ebx,eax
  00000000654F12D6: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F12DE: FF 15 64 75 1A 00  call        qword ptr [0000000065698848h]
  00000000654F12E4: 3B D8              cmp         ebx,eax
  00000000654F12E6: 75 54              jne         00000000654F133C
  00000000654F12E8: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000654F12ED: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F12F5: FF 15 5D 75 1A 00  call        qword ptr [0000000065698858h]
  00000000654F12FB: 48 8B D8           mov         rbx,rax
  00000000654F12FE: 83 CF 04           or          edi,4
  00000000654F1301: 89 BC 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],edi
                    00
  00000000654F1308: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000654F130D: 48 8B CE           mov         rcx,rsi
  00000000654F1310: FF 15 42 75 1A 00  call        qword ptr [0000000065698858h]
  00000000654F1316: 8B 08              mov         ecx,dword ptr [rax]
  00000000654F1318: 39 0B              cmp         dword ptr [rbx],ecx
  00000000654F131A: 75 20              jne         00000000654F133C
  00000000654F131C: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000000654F1320: 48 39 4B 08        cmp         qword ptr [rbx+8],rcx
  00000000654F1324: 75 16              jne         00000000654F133C
  00000000654F1326: 8B 48 04           mov         ecx,dword ptr [rax+4]
  00000000654F1329: 39 4B 04           cmp         dword ptr [rbx+4],ecx
  00000000654F132C: 75 0E              jne         00000000654F133C
  00000000654F132E: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000000654F1332: 48 39 43 10        cmp         qword ptr [rbx+10h],rax
  00000000654F1336: 75 04              jne         00000000654F133C
  00000000654F1338: 32 C0              xor         al,al
  00000000654F133A: EB 02              jmp         00000000654F133E
  00000000654F133C: B0 01              mov         al,1
  00000000654F133E: 84 C0              test        al,al
  00000000654F1340: 74 49              je          00000000654F138B
  00000000654F1342: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F134A: FF 15 00 75 1A 00  call        qword ptr [0000000065698850h]
  00000000654F1350: 48 8B D8           mov         rbx,rax
  00000000654F1353: 48 8B CE           mov         rcx,rsi
  00000000654F1356: FF 15 F4 74 1A 00  call        qword ptr [0000000065698850h]
  00000000654F135C: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000000654F1365: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000000654F136A: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000000654F136F: 4C 8D 4C 24 28     lea         r9,[rsp+28h]
  00000000654F1374: 41 B8 03 00 00 00  mov         r8d,3
  00000000654F137A: 48 8D 15 F7 57 4D  lea         rdx,[00000000659C6B78h]
                    00
  00000000654F1381: 48 8B CD           mov         rcx,rbp
  00000000654F1384: FF 15 26 55 1A 00  call        qword ptr [00000000656968B0h]
  00000000654F138A: 90                 nop
  00000000654F138B: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000654F1393: FF 15 7F 74 1A 00  call        qword ptr [0000000065698818h]
  00000000654F1399: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  00000000654F139E: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  00000000654F13A2: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  00000000654F13A6: 49 8B E3           mov         rsp,r11
  00000000654F13A9: 41 5C              pop         r12
  00000000654F13AB: 5F                 pop         rdi
  00000000654F13AC: 5E                 pop         rsi
  00000000654F13AD: C3                 ret

  Summary

      695000 .text
