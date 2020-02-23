#ifndef GenAlg
#define GenAlg

#include <cstdlib>
#include <vector> 
#include "../Solution.h"
#include "GenOp.h"
#include <string>

class AbsGenAlg {
    public:
    virtual ~AbsGenAlg(){};
    virtual Solution* run(bool plot_avgF = false,bool Verbose=false, double optVal = -1) = 0;
    virtual void changePanel(const Panel* newPanel) = 0;
    virtual void reset()=0;
    std::string colorPlot = "blue";
    std::string legendName = "avgFiness";
};
class GeneticAlgorithm: public AbsGenAlg {
    private:
    //iperparametri
    int initPopN;
    //components 
    const Panel* panel; 
    PopulationGenerator& initPopGen;
    StoppingCriteria& stopCriteria;
    SelectionOperator& selOperator;
    GeneticOperator& genOperator;
    ReplacementOperator& repOperator;
    
    //DATA
    std::vector<Solution*> currentPop = std::vector<Solution*>();
    
    public:
    ~GeneticAlgorithm(){};
    GeneticAlgorithm(
        const Panel* panel,
        PopulationGenerator& pg,
        int initPopN,
        StoppingCriteria& stopCrit,
        SelectionOperator& selOp,
        GeneticOperator& genOp,
        ReplacementOperator& repOp
    );
    int iterazioniLastRun = -1;
    Solution* run(bool plot_avgF = false,bool Verbose=false, double optVal = -1);
    void changePanel(const Panel* newPanel);
    void reset() override;
    

};



#endif