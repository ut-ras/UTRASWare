/**
 * @file FilterExample.c
 * @author Matthew Yu (matthewjkyu@gmail.com)
 * @brief Showing off the capability of various filters.
 * @version 0.1
 * @date 2021-11-03
 * @copyright Copyright (c) 2021
 * Modify the value of "__MAIN__" on line 14 to choose which program to run:
 *
 * __MAIN__ = 0: demonstrates initialization and execution of a SMA Filter.
 * __MAIN__ = 1: demonstrates initialization and execution of a Median Filter.
 * __MAIN__ = 2: demonstrates initialization and execution of a Kalman Filter.
 */
#define __MAIN__ 0

/** General imports. */
#include <stdlib.h>
#include <assert.h>

/** Device specific imports. */
#include <lib/PLL/PLL.h>
#include <lib/GPIO/GPIO.h>
#include <raslib/Filter/Filter.h>
#include <raslib/Filter/SMAFilter.h>
#include <raslib/Filter/MedianFilter.h>
#include <raslib/Filter/KalmanFilter.h>


#if __MAIN__ == 0
static float buffer[5] = { 0.0 };
static SMAFilter_t smaFilter;

void checkMacro(SMAFilter_t * filter, float expectedVal) {
    float result = SMAFilterGetSample(filter);
    assert((int) result*100 == (int)expectedVal*100);
}

int main(void) {
    /**
     * @brief This program tests the execution of a simple moving average filter.
     */
    PLLInit(BUS_80_MHZ);

    /* Initialize PF1 as a GPIO output. This is associated with the RED led on
       the TM4C. */
    GPIOConfig_t PF1Config = {
        .pin=PIN_F1,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };

    /* Initialize PF3 as a GPIO output. This is associated with the GREEN led on
       the TM4C. */
    GPIOConfig_t PF3Config = {
        .pin=PIN_F3,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };

    GPIOInit(PF1Config);
    GPIOInit(PF3Config);

    while (1) {
        FilterConfig_t filterConfig = {
            .type=FILTER_SMA,
            .buffer=buffer,
            .bufferSize=5
        };
        smaFilter = SMAFilterInit(filterConfig);

        /* Test 1. Read while empty. */
        checkMacro(&smaFilter, 0.0);

        /* Test 2. Read after write. */
        SMAFilterAddSample(&smaFilter, 10.0);
        checkMacro(&smaFilter, 10.0);

        /* Test 3. Clear and read. */
        SMAFilterClear(&smaFilter);
        checkMacro(&smaFilter, 0.0);

        /* Test 4. Read after many writes. */
        float expectedRes[20] = {
            100,
            55,
            43.33,
            40,
            40,
            40,
            50,
            60,
            70,
            80,
            80,
            90,
            100,
            110,
            120,
            120,
            130,
            140,
            150,
            160
        };
        /* Add 20 samples, increasing linearly by 10, and then some noisy 100s
           every 5 cycles. */
        for (int i = 0; i < 20; ++i) {
            if (i%5 == 0)
                SMAFilterAddSample(&smaFilter, 100.0);
            else
                SMAFilterAddSample(&smaFilter, i * 10.0);

            /* Assert the expected filter output at every point */
            checkMacro(&smaFilter, expectedRes[i]);
        }

        GPIOSetBit(PIN_F3, 1);
        while (1) {}
    }
}
#elif __MAIN__ == 1
static float buffer[5] = { 0.0 };
static MedianFilter_t medianFilter;

void checkMacro(MedianFilter_t * filter, float expectedVal) {
    float result = MedianFilterGetSample(filter);
    assert((int) result*100 == (int)expectedVal*100);
}

