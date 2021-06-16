#include <stdbool.h>
#include "rover.h"

bool go_forward(rover_t *rover, const map_limit_t map)
{
    switch (rover->o)
    {
    case NORTH:
        rover->pos.y = (rover->pos.y + 1) % map.y;
        break;
    case EAST:
        rover->pos.x = (rover->pos.x + 1) % map.x;
        break;
    case SOUTH:
        rover->pos.y = (rover->pos.y + map.y - 1) % map.y;
        break;
    case WEST:
        rover->pos.x = (rover->pos.x + map.x - 1) % map.x;
        break;
    default:
        return true;
    }
    return false;
}

bool rover_take_action(rover_t *rover, const char action, const map_limit_t *map)
{
    switch (action)
    {
    case 'L':
        turn_left(rover);
        break;
    case 'R':
        turn_right(rover);
        break;
    case 'M':
        return go_forward(rover, *map);
    case '\n':
        break;
    default:
        return true;
        break;
    }
    return false;
}

inline void turn_left(rover_t *rover)
{
    rover->o = (rover->o + MAX_ORIENTATION - 1) % MAX_ORIENTATION;
}

inline void turn_right(rover_t *rover)
{
    rover->o = (rover->o + 1) % MAX_ORIENTATION;
}