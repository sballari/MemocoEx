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
    /*if (i<=j)*/ return -(i+1)+ i*Nh+j;
    // else return j*Nh+i;
}
int idY(int i, int j,int Nh){
    int offset = Nh*Nh-Nh ;//(Nh*Nh+Nh)/2;
    /*if (i<=j)*/ return offset + i*Nh+j;
    // else return offset +j*Nh+i;  ;
}

void setupLP(CEnv env, Prob lp, Pabnel* panel){
    int Nh = panel->get_holesN();
    int startH = 0;

    std::vector<char> var_types = std::vector<char>();
    std::vector<double> var_lbs = std::vector<double>();
    std::vector<double> var_ubs = std::vector<double>();
    std::vector<char*> var_names = std::vector<char*>(); 
    std::vector<double> obj_fun_coefs = std::vector<double>();       

    for(int i =0; i<Nh; i++){ //var x
        for (int j = 1; j<Nh; j++){
            // if (i!=j){
                var_types.push_back('C');
                var_lbs.push_back(0.0);
                var_ubs.push_back(CPX_INFBOUND);
                char* var_name = new char[10];
                snprintf(var_name,10,"x_%d_%d",i,j);
                var_names.push_back(var_name);
                obj_fun_coefs.push_back(0.0);
            // }
        }
    }
    
    for(int i =0; i<Nh; i++){ //var y
        for (int j=0; j<Nh; j++){
            // if (i!=j){
                var_types.push_back('B');
                var_lbs.push_back(0.0);
                var_ubs.push_back(1.0);
                char* var_name = new char[10];
                snprintf(var_name,10,"y_%d_%d",i,j);
                var_names.push_back(var_name);
                obj_fun_coefs.push_back(panel->get_euc_dist(i,j));
                // (i!=j)? obj_fun_coefs.push_back(panel->get_euc_dist(i,j)) : obj_fun_coefs.push_back(Nh);
            // }
        }      
    }
    CHECKED_CPX_CALL( CPXnewcols, env, lp, obj_fun_coefs.size(), &obj_fun_coefs[0], &var_lbs[0], &var_ubs[0], &var_types[0], &var_names[0]);
    
    //CONSTRAINTS
    
    std::vector<int> idx = std::vector<int>();
    std::vector<double> coef = std::vector<double>();
    std::vector<int> matbegin = std::vector<int>();
    std::vector<double> constValues = std::vector<double>();
    std::vector<char> senses = std::vector<char>();
    

    int matbegin2use = 0;
    for (int i =1; i<Nh; i++){
        matbegin.push_back(matbegin2use);
        idx.push_back(idY(i,i,Nh));
        coef.push_back(1);
        senses.push_back('E');
        constValues.push_back(0.0);
        matbegin2use++;
    }
    for (int k = 1; k < Nh; k++) //zero nodo partente
	{
        matbegin.push_back(matbegin2use);
        //somma archi entranti - somma archi uscenti da k
        for(int i=0; i<Nh; i++){ //archi entranti
            // if (k!=i) {
                idx.push_back(idX(i,k,Nh));
                coef.push_back(1);
                matbegin2use++;
            // }
        }
        for(int j=1; j<Nh; j++){ //archi uscenti
            if (k!=j) {
                idx.push_back(idX(k,j,Nh));
                coef.push_back(-1);
                matbegin2use++;
            }
        }         
        senses.push_back('E');
        constValues.push_back(1.0);
	}

    for (int i=0; i<Nh; i++){
        matbegin.push_back(matbegin2use);

        for (int j=0; j<Nh; j++){ //somma nodi uscenti
            // if (i!=j) {
                idx.push_back(idY(i,j,Nh));
                coef.push_back(1);
                matbegin2use++;
            // }
        }
        senses.push_back('E');
        constValues.push_back(1);
    }

    for (int j=0; j<Nh; j++){   
        matbegin.push_back(matbegin2use);

        for (int i=0; i<Nh; i++){//somma nodi entranti
            // if (i!=j){
                idx.push_back(idY(i,j,Nh));
                coef.push_back(1);
                matbegin2use++;
            // }
        }
        senses.push_back('E');
        constValues.push_back(1);
    }

    for (int i=0; i<Nh; i++){
        for(int j=1; j<Nh; j++){
            // if (i!=j){
                matbegin.push_back(matbegin2use);
                idx.push_back(idX(i,j,Nh));
                idx.push_back(idY(i,j,Nh));
                coef.push_back(1);
                coef.push_back(-Nh+1);
                senses.push_back('L');
                constValues.push_back(0);
                matbegin2use+=2;
            // }
        }
    }
    
	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, constValues.size(), idx.size(), &constValues[0], &senses[0], &matbegin[0], &idx[0], &coef[0], nullptr, nullptr );
}   



