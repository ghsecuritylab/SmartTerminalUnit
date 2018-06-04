/**
  * @file   imageArray.h
  * @author  guoweilkd
  * @version V1.1.0
  * @date    2018/05/20
  * @brief   
  */

#ifndef __IMAGEARRAY_H
#define	__IMAGEARRAY_H

#include "stdint.h"

/* ͼ����ʾ */
typedef struct{
	uint16_t wide;
	uint16_t high;
	const uint8_t *image;
	uint8_t imageNumber;
}ImageInfo;

enum ImageName{
	IMAGE_DEFAULT,
	IMAGE_SOJO,
};
extern ImageInfo imageTab[];
extern const uint8_t imageTabItems;
#endif /* END __IMAGEARRAY_H */
