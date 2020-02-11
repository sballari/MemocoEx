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

int performExperimentEM(std::vector<Panel*> instances){
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
            //problem info
            std::cout<<"-------------------------------------------"<<std::endl;
            std::cout<<"PROBLEM: "<<in<<std::endl;
            std::cout<<"holes number : "<<(instances[in])->get_holesN()<<std::endl;

            // setup LP
            auto start_construction = high_resolution_clock::now(); 
            TSPModel::setupLP(env, lp,instances[in]);
            auto stop_construction = high_resolution_clock::now(); 
            auto duration_construction = duration_cast<microseconds>(stop_construction - start_construction);
            timeSetting.push_back(duration_construction.count()); 
            //write the problem in a file
            // CHECKED_CPX_CALL( CPXwriteprob, env, lp, "lp_file/tsp.lp", NULL );
            // optimize
            auto start_optimize = high_resolution_clock::now(); 
            CHECKED_CPX_CALL( CPXmipopt, env, lp );
            auto stop_optimize = high_resolution_clock::now(); 
            auto duration_optimize = duration_cast<microseconds>(stop_optimize - start_optimize); 
            timeSolving.push_back(duration_optimize.count());
            // print
            double objval;
            CHECKED_CPX_CALL( CPXgetobjval, env, 
            lp, &objval );
            std::cout<<  "problem setting time : "<<duration_construction.count()<<" micro_sec"<<std::endl;
            std::cout<<  "problem optimizing time : "<<duration_optimize.count()<<" micro_sec"<<std::endl;
            std::cout << "Objval: " << objval << std::endl;
            int n = CPXgetnumcols(env, lp);
            
            std::vector<double> varVals;
            varVals.resize(n);
            CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
            std::cout<<"plotting solution panel"<<std::endl;
            instances[in]->plotSol(varVals);
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
    int exp =0;
    auto instances = std::vector<BoardPanel> ();
    auto p = std::vector<Panel*> ();
    switch (exp){
        case 0 : 
            std::cout<<"esecuzione grid1"<<std::endl;
            instances.push_back(BoardPanel::create_gridPanel1(50,70,5));
            instances.push_back(BoardPanel::create_gridPanel1(50,70,10));
            instances.push_back(BoardPanel::create_gridPanel1(50,70,20));
            instances.push_back(BoardPanel::create_gridPanel1(50,70,30));
            instances.push_back(BoardPanel::create_gridPanel1(45,35,30));
        break;
        case 1 :
            std::cout<<"esecuzione grid"<<std::endl;
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,5));
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,10));
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,20));
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,30));
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,40));
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,50));
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,60));
            instances.push_back(BoardPanel::create_gridPanel(10000,10000,70));
        break;
        case 2:
            std::cout<<"esecuzione weird"<<std::endl;
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,5));
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,10));
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,20));
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,30));
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,40));
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,50));
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,60));
            instances.push_back(BoardPanel::create_weirdPanel(1000,1000,70));
        break;
    }
    for (auto i = instances.begin(); i!=instances.end(); i++){
        p.push_back(&(*i));
    }

    performExperimentEM(p);
    } 
    catch (std::string e){
        std::cout<<e<<std::endl;
    }
    return 0;
}


int main1(){
    try
	{
            auto a = BoardPanel::create_gridPanel1(100,70,15);
            DECL_ENV(env);
            DECL_PROB( env, lp);
            // setup LP
            TSPModel::setupLP(env, lp,&a);
            //write the problem in a file
            CHECKED_CPX_CALL( CPXwriteprob, env, lp, "lp_file/tsp.lp", NULL );
            // optimize
            CHECKED_CPX_CALL( CPXmipopt, env, lp );
            // print
            double objval;
            CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
            int n = CPXgetnumcols(env, lp);
            
            std::vector<double> varVals;
            varVals.resize(n);
            CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
            std::cout<<"plotting solution panel"<<std::endl;
            a.plotSol(varVals);
            //write the solution
            CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );
            // free
            CPXfreeprob(env, &lp);
            CPXcloseCPLEX(&env);
            
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
    catch(std::string& e)
	{
		std::cout << ">>>EXCEPTION: " << e << std::endl;
	}
	return 0;
}