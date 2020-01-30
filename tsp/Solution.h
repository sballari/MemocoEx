#ifndef Sol
#define Sol

#include <cstdlib>
#include <vector> 

class Solution { 
    public: 
        virtual ~Solution(){}
        virtual  void addCity(int city)=0;
};

class PathRappr : public Solution {
    public :
        std::vector<int> path;

        PathRappr(std::vector<int> p);
        PathRappr();
        void addCity(int city);

};

#endif /* Solution */