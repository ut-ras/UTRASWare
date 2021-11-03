/**
 * @file Filter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Generic filter driver data structures.
 * @version 0.1
 * @date 2021-10-28
 * @copyright Copyright (c) 2021
 */
#pragma once

/** General imports. */
#include <stdbool.h>
#include <stdint.h>

enum FilterType {
    FILTER_MEDIAN,
    FILTER_EMA,
    FILTER_SMA,
    FILTER_KALMAN,
    NUM_FILTERS
};

typedef struct FilterConfig {
    enum FilterType type;

    float * buffer;

    uint16_t bufferSize;
} FilterConfig_t;

typedef struct Filter {
    enum FilterType type;

    float * buffer;

    uint16_t bufferSize;
} Filter_t;