int main(void) {
    /**
     * @brief This program tests the execution of a simple moving average filter.
     */
    PLLInit(BUS_80_MHZ);

    /* Initialize PF1 as a GPIO output. This is associated with the RED led on
       the TM4C. */
    GPIOConfig_t PF1Config = {
        .pin=PIN_F1,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };

    /* Initialize PF3 as a GPIO output. This is associated with the GREEN led on
       the TM4C. */
    GPIOConfig_t PF3Config = {
        .pin=PIN_F3,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };

    GPIOInit(PF1Config);
    GPIOInit(PF3Config);

    while (1) {
        FilterConfig_t filterConfig = {
            .type=FILTER_MEDIAN,
            .buffer=buffer,
            .bufferSize=5
        };
        medianFilter = MedianFilterInit(filterConfig);

        /* Test 1. Read while empty. */
        checkMacro(&medianFilter, 0.0);

        /* Test 2. Read after write. */
        MedianFilterAddSample(&medianFilter, 10.0);
        checkMacro(&medianFilter, 0.0);

        /* Test 3. Clear and read. */
        MedianFilterClear(&medianFilter);
        checkMacro(&medianFilter, 0.0);

        /* Test 4. Read after many writes. */
        float testcase[20][2] = {
            {10.0, 0}, // Input, expected median output
            {10.0, 0}, // Fixed input.
            {10.0, 10},
            {10.0, 10},
            {10.0, 10},
            {11.0, 10},// Incrementing series.
            {12.0, 10},
            {13.0, 11},
            {13.0, 12},
            {13.0, 13},
            {0.00, 13}, // Reset.
            {0.00, 13},
            {0.00, 0},
            {0.00, 0},
            {0.00, 0},
            {5.45, 0},  // Random series.
            {10.2, 0},
            {7.00, 5.45},
            {1.50, 5.45},
            {15.0, 7.00}
        };
        /* Add 20 samples and check the expected median. */
        for (int i = 0; i < 20; ++i) {
            MedianFilterAddSample(&medianFilter, testcase[i][0]);

            /* Assert the expected filter output at every point */
            checkMacro(&medianFilter, testcase[i][1]);
        }

        GPIOSetBit(PIN_F3, 1);
        while (1) {}
    }
}
#elif __MAIN__ == 2
static float buffer[5] = { 0.0 };
static KalmanFilter_t kalmanFilter;
float history[20][2] = { 0.0 };

int main(void) {
    /**
     * @brief This program tests the execution of a Kalman filter.
     */
    PLLInit(BUS_80_MHZ);

    /* Initialize PF1 as a GPIO output. This is associated with the RED led on
       the TM4C. */
    GPIOConfig_t PF1Config = {
        .pin=PIN_F1,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };

    /* Initialize PF3 as a GPIO output. This is associated with the GREEN led on
       the TM4C. */
    GPIOConfig_t PF3Config = {
        .pin=PIN_F3,
        .pull=GPIO_PULL_DOWN,
        .isOutput=true,
        .alternateFunction=0,
        .isAnalog=false,
        .drive=GPIO_DRIVE_2MA,
        .enableSlew=false
    };

    GPIOInit(PF1Config);
    GPIOInit(PF3Config);

    while (1) {
        FilterConfig_t filterConfig = {
            .type=FILTER_KALMAN,
            .buffer=buffer,
            .bufferSize=5
        };
        kalmanFilter = KalmanFilterInit(filterConfig, 25.0, 25, 10, 0.15);

        /* Add 20 samples with a true value around 20.0, every third sample has
           an uncertainty of 10%. */ 
        uint16_t i;
        for (i = 0; i < 20; i++) {
            if (i%3 == 0) {
                history[i][0] = 20.0 + (rand()%101)/100.0*4.0-2.0;
                KalmanFilterAddSample(&kalmanFilter, history[i][0]);
                
            } else {
                history[i][0] = 20.0;
                KalmanFilterAddSample(&kalmanFilter, history[i][0]);
            }
        
            /* Read the filter output at every point. */
            history[i][1] = KalmanFilterGetSample(&kalmanFilter);
        }
        
        GPIOSetBit(PIN_F3, 1);
        while (1) {
            uint8_t i = 0;
            /* Put a breakpoint here and check the history array. An example
               output is provided in the folder. */ 
        }
    }
}
#endif
