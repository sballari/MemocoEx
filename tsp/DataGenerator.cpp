#include "DataGenerator.h"
#include <cstdlib>
#include <math.h>
#include <vector> 

const double BoardPanel::ButtonB = 18;
const double BoardPanel::ButtonH = 86;


BoardPanel BoardPanel::create_gridPanel(double base, double height,int maxH){

            int rowN = height/(ButtonH+10); //10 mm of margin
            int holesN = base/(ButtonB+5);
            std::vector<double> holesX = std::vector<double>();
            std::vector<double> holesY = std::vector<double>();
            
            for (int r=0; r<rowN && holesX.size()<=maxH; r++){
                for (int c=0; c<holesN && holesX.size()<=maxH; c++){
                    if(rand()%1<0.5){
                        double y = r*ButtonH+(ButtonH/2);
                        double x = c*ButtonB+(ButtonB/2);
                        holesX.push_back(x);
                        holesY.push_back(y);
                    }
                }
            }
            BoardPanel gridBP = BoardPanel(holesX,holesY);
            return gridBP;

}

double const BoardPanel::get_euc_dist(int holeA, int holeB){
    if (holeA>=get_holesN() || holeB>=get_holesN()) {
        throw "Exception: illegal hole index";
    } else {
        double dist = pow((holesX[holeA]-holesX[holeB]),2) + pow((holesY[holeA]-holesY[holeB]),2);
        return sqrt(dist);
    }
}
int const BoardPanel::get_holesN(){
    return this->holesX.size();
}
