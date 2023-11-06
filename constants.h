//
// Created by joelh on 19/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_CONSTANTS_H
#define MRMP_IMPLEMENTATION_CONSTANTS_H


#include "testParameter.h"

const int OBSTACLE_SPACE_OFFSET = 5;
const int ROBOT_SIZE = 2;

const int NMBR_POINT_TRIES = 20000;
const int NMBR_PAIR_POINT_TRIES = 1000;

const bool GENERATE_DEBUG_DRAWINGS = true;

const double MONOCHROMATIC_SEPARATION = 4.0;
const double BICHROMATIC_SEPARATION = USE_WS_SOLVER ? 4.0 : 2.0;

const std::string FREE_SPACE_PREFIX = "F";
const std::string F_STAR_PREFIX = "F*";
const std::string START_CONF_PREFIX = "S";
const std::string TARGET_CONF_PREFIX = "T";
const std::string ROBOT_PREFIX = "R";




#endif//MRMP_IMPLEMENTATION_CONSTANTS_H
