
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1400ab170(int *param_1)

{
  int iVar1;
  undefined1 auStack_168 [32];
  undefined4 uStack_148;
  undefined1 auStack_128 [208];
  longlong lStack_58;
  int iStack_50;
  char cStack_4c;
  undefined1 auStack_48 [4];
  undefined1 uStack_44;
  undefined1 uStack_43;
  undefined8 uStack_40;
  int iStack_38;
  int iStack_34;
  undefined1 uStack_30;
  undefined1 uStack_2f;
  double dStack_28;
  double dStack_20;
  undefined1 uStack_18;
  ulonglong uStack_10;
  
  uStack_40 = 0xfffffffffffffffe;
  uStack_10 = uRam0000000143b5e030 ^ (ulonglong)auStack_168;
  func_0x000140906740(auStack_48);
  cStack_4c = '\0';
  lStack_58 = func_0x00014146fcd8(&UNK_142142180,&UNK_142142194);
  if (lStack_58 != 0) {
    do {
      uStack_148 = 0xffffec78;
      FUN_1409068f0(auStack_48,lStack_58,2,auStack_128);
      iVar1 = func_0x000141470380(auStack_128,&UNK_142142170,0x50);
      if (iVar1 == 0) {
        cStack_4c = '\x01';
        break;
      }
      iVar1 = func_0x000141470380(auStack_128,&UNK_142142164,0x50);
    } while (iVar1 != 0);
    FUN_141470298(lStack_58);
  }
  if (cStack_4c == '\0') {
    if (lStack_58 != 0) {
      FUN_14146fb28(lStack_58);
    }
    lStack_58 = func_0x00014146fcd8(&UNK_142142150,&UNK_142142194);
    if (lStack_58 == 0) {
      uStack_44 = 0;
      func_0x000140906760(auStack_48);
      goto code_r0x0001400ab6d0;
    }
  }
  do {
    uStack_148 = 0xffffec78;
    FUN_1409068f0(auStack_48,lStack_58,2,auStack_128);
    iVar1 = func_0x000141470380(auStack_128,&UNK_142142128,0x50);
    if (iVar1 == 0) {
      uStack_148 = 0;
      FUN_1409068f0(auStack_48,lStack_58,5,&iStack_50);
      *param_1 = iStack_50;
      if (iStack_50 < 0) {
        iStack_38 = 0;
      }
      else {
        if (iStack_50 < 6) {
          iStack_34 = iStack_50;
        }
        else {
          iStack_34 = 5;
        }
        iStack_38 = iStack_34;
      }
      *param_1 = iStack_38;
    }
    else {
      iVar1 = func_0x000141470380(auStack_128,&UNK_142142100,0x50);
      if (iVar1 == 0) {
        uStack_148 = 0;
        FUN_1409068f0(auStack_48,lStack_58,5,&iStack_50);
        uStack_30 = iStack_50 != 0;
        *(undefined1 *)(param_1 + 1) = uStack_30;
      }
      else {
        iVar1 = func_0x000141470380(auStack_128,&UNK_1421420f0,0x50);
        if (iVar1 == 0) {
          uStack_148 = 0;
          FUN_1409068f0(auStack_48,lStack_58,10,param_1 + 2);
        }
        else {
          iVar1 = func_0x000141470380(auStack_128,&UNK_1421420d0,0x50);
          if (iVar1 == 0) {
            uStack_148 = 0;
            FUN_1409068f0(auStack_48,lStack_58,4,param_1 + 4);
          }
          else {
            iVar1 = func_0x000141470380(auStack_128,&UNK_1421420a8,0x50);
            if (iVar1 == 0) {
              uStack_148 = 0;
              FUN_1409068f0(auStack_48,lStack_58,5,&iStack_50);
              uStack_2f = iStack_50 != 0;
              *(undefined1 *)(param_1 + 6) = uStack_2f;
            }
            else {
              iVar1 = func_0x000141470380(auStack_128,&UNK_142142080,0x50);
              if (iVar1 == 0) {
                uStack_148 = 0;
                FUN_1409068f0(auStack_48,lStack_58,10,param_1 + 8);
                if (_DAT_1421249e0 <= *(double *)(param_1 + 8)) {
                  if (*(double *)(param_1 + 8) <= _DAT_142142078) {
                    dStack_20 = *(double *)(param_1 + 8);
                  }
                  else {
                    dStack_20 = _DAT_142142078;
                  }
                  dStack_28 = dStack_20;
                }
                else {
                  dStack_28 = _DAT_1421249e0;
                }
                *(double *)(param_1 + 8) = dStack_28;
              }
              else {
                iVar1 = func_0x000141470380(auStack_128,&UNK_142142058,0x50);
                if (iVar1 == 0) {
                  uStack_148 = 0;
                  FUN_1409068f0(auStack_48,lStack_58,5,&iStack_50);
                  uStack_18 = iStack_50 != 0;
                  *(undefined1 *)(param_1 + 10) = uStack_18;
                }
              }
            }
          }
        }
      }
    }
    iVar1 = func_0x000141470380(auStack_128,&UNK_142142164,0x50);
  } while (iVar1 != 0);
  FUN_14146fb28(lStack_58);
  uStack_43 = 1;
  func_0x000140906760(auStack_48);
code_r0x0001400ab6d0:
  func_0x00014146df10(uStack_10 ^ (ulonglong)auStack_168);
  return;
}

