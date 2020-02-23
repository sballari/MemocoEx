#ifndef GenOp
#define GenOp

#include <cstdlib>
#include <vector> 
#include <limits>
#include "../Solution.h"

class GenOperator {
    public :
    virtual void reset() {}; //resetto tutta la memoria che usa tra le iterazioni (per il cambio pannello per esempio)
};
class PopulationGenerator : public GenOperator{
    public:
    virtual std::vector<Solution*> generateInitPopulation(int N, const Panel* panel)=0;
};

class RandomInsertionGenerator: public PopulationGenerator{
    public:
    std::vector<Solution*> generateInitPopulation(int N, const Panel* panel) override;
};


class SelectionOperator: public GenOperator{
    public:
    virtual std::vector<Solution*> select(std::vector<Solution*>& currentPop) =0;
};

class MonteCarloSelection : public SelectionOperator{
    /*
    @description: ritorna un nuovo di vettore di puntatori a oggetti che esistono in
    currentPop
    */
    public:
    std::vector<Solution*> select(std::vector<Solution*>& currentPop) override;   
};
class GeneticOperator :public GenOperator{
    public:
    virtual std::vector<Solution*>  offspring(std::vector<Solution*> parents)=0;
};

class SubStringRevelsal : public GeneticOperator {
    private:
        int minAlt;
        int maxAlt;
    public:
        SubStringRevelsal(int minAlt, int maxAlt);
        std::vector<Solution*> offspring(std::vector<Solution*> parents) override;
};

class OrderCrossOver: public GeneticOperator {
    private:
        int minAlt;
        int maxAlt;
    public:
        OrderCrossOver(int minAlt, int maxAlt);
        std::vector<Solution*> offspring(std::vector<Solution*> parents) override;
};

class ReplacementOperator :public GenOperator {
    public:
    virtual void replacement(std::vector<Solution*>& currentPop, std::vector<Solution*>& offspring)=0;

};

class SteadyStateReplacement : public ReplacementOperator {
    private:
    int changeN = 5;
    public : 
        SteadyStateReplacement(int changeN);
        void replacement(std::vector<Solution*>& currentPop, std::vector<Solution*>& offspring) override;
};

class StoppingCriteria :public GenOperator{
    public:
        virtual bool stop(std::vector<Solution*>& currentPop) =0;
};

class NotImprovingCriteria : public StoppingCriteria{
    private:
        int attempt = 0;
        int maxAttempt;
        double minIncrement;
        double previuslyAvgFitness = std::numeric_limits<double>::max();
    public :
        bool stop( std::vector<Solution*>& currentPop) override;
        NotImprovingCriteria(double minIncr, int maxAttemp);
        void reset() override;
};
#endif