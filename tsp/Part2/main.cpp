#include "CostrSol.h"
#include "../DataGenerator.h"
#include <iostream>
#include <vector> 
#include <cstdlib>
#include <limits>
#include <utility>
#include "../DataGenerator.h"
#include "../Solution.h"
#include <string>
#include "GenOp.h"
using std::cout;
using std::endl;
using std::vector;
using std::string;

int main(){

    try {
        vector<Solution*> sp = vector<Solution*>();

        auto panel = BoardPanel::create_gridPanel1(45,35,30);
        for (int i =0; i<20; i++){
            sp.push_back(RandomInsertion::get_sol(panel));
        }   
        int n = 0;
        for (auto i = sp.begin(); i!=sp.end(); i++){
            cout<<n<<": "<<*i<<" : "<<(*i)->fitness()<<endl;
            n++;
        }
        auto ssr = SteadyStateReplacement(5);
        ssr.replacement(sp,sp);
        return 0;
    }
    catch(string m){
        cout<<m<<endl;
    }
 
}