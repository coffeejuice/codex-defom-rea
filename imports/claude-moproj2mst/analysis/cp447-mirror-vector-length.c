// ===== FUN_14093c500 @ 14093c500

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 FUN_14093c500(double *param_1)

{
  double dVar1;
  undefined8 uVar2;
  
  dVar1 = *param_1 * *param_1 + param_1[1] * param_1[1] + param_1[2] * param_1[2];
  if (dVar1 == _DAT_1421ad1d0) {
    uVar2 = 0;
  }
  else {
    uVar2 = FUN_141471bbc(dVar1);
  }
  return uVar2;
}


