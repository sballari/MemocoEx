#include <vector>
#include "CostrSol.h"
#include "GenOp.h"
#include <limits>
#include <set> 
#include <functional>
#include <iostream>
#include <algorithm>
#include <string>
#include<cmath>
#include <random>
#include <chrono>

using std::pair;
using std::vector;
using std::set;
using std::string;
using std::cout;
using std::endl;

//utility function
double avgFitness(std::vector<Solution*>& sols){
    double sumFit=0;
    for (auto i = sols.begin();i!=sols.end(); i++){
        sumFit+=(*i)->fitness();
    }
    return sumFit/sols.size();
}

////RANDOM_INSERT_GENERATOR////
std::vector<Solution*> RandomInsertionGenerator::generateInitPopulation(int N, const Panel* panel){
    auto initPop = std::vector<Solution*>();
    for (int i=0; i<N; i++){
        PathRappr* sol = RandomInsertion::get_sol(panel);
        initPop.push_back(sol);
    }
    //???????????????????????????????????????????????????????
    //codice di dati di initoPOp TODO :[rimuovere]
    /*
    double avg = avgFitness(initPop);
    double sqm = 0;
    for (auto i = initPop.begin();i!=initPop.end();i++){
        sqm += ((*i)->fitness()-avg)*((*i)->fitness()-avg);
    }
    sqm = std::sqrt(sqm/initPop.size());
    cout<<"initPop avg "<<avg<<endl;
    cout<<"initPop sqm "<<sqm<<endl;
    */
    //?????????????????????????????????????????????????????
    return initPop;
}


//SELECTION OPERATOR
std::vector<Solution*> MonteCarloSelection::select(std::vector<Solution*>& currentPop) {
    /*
    @return : vector di puntatore ad oggetti di currentPop
    */
    //Super-individuals may be selected too often
    //TODO ricordarsi delle fitness gia' calcolate

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine gen(seed);
    std::uniform_real_distribution<double> dis(0, 1);

    double totalFitness = 0;
    double worstFitness = -1; //highest // TODO
    double bestFitness =  std::numeric_limits<double>::max(); //lower // TODO
    for (auto i = currentPop.begin();i!=currentPop.end();i++){
        totalFitness += (*i)->fitness();
        if ((*i)->fitness() > worstFitness) worstFitness = (*i)->fitness();// TODO
        if ((*i)->fitness() < bestFitness) bestFitness = (*i)->fitness();// TODO
    }
    auto selected = std::vector<Solution*> ();
    for (auto i = currentPop.begin();i!=currentPop.end();i++){
        auto pi =  (worstFitness - (*i)->fitness())/(worstFitness- bestFitness) ;// (*i)->fitness()/totalFitness; //<---- TODO
        // pi = 0.2;
        double r = dis(gen);
        //selezione
        //TODO : possibili problemi di memoria, non mi ricordo come funziona il tutto
        //TODO : il padre potrebbe uscire piu' volte. Problema? No, non lo faccio
        
        if (r <= pi){//selezionato
            // cout<<"selezionata fitness "<<(*i)->fitness()<<endl;
            selected.push_back(*i);
        }
    }
    return selected;
}
SubStringRevelsal::SubStringRevelsal(int _minAlt,int _maxAlt): minAlt(_minAlt), maxAlt(_maxAlt){}

OrderCrossOver::OrderCrossOver(int _minAlt,int _maxAlt): minAlt(_minAlt), maxAlt(_maxAlt){}

std::vector<Solution*>  OrderCrossOver::offspring(std::vector<Solution*> parents){
    /*
    Returns a vector of new sons (in the heap). offspring.size == parents.size, 
    if size <2 then offspring.size = 0 (no son will be created)
    @param parents : vector of parents
    @description: prende i parents a coppie (1,2),(2,3),(3,4) .. e genera figli con 
    order cross over.
    */
    
    
    std::vector<Solution*> offspring = {};
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());
    
    if (parents.size()<2) return offspring; //empty vector (not enougth parents)
    for (auto i = ++parents.begin(); i!=parents.end(); i++){
        auto j = i;
        j--;
        auto p1 = dynamic_cast<PathRappr*>(*i);
        auto p2 = dynamic_cast<PathRappr*>(*j);
        if (p1==nullptr|| p2==nullptr) throw string("GenOp non compatibile: non stai usando una PathRappr");
        
        int delta = 0;
        int start = 0;
        int stop =0;
        while (delta<minAlt || delta >maxAlt){ //selezione k1 , k2
            std::uniform_int_distribution<> dis1(0, (p1->getHolesN()-1)-maxAlt);
            int k1 = dis1(gen);
            std::uniform_int_distribution<> dis2(k1+minAlt, k1+maxAlt);
            int k2 = dis2(gen);
            start = (k1 <= k2)? k1 : k2;
            stop = (k1 >= k2)? k1 : k2;
            delta = stop - start; //(*p1)[stop]-(*p1)[start];<------ TOlO TOlO
        }
        // std::cout<<start<<" , "<<stop<<" delta : "<<delta<<" min-max : "<<minAlt<<","<<maxAlt<<std::endl;
        auto twins = PathRappr::orderCrossover(p1,p2,start, stop);
        offspring.push_back(twins[0]);
        offspring.push_back(twins[1]);
    }
    return offspring;
}

