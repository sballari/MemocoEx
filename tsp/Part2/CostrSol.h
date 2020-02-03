#ifndef CostrSolver
#define CostrSolver

#include <cstdlib>
#include <vector> 
#include "../Solution.h"
#include "../DataGenerator.h"


class RandomInsertion {
    public:
        static PathRappr get_sol(Panel& panel);
};

#endif