//
// Created by joelh on 03/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_RUNTIMETESTVALUES_H
#define MRMP_IMPLEMENTATION_RUNTIMETESTVALUES_H


const std::vector<double> MAX_WORKSPACE_SIZE = {140};
const std::vector<int> WORKSPACE_COMPLEXITY = {40};//{10,20,30,40,50,60,70,80,90,100};
const std::vector<int> NMBR_START_POS = {5,15,25,35,45,55,65,75,85};
const int REPETITIONS = 1;
const bool USE_WS_SOLVER = true; //Use Well Seprated or Chromatic Solver
const bool LIMIT_TO_ONE_FS_COMPONENT = false; // Enforce the free space consisting of one connected component

#endif//MRMP_IMPLEMENTATION_RUNTIMETESTVALUES_H
