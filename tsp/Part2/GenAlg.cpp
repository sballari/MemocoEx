#include "GenAlg.h"
#include <vector>
#include "GenOp.h"
#include <iostream>
#include "../matplotlib-cpp/matplotlibcpp.h"
namespace plt = matplotlibcpp;

using namespace std;

/////GENETIC_ALGORITHM///////
GeneticAlgorithm::GeneticAlgorithm(
    const Panel* panel,
    PopulationGenerator& pg, 
    int initPopN,
    StoppingCriteria& sc,
    SelectionOperator& selOp,
    GeneticOperator& genOp,
    ReplacementOperator& repOp
    ):
    panel(panel),
    initPopGen(pg),
    initPopN (initPopN),
    stopCriteria(sc),
    genOperator(genOp),
    selOperator(selOp),
    repOperator(repOp)
{}

Solution* GeneticAlgorithm::run(bool plot_avgF, bool Verbose,double optVal){
    reset();
    currentPop = initPopGen.generateInitPopulation(initPopN,panel);
    int iterazione = 0;
    std::vector<double> avgFitnessV ={}; //codice per plot 
    std::vector<double> iterazioni ={}; //codice per plot
    while (!stopCriteria.stop(currentPop)){
        if (Verbose) cout<<"iterazione: "<<iterazione<<std::endl;
        auto selected = selOperator.select(currentPop);
        auto offspring = genOperator.offspring(selected); 
        //offspring sono nuovi oggetti
        repOperator.replacement(currentPop,offspring);
        //current pop aggiorname per side effect
        auto avgFitness = Solution::avgFitness(currentPop);
        if (Verbose) cout<<"new avgFitness : "<<avgFitness<<endl;
        if (plot_avgF){
            iterazioni.push_back(iterazione);
            avgFitnessV.push_back(avgFitness);
        }
        iterazione++;
        if (Verbose) std::cout<<"--------------------------"<<endl;
    }
    iterazioniLastRun = iterazione;
    if (Verbose) cout<<"iterazioni: "<<iterazioniLastRun<<endl;
    if (plot_avgF){
            plt::title("avg Fitness");
            if (optVal!=-1) {
                vector<double> opt = vector<double>(iterazioni.size(),optVal);
                plt::named_plot("optimum",iterazioni,opt,"red");
            }
            plt::named_plot("avg fitness",iterazioni,avgFitnessV,"blue");
            plt::legend();
            plt::show();
    }
    //TROVO IL MIGLIORE e cancello le altre
    auto bestSol = currentPop.begin();
    for (auto i = ++currentPop.begin(); i!=currentPop.end(); i++){
        if ((*i)->fitness() > (*bestSol)->fitness()){
            delete (*bestSol); //cancello la vecchia bestSol
            bestSol = i;
        }
        else delete (*i);
    }
    return *bestSol;

}

void GeneticAlgorithm::changePanel(const Panel* newPanel){
    panel = newPanel;   
}

void GeneticAlgorithm::reset(){
    iterazioniLastRun = -1;
    initPopGen.reset();
    stopCriteria.reset();
    selOperator.reset();
    genOperator.reset();
    repOperator.reset();
}