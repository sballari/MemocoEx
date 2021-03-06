#include "DataGenerator.h"
#include <cstdlib>
#include <math.h>
#include <vector> 
// #include "matplotlib-cpp/matplotlibcpp.h"
#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <utility>
#include <stdexcept>
using std::cout;
using std::endl;
using std::string;

// namespace plt = matplotlibcpp;

const double Panel::ButtonB = 1.8; //standard hole dimension
const double Panel::ButtonH = 8.6;


BoardPanel* BoardPanel::create_gridPanel(double base, double height,uint maxH,double p){
            //every block has probability p to be choose so the algorithm will put the
            //block in the first positizions of the grid
            //bias: preference on the initial positions
            std::random_device rd;  
            std::mt19937 gen(rd()); 
            std::uniform_real_distribution<double> dis(0, 1);
 
            uint rowN = height/(ButtonH+0.10); //10 mm of margin
            uint holesN = base/(ButtonB+0.5);
            if (holesN*rowN < maxH) throw std::string("spazio insufficente nel pannello");
            std::vector<double> holesX = std::vector<double>();
            std::vector<double> holesY = std::vector<double>();
            
            for (uint r=0; r<rowN && holesX.size()<maxH; r++){
                for (uint c=0; c<holesN && holesX.size()<maxH; c++){
                    if(dis(gen)<p){
                        double y = r*ButtonH+(ButtonH/2);
                        double x = c*ButtonB+(ButtonB/2);
                        holesX.push_back(x);
                        holesY.push_back(y);
                    }
                }
            }
            BoardPanel* gridBP = new BoardPanel(base,height,holesX,holesY);
            return gridBP;

}

BoardPanel* BoardPanel::create_gridPanel1(double base, double height,uint maxH){
    //completely unbiased choose of the holes
    std::vector<double> holesX = std::vector<double>();
    std::vector<double> holesY = std::vector<double>();

    std::random_device rd;  
    std::mt19937 gen(rd()); 
    

    uint rowN = height/(ButtonH+0.10); //10 mm of margin
    uint holesN = base/(ButtonB+0.5);

    std::uniform_int_distribution<> disX(0, holesN);
    std::uniform_int_distribution<> disY(0, rowN);

    if (holesN*rowN < maxH) throw std::string("spazio insufficente nel pannello");
    for (uint i=0; i<maxH; i++){
        double rx = disX(gen)*ButtonB+(ButtonB/2);
        double ry = disY(gen)*ButtonH+(ButtonH/2);
        
        if (rx>2 && rx<base-2 && ry>2 && ry<height-2) { //bordi del pannello
            holesX.push_back(rx);
            holesY.push_back(ry);
        } else i--;
    }
    BoardPanel* gridBP = new BoardPanel(base,height,holesX,holesY);
    return gridBP;
}

double euc_dist(double xa,double ya, double xb, double yb){
    double dist = pow((xa-xb),2) + pow((ya-yb),2);
    return sqrt(dist);
}
double man_dist(double xa,double ya, double xb, double yb){
    double dist = abs(xa-xb) + abs(ya-yb);
    return dist;
}

double BoardPanel::get_dist(int holeA, int holeB) const{
    double dist = -1;
    if (holeA>=get_holesN() || holeB>=get_holesN()) {
        throw std::runtime_error("Exception: illegal hole index, "+std::to_string(holeA)+", "+std::to_string(holeB));
    } else {
        if (holeA == holeB) return 0;
        if (dist_type=="euc")
            dist = euc_dist(holesX[holeA],holesY[holeA],holesX[holeB],holesY[holeB]);
        if (dist_type=="man")
            dist = man_dist(holesX[holeA],holesY[holeA],holesX[holeB],holesY[holeB]);
    }
    if (dist < 0) throw std::string("Exception: illegal dist type "+dist_type);
    return dist;
}

int BoardPanel::get_holesN()const {
    return this->holesX.size();
}


BoardPanel* BoardPanel::create_weirdPanel(double base, double height,uint maxH){
    auto hX = std::vector<double>();
    auto hY = std::vector<double>();

    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_real_distribution<double> dis(0, 1);
    
    for (uint i =0; i<maxH; i++){
        // (rx,ry) center of the button
        double rx = (dis(gen))*base;
        double ry = (dis(gen))*height;
        if (rx+(ButtonB/2)<base && ry+(ButtonH/2)<height){ //feasible button
            hX.push_back(rx);
            hY.push_back(ry);
        } else {i--;} //unfeasible button -> retry
        
    }
    return new BoardPanel(base,height,hX,hY);
}

BoardPanel::BoardPanel(){
    this->base = 0;
    this->height = 0;
    holesX = std::vector<double>();
    holesY = std::vector<double>();
    std::string dist_type = "euc";
}

BoardPanel::BoardPanel(double b, double h,std::vector<double> hx, std::vector<double> hy, std::string dist_type){
    this->base = b;
    this->height = h;
    holesX = hx;
    holesY = hy;
    this->dist_type = dist_type;
}


// void BoardPanel::plot(bool show = true) const{
    
//     plt::title("panel");
//     plt::plot({0,0},{0,height}, "black");
//     plt::plot({0,base},{height,height}, "black");
//     plt::plot({base,base},{height,0}, "black");
//     plt::plot({0,base},{0,0}, "black");
//     plt::scatter(holesX,holesY);
//     if (show) plt::show();
// }
// void BoardPanel::plotSol(std::vector<double> decVar,int* Ys) const{

//     plot(false);
    
//     int Nh = get_holesN();
//     for (int i=0; i<Nh; i++){
//         for (int j=0; j<Nh; j++){
//             if (i==j) continue;
//             if (decVar[Ys[i*Nh+j]]>0.9 && decVar[Ys[i*Nh+j]]<1.1 ){
//                 plt::plot({holesX[i],holesX[j]},{holesY[i],holesY[j]},"black");
//             }
//         }
//     }
//     plt::show();
// }

std::vector<double> BoardPanel::getPoint(uint label) const {
    if (label<0 || label>=holesX.size()) {
        throw string("Exception:  index");
    }
    return {holesX[label],holesY[label]};
    
}

void BoardPanel::write(std::string file_name) const {
    //DSimpone eccher . dat codifica
    //numero_node\n base\n heigth \n x1 y1 x2 y2 ...
    std::ofstream output;
    output.open(file_name, std::ofstream::out);
    if (!output) {
        throw std::string("file .dat non aprile");
    } else {
        int Nh = this->get_holesN();
        output<<Nh<<std::endl;
        output<<base<<std::endl;
        output<<height<<std::endl;
        for (int i =0; i<Nh ; i++){
            std::vector<double> xy = this->getPoint(i);
            output<<xy[0]<<" "<<xy[1]<<" ";
        }
        output<<std::endl;
        output.close();
    }
}

BoardPanel BoardPanel::read(std::string file_name) {
    //DSimpone eccher . dat codifica
    //numero_node \n x1 y1 x2 y2 ...
    std::ifstream input;
    input.open(file_name, std::ifstream::in);
    if (!input) {
        throw std::string("file .dat non apribile");
    } else {
        std::vector<double> holesX = std::vector<double>();
        std::vector<double> holesY = std::vector<double>();
        double Nh;
        double base;
        double height;
        input>>Nh;
        input>>base;
        input>>height;
        for (int i =0; i<Nh ; i++){
            double x;
            double y;
            input>>x;
            input>>y;
            holesX.push_back(x);
            holesY.push_back(y);
        }
        return BoardPanel(base,height,holesX,holesY);

    }
}

BoardPanel* BoardPanel::clone() const {
    return new BoardPanel(*this);
}