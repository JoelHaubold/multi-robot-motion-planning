//
// Created by joelh on 03/11/2023.
//

#include "MRMPTimer.h"
#include "mytypedefs.h"

static std::chrono::high_resolution_clock::time_point fsStartTime;
static std::chrono::high_resolution_clock::time_point fsEndTime;
static std::chrono::high_resolution_clock::time_point gwStartTime;
static std::chrono::high_resolution_clock::time_point gwEndTime;

TimeReport MRMPTimer::resetAndGetReport()
{
    long long fsDuration = std::chrono::duration_cast<std::chrono::milliseconds >(fsEndTime - fsStartTime).count();
    long long gwDuration = std::chrono::duration_cast<std::chrono::milliseconds >(gwEndTime - gwStartTime).count();
    return {fsDuration, gwDuration};
}
void MRMPTimer::setStartOfFreeSpaceGen()
{
    fsStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setEndOfFreeSpaceGen()
{
    fsEndTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setStartOfGraphWork()
{
    gwStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setEndOfGraphWork()
{
    gwEndTime = std::chrono::high_resolution_clock::now();
}

