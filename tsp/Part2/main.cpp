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
#include <math.h>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using namespace std::chrono;

void performExp(vector<Panel*> panels, vector<double> optVal,vector<double> optTime, GeneticAlgorithm alg,bool verbose = false,bool plot = false){
    for (uint i = 0; i<panels.size(); i++){
        cout<<"\033[0;31m----------------------------------------------------------\033[0m"<<endl;
        cout<<"PANNELLO : GRID1 SIZE "<<(panels[i])->get_holesN()<<endl;
        alg.changePanel(panels[i]);
        auto start = high_resolution_clock::now();
        auto genetic_sol = alg.run(plot,verbose,optVal[i]);
        auto stop = high_resolution_clock::now(); 
        auto duration = duration_cast<microseconds>(stop - start);
        cout<<"time : "<<duration.count()<<endl;
        cout<<"sol cost : "<<genetic_sol->fitness()<<endl;
        cout<<"opt sol : "<<optVal[i]<<endl;
        cout<<"sol_ott/sol %: "<< optVal[i]/(genetic_sol->fitness())*100<<endl;
        cout<<"tempo/tempo_ott(zc) %"<< duration.count()/optTime[i]<<endl;
        cout<<"correttezza : "<<genetic_sol->checkCorrectness()<<endl;
        cout<<"----------------------------------------------------------"<<endl;
        //genetic_sol->plot();
    }
}


void ModelSelection(){
    std::vector<Panel*> panels = {};
    for (uint i = 0; i<10; i++){
        auto p = BoardPanel::create_gridPanel1(1000,1000,200);
        panels.push_back(p);
    }
    std::vector<int> Pop = {100,150,1000,2000,5000};
    std::vector<double> RepNpc = {0.1,0.2,0.5};
    std::vector<double> impr ={1,5,10};
    std::vector<int> maxAttemp = {5,100,500};

    double bestCost = std::numeric_limits<double>::max();
    int bestPopN = 0, bestRepN=0, bestMi=0, bestMa=0;
    int bestTime =0, bestIt =0;

    for (auto p = Pop.begin(); p!=Pop.end(); p++){
        for (auto rep = RepNpc.begin(); rep!=RepNpc.end(); rep++){
            for (auto mI = impr.begin(); mI!=impr.end(); mI++){
                for (auto mA = maxAttemp.begin(); mA!=maxAttemp.end(); mA++){
                    int min = 0, max = 198;
                    int repN = (*rep)*(*p);
                    auto pg = RandomInsertionGenerator();
                    auto so = MonteCarloSelection(); //TODO NON E' PIU' MONTECARLO 
                    auto go = OrderCrossOver(min,max); //USO DELLA NUMERAZIONE DEI NODI [ricordarsi]
                    auto ro = SteadyStateReplacement(repN);
                    auto sc = NotImprovingCriteria(*mI,*mA);
                    auto alg = GeneticAlgorithm(nullptr,pg,*p,sc,so,go,ro);
                    cout<<"\033[0;31mIperparametri GeneticAlgorithm ---------------------------\033[0m"<<endl;
                    cout<<"popGenerator : RandomInsertionGenerator, PopN : "<<*p<<endl;
                    cout<<"Selection : MonteCarlo"<<endl;
                    cout<<"Alt OrderCrossOver : "<<min<<", "<<max<<endl;
                    cout<<"steady state replacement N : "<<repN<<endl;
                    cout<<"Stop Criteria : NotImprCriteria, minImpr "<<*mI<<" maxAttempt : "<<*mA<<endl;
                    double sumTime = 0;
                    double sumCost = 0;
                    double sumIt = 0;
                    vector<double> times = {};
                    vector<double> costs = {};               
                    vector<double> its = {};

                    for (auto panel = panels.begin(); panel!=panels.end(); panel++)
                        {    
                            // cout<<"----------------------------------"<<endl;                      
                            alg.changePanel(*panel);
                            auto start = high_resolution_clock::now();
                            auto genetic_sol = alg.run(false,false,0);
                            auto stop = high_resolution_clock::now(); 
                            auto duration = duration_cast<microseconds>(stop - start);
                            cout<<"time : "<<duration.count()<<", ";
                            cout<<"sol cost : "<<genetic_sol->fitness()<<", ";
                            cout<<"iterazioni : "<<alg.iterazioniLastRun<<endl;
                            double time = duration.count();
                            sumTime += time;
                            times.push_back(time);
                            double fit = genetic_sol->fitness();
                            sumCost += fit;
                            costs.push_back(fit);
                            sumIt += alg.iterazioniLastRun;
                            its.push_back(alg.iterazioniLastRun);

                        }
                        double avgTime = sumTime/panels.size();
                        double avgCost = sumCost/panels.size();
                        double avgIt = sumIt/panels.size();
                        double sqmTime, sqmCost, sqmIt;
                        for (uint i=0; i<panels.size(); i++){
                            sqmTime += (times[i]-avgTime)*(times[i]-avgTime);
                            sqmCost += (costs[i]-avgCost)*(costs[i]-avgCost);
                            sqmIt += (its[i]-avgIt)*(its[i]-avgIt);
                        }
                        sqmCost = sqrt(sqmCost/panels.size());
                        sqmIt = sqrt(sqmIt/panels.size());
                        sqmTime = sqrt(sqmTime/panels.size());
                        cout<<"avgTime = "<<avgTime<<"\tsqmTime = "<<sqmTime<<endl;
                        cout<<"avgCost = "<<avgCost<<"\tsqmCost = "<<sqmCost<<endl;
                        cout<<"avgIt = "<< avgIt<<"\tsqmIt = "<< sqmIt<<endl;

                        if (avgCost < bestCost) {
                            bestCost = avgCost;
                            bestTime = avgTime;
                            bestIt = alg.iterazioniLastRun;
                            bestPopN = *p, bestRepN=repN, bestMi = *mI, bestMa = *mA;
                        }
                        cout<<"\033[0;31m----------------------------------------------------------\033[0m"<<endl;
                }
            }
        }
    }
    cout<<"best configuration : \n"<<"bestCost = "<<bestCost<<"iterazione = "<<bestIt<<" , bestTime = "<<bestTime<<" ,popN = "<<bestPopN<<", repN = "<<bestRepN<<", bestMi = "<<bestMi<<", bestMa "<<bestMa<<endl;
    for (auto p = panels.begin(); p!= panels.end(); p++){
        delete *p;
    }
}



