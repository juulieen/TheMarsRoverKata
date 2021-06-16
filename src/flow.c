#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "map_limit.h"
#include "rover.h"

int default_error()
{
    char *error = "Should not happen check the input format\n";
    write(2, error, strlen(error));
    return 1;
}

bool get_and_parse_first_line(map_limit_t *map)
{
    char *line = NULL;
    size_t lene = 0;
    ssize_t lineSiz = 0;
    lineSiz = getline(&line, &lene, stdin);
    if (lineSiz == -1 || parse_map_limit_from(line, map))
    {
        free(line);
        return true;
    }
    free(line);
    return false;
}

bool apply_actions_to_rover(rover_t *rover, const char *actions, const map_limit_t *map)
{
    int nb_actions = strlen(actions);
    for (int i = 0; i < nb_actions; ++i)
    {
        if (rover_take_action(rover, actions[i], map))
            return true;
    }
    return false;
}

bool process_a_rover(const map_limit_t *map, const char *position, const char *actions)
{
    rover_t rover = {0};

    if (parse_rover_from(position, &rover) || apply_actions_to_rover(&rover, actions, map))
        return true;
    char s[100];
    sprintf(s, "%d %d %c\n", rover.pos.x, rover.pos.y, "NESW"[(int)rover.o]);
    write(1, s, strlen(s));
    return false;
}

bool process_rovers(map_limit_t *map)
{
    char *positions = NULL;
    char *actions = NULL;
    bool error = false;
    size_t len[2] = {0, 0};
    ssize_t lineSize[2] = {0, 0};

    lineSize[0] = getline(&positions, &(len[0]), stdin);
    lineSize[1] = getline(&actions, &(len[1]), stdin);
    error = (lineSize[0] == -1 || lineSize[1] == -1);
    while (!error && lineSize[0] != -1 && lineSize[1] != -1)
    {
        error = process_a_rover(map, positions, actions);
        lineSize[0] = getline(&positions, &(len[0]), stdin);
        lineSize[1] = getline(&actions, &(len[1]), stdin);
    }
    free(positions);
    free(actions);
    return error || (lineSize[0] != -1 && lineSize[1] == -1);
}

int the_mars_rover()
{
    map_limit_t map = {0};
    if (get_and_parse_first_line(&map))
        return default_error();
    if (process_rovers(&map))
        return default_error();
    return 0;
}