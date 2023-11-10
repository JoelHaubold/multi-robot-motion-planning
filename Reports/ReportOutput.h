//
// Created by joelh on 05/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_REPORTOUTPUT_H
#define MRMP_IMPLEMENTATION_REPORTOUTPUT_H

#include "../mytypedefs.h"

class ReportOutput
{
public:
    static void printAndSaveReports(const std::vector<TestRunReport>& reports);
private:
    static void printReports(const std::vector<TestRunReport>& reports, const long long TimeReport::* field, const std::string& name);
    static void printReports(const std::vector<TestRunReport>& reports, const long long TimeReport::* field, const double TestRunReport::* runtimeBound, const std::string& name);
};


#endif//MRMP_IMPLEMENTATION_REPORTOUTPUT_H
