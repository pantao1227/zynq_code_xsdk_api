/*
 * xaxivdma_conf.h
 *
 *  Created on: 2020年8月14日
 *      Author: Pan Tao
 */

#ifndef XAXIVDMA_CONF_H_
#define XAXIVDMA_CONF_H_

#include <xaxivdma.h>
#include "../my_includes/video_format.h"

#define VW	XAXIVDMA_WRITE
#define VR	XAXIVDMA_READ

int init_xaxivdma_3buf_rw(XAxiVdma * _pVdmaInst, u16 _VdmaDeviceId);
int launch_xaxivdma_3buf_rw(XAxiVdma * _pVdma3bufRw);
void clearVideoBuffer24bitColor(u8 * pBuffer, u8 r, u8 g, u8 b);
int next3(int num);

extern u8 VideoWriteBuffer[3][VID_FRAME_SIZE_BYTE] __attribute__((aligned(64)));
extern u8 VideoReadBuffer[3][VID_FRAME_SIZE_BYTE] __attribute__((aligned(64)));
extern volatile int vdma_r_index;
extern volatile int vdma_w_index;

#endif /* XAXIVDMA_CONF_H_ */
