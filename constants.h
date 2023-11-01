//
// Created by joelh on 19/10/2023.
//

#ifndef MRMP_IMPLEMENTATION_CONSTANTS_H
#define MRMP_IMPLEMENTATION_CONSTANTS_H


//TODO: Split into constant file and parameter file
const double MAX_WORKSPACE_SIZE = 150;
const int WORKSPACE_COMPLEXITY = 100;

const int OBSTACLE_OFFSET = 5;
const int ROBOT_SIZE = 2;

const int NMBR_START_POS = 10;

const int NMBR_POINT_TRIES = 10000;
const int NMBR_PAIR_POINT_TRIES = 100;

const bool USE_WS = true;
//TODO: Set automically based on bool
const double MONOCHROMATIC_SEPARATION = 4.0;
const double BICHROMATIC_SEPARATION = 4.0;

const std::string FREE_SPACE_PREFIX = "F";
const std::string F_STAR_PREFIX = "F*";
const std::string START_CONF_PREFIX = "S";
const std::string TARGET_CONF_PREFIX = "T";
const std::string ROBOT_PREFIX = "R";




#endif//MRMP_IMPLEMENTATION_CONSTANTS_H
