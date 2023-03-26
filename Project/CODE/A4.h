#ifndef _A4_H
#define _A4_H

#include "headfile.h"

extern uint8 image_threshold;
extern uint8 output_img[120][188];

uint8 otsuThreshold(uint8 *image, uint16 width, uint16 height);
void Binarization(uint8 input_pic[120][188], uint8 output_pic[120][188], int threshold);


#endif