//
// Created by joelh on 05/11/2023.
//

#ifndef MRMP_IMPLEMENTATION_INTERFERENCEFORESTGENERATOR_H
#define MRMP_IMPLEMENTATION_INTERFERENCEFORESTGENERATOR_H

#include "../mytypedefs.h"
class InterferenceForestGenerator
{
public:
    static DirectedInterferenceForest getInterferenceForest(const std::vector<FreeSpaceComponent>& fSpaceComponents);
};


#endif//MRMP_IMPLEMENTATION_INTERFERENCEFORESTGENERATOR_H
