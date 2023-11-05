//
// Created by joelh on 03/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_RUNTIMETESTVALUES_H
#define MRMP_IMPLEMENTATION_RUNTIMETESTVALUES_H

#include <boost/optional.hpp>
#include <vector>

const boost::optional<unsigned int> PRE_SET_SEED = boost::none;
const double MAX_WORKSPACE_SIZE = 140;
const std::vector<int> WORKSPACE_COMPLEXITY = {10};
const std::vector<int> NMBR_START_POS = {5,10,15,20,25};
const int REPETITIONS = 2;
const bool USE_WS_SOLVER = true; //Use Well Seprated or Chromatic Solver
const bool LIMIT_TO_ONE_FS_COMPONENT = false; // Enforce the free space consisting of one connected component

#endif//MRMP_IMPLEMENTATION_RUNTIMETESTVALUES_H
