#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include "cpxmacro.h"

using namespace std;


 int lp (){
    int Ns = 3;
    int Na = 3;
    double Costs[3][3] = 
    {   
        {9,6,5},
        {7,4,9},
        {4,6,3}
    };

    int Av[Ns] = {7000,6000,4000};
    int N[Na] = {8000,5000,4000};
    int CapMax = 10000;
    double Ktruck = 50;
    double Ktruck5 = 65; //costo addizzionale per il quinto truck 

    //////////////////////////////////////////////
    vector<double> objParam = vector<double>();
    for (int i=0; i<9; i++){ //costi variabili
        objParam.push_back(Costs[i/3][i%3]);
    }
    for (int i=0; i<9; i++){ //costi fissi
        objParam.push_back(Ktruck);
    }
    objParam.push_back(Ktruck5-Ktruck); //quinto truck


    /////////////////////////////////////////////
    //ccnt: number of parameters
    int ccnt = objParam.size();
    std::cout<<ccnt<<"\n";
    vector<double> lb = vector<double>(ccnt,0);

    vector<double> ub = vector<double>(9,CPX_INFBOUND) ;
    auto ub1 = vector<double>(10,1);
    ub.insert(ub.end(), ub1.begin(), ub1.end()); 
    
    vector<char> xtype = vector<char>(9,'C');
    auto xtype1 =  vector<char>(10,'B');
    xtype.insert(xtype.end(), xtype1.begin(), xtype1.end());

    vector<char*> xname_aux = {"xa1","xa2","xa3","xb1","xb2","xb3","xc1","xc2","xc3","ta1","ta2","ta3","tb1","tb2","tb3","tc1","tc2","tc3","T5"};
    char** xname = &(xname_aux[0]);
    ////////////////////////////////////////////

    int colcount = 0;
    int rowcount = 0; 
    int nzcnt = 0;

    vector<int> rmatind = vector<int>();
    vector<int> rmatbeg = vector<int>();
    vector<char> sense = vector<char>();
    vector<double> rmatval = vector<double>();
    vector<double> rhs = vector<double>();
    int last_ind_to_use = 0;

    for (int i=0; i<Ns; i++){ //disponibilita'

        rmatbeg.push_back(last_ind_to_use);
        last_ind_to_use += 3;
        rmatval.push_back(1);
        rmatind.push_back(i*Na+0);
        rmatval.push_back(1);
        rmatind.push_back(i*Na+1);
        rmatval.push_back(1);
        rmatind.push_back(i*Na+2);

        sense.push_back('L');
        rhs.push_back(Av[i]);
        
        nzcnt+=3;
        rowcount++;
    }

    for (int i=0; i<Na; i++){ //necessita'

        rmatbeg.push_back(last_ind_to_use);
        last_ind_to_use+=3;
        rmatval.push_back(1);
        rmatind.push_back(i);
        rmatval.push_back(1);
        rmatind.push_back(i+Ns);
        rmatval.push_back(1);
        rmatind.push_back(i+2*Ns);

        sense.push_back('G');
        rhs.push_back(N[i]);

        nzcnt+=3;
        rowcount++;
    }
    
    //attivazione tij
    std::cout<<"attivazioen tij...";
    double bigM = 8000;
    for(int i=0; i<Ns; i++){
        for(int j=0; j<Na; j++){
            rmatbeg.push_back(last_ind_to_use);
            last_ind_to_use+=2;
            rmatind.push_back(i*Ns+j);
            rmatval.push_back(1);
            rmatind.push_back(Ns*Na+i*Ns+j);
            rmatval.push_back(-bigM);

            sense.push_back('L');
            rhs.push_back(0);

            nzcnt+=2;
            rowcount++;
        }
    }
    //capacita' massima truck i,j
    for(int i=0; i<Ns; i++){
        for(int j=0; j<Na; j++){
            rmatbeg.push_back(last_ind_to_use);
            last_ind_to_use+=1;
            rmatind.push_back(i*Ns+j);
            rmatval.push_back(1);
            

            sense.push_back('L');
            rhs.push_back(CapMax);

            nzcnt++;
            rowcount++;
        }
    }

    //attivazione T5
    int bigT = 5;
    for(int i=0; i<Ns; i++){
        for(int j=0; j<Na; j++){
            rmatind.push_back(Ns*Na+i*Ns+j);
            rmatval.push_back(1);  
            nzcnt++;        
        }
    }
    rmatind.push_back(Ns*Na*2);
    rmatval.push_back(-1);
    rmatbeg.push_back(last_ind_to_use);
    last_ind_to_use+=(Ns*Na+1);
    sense.push_back('L');
    rhs.push_back(4);
    nzcnt++;
    rowcount++;
    
    char ** newcolnames = nullptr;
    char ** rownames = nullptr;
    
    
    try
	{
        int status;
        char errmsg[BUF_SIZE];

	    std::cout<<"i'm creating the enviroment and an empty linear problem...";
        DECL_ENV( env );
        DECL_PROB( env, lp );
        std::cout<<"problem created\n";
        

        
        //obj fun
        std::cout<<"objParam: \n";
        for(int i=0; i<objParam.size();i++){
            std::cout<<objParam[i]<<", ";
        }
        std::cout<<"\n";

        std::cout<<"objParam.size(): "<<objParam.size()<<"\n";
        std::cout<<"lb.size(): "<<lb.size()<<"\n";
        std::cout<<"ub.size(): "<<ub.size()<<"\n";
        std::cout<<"xtype.size(): "<<xtype.size()<<"\n";
        
        
        CHECKED_CPX_CALL( CPXnewcols, env, lp, 19   , &(objParam[0]), &(lb[0]), &(ub[0]), &(xtype[0]), &(xname[0]) ); //obj fun
        std::cout<<"defined the objfunction\n";
        
        //constraints
        std::cout<<"\n\n\nnumber of parameters: "<<std::to_string(ccnt)<<"\n";
        std::cout<<"number of costraints: "<<std::to_string(rowcount)<<"\n";
        std::cout<<"number of nonzero elements: "<<std::to_string(nzcnt)<<"\n";
        std::cout<<"number of costraints: "<<std::to_string(rowcount)<<"\n";
        std::cout<<"last index to use: "<<last_ind_to_use<<"\n";

        CHECKED_CPX_CALL( CPXaddrows, env, lp, ccnt, rowcount, nzcnt, &rhs[0], &sense[0], &rmatbeg[0], &rmatind[0], &rmatval[0], &newcolnames[0] , &rownames[0] );
        //type of the problemint status;
        
        CHECKED_CPX_CALL( CPXchgobjsen, env, lp, CPX_MIN );
        
        //scrivi file con il problema
        CHECKED_CPX_CALL( CPXwriteprob, env, lp, "myRods.lp", NULL );
        //risolvi
        CHECKED_CPX_CALL( CPXmipopt, env, lp );

        double objval;
		CHECKED_CPX_CALL( CPXgetobjval, env, lp, &objval );
		std::cout << "Objval: " << objval << std::endl;
		int n = CPXgetnumcols(env, lp);
		
	  	std::vector<double> varVals;
		varVals.resize(n);
  		CHECKED_CPX_CALL( CPXgetx, env, lp, &varVals[0], 0, n - 1 );
		/// status =      CPXgetx (env, lp, x          , 0, CPXgetnumcols(env, lp)-1);
  		for ( int i = 0 ; i < n ; ++i ) {
  	  	std::cout << "var in position " << i << " : " << varVals[i] << std::endl;
  	  		/// to read variables names the API function ``CPXgetcolname'' may be used (it is rather tricky, see the API manual if you like...)
  	  		/// status = CPXgetcolname (env, lp, cur_colname, cur_colnamestore, cur_storespace, &surplus, 0, cur_numcols-1);
  		}
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

int main(){
    lp();
    return 0;
}