#ifndef Sol
#define Sol

#include <cstdlib>
#include <vector> 
#include "DataGenerator.h"

class Solution { 
    public: 
        virtual ~Solution(){}
        virtual  void addCity(int city)=0;
        virtual void plot(Panel& panel)=0;
};

class PathRappr : public Solution {
    public :
        std::vector<int> path;

        PathRappr(std::vector<int> p);
        PathRappr();
        void addCity(int city);
        void plot(Panel& panel) override;

};

#endif /* Solution */