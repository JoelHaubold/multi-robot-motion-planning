//
// Created by joelh on 03/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_WSSOLVER_H
#define MRMP_IMPLEMENTATION_WSSOLVER_H


class WSSolver
{
public:
    static void runMRMPProblem(int workspaceComplexity, double maxWorkspaceSize, int numberStartPositions);
    static void doMRMPRuntimeTest();
};


#endif//MRMP_IMPLEMENTATION_WSSOLVER_H
