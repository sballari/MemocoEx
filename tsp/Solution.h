#ifndef Sol
#define Sol

#include <cstdlib>
#include <vector> 
#include "DataGenerator.h"


class Solution { 
    public: 
        virtual Solution* clone() const = 0;
        virtual ~Solution(){}
        virtual  void addCity(int city)=0;
        virtual void plot()=0;
        virtual double evaluate_cost()=0;
        virtual double fitness() = 0;
        virtual void substringReversal(int minAlt)=0; //non e' detto che si puo' fare
        static double avgFitness(const std::vector<Solution*>& s);
};

class PathRappr : public Solution {
    private: 
        //static FitnessOperator& fitnessOp;
        static Panel* panel;
        std::vector<int> path;
        double costValue = -1;
    public :
        
        PathRappr* clone() const override;
        PathRappr(std::vector<int> p, Panel& panel);
        PathRappr(Panel& panel);
        void addCity(int city) override;
        void plot() override;
        double evaluate_cost() override;
        double fitness() override;
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