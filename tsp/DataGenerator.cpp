#include "DataGenerator.h"
#include <cstdlib>
#include <math.h>
#include <vector> 
#include "matplotlib-cpp/matplotlibcpp.h"
#include <iostream>
namespace plt = matplotlibcpp;

const double Panel::ButtonB = 18; //standard hole dimension
const double Panel::ButtonH = 86;


BoardPanel BoardPanel::create_gridPanel(double base, double height,int maxH){
            //every block has probability 0.5 to be choose so the algorithm will put the
            //block in the first positizions of the grid
            //bias: preference on the initial positions
            int rowN = height/(ButtonH+10); //10 mm of margin
            int holesN = base/(ButtonB+5);
            std::vector<double> holesX = std::vector<double>();
            std::vector<double> holesY = std::vector<double>();
            
            for (int r=0; r<rowN && holesX.size()<maxH; r++){
                for (int c=0; c<holesN && holesX.size()<maxH; c++){
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

BoardPanel BoardPanel::create_gridPanel1(double base, double height,int maxH){
    //completely unbiased choose of the holes
    std::vector<double> holesX = std::vector<double>();
    std::vector<double> holesY = std::vector<double>();

    int rowN = height/(ButtonH+10); //10 mm of margin
    int holesN = base/(ButtonB+5);
    
    for (int i=0; i<maxH; i++){
        int rx = rand()%holesN*ButtonH+(ButtonH/2);
        int ry = rand()%rowN*ButtonB+(ButtonB/2);
        if (1) {
            holesX.push_back(rx);
            holesY.push_back(ry);
        } else i--;
    }
    BoardPanel gridBP = BoardPanel(holesX,holesY);
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
    return BoardPanel(hX,hY);
}

BoardPanel::BoardPanel(){
    holesX = std::vector<double>();
    holesY = std::vector<double>();
}

BoardPanel::BoardPanel(std::vector<double> hx, std::vector<double> hy){
    holesX = hx;
    holesY = hy;
}


void plotPanel(std::vector<double> hx, std::vector<double> hy,double b, double h){
    std::vector<double> zz ;//= {0,0};
    std::vector<double> zh ;//= {0,h};
    std::vector<double> zb ;//= {0,b};
    std::vector<double> hh ;//= {h,h};
    std::vector<double> bb ;//= {b,b};
    zz.push_back(0); zz.push_back(0);
    zh.push_back(0); zh.push_back(h);
    zb.push_back(0); zb.push_back(b);
    hh.push_back(h); hh.push_back(h);
    bb.push_back(b); bb.push_back(b);
    
    plt::title("panel");
    plt::plot(zz,zh, "blue");
    plt::plot(bb,zh, "blue");
    plt::plot(zb,zz, "blue");
    plt::plot(zb,hh, "blue");
    plt::scatter(hx,hy);
    plt::show();
}
void BoardPanel::plot(double b, double h){
    plotPanel(this->holesX,this->holesY,b,h);
}

void BoardPanel::plotSol(double b, double h,std::vector<double> decVar){

    std::vector<double> zz ;//= {0,0};
    std::vector<double> zh ;//= {0,h};
    std::vector<double> zb ;//= {0,b};
    std::vector<double> hh ;//= {h,h};
    std::vector<double> bb ;//= {b,b};
    zz.push_back(0); zz.push_back(0);
    zh.push_back(0); zh.push_back(h);
    zb.push_back(0); zb.push_back(b);
    hh.push_back(h); hh.push_back(h);
    bb.push_back(b); bb.push_back(b);
    
    plt::title("panel");
    plt::plot(zz,zh, "blue");
    plt::plot(bb,zh, "blue");
    plt::plot(zb,zz, "blue");
    plt::plot(zb,hh, "blue");
    plt::scatter(holesX,holesY);
    
    int Nh = get_holesN();
    for (int i=0; i<Nh; i++){
        for (int j=0; j<Nh; j++){
            if (decVar[Nh*Nh-Nh+i*Nh+j]==1){
                plt::plot({holesX[i],holesX[j]},{holesY[i],holesY[j]},"black");
            }
        }
    }
    plt::show();
}