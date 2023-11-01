//
// Created by joelh on 01/11/2023.
//

#include "StringUtils.h"
std::string StringUtils::toCoordinateString(Point_2 point)
{
    return "("+std::to_string(CGAL::to_double(point.x())) + ","+ std::to_string(CGAL::to_double(point.y()))+ ")";
}

void StringUtils::printMotionSteps(MotionSchedule ms)
{
    for(const auto& pair : ms.robots2VerticesTraveled) {
        std::cout << pair.first + ": " + pair.second << std::endl;
    }
}