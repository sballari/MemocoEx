#ifndef GenAlg
#define GenAlg

#include <cstdlib>
#include <vector> 
#include "../Solution.h"


class PopulationGenerator {
    public:
    virtual std::vector<Solution&> generateInitPopulation(int N, Panel& panel)=0;
};

class RandomInsertionGenerator: public PopulationGenerator{
    public:
    std::vector<Solution&> generateInitPopulation(int N, Panel& panel) override;
};

class FitnessOperator {
    public:
    virtual double fitness(Solution& solution)=0;
};

class CostFitness: public FitnessOperator {
    public:
    CostFitness(Panel& p);
    double fitness(Solution& solution) override;
};

class SelectionOperator {
    public:
    virtual std::vector<Solution&> select(std::vector<Solution&> currentPop, FitnessOperator& fitOp) =0;
};

class MonteCarloSelection : public SelectionOperator{
    public:
    std::vector<Solution&> select(std::vector<Solution&> currentPop, FitnessOperator& fitOp) override;   
};
class GeneticOperator {
    public:
    virtual std::vector<Solution&>  offspring(std::vector<Solution&> parents)=0;
};

class CrossOver : public GeneticOperator {
    std::vector<Solution&>  offspring(std::vector<Solution&> parents) override;
};

#endif