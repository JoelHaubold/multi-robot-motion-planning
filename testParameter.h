//
// Created by joelh on 03/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_TESTPARAMETER_H
#define MRMP_IMPLEMENTATION_TESTPARAMETER_H

#include <boost/optional.hpp>
#include <vector>
//Todo: optional struct with three test params to run single
const boost::optional<unsigned int> PRE_SET_SEED = 258948431;
const double MAX_WORKSPACE_SIZE = 200;
const std::vector<int> WORKSPACE_COMPLEXITY = {65};
const std::vector<int> NMBR_START_POS = {20};
const int REPETITIONS = 2;
const bool USE_WS_SOLVER = true; //Use Well Seprated or Chromatic Solver
const bool LIMIT_TO_ONE_FS_COMPONENT = false; // Enforce the free space consisting of one connected component

#endif//MRMP_IMPLEMENTATION_TESTPARAMETER_H
