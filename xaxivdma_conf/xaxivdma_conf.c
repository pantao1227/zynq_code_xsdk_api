/*
 * xaxivdma_conf.c
 *  
 *  Created on: 2020年8月14日
 *      Author: Pan Tao
 */

#include <xil_types.h>
#include <xaxivdma.h>
#include "../my_includes/video_format.h"

/* 我设定的 AXI VDMA 的数据宽度是64bits，因此我觉得这里应该64bit对齐 */
u8 VideoWriteBuffer[3][VID_FRAME_SIZE_BYTE] __attribute__((aligned(64)));
u8 VideoReadBuffer[3][VID_FRAME_SIZE_BYTE] __attribute__((aligned(64)));

XAxiVdma_FrameCounter FrameCfg;
XAxiVdma_DmaSetup ReadCfg;
XAxiVdma_DmaSetup WriteCfg;

/* Global tags */
volatile int vdma_r_index = 0;
volatile int vdma_w_index = 0;

int init_xaxivdma_3buf_rw(XAxiVdma * _pVdmaInst, u16 _VdmaDeviceId)
{
	int Status;
	XAxiVdma_Config * _pVdmaCfg;

	_pVdmaCfg = XAxiVdma_LookupConfig(_VdmaDeviceId);
	if(NULL==_pVdmaCfg){
		xil_printf("[init_vdma_3_buf] Lookup configuration failed.\r\n");
		return XST_FAILURE;
	}

	Status = XAxiVdma_CfgInitialize(_pVdmaInst, _pVdmaCfg, _pVdmaCfg->BaseAddress);
	if(Status){
		xil_printf("[init_vdma_3_buf] Configuration initialize failed. Status = %d. \r\n", Status);
		return Status;
	}

	FrameCfg.ReadFrameCount = 3;
	FrameCfg.WriteFrameCount = 3;
	FrameCfg.ReadDelayTimerCount = 0;
	FrameCfg.WriteDelayTimerCount = 0;
	Status = XAxiVdma_SetFrameCounter(_pVdmaInst, &FrameCfg);
	if(Status){
		xil_printf("[init_vdma_3buf] Set frame counter failed. Status = %d. \r\n", Status);
		return Status;
	}
	return 0;
}

int launch_xaxivdma_3buf_rw(XAxiVdma * _pVdma3bufRw)
{
	int Status;

	WriteCfg.VertSizeInput = VID_HEIGHT;
	WriteCfg.HoriSizeInput = VID_WIDTH*PIXEL_SIZE_BYTE;
	WriteCfg.Stride = VID_STRIDE*PIXEL_SIZE_BYTE;
	WriteCfg.FrameDelay=0;
	WriteCfg.EnableCircularBuf = 0;
	WriteCfg.EnableSync = 0;
	WriteCfg.PointNum = 0;
	WriteCfg.EnableFrameCounter = 0;
	WriteCfg.FixedFrameStoreAddr = 0;
	WriteCfg.GenLockRepeat = 0;
	WriteCfg.FrameStoreStartAddr[0] = (UINTPTR)VideoWriteBuffer[0];
	WriteCfg.FrameStoreStartAddr[1] = (UINTPTR)VideoWriteBuffer[1];
	WriteCfg.FrameStoreStartAddr[2] = (UINTPTR)VideoWriteBuffer[2];

	ReadCfg.VertSizeInput = VID_HEIGHT;
	ReadCfg.HoriSizeInput = VID_WIDTH*PIXEL_SIZE_BYTE;
	ReadCfg.Stride = VID_STRIDE*PIXEL_SIZE_BYTE;
	ReadCfg.FrameDelay = 0;
	ReadCfg.EnableCircularBuf = 0;
	ReadCfg.EnableSync = 0;
	ReadCfg.PointNum = 0;
	ReadCfg.EnableFrameCounter = 0;
	ReadCfg.FixedFrameStoreAddr = 0;
	ReadCfg.GenLockRepeat = 0;
	ReadCfg.FrameStoreStartAddr[0] = (UINTPTR)VideoReadBuffer[0];
	ReadCfg.FrameStoreStartAddr[1] = (UINTPTR)VideoReadBuffer[1];
	ReadCfg.FrameStoreStartAddr[2] = (UINTPTR)VideoReadBuffer[2];

	Status = XAxiVdma_StartWriteFrame(_pVdma3bufRw, &WriteCfg);
	if(Status != XST_SUCCESS){
		xil_printf("[startVdma] vdma write channel set buffer address failed %d\r\n", Status);
		return Status;
	}
	Status = XAxiVdma_StartReadFrame(_pVdma3bufRw, &ReadCfg);
	if(Status != XST_SUCCESS){
		xil_printf("[startVdma] vdma read channel set buffer address failed %d\r\n", Status);
		return Status;
	}

	XAxiVdma_IntrEnable(_pVdma3bufRw, XAXIVDMA_IXR_ALL_MASK, XAXIVDMA_WRITE);
	XAxiVdma_IntrEnable(_pVdma3bufRw, XAXIVDMA_IXR_ALL_MASK, XAXIVDMA_READ);

	XAxiVdma_StartParking(_pVdma3bufRw, 0, XAXIVDMA_WRITE);
	XAxiVdma_StartParking(_pVdma3bufRw, 0, XAXIVDMA_READ);

	return 0;
}

void clearVideoBuffer24bitColor(u8 * pBuffer, u8 r, u8 g, u8 b)
{
	int i;
	for(i = 0; i < VID_FRAME_SIZE_BYTE; i++)
	{
		if(i%3 == 0) *(pBuffer + i) = g;
		else if(i%3 == 1) *(pBuffer + i) = b;
		else *(pBuffer + i) = r;
	}
}

/*
 * The next number in 0, 1, 2, 0, 1, ...
 */
int next3(int num)
{
	if (num < 0)
		return 0;
	else if (num > 2)
		return 0;
	else
		return (num+1)%3;
}


