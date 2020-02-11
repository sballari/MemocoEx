#ifndef Model
#define Model

#include <cstdio>
#include <iostream>
#include <vector>
#include <chrono>
#include "cpxmacro.h"
#include "../DataGenerator.h"

using namespace std::chrono;
using std::cout;
using std::endl;

class TSPModel {
    public:
        static int idX(int i, int j,int Nh){
            return -(i+1)+ i*Nh+j; // rimozione cappi
            // return i*Nh+j; //! non rimuovo i cappi
        }
        static int idY(int i, int j,int Nh){
            int offset = Nh*Nh-Nh ;
            return offset + i*Nh+j; //rimozione cappi forzata
            // return Nh*Nh+i*Nh+j; //! non rimuovo i cappi
        }

        static void setupLP(CEnv env, Prob lp, Panel* panel){
            int Nh = panel->get_holesN();
            // int startH = 0;
            std::vector<char> var_types = std::vector<char>();
            std::vector<double> var_lbs = std::vector<double>();
            std::vector<double> var_ubs = std::vector<double>();
            std::vector<char*> var_names = std::vector<char*>(); 
            std::vector<double> obj_fun_coefs = std::vector<double>();       

            for(int i =0; i<Nh; i++){ //var x
                for (int j = 1; j<Nh; j++){
                    //if (i!=j){
                        var_types.push_back('C');
                        var_lbs.push_back(0.0);
                        var_ubs.push_back(CPX_INFBOUND);
                        char* var_name = new char[10];
                        snprintf(var_name,10,"x_%d_%d",i,j);
                        var_names.push_back(var_name);
                        obj_fun_coefs.push_back(0.0);
                    //}
                }
            }

            for(int i =0; i<Nh; i++){ //var y
                for (int j=0; j<Nh; j++){
                    //if (i!=j){
                        var_types.push_back('B');
                        var_lbs.push_back(0.0);
                        var_ubs.push_back(1.0);
                        char* var_name = new char[10];
                        snprintf(var_name,10,"y_%d_%d",i,j);
                        var_names.push_back(var_name);
                        obj_fun_coefs.push_back(panel->get_dist(i,j));
                    //}
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
            //costraints per non usare i cappi
            // for (int i =1; i<Nh; i++){
            //     matbegin.push_back(matbegin2use);
            //     idx.push_back(idY(i,i,Nh));
            //     coef.push_back(1);
            //     senses.push_back('E');
            //     constValues.push_back(0.0);
            //     matbegin2use++;
            // }
            for (int k = 1; k < Nh; k++) //zero nodo partente
            {
                matbegin.push_back(matbegin2use);
                //somma archi entranti - somma archi uscenti da k
                for(int i=0; i<Nh; i++){ //archi entranti
                    //if (i!=k){
                        idx.push_back(idX(i,k,Nh));
                        coef.push_back(1);
                        matbegin2use++;
                    //}
                }
                for(int j=1; j<Nh; j++){ //archi uscenti
                    //if (j!=k) {
                        idx.push_back(idX(k,j,Nh));
                        coef.push_back(-1);
                        matbegin2use++;
                    //}
                }         
                senses.push_back('E');
                constValues.push_back(1.0);
            }

            for (int i=0; i<Nh; i++){
                matbegin.push_back(matbegin2use);

                for (int j=0; j<Nh; j++){ //somma nodi uscenti
                    //if (i!=j){
                        idx.push_back(idY(i,j,Nh));
                        coef.push_back(1);
                        matbegin2use++;
                    //}
                }
                senses.push_back('E');
                constValues.push_back(1);
            }

            for (int j=0; j<Nh; j++){   
                matbegin.push_back(matbegin2use);

                for (int i=0; i<Nh; i++){//somma nodi entranti
                    //if (i!=j) {    
                        idx.push_back(idY(i,j,Nh));
                        coef.push_back(1);
                        matbegin2use++;
                    //}
                }
                senses.push_back('E');
                constValues.push_back(1);
            }

            for (int i=0; i<Nh; i++){
                for(int j=1; j<Nh; j++){
                    //if (i!=j){
                        matbegin.push_back(matbegin2use);
                        idx.push_back(idX(i,j,Nh));
                        idx.push_back(idY(i,j,Nh));
                        coef.push_back(1);
                        coef.push_back(-Nh+1);
                        senses.push_back('L');
                        constValues.push_back(0);
                        matbegin2use+=2;
                    //}
                }
            }
            CHECKED_CPX_CALL( CPXaddrows, env, lp, 0, constValues.size(), idx.size(), &constValues[0], &senses[0], &matbegin[0], &idx[0], &coef[0], nullptr, nullptr );
        }  
};
#endif