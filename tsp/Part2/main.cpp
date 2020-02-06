#include "CostrSol.h"
#include "../DataGenerator.h"
#include <iostream>
#include <vector> 
#include <cstdlib>
#include <limits>
#include <utility>
#include "../DataGenerator.h"
#include "GenAlg.h"
#include "../Solution.h"
#include <string>
#include "GenOp.h"
using std::cout;
using std::endl;
using std::vector;
using std::string;

int main(){

    try {
        auto panel = BoardPanel::create_gridPanel1(45,35,30);
        auto pg = RandomInsertionGenerator();
        auto so = MonteCarloSelection();
        auto go = SubStringRevelsal(10);
        auto ro = SteadyStateReplacement(5);
        auto sc = NotImprovingCriteria();
        auto alg = GeneticAlgorithm(panel,pg,100,sc,so,go,ro);
        auto genetic_sol = alg.run();
        genetic_sol->plot();
    }
    catch(string m){
        cout<<m<<endl;
    }
 
}