#include "GenAlg.h"
#include <vector>
#include "GenOp.h"
#include <iostream>

using namespace std;

/////GENETIC_ALGORITHM///////
GeneticAlgorithm::GeneticAlgorithm(
    Panel& panel,
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

Solution* GeneticAlgorithm::run(){
    currentPop = initPopGen.generateInitPopulation(initPopN,panel);
    int iterazione = 0;
    while (!stopCriteria.stop(currentPop)){
        cout<<"iterazione: "<<iterazione<<std::endl;
        auto selected = selOperator.select(currentPop);
        cout<<"elementi selezionati"<<endl;
        auto offspring = genOperator.offspring(selected); 
        cout<<"generata figlianza"<<endl;
        //offspring sono nuovi oggetti
        repOperator.replacement(currentPop,offspring);
        std::cout<<"effettuata replacement"<<endl;
        //current pop aggiorname per side effect
        cout<<"new avgFitness : "<<Solution::avgFitness(currentPop)<<endl;
        iterazione++;
        std::cout<<"--------------------------"<<endl;
    }
    std::cout<<" stopCriteria has happened"<<std::endl;

    auto bestSol = currentPop.begin();
    for (auto i = ++currentPop.begin(); i!=currentPop.end(); i++){
        if ((*i)->fitness() > (*bestSol)->fitness()){
            bestSol = i;
        }
    }
    return *bestSol;

}

