
void FUN_14140d100(undefined8 param_1,undefined8 param_2)

{
  longlong *plVar1;
  longlong lVar2;
  undefined1 auStack_30 [8];
  undefined1 *puStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_20 = 0xfffffffffffffffe;
  plVar1 = (longlong *)func_0x0001402c8090(0x143d766b8);
  if (plVar1 == (longlong *)0x0) {
    __1QDomElement__QEAA_XZ(param_2);
  }
  else {
    lVar2 = (**(code **)(*plVar1 + 0x778))(plVar1);
    lVar2 = FUN_140cf94f0(lVar2 + 0x10);
    if (lVar2 != 0) {
      puStack_28 = auStack_30;
      uStack_18 = __0QDomNode__QEAA_AEBV0__Z(puStack_28,param_2);
      FUN_140c90bd0(lVar2,uStack_18,1);
    }
    __1QDomElement__QEAA_XZ(param_2);
  }
  return;
}

