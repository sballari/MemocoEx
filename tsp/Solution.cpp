#include <cstdlib>
#include <vector> 
#include "Solution.h"
#include "matplotlib-cpp/matplotlibcpp.h"
namespace plt = matplotlibcpp;

PathRappr::PathRappr(std::vector<int> p){
    path=p;
};
PathRappr::PathRappr(){
    path = std::vector<int>();
};
void PathRappr::addCity(int city){
    path.push_back(city);
};

 void PathRappr::plot(Panel& panel) {
    panel.plot(false);
    auto Xs = std::vector<double>();
    auto Ys = std::vector<double>();

    for (auto j = path.begin(); j!=path.end()--; j++){
        std::cout<<"buco "<<*j<<" "<<panel.getPoint(*j)[0]<<", "<<panel.getPoint(*j)[1]<<std::endl;
        Xs.push_back(panel.getPoint(*j)[0]);
        Ys.push_back(panel.getPoint(*j)[1]);
    }
    for (int i=0; i<Xs.size(); i++){
        std::cout<<Xs[i]<<" ";
    }
    std::cout<<std::endl;
    for (int i=0; i<Xs.size(); i++){
        std::cout<<Ys[i]<<" ";
    }
    plt::plot(Xs,Ys,"blue");
    plt::show();
 };

double PathRappr::evaluate_cost(Panel& panel){
    //funzione di scoring piu' banale possibile
    double cost = 0;
    for( auto i = path.begin()++; i!= path.end(); i++ ){
        auto j = i;
        j--;
        cost+=panel.get_euc_dist(*j,*i);
    }
    return cost;
}