#include <vector>
#include "CostrSol.h"
#include "GenOp.h"
#include <limits>
#include <set> 
#include <functional>
#include <iostream>
#include <algorithm>
#include <string>

using std::pair;
using std::vector;
using std::set;
using std::string;
using std::cout;
using std::endl;

////RANDOM_INSERT_GENERATOR////
std::vector<Solution*> RandomInsertionGenerator::generateInitPopulation(int N, Panel& panel){
    auto initPop = std::vector<Solution*>();
    for (int i=0; i<N; i++){
        PathRappr* sol = RandomInsertion::get_sol(panel);
        initPop.push_back(sol);
    }
    return initPop;
}


//SELECTION OPERATOR
std::vector<Solution*> MonteCarloSelection::select(std::vector<Solution*> currentPop) {
    //Super-individuals may be selected too often
    //auto p = std::vector<double>(); //TODO ricordarsi delle fitness gia' calcolate
    double totalFitness = 0;
    for (auto i = currentPop.begin();i!=currentPop.end();i++){
        totalFitness += (*i)->fitness();
    }
    auto selected = std::vector<Solution*> ();
    for (auto i = currentPop.begin();i!=currentPop.end();i++){
        auto pi = (*i)->fitness()/totalFitness; 
        //selezione
        //TODO : possibili problemi di memoria, non mi ricordo come funziona il tutto
        if (rand()%1 <= pi){//selezionato
            selected.push_back(*i);
        }
    }
    return selected;
}
SubStringRevelsal::SubStringRevelsal(int minAlt): minAlt(minAlt){}

std::vector<Solution*>  SubStringRevelsal::offspring(std::vector<Solution*> parents){
    /*
     parents : lista dei genitori selezionati
     description : substring reversal
    */
   
   for (auto i=parents.begin(); i !=parents.end(); i++){
        //combine phase
        //substring reversal
        (*i)->substringReversal(minAlt);  
   }

   return parents;
}

SteadyStateReplacement::SteadyStateReplacement(int changeN): changeN(changeN){}

std::vector<Solution*> SteadyStateReplacement::replacement(std::vector<Solution*> currentPop, std::vector<Solution*> offspring){
    /*
        @description : a few individuals (the worst ones) are replaced, la funzione rimuove le soluzioni uguali
        incremento di diversita'
        TODO : POSSIBILI PROBLEMI IN CASI DOVE MIGLIORE = PEGGIORE
    */
    
    auto worstN = vector<vector<Solution *>::const_iterator>();
    //individuo gli N peggiori
    //peggiore alla fine 
    //migliore all'inizio
    worstN.push_back(currentPop.begin());
    for (auto i = ++currentPop.begin(); i!=currentPop.end(); i++){
        auto fi = (*i)->fitness();
        if (worstN.size()<changeN) {    
            //inserimento ordinato 
            if (fi > (*(worstN.back()))->fitness()) worstN.insert(worstN.end(),i);
            if (fi < (*(worstN.front()))->fitness()) worstN.insert(worstN.begin(),i);
            else worstN.insert(++worstN.begin(),i);
        } 
        else {
            //ho gia' changeN elementi
            if (fi<(*(worstN.back()))->fitness()) { 
                worstN.pop_back(); //via il migliore
                if (fi<(*(worstN.front()))->fitness()) worstN.insert(worstN.begin(),i);
                else worstN.push_back(i);              
                for (int j = 0; j<worstN.size(); j++){
                    double f = (*(worstN[j]))->fitness();
                    if (f > (*(worstN.back()))->fitness()) { 
                        auto aux = worstN.back();
                        worstN[worstN.size()-1] = worstN[j];
                        worstN[j]=aux;     
                    }
                }   
            }
        }
    }
    //trovati i peggiori
    auto bestN = vector<vector<Solution *>::const_iterator>();
    bestN.push_back(offspring.begin());
    for (auto i = ++offspring.begin(); i!=offspring.end(); i++){
        auto fi = (*i)->fitness();
        if (bestN.size()<changeN) {    
            //inserimento ordinato 
            if (fi > (*(bestN.front()))->fitness()) bestN.insert(bestN.begin(),i);
            if (fi < (*(bestN.back()))->fitness()) bestN.insert(bestN.end(),i);
            else bestN.insert(++bestN.begin(),i);
        } 
        else {
            //ho gia' changeN elementi
            if (fi>(*(bestN.back()))->fitness()) { 
                bestN.pop_back(); //via il peggiore
                if (fi>(*(bestN.front()))->fitness()) bestN.insert(bestN.begin(),i);
                else bestN.insert(--bestN.end(),i); //lo metto prima dell'ultimo per minimizzare gli spostamenti 
                //cerco il nuovo peggiore            
                for (int j = 0; j<bestN.size(); j++){
                    double f = (*(bestN[j]))->fitness();
                    if (f < (*(bestN.back()))->fitness()) { 
                        auto aux = bestN.back();
                        bestN[bestN.size()-1] = bestN[j];
                        bestN[j]=aux;     
                    }
                }   
            }
        }
    }
    //switch worstN with best N
    
    if (bestN.size()<worstN.size()) throw string("check size non passato");
    auto b = bestN.begin();
    for (auto w = worstN.begin(); w!=worstN.end(); w++){
        delete (**w); //elimino l'oggetto soluzione
        auto ne = currentPop.insert(*w,**b); //inserisco l'elemento nuovo
        currentPop.erase(ne+1); //elimino il puntatore diventato vecchio
        b++;
    }
    //fine test
    return currentPop;
}

 