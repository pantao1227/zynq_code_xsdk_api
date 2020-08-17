/*
 * i2cps_conf.c
 *
 * 	配置ADV7611和ADV7511需要用到的I2C
 *
 *  Created on: 2020年8月13日
 *      Author: Pan Tao
 */

#include <xiicps.h>

#define I2C_SCLK_RATE		100000

int ps_i2c_init(XIicPs* I2cPs, u16 DeviceId)
{
	int Status;
	XIicPs_Config *I2cPsConfig;

	I2cPsConfig = XIicPs_LookupConfig(DeviceId);
	if (NULL == I2cPsConfig) {
		return XST_FAILURE;
	}

	Status = XIicPs_CfgInitialize(I2cPs, I2cPsConfig, I2cPsConfig->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* 设置I2C速率 */
	Status = XIicPs_SetSClk(I2cPs, I2C_SCLK_RATE);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int i2c_write( XIicPs *I2cPs, u8 Address, u8 Data, s32 ByteCount)
{
	/* XSDK I2C API 使用的是7bit 地址 */
	Address = Address>>1;

	int Status;

	/* 等待I2C空闲 */
	while (XIicPs_BusIsBusy(I2cPs)) {
		/* NOP */
	}

	/* 根据XSDK例程，发送数据 */
	Status = XIicPs_MasterSendPolled(I2cPs, &Data, ByteCount, Address);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int i2c_write2( XIicPs *I2cPs, u8 Address, u8 Register, u8 Data )
{
	/* XSDK I2C API 使用的是7bit 地址 */
	Address = Address>>1;

	u8 WriteBuffer[2];
	int Status;

	/* 地址在前，数据在后 */
	WriteBuffer[0] = Register;
	WriteBuffer[1] = Data;


	/* 等待I2C空闲 */
	while (XIicPs_BusIsBusy(I2cPs)) {
		/* NOP */
	}

	/* 发送数据 */
	Status = XIicPs_MasterSendPolled(I2cPs, WriteBuffer, 2, Address);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int i2c_read( XIicPs *I2cPs, u8 Address, u8 Register, u8 *Data, int ByteCount)
{
	/* XSDK I2C API 使用的是7bit 地址 */
	Address = Address>>1;

	int Status;

	/* 等待I2C空闲 */
	while (XIicPs_BusIsBusy(I2cPs)) {
		/* NOP */
	}

	/* 设置这个选项，才能符合ADV7511、ADV7611的时序要求 */
	Status = XIicPs_SetOptions(I2cPs, XIICPS_REP_START_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/* 设备地址、寄存器地址 */
	Status = XIicPs_MasterSendPolled(I2cPs, &Register, 1, Address);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* 接收数据 */
	Status = XIicPs_MasterRecvPolled(I2cPs, Data, ByteCount, Address);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	// 清除配置
	Status = XIicPs_ClearOptions(I2cPs, XIICPS_REP_START_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