int main1(){
    try
	{
        std::vector<BoardPanel> instances;
        instances.push_back(BoardPanel::create_gridPanel1(1000,1000,5));
        instances.push_back(BoardPanel::create_gridPanel1(1000,1000,10));
        instances.push_back(BoardPanel::create_gridPanel1(1000,1000,20));
        instances.push_back(BoardPanel::create_gridPanel1(1000,1000,30));

        auto timeSetting = std::vector<double>();
        auto timeSolving = std::vector<double>();

        for (long unsigned int in=0; in<instances.size(); in++ ){
            // init
            DECL_ENV(env);
            DECL_PROB( env, lp);
            //problem info
            std::cout<<"-------------------------------------------"<<std::endl;
            std::cout<<"PROBLEM: "<<in<<std::endl;
            std::cout<<"holes number : "<<instances[in].get_holesN()<<std::endl;

            // setup LP
            auto start_construction = high_resolution_clock::now(); 
            setupLP(env, lp,&(instances[in]));
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
            std::cout<<"plotting solution panel"<<std::endl;
            instances[in].plotSol(1000,1000,varVals);
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


int exp2(){
    try
	{
        std::vector<BoardPanel> instances;
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,5));
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,10));
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,20));
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,30));
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,40));
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,50));
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,60));
        instances.push_back(BoardPanel::create_gridPanel(10000,10000,70));

        auto timeSetting = std::vector<double>();
        auto timeSolving = double>();

        for (long unsigned int in=0; in<instances.size(); in++ ){
            // init
            DECL_ENV(env);
            DECL_PROB( env, lp);
            //problem info
            std::cout<<"-------------------------------------------"<<std::endl;
            std::cout<<"PROBLEM: "<<in<<std::endl;
            std::cout<<"holes number : "<<instances[in].get_holesN()<<std::endl;

            // setup LP
            auto start_construction = high_resolution_clock::now(); 
            setupLP(env, lp,&instances[in]);
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
            //write the solution
            CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );
            // free
            CPXfreeprob(env, &lp);
            CPXcloseCPLEX(&env);
        }
        //plotting 
        plt::title("tsp problem f1 \nGB=1000x1000\nX:holesN, Y:time in micro_sec");
        plt::named_plot("setting time",{5,10,20,30,40,50,60,70},timeSetting,"green");
        plt::named_plot("solving time", {5,10,20,30,40,50,60,70},timeSolving,"red");
        plt::legend();
        plt::show();
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}





int main3(){
    try
	{
        std::vector<BoardPanel> instances;
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,5));
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,10));
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,20));
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,30));
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,40));
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,50));
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,60));
        instances.push_back(BoardPanel::create_weirdPanel(1000,1000,70));



        auto timeSetting = std::vector<double>();
        auto timeSolving = std::vector<double>();

        for (long unsigned int in=0; in<instances.size(); in++ ){
            // init
            DECL_ENV(env);
            DECL_PROB( env, lp);
            //problem info
            std::cout<<"-------------------------------------------"<<std::endl;
            std::cout<<"PROBLEM weirdPanel: "<<in<<std::endl;
            std::cout<<"holes number : "<<instances[in].get_holesN()<<std::endl;

            // setup LP
            auto start_construction = high_resolution_clock::now(); 
            setupLP(env, lp,&instances[in]);
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
            std::cout<<"plotting solution panel"<<std::endl;
            instances[in].plotSol(1000,1000,varVals);
            //write the solution
            CHECKED_CPX_CALL( CPXsolwrite, env, lp, "tsp.sol" );
            // free
            CPXfreeprob(env, &lp);
            CPXcloseCPLEX(&env);
        }
        //plotting 
        plt::title("tsp problem f1 \nGB=1000x1000\nX:holesN, Y:time in micro_sec");
        plt::named_plot("setting time",{5,10,20,30,40,50,60,70},timeSetting,"green");
        plt::named_plot("solving time", {5,10,20,30,40,50,60,70},timeSolving,"red");
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
    return main3();
}