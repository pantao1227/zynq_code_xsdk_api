/*
 * video_format.h
 *
 *  Created on: 2020年8月14日
 *      Author: Pan Tao
 */

#ifndef VIDEO_FORMAT_H_
#define VIDEO_FORMAT_H_

/* Video Data Format */
#define VID_STRIDE				1920
#define VID_WIDTH				1920
#define VID_HEIGHT				1080
#define PIXEL_SIZE_BYTE			3
#define VID_FRAME_SIZE_BYTE	VID_STRIDE*VID_HEIGHT*PIXEL_SIZE_BYTE

#endif /* VIDEO_FORMAT_H_ */
