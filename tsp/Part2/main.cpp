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
        vector<Solution*> off = vector<Solution*>();

        auto panel = BoardPanel::create_gridPanel1(45,35,30);
        for (int i =0; i<20; i++){
            sp.push_back(RandomInsertion::get_sol(panel));
        }   
        for (int i =0; i<20; i++){
            off.push_back(RandomInsertion::get_sol(panel));
        }  
        int n = 0;
        auto o = off.begin();
        for (auto i = sp.begin(); i!=sp.end(); i++){
            cout<<n<<":\t"<<*i<<" : "<<(*i)->fitness()<<"\t"<<(*o)<<" : "<<(*o)->fitness()<<endl;
            n++;
            o++;
        }
        auto ssr = SteadyStateReplacement(5);
        n=0;
        auto c = ssr.replacement(sp,off);
        for (auto i = c.begin(); i!=c.end(); i++){
            cout<<n<<":\t"<<*i<<"\t"<<(*i)->fitness()<<endl;
            n++;
        }
        return 0;
    }
    catch(string m){
        cout<<m<<endl;
    }
 
}