//
// Created by joelh on 05/11/2023.
//

#include "ReportOutput.h"
#include "../runtimeTestValues.h"
void ReportOutput::printAndSaveReports(const std::vector<TestRunReport>& reports)
{
    for(int i = -1; i < static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
            if(i == -1 && j == -1) {
                std::cout << "Runtime" << "\t";
            } else if(i == -1) {
                std::cout << NMBR_START_POS[j] << "\t";
            } else if(j == -1) {
                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
            } else {
                std::cout << reports[i+j].timeReport.totalTime << "\t";
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for(int i = -1; i <static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
            if(i == -1 && j == -1) {
                std::cout << "Bound" << "\t";
            } else if(i == -1) {
                std::cout << NMBR_START_POS[j] << "\t";
            } else if(j == -1) {
                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
            } else {
                std::cout << reports[i+j].timeReport.totalTime/ reports[i+j].theoRuntimebound << "\t";
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    for(int i = -1; i <static_cast<int>(WORKSPACE_COMPLEXITY.size()); i++) {
        for(int j = -1; j < static_cast<int>(NMBR_START_POS.size()); j++) {
            if(i == -1 && j == -1) {
                std::cout << "m^2" << "\t";
            } else if(i == -1) {
                std::cout << NMBR_START_POS[j] << "\t";
            } else if(j == -1) {
                std::cout << WORKSPACE_COMPLEXITY[i] << "\t";
            } else {
                std::cout << reports[i+j].timeReport.totalTime/ std::pow(NMBR_START_POS[j],2) << "\t";
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
                std::cout << reports[i+j].timeReport.totalTime/ (WORKSPACE_COMPLEXITY[i]* std::log(WORKSPACE_COMPLEXITY[i])) << "\t";
            }

        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
