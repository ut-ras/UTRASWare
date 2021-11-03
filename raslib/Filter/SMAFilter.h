/**
 * @file SMAFilter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Simple Moving Average filter.
 * @version 0.1
 * @date 2021-11-03
 * @copyright Copyright (c) 2021
 */
#pragma once

/** Device Specific imports. */
#include <raslib/Filter/Filter.h>


typedef struct SMAFilter {
    Filter_t filter;

    float sum;

    uint16_t curIdx;

    uint16_t numSamples;
} SMAFilter_t;

SMAFilter_t SMAFilterInit(const FilterConfig_t config);

void SMAFilterAddSample(SMAFilter_t * filter, const float sample);

float SMAFilterGetSample(const SMAFilter_t * filter);

void SMAFilterClear(SMAFilter_t * filter);