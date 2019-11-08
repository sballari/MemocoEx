#include <cstdio>
#include <iostream>
#include <vector>
#include "../cpxmacro.h"

//data initialization
const int Nh = 3; //number of holes (nodes of the graph)
double Costs[Nh*Nh] = { //ridondante...
    0.0,0.5,0.3,
    0.5,0.0,0.3,
    0.3,0.5,0.0
};
int status;
char errmsg[BUF_SIZE];

const int startH = 0;


int idX(int i, int j){
    return i*Nh+j;
}
int idY(int i, int j){
    return (Nh*Nh) + i*Nh+j   ;
}

void setupLP(CEnv env, Prob lp){

    //variables: x_i_j::Binary
    std::vector<char> var_types(Nh*Nh);
    std::vector<double> var_lbs(Nh*Nh);
    std::vector<double> var_ubs(Nh*Nh);
    std::vector<char*> var_names(Nh*Nh); 
    std::vector<double> obj_fun_coefs(Nh*Nh);

    for(int i =0; i<Nh*Nh; i++){ //var x
        var_types.push_back('C');
        var_lbs.push_back(0.0);
        var_ubs.push_back(CPX_INFBOUND);
        char* var_name;
        snprintf(var_name,10,"x_%c_%c",i/Nh,i%Nh);
        var_names.push_back(var_name);
        obj_fun_coefs.push_back(0.0);
    }

    for(int i =0; i<Nh*Nh; i++){ 
        var_types.push_back('B');
        var_lbs.push_back(0.0);
        var_ubs.push_back(1.0);
        char* var_name;
        snprintf(var_name,10,"y_%c_%c",i/Nh,i%Nh);
        var_names.push_back(var_name);
        obj_fun_coefs.push_back(Costs[i]);
    }
    CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj_fun_coefs[0], &var_lbs[0], &var_ubs[0], &var_types[0], &var_names[0]);
    
    //CONSTRAINTS
    std::vector<int> idx = std::vector<int>();
    std::vector<double> coef = std::vector<double>();
    std::vector<int> matbegin = std::vector<int>();
    std::vector<double> constValues = std::vector<double>();
    std::vector<char> senses = std::vector<char>();


    int matbegin2use = 0;

    for (int k = 0; k < Nh; k++) //zero nodo partente
	{
        matbegin.push_back(matbegin2use);
        
        if (k!=startH) {
            //somma archi entranti - somma archi uscenti da k
            for(int i=0; i<Nh; i++){ //archi entranti
                idx.push_back(idX(i,k));
		        coef.push_back(1);
                matbegin2use++;
            }
            for(int j=0; j<Nh; j++){ //archi uscenti
                if (j!=0) {
                    idx.push_back(idX(k,j));
                    coef.push_back(-1);
                    matbegin2use++;

                }
            }
        }
		senses.push_back('E');
        constValues.push_back(1.0);
	}


    for (int i=0; i<Nh; i++){
        matbegin.push_back(matbegin2use);

        for (int j=0; j<Nh; j++){ //somma nodi uscenti
            idx.push_back(idY(i,j));
            coef.push_back(1);
            matbegin2use++;
        }
        senses.push_back('E');
        constValues.push_back(1);
    }

    for (int j=0; j<Nh; j++){   
        matbegin.push_back(matbegin2use);

        for (int i=0; i<Nh; i++){//somma nodi entranti
            idx.push_back(idY(i,j));
            coef.push_back(1);
            matbegin2use++;
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
                senses.push_back('E');
                constValues.push_back(0);
                matbegin2use+=2;
            }
        }
    }


	CHECKED_CPX_CALL( CPXaddrows, env, lp, Nh*Nh*2, matbegin.size(), idx.size(), &constValues[0], &senses[0], &matbegin[0], &idx[0], &coef[0], nullptr, nullptr );

}   

int main(){
    try
	{
		// init
		DECL_ENV(env);
		DECL_PROB( env, lp );
		// setup LP
		setupLP(env, lp);
		// optimize
		CHECKED_CPX_CALL( CPXmipopt, env, lp );
		// print
		double objval;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
		std::cout << "Objval: " << objval << std::endl;
		int n = CPXgetnumcols(env, lp);
		//if (n != 2*I*J+1) { throw std::runtime_error(std::string(__FILE__) + ":" + STRINGIZE(__LINE__) + ": " + "different number of variables"); }
	  	std::vector<double> varVals;
		varVals.resize(n);
  		CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
		/// status =      CPXgetx (env, lp, x          , 0, CPXgetnumcols(env, lp)-1);
  		
          
		CHECKED_CPX_CALL( CPXsolwrite, env, lp, "ironrods.sol" );
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