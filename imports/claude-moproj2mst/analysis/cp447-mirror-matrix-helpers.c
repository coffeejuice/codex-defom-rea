// ===== FUN_140945d20 @ 140945d20

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140945d20(longlong param_1)

{
  undefined2 uStack_18;
  undefined2 uStack_14;
  
  for (uStack_18 = 0; uStack_18 < 4; uStack_18 = uStack_18 + 1) {
    for (uStack_14 = 0; uStack_14 < 4; uStack_14 = uStack_14 + 1) {
      if (uStack_18 == uStack_14) {
        *(undefined8 *)(param_1 + (longlong)uStack_18 * 0x20 + (longlong)uStack_14 * 8) =
             _DAT_1421249e0;
      }
      else {
        *(undefined8 *)(param_1 + (longlong)uStack_18 * 0x20 + (longlong)uStack_14 * 8) = 0;
      }
    }
  }
  return;
}


// ===== FUN_140946ca0 @ 140946ca0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140946ca0(double *param_1,double *param_2,undefined8 param_3)

{
  double dVar1;
  
  dVar1 = (double)func_0x000140945050(param_2,param_3);
  *param_1 = _DAT_1421249e0 - _DAT_142101520 * *param_2 * *param_2;
  param_1[5] = _DAT_1421249e0 - _DAT_142101520 * param_2[1] * param_2[1];
  param_1[10] = _DAT_1421249e0 - _DAT_142101520 * param_2[2] * param_2[2];
  param_1[0xf] = _DAT_1421249e0;
  param_1[1] = _DAT_142680a30 * *param_2 * param_2[1];
  param_1[2] = _DAT_142680a30 * *param_2 * param_2[2];
  param_1[6] = _DAT_142680a30 * param_2[1] * param_2[2];
  param_1[4] = param_1[1];
  param_1[8] = param_1[2];
  param_1[9] = param_1[6];
  param_1[0xc] = _DAT_142101520 * dVar1 * *param_2;
  param_1[0xd] = _DAT_142101520 * dVar1 * param_2[1];
  param_1[0xe] = _DAT_142101520 * dVar1 * param_2[2];
  param_1[3] = 0.0;
  param_1[7] = 0.0;
  param_1[0xb] = 0.0;
  return;
}


// ===== FUN_140944d10 @ 140944d10

void FUN_140944d10(double *param_1,double *param_2,double *param_3)

{
  *param_3 = *param_1 + *param_2;
  param_3[1] = param_1[1] + param_2[1];
  param_3[2] = param_1[2] + param_2[2];
  return;
}


// ===== FUN_1409464c0 @ 1409464c0

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_1409464c0(longlong param_1,double *param_2)

{
  double adStack_40060 [32767];
  short sStack_68;
  double adStack_60 [4];
  double adStack_40 [4];
  double dStack_20;
  short sStack_18;
  
  adStack_60[0] = *param_2;
  adStack_60[1] = param_2[1];
  adStack_60[2] = param_2[2];
  adStack_60[3] = (double)_DAT_1421249e0;
  for (sStack_68 = 0; sStack_68 < 4; sStack_68 = sStack_68 + 1) {
    dStack_20 = 0.0;
    for (sStack_18 = 0; sStack_18 < 4; sStack_18 = sStack_18 + 1) {
      dStack_20 = dStack_20 +
                  *(double *)(param_1 + (longlong)sStack_18 * 0x20 + (longlong)sStack_68 * 8) *
                  adStack_60[sStack_18];
    }
    adStack_40[sStack_68] = dStack_20;
  }
  *param_2 = adStack_40[0];
  param_2[1] = adStack_40[1];
  param_2[2] = adStack_40[2];
  return;
}


// ===== FUN_140944f20 @ 140944f20

void FUN_140944f20(double *param_1,double *param_2,double *param_3)

{
  *param_3 = *param_1 - *param_2;
  param_3[1] = param_1[1] - param_2[1];
  param_3[2] = param_1[2] - param_2[2];
  return;
}


// ===== FUN_141247e90 @ 141247e90

undefined1 FUN_141247e90(longlong param_1)

{
  return *(undefined1 *)(param_1 + 0xf8);
}


// ===== FUN_140945e70 @ 140945e70

void FUN_140945e70(longlong param_1,longlong param_2)

{
  undefined2 uStack_18;
  undefined2 uStack_14;
  
  for (uStack_18 = 0; uStack_18 < 4; uStack_18 = uStack_18 + 1) {
    for (uStack_14 = 0; uStack_14 < 4; uStack_14 = uStack_14 + 1) {
      *(undefined8 *)(param_1 + (longlong)uStack_18 * 0x20 + (longlong)uStack_14 * 8) =
           *(undefined8 *)(param_2 + (longlong)uStack_18 * 0x20 + (longlong)uStack_14 * 8);
    }
  }
  return;
}


// ===== FUN_140946040 @ 140946040

void FUN_140946040(longlong param_1,longlong param_2)

{
  short sStack_a8;
  double adStack_98 [16];
  double dStack_18;
  short sStack_10;
  short sStack_c;
  
  for (sStack_a8 = 0; sStack_a8 < 4; sStack_a8 = sStack_a8 + 1) {
    for (sStack_c = 0; sStack_c < 4; sStack_c = sStack_c + 1) {
      dStack_18 = 0.0;
      for (sStack_10 = 0; sStack_10 < 4; sStack_10 = sStack_10 + 1) {
        dStack_18 = dStack_18 +
                    *(double *)(param_1 + (longlong)sStack_10 * 0x20 + (longlong)sStack_a8 * 8) *
                    *(double *)(param_2 + (longlong)sStack_c * 0x20 + (longlong)sStack_10 * 8);
      }
      adStack_98[(longlong)sStack_c * 4 + (longlong)sStack_a8] = dStack_18;
    }
  }
  FUN_140945e70(param_1,adStack_98);
  return;
}


