//
// Created by joelh on 03/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_MRMPTIMER_H
#define MRMP_IMPLEMENTATION_MRMPTIMER_H


#include "../mytypedefs.h"



class MRMPTimer
{
public:
    static void setStartOfFreeSpaceGen();
    static void setEndOfFreeSpaceGen();
    static void setStartOfAssociatingSTConfs();
    static void setStartOfFStarGen();
    static void setStartOfMGCreation();
    static void setStartOfIntForest();
    static void setStartOfSolvingMG();
    static void setEnd();

    static TimeReport resetAndGetReport();

private:
    static long long getTimeInMicros(std::chrono::high_resolution_clock::time_point tp1, std::chrono::high_resolution_clock::time_point tp2);
};


#endif//MRMP_IMPLEMENTATION_MRMPTIMER_H
