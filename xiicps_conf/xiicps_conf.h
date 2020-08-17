/*
 * iicps_conf.h
 *
 *  Created on: 2020年8月13日
 *      Author: Pan Tao
 */

#ifndef I2CPS_CONF_H_
#define I2CPS_CONF_H_

#include <xiicps.h>

/* Functions */
int ps_i2c_init(XIicPs* I2cPs, u16 DeviceId);
int i2c_write( XIicPs *I2cPs, u8 Address, u8 Data, s32 ByteCount);
int i2c_write2( XIicPs *I2cPs, u8 Address, u8 Register, u8 Data );
int i2c_read( XIicPs *I2cPs, u8 Address, u8 Register, u8 *Data, int ByteCount);

#endif /* I2CPS_CONF_H_ */
