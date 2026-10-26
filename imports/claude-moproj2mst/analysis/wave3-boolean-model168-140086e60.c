
void FUN_140086e60(longlong param_1,undefined4 param_2,char param_3)

{
  undefined8 uVar1;
  longlong lVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [8];
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_28 = 0xfffffffffffffffe;
  if ((*(longlong *)(param_1 + 0x18) != 0) && (lVar2 = FUN_140086820(param_1,param_2), lVar2 != 0))
  {
    if (param_3 == '\0') {
      (**(code **)(**(longlong **)(param_1 + 0x18) + 0x238))
                (*(undefined8 *)(param_1 + 0x18),lVar2,0,2);
    }
    else {
      lVar3 = FUN_14146eabc(8);
      if (lVar3 == 0) {
        uStack_20 = 0;
      }
      else {
        uStack_20 = __0QIcon__QEAA_XZ(lVar3);
      }
      uVar1 = uStack_20;
      uVar4 = __0QSize__QEAA_XZ(auStack_38);
      uStack_18 = __0QString__QEAA_PEBD_Z(auStack_30,&UNK_14213b000);
      uStack_10 = uStack_18;
      _addFile_QIcon__QEAAXAEBVQString__AEBVQSize__W4Mode_1_W4State_1__Z(uVar1,uStack_18,uVar4,0,1);
      __1QString__QEAA_XZ(auStack_30);
      (**(code **)(**(longlong **)(param_1 + 0x18) + 0x238))
                (*(undefined8 *)(param_1 + 0x18),lVar2,uVar1,2);
    }
  }
  return;
}

