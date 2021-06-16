#pragma once

typedef enum orientations_e
{
    NORTH,
    EAST,
    SOUTH,
    WEST,
    MAX_ORIENTATION
} orientations_t;

extern bool parse_orientation_from(const char o, orientations_t *orientation) __attribute__((__warn_unused_result__, nonnull(2), pure));