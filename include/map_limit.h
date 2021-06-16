#pragma once

#include "position.h"

typedef pos_t map_limit_t;

extern bool parse_map_limit_from(const char *, map_limit_t *)
    __attribute__((__warn_unused_result__, nonnull(1, 2), pure));
