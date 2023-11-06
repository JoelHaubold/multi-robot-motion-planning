    //
    // Created by joelh on 03/11/2023.
    //

    #ifndef MRMP_IMPLEMENTATION_TESTPARAMETERS_H
#define MRMP_IMPLEMENTATION_TESTPARAMETERS_H

#include <boost/optional.hpp>
    #include <vector>
    //Todo: optional struct with three test params to run single
    const boost::optional<unsigned int> PRE_SET_SEED = boost::none;
    const double MAX_WORKSPACE_SIZE = 170;
    const std::vector<int> WORKSPACE_COMPLEXITY = {50,55,60};
    const std::vector<int> NMBR_START_POS = {10, 15, 20, 25, 30, 35, 40};
    const int REPETITIONS = 2;
    const bool USE_WS_SOLVER = true; //Use Well Seprated or Chromatic Solver
    const bool LIMIT_TO_ONE_FS_COMPONENT = false; // Enforce the free space consisting of one connected component

    #endif//MRMP_IMPLEMENTATION_TESTPARAMETERS_H