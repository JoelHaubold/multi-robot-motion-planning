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
};


#endif//MRMP_IMPLEMENTATION_REPORTOUTPUT_H
