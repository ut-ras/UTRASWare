/**
 * @file MedianFilter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Median Filter Definitions.
 * @version 0.1
 * @date 2021-11-03
 * @copyright Copyright (c) 2021
 */
#pragma once

/** Device Specific imports. */
#include <raslib/Filter/Filter.h>


typedef struct MedianFilter {
    Filter_t filter;

    uint16_t curIdx;

    uint16_t maxSampleWindow;

    float median;
} MedianFilter_t;

MedianFilter_t MedianFilterInit(const FilterConfig_t config);

void MedianFilterAddSample(MedianFilter_t * filter, const float sample);

float MedianFilterGetSample(MedianFilter_t * filter);

void MedianFilterClear(MedianFilter_t * filter);
