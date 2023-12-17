//
// Created by joelh on 03/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_TESTPARAMETERS_H
#define MRMP_IMPLEMENTATION_TESTPARAMETERS_H

#include <boost/optional.hpp>
#include <vector>

//TODO: Define used mono and bi sep here to compare runtime of MRMP [At beginning check if permissible]
//TODO: Disallow parallel edges in motion graph
const boost::optional<unsigned int> PRE_SET_SEED = 3562520494;//boost::none;
const double MAX_WORKSPACE_SIZE = 100;
const std::vector<int> WORKSPACE_COMPLEXITY = {100};
const std::vector<int> NMBR_START_POS = {20};
//const std::vector<int> NMBR_START_POS = {10, 15, 75};

const int REPETITIONS = 5;
const bool USE_WS_SOLVER = true;             //Use Well Seprated or Chromatic Solver
const bool LIMIT_TO_ONE_FS_COMPONENT = false;// Enforce the free space to consist of one connected component

#endif//MRMP_IMPLEMENTATION_TESTPARAMETERS_H
