#include <cstdio>
#include <iostream>
#include <vector>
#include <chrono>
#include "../cpxmacro.h"
#include "DataGenerator.h"
#include "matplotlib-cpp/matplotlibcpp.h"


using namespace std::chrono;
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


int idX(int i, int j,int Nh){
    return i*Nh+j;
}

void setupLP(CEnv env, Prob lp, BoardPanel panel, std::vector<int> subPanelIdX){
    int Nh = panel.get_holesN();


    std::vector<char> var_types = std::vector<char>();
    std::vector<double> var_lbs = std::vector<double>();
    std::vector<double> var_ubs = std::vector<double>();
    std::vector<char*> var_names = std::vector<char*>(); 
    std::vector<double> obj_fun_coefs = std::vector<double>();       

    // Dantzig, Fulkerson, Johnson, Oper. Res. 1954)

    for(int i =0; i<Nh*Nh; i++){ //var x
        var_types.push_back('B');
        var_lbs.push_back(0.0);
        var_ubs.push_back(1.0);
        char var_name[10];
        snprintf(var_name,10,"x_%d_%d",i/Nh,i%Nh);
        var_names.push_back(var_name);
        obj_fun_coefs.push_back(panel.get_euc_dist(i/Nh,i%Nh));
    }
    CHECKED_CPX_CALL( CPXnewcols, env, lp, Nh*Nh, &obj_fun_coefs[0], &var_lbs[0], &var_ubs[0], &var_types[0], &var_names[0]);

    
    //CONSTRAINTS
    
    std::vector<int> idx = std::vector<int>();
    std::vector<double> coef = std::vector<double>();
    std::vector<int> matbegin = std::vector<int>();
    std::vector<double> constValues = std::vector<double>();
    std::vector<char> senses = std::vector<char>();
    

    int matbegin2use = 0;

    for (int i = 0; i < Nh; i++) //outdegree constraints
	{
        for (int j=0; j<Nh; j++){
            idx.push_back(idX(i,j,Nh));
            coef.push_back(1.0);
        }
        matbegin.push_back(matbegin2use);
        matbegin2use++;
        senses.push_back('E');
        constValues.push_back(1.0);
	}

    for (int i = 0; i < Nh; i++) //indegree constraints
	{
        for (int j=0; j<Nh; j++){
            idx.push_back(idX(j,i,Nh));
            coef.push_back(1.0);
        }
        matbegin.push_back(matbegin2use);
        matbegin2use++;
        senses.push_back('E');
        constValues.push_back(1.0);
	}
    

    idx.insert(idx.end(), subPanelIdX.begin(), subPanelIdX.end());
    for (int i=0; i<subPanelIdX.size(); i++){
        for (int j=0; j<subPanelIdX.size(); j++){           
                coef.push_back(1);
        }
    }
    senses.push_back('L');
    constValues.push_back(subPanelIdX.size()-1);
    matbegin.push_back(matbegin2use);
    matbegin2use++;

	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, constValues.size(), idx.size(), &constValues[0], &senses[0], &matbegin[0], &idx[0], &coef[0], nullptr, nullptr );
}   



int main1(){
    try
	{
        std::vector<BoardPanel> instances;
        instances.push_back(BoardPanel::create_gridPanel(1000,1000,5));
        instances.push_back(BoardPanel::create_gridPanel(1000,1000,10));
        instances.push_back(BoardPanel::create_gridPanel(1000,1000,20));
        instances.push_back(BoardPanel::create_gridPanel(1000,1000,30));

        auto timeSetting = std::vector<double>();
        auto timeSolving = std::vector<double>();
        auto sizeS = 5;

        for (long unsigned int in=0; in<instances.size(); in++ ){
            // init
            DECL_ENV(env);
            DECL_PROB( env, lp);
            //problem info
            std::cout<<"-------------------------------------------"<<std::endl;
            std::cout<<"[DFJ]PROBLEM: "<<in<<std::endl;
            std::cout<<"holes number : "<<instances[in].get_holesN()<<std::endl;
            std::cout<<"subtour(random) size : "<<sizeS<<std::endl;
            

            // setup LP
            auto start_construction = high_resolution_clock::now(); 
            setupLP(env, lp,instances[in],instances[in].randomSubPanelIndex(sizeS));
            auto stop_construction = high_resolution_clock::now(); 
            auto duration_construction = duration_cast<microseconds>(stop_construction - start_construction);
            timeSetting.push_back(duration_construction.count()); 
            //write the problem in a file
            CHECKED_CPX_CALL( CPXwriteprob, env, lp, "lp_file/DFJ_tsp.lp", NULL );
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
            //write the solution
            CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );
            // free
            CPXfreeprob(env, &lp);
            CPXcloseCPLEX(&env);
        }
        //plotting 
        plt::title("tsp problem f1 \nGB=1000x1000\nX:holesN, Y:time in micro_sec");
        plt::named_plot("setting time",{0,1,2,3},timeSetting,"green");
        plt::named_plot("solving time", {0,1,2,3},timeSolving,"red");
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
    return main1();
}