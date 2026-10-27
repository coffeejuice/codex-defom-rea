// ===== FUN_141249680 @ 141249680

int FUN_141249680(longlong param_1)

{
  char cVar1;
  undefined4 uStack_18;
  undefined4 uStack_14;
  
  uStack_18 = -1;
  uStack_14 = 0;
  do {
    if (*(int *)(param_1 + 0x8468) <= uStack_14) {
code_r0x0001412496e3:
      if (uStack_18 == -1) {
        uStack_18 = *(int *)(param_1 + 0x8468);
        *(int *)(param_1 + 0x8468) = *(int *)(param_1 + 0x8468) + 1;
      }
      return uStack_18;
    }
    cVar1 = FUN_141247eb0(param_1 + (longlong)uStack_14 * 0x108);
    if (cVar1 == '\0') {
      uStack_18 = uStack_14;
      goto code_r0x0001412496e3;
    }
    uStack_14 = uStack_14 + 1;
  } while( true );
}


// ===== FUN_141247a70 @ 141247a70

void FUN_141247a70(longlong param_1,longlong param_2,undefined1 param_3)

{
  undefined2 uStack_18;
  undefined2 uStack_14;
  
  for (uStack_18 = 0; uStack_18 < 4; uStack_18 = uStack_18 + 1) {
    for (uStack_14 = 0; uStack_14 < 4; uStack_14 = uStack_14 + 1) {
      *(undefined8 *)(param_1 + 0x40 + (longlong)uStack_18 * 0x20 + (longlong)uStack_14 * 8) =
           *(undefined8 *)(param_2 + (longlong)uStack_18 * 0x20 + (longlong)uStack_14 * 8);
    }
  }
  for (uStack_18 = 0; uStack_18 < 4; uStack_18 = uStack_18 + 1) {
    for (uStack_14 = 0; uStack_14 < 4; uStack_14 = uStack_14 + 1) {
      *(float *)(param_1 + (longlong)uStack_18 * 0x10 + (longlong)uStack_14 * 4) =
           (float)*(double *)(param_2 + (longlong)uStack_18 * 0x20 + (longlong)uStack_14 * 8);
    }
  }
  *(undefined1 *)(param_1 + 0xf8) = param_3;
  return;
}


// ===== FUN_141249e40 @ 141249e40

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_141249e40(undefined8 param_1,longlong param_2,double *param_3,double *param_4)

{
  double adStack_40060 [32767];
  short sStack_68;
  double adStack_60 [4];
  double adStack_40 [4];
  double dStack_20;
  short sStack_18;
  
  adStack_60[0] = *param_3;
  adStack_60[1] = param_3[1];
  adStack_60[2] = param_3[2];
  adStack_60[3] = (double)_DAT_1421249e0;
  for (sStack_68 = 0; sStack_68 < 4; sStack_68 = sStack_68 + 1) {
    dStack_20 = 0.0;
    for (sStack_18 = 0; sStack_18 < 4; sStack_18 = sStack_18 + 1) {
      dStack_20 = dStack_20 +
                  *(double *)(param_2 + (longlong)sStack_18 * 0x20 + (longlong)sStack_68 * 8) *
                  adStack_60[sStack_18];
    }
    adStack_40[sStack_68] = dStack_20;
  }
  *param_4 = adStack_40[0];
  param_4[1] = adStack_40[1];
  param_4[2] = adStack_40[2];
  return;
}


// ===== FUN_14124a010 @ 14124a010

void FUN_14124a010(longlong param_1,undefined4 *param_2)

{
  if (*(int *)(param_1 + 0x846c) == -1) {
    *(undefined4 *)(param_1 + 0x846c) = *param_2;
  }
  else if (*(int *)(param_1 + 0x8470) == -1) {
    *(undefined4 *)(param_1 + 0x8470) = *param_2;
  }
  return;
}


// ===== FUN_141247d90 @ 141247d90

void FUN_141247d90(longlong param_1,undefined4 *param_2)

{
  if (*(int *)(param_1 + 0xfc) == -1) {
    *(undefined4 *)(param_1 + 0xfc) = *param_2;
  }
  else if (*(int *)(param_1 + 0x100) == -1) {
    *(undefined4 *)(param_1 + 0x100) = *param_2;
  }
  return;
}


// ===== FUN_141247ba0 @ 141247ba0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_141247ba0(longlong param_1,undefined8 *param_2,undefined8 *param_3)

{
  ulonglong uVar1;
  
  FUN_14093c610(param_2);
  uVar1 = func_0x00014093c290(param_2,param_3);
  *(undefined8 *)(param_1 + 0xc0) = *param_2;
  *(undefined8 *)(param_1 + 200) = param_2[1];
  *(undefined8 *)(param_1 + 0xd0) = param_2[2];
  *(ulonglong *)(param_1 + 0xd8) = uVar1 ^ _DAT_1421d3d30;
  *(undefined8 *)(param_1 + 0xe0) = *param_3;
  *(undefined8 *)(param_1 + 0xe8) = param_3[1];
  *(undefined8 *)(param_1 + 0xf0) = param_3[2];
  return;
}


// ===== FUN_141247e50 @ 141247e50

undefined4 FUN_141247e50(longlong param_1,int *param_2)

{
  undefined4 uVar1;
  
  if ((*param_2 < 0) || (1 < *param_2)) {
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined4 *)(param_1 + 0xfc + (longlong)*param_2 * 4);
  }
  return uVar1;
}


// ===== FUN_141247eb0 @ 141247eb0

undefined1 FUN_141247eb0(longlong param_1)

{
  return *(undefined1 *)(param_1 + 0xf9);
}


