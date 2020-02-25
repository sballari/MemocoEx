#include <cstdio>
#include <iostream>
#include <vector>
#include <chrono>
#include "../DataGenerator.h"
// #include "../matplotlib-cpp/matplotlibcpp.h"
#include "model1.cpp"
#include "cpxmacro.h"


// namespace plt = matplotlibcpp;

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
            // if (plot){
            //     std::cout<<"plotting solution panel"<<std::endl;
            //     instances[in]->plotSol(varVals,Y);
            // }
            
            //write the solution
            CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );
            // free
            CPXfreeprob(env, &lp);
            CPXcloseCPLEX(&env);
        }
        //plotting 
        // plt::title("tsp problem times");
        // plt::named_plot("setting time",index,timeSetting,"green");
        // plt::named_plot("solving time", index,timeSolving,"red");
        // plt::legend();
        // plt::show();
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}





int main(){
    try {
        auto instances = std::vector<Panel*> ();
        auto med = BoardPanel::read("../Data/grid1_60.dat");
        instances.push_back(&med);
        performExperimentEM(instances,true,-1);
    } 
    catch (std::string e){
        std::cout<<e<<std::endl;
    }
    return 0;
}