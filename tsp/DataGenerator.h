#ifndef Data_gen
#define Data_gen

#include <cstdlib>
#include <string>
#include <vector> 

class Panel { 
    public:
        virtual ~Panel(){};
        static const double ButtonB; //length of a hole
        static const double ButtonH; //heigth of a hole
        virtual double get_dist(int a, int b)const =0;
        virtual std::vector<double> getPoint(uint label)const =0;
        virtual int get_holesN()const =0;
        // virtual void plot(bool show)const =0;
        // virtual void plotSol(std::vector<double> decVar, int* Ys)const =0;
        virtual void write(std::string file_name)const =0;
        virtual Panel* clone() const =0;
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
        ~BoardPanel(){};
        double  get_dist(int a, int b) const override;
        int  get_holesN()const override;
        // void plot(bool show)const override;
        // void plotSol(std::vector<double> decVar,int* Ys)const override;
        std::vector<double> getPoint(uint label)const override;
        void write(std::string file_name)const override;
        
        static BoardPanel read(std::string file_name);
        BoardPanel* clone() const override;

        BoardPanel();

        static BoardPanel* create_gridPanel(double base, double height,uint maxH, double prob = 0.2);
        static BoardPanel* create_gridPanel1(double base, double height,uint maxH);
        static BoardPanel* create_weirdPanel(double base, double height, uint maxH);
};

#endif /* Data_gen */