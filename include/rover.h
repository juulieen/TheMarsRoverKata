#pragma once

#include "position.h"
#include "orientation.h"
#include "map_limit.h"

typedef struct rover_s
{
    pos_t pos;
    enum orientations_e o;
} rover_t;

extern bool parse_rover_from(const char *s, rover_t *rover)
    __attribute__((__warn_unused_result__, nonnull(1, 2), pure));

extern bool go_forward(rover_t *rover, const map_limit_t map)
    __attribute__((__warn_unused_result__, nonnull(1), pure));

extern bool rover_take_action(rover_t *rover, const char action, const map_limit_t *map) __attribute__((__warn_unused_result__, nonnull(1, 3), pure));

extern void turn_left(rover_t *rover) __attribute__((nonnull(1)));

extern void turn_right(rover_t *rover) __attribute__((nonnull(1)));