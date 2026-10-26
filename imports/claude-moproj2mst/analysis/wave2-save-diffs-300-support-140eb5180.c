
void FUN_140eb5180(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_20 = 0xfffffffffffffffe;
  lVar2 = FUN_140130f90(param_1);
  uVar1 = _currentRow_QListWidget__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 8));
  FUN_140eca080(param_1);
  FUN_140131110(param_1);
  uStack_18 = _fromLocal8Bit_QString__SA_AV1_PEBDH_Z
                        (auStack_28,lVar2 + 0x51,0xffffffff,param_4,uVar1);
  uStack_10 = uStack_18;
  uVar3 = FUN_140130af0(param_1);
  FUN_140050a90(uVar3,uStack_10);
  __1QString__QEAA_XZ(auStack_28);
  FUN_140eb5250(param_1);
  FUN_140ece7b0(param_1,0);
  FUN_140eb5500(param_1);
  return;
}

