/**
 * @file KalmanFilter.h
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Median Filter Definitions.
 * @version 0.1
 * @date 2021-11-03
 * @copyright Copyright (c) 2021
 * @note Source: https://www.kalmanfilter.net/kalman1d.html
 */
#pragma once

/** Device Specific imports. */
#include <raslib/Filter/Filter.h>


typedef struct KalmanFilter {
    Filter_t filter;

    float userSettings[4];

    float mEstimate;

    float mEu;

    float mMu;

    float mQ;
} KalmanFilter_t;

KalmanFilter_t KalmanFilterInit(const FilterConfig_t config, const float initialEstimate, const float estimateUncertainty, const float measurementUncertainty, const float processNoiseVariance);

void KalmanFilterAddSample(KalmanFilter_t * filter, const float sample);

float KalmanFilterGetSample(KalmanFilter_t * filter);

void KalmanFilterClear(KalmanFilter_t * filter);