'use strict';
const base=Process.mainModule.base;
for(const rva of [0xa2200,0x12bcc0,0x1cae90,0xe29270,0xeaf030]){
 Interceptor.attach(base.add(rva),{onEnter(){send({kind:'route_entry',utc:new Date().toISOString(),pid:Process.id,rva:'0x'+rva.toString(16),return_address:this.returnAddress.toString(),return_rva:this.returnAddress.sub(base).toString()});}});
}
send({kind:'route_probe_ready',utc:new Date().toISOString(),pid:Process.id,base:base.toString()});
