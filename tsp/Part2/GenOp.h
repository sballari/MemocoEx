#ifndef GenOp
#define GenOp

#include <cstdlib>
#include <vector> 
#include "../Solution.h"


class PopulationGenerator {
    public:
    virtual std::vector<Solution*> generateInitPopulation(int N, Panel& panel)=0;
};

class RandomInsertionGenerator: public PopulationGenerator{
    public:
    std::vector<Solution*> generateInitPopulation(int N, Panel& panel) override;
};



class SelectionOperator {
    public:
    virtual std::vector<Solution*> select(std::vector<Solution*> currentPop) =0;
};

class MonteCarloSelection : public SelectionOperator{
    public:
    std::vector<Solution*> select(std::vector<Solution*> currentPop) override;   
};
class GeneticOperator {
    public:
    virtual std::vector<Solution*>  offspring(std::vector<Solution*> parents)=0;
};

class SubStringRevelsal : public GeneticOperator {
    private:
        int minAlt = 0;
    public:
        SubStringRevelsal(int minAlt);
        std::vector<Solution*> offspring(std::vector<Solution*> parents) override;
};

class ReplacementOperator {
    public:
    virtual std::vector<Solution*> replacement(std::vector<Solution*> currentPop, std::vector<Solution*> offspring)=0;

};

class SteadyStateReplacement : public ReplacementOperator {
    private:
    int changeN = 5;
    public : 
        SteadyStateReplacement(int changeN);
        std::vector<Solution*> replacement(std::vector<Solution*> currentPop, std::vector<Solution*> offspring) override;
};
#endif