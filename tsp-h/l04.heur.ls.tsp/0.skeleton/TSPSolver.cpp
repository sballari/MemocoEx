/**
 * @file TSPSolver.cpp
 * @brief TSP solver (neighborhood search)
 *
 */

#include "TSPSolver.h"
#include <iostream>

bool TSPSolver::solve ( const TSP& tsp , const TSPSolution& initSol , TSPSolution& bestSol )
{
  try
  {
    bool stop = false;
    int  iter = 0;

    TSPSolution currSol(initSol);
    double bestValue, currValue;
    bestValue = currValue = evaluate(currSol,tsp);
    TSPMove move;
    while ( ! stop ) {
      if ( tsp.n < 20 ) currSol.print(); //log current solution (only small instances)
      std::cout << " (" << ++iter << ") value " << currValue << " (" << evaluate(currSol,tsp) << ")";
      double bestNeighValue = currValue + findBestNeighbor(tsp,currSol,move);
      ///incremental evaluation: findBestNeighbour returns the cost increment
      std::cout << " move: " << move.from << " , " << move.to << std::endl;
      if ( bestNeighValue < currValue ) {
        bestValue = currValue = bestNeighValue;
        currSol = swap(currSol,move);
        stop = false;
      } else {
        stop = true;
      }
    }
    bestSol = currSol;
  }
  catch(std::exception& e)
  {
    std::cout << ">>>EXCEPTION: " << e.what() << std::endl;
    return false;
  }
  return true;
}

TSPSolution& TSPSolver::swap ( TSPSolution& tspSol , const TSPMove& move ) 
{
  TSPSolution tmpSol(tspSol);
  for ( int i = move.from ; i <= move.to ; ++i ) {
    tspSol.sequence[i] = tmpSol.sequence[move.to-(i-move.from)];
  }
  return tspSol;
}


double TSPSolver::findBestNeighbor ( const TSP& tsp , const TSPSolution& currSol , TSPMove& move )
/* Determine the *move* yielding the best 2-opt neigbor solution
 */
{
  double bestCostVariation = tsp.infinite;
	/// incremental evaluation --> bestCostVariation (instead of best cost)
	
  /// TODO FROM HERE... we just generate the cost of the solutions  not the sol (it's not necessary)
  move.from = move.to = 0;
	int curr_var = 0;
	for (int i=1; i<currSol.sequence.size()-2;i++){
		int h = currSol[i-1];
		int i = currSol[i];
		for (int j=i+1; j<currSol.sequence.size()-1; j++){
			int c_var = -tsp.cost[h][i]-tsp.cost[j][l]+tsp.cost[h][j]+tsp.cost[i][l];
			if (c_var>curr_var) {
				curr_val = c_var;
				move.from = i;
				move.to = j;
			}
		}
	}
	bestCostVariation = curr_var;
  /// ...TO HERE
  return bestCostVariation;
}








