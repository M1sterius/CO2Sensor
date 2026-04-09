#pragma once

// TODO: Add normal compile definition
#define _DEBUG

#include <Arduino.h>

namespace CO2
{
    // TODO: Those macros aren't the best practice. Implement better error handling
    #ifdef _DEBUG
        #define DEBUG_LOG(fmt, ...) Serial.printf(fmt "\n", ##__VA_ARGS__)

        #define CATASTROPHIC_ERROR(text) \
            while (true) \
            { \
                DEBUG_LOG(text); \
                vTaskDelay(pdMS_TO_TICKS(1000)); \
            }
    #else
        #define DEBUG_LOG(str)
    #endif
}
