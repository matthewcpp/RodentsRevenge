#include "spawn.h"

#include <stdlib.h>

#define RR_SPAWN_POS_COUNT 8

rrPoint spawn_positions[RR_SPAWN_POS_COUNT] = {
        {1, 1}, {21, 1}, {21, 1}, {21, 21},
        {11, 1}, {21, 11}, {11, 21}, {1, 11}
};

void randomize_index_array(int* spawn_points, int size) {
    int i;

    for (i = 0; i < size; i++)
        spawn_points[i] = i;

    for (i = 0; i < size; i++) {
        int swap_index = rand() % size;
        int temp = spawn_points[i];
        spawn_points[i] = spawn_points[swap_index];
        spawn_points[swap_index] = temp;
    }
}

void rr_get_spawn_pos(rrGrid* grid, rrPoint* position) {
    int i;
    int spawn_points[RR_SPAWN_POS_COUNT];
    randomize_index_array(spawn_points, RR_SPAWN_POS_COUNT);

    for (i = 0; i < RR_SPAWN_POS_COUNT; i++) {
        rrPoint* pos = spawn_positions + spawn_points[i];
        if (rr_grid_get_cell(grid, pos) == NULL){
            rr_point_copy(position, pos);
            break;
        }
    }
}