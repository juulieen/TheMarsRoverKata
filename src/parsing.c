#include <stdbool.h>
#include <stdio.h>
#include "orientation.h"
#include "map_limit.h"
#include "rover.h"

bool parse_orientation_from(const char o, orientations_t *orientation)
{
    const char *orientations = "NESW";

    for (int i = 0; i < 4; ++i)
    {
        if (orientations[i] == o)
        {
            *orientation = (orientations_t)i;
            return false;
        }
    }
    return true;
}

bool parse_map_limit_from(const char *s, map_limit_t *map)
{
    if (sscanf(s, "%d %d", &map->x, &map->y) != 2)
        return true;

    /* the map limit are the coord of the last elem from the grid +1
     * +-+-+-+
     * | | | |      the map limit are 3,3
     * +-+-+-+
     * | | | | <- +1
     * +-+-+-+
     *      ^
     *      +1
    */
    map->x += 1;
    map->y += 1;
    return false;
}

bool parse_rover_from(const char *s, rover_t *rover)
{
    char o = 0;
    if (sscanf(s, "%d %d %c", &(rover->pos.x), &rover->pos.y, &o) != 3)
        return true;
    return parse_orientation_from(o, &rover->o);
}
