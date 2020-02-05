#ifndef GenAlg
#define GenAlg

#include <cstdlib>
#include <vector> 
#include "../Solution.h"
#include "GenOp.h"

class GeneticAlgorithm {
    private:
    //iperparametri
    int initPopN;
    //components 
    Panel& panel; 
    PopulationGenerator& initPopGen;
    // FitnessOperator& fitOperator;

    //DATA
    std::vector<Solution&> currentPop = std::vector<Solution&>();
    public:
    GeneticAlgorithm(Panel& panel, PopulationGenerator& pg, int initPopN);
    void generateInitPop();
    

};



#endif