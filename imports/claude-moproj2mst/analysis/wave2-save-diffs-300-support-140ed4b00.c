
void FUN_140ed4b00(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  longlong lVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  undefined8 uVar4;
  
  uVar3 = FUN_140130af0(param_1);
  lVar1 = FUN_14005a400(uVar3);
  uVar3 = FUN_140130af0(param_1);
  uVar3 = func_0x0001401cc700(uVar3);
  uVar2 = 0;
  if (lVar1 != 0) {
    uVar2 = FUN_140004ae0(lVar1);
  }
  uVar4 = FUN_140130f90(param_1);
  FUN_140ed4bb0(param_1);
  _selectRow_QTableView__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1800),0);
  FUN_140ed6460(param_1,0,0,param_4,uVar2,uVar3,uVar4);
  return;
}

