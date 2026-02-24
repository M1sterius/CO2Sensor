#pragma once

// TODO: Add normal compile definition
#define _DEBUG

namespace CO2
{
    #ifdef _DEBUG
        #define DEBUG_LOG(str) Serial.println(str)
    #else
        #define DEBUG_LOG(str)
    #endif
}