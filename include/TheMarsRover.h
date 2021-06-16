#pragma once

#include <stdbool.h>
#include "orientation.h"
#include "position.h"
#include "map_limit.h"
#include "rover.h"


// This will read the stdin line by line to gather information about map limitation and rovers on it
int the_mars_rover();


bool process_rovers(map_limit_t *);
bool process_a_rover(const map_limit_t *, const char *, const char *);
bool apply_actions_to_rover(rover_t *, const char *, const map_limit_t *);
bool get_and_parse_first_line(map_limit_t *);