std::vector<Solution*>  SubStringRevelsal::offspring(std::vector<Solution*> parents){
    /*
     parents : lista dei genitori selezionati
     description : substring reversal
    */
    auto offspring = std::vector<Solution*>();
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());
    for (auto i=parents.begin(); i !=parents.end(); i++){
            //combine phase
            //substring reversal
            Solution* son = (*i)->clone();

            int delta = 0;
            int start = 0;
            int stop =0;
            while (delta<minAlt || delta >maxAlt){ //selezione k1 , k2
                std::uniform_int_distribution<> dis1(0, (son->getHolesN()-1)-maxAlt);
                int k1 = dis1(gen);
                std::uniform_int_distribution<> dis2(k1+minAlt, k1+maxAlt);
                int k2 = dis2(gen);
                start = (k1 <= k2)? k1 : k2;
                stop = (k1 >= k2)? k1 : k2;
                delta = stop-start;
            }
            // std::cout<<start<<" , "<<stop<<" delta : "<<delta<<std::endl;
            
            son->substringReversal(start, stop);  
            offspring.push_back(son);
    }

   return offspring;
}

SteadyStateReplacement::SteadyStateReplacement(int changeN): changeN(changeN){}

void SteadyStateReplacement::replacement(std::vector<Solution*>& currentPop, std::vector<Solution*>& offspring){
    /*
        @description : a few individuals (the worst ones) are replaced, la funzione rimuove le soluzioni uguali
        incremento di diversita', ripulisce anche completamento la offsprint non usata
        l'offspring e i rimossi da current sono deletati.
        #side effect su currentPop !
    */
    
    
    int _changeN = changeN; //caso particolare, se non ho almeno change N elementi setto _changeN alla
    //size della figlianza e poi, all'iterazione successiva _changeN ritorna al valore suo.
    if (offspring.size()<changeN) _changeN = offspring.size();
    if (_changeN==0){
        for (auto i = offspring.begin(); i!=offspring.end();i++) if (*i!=nullptr) delete *i;
        return;
    }
    
    offspring.reserve(offspring.size()); //per evitare shringking del vettore e quindi L'invalidazione 
    currentPop.reserve(currentPop.size()); //dei puntatori

    //test fine
    auto worstN = vector<vector<Solution *>::const_iterator>();
    worstN.reserve(_changeN);
    //individuo gli N peggiori
    //peggiore alla fine 
    //migliore all'inizio
    worstN.push_back(currentPop.begin());
    for (auto i = ++currentPop.begin(); i!=currentPop.end(); i++){
        auto fi = (*i)->fitness();
        if (worstN.size()<_changeN) {    
            //inserimento ordinato 
            if (fi > (*(worstN.back()))->fitness()) worstN.insert(worstN.end(),i);
            else {
                if (fi < (*(worstN.front()))->fitness()) worstN.insert(worstN.begin(),i);
                else worstN.insert(++worstN.begin(),i);
            }
        } 
        else {
            //ho gia' _changeN elementi
            if (fi>(*(worstN.back()))->fitness()) { 
                worstN.pop_back(); //via il peggiore
                if (fi>(*(worstN.front()))->fitness()) worstN.insert(worstN.begin(),i);
                else worstN.insert(--worstN.end(),i); //lo metto prima dell'ultimo per minimizzare gli spostamenti 
                //cerco il nuovo peggiore            
                for (int j = 0; j<worstN.size(); j++){
                    double f = (*(worstN[j]))->fitness();
                    if (f < (*(worstN.back()))->fitness()) { 
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
    //peggiore in testa 
    //migliore in coda
    bestN.reserve(_changeN);
    bestN.push_back(offspring.begin());
    for (auto i = ++offspring.begin(); i!=offspring.end(); i++){
        auto fi = (*i)->fitness();
        if (bestN.size()<_changeN) {    
            //inserimento ordinato 
            if (fi > (*(bestN.front()))->fitness()) bestN.insert(bestN.begin(),i);
            else {
                if (fi < (*(bestN.back()))->fitness()) bestN.insert(bestN.end(),i);
                else bestN.insert(++bestN.begin(),i);
            }
        } 
        else {
            //ho gia' _changeN elementi
            if (fi<(*(bestN.back()))->fitness()) { 
                bestN.pop_back(); //via il migliore
                if (fi<(*(bestN.front()))->fitness()) bestN.insert(bestN.begin(),i);
                else bestN.push_back(i);              
                for (int j = 0; j<bestN.size(); j++){
                    double f = (*(bestN[j]))->fitness();
                    if (f > (*(bestN.back()))->fitness()) { 
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
        // cout<<"cambio "<<(**w)->fitness()<<" con "<<(**b)->fitness()<<endl;
        delete (**w); //elimino la soluzione scartata da currentPop
        currentPop.erase(*w);    //elimino il puntatore diventato vecchio
        auto ne = currentPop.insert(*w,**b); //inserisco l'elemento nuovo 
        offspring.erase(*b); //lo rimuovo anche dalla offspring perche' questa dopo sara' ripulita
        offspring.insert(*b,nullptr); //per evitare resising vettore (anche se non ci dovrebbe essere)
        b++;
    }
    //pulizia offspring non usata
    for (auto i = offspring.begin(); i!=offspring.end();i++) if (*i!=nullptr) delete *i;
}

bool NotImprovingCriteria::stop(std::vector<Solution*>& currentPop){
    double current_avg = avgFitness(currentPop);
    
    double improvement = previuslyAvgFitness - current_avg;
    bool stop = improvement  < minIncrement;
    previuslyAvgFitness = current_avg;

    if (stop == false) attempt = 0;
    else attempt ++;

    if (attempt<maxAttempt) return false;
    return stop;
}
NotImprovingCriteria::NotImprovingCriteria(double minIncr, int maxAttempt)
: minIncrement(minIncr), maxAttempt(maxAttempt) {}

void NotImprovingCriteria::reset(){
    attempt = 0;
    previuslyAvgFitness = std::numeric_limits<double>::max();
}