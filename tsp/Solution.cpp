#include <cstdlib>
#include <vector> 
#include "Solution.h"



PathRappr::PathRappr(std::vector<int> p){
    path=p;
};
PathRappr::PathRappr(){
    path = std::vector<int>();
};
void PathRappr::addCity(int city){
    path.push_back(city);
};
