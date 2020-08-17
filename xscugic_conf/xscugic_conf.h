/*
 * xscugic_conf.h
 *
 *  Created on: 2020年8月14日
 *      Author: Pan Tao
 */

#ifndef XSCUGIC_CONF_H_
#define XSCUGIC_CONF_H_

#include <xscugic.h>

int init_intc(XScuGic * _pIntcInst, u16 _IntcDeviceId);
int setup_intr(XScuGic * _pIntcInst,
		u32 _intrId,
		u8 _priority,
        Xil_InterruptHandler _intrHandler,
		void *CallBackRef);

#endif /* XSCUGIC_CONF_H_ */
