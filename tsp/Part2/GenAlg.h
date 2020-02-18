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
    Panel* panel; 
    PopulationGenerator& initPopGen;
    StoppingCriteria& stopCriteria;
    SelectionOperator& selOperator;
    GeneticOperator& genOperator;
    ReplacementOperator& repOperator;
    
    //DATA
    std::vector<Solution*> currentPop = std::vector<Solution*>();
    
    public:
    GeneticAlgorithm(
        Panel* panel,
        PopulationGenerator& pg,
        int initPopN,
        StoppingCriteria& stopCrit,
        SelectionOperator& selOp,
        GeneticOperator& genOp,
        ReplacementOperator& repOp
    );
    Solution* run(bool plot_avgF = false,bool Verbose=false);
    void changePanel(Panel* newPanel);
    

};



#endif