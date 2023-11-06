//
// Created by joelh on 03/11/2023.
//


#include "WSSolver.h"
#include "../MRMPInputGenerator.h"
#include "../RandomGenerator.h"
#include "../Reports/MRMPTimer.h"
#include "../Reports/ReportOutput.h"
#include "../SharedSolverComponents/InterferenceForestGenerator.h"
#include "../Utils/GraphvizDrawUtils.h"
#include "../Utils/SFMLDrawUtils.h"
#include "../Utils/StringUtils.h"
#include "../Utils/Utils.h"
#include "../mytypedefs.h"
#include "../testParameters.h"
#include "WSFreeSpaceGenerator.h"
#include "WSMotionGraphGenerator.h"
#include "WSMotionGraphSolver.h"

void WSSolver::runMRMPProblem(int workspaceComplexity, double maxWorkspaceSize, int numberStartPositions)
{
    std::cout << "Solving random WS Problem with params workspaceComplexity: " << workspaceComplexity << "; nmbrStartPositions: " << numberStartPositions << "; maxWorkspaceSize " << maxWorkspaceSize << std::endl;
    Polygon_2 workspacePolygon = MRMPInputGenerator::generateRandomPolygon(workspaceComplexity, maxWorkspaceSize);
    //    CGAL::draw(workspacePolygon);
    //    SFMLDrawUtils::drawPolygon_2(workspacePolygon, "workspace");

    //DrawUtils::drawPolygon(workspacePolygon);
    //    std::cout << "FC:" << std::endl;
    MRMPTimer::setStartOfFreeSpaceGen();
    std::vector<FreeSpaceComponent> freeSpaceComponents = WSFreeSpaceGenerator::getFreeSpaceComponents(workspacePolygon);
    MRMPTimer::setEndOfFreeSpaceGen();
    //    SFMLDrawUtils::drawFreeSpace(freeSpaceComponents, "freeSpace");


    STConfigurations stConfigurations = MRMPInputGenerator::getStartAndTargetConfigurations(freeSpaceComponents, numberStartPositions);
    MRMPTimer::setStartOfGraphWork();
    WSFreeSpaceGenerator::associateSTConfs(freeSpaceComponents, stConfigurations.startConfigurations, stConfigurations.targetConfigurations);
    std::vector<FStarComponent> fStarSet = WSFreeSpaceGenerator::getFStar2(freeSpaceComponents);
    //    Polygon_set_2 set;
    //    for(const auto& fs : fStarSet) {
    //        set.insert(fs.fStarPolygon);
    //    }
    //
    //    CGAL::draw(set);
    std::cout << "fSpace comps: " << freeSpaceComponents.size() << "; fStar comps: " << fStarSet.size() << std::endl;
    if (GENERATE_DEBUG_DRAWINGS)
    {
        SFMLDrawUtils::drawFStar(fStarSet, "fStar");
    }


    //WSFreeSpaceGenerator::associateSTConfs(freeSpaceComponents, fStarSet, stConfigurations.startConfigurations, stConfigurations.targetConfigurations);
    //WSFreeSpaceGenerator::getMotionGraph(fStarSet, startConfs, targetConfs);

    //    Motion_Graph motionGraph;
    //
    //    const STConfId2MGVertex id2Vertex = WSMotionGraphGenerator::insertVertices(motionGraph, stConfigurations);
    //    WSMotionGraphGenerator::insertEdges(motionGraph, fStarSet, id2Vertex);

    std::unordered_map<std::string, Motion_Graph> mgs = WSMotionGraphGenerator::getMotionGraphs(freeSpaceComponents, fStarSet);


    if (GENERATE_DEBUG_DRAWINGS)
    {
        GraphvizDrawUtils::drawMotionGraphs(mgs);
        std::vector<MGEdgeProperty> edges;

        for (const auto &fsMg: mgs)
        {
            const auto &motionGraph = fsMg.second;
            auto edgePair = boost::edges(motionGraph);
            auto edgeIt = edgePair.first;
            auto edgeEnd = edgePair.second;
            auto edgePropertyMap = boost::get(boost::edge_bundle, motionGraph);
            for (; edgeIt != edgeEnd; ++edgeIt)
            {
                edges.push_back(motionGraph[*edgeIt]);
                //        MGEdgeProperty edgeProperty = edgePropertyMap[edgePair.first];
                //        edges.push_back(motionGraph[edge_descriptor]);
            }
        }

        SFMLDrawUtils::drawPaths(fStarSet, edges, "Paths");
    }
    DirectedInterferenceForest diForest = InterferenceForestGenerator::getInterferenceForest(freeSpaceComponents);
    if(GENERATE_DEBUG_DRAWINGS) {
        GraphvizDrawUtils::drawDIForest(diForest, "DIForest.dot");
    }
    MotionSchedule ms = WSMotionGraphSolver::solveMotionGraphs(mgs, diForest);
    MRMPTimer::setEndOfGraphWork();
    if (GENERATE_DEBUG_DRAWINGS)
    {
        StringUtils::printMotionSteps(ms);
    }
}

void WSSolver::doMRMPRuntimeTest()
{
    std::cerr << "Starting Tests using seed: " << RandomGenerator::getSeed() << std::endl;
    std::vector<TestRunReport> reports;
    std::vector<unsigned int> seeds;
    seeds.reserve(REPETITIONS);
    seeds.push_back(RandomGenerator::getSeed());
    for (int i = 1; i < REPETITIONS; i++)
    {
        seeds.push_back(RandomGenerator::getRandomInt(0, std::numeric_limits<int>::max()));
    }

    for (const auto &workspaceComplexity: WORKSPACE_COMPLEXITY)
    {
        for (const auto &nmbrStartConfs: NMBR_START_POS)
        {
            long long sumFSTime = 0;
            long long sumGWTime = 0;
            const double runtimeBound = Utils::getRuntimeBound(workspaceComplexity, nmbrStartConfs);
            for (int i = 0; i < REPETITIONS; i++)
            {
                RandomGenerator::setSeed(seeds[i]);
                runMRMPProblem(workspaceComplexity, MAX_WORKSPACE_SIZE, nmbrStartConfs);
                TimeReport tr = MRMPTimer::resetAndGetReport();
                sumFSTime += tr.freeSpaceTime;
                sumGWTime += tr.graphWorkTime;
            }
            long long avgFSTime = sumFSTime / REPETITIONS;
            long long avgGWTime = sumGWTime / REPETITIONS;
            reports.emplace_back(TimeReport{avgFSTime, avgGWTime}, runtimeBound);
            std::cout << "WC: " << workspaceComplexity << "; NSP: " << nmbrStartConfs << "; Time in microseconds: " << reports[reports.size() - 1].timeReport.totalTime << std::endl;
        }
    }
    ReportOutput::printAndSaveReports(reports);
    //    for(const auto& report: reports) {
    //        std::cout << "Time taken by function: " << report.timeReport.freeSpaceTime << "; " << report.timeReport.graphWorkTime << " microseconds" << std::endl;
    //        std::cout << "Normalized: " << report.timeReport.freeSpaceTime/ report.theoRuntimebound << "; " << report.timeReport.graphWorkTime/ report.theoRuntimebound << std::endl;
    //    }
}
