#include <cstdio>
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <cpxmacro.h>

using namespace std;

int status;
char errmsg[BUF_SIZE];



void print(vector<double> const &input)
{
	copy(input.begin(),
			input.end(),
			ostream_iterator<int>(cout, " "));
}

int main (int argc, char const *argv[]){

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
    int CapMax = 1000;
    double Ktruck = 50;
    double Ktruck5 = 65; //costo addizzionale per il quinto truck 

    //////////////////////////////////////////////
    vector<double> objParam = vector<double>();
    for (int i=0; i<9; i++){ //costi variabili
        objParam.push_back(Costs[i/3][i%3]);
    }
    for (int i=0; i<9; i++){ //costi fissi
        objParam.push_back(50);
    }
    objParam.push_back(15); //quinto truck
    /////////////////////////////////////////////
    int ccnt = objParam.size();
    vector<double> lb = vector<double>(ccnt,0);

    vector<double> ub = vector<double>(9,0)++vector<double>(10,1);
    
    vector<char> xtype = vector<char>(9,'C') ++ vector<char>(10,"B");
    char ** xname = {"xa1,xa2,xa3,xb1,xb2,xb3,xc1,xc2,xc3,ta1,ta2,ta3,tb1,tb2,tb3,tc1,tc2,tc3,T5"};
    ////////////////////////////////////////////

    int colcount = 0;
    int rowcount = 0; 
    int nzcnt = 0;

    vector<int> rmatind = vector<int>();
    vector<int> rmatbeg = vector<int>();
    vector<char> sense = vector<char>();
    vector<double> rmatval = vector<double>();
    vector<double> rhs = vector<double>();
    for (int i=0; i<Ns; i++){ //disponibilita'

        rmatbeg.push_back(i*Na)
        rmatval.push_back(1);
        rmatind.push_back(i*Na+0);
        rmatval.push_back(1);
        rmatind.push_back(i*Na+1);
        rmatval.push_back(1);
        rmatind.push_back(i*Na+2);

        sense.push_back("L");
        rhs.push_back(Av[i]);
        
        nzcnt+=3;
        rowcount++;
    }

    for (int i=0; i<Na; i++){ //necessita'

        rmatval.push_back(1);
        rmatbeg.push_back(i)
        rmatval.push_back(1);
        rmatbeg.push_back(i+Ns)
        rmatval.push_back(1);
        rmatbeg.push_back(i+2*Ns)

        sense.push_back("G");
        rhs.push_back(N[i]);

        nzcnt+=3;
        rowcount++;
    }
    
    //attivazione tij
    int bigM = 8000;
    for(int i=0; i<Ns; i++){
        for(int j=0; j<Na; i++){
            rmatbeg.push_back(i*Ns+j);
            rmatval.push_back(1);
            rmatbeg.push_back(Ns*Na+i*Ns+j);
            rmatval.push_back(-1);

            sense.push_back("L");
            rhs.push_back(0);

            nzcnt+=2;
            rowcount++;
        }
    }
    //capacita' massima truck i,j
    for(int i=0; i<Ns; i++){
        for(int j=0; j<Na; i++){
            rmatbeg.push_back(i*Ns+j);
            rmatval.push_back(1);
            

            sense.push_back("L");
            rhs.push_back(CapMax);

            nzcnt++;
            rowcount++;
        }
    }


    //massimo 5 truck
    for(int i=0; i<Na*Ns; i++){
        rmatbeg.push_back(Ns*Na+i);
        rmatval.push_back(1);
        nzcnt++;  
    }
    sense.push_back("L");
    rhs.push_back(5);  
    rowcount++;

    //attivazione T5
    int bigT = 5;
    for(int i=0; i<Ns; i++){
        for(int j=0; j<Na; i++){
            rmatbeg.push_back(Ns*Na+i*Ns+j);
            rmatval.push_back(1);  
            nzcnt++;        
        }
    }
    rmatbeg.push_back(Ns*Na*2);
    rmatval.push_back(-1);

    sense.push_back("L");
    rhs.push_back(4);
    nzcnt++;
    rowcount++;
    
    char ** newcolnames = NULL;
    char ** rownames = NULL;
    

    try
	{
	    DECL_ENV( env );
	    DECL_PROB( env, lp );
    }
    catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
    return 0;
}