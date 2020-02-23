#ifndef Sol
#define Sol

#include <cstdlib>
#include <vector> 
#include "DataGenerator.h"


class Solution { 
    public: 
        virtual Solution* clone() const = 0;
        virtual ~Solution(){};
        virtual  void addCity(int city)=0;
        virtual void plot()=0;
        virtual double evaluate_cost()=0;
        virtual double fitness() = 0;
        virtual void substringReversal(int start, int stop)=0;
        static double avgFitness(const std::vector<Solution*>& s);
        virtual bool checkCorrectness() const = 0;
        virtual void printSol() const =0;
        virtual int getHolesN() const =0;
};

class PathRappr : public Solution {
    private: 
        //static FitnessOperator& fitnessOp;
        const Panel* panel; //puntatore che viene da fuori (NON FREEARE)
        std::vector<int> path;
        double costValue = -1;
    public :
        ~PathRappr();
        PathRappr* clone() const override;
        PathRappr(std::vector<int> p,const Panel* panel);
        PathRappr(const Panel* panel);
        void addCity(int city) override;
        void plot() override;
        double evaluate_cost() override;
        double fitness() override;
        void substringReversal(int start, int stop) override;
        bool checkCorrectness() const override;
        static std::vector<PathRappr*> orderCrossover(const PathRappr* p1, const PathRappr* p2,int start, int stop);
        void printSol() const override;
        int getHolesN() const override;
        int &operator[] (int i){return path[i];};

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