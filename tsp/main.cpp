#include <cstdio>
#include <iostream>
#include <vector>
#include <chrono>
#include "../cpxmacro.h"

using namespace std::chrono;

//data initialization
const int Nh = 4; //number of holes (nodes of the graph)
double Costs[Nh*Nh] = { //ridondante...
//  0 1  2  3
    0,10,15,30, //0
    10,0,35,25, //1
    15,35,0,30, //2
    20,25,30,0  //3
};
int status;
char errmsg[BUF_SIZE];

const int startH = 0; //nodo partenza 


int idX(int i, int j){
    return i*Nh+j;
}
int idY(int i, int j){
    return (Nh*Nh) + i*Nh+j   ;
}

void setupLP(CEnv env, Prob lp){
    std::vector<char> var_types = std::vector<char>();
    std::vector<double> var_lbs = std::vector<double>();
    std::vector<double> var_ubs = std::vector<double>();
    std::vector<char*> var_names = std::vector<char*>(); 
    std::vector<double> obj_fun_coefs = std::vector<double>();       

    for(int i =0; i<Nh*Nh; i++){ //var x
        var_types.push_back('C');
        var_lbs.push_back(0.0);
        var_ubs.push_back(CPX_INFBOUND);
        char var_name[10];
        snprintf(var_name,10,"x_%d_%d",i/Nh,i%Nh);
        var_names.push_back(var_name);
        obj_fun_coefs.push_back(0.0);
    }
    
    for(int i =0; i<Nh*Nh; i++){ //var y
        var_types.push_back('B');
        var_lbs.push_back(0.0);
        var_ubs.push_back(1.0);
        char var_name[10];
        snprintf(var_name,10,"y_%d_%d",i/Nh,i%Nh);
        var_names.push_back(var_name);
        obj_fun_coefs.push_back(Costs[i]);
    }
    CHECKED_CPX_CALL( CPXnewcols, env, lp, 2*Nh*Nh, &obj_fun_coefs[0], &var_lbs[0], &var_ubs[0], &var_types[0], &var_names[0]);
    
    //CONSTRAINTS
    
    std::vector<int> idx = std::vector<int>();
    std::vector<double> coef = std::vector<double>();
    std::vector<int> matbegin = std::vector<int>();
    std::vector<double> constValues = std::vector<double>();
    std::vector<char> senses = std::vector<char>();
    

    int matbegin2use = 0;

    for (int k = 0; k < Nh; k++) //zero nodo partente
	{
        if (k!=startH) {
            matbegin.push_back(matbegin2use);
            //somma archi entranti - somma archi uscenti da k
            for(int i=0; i<Nh; i++){ //archi entranti
                if (k!=i) {
                    idx.push_back(idX(i,k));
                    coef.push_back(1);
                    matbegin2use++;
                }
            }
            for(int j=0; j<Nh; j++){ //archi uscenti
                if (j!=0 || k!=j) {
                    idx.push_back(idX(k,j));
                    coef.push_back(-1);
                    matbegin2use++;
                }
            }         
    		senses.push_back('E');
            constValues.push_back(1.0);
        }
	}

    for (int i=0; i<Nh; i++){
        matbegin.push_back(matbegin2use);

        for (int j=0; j<Nh; j++){ //somma nodi uscenti
            if (j!=i) {
                idx.push_back(idY(i,j));
                coef.push_back(1);
                matbegin2use++;
            }
        }
        senses.push_back('E');
        constValues.push_back(1);
    }

    for (int j=0; j<Nh; j++){   
        matbegin.push_back(matbegin2use);

        for (int i=0; i<Nh; i++){//somma nodi entranti
            if (j!=i){
                idx.push_back(idY(i,j));
                coef.push_back(1);
                matbegin2use++;
            }
        }
        senses.push_back('E');
        constValues.push_back(1);
    }

    for (int i=0; i<Nh; i++){
        for(int j=0; j<Nh; j++){
            if (j!=0){
                matbegin.push_back(matbegin2use);
                idx.push_back(idX(i,j));
                idx.push_back(idY(i,j));
                coef.push_back(1);
                coef.push_back(-Nh+1);
                senses.push_back('L');
                constValues.push_back(0);
                matbegin2use+=2;
            }
        }
    }
    
	CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, constValues.size(), idx.size(), &constValues[0], &senses[0], &matbegin[0], &idx[0], &coef[0], nullptr, nullptr );
}   

int main(){
    try
	{
		// init
		DECL_ENV(env);
		DECL_PROB( env, lp );
		// setup LP
        auto start_construction = high_resolution_clock::now(); 
		setupLP(env, lp);
        auto stop_construction = high_resolution_clock::now(); 
        auto duration_construction = duration_cast<microseconds>(stop_construction - start_construction); 
        //write the problem in a file
        CHECKED_CPX_CALL( CPXwriteprob, env, lp, "lp_file/tsp.lp", NULL );
		// optimize
        auto start_optimize = high_resolution_clock::now(); 
		CHECKED_CPX_CALL( CPXmipopt, env, lp );
        auto stop_optimize = high_resolution_clock::now(); 
        auto duration_optimize = duration_cast<microseconds>(stop_optimize - start_optimize); 
		// print
		double objval;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
        std::cout<<  "problem setting time : "<<duration_construction.count()<<"ms"<<std::endl;
        std::cout<<  "problem optimizing time : "<<duration_optimize.count()<<"ms"<<std::endl;
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
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}