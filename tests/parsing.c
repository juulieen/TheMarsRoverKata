#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "orientation.h"
#include "map_limit.h"
#include "rover.h"

TestSuite(Parsing,
          .init = NULL,
          .fini = NULL,
          .signal = 0,
          .exit_code = 0,
          .disabled = 0,
          .description = "Tests the parsing of the Data-Structure",
          .timeout = 0);

struct orientation_params
{
    const char o;
    const orientations_t expected;
    const bool error;
};

ParameterizedTestParameters(Parsing, parse_orientation_from)
{
    static struct orientation_params params[5] = {
        {'N', NORTH, false},
        {'E', EAST, false},
        {'S', SOUTH, false},
        {'W', WEST, false},
        {'\n', WEST, true},
    };
    size_t nb_params = sizeof(params) / sizeof(struct orientation_params);
    return cr_make_param_array(struct orientation_params, params, nb_params);
}

ParameterizedTest(struct orientation_params *param, Parsing, parse_orientation_from)
{
    orientations_t orientation = MAX_ORIENTATION;
    cr_assert_eq(parse_orientation_from(param->o, &orientation), param->error);
    if (param->error)
        return;
    cr_assert_eq(orientation, param->expected, "%d %d", orientation, param->expected);
}

struct map_limit_params
{
    const char *o;
    const map_limit_t expected;
    const bool error;
};

ParameterizedTestParameters(Parsing, parse_map_limit_from)
{
    static struct map_limit_params params[6] = {
        {"5 5", {6, 6}, false},
        {"10 10", {11, 11}, false},
        {"1 2", {2, 3}, false},
        {"2 1", {3, 2}, false},
        {"1", {0, 0}, true},
        {" 1", {0, 0}, true},
    };
    size_t nb_params = sizeof(params) / sizeof(struct map_limit_params);
    return cr_make_param_array(struct map_limit_params, params, nb_params);
}

ParameterizedTest(struct map_limit_params *param, Parsing, parse_map_limit_from)
{
    map_limit_t map = {0};
    cr_assert_eq(parse_map_limit_from(param->o, &map), param->error);
    if (param->error)
        return;
    cr_assert_eq(map.x, param->expected.x, "%d %d", map.x, param->expected.x);
    cr_assert_eq(map.y, param->expected.y, "%d %d", map.y, param->expected.y);
}

struct rover_params
{
    const char *o;
    const rover_t expected;
    const bool error;
};

ParameterizedTestParameters(Parsing, parse_rover_from)
{
    static struct rover_params params[8] = {
        {"5 5 N", {{5, 5}, NORTH}, false},
        {"5 5 E", {{5, 5}, EAST}, false},
        {"5 5 S", {{5, 5}, SOUTH}, false},
        {"5 5 W", {{5, 5}, WEST}, false},
        {"10 10 E", {{10, 10}, EAST}, false},
        {"1 2 S", {{1, 2}, SOUTH}, false},
        {"10 E", {{0, 0}, NORTH}, true},
        {"  10 E", {{0, 0}, NORTH}, true},
    };
    size_t nb_params = sizeof(params) / sizeof(struct rover_params);
    return cr_make_param_array(struct rover_params, params, nb_params);
}

ParameterizedTest(struct rover_params *param, Parsing, parse_rover_from)
{
    rover_t rover = {0};
    cr_assert_eq(parse_rover_from(param->o, &rover), param->error);
    if (param->error)
        return;
    cr_assert_eq(rover.pos.x, param->expected.pos.x, "%d %d", rover.pos.x, param->expected.pos.x);
    cr_assert_eq(rover.pos.y, param->expected.pos.y, "%d %d", rover.pos.y, param->expected.pos.y);
    cr_assert_eq(rover.o, param->expected.o, "%d %d", rover.o, param->expected.o);
}
