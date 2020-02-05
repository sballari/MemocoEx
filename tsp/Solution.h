#ifndef Sol
#define Sol

#include <cstdlib>
#include <vector> 
#include "DataGenerator.h"


class Solution { 
    public: 
        virtual ~Solution(){}
        virtual  void addCity(int city)=0;
        virtual void plot()=0;
        virtual double evaluate_cost()=0;
        //virtual void setFitnessOp(FitnessOperator& fitOp)=0;
        virtual double fitness() = 0;
        //virtual Solution& combine(Solution& parent2)=0;
        virtual void substringReversal(int minAlt)=0; //non e' detto che si puo' fare
};

class PathRappr : public Solution {
    private: 
        //static FitnessOperator& fitnessOp;
        static Panel* panel;
    public :
        std::vector<int> path;

        PathRappr(std::vector<int> p, Panel& panel);
        PathRappr(Panel& panel);
        void addCity(int city) override;
        void plot() override;
        double evaluate_cost() override;
        //void setFitnessOp(FitnessOperator& fitOp) override;
        double fitness() override;
        //PathRappr& combine(PathRappr& parent2) override;
        void substringReversal(int minAlt) override;

};

// class FitnessOperator {
//     public:
//     virtual double fitness(Solution& solution)=0;
// };

// class CostFitness: public FitnessOperator {
//     private:
//     Panel& panel;
//     public:
//     CostFitness(Panel& p);
//     double fitness(Solution& solution) override;
// };

#endif /* Solution */