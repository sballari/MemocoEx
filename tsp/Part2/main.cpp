#include "CostrSol.h"
#include "../DataGenerator.h"
#include <iostream>
#include <vector> 
#include "../DataGenerator.h"
#include "GenAlg.h"
#include "../Solution.h"
#include <string>
#include "GenOp.h"
#include <chrono>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using namespace std::chrono;

void performExp(vector<Panel*> panels, vector<double> optVal,vector<double> optTime, GeneticAlgorithm alg,bool verbose = false,bool plot = false){
    for (int i = 0; i<panels.size(); i++){
        cout<<"----------------------------------------------------------"<<endl;
        cout<<"PANNELLO : GRID1 SIZE "<<(panels[i])->get_holesN()<<endl;
        alg.changePanel(panels[i]);
        auto start = high_resolution_clock::now();
        auto genetic_sol = alg.run(plot,verbose);
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start);
        cout<<"time : "<<duration.count()<<endl;
        cout<<"sol cost : "<<genetic_sol->fitness()<<endl;
        cout<<"opt sol : "<<optVal[i]<<endl;
        cout<<"sol_ott/sol %: "<< optVal[i]/(genetic_sol->fitness())*100<<endl;
        cout<<"tempo/tempo_ott(zc) %"<< duration.count()/optTime[i]<<endl;
        cout<<"correttezza : "<<genetic_sol->checkCorrectness()<<endl;
        cout<<"--------------------------------------------________________"<<endl;
        //genetic_sol->plot();
    }
}


int main(){
    

    try {
        int exp = 1;
        switch(exp){
            case 1 :
            {
            // auto low = BoardPanel::read("../Part1/Data/grid1_30.dat");
            // auto med = BoardPanel::read("../Part1/Data/grid1_60.dat");
            // auto large = BoardPanel::read("../Part1/Data/grid1_100.dat");
            auto ultralarge = BoardPanel::read("../Part1/Data/grid1_200.dat");
            vector<Panel*> panels = {&ultralarge};//{&low,&med,&large,&ultralarge};
            vector<double> optVal= {417.464,539.265,604.344,9503.95};
            vector<double> optTime= {752030,11743409,202298252,3972817779};

            int population = 100;
            int Alt = 5;
            int RepN = 5;
            double  imprLimit = 0.01;
            int maxAttempt = 5;
            auto pg = RandomInsertionGenerator();
            auto so = MonteCarloSelection();
            auto go = OrderCrossOver(Alt); //min alt non usato [ricordarsi]
            auto ro = SteadyStateReplacement(RepN);
            auto sc = NotImprovingCriteria(imprLimit,maxAttempt);
            auto alg = GeneticAlgorithm(nullptr,pg,population,sc,so,go,ro);

            cout<<"Iperparametri GeneticAlgorithm -----------"<<endl;
            cout<<"popGenerator : RandomInsertionGenerator, PopN : "<<population<<endl;
            cout<<"Selection : MonteCarlo"<<endl;
            cout<<"Alt OrderCrossOver : "<<Alt<<endl;
            cout<<"steady state replacement N : "<<RepN<<endl;
            cout<<"Stop Criteria : NotImprCriteria, minImpr "<<imprLimit<<" maxAttempt : "<<maxAttempt<<endl;
            cout<<"--------------------------------------------"<<endl;
            performExp(panels,optVal,optTime,alg,true ,true);
            break;
            }
            case 2:
            {
                for ( int k=0; k<2; k++){
                    auto panel = BoardPanel::create_gridPanel1(100,100,10);
                    auto sol1 = RandomInsertion::get_sol(panel); 
                    auto sol2 = RandomInsertion::get_sol(panel);
                    cout<<"gen1 "; sol1->printSol();
                    cout<<"gen2 ";sol2->printSol(); 
                    auto off = PathRappr::orderCrossover(sol1,sol2,7);   
                    (off[0])->printSol();
                    (off[1])->printSol();
                    cout<<"----------------------------------"<<endl;
                }
            }
            break;
        }

    }
    catch(string m){
        cout<<m<<endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception caught : " << e.what() << std::endl;
    }
    catch (std::string e)
    {
        std::cerr << "Exception caught : " << e << std::endl;
    }
 
}