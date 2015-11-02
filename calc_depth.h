#ifndef CALC_DIFF_H
#define CALC_DIFF_H

#include "utils.h"

void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement);

#endif
