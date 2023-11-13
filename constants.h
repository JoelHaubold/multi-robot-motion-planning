//
// Created by joelh on 19/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_CONSTANTS_H
#define MRMP_IMPLEMENTATION_CONSTANTS_H

const int OBSTACLE_SPACE_OFFSET = 5;
const int ROBOT_SIZE = 2;

const int NMBR_POINT_TRIES = 20000;
const int NMBR_PAIR_POINT_TRIES = 1000;

const bool GENERATE_DEBUG_DRAWINGS = false;

const double MONOCHROMATIC_SEPARATION = ROBOT_SIZE*2;
const double BICHROMATIC_SEPARATION = ROBOT_SIZE;
const double WS_SEPERATION = ROBOT_SIZE*2;

const std::string FREE_SPACE_PREFIX = "F";
const std::string F_STAR_PREFIX = "F*";
const std::string START_CONF_PREFIX = "S";
const std::string TARGET_CONF_PREFIX = "T";
const std::string ROBOT_PREFIX = "R";

const std::string RESIDUAL_PREFIX = "F-";
const std::string BLOCKER_PREFIX = "B";




#endif//MRMP_IMPLEMENTATION_CONSTANTS_H
