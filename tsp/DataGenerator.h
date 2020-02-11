#ifndef Data_gen
#define Data_gen

#include <cstdlib>
#include <string>
#include <vector> 

class Panel { 
    public:
        virtual ~Panel(){}
        static const double ButtonB; //length of a hole
        static const double ButtonH; //heigth of a hole
        virtual double get_dist(int a, int b)=0;
        virtual std::vector<double> getPoint(int label) =0;
        virtual int get_holesN()=0;
        virtual void plot(bool show)=0;
        virtual void plotSol(std::vector<double> decVar)=0;
        
};
class BoardPanel : public Panel   {
    private: 
        std::vector<double> holesX;
        std::vector<double> holesY;
        double base;
        double height;
        std::string dist_type ; //{'euc','man'}
        BoardPanel(double b, double h,std::vector<double> hx, std::vector<double> hy, std::string dist_type = "euc");
    public:
        ~BoardPanel(){}
        double  get_dist(int a, int b) override;
        int  get_holesN() override;
        void plot(bool show) override;
        void plotSol(std::vector<double> decVar) override;
        std::vector<double> getPoint(int label) override;

        BoardPanel();

        static BoardPanel create_gridPanel(double base, double height,int maxH);
        static BoardPanel create_gridPanel1(double base, double height,int maxH);
        static BoardPanel create_weirdPanel(double base, double height, int maxH);
};

#endif /* Data_gen */