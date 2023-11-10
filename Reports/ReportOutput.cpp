//
// Created by joelh on 05/11/2023.
//

#include "ReportOutput.h"
#include "../testParameters.h"
void ReportOutput::printAndSaveReports(const std::vector<TestRunReport>& reports)
{
//    for(int i = -1; i < static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
//        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
//            if(i == -1 && j == -1) {
//                std::cout << "Runtime" << "\t";
//            } else if(i == -1) {
//                std::cout << NMBR_START_POS[j] << "\t";
//            } else if(j == -1) {
//                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
//            } else {
//                std::cout << reports[i*NMBR_START_POS.size()+j].timeReport.totalTime << "\t";
//            }
//
//        }
//        std::cout << std::endl;
//    }
    printReports(reports, &TimeReport::totalTime, "Runtime");
    printReports(reports, &TimeReport::totalTime, &TestRunReport::theoRuntimebound, "Bound");
//    std::cout << std::endl;
//    for(int i = -1; i <static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
//        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
//            if(i == -1 && j == -1) {
//                std::cout << "Bound" << "\t";
//            } else if(i == -1) {
//                std::cout << NMBR_START_POS[j] << "\t";
//            } else if(j == -1) {
//                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
//            } else {
//                std::cout << std::fixed << std::setprecision(4) << reports[i*NMBR_START_POS.size()+j].timeReport.totalTime/ reports[i*NMBR_START_POS.size()+j].theoRuntimebound << "\t";
//            }
//
//        }
//        std::cout << std::endl;
//    }
    for(int i = -1; i <static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
            if(i == -1 && j == -1) {
                std::cout << "m^2" << "\t";
            } else if(i == -1) {
                std::cout << NMBR_START_POS[j] << "\t";
            } else if(j == -1) {
                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
            } else {
                std::cout << std::fixed << std::setprecision(4) << reports[i*NMBR_START_POS.size()+j].timeReport.totalTime/ std::pow(NMBR_START_POS[j],2) << "\t";
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;    for(int i = -1; i <static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
            if(i == -1 && j == -1) {
                std::cout << "nlogn" << "\t";
            } else if(i == -1) {
                std::cout << NMBR_START_POS[j] << "\t";
            } else if(j == -1) {
                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
            } else {
                std::cout << std::fixed << std::setprecision(4) << reports[i*NMBR_START_POS.size()+j].timeReport.totalTime/ (WORKSPACE_COMPLEXITY[i]* std::log(WORKSPACE_COMPLEXITY[i])) << "\t";
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    printReports(reports, &TimeReport::freeSpaceTime, "freeSp");
    printReports(reports, &TimeReport::freeSpaceTime, &TestRunReport::theoRuntimebound, "freeSp");
    printReports(reports, &TimeReport::stAssociationTime, "stAssoc");
    printReports(reports, &TimeReport::stAssociationTime, &TestRunReport::theoRuntimebound, "stAssoc");
    printReports(reports, &TimeReport::fStarTime, "fStar");
    printReports(reports, &TimeReport::fStarTime, &TestRunReport::theoRuntimebound, "fStar");
    printReports(reports, &TimeReport::mgCreationTime, "mgCrea");
    printReports(reports, &TimeReport::mgCreationTime, &TestRunReport::theoRuntimebound, "mgCrea");
    printReports(reports, &TimeReport::difTime, "dif");
    printReports(reports, &TimeReport::difTime, &TestRunReport::theoRuntimebound, "dif");
    printReports(reports, &TimeReport::mgSolveTime, "mgSolve");
    printReports(reports, &TimeReport::mgSolveTime, &TestRunReport::theoRuntimebound, "mgSolve");
}


void ReportOutput::printReports(const std::vector<TestRunReport> &reports, const long long TimeReport::* field, const std::string& name)
{
    for(int i = -1; i < static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
            if(i == -1 && j == -1) {
                std::cout << name << "\t";
            } else if(i == -1) {
                std::cout << NMBR_START_POS[j] << "\t";
            } else if(j == -1) {
                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
            } else {
                std::cout << (reports[i*NMBR_START_POS.size()+j].timeReport.*field) << "\t";
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void ReportOutput::printReports(const std::vector<TestRunReport> &reports, const long long TimeReport::* field, const double TestRunReport::* runtimeBound, const std::string& name)
{
    for(int i = -1; i < static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
            if(i == -1 && j == -1) {
                std::cout << name << "\t";
            } else if(i == -1) {
                std::cout << NMBR_START_POS[j] << "\t";
            } else if(j == -1) {
                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
            } else {
                std::cout << std::fixed << std::setprecision(3) << (reports[i*NMBR_START_POS.size()+j].timeReport.*field)/(reports[i*NMBR_START_POS.size()+j].*runtimeBound) << "\t";
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
