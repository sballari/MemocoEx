#ifndef Data_gen
#define Data_gen

#include <cstdlib>
#include <vector> 

class BoardPanel{
    private: 
        std::vector<double> holesX;
        std::vector<double> holesY;

        BoardPanel(std::vector<double> hx, std::vector<double> hy){
            holesX = hx;
            holesY = hy;
        }
    public:

        static const double ButtonB;
        static const double ButtonH;

        BoardPanel(){
            holesX = std::vector<double>();
            holesY = std::vector<double>();
        }

        static BoardPanel create_gridPanel(double base, double height,int maxH);
        int const get_holesN();
        double const get_euc_dist(int a, int b);
    };


#endif /* Data_gen */