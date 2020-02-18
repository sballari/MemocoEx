#include "CostrSol.h"
#include "../DataGenerator.h"
#include <iostream>
#include <vector> 
#include <cstdlib>
#include <stdlib.h>
#include <limits>
#include <utility>
#include <time.h>
#include <random>
#include "../DataGenerator.h"
#include "../Solution.h"


PathRappr* RandomInsertion::get_sol(Panel& panel){
    // descr : algoritmo che risolve il problema TSP sul grafo G usando l'euristica Random Insertion. La soluzione ottenuta e' 2-approssimata
    // panel : grafo pesato
    // ret : tupla contenente il circuito hamiltoniano approssima con Random insertion e il tempo di esecuzione dell'algoritmo
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd());
    

    std::vector<int> circParz = std::vector<int> ();
    circParz.push_back(0);
    std::vector<int> nodi_non_usati = std::vector<int> ();
    //riempio il vettore dei nodi non usati.
    for (int i=1; i<panel.get_holesN(); i++){
        nodi_non_usati.push_back(i);
    }

    //scelto il primo arco
    std::vector<int>::iterator minimo = nodi_non_usati.begin();
    for (auto i = nodi_non_usati.begin()++; i!=nodi_non_usati.end(); i++ ){
        if (panel.get_dist(0,*i) < panel.get_dist(0,*minimo)) minimo = i;
    }

    circParz.push_back(*minimo);
    circParz.push_back(0); //l'ultimo elemente per chiudere il ciclo [non serve, tanto per]
    nodi_non_usati.erase(minimo);
    
    while (!nodi_non_usati.empty()) {
        //int rint = rand()%nodi_non_usati.size();
        std::uniform_int_distribution<> dis(0, nodi_non_usati.size()-1);
        int rint = dis(gen);        
        int k = nodi_non_usati[rint];
        //rimuovo il nodo k dai nodi non usati
        if (rint == nodi_non_usati.size()-1)
            nodi_non_usati.pop_back();
        else { //il codice seguente e' per evitare la ricompattazzione di vector
            nodi_non_usati[rint] = nodi_non_usati[nodi_non_usati.size()-1];
            nodi_non_usati.pop_back();
        }

        //dichiaro minimaC fittizzio
        auto minimaC = std::make_pair(circParz.begin(), circParz.begin());
        double delta_min = std::numeric_limits<double>::max();

        //trovo l'intermezzo piu' conveniente dove inserire k
        for (auto j=circParz.begin()++; j!=circParz.end(); j++){
            auto i = j;
            i--;
            double djk = panel.get_dist(*j,k);
            double dik = panel.get_dist(*i,k);
            double dij = panel.get_dist(*i,*j);
            double delta = djk + dik - dij;
            if (delta < delta_min){
                minimaC = make_pair(i, j);
                delta_min = delta;
            }
        }
        //ho trovato dove inserire k, quindi inserisco del cammino
        circParz.insert(std::get<1>(minimaC),k);
    }
    return new PathRappr(circParz,panel.clone());
}