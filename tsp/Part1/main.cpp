#include <cstdio>
#include <iostream>
#include <vector>
#include <chrono>
#include "../DataGenerator.h"
#include "../matplotlib-cpp/matplotlibcpp.h"
#include "model1.cpp"
#include "cpxmacro.h"


namespace plt = matplotlibcpp;
/*
//data initialization
const int Nh = 4; //number of holes (nodes of the graph)
double Costs[Nh*Nh] = { //ridondante...
//  0 1  2  3
    0,10,15,30, //0
    10,0,35,25, //1
    15,35,0,30, //2
    20,25,30,0  //3
};
const int startH = 0; //nodo partenza 
*/


int status;
char errmsg[BUF_SIZE];

int performExperimentEM(std::vector<Panel*> instances,bool plot = false,double timeLimit=-1){
    try
	{
        auto timeSetting = std::vector<double>();
        auto timeSolving = std::vector<double>();
        auto index = std::vector<double>();

        for (long unsigned int in=0; in<instances.size(); in++ ){
            index.push_back(in);
            // init
            DECL_ENV(env);
            DECL_PROB( env, lp);
            if (timeLimit>0) CHECKED_CPX_CALL(CPXsetdblparam,env,1039,timeLimit);
            //problem info
            std::cout<<"-------------------------------------------"<<std::endl;
            std::cout<<"PROBLEM: "<<in<<std::endl;
            int Nh = (instances[in])->get_holesN();
            std::cout<<"holes number : "<<Nh<<std::endl;

            // setup LP
            auto start_construction = high_resolution_clock::now(); 
            int Y[Nh*Nh];
            TSPModel::setupLP(env, lp,instances[in],Y,true);
            auto stop_construction = high_resolution_clock::now(); 
            auto duration_construction = duration_cast<microseconds>(stop_construction - start_construction);
            timeSetting.push_back(duration_construction.count()); 
            //write the problem in a file
            CHECKED_CPX_CALL( CPXwriteprob, env, lp, "lp_file/tsp.lp", NULL );
            // optimize
            auto start_optimize = high_resolution_clock::now(); 
            CHECKED_CPX_CALL( CPXmipopt, env, lp );
            auto stop_optimize = high_resolution_clock::now(); 
            auto duration_optimize = duration_cast<microseconds>(stop_optimize - start_optimize); 
            timeSolving.push_back(duration_optimize.count());
            // print
            double objval;
            CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
            std::cout<<  "problem setting time : "<<duration_construction.count()<<" micro_sec"<<std::endl;
            std::cout<<  "problem optimizing time : "<<duration_optimize.count()<<" micro_sec"<<std::endl;
            std::cout << "Objval: " << objval << std::endl;
            int n = CPXgetnumcols(env, lp);
            
            std::vector<double> varVals;
            varVals.resize(n);
            CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
            if (plot){
                std::cout<<"plotting solution panel"<<std::endl;
                instances[in]->plotSol(varVals,Y);
            }
            
            //write the solution
            CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );
            // free
            CPXfreeprob(env, &lp);
            CPXcloseCPLEX(&env);
        }
        //plotting 
        plt::title("tsp problem f1 \nGB=1000x1000\nX:holesN, Y:time in micro_sec");
        plt::named_plot("setting time",index,timeSetting,"green");
        plt::named_plot("solving time", index,timeSolving,"red");
        plt::legend();
        plt::show();
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}





int main(){
    try {
    int exp =5;
    auto instances = std::vector<BoardPanel> ();
    auto p = std::vector<Panel*> ();
    switch (exp){
        case 1 : 
            {
            std::cout<<"esecuzione grid1"<<std::endl;
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,5));
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,30));
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,100));
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,120));
            }
        break;
        case 0 :
            {
            std::cout<<"esecuzione grid0"<<std::endl;
            instances.push_back(BoardPanel::create_gridPanel(1000,1000,5));
            instances.push_back(BoardPanel::create_gridPanel(1000,1000,30));
            instances.push_back(BoardPanel::create_gridPanel(1000,1000,100));
            instances.push_back(BoardPanel::create_gridPanel(1000,1000,120));
            // instances.push_back(BoardPanel::create_gridPanel(1000,1000,50));
            // instances.push_back(BoardPanel::create_gridPanel(1000,1000,60));
            // instances.push_back(BoardPanel::create_gridPanel(1000,1000,70));
            }
        break;
        case 2:
            {
            std::cout<<"esecuzione weird"<<std::endl;
            std::cout<<"esecuzione grid1"<<std::endl;
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,5));
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,30));
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,100));
            instances.push_back(BoardPanel::create_gridPanel1(1000,1000,120));
            }
        break;
        case 3:
            {
            cout<<"creazione istanze"<<endl;
            cout<<"creazione weird"<<endl;
            auto a1 = BoardPanel::create_weirdPanel(20,30,20);
            cout<<"creazione grid"<<endl;
            auto a2 = BoardPanel::create_gridPanel(20,30,20);
            cout<<"creazione grid1"<<endl;
            auto a3 = BoardPanel::create_gridPanel1(20,30,20);
            
            a1.plot(true);
            a2.plot(true);
            a3.plot(true);
            }
        break;
        case 4:
            {   
                auto low = BoardPanel::read("Data/grid1_30.dat");
                auto med = BoardPanel::read("Data/grid1_60.dat");
                auto large = BoardPanel::read("Data/grid1_100.dat");


                // auto low = BoardPanel::create_gridPanel1(100,100,30);
                // auto med = BoardPanel::create_gridPanel1(100,100,60);
                // auto large = BoardPanel::create_gridPanel1(100,100,100);
                // low.write("Data/grid1_30.dat");
                // med.write("Data/grid1_60.dat");
                // large.write("Data/grid1_100.dat");
                // low.writePaolo("Data/grid1_30P.dat");
                // med.writePaolo("Data/grid1_60P.dat");
                // large.writePaolo("Data/grid1_100P.dat");

                // low.plot(true);
                // med.plot(true);
                // large.plot(true);
                instances.push_back(low);
                instances.push_back(med);
                instances.push_back(large);
            }
        break;
        case 5:
            {
                // auto ultralarge = BoardPanel::create_gridPanel1(1000,1000,200);
                // ultralarge.write("Data/grid1_200.dat");
                auto ultralarge = BoardPanel::read("Data/grid1_200.dat");
                ultralarge.plot(true);
                instances.push_back(ultralarge);
                
            }
        break;
    }
    for (auto i = instances.begin(); i!=instances.end(); i++){
        p.push_back(&(*i));
    }

    performExperimentEM(p,true,-1);
    } 
    catch (std::string e){
        std::cout<<e<<std::endl;
    }
    return 0;
}