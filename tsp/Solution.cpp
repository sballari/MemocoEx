#include <cstdlib>
#include <vector> 
#include "Solution.h"
#include "matplotlib-cpp/matplotlibcpp.h"

namespace plt = matplotlibcpp;

Panel* PathRappr::panel = nullptr;

PathRappr::PathRappr(std::vector<int> p, Panel& panel) {
    this->panel = &panel;
    path=p;
};
PathRappr::PathRappr(Panel& panel) {
    this->panel = &panel;
    path = std::vector<int>();
};
void PathRappr::addCity(int city){
    path.push_back(city);
};

 void PathRappr::plot() {
    panel->plot(false);
    auto Xs = std::vector<double>();
    auto Ys = std::vector<double>();

    for (auto j = path.begin(); j!=path.end()--; j++){
        Xs.push_back(panel->getPoint(*j)[0]);
        Ys.push_back(panel->getPoint(*j)[1]);
    }
    plt::plot(Xs,Ys,"blue");
    plt::show();
 };

double PathRappr::evaluate_cost(){
    double cost = 0;
    for( auto i = path.begin()++; i!= path.end(); i++ ){
        auto j = i;
        j--;
        cost+= panel->get_euc_dist(*j,*i);
    }
    return cost;
}

// void PathRappr::setFitnessOp(FitnessOperator& fitOp){
//     fitnessOp = fitOp;
// }
double PathRappr::fitness(){
    //return fitnessOp.fitness(*this);
    return evaluate_cost();
}

// PathRappr& PathRappr::combine(PathRappr& parent2) {
//     // Mutation by substring reversal
//     // guaranteeing feasibility
//     //completely random, TODO: cercare di basarsi probalisticamente
//     auto p1 = this->path;
//     auto p2 = parent2.path;
//     //TODO : lista di dim`ensione divers???
//     for (int i=0; i<=p1.size(); i++){
//         p1[i]
//     }
// }

void PathRappr::substringReversal(int minAlt = 5) {
    int delta = 0;
    int start = 0;
    int stop =0;
    while (delta<minAlt){
        int k1 = rand()%path.size();
        int k2 = rand()%path.size();
        start = (k1 <= k2)? k1 : k2;
        stop = (k1 >= k2)? k1 : k2;
        delta = stop-start;
    }
    std::cout<<"start: "<<start<<" , stop: "<<stop<<std::endl;
    for (int i=0; i<=(stop-start)/2; i++){
        int tmp = path[stop-i];
        path[stop-i] = path[start+i];
        path[start+i] = tmp;
    }
}

//FITNESS OPERATOR///
// CostFitness::CostFitness(Panel& p) : panel(p) {}

// double CostFitness::fitness(Solution& solution){
//     return solution.evaluate_cost(panel);
// }