/**
 * @file main.cpp
 * @brief 
 */

#include <iostream>
#include <iomanip>
#include "cpxmacro.h"

#include "cs1d.h"

using namespace std;

// error status and messagge buffer
int status;
char errmsg[BUF_SIZE];

int main (int argc, char const *argv[])
{
	try
	{
		//it's convenient have two enviroment
		DECL_ENV( env ); //master (its relaxation)
		DECL_ENV( env2 ); //slave problem : greedy choose 
		DECL_PROB( env, lp );
		CS1D cs1dSolver(env, lp); 
		Data data;
		data.read(argv[1]); //read data from the file
		
		cs1dSolver.initMaster(data);
		
		std::cout << std::endl << "Solving the linear relaxation LP" << std::endl << "*** It ***\tLP_Obj\tx" << std::endl;
		int it = 1;
		std::vector<double> x;
		std::vector<double> u;
		double objval;
		bool newcol = true;
		while(newcol)
		{
			// TODO... 
			//	- solve master obtaining dual information
			//	- provide the slave [price] with the dual information (the slave also adds a variable to the master, if any)
			// ...
			solveMasterLP(x, u, objval);
			newcol = price(env2, data, u);
		}
		cout << "\nx: "; for (unsigned int i = 0; i < x.size(); i++) std::cout << setw(7) << x[i] << " ";

		// The column generation above solves the LINEAR RELAXATION TO OPTIMALITY. 
		//  We can provide an INTEGER FEASIBLE (non necessarily optimal) solution by:
		//  - rounding up all variables or by 
		//  - considering the partial model (with a subset of variables) obtained at 
		//  the end of the column generation, switching to "integer" its variables
		//  and solving as a Integer Linear Programming model (cplex will apply 
		//  Branch-and-Cut). NOTE that the solution obtained after switching the
		//  variables to integer is NOT NECESSARILY INTEGER-OPTIMAL (some of the 
		//  variables that are not necessary for the optimality of the linear relaxation
		//  (hence not generated before) may be necessary for the optimality of the
		//  integer model.
		std::cout << "\nLP value: " << objval << std::endl;
		std::cout << "\n\nObtaining a HEURISTIC integer solution by branch-and-cut on the generated variables... " << std::endl;
		cs1dSolver.branchAndBoundOnThePartialModel(x, objval);
		std::cout << "x = ";
		for (unsigned int i = 0; i < x.size(); i++) std::cout << setw(5) << x[i] << " ";
		std::cout << std::endl;
		std::cout << "Value of an integer solution: " << objval << std::endl;
		
		//debug
		CHECKED_CPX_CALL( CPXwriteprob, env, lp, "final.lp", NULL );
		
		// free
		CPXfreeprob(env, &lp);
		CPXcloseCPLEX(&env);
		CPXcloseCPLEX(&env2);
	}
	catch(std::exception& e)
	{
		std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
	}
	return 0;
}
