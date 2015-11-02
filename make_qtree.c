#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))


int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {
    unsigned char curr_color = depth_map[x + (y * map_width)];
    for(int i = 0; i < section_width; i = i + 1) {
        for(int j = 0; j < section_width; j = j + 1) {
            unsigned char next_color = depth_map[(x + i) + ((j + y) * map_width)];
            if (curr_color != next_color) {
                return 256;
            }
        }
    }
    return curr_color;
}

//check if enitre stucture is of a color 
//if not break into 4 parts and check 

qNode* depth_helper(unsigned char *depth_map, int map_width, int section_width, int x, int y) {
    
    qNode *node = (qNode*) malloc(sizeof(qNode));
    if (!(node)){
        allocation_failed();
    }

    int gray_value = homogenous(depth_map, map_width, x, y, section_width);
    node->gray_value = gray_value;
    node->size = section_width;
    node->x = x;
    node->y = y;

    if(gray_value == 256) {
        node->leaf = 0;

        node->child_NW = depth_helper(depth_map, map_width, .5 * section_width, x, y);
        node->child_NE = depth_helper(depth_map, map_width, .5 * section_width, x + (.5 * section_width), y);
        node->child_SE = depth_helper(depth_map, map_width, .5 * section_width, x + (.5 * section_width), y + (.5 * section_width));
        node->child_SW = depth_helper(depth_map, map_width, .5 * section_width, x, y + (.5 * section_width));
                        
    
    } else {
        node -> leaf = 1;
    }
    return node;
}


qNode *depth_to_quad(unsigned char *depth_map, int map_width) {
    return depth_helper(depth_map, map_width, map_width, 0, 0);
}


void free_qtree(qNode *qtree_node) {
    if(qtree_node) {
        if(!qtree_node->leaf){
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_SW);
        }
        free(qtree_node);
    }
}



