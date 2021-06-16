#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <criterion/parameterized.h>
#include "TheMarsRover.h"

TestSuite(Flow,
          .init = cr_redirect_stdin,
          .fini = NULL,
          .signal = 0,
          .exit_code = 0,
          .disabled = 0,
          .description = "Tests the flow of the program",
          .timeout = 0);

Test(Flow, get_and_parse_first_line)
{
    char str[] = "5 5\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str), file_stdin), -1);

    map_limit_t map = {0};
    cr_assert_eq(get_and_parse_first_line(&map), false);
    cr_assert_eq(map.x, 6, "x: %d e: %d", map.x, 6);
    cr_assert_eq(map.y, 6, "y: %d e: %d", map.y, 6);
}

Test(Flow, get_and_parse_first_line_fail_parsing)
{
    char str[] = "\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str), file_stdin), -1);

    map_limit_t map = {0};
    cr_assert_eq(get_and_parse_first_line(&map), true);
}

Test(Flow, get_and_parse_first_line_fail_getting)
{
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fclose(file_stdin), -1);

    map_limit_t map = {0};
    cr_assert_eq(get_and_parse_first_line(&map), true);
}

struct actions_apply_params
{
    const char *actions;
    const map_limit_t map;
    const rover_t expected;
    const bool error;
};

ParameterizedTestParameters(Flow, apply_actions_to_rover)
{
    static struct actions_apply_params params[5] = {
        {"LLRRMM", {10, 10}, {{0, 2}, NORTH}, false},
        {"LLMM", {10, 10}, {{0, 8}, SOUTH}, false},
        {"", {10, 10}, {{0, 0}, NORTH}, false},
        {"MMMMMMMMMM", {10, 10}, {{0, 0}, NORTH}, false},
        {"B", {10, 10}, {{0, 0}, NORTH}, true},
    };
    size_t nb_params = sizeof(params) / sizeof(struct actions_apply_params);
    return cr_make_param_array(struct actions_apply_params, params, nb_params);
}

ParameterizedTest(struct actions_apply_params *param, Flow, apply_actions_to_rover)
{
    rover_t rover = {0};
    cr_assert_eq(apply_actions_to_rover(&rover, param->actions, &param->map), param->error);
    if (param->error)
        return;
    cr_assert_eq(rover.pos.x, param->expected.pos.x, "x: %d e: %d", rover.pos.x, param->expected.pos.x);
    cr_assert_eq(rover.pos.y, param->expected.pos.y, "y: %d e: %d", rover.pos.y, param->expected.pos.y);
    cr_assert_eq(rover.o, param->expected.o, "o: %d e: %d", rover.o, param->expected.o);
}

struct process_a_rover_params
{
    const char *actions;
    const char *position;
    const map_limit_t map;
    const char *expected;
    const bool error;
};

ParameterizedTestParameters(Flow, process_a_rover)
{
    static struct process_a_rover_params params[5] = {
        {"LLRRMM", "1 1 N\n", {10, 10}, "1 3 N\n", false},
        {"LLMM", "1 1 N\n", {10, 10}, "1 9 S\n", false},
        {"MMMMMMMMMM", "1 1 N\n", {10, 10}, "1 1 N\n", false},
        {"EXIT", "1 1 N\n", {10, 10}, NULL, true},
        {"LLMM", "1 N\n", {10, 10}, NULL, true},
    };
    size_t nb_params = sizeof(params) / sizeof(struct process_a_rover_params);
    return cr_make_param_array(struct process_a_rover_params, params, nb_params);
}

ParameterizedTest(struct process_a_rover_params *param, Flow, process_a_rover, .init = cr_redirect_stdout)
{
    cr_assert_eq(process_a_rover(&param->map, param->position, param->actions), param->error);
    if (param->error)
        return;
    cr_assert_stdout_eq_str(param->expected);
}

void initialisation()
{
    cr_redirect_stdin();
    cr_redirect_stdout();
}

Test(Flow, the_mars_rover, .init = initialisation)
{
    char str[] = "5 5\n1 2 N\nLMLMLMLMM\n3 3 E\nMMRMMRMRRM\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str) - 1, file_stdin), -1);
    cr_expect_neq(fclose(file_stdin), -1);

    cr_assert_eq(the_mars_rover(), 0);
    cr_assert_stdout_eq_str("1 3 N\n5 1 E\n");
}

Test(Flow, the_mars_rover_one_and_a_half_rover, .init = cr_redirect_stdin)
{
    char str[] = "5 5\n1 2 N\nLMLMLMLMM\n3 3 E\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str) - 1, file_stdin), -1);
    cr_expect_neq(fclose(file_stdin), -1);

    cr_assert_eq(the_mars_rover(), 1);
}

Test(Flow, the_mars_rover_half_rover, .init = cr_redirect_stdin)
{
    char str[] = "5 5\n1 2 N\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str) - 1, file_stdin), -1);
    cr_expect_neq(fclose(file_stdin), -1);

    cr_assert_eq(the_mars_rover(), 1);
}

Test(Flow, the_mars_rover_no_rover, .init = cr_redirect_stdin)
{
    char str[] = "5 5\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str) - 1, file_stdin), -1);
    cr_expect_neq(fclose(file_stdin), -1);

    cr_assert_eq(the_mars_rover(), 1);
}

Test(Flow, the_mars_rover_no_other_rover, .init = initialisation)
{
    char str[] = "5 5\n1 2 N\nLMLMLMLMM\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str) - 1, file_stdin), -1);
    cr_expect_neq(fclose(file_stdin), -1);

    cr_assert_eq(the_mars_rover(), 0);
    cr_assert_stdout_eq_str("1 3 N\n");
}

Test(Flow, the_mars_rover_error_config_rover, .init = cr_redirect_stdin)
{
    char str[] = "5 \n1 2 N\nLMLMLMLMM\n";
    FILE *file_stdin = cr_get_redirected_stdin();

    cr_expect_neq(fwrite(str, 1, sizeof(str) - 1, file_stdin), -1);
    cr_expect_neq(fclose(file_stdin), -1);

    cr_assert_eq(the_mars_rover(), 1);
}