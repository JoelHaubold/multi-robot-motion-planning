//
// Created by joelh on 03/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_TESTPARAMETERS_H
#define MRMP_IMPLEMENTATION_TESTPARAMETERS_H

#include <boost/optional.hpp>
#include <vector>

const boost::optional<unsigned int> PRE_SET_SEED = boost::none;
const double MAX_WORKSPACE_SIZE = 170;
const std::vector<int> WORKSPACE_COMPLEXITY = {40, 80, 120};
const std::vector<int> NMBR_START_POS = {10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 75, 75};
const int REPETITIONS = 5;
const bool USE_WS_SOLVER = true;             //Use Well Seprated or Chromatic Solver
const bool LIMIT_TO_ONE_FS_COMPONENT = false;// Enforce the free space to consist of one connected component

#endif//MRMP_IMPLEMENTATION_TESTPARAMETERS_H
