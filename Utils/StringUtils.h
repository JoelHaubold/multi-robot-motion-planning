//
// Created by joelh on 01/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_STRINGUTILS_H
#define MRMP_IMPLEMENTATION_STRINGUTILS_H

#include "../mytypedefs.h"

class StringUtils
{
public:
    //static std::string toMotionScheduleString(Segment_2 pathSegment);
    static std::string toCoordinateString(Point_2 pathSegment);
    static void printMotionSteps(MotionSchedule ms);
};


#endif//MRMP_IMPLEMENTATION_STRINGUTILS_H
