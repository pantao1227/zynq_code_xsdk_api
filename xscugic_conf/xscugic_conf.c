/*
 * xscugic_conf.c
 *
 *  Created on: 2020年8月14日
 *      Author: Pan Tao
 */

#include <xscugic.h>

int init_intc(XScuGic * _pIntcInst, u16 _IntcDeviceId)
{
	XScuGic_Config * _pIntcCfg;
	int Status;

	_pIntcCfg = XScuGic_LookupConfig(_IntcDeviceId);
	if(NULL==_pIntcInst){
		xil_printf("[init_intc] Lookup configuration failed.\r\n");
		return 1;
	}

	Status = XScuGic_CfgInitialize(_pIntcInst, _pIntcCfg, _pIntcCfg->CpuBaseAddress);
	if(Status){
		xil_printf("[init_intc] Configuration initialize failed. Status = %d \r\n");
		return 1;
	}

	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(
			XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			(void *)_pIntcInst);
	Xil_ExceptionEnable();

	xil_printf("[init_intc] Success.\r\n");

	return 0;
}


int setup_intr(XScuGic * _pIntcInst,
		u32 _intrId,
		u8 _priority,
        Xil_InterruptHandler _intrHandler,
		void *CallBackRef)
{

	int Status;

	XScuGic_SetPriorityTriggerType(_pIntcInst, _intrId, _priority, 0x3);
	Status = XScuGic_Connect(_pIntcInst, _intrId, _intrHandler, CallBackRef);
	if(Status){
		xil_printf("[setup_intr] Connect failed.\r\n");
		return 1;
	}

	XScuGic_Enable(_pIntcInst, _intrId);

	xil_printf("[setup_intr] for Interrupt ID = 0x%08x.\r\n", _intrId);

	return 0;
}

