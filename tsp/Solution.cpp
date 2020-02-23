#include <cstdlib>
#include <vector> 
#include "Solution.h"
#include <iostream>
#include "matplotlib-cpp/matplotlibcpp.h"
#include <random>

namespace plt = matplotlibcpp;



PathRappr::~PathRappr(){};
PathRappr::PathRappr(std::vector<int> p, const Panel* panel) {
    this->panel = panel;
    path=p;
};
PathRappr::PathRappr(const Panel* panel) {
    this->panel = panel;
    path = std::vector<int>();
};
void PathRappr::addCity(int city){
    path.push_back(city);
};

 void PathRappr::plot(bool plot) {
    panel->plot(false);
    auto Xs = std::vector<double>();
    auto Ys = std::vector<double>();

    for (auto j = path.begin(); j!=path.end()--; j++){
        Xs.push_back(panel->getPoint(*j)[0]);
        Ys.push_back(panel->getPoint(*j)[1]);
    }
    plt::plot(Xs,Ys,"blue");
    if (plot) plt::show();
 };

double PathRappr::evaluate_cost(){
    if (costValue!=-1) return costValue; 
    double cost = 0;
    for( auto i = ++path.begin(); i!= path.end(); i++ ){
        auto j = i;
        j--;
        cost+= panel->get_dist(*j,*i);
    }
    costValue = cost;
    return cost;
}

// void PathRappr::setFitnessOp(FitnessOperator& fitOp){
//     fitnessOp = fitOp;
// }
double PathRappr::fitness(){
    //return fitnessOp.fitness(*this);
    return evaluate_cost();
}

std::vector<PathRappr*> PathRappr::orderCrossover(const PathRappr* p1, const PathRappr* p2, int start, int stop){
    /* 
    Returns a vector with two new solution (int the heap) obtained combaning p1 and p2
    @description : crea dei nuovi oggetti, i genitori vengono lasciati stare
    */
    auto path1 = p1->path;
    auto path2 = p2->path;

    std::vector<int> off1p = std::vector<int>(path1.size(),-1);
    std::vector<int> off2p = std::vector<int>(path1.size(),-1);

    //creazione di off1p
    int s =stop+1; //indice su padre
    int stop_ = stop;
    for (int i=1; i<=path1.size(); i++){ // s indice figlio
        if (stop_+i==path1.size()) {stop_= -i; continue;}
        if (s==path1.size()-1) s=0;
        //controllo presenta in sezione centrale di i
        bool present = false;
        for (int j=0; j<=(stop-start) & !present; j++) { //controllo presenza in sezione centrale
            if (path1[start+j] == path2[stop_+i]) present = true;
        }
        if (!present) {off1p[s]=path2[stop_+i]; s++;}
        //else provo i successivo
    }
    
    //creazione di off2p
    s = stop+1;// s indice figlio
    stop_ = stop;
    for (int i=1; i<=path1.size(); i++){  // i indice su padre
        if (stop_+i==path1.size()) {stop_= -i; continue;}
        if (s==path1.size()-1) s=0;
        //controllo presenta in sezione centrale di i
        bool present = false;
        for (uint j=0; j<=(stop-start) & !present; j++) { //controllo presenza in sezione centrale
            if (path2[start+j] == path1[stop_+i]) present = true;
        }
        if (!present) {off2p[s]=path1[stop_+i]; s++;}
        //else provo i successivo
    }

    // creazione sezione centrale di entrambi
    for (int i=0; i<=(stop-start); i++){  //sezione centrale
        off1p[start+i] = path1[start+i];
        off2p[start+i] = path2[start+i];
    }

    off1p[path1.size()-1] = off1p[0];
    off2p[path1.size()-1] = off2p[0];

    const Panel* panel = (p1->panel);
    PathRappr* off1 = new PathRappr(off1p, panel);
    off1->costValue =-1;
    PathRappr* off2 = new PathRappr(off2p, panel);
    off2->costValue =-1;
    std::vector<PathRappr*> offspring = {off1,off2};
    return offspring;
}

void PathRappr::substringReversal(int start,int stop) {
    /*
    @description: side effect sull'oggetto chiamante. 
    Start e stop devono essere comprese tra 0 e holesN-1
    @param start : inizio del reversal
    @param stop : fine del reversal
    */
    
    for (int i=0; i<=(stop-start)/2; i++){
        costValue -= panel->get_dist(path[start+i],path[stop-i]);
        int tmp = path[stop-i];
        path[stop-i] = path[start+i];
        path[start+i] = tmp;
        costValue += panel->get_dist(path[start+i],path[stop-i]);
    }
}


PathRappr* PathRappr::clone() const{
    return new PathRappr(*this);
}

double Solution::avgFitness(const std::vector<Solution*>& s){
    double sum=0;
    for (auto i = s.begin(); i!=s.end(); i++){
        sum += (*i)->fitness();
    }
    return sum/s.size();
}

bool PathRappr::checkCorrectness() const{
    /*
    Returns true <-> ciclo hamiltoniano
    @description : controlla se il ciclo e' valido, quindi se non 
    ci sono ripetizioni di nodi e se compaiono tutti.
    */
    int n = path.size()-1; //tolto l'ultimo che e' ripetuto
    int sum = n*(n+1)/2; //somma 1->holesN (n incluso, 0 == n)
    for (auto i = path.begin(); i!= path.end(); i++){
        if ((*i) ==0) sum-= n;
        sum-=(*i);
    }
    int firstNode = (path[0]==0)? n : path[0];
    if (sum == (-firstNode)) return true; //ho tolto due volte la partenza
    else return false;
}

void PathRappr::printSol() const {
    std::cout<<"path"<<std::endl;
    for (auto i = path.begin(); i!=path.end(); i++){
        std::cout<<(*i)<<">";
    }
    std::cout<<std::endl;
}

int PathRappr::getHolesN() const {
    return panel->get_holesN();
}