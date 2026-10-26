
void FUN_14082a360(longlong param_1,char param_2)

{
  char cVar1;
  int iVar2;
  int aiStack_1c8 [80];
  int iStack_88;
  undefined1 auStack_80 [32];
  longlong lStack_60;
  longlong lStack_58;
  int iStack_50;
  int iStack_4c;
  longlong lStack_48;
  longlong lStack_40;
  longlong lStack_38;
  longlong lStack_30;
  int iStack_28;
  int iStack_24;
  undefined4 uStack_20;
  undefined1 uStack_1c;
  undefined1 uStack_1b;
  undefined1 uStack_1a;
  undefined1 uStack_19;
  undefined1 uStack_18;
  undefined1 auStack_17 [7];
  undefined8 uStack_10;
  
  uStack_10 = 0xfffffffffffffffe;
  iStack_4c = 0;
  iStack_50 = 0;
  lStack_58 = 0;
  lStack_60 = 0;
  for (iStack_88 = 0; iStack_88 < 0x4f; iStack_88 = iStack_88 + 1) {
    aiStack_1c8[iStack_88] = *(int *)(param_1 + 0x15f8 + (longlong)iStack_88 * 4);
  }
  FUN_14084e8e0(auStack_80,0);
  func_0x00014084e920(auStack_80);
  func_0x0001408316d0(auStack_80,param_1);
  if (param_2 != '\0') {
    lStack_58 = func_0x000140b172d0(param_1 + 0x1328);
    lStack_60 = func_0x0001401cc700(param_1 + 0x1328);
  }
  iStack_50 = 0;
  while (iVar2 = func_0x0001407d3070(param_1 + 0x1380), iStack_50 < iVar2) {
    lStack_48 = FUN_14084eca0(param_1 + 0x1380,iStack_50 + 1);
    if (((param_2 != '\0') && (lStack_48 != 0)) &&
       ((cVar1 = FUN_140834fa0(lStack_48), cVar1 != '\0' && ((lStack_58 != 0 && (lStack_60 != 0)))))
       ) {
      lStack_40 = FUN_14084eaa0(auStack_80);
      uStack_1c = 1;
      uStack_1b = 0;
      uStack_1a = 1;
      uStack_19 = 1;
      uStack_18 = 1;
      auStack_17[0] = 1;
      FUN_140835430(lStack_40,lStack_48,param_1 + 0x15f8,auStack_17,&uStack_18,&uStack_19,&uStack_1a
                    ,&uStack_1b,&uStack_1c);
      *(undefined4 *)(lStack_40 + 8) = *(undefined4 *)(lStack_48 + 8);
    }
    iStack_50 = iStack_50 + 1;
  }
  FUN_1408296c0(param_1);
  iStack_50 = 0;
  while (iVar2 = func_0x0001407d3070(param_1 + 0x1380), iStack_50 < iVar2) {
    lStack_38 = FUN_14084eca0(param_1 + 0x1380,iStack_50 + 1);
    if (((((lStack_38 != 0) && (cVar1 = FUN_140834fa0(lStack_38), cVar1 != '\0')) &&
         (FUN_14086d7f0(lStack_38,param_1 + 0x15f8,aiStack_1c8), param_2 != '\0')) &&
        ((lStack_58 != 0 && (lStack_60 != 0)))) &&
       ((lStack_30 = FUN_14084ed10(auStack_80,*(undefined4 *)(lStack_38 + 8)), lStack_30 != 0 &&
        (cVar1 = FUN_14082f8c0(param_1,0x31,0), cVar1 != '\0')))) {
      for (iStack_4c = 0; iStack_4c < *(int *)(param_1 + 0x16bc); iStack_4c = iStack_4c + 1) {
        for (iStack_88 = 0; iStack_88 < *(int *)(lStack_38 + 0x884); iStack_88 = iStack_88 + 1) {
          *(undefined8 *)
           (*(longlong *)(lStack_38 + 0xa58) +
           (longlong)(iStack_88 * *(int *)(param_1 + 0x16bc) + iStack_4c) * 8) = 0;
          *(undefined8 *)
           (*(longlong *)(lStack_38 + 0xa60) +
           (longlong)(iStack_88 * *(int *)(param_1 + 0x16bc) + iStack_4c) * 8) = 0;
        }
      }
      for (iStack_4c = 0; iStack_4c < aiStack_1c8[0x31]; iStack_4c = iStack_4c + 1) {
        uStack_20 = *(undefined4 *)(lStack_58 + (longlong)iStack_4c * 4);
        iStack_24 = *(int *)(lStack_60 + (longlong)iStack_4c * 4);
        iStack_28 = FUN_1408bc900(param_1 + 0x1328,uStack_20);
        iVar2 = FUN_1408bc880(param_1 + 0x1328,uStack_20);
        if (-1 < iVar2) {
          for (iStack_88 = 0; iStack_88 < *(int *)(lStack_38 + 0x884); iStack_88 = iStack_88 + 1) {
            *(undefined8 *)
             (*(longlong *)(lStack_38 + 0xa58) +
             (longlong)(iStack_88 * *(int *)(param_1 + 0x16bc) + iStack_28) * 8) =
                 *(undefined8 *)
                  (*(longlong *)(lStack_30 + 0xa58) +
                  (longlong)(iStack_88 * aiStack_1c8[0x31] + iStack_24) * 8);
            *(undefined8 *)
             (*(longlong *)(lStack_38 + 0xa60) +
             (longlong)(iStack_88 * *(int *)(param_1 + 0x16bc) + iStack_28) * 8) =
                 *(undefined8 *)
                  (*(longlong *)(lStack_30 + 0xa60) +
                  (longlong)(iStack_88 * aiStack_1c8[0x31] + iStack_24) * 8);
          }
        }
      }
    }
    iStack_50 = iStack_50 + 1;
  }
  FUN_14084ed90(auStack_80);
  FUN_14084e960(auStack_80);
  return;
}