int main(){
    

    try {
        int exp = 2;
        switch(exp){
            case 1 :
            {
            // auto low = BoardPanel::read("../Part1/Data/grid1_30.dat");
            // auto med = BoardPanel::read("../Part1/Data/grid1_60.dat");
            // auto large = BoardPanel::read("../Part1/Data/grid1_100.dat");
            auto ultralarge = BoardPanel::read("../Part1/Data/grid1_200.dat");
            // auto insanePanel = BoardPanel::create_gridPanel1(1000,1000,1000); cout<<"creato insane panel\a"<<endl;
            vector<Panel*> panels = {&ultralarge};//{&low,&med,&large,&ultralarge};
            vector<double> optVal= {9503.95};//{417.464,539.265,604.344,9503.95};
            vector<double> optTime= {3972817779};// {752030,11743409,202298252,3972817779};

            //ALGORTITHM SETTING
            int population = 1000;
            int min = 5; int max = 20;
            int RepN = 50;
            double  imprLimit = 1;
            int maxAttempt = 1000;
            auto pg = RandomInsertionGenerator();
            auto so = MonteCarloSelection(); //TODO NON E' PIU' MONTECARLO 
            auto go = OrderCrossOver(min,max); //USO DELLA NUMERAZIONE DEI NODI [ricordarsi]
            auto ro = SteadyStateReplacement(RepN);
            auto sc = NotImprovingCriteria(imprLimit,maxAttempt);
            auto alg = GeneticAlgorithm(nullptr,pg,population,sc,so,go,ro);

            cout<<"\033[0;31mIperparametri GeneticAlgorithm ---------------------------\033[0m"<<endl;
            cout<<"popGenerator : RandomInsertionGenerator, PopN : "<<population<<endl;
            cout<<"Selection : MonteCarlo"<<endl;
            cout<<"Alt OrderCrossOver : "<<min<<", "<<max<<endl;
            cout<<"steady state replacement N : "<<RepN<<endl;
            cout<<"Stop Criteria : NotImprCriteria, minImpr "<<imprLimit<<" maxAttempt : "<<maxAttempt<<endl;
            cout<<"\033[0;31m----------------------------------------------------------\033[0m"<<endl;
            performExp(panels,optVal,optTime,alg,true ,true);
            for (auto i = panels.begin(); i != panels.end(); i++){
                delete (*i);
            }
            break;
            }
            case 2:
            {
                
                ModelSelection();
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