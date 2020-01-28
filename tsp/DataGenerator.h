#ifndef Data_gen
#define Data_gen

#include <cstdlib>
#include <vector> 

class Panel { 
    public:
        virtual ~Panel(){}
        static const double ButtonB; //length of a hole
        static const double ButtonH; //heigth of a hole
        virtual double get_euc_dist(int a, int b)=0;
        virtual int get_holesN()=0;
        virtual void plot(double b, double h)=0;
        virtual void plotSol(double b, double h,std::vector<double> decVar)=0;
};
class BoardPanel : public Panel   {
    private: 
        std::vector<double> holesX;
        std::vector<double> holesY;

        BoardPanel(std::vector<double> hx, std::vector<double> hy);
    public:
        ~BoardPanel(){}
        double  get_euc_dist(int a, int b) override; 
        int  get_holesN() override;
        void plot(double b, double h) override;
        void plotSol(double b, double h,std::vector<double> decVar) override;
        BoardPanel();

        static BoardPanel create_gridPanel(double base, double height,int maxH);
        static BoardPanel create_gridPanel1(double base, double height,int maxH);
        static BoardPanel create_weirdPanel(double base, double height, int maxH);
};

#endif /* Data_gen */