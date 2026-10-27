// ===== FUN_141249730 @ 141249730

int FUN_141249730(longlong param_1,undefined8 param_2,undefined1 param_3,longlong param_4,
                 longlong param_5)

{
  double *pdVar1;
  double dVar2;
  double adStack_68 [3];
  int iStack_50;
  int iStack_4c;
  double adStack_40 [3];
  longlong lStack_28;
  undefined1 auStack_20 [8];
  double dStack_18;
  double dStack_10;
  
  if (*(int *)(param_1 + 0x8468) == 0x80) {
    iStack_4c = 0;
  }
  else {
    iStack_4c = FUN_141249680(param_1);
    if (iStack_4c < 0x80) {
      lStack_28 = param_1 + (longlong)iStack_4c * 0x108;
      FUN_141247a70(lStack_28,param_2,param_3);
      auStack_20[0] = 1;
      FUN_141247ed0(lStack_28,auStack_20);
      FUN_141249e40(param_1,param_2,param_4,adStack_68);
      FUN_141249e40(param_1,param_2,param_5,adStack_40);
      for (iStack_50 = 0; iStack_50 < 3; iStack_50 = iStack_50 + 1) {
        if (adStack_40[iStack_50] <= adStack_68[iStack_50] &&
            adStack_68[iStack_50] != adStack_40[iStack_50]) {
          dVar2 = adStack_68[iStack_50];
          adStack_68[iStack_50] = adStack_40[iStack_50];
          adStack_40[iStack_50] = dVar2;
        }
        if (*(double *)(param_4 + (longlong)iStack_50 * 8) <= adStack_68[iStack_50]) {
          dStack_18 = *(double *)(param_4 + (longlong)iStack_50 * 8);
        }
        else {
          dStack_18 = adStack_68[iStack_50];
        }
        adStack_68[iStack_50] = dStack_18;
        if (adStack_40[iStack_50] <= *(double *)(param_5 + (longlong)iStack_50 * 8)) {
          dStack_10 = *(double *)(param_5 + (longlong)iStack_50 * 8);
        }
        else {
          dStack_10 = adStack_40[iStack_50];
        }
        adStack_40[iStack_50] = dStack_10;
      }
      if (*(int *)(param_1 + 0x8468) == 1) {
        for (iStack_50 = 0; iStack_50 < 3; iStack_50 = iStack_50 + 1) {
          *(double *)(param_1 + 0x8438 + (longlong)iStack_50 * 8) = adStack_68[iStack_50];
          *(double *)(param_1 + 0x8450 + (longlong)iStack_50 * 8) = adStack_40[iStack_50];
        }
      }
      else {
        for (iStack_50 = 0; iStack_50 < 3; iStack_50 = iStack_50 + 1) {
          dVar2 = *(double *)(param_1 + 0x8438 + (longlong)iStack_50 * 8);
          if (adStack_68[iStack_50] <= dVar2 && dVar2 != adStack_68[iStack_50]) {
            *(double *)(param_1 + 0x8438 + (longlong)iStack_50 * 8) = adStack_68[iStack_50];
          }
          pdVar1 = (double *)(param_1 + 0x8450 + (longlong)iStack_50 * 8);
          if (*pdVar1 <= adStack_40[iStack_50] && adStack_40[iStack_50] != *pdVar1) {
            *(double *)(param_1 + 0x8450 + (longlong)iStack_50 * 8) = adStack_40[iStack_50];
          }
        }
      }
      iStack_4c = iStack_4c + 1;
    }
    else {
      iStack_4c = 0;
    }
  }
  return iStack_4c;
}


// ===== FUN_141249f80 @ 141249f80

void FUN_141249f80(undefined8 param_1,int *param_2,undefined8 param_3)

{
  longlong lVar1;
  
  if (*param_2 == 0) {
    func_0x00014124a010(param_1,param_3);
  }
  else {
    lVar1 = FUN_1412494c0(param_1,param_2);
    if (lVar1 != 0) {
      func_0x000141247d90(lVar1,param_3);
    }
  }
  lVar1 = FUN_1412494c0(param_1,param_3);
  if (lVar1 != 0) {
    func_0x000141247d90(lVar1,param_2);
  }
  return;
}


// ===== FUN_1412479e0 @ 1412479e0

longlong FUN_1412479e0(longlong param_1)

{
  undefined4 uStack_18;
  
  *(undefined1 *)(param_1 + 0xf8) = 1;
  *(undefined1 *)(param_1 + 0xf9) = 0;
  for (uStack_18 = 0; uStack_18 < 4; uStack_18 = uStack_18 + 1) {
    *(undefined8 *)(param_1 + 0xc0 + (longlong)uStack_18 * 8) = 0;
  }
  *(undefined4 *)(param_1 + 0xfc) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x100) = 0xffffffff;
  return param_1;
}


// ===== FUN_1412494c0 @ 1412494c0

longlong FUN_1412494c0(longlong param_1,int *param_2)

{
  int iVar1;
  
  if ((0 < *param_2) && (iVar1 = FUN_141249420(param_1), *param_2 <= iVar1)) {
    return param_1 + (longlong)(*param_2 + -1) * 0x108;
  }
  return 0;
}


