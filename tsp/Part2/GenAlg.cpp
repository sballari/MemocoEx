#include "GenAlg.h"
#include <vector>

/////GENETIC_ALGORITHM///////
GeneticAlgorithm::GeneticAlgorithm(Panel& panel,PopulationGenerator& pg, int initPopN,FitnessOperator& fitOp){
    this->panel = panel;
    initPopGen = pg;
    this->initPopN = initPopN;
    this->fitOperator = fitOp;
};

void GeneticAlgorithm::generateInitPop(){
    currentPop = initPopGen.generateInitPopulation(initPopN,panel);
}

