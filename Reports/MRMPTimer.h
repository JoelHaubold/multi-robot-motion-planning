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
    static void setStartOfGraphWork();
    static void setEndOfGraphWork();

    static TimeReport resetAndGetReport();
};


#endif//MRMP_IMPLEMENTATION_MRMPTIMER_H
