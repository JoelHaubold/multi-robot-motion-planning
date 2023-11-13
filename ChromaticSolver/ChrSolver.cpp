//
// Created by joelh on 13/11/2023.
//

#include "ChrSolver.h"
#include "../chrTypedefs.h"
#include "ChrFreeSpaceGenerator.h"
#include "../MRMPInputGenerator.h"

void ChrSolver::runMRMPProblem(int workspaceComplexity, double maxWorkspaceSize, int numberStartPositions, bool enforceConnectedFreeSpace)
{
    std::cout << "Solving random Chr Problem with params workspaceComplexity: " << workspaceComplexity << "; nmbrStartPositions: " << numberStartPositions << "; maxWorkspaceSize " << maxWorkspaceSize << std::endl;
    Polygon_2 workspacePolygon = MRMPInputGenerator::generateRandomPolygon(workspaceComplexity, maxWorkspaceSize);

    std::vector<FreeSpaceComponent> freeSpaceComponents = ChrFreeSpaceGenerator::getFreeSpaceComponents(workspacePolygon);

    STConfigurations stConfigurations = MRMPInputGenerator::getStartAndTargetConfigurations(freeSpaceComponents, numberStartPositions);

    ChrFreeSpaceGenerator::associateSTConfs(freeSpaceComponents, stConfigurations.startConfigurations, stConfigurations.targetConfigurations);

    std::vector<Tq> tqs = ChrFreeSpaceGenerator::getTq(freeSpaceComponents);

    ChrFreeSpaceSubsets freeSpaceSubsets = ChrFreeSpaceGenerator::getResidualComponents(freeSpaceComponents);




}
void ChrSolver::doMRMPRuntimeTest()
{
}
