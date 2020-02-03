#include <vector>
#include "CostrSol.h"
#include "GenOp.h"

////RANDOM_INSERT_GENERATOR////
std::vector<Solution&> RandomInsertionGenerator::generateInitPopulation(int N, Panel& panel){
    auto initPop = std::vector<Solution&>();
    for (int i=0; i<N; i++){
        PathRappr sol = RandomInsertion::get_sol(panel);
        initPop.push_back(sol);
    }
    return initPop;
}

//FITNESS OPERATOR///
CostFitness::CostFitness(Panel& p) : panel(p) {}

double CostFitness::fitness(Solution& solution){
    return solution.evaluate_cost(panel);
}
//SELECTION OPERATOR
std::vector<Solution&> MonteCarloSelection::select(std::vector<Solution&> currentPop, FitnessOperator& fitOp) {
    //Super-individuals may be selected too often
    //auto p = std::vector<double>(); //TODO ricordarsi delle fitness gia' calcolate
    double totalFitness = 0;
    for (auto i = currentPop.begin();i!=currentPop.end();i++){
        totalFitness += fitOp.fitness(*i);
    }
    auto selected = std::vector<Solution&> ();
    for (auto i = currentPop.begin();i!=currentPop.end();i++){
        auto pi = fitOp.fitness((*i)/totalFitness); 
        //selezione
        //TODO : possibili problemi di memoria, non mi ricordo come funziona il tutto
        if (rand()%1 <= pi){//selezionato
            selected.push_back(*i);
        }
    }
    return selected;
}