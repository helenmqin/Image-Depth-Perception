#include "calc_depth.h"
#include "utils.h"
#include <math.h>
#include <limits.h>
#include <stdio.h>

/* Implements the normalized displacement function */
unsigned char normalized_displacement(int dx, int dy,
        int maximum_displacement) {

    double squared_displacement = dx * dx + dy * dy;
    double normalized_displacement = round(255 * sqrt(squared_displacement) / sqrt(2 * maximum_displacement * maximum_displacement));
    return (unsigned char) normalized_displacement;

}

unsigned int euclidean(unsigned char *leftfb, unsigned char *currfb, int size){
    unsigned int edis = (leftfb[0] - currfb[0]) * (leftfb[0]- currfb[0]);
    for (int i = 1; i < size; i++){
        edis = edis + ((leftfb[i]-currfb[i]) * (leftfb[i]-currfb[i]));

    }
    return edis;   
}


void calc_depth(unsigned char *depth_map, unsigned char *left,
        unsigned char *right, int image_width, int image_height,
        int feature_width, int feature_height, int maximum_displacement) {

    int mapsize = image_width * image_height;
    int ftrboxw = (feature_width * 2) + 1;
    int ftrboxh = (feature_height * 2) + 1;
    int ftrboxsize = ftrboxw * ftrboxh;
    int srchboxwidth = feature_width + maximum_displacement;
    int srchboxheight = feature_height + maximum_displacement;
    int j = 0;

    if (maximum_displacement == 0){
        for (int z = 0; z < mapsize; z++){
            depth_map[z] = 0;
        }
    }else{   
        while (j < mapsize) {
        int x = j % image_width;
        int y = (j - (j % image_width))/image_width;

            if (feature_width > x || feature_width >= image_width - x ||
                feature_height > y || feature_height >= image_height - y) {
                depth_map[j] = 0;
            }else{
                int startx, starty, endx, endy;
                /* Establish search box boundaries */
                startx = x - srchboxwidth;
                starty = y - srchboxheight;
                endx = x + srchboxwidth;
                endy = y + srchboxheight; 
                if (startx < 0){
                    startx = 0;
                }
                if (starty < 0){
                    starty = 0;
                }
                if (endx >= image_width){
                    endx = image_width - 1;
                }
                if (endy >= image_height){
                    endy = image_height - 1;
                }

                /* Creates an array for left-image feature box centered around j */
                unsigned char leftftrbox[ftrboxsize];
                int lstart = ((y-feature_height)*image_width) + (x-feature_width);
                int count = 0;

                for (int s = lstart; s <= lstart + ((ftrboxh-1)*image_width); s=s+image_width){
                    for (int t = 0; t < ftrboxw; t++){
                        leftftrbox[t+count] = left[s+t];
                    }
                    count = count+ftrboxw;
                }
            
                int start = (starty * image_width) + startx;
                int iterendx = endx - (startx + (ftrboxw-1));
                int iterendy = endy - (starty + (ftrboxh-1));            
                int abs(int x);

                unsigned char normdis;
                unsigned char normdistemp;
                unsigned int tempedistnc;
                unsigned int edistance;
                int temp = 0;
                /* Iterate through all right-image feature box in search box */
                for (int k = start; k <= start + iterendx; k++){
                    for (int m = k; m <= k+(iterendy*image_width); m=m+image_width){
                        /* Creates an array for each right-image feature box centered around m */
                        unsigned char currftrbox[ftrboxsize];
                        int counter = 0;
                        for (int p = m; p <= m + ((ftrboxh-1)*image_width); p=p+image_width){
                            for (int n = 0; n < ftrboxw; n++) {
                                currftrbox[n+counter] = right[n+p];
                            }
                            counter = counter+ftrboxw;
                        }
                        int centerftrx = (m+feature_width)%image_width;
                        int centerftry = ((m+ (feature_height*image_width)) -((m+ (feature_height*image_width))%image_width)) / image_width;
                        int xloc = abs(centerftrx - x);
                        int yloc = abs(centerftry - y);
                        normdistemp = normalized_displacement(xloc, yloc, maximum_displacement);                    
                        if (temp == 0) {
                            normdis = normalized_displacement(xloc, yloc, maximum_displacement);
                            edistance = euclidean(leftftrbox, currftrbox, ftrboxsize);
                        }else{
                        
                            tempedistnc = euclidean(leftftrbox, currftrbox, ftrboxsize);
                            if (tempedistnc < edistance) {
                                edistance = tempedistnc;
                                normdis = normalized_displacement(xloc, yloc, maximum_displacement);                                        }
                            if (tempedistnc == edistance){
                                if (normdistemp < normdis){
                                    normdis = normdistemp;
                                }
                            }
                        }   
                        temp++;
                    }
                }   
                depth_map[j] = normdis;
            }
            j++;
        }
    }
}


