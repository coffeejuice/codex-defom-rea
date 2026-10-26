
void FUN_140ed5dd0(longlong *param_1)

{
  int iVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong lVar4;
  int *piVar5;
  undefined8 uVar6;
  longlong lVar7;
  int iStack_58;
  int iStack_54;
  int iStack_48;
  int iStack_38;
  int iStack_20;
  int iStack_18;
  
  (**(code **)(*param_1 + 0x198))(param_1);
  lVar2 = (**(code **)(*param_1 + 0x198))(param_1);
  if ((lVar2 != 0) && (lVar2 = FUN_140130f90(param_1), lVar2 != 0)) {
    uVar3 = FUN_140130af0(param_1);
    lVar4 = FUN_14005a400(uVar3);
    uVar3 = FUN_140130af0(param_1);
    uVar3 = func_0x0001401cc700(uVar3);
    if (0 < *(int *)(lVar2 + 0xb64)) {
      FUN_14089b1f0(lVar2 + 0xb48);
    }
    *(undefined4 *)(lVar2 + 0xb64) = 0;
    *(undefined4 *)(lVar2 + 0xb88) = 0;
    iStack_58 = 0;
    iStack_48 = 0;
    iStack_38 = 0;
    if (lVar4 != 0) {
      iStack_38 = FUN_140004ae0(lVar4);
      for (iStack_54 = 0; iStack_54 < iStack_38; iStack_54 = iStack_54 + 1) {
        piVar5 = (int *)FUN_140067100(lVar4,iStack_54);
        if (*piVar5 == 0) {
          iStack_58 = iStack_58 + 1;
        }
        else {
          iStack_48 = iStack_48 + 1;
        }
      }
    }
    for (iStack_54 = 0; iVar1 = FUN_140004ae0(uVar3), iStack_54 < iVar1; iStack_54 = iStack_54 + 1)
    {
      piVar5 = (int *)FUN_140067100(uVar3,iStack_54);
      if (*piVar5 == 0) {
        iStack_58 = iStack_58 + 1;
      }
      else {
        iStack_48 = iStack_48 + 1;
      }
    }
    uVar6 = FUN_140130af0(param_1);
    FUN_14005a3a0(uVar6,lVar4);
    uVar6 = FUN_140130af0(param_1);
    FUN_140f34b60(uVar6,uVar3);
    if (0 < iStack_58) {
      *(int *)(lVar2 + 0xb64) = iStack_58;
      *(undefined4 *)(lVar2 + 0xb88) = 2;
      FUN_14089b840(lVar2 + 0xb48);
      iStack_20 = 0;
      for (iStack_54 = 0; iStack_54 < iStack_38; iStack_54 = iStack_54 + 1) {
        piVar5 = (int *)FUN_140067100(lVar4,iStack_54);
        if (*piVar5 == 0) {
          lVar7 = FUN_140067100(lVar4,iStack_54);
          *(double *)(*(longlong *)(lVar2 + 0xb70) + (longlong)iStack_20 * 8) =
               (double)*(int *)(lVar7 + 4);
          lVar7 = FUN_140067100(lVar4,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb90) + (longlong)(iStack_20 * 2) * 8) =
               *(undefined8 *)(lVar7 + 8);
          lVar7 = FUN_140067100(lVar4,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb90) + (longlong)(iStack_20 * 2 + 1) * 8) =
               *(undefined8 *)(lVar7 + 0x10);
          iStack_20 = iStack_20 + 1;
        }
      }
      for (iStack_54 = 0; iVar1 = FUN_140004ae0(uVar3), iStack_54 < iVar1; iStack_54 = iStack_54 + 1
          ) {
        piVar5 = (int *)FUN_140067100(uVar3,iStack_54);
        if (*piVar5 == 0) {
          lVar7 = FUN_140067100(uVar3,iStack_54);
          *(double *)(*(longlong *)(lVar2 + 0xb70) + (longlong)iStack_20 * 8) =
               (double)*(int *)(lVar7 + 4);
          lVar7 = FUN_140067100(uVar3,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb90) + (longlong)(iStack_20 * 2) * 8) =
               *(undefined8 *)(lVar7 + 8);
          lVar7 = FUN_140067100(uVar3,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb90) + (longlong)(iStack_20 * 2 + 1) * 8) =
               *(undefined8 *)(lVar7 + 0x10);
          iStack_20 = iStack_20 + 1;
        }
      }
    }
    if (0 < iStack_48) {
      *(int *)(lVar2 + 0xaec) = iStack_48;
      *(undefined4 *)(lVar2 + 0xb10) = 2;
      FUN_14089b840(lVar2 + 0xad0);
      iStack_18 = 0;
      for (iStack_54 = 0; iStack_54 < iStack_38; iStack_54 = iStack_54 + 1) {
        piVar5 = (int *)FUN_140067100(lVar4,iStack_54);
        if (*piVar5 == 1) {
          lVar7 = FUN_140067100(lVar4,iStack_54);
          *(double *)(*(longlong *)(lVar2 + 0xaf8) + (longlong)iStack_18 * 8) =
               (double)*(int *)(lVar7 + 4);
          lVar7 = FUN_140067100(lVar4,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb18) + (longlong)(iStack_18 * 2) * 8) =
               *(undefined8 *)(lVar7 + 8);
          lVar7 = FUN_140067100(lVar4,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb18) + (longlong)(iStack_18 * 2 + 1) * 8) =
               *(undefined8 *)(lVar7 + 0x10);
          iStack_18 = iStack_18 + 1;
        }
      }
      for (iStack_54 = 0; iVar1 = FUN_140004ae0(uVar3), iStack_54 < iVar1; iStack_54 = iStack_54 + 1
          ) {
        piVar5 = (int *)FUN_140067100(uVar3,iStack_54);
        if (*piVar5 == 1) {
          lVar4 = FUN_140067100(uVar3,iStack_54);
          *(double *)(*(longlong *)(lVar2 + 0xaf8) + (longlong)iStack_18 * 8) =
               (double)*(int *)(lVar4 + 4);
          lVar4 = FUN_140067100(uVar3,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb18) + (longlong)(iStack_18 * 2) * 8) =
               *(undefined8 *)(lVar4 + 8);
          lVar4 = FUN_140067100(uVar3,iStack_54);
          *(undefined8 *)(*(longlong *)(lVar2 + 0xb18) + (longlong)(iStack_18 * 2 + 1) * 8) =
               *(undefined8 *)(lVar4 + 0x10);
          iStack_18 = iStack_18 + 1;
        }
      }
    }
    uVar3 = FUN_140130f90(param_1);
    FUN_140ec7500(param_1,uVar3);
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    uVar3 = FUN_140c7acd0(uVar3);
    FUN_14082aaa0(uVar3,0x22);
    uVar3 = (**(code **)(*param_1 + 0x198))(param_1);
    uVar3 = FUN_140c7acd0(uVar3);
    FUN_14082aaa0(uVar3,0x3f);
  }
  return;
}

