//
// Created by joelh on 03/11/2023.
//

#include "MRMPTimer.h"
#include "../mytypedefs.h"

static std::chrono::high_resolution_clock::time_point fsStartTime;
static std::chrono::high_resolution_clock::time_point fsEndTime;
static std::chrono::high_resolution_clock::time_point stAssociateStartTime;
static std::chrono::high_resolution_clock::time_point fStarStartTime;
static std::chrono::high_resolution_clock::time_point mgCreationStartTime;
static std::chrono::high_resolution_clock::time_point intForestStartTime;
static std::chrono::high_resolution_clock::time_point solvingMGStartTime;
static std::chrono::high_resolution_clock::time_point calculationEndTime;


TimeReport MRMPTimer::resetAndGetReport()
{
    long long fsDuration = getTimeInMicros(fsEndTime, fsStartTime);
    long long associateDuration = getTimeInMicros(fStarStartTime, stAssociateStartTime);
    long long fStarDuration = getTimeInMicros(mgCreationStartTime, fStarStartTime);
    long long mgCreationDuration = getTimeInMicros(intForestStartTime, mgCreationStartTime);
    long long intForestDuration = getTimeInMicros(solvingMGStartTime, intForestStartTime);
    long long mgSolvingDuration = getTimeInMicros(calculationEndTime, solvingMGStartTime);
    return {fsDuration, associateDuration, fStarDuration, mgCreationDuration, intForestDuration, mgSolvingDuration};
}
void MRMPTimer::setStartOfFreeSpaceGen()
{
    fsStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setEndOfFreeSpaceGen()
{
    fsEndTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setStartOfAssociatingSTConfs() {
    stAssociateStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setStartOfFStarGen(){
    fStarStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setStartOfMGCreation(){
    mgCreationStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setStartOfIntForest(){
    intForestStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setStartOfSolvingMG(){
    solvingMGStartTime = std::chrono::high_resolution_clock::now();
}
void MRMPTimer::setEnd(){
    calculationEndTime = std::chrono::high_resolution_clock::now();
}

long long MRMPTimer::getTimeInMicros(std::chrono::high_resolution_clock::time_point tp1, std::chrono::high_resolution_clock::time_point tp2)
{
    return std::chrono::duration_cast<std::chrono::microseconds >(tp1 - tp2).count();
}
