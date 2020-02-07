#include "DataGenerator.h"
#include <cstdlib>
#include <math.h>
#include <vector> 
#include "matplotlib-cpp/matplotlibcpp.h"
#include <iostream>

namespace plt = matplotlibcpp;

const double Panel::ButtonB = 1.8; //standard hole dimension
const double Panel::ButtonH = 8.6;


BoardPanel BoardPanel::create_gridPanel(double base, double height,int maxH){
            //every block has probability 0.5 to be choose so the algorithm will put the
            //block in the first positizions of the grid
            //bias: preference on the initial positions
            int rowN = height/(ButtonH+0.10); //10 mm of margin
            int holesN = base/(ButtonB+0.5);
            std::vector<double> holesX = std::vector<double>();
            std::vector<double> holesY = std::vector<double>();
            
            for (int r=0; r<rowN && holesX.size()<maxH; r++){
                for (int c=0; c<holesN && holesX.size()<maxH; c++){
                    if(rand()%1<0.2){
                        double y = r*ButtonH+(ButtonH/2);
                        double x = c*ButtonB+(ButtonB/2);
                        holesX.push_back(x);
                        holesY.push_back(y);
                    }
                }
            }
            BoardPanel gridBP = BoardPanel(base,height,holesX,holesY);
            return gridBP;

}

BoardPanel BoardPanel::create_gridPanel1(double base, double height,int maxH){
    //completely unbiased choose of the holes
    std::vector<double> holesX = std::vector<double>();
    std::vector<double> holesY = std::vector<double>();

    int rowN = height/(ButtonH+0.10); //10 mm of margin
    int holesN = base/(ButtonB+0.5);
    
    for (int i=0; i<maxH; i++){
        int rx = rand()%holesN+(ButtonH/2);
        int ry = rand()%rowN*ButtonB+(ButtonB/2);
        if (rx>2 && rx<base-2 && ry>2 && ry<height-2) { //bordi del pannello
            holesX.push_back(rx);
            holesY.push_back(ry);
        } else i--;
    }
    BoardPanel gridBP = BoardPanel(base,height,holesX,holesY);
    return gridBP;
}

double euc_dist(double xa,double ya, double xb, double yb){
    double dist = pow((xa-xb),2) + pow((ya-yb),2);
    return sqrt(dist);
}
double BoardPanel::get_euc_dist(int holeA, int holeB){
    if (holeA>=get_holesN() || holeB>=get_holesN()) {
        throw "Exception: illegal hole index";
    } else {
        double dist = euc_dist(holesX[holeA],holesY[holeA],holesX[holeB],holesY[holeB]);
        return dist;
    }
}
int BoardPanel::get_holesN(){
    return this->holesX.size();
}


BoardPanel BoardPanel::create_weirdPanel(double base, double height,int maxH){
    auto hX = std::vector<double>();
    auto hY = std::vector<double>();
    for (int i =0; i<maxH; i++){
        // (rx,ry) center of the button
        double rx = (rand()/(double)RAND_MAX)*base;
        double ry = (rand()/(double)RAND_MAX)*height;
        if (rx+(ButtonB/2)<base && ry+(ButtonH/2)<height){ //feasible button
            hX.push_back(rx);
            hY.push_back(ry);
        } else {i--;} //unfeasible button -> retry
        
    }
    return BoardPanel(base,height,hX,hY);
}

BoardPanel::BoardPanel(){
    this->base = 0;
    this->height = 0;
    holesX = std::vector<double>();
    holesY = std::vector<double>();
}

BoardPanel::BoardPanel(double b, double h,std::vector<double> hx, std::vector<double> hy){
    this->base = b;
    this->height = h;
    holesX = hx;
    holesY = hy;
}


void BoardPanel::plot(bool show = true){
    
    plt::title("panel");
    plt::plot({0,0},{0,height}, "black");
    plt::plot({0,base},{height,height}, "black");
    plt::plot({base,base},{height,0}, "black");
    plt::plot({0,base},{0,0}, "black");
    plt::scatter(holesX,holesY);
    if (show) plt::show();
}
void BoardPanel::plotSol(std::vector<double> decVar){

    plot(false);
    
    int Nh = get_holesN();
    for (int i=0; i<Nh; i++){
        for (int j=0; j<Nh; j++){
            if (decVar[Nh*Nh-Nh+i*Nh+j]>0.9 && decVar[Nh*Nh-Nh+i*Nh+j]<1.1 ){
                plt::plot({holesX[i],holesX[j]},{holesY[i],holesY[j]},"black");
            }
        }
    }
    plt::show();
}

std::vector<double> BoardPanel::getPoint(int label) {
    if (label<0 || label>=holesX.size()) {
        throw "Exception: illegal hole index";
    }
    return {holesX[label],holesY[label]};
    
}