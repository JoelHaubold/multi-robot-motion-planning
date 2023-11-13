//
// Created by joelh on 13/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_CHRSOLVER_H
#define MRMP_IMPLEMENTATION_CHRSOLVER_H


class ChrSolver
{
public:
    static void runMRMPProblem(int workspaceComplexity, double maxWorkspaceSize, int numberStartPositions, bool enforceConnectedFreeSpace);
    static void doMRMPRuntimeTest();
};


#endif//MRMP_IMPLEMENTATION_CHRSOLVER_H
