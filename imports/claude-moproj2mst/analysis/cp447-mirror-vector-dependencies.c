// ===== FUN_140945050 @ 140945050

double FUN_140945050(double *param_1,double *param_2)

{
  return *param_1 * *param_2 + param_1[1] * param_2[1] + param_1[2] * param_2[2];
}


// ===== FUN_14093c610 @ 14093c610

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

double FUN_14093c610(double *param_1)

{
  double dVar1;
  
  dVar1 = (double)FUN_14093c500(param_1);
  if (dVar1 != _DAT_1421ad1d0) {
    *param_1 = *param_1 / dVar1;
    param_1[1] = param_1[1] / dVar1;
    param_1[2] = param_1[2] / dVar1;
  }
  return dVar1;
}


// ===== FUN_14093c290 @ 14093c290

double FUN_14093c290(double *param_1,double *param_2)

{
  return *param_1 * *param_2 + param_1[1] * param_2[1] + param_1[2] * param_2[2];
}


