#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include "rover.h"

TestSuite(Rover,
          .init = NULL,
          .fini = NULL,
          .signal = 0,
          .exit_code = 0,
          .disabled = 0,
          .description = "Tests the posible interaction with the rover struct using the dedicated function",
          .timeout = 0);

struct turn_params
{
    rover_t rover;
    orientations_t expected;
};

ParameterizedTestParameters(Rover, turn_left)
{
    static struct turn_params params[MAX_ORIENTATION] = {
        {{{0, 0}, NORTH}, WEST},
        {{{0, 0}, WEST}, SOUTH},
        {{{0, 0}, SOUTH}, EAST},
        {{{0, 0}, EAST}, NORTH},
    };
    size_t nb_params = sizeof(params) / sizeof(struct turn_params);
    return cr_make_param_array(struct turn_params, params, nb_params);
}

ParameterizedTest(struct turn_params *param, Rover, turn_left)
{
    turn_left(&param->rover);
    cr_assert_eq(param->rover.o, param->expected, "%d %d", param->rover.o, param->expected);
}

ParameterizedTestParameters(Rover, turn_right)
{
    static struct turn_params params[MAX_ORIENTATION] = {
        {{{0, 0}, NORTH}, EAST},
        {{{0, 0}, EAST}, SOUTH},
        {{{0, 0}, SOUTH}, WEST},
        {{{0, 0}, WEST}, NORTH},
    };
    size_t nb_params = sizeof(params) / sizeof(struct turn_params);
    return cr_make_param_array(struct turn_params, params, nb_params);
}

ParameterizedTest(struct turn_params *param, Rover, turn_right)
{
    turn_right(&param->rover);
    cr_assert_eq(param->rover.o, param->expected, "%d %d", param->rover.o, param->expected);
}

struct go_forward_param
{
    rover_t rover;
    pos_t expected;
    bool error;
};

ParameterizedTestParameters(Rover, go_forward)
{
    static struct go_forward_param params[MAX_ORIENTATION + 1] = {
        {
            {{1, 1}, NORTH},
            {1, 2},
            false,
        },
        {
            {{1, 1}, WEST},
            {0, 1},
            false,
        },
        {
            {{1, 1}, SOUTH},
            {1, 0},
            false,
        },
        {
            {{1, 1}, EAST},
            {2, 1},
            false,
        },
        {
            {{1, 1}, MAX_ORIENTATION},
            {2, 1},
            true,
        },
    };
    size_t nb_params = sizeof(params) / sizeof(struct go_forward_param);
    return cr_make_param_array(struct go_forward_param, params, nb_params);
}

ParameterizedTest(struct go_forward_param *param, Rover, go_forward)
{
    cr_assert_eq(go_forward(&param->rover, (map_limit_t){5, 5}), param->error);
    if (param->error)
        return;
    cr_assert_eq(param->rover.pos.x, param->expected.x, "{%d, %d} {%d, %d}", param->rover.pos.x, param->rover.pos.y, param->expected.x, param->expected.y);
    cr_assert_eq(param->rover.pos.y, param->expected.y, "{%d, %d} {%d, %d}", param->rover.pos.x, param->rover.pos.y, param->expected.x, param->expected.y);
}

struct take_action_param
{
    rover_t rover;
    rover_t expected;
    const char action;
    bool error;
};

ParameterizedTestParameters(Rover, rover_take_action)
{
    static struct take_action_param params[5] = {
        {
            {{1, 1}, NORTH},
            {{1, 1}, WEST},
            'L',
            false,
        },
        {
            {{1, 1}, NORTH},
            {{1, 1}, NORTH},
            'R',
            false,
        },
        {
            {{1, 1}, SOUTH},
            {{1, 0}, SOUTH},
            'M',
            false,
        },
        {
            {{1, 1}, EAST},
            {{1, 1}, EAST},
            '\n',
            false,
        },
        {
            {{1, 1}, MAX_ORIENTATION},
            {{1, 1}, MAX_ORIENTATION},
            '\0',
            true,
        },
    };
    size_t nb_params = sizeof(params) / sizeof(struct take_action_param);
    return cr_make_param_array(struct take_action_param, params, nb_params);
}

ParameterizedTest(struct take_action_param *param, Rover, rover_take_action)
{
    cr_assert_eq(rover_take_action(&param->rover, param->action, &(const map_limit_t){5, 5}), param->error);
    if (param->error)
        return;
    cr_assert_eq(param->rover.pos.x, param->expected.pos.x, "{%d, %d} {%d, %d}", param->rover.pos.x, param->rover.pos.y, param->expected.pos.x, param->expected.pos.y);
    cr_assert_eq(param->rover.pos.y, param->expected.pos.y, "{%d, %d} {%d, %d}", param->rover.pos.x, param->rover.pos.y, param->expected.pos.x, param->expected.pos.y);
}
