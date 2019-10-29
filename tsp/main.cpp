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

void setupLP(CEnv& env, Prob& lp){

    //variables: x_i_j::Binary
    std::vector<char> var_types(Nh*Nh);
    std::vector<double> var_lbs(Nh*Nh);
    std::vector<double> var_ubs(Nh*Nh);
    std::vector<char*> var_names(Nh*Nh); 
    std::vector<double> obj_fun_coefs(Nh*Nh);

    for(int i =0; i<Nh*Nh; i++){ //var x
        var_types.push_back('B');
        var_lbs.push_back(0.0);
        var_ubs.push_back(1.0);
        char* var_name;
        snprintf(var_name,10,"x_%c_%c",i/Nh,i%Nh);
        var_names.push_back(var_name);
        obj_fun_coefs.push_back(Costs[i]);
    }
    CHECKED_CPX_CALL( CPXnewcols, env, lp, 1, &obj_fun_coefs[0], &var_lbs[0], &var_ubs[0], &var_types[0], &var_names[0]);
}   

int main(){
    
}