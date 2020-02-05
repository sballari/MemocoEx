#include "GenAlg.h"
#include <vector>
#include "GenOp.h"

/////GENETIC_ALGORITHM///////
GeneticAlgorithm::GeneticAlgorithm(Panel& panel,PopulationGenerator& pg, int initPopN,FitnessOperator& fitOp):
    panel(panel),
    initPopGen(pg),
    initPopN (initPopN),
    fitOperator (fitOp)
{};

void GeneticAlgorithm::generateInitPop(){
    currentPop = initPopGen.generateInitPopulation(initPopN,panel);
}

